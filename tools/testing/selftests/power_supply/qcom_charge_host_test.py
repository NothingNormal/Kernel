#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-only
"""Compile real Qualcomm charging logic against host mocks, not phone hardware."""

import argparse
import os
from pathlib import Path
import re
import shlex
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
DRIVERS = ROOT / "drivers/power/supply/qcom"
DTS = ROOT / "arch/arm64/boot/dts/qcom"
MOCK = Path(__file__).with_name("qcom_charge_mock.c")


def section(text, start, end):
    first = text.index(start)
    return text[first:text.index(end, first)]


def struct_definition(text, name):
    match = re.search(r"struct\s+" + re.escape(name) + r"\s*\{[^}]*\};", text, re.S)
    if not match:
        raise ValueError("struct definition not found: " + name)
    return match.group(0)


def define(text, name):
    match = re.search(r"^#define\s+" + re.escape(name) + r"\b[^\n]*", text, re.M)
    if not match:
        raise ValueError("define not found: " + name)
    return match.group(0)


def profile_array(text, property_name, output_name):
    pattern = re.escape(property_name) + r"\s*=\s*<([^>]+)>\s*;"
    match = re.search(pattern, text, re.S)
    if not match:
        raise ValueError("battery profile property not found: " + property_name)
    values = [int(value.strip("()"), 0) for value in match.group(1).split()]
    if not values or len(values) % 3 or len(values) > 15:
        raise ValueError("unexpected profile data: " + property_name)
    rows = ["\t{%d, %d, %d}," % tuple(values[i:i + 3])
            for i in range(0, len(values), 3)]
    return ("static const struct range_data " + output_name +
            "[MAX_STEP_CHG_ENTRIES] = {\n" + "\n".join(rows) + "\n};\n")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--step-source", type=Path,
                        default=DRIVERS / "step-chg-jeita.c")
    parser.add_argument("--smb-source", type=Path,
                        default=DRIVERS / "smb5-lib.c")
    parser.add_argument("--case", default="all",
                        choices=("all", "ranges", "step", "dynamic", "jeita", "sixpin"))
    args = parser.parse_args()
    step = args.step_source.read_text()
    smb = args.smb_source.read_text()
    step_header = (DRIVERS / "step-chg-jeita.h").read_text()
    smb_header = (DRIVERS / "smb5-lib.h").read_text()
    profile = (DTS / "qg-batterydata-atl_5020mAh.dtsi").read_text()
    j6b_profile = (DTS / "batterydata-J6B-nvt-5020mah.dtsi").read_text()

    types = "\n".join(
        [define(step_header, "MAX_STEP_CHG_ENTRIES"),
         define(smb_header, "MAX_STEP_ENTRIES"),
         define(smb_header, "TAPER_IBAT_TRH_HYS_UA"),
         struct_definition(step_header, "step_chg_jeita_param"),
         struct_definition(step_header, "range_data"),
         struct_definition(smb_header, "six_pin_step_data")]
        + [struct_definition(step, name) for name in (
            "step_chg_cfg", "jeita_fcc_cfg", "jeita_fv_cfg",
            "dynamic_fv_cfg", "step_chg_info")]
    )
    constants = "\n".join(
        [define(step_header, name) for name in (
            "BATT_CP_COOL_THRESHOLD", "BATT_CP_WARM_THRESHOLD",
            "BATT_COOL_THRESHOLD", "BATT_WARM_THRESHOLD",
            "HVDCP3_CLASS_B_27W")]
        + [define(step, name) for name in (
            "STEP_CHG_HYSTERISIS_DELAY_US", "JEITA_SUSPEND_HYST_UV",
            "JEITA_SIX_PIN_BATT_HYST_UV", "WARM_VFLOAT_UV")]
    )
    functions = "\n".join([
        constants,
        section(step, "#define is_between(", "\n\nstruct step_chg_cfg"),
        section(step, "static bool is_usb_available(",
                "\nstatic bool is_input_present("),
        section(step, "int read_range_data_from_node(",
                "\nEXPORT_SYMBOL(read_range_data_from_node);"),
        section(step, "static int get_val(",
                "\n#define TAPERED_STEP_CHG_FCC_REDUCTION_STEP_MA"),
        section(step, "#define TAPERED_STEP_CHG_FCC_REDUCTION_STEP_MA",
                "\nstatic int handle_step_chg_config("),
        section(step, "static int handle_step_chg_config(",
                "\n#define JEITA_SUSPEND_HYST_UV"),
        section(step, "static int handle_jeita(",
                "\nstatic int handle_battery_insertion("),
    ])
    worker_start = smb.index("static void smblib_six_pin_batt_step_chg_work(")
    worker = smb[worker_start:]
    sixpin_branch = section(worker, "\tif (drop_fcc_enable && !chg->health_not_good",
                            "\n\tif ((main_charge_type == POWER_SUPPLY_CHARGE_TYPE_TAPER)")
    profiles = "\n".join([
        profile_array(profile, "qcom,step-chg-ranges", "profile_atl_step"),
        profile_array(profile, "qcom,jeita-fcc-ranges", "profile_atl_jeita_fcc"),
        profile_array(profile, "qcom,jeita-fv-ranges", "profile_atl_jeita_fv"),
        profile_array(profile, "qcom,dynamic-fv-ranges", "profile_atl_dynamic_fv"),
        profile_array(j6b_profile, "qcom,step-chg-ranges", "profile_j6b_step"),
        profile_array(j6b_profile, "qcom,jeita-fcc-ranges", "profile_j6b_jeita_fcc"),
        profile_array(j6b_profile, "qcom,jeita-fv-ranges", "profile_j6b_jeita_fv"),
    ])

    with tempfile.TemporaryDirectory(prefix="qcom-charge-test-") as tmp:
        temp = Path(tmp)
        (temp / "qcom_charge_types.h").write_text(types + "\n")
        (temp / "qcom_charge_extracted.h").write_text(functions + "\n")
        (temp / "qcom_sixpin_branch.h").write_text(sixpin_branch + "\n")
        (temp / "qcom_charge_profiles.h").write_text(profiles + "\n")
        exe = temp / "qcom_charge_mock"
        cc = shlex.split(os.environ.get("HOSTCC", "cc"))
        flags = ["-std=gnu11", "-O1", "-g", "-fno-omit-frame-pointer",
                 "-fsanitize=address,undefined", "-fno-sanitize-recover=all"]
        subprocess.run(cc + flags + ["-I", str(temp), str(MOCK), "-o", str(exe)],
                       check=True)
        environment = os.environ.copy()
        environment.setdefault("ASAN_OPTIONS", "detect_leaks=0")
        subprocess.run([str(exe), args.case], check=True, env=environment)


if __name__ == "__main__":
    main()
