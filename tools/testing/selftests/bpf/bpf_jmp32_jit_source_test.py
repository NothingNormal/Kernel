#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Guard the ARM64 JIT's JMP32 width selection and opcode coverage."""
import re
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
source = (ROOT / "arch/arm64/net/bpf_jit_comp.c").read_text()
start = source.index("static int build_insn(")
end = source.index("\nstatic int", start + 1)
body = source[start:end]
assert re.search(r"BPF_CLASS\(code\)\s*==\s*BPF_ALU64\s*\|\|\s*"
                 r"BPF_CLASS\(code\)\s*==\s*BPF_JMP", body), \
    "64-bit width must be selected for ALU64/JMP, while JMP32 uses W registers"

x_start = body.index("/* IF (dst COND src) JUMP off */")
k_start = body.index("/* IF (dst COND imm) JUMP off */")
call_start = body.index("/* function call */")
reg_block = body[x_start:k_start]
imm_block = body[k_start:call_start]
ops = ("JEQ", "JGT", "JLT", "JGE", "JLE", "JNE",
       "JSGT", "JSLT", "JSGE", "JSLE")
for op in ops:
    assert f"case BPF_JMP32 | BPF_{op} | BPF_X:" in reg_block, f"missing JMP32 {op}/X"
    assert f"case BPF_JMP32 | BPF_{op} | BPF_K:" in imm_block, f"missing JMP32 {op}/K"
assert "case BPF_JMP32 | BPF_JSET | BPF_X:" in reg_block
assert "case BPF_JMP32 | BPF_JSET | BPF_K:" in imm_block
assert "emit(A64_CMP(is64, dst, src), ctx);" in reg_block
assert "emit(A64_TST(is64, dst, src), ctx);" in reg_block
assert "emit_a64_mov_i(is64, tmp, imm, ctx);" in imm_block
assert "emit(A64_CMP(is64, dst, tmp), ctx);" in imm_block
assert "emit(A64_TST(is64, dst, tmp), ctx);" in imm_block
print("PASS: ARM64 JIT covers all 22 JMP32 conditions with 32-bit compare/test width")
