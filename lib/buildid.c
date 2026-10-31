// SPDX-License-Identifier: GPL-2.0

#include <linux/buildid.h>
#include <linux/compiler.h>
#include <linux/elf.h>
#include <linux/pagemap.h>

#define BUILD_ID 3
/*
 * Parse build id from the note segment. This logic can be shared between
 * 32-bit and 64-bit system, because Elf32_Nhdr and Elf64_Nhdr are
 * identical.
 */
static inline int parse_build_id(void *page_addr,
				 unsigned char *build_id,
				 __u32 *size,
				 void *note_start,
				 Elf32_Word note_size)
{
	const unsigned long page = (unsigned long)page_addr;
	const unsigned long start = (unsigned long)note_start;
	u64 note_off = 0;

	/* Bound the complete note segment before dereferencing any header. */
	if (start < page || note_size > PAGE_SIZE ||
	    start - page > PAGE_SIZE - note_size)
		return -EINVAL;

	while (note_off <= note_size &&
	       note_size - note_off >= sizeof(Elf32_Nhdr)) {
		Elf32_Nhdr nhdr;
		u32 raw_name_size, raw_desc_size, note_type;
		u64 name_off, name_size, desc_size, desc_off;

		/* ELF note offsets are untrusted and need not be naturally aligned. */
		memcpy(&nhdr, note_start + note_off, sizeof(nhdr));
		raw_name_size = nhdr.n_namesz;
		raw_desc_size = nhdr.n_descsz;
		note_type = nhdr.n_type;
		name_off = note_off + sizeof(nhdr);
		name_size = ALIGN((u64)raw_name_size, 4);
		desc_size = ALIGN((u64)raw_desc_size, 4);

		/* Check each aligned field with subtraction to avoid wraparound. */
		if (name_size > note_size - name_off)
			break;
		desc_off = name_off + name_size;
		if (desc_size > note_size - desc_off)
			break;

		if (note_type == BUILD_ID &&
		    raw_name_size == sizeof("GNU") &&
		    !memcmp(note_start + name_off, "GNU", sizeof("GNU")) &&
		    raw_desc_size > 0 &&
		    raw_desc_size <= BUILD_ID_SIZE_MAX) {
			memcpy(build_id, note_start + desc_off, raw_desc_size);
			memset(build_id + raw_desc_size, 0,
			       BUILD_ID_SIZE_MAX - raw_desc_size);
			if (size)
				*size = raw_desc_size;
			return 0;
		}

		note_off = desc_off + desc_size;
	}
	return -EINVAL;
}

/* Parse build ID from 32-bit ELF */
static int get_build_id_32(void *page_addr, unsigned char *build_id,
			   __u32 *size)
{
	Elf32_Ehdr *ehdr = (Elf32_Ehdr *)page_addr;
	Elf32_Phdr *phdr;
	__u32 phnum, i;

	/* This parser only supports the ELF header and phdr table in page one. */
	if (READ_ONCE(ehdr->e_phoff) != sizeof(Elf32_Ehdr))
		return -EINVAL;
	phnum = READ_ONCE(ehdr->e_phnum);
	if (phnum >
	    (PAGE_SIZE - sizeof(Elf32_Ehdr)) / sizeof(Elf32_Phdr))
		return -EINVAL;

	phdr = (Elf32_Phdr *)(page_addr + sizeof(Elf32_Ehdr));

	for (i = 0; i < phnum; ++i) {
		Elf32_Word note_off, note_size;

		if (READ_ONCE(phdr[i].p_type) != PT_NOTE)
			continue;
		note_off = READ_ONCE(phdr[i].p_offset);
		note_size = READ_ONCE(phdr[i].p_filesz);
		if (note_off > PAGE_SIZE || note_size > PAGE_SIZE - note_off)
			continue;
		if (!parse_build_id(page_addr, build_id, size,
				    page_addr + note_off, note_size))
			return 0;
	}
	return -EINVAL;
}

/* Parse build ID from 64-bit ELF */
static int get_build_id_64(void *page_addr, unsigned char *build_id,
			   __u32 *size)
{
	Elf64_Ehdr *ehdr = (Elf64_Ehdr *)page_addr;
	Elf64_Phdr *phdr;
	__u32 phnum, i;

	/* This parser only supports the ELF header and phdr table in page one. */
	if (READ_ONCE(ehdr->e_phoff) != sizeof(Elf64_Ehdr))
		return -EINVAL;
	phnum = READ_ONCE(ehdr->e_phnum);
	if (phnum >
	    (PAGE_SIZE - sizeof(Elf64_Ehdr)) / sizeof(Elf64_Phdr))
		return -EINVAL;

	phdr = (Elf64_Phdr *)(page_addr + sizeof(Elf64_Ehdr));

	for (i = 0; i < phnum; ++i) {
		Elf64_Off note_off;
		Elf64_Xword note_size;

		if (READ_ONCE(phdr[i].p_type) != PT_NOTE)
			continue;
		note_off = READ_ONCE(phdr[i].p_offset);
		note_size = READ_ONCE(phdr[i].p_filesz);
		if (note_off > PAGE_SIZE || note_size > PAGE_SIZE - note_off)
			continue;
		if (!parse_build_id(page_addr, build_id, size,
				    page_addr + note_off, note_size))
			return 0;
	}
	return -EINVAL;
}

/*
 * Parse build ID of ELF file mapped to vma
 * @vma:      vma object
 * @build_id: buffer to store build id, at least BUILD_ID_SIZE long
 * @size:     returns actual build id size in case of success
 *
 * Returns 0 on success, otherwise error (< 0).
 */
int build_id_parse(struct vm_area_struct *vma, unsigned char *build_id,
		   __u32 *size)
{
	Elf32_Ehdr *ehdr;
	struct page *page;
	void *page_addr;
	int ret;

	/* only works for page backed storage  */
	if (!vma->vm_file)
		return -EINVAL;

	page = find_get_page(vma->vm_file->f_mapping, 0);
	if (!page)
		return -EFAULT;	/* page not mapped */

	ret = -EINVAL;
	page_addr = kmap_atomic(page);
	ehdr = (Elf32_Ehdr *)page_addr;

	/* compare magic x7f "ELF" */
	if (memcmp(ehdr->e_ident, ELFMAG, SELFMAG) != 0)
		goto out;

	/* only support executable file and shared object file */
	if (ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN)
		goto out;

	if (ehdr->e_ident[EI_CLASS] == ELFCLASS32)
		ret = get_build_id_32(page_addr, build_id, size);
	else if (ehdr->e_ident[EI_CLASS] == ELFCLASS64)
		ret = get_build_id_64(page_addr, build_id, size);
out:
	kunmap_atomic(page_addr);
	put_page(page);
	return ret;
}
