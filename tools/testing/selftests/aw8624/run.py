#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Compile selected actual AW8624 driver functions against userspace mocks."""
from pathlib import Path
import os
import re
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]
DRIVER = ROOT / "drivers/misc/aw8624_haptic/aw8624.c"
DRV_DRIVER = ROOT / "drivers/misc/drv2624_haptic/drv2624.c"
source = DRIVER.read_text()
drv_source = DRV_DRIVER.read_text()
MOCKS = [(Path(__file__).with_name("mock.c"), source),
         (Path(__file__).with_name("lifetime.c"), source),
         (Path(__file__).with_name("files.c"), source),
         (Path(__file__).with_name("drv2624.c"), drv_source)]
header = (ROOT / "drivers/misc/aw8624_haptic/aw8624.h").read_text()
default_duration = re.search(r"(?m)^#define AW8624_DEFAULT_PLAYBACK_MS\s+\d+$", header)
if not default_duration:
    raise RuntimeError("cannot find fallback duration")

def extract(driver_source, name):
    pattern = r"(?m)^static [^\n]*\b" + re.escape(name) + r"\s*\([^;]*?\)\s*\{"
    match = re.search(pattern, driver_source)
    if match is None:
        raise RuntimeError("cannot find " + name)
    start, depth = driver_source.index("{", match.start()), 0
    for end in range(start, len(driver_source)):
        if driver_source[end] == "{":
            depth += 1
        elif driver_source[end] == "}":
            depth -= 1
            if depth == 0:
                return driver_source[match.start():end + 1]
    raise RuntimeError("unterminated " + name)

table = re.search(r"static char aw8624_rtp_name\[\]\[AW8624_RTP_NAME_MAX\] = \{.*?\n\};", source, re.S)
if not table:
    raise RuntimeError("cannot find RTP filename table")
with tempfile.TemporaryDirectory(prefix="aw8624-test-") as temp:
    flags = ["-std=gnu11", "-O1", "-g", "-pthread", "-Wall", "-Wextra",
             "-Wno-unused-parameter", "-Wno-sign-compare", "-Wno-unused-function",
             "-fsanitize=address,undefined", "-fno-omit-frame-pointer", "-no-pie"]
    repeats = int(os.environ.get("AW8624_TEST_REPEATS", "1"))
    if repeats < 1:
        raise ValueError("AW8624_TEST_REPEATS must be at least 1")
    for template, driver_source in MOCKS:
        mock = re.sub(r"/\* EXTRACT ([a-zA-Z0-9_]+) \*/",
                      lambda m: extract(driver_source, m.group(1)), template.read_text())
        mock = mock.replace("/* EXTRACT_RTP_TABLE */", table.group(0))
        mock = mock.replace("/* EXTRACT_PLAYBACK_DEFAULT */", default_duration.group(0))
        cfile, exe = Path(temp) / template.name, Path(temp) / template.stem
        cfile.write_text(mock)
        subprocess.run([os.environ.get("CC", "cc"), *flags, str(cfile), "-o", str(exe)], check=True)
        for i in range(repeats):
            subprocess.run([str(exe)], check=True,
                           stdout=None if i == repeats - 1 else subprocess.DEVNULL)
    if repeats > 1:
        print(f"All four tests passed {repeats} times each")
