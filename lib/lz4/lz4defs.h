#ifndef __LZ4DEFS_H__
#define __LZ4DEFS_H__

/*
 * lz4defs.h -- common and architecture specific defines for the kernel usage

 * LZ4 - Fast LZ compression algorithm
 * Copyright (C) 2011-2026, Yann Collet.
 * BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *	* Redistributions of source code must retain the above copyright
 *	  notice, this list of conditions and the following disclaimer.
 *	* Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * You can contact the author at :
 *	- LZ4 homepage : http://www.lz4.org
 *	- LZ4 source repository : https://github.com/lz4/lz4
 *
 *	Changed for kernel usage by:
 *	Sven Schmidt <4sschmid@informatik.uni-hamburg.de>
 */

#include <asm/unaligned.h>
#include <linux/compiler.h>
#include <linux/string.h> /* memset, memcpy */
#include <linux/types.h>
#include <linux/build_bug.h>

/*
 * The vendored sources are the upstream v1.11.0 library self-contained
 * translation units, so their private configuration has to be provided here.
 * No libc is available, hence no dynamic allocation.
 */
#define LZ4_FREESTANDING 1
#define LZ4_STATIC_LINKING_ONLY_DISABLE_MEMORY_ALLOCATION 1

#define FORCE_INLINE __always_inline
#define LZ4_FORCE_INLINE static __always_inline
#define LZ4_FORCE_NOINLINE static __attribute__((noinline))
#define LZ4_FORCE_O2
#define LZ4_ALIGN_TEST 0

/* Upstream v1.11.0 HC level names (the kernel header uses different names) */
#define LZ4HC_CLEVEL_MIN 2
#define LZ4HC_CLEVEL_DEFAULT 9
#define LZ4HC_CLEVEL_OPT_MIN 10
#define LZ4HC_CLEVEL_MAX 12

/*-************************************
 *	Basic Types
 **************************************/
typedef uint8_t BYTE;
typedef uint16_t U16;
typedef uint32_t U32;
typedef int32_t S32;
typedef uint64_t U64;
typedef uintptr_t uptrval;

/* Upstream v1.11.0 private type names */
typedef int8_t LZ4_i8;
typedef uint8_t LZ4_byte;
typedef uint16_t LZ4_u16;
typedef uint32_t LZ4_u32;

/*-************************************
 *	Architecture specifics
 **************************************/
#if defined(CONFIG_64BIT)
#define LZ4_ARCH64 1
#else
#define LZ4_ARCH64 0
#endif

#if defined(__LITTLE_ENDIAN)
#define LZ4_LITTLE_ENDIAN 1
#else
#define LZ4_LITTLE_ENDIAN 0
#endif

#define DEBUGLOG(l, ...) \
	{                \
	} /* disabled */

#ifndef assert
#define assert(condition) ((void)0)
#endif

/*-************************************
 *	Constants
 **************************************/
#define LZ4_DISTANCE_ABSOLUTE_MAX 65535
#define LZ4_DISTANCE_MAX 65535
#define MINMATCH 4

#define WILDCOPYLENGTH 8
#define LASTLITERALS 5 /* see ../doc/lz4_Block_format.md#parsing-restrictions */
#define MFLIMIT 12 /* see ../doc/lz4_Block_format.md#parsing-restrictions */
#define MATCH_SAFEGUARD_DISTANCE \
	((2 * WILDCOPYLENGTH) -  \
	 MINMATCH) /* ensure it's possible to write 2 x wildcopyLength without overflowing output buffer */
#define FASTLOOP_SAFE_DISTANCE 64
static const int LZ4_minLength = (MFLIMIT + 1);

#define KB *(1 << 10)
#define MB *(1 << 20)
#define GB *(1U << 30)

#if defined(__x86_64__)
typedef U64 reg_t; /* 64-bits in x32 mode */
#else
typedef size_t reg_t; /* 32-bits in x32 mode */
#endif

#define STEPSIZE sizeof(size_t)

#define ML_BITS 4
#define ML_MASK ((1U << ML_BITS) - 1)
#define RUN_BITS (8 - ML_BITS)
#define RUN_MASK ((1U << RUN_BITS) - 1)

/*-************************************
 *	Memory routines
 **************************************/
#ifndef LZ4_memcpy
#define LZ4_memcpy(dst, src, size) __builtin_memcpy(dst, src, size)
#endif
#ifndef LZ4_memmove
#define LZ4_memmove(dst, src, size) __builtin_memmove(dst, src, size)
#endif
#ifndef LZ4_memset
#define LZ4_memset(p, v, s) __builtin_memset((p), (v), (s))
#endif
#define MEM_INIT(p, v, s) LZ4_memset((p), (v), (s))

