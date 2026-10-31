#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0
"""Source-extracted ASan/UBSan regression for BPF stack build-ID parsing."""
import os
import shlex
import subprocess
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[4]
SOURCE = ROOT / "lib/buildid.c"
source = SOURCE.read_text()
parse_start = source.index("static inline int parse_build_id(")
parse_end = source.index("\n/* Parse build ID from 32-bit ELF */", parse_start)
get32_start = parse_end + 1
get32_end = source.index("\n/* Parse build ID from 64-bit ELF */", get32_start)
get64_start = get32_end + 1
get64_end = source.index("\n/*\n * Parse build ID of ELF file mapped to vma", get64_start)

prefix = r'''
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PAGE_SIZE 4096
#define BUILD_ID 3
#define PT_NOTE 4
#define BUILD_ID_SIZE_MAX 20
#define READ_ONCE(x) (x)
#define ALIGN(x, a) (((x) + ((a) - 1)) & ~((a) - 1))
typedef uint32_t __u32;
typedef uint32_t Elf32_Word;
typedef uint64_t Elf64_Off;
typedef uint64_t Elf64_Xword;
typedef uint32_t u32;
typedef uint64_t u64;
typedef struct { Elf32_Word n_namesz, n_descsz, n_type; } Elf32_Nhdr;
typedef struct {
    unsigned char e_ident[16]; uint16_t e_type, e_machine; uint32_t e_version;
    uint32_t e_entry, e_phoff, e_shoff, e_flags;
    uint16_t e_ehsize, e_phentsize, e_phnum, e_shentsize, e_shnum, e_shstrndx;
} Elf32_Ehdr;
typedef struct {
    uint32_t p_type, p_offset, p_vaddr, p_paddr, p_filesz, p_memsz, p_flags, p_align;
} Elf32_Phdr;
typedef struct {
    unsigned char e_ident[16]; uint16_t e_type, e_machine; uint32_t e_version;
    uint64_t e_entry, e_phoff, e_shoff; uint32_t e_flags;
    uint16_t e_ehsize, e_phentsize, e_phnum, e_shentsize, e_shnum, e_shstrndx;
} Elf64_Ehdr;
typedef struct {
    uint32_t p_type, p_flags; uint64_t p_offset, p_vaddr, p_paddr, p_filesz, p_memsz, p_align;
} Elf64_Phdr;
'''

