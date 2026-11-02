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
MOCKS = [Path(__file__).with_name("mock.c"), Path(__file__).with_name("lifetime.c"), Path(__file__).with_name("files.c")]
source = DRIVER.read_text()

def extract(name):
    pattern = r"(?m)^static [^\n]*\b" + re.escape(name) + r"\s*\([^;]*?\)\s*\{"
    match = re.search(pattern, source)
    if match is None:
        raise RuntimeError("cannot find " + name)
    start, depth = source.index("{", match.start()), 0
    for end in range(start, len(source)):
        if source[end] == "{":
            depth += 1
        elif source[end] == "}":
            depth -= 1
            if depth == 0:
                return source[match.start():end + 1]
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
    for template in MOCKS:
        mock = re.sub(r"/\* EXTRACT ([a-zA-Z0-9_]+) \*/",
                      lambda m: extract(m.group(1)), template.read_text())
        mock = mock.replace("/* EXTRACT_RTP_TABLE */", table.group(0))
        cfile, exe = Path(temp) / template.name, Path(temp) / template.stem
        cfile.write_text(mock)
        subprocess.run([os.environ.get("CC", "cc"), *flags, str(cfile), "-o", str(exe)], check=True)
        for i in range(repeats):
            subprocess.run([str(exe)], check=True,
                           stdout=None if i == repeats - 1 else subprocess.DEVNULL)
    if repeats > 1:
        print(f"All three tests passed {repeats} times each")