static FORCE_INLINE unsigned LZ4_isLittleEndian(void)
{
	return LZ4_LITTLE_ENDIAN;
}

static FORCE_INLINE int LZ4_isAligned(const void *ptr, size_t alignment)
{
	return ((size_t)ptr & (alignment - 1)) == 0;
}

/*-************************************
 *	Reading and writing into memory
 **************************************/
static FORCE_INLINE U16 LZ4_read16(const void *ptr)
{
	return get_unaligned((const U16 *)ptr);
}

static FORCE_INLINE U32 LZ4_read32(const void *ptr)
{
	return get_unaligned((const U32 *)ptr);
}

static FORCE_INLINE reg_t LZ4_read_ARCH(const void *ptr)
{
	return get_unaligned((const reg_t *)ptr);
}

static FORCE_INLINE void LZ4_write16(void *memPtr, U16 value)
{
	put_unaligned(value, (U16 *)memPtr);
}

static FORCE_INLINE void LZ4_write32(void *memPtr, U32 value)
{
	put_unaligned(value, (U32 *)memPtr);
}

static FORCE_INLINE U16 LZ4_readLE16(const void *memPtr)
{
	return get_unaligned_le16(memPtr);
}

static FORCE_INLINE void LZ4_writeLE16(void *memPtr, U16 value)
{
	put_unaligned_le16(value, memPtr);
}

/* customized variant of memcpy, which can overwrite up to 8 bytes beyond dstEnd */
static FORCE_INLINE void LZ4_wildCopy8(void *dstPtr, const void *srcPtr,
				       void *dstEnd)
{
	BYTE *d = (BYTE *)dstPtr;
	const BYTE *s = (const BYTE *)srcPtr;
	BYTE *const e = (BYTE *)dstEnd;

	do {
		LZ4_memcpy(d, s, 8);
		d += 8;
		s += 8;
	} while (d < e);
}

static const unsigned inc32table[8] = { 0, 1, 2, 1, 0, 4, 4, 4 };
static const int dec64table[8] = { 0, 0, 0, -1, -4, 1, 2, 3 };

#ifndef LZ4_FAST_DEC_LOOP
#if defined(__i386__) || defined(__x86_64__) || defined(__aarch64__)
#define LZ4_FAST_DEC_LOOP 1
#else
#define LZ4_FAST_DEC_LOOP 0
#endif
#endif

#if !defined(LZ4_WILDCOPY64)
#if defined(__aarch64__) || defined(__x86_64__)
#define LZ4_WILDCOPY64 1
#else
#define LZ4_WILDCOPY64 0
#endif
#endif

#if LZ4_FAST_DEC_LOOP
#if LZ4_WILDCOPY64
/* customized variant of memcpy, which can overwrite up to 64 bytes beyond dstEnd */
static FORCE_INLINE void LZ4_wildCopy64(void *dstPtr, const void *srcPtr,
					void *dstEnd)
{
	BYTE *d = (BYTE *)dstPtr;
	const BYTE *s = (const BYTE *)srcPtr;
	BYTE *const e = (BYTE *)dstEnd;

	do {
		LZ4_memcpy(d, s, 64);
		d += 64;
		s += 64;
	} while (d < e);
}
#endif

/* customized variant of memcpy, which can overwrite up to 32 bytes beyond dstEnd
 * this version copies two times 16 bytes (instead of one time 32 bytes)
 * because it must be compatible with offsets >= 16. */
static FORCE_INLINE void LZ4_wildCopy32(void *dstPtr, const void *srcPtr,
					void *dstEnd)
{
	BYTE *d = (BYTE *)dstPtr;
	const BYTE *s = (const BYTE *)srcPtr;
	BYTE *const e = (BYTE *)dstEnd;

	do {
		LZ4_memcpy(d, s, 16);
		LZ4_memcpy(d + 16, s + 16, 16);
		d += 32;
		s += 32;
	} while (d < e);
}

/* LZ4_memcpy_using_offset_base() :
 * copy a match of length >= MINMATCH (srcPtr + offset == dstPtr, overlap allowed),
 * can overwrite up to 12 bytes beyond dstEnd */