suffix = r'''
static size_t put_note(unsigned char *at, uint32_t type, const char *name,
                       uint32_t namesz, const unsigned char *desc, uint32_t descsz)
{
    Elf32_Nhdr h = {.n_namesz = namesz, .n_descsz = descsz, .n_type = type};
    size_t namesz_aligned = ALIGN((uint64_t)namesz, 4);
    size_t descsz_aligned = ALIGN((uint64_t)descsz, 4);
    memcpy(at, &h, sizeof(h));
    if (namesz && name) memcpy(at + sizeof(h), name, namesz);
    if (descsz && desc) memcpy(at + sizeof(h) + namesz_aligned, desc, descsz);
    return sizeof(h) + namesz_aligned + descsz_aligned;
}

static void expect_bad(unsigned char *page, void *start, uint32_t length)
{
    unsigned char id[BUILD_ID_SIZE_MAX]; uint32_t size = 0xfeed;
    memset(id, 0xa5, sizeof(id));
    assert(parse_build_id(page, id, &size, start, length) == -EINVAL);
    assert(size == 0xfeed);
}

int main(void)
{
    unsigned char *allocation = calloc(1, PAGE_SIZE * 2);
    unsigned char *page, *note, desc[BUILD_ID_SIZE_MAX + 4], id[BUILD_ID_SIZE_MAX];
    uint32_t size; size_t len; int i;
    assert(allocation);
    page = allocation + PAGE_SIZE;
    memset(desc, 0x42, sizeof(desc));

    note = page + 128; len = put_note(note, BUILD_ID, "GNU\0", 4, desc, 20);
    size = 0; memset(id, 0, sizeof(id));
    assert(parse_build_id(page, id, &size, note, len) == 0);
    assert(size == 20 && id[0] == 0x42 && id[19] == 0x42);
    for (i = 20; i < BUILD_ID_SIZE_MAX; ++i) assert(id[i] == 0);

    note = page + 256; len = put_note(note, BUILD_ID, "BAD\0", 4, desc, 20);
    expect_bad(page, note, len);
    note = page + 320; len = put_note(note, BUILD_ID, "GNU\0", 4, desc, 0);
    expect_bad(page, note, len);
    note = page + 384; len = put_note(note, BUILD_ID, "GNU\0", 4, desc, 21);
    expect_bad(page, note, len);

    /* Segment has a complete header and GNU name, but only one desc byte. */
    note = page + PAGE_SIZE - 13;
    {
        Elf32_Nhdr h = {.n_namesz = 4, .n_descsz = 20, .n_type = BUILD_ID};
        memcpy(note, &h, sizeof(h)); note[sizeof(h)] = 'G';
    }
    expect_bad(page, note, 13);
    /* Full GNU name at the page end, but no descriptor bytes remain. */
    note = page + PAGE_SIZE - 16;
    {
        Elf32_Nhdr h = {.n_namesz = 4, .n_descsz = 20, .n_type = BUILD_ID};
        memcpy(note, &h, sizeof(h)); memcpy(note + sizeof(h), "GNU\\0", 4);
    }
    expect_bad(page, note, 16);
    expect_bad(page, page + 512, sizeof(Elf32_Nhdr) - 1);
    note = page + 640; memset(note, 0, 16);
    {
        Elf32_Nhdr h = {.n_namesz = UINT32_MAX, .n_descsz = UINT32_MAX,
                        .n_type = BUILD_ID};
        memcpy(note, &h, sizeof(h));
    }
    expect_bad(page, note, 16);
    expect_bad(page, page - 1, 1);
    expect_bad(page, page + PAGE_SIZE - 1, 2);
    expect_bad(page, page + 8, PAGE_SIZE + 1);

    /* An unrelated note may precede a valid GNU build-ID note. */
    note = page + 768;
    len = put_note(note, 1, "XYZ\0", 4, desc, 4);
    len += put_note(note + len, BUILD_ID, "GNU\0", 4, desc, 12);
    size = 0;
    assert(parse_build_id(page, id, &size, note, len) == 0 && size == 12);

    /* ELF32/ELF64 program-header offsets and PT_NOTE ranges are also bounded. */
    memset(page, 0, PAGE_SIZE);
    {
        Elf32_Ehdr *eh = (Elf32_Ehdr *)page;
        Elf32_Phdr *ph = (Elf32_Phdr *)(page + sizeof(*eh));
        eh->e_phoff = sizeof(*eh); eh->e_phnum = 1;
        ph->p_type = PT_NOTE; ph->p_offset = 512;
        ph->p_filesz = put_note(page + 512, BUILD_ID, "GNU\0", 4, desc, 20);
        size = 0; assert(get_build_id_32(page, id, &size) == 0 && size == 20);
        eh->e_phoff = 4; assert(get_build_id_32(page, id, &size) == -EINVAL);
        eh->e_phoff = sizeof(*eh); ph->p_offset = UINT32_MAX;
        assert(get_build_id_32(page, id, &size) == -EINVAL);
        ph->p_offset = PAGE_SIZE - 8; ph->p_filesz = 16;
        assert(get_build_id_32(page, id, &size) == -EINVAL);
        eh->e_phnum = UINT16_MAX;
        assert(get_build_id_32(page, id, &size) == -EINVAL);
    }
    memset(page, 0, PAGE_SIZE);
    {
        Elf64_Ehdr *eh = (Elf64_Ehdr *)page;
        Elf64_Phdr *ph = (Elf64_Phdr *)(page + sizeof(*eh));
        eh->e_phoff = sizeof(*eh); eh->e_phnum = 1;
        ph->p_type = PT_NOTE; ph->p_offset = 512;
        ph->p_filesz = put_note(page + 512, BUILD_ID, "GNU\0", 4, desc, 20);
        size = 0; assert(get_build_id_64(page, id, &size) == 0 && size == 20);
        ph->p_offset = UINT64_MAX;
        assert(get_build_id_64(page, id, &size) == -EINVAL);
        ph->p_offset = PAGE_SIZE - 8; ph->p_filesz = 16;
        assert(get_build_id_64(page, id, &size) == -EINVAL);
    }
    free(allocation);
    puts("PASS: build-ID notes validate GNU names, lengths, offsets and ELF32/64 bounds");
    return 0;
}
'''

with tempfile.TemporaryDirectory(prefix="build-id-host-test-") as tmp:
    c_file = Path(tmp) / "build_id_parser_test.c"
    exe = Path(tmp) / "build_id_parser_test"
    c_file.write_text(prefix + source[parse_start:parse_end] + "\n" +
                      source[get32_start:get32_end] + "\n" +
                      source[get64_start:get64_end] + suffix)
    cc = shlex.split(os.environ.get("CC", "cc"))
    subprocess.run(cc + ["-std=gnu11", "-O1", "-g", "-Wall", "-Wextra", "-Werror",
                         "-fsanitize=address,undefined", "-fno-sanitize-recover=all",
                         str(c_file), "-o", str(exe)], check=True)
    env = os.environ.copy()
    env.setdefault("ASAN_OPTIONS", "detect_leaks=0")
    subprocess.run([str(exe)], check=True, env=env)