static FORCE_INLINE void LZ4_memcpy_using_offset_base(BYTE *dstPtr,
						      const BYTE *srcPtr,
						      BYTE *dstEnd,
						      const size_t offset)
{
	assert(srcPtr + offset == dstPtr);
	if (offset < 8) {
		LZ4_write32(dstPtr,
			    0); /* silence an msan warning when offset==0 */
		dstPtr[0] = srcPtr[0];
		dstPtr[1] = srcPtr[1];
		dstPtr[2] = srcPtr[2];
		dstPtr[3] = srcPtr[3];
		srcPtr += inc32table[offset];
		LZ4_memcpy(dstPtr + 4, srcPtr, 4);
		srcPtr -= dec64table[offset];
	} else {
		LZ4_memcpy(dstPtr, srcPtr, 8);
		srcPtr += 8;
	}
	dstPtr += 8;

	/* srcPtr is now at least 8 bytes behind dstPtr */
	LZ4_memcpy(dstPtr, srcPtr, 8);
	if (dstEnd > dstPtr + 8)
		LZ4_wildCopy8(dstPtr + 8, srcPtr + 8, dstEnd);
}

/* LZ4_memcpy_using_offset()  presumes :
 * - dstEnd >= dstPtr + MINMATCH
 * - there is at least 12 bytes available to write after dstEnd */
static FORCE_INLINE void LZ4_memcpy_using_offset(BYTE *dstPtr,
						 const BYTE *srcPtr,
						 BYTE *dstEnd,
						 const size_t offset)
{
	BYTE v[8];

	assert(dstEnd >= dstPtr + MINMATCH);

	switch (offset) {
	case 1:
		MEM_INIT(v, *srcPtr, 8);
		break;
	case 2:
		LZ4_memcpy(v, srcPtr, 2);
		LZ4_memcpy(&v[2], srcPtr, 2);
		LZ4_memcpy(&v[4], v, 4);
		break;
	case 4:
		LZ4_memcpy(v, srcPtr, 4);
		LZ4_memcpy(&v[4], srcPtr, 4);
		break;
	default:
		LZ4_memcpy_using_offset_base(dstPtr, srcPtr, dstEnd, offset);
		return;
	}

	LZ4_memcpy(dstPtr, v, 8);
	dstPtr += 8;
	while (dstPtr < dstEnd) {
		LZ4_memcpy(dstPtr, v, 8);
		dstPtr += 8;
	}
}
#endif /* LZ4_FAST_DEC_LOOP */

static FORCE_INLINE unsigned int LZ4_NbCommonBytes(reg_t val)
{
#if LZ4_LITTLE_ENDIAN
	return __ffs(val) >> 3;
#else
	return (BITS_PER_LONG - 1 - __fls(val)) >> 3;
#endif
}

static FORCE_INLINE unsigned int LZ4_count(const BYTE *pIn, const BYTE *pMatch,
					   const BYTE *pInLimit)
{
	const BYTE *const pStart = pIn;

	if (likely(pIn < pInLimit - (STEPSIZE - 1))) {
		reg_t const diff = LZ4_read_ARCH(pMatch) ^ LZ4_read_ARCH(pIn);
		if (!diff) {
			pIn += STEPSIZE;
			pMatch += STEPSIZE;
		} else {
			return LZ4_NbCommonBytes(diff);
		}
	}

	while (likely(pIn < pInLimit - (STEPSIZE - 1))) {
		reg_t const diff = LZ4_read_ARCH(pMatch) ^ LZ4_read_ARCH(pIn);
		if (!diff) {
			pIn += STEPSIZE;
			pMatch += STEPSIZE;
			continue;
		}
		pIn += LZ4_NbCommonBytes(diff);
		return (unsigned)(pIn - pStart);
	}

	if ((STEPSIZE == 8) && (pIn < (pInLimit - 3)) &&
	    (LZ4_read32(pMatch) == LZ4_read32(pIn))) {
		pIn += 4;
		pMatch += 4;
	}
	if ((pIn < (pInLimit - 1)) && (LZ4_read16(pMatch) == LZ4_read16(pIn))) {
		pIn += 2;
		pMatch += 2;
	}
	if ((pIn < pInLimit) && (*pMatch == *pIn))
		pIn++;
	return (unsigned)(pIn - pStart);
}

typedef enum {
	notLimited = 0,
	limitedOutput = 1,
	fillOutput = 2
} limitedOutput_directive;
typedef enum { clearedTable = 0, byPtr, byU32, byU16 } tableType_t;

typedef enum {
	noDict = 0,
	withPrefix64k,
	usingExtDict,
	usingDictCtx
} dict_directive;
typedef enum { noDictIssue = 0, dictSmall } dictIssue_directive;

typedef enum { endOnOutputSize = 0, endOnInputSize = 1 } endCondition_directive;
typedef enum { decode_full_block = 0, partial_decode = 1 } earlyEnd_directive;

#define LZ4_DECODER_RING_BUFFER_SIZE(maxBlockSize) \
	(65536 + 64 + (maxBlockSize)) /* for static allocation */

#define LZ4_STATIC_ASSERT(c) BUILD_BUG_ON(!(c))

#endif /* __LZ4DEFS_H__ */
