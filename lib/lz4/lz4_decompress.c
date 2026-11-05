/*
 * LZ4 - Fast LZ compression algorithm
 * Copyright (C) 2011 - 2026, Yann Collet.
 * BSD 2 - Clause License (http://www.opensource.org/licenses/bsd - license.php)
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

/*-************************************
 *	Dependencies
 **************************************/
#include <linux/lz4.h>
#include "lz4defs.h"
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/unaligned.h>

/*-*****************************
 *	Decompression functions
 *******************************/

/*-*******************************
 *  Decompression functions
 ********************************/

#undef MIN
#define MIN(a,b)    ( (a) < (b) ? (a) : (b) )


/* variant for decompress_unsafe()
 * does not know end of input
 * presumes input is well formed
 * note : will consume at least one byte */
static size_t read_long_length_no_check(const BYTE** pp)
{
    size_t b, l = 0;
    do { b = **pp; (*pp)++; l += b; } while (b==255);
    DEBUGLOG(6, "read_long_length_no_check: +length=%zu using %zu input bytes", l, l/255 + 1)
    return l;
}

/* core decoder variant for LZ4_decompress_fast*()
 * for legacy support only : these entry points are deprecated.
 * - Presumes input is correctly formed (no defense vs malformed inputs)
 * - Does not know input size (presume input buffer is "large enough")
 * - Decompress a full block (only)
 * @return : nb of bytes read from input.
 * Note : this variant is not optimized for speed, just for maintenance.
 *        the goal is to remove support of decompress_fast*() variants by v2.0
**/
LZ4_FORCE_INLINE int
LZ4_decompress_unsafe_generic(
                 const BYTE* const istart,
                 BYTE* const ostart,
                 int decompressedSize,

                 size_t prefixSize,
                 const BYTE* const dictStart,  /* only if dict==usingExtDict */
                 const size_t dictSize         /* note: =0 if dictStart==NULL */
                 )
{
    const BYTE* ip = istart;
    BYTE* op = (BYTE*)ostart;
    BYTE* oend;
    const BYTE* const prefixStart = ostart - prefixSize;

    DEBUGLOG(5, "LZ4_decompress_unsafe_generic");
    if (dictStart == NULL) assert(dictSize == 0);
    if (decompressedSize < 0) return -1;
    oend = ostart + decompressedSize;

    while (1) {
        /* start new sequence */
        unsigned token = *ip++;

        /* literals */
        {   size_t ll = token >> ML_BITS;
            if (ll==15) {
                /* long literal length */
                ll += read_long_length_no_check(&ip);
            }
            if ((size_t)(oend-op) < ll) return -1; /* output buffer overflow */
            LZ4_memmove(op, ip, ll); /* support in-place decompression */
            op += ll;
            ip += ll;
            if ((size_t)(oend-op) < MFLIMIT) {
                if (op==oend) break;  /* end of block */
                DEBUGLOG(5, "invalid: literals end at distance %zi from end of block", oend-op);
                /* incorrect end of block :
                 * last match must start at least MFLIMIT==12 bytes before end of output block */
                return -1;
        }   }

        /* match */
        {   size_t ml = token & 15;
            size_t const offset = LZ4_readLE16(ip);
            ip+=2;

            if (ml==15) {
                /* long literal length */
                ml += read_long_length_no_check(&ip);
            }
            ml += MINMATCH;

            if ((size_t)(oend-op) < ml) return -1; /* output buffer overflow */

            {   const BYTE* match = op - offset;

                /* out of range */
                if (offset > (size_t)(op - prefixStart) + dictSize) {
                    DEBUGLOG(6, "offset out of range");
                    return -1;
                }

                /* check special case : extDict */
                if (offset > (size_t)(op - prefixStart)) {
                    /* extDict scenario */
                    const BYTE* const dictEnd = dictStart + dictSize;
                    const BYTE* extMatch = dictEnd - (offset - (size_t)(op-prefixStart));
                    size_t const extml = (size_t)(dictEnd - extMatch);
                    if (extml > ml) {
                        /* match entirely within extDict */
                        LZ4_memmove(op, extMatch, ml);
                        op += ml;
                        ml = 0;
                    } else {
                        /* match split between extDict & prefix */
                        LZ4_memmove(op, extMatch, extml);
                        op += extml;
                        ml -= extml;
                    }
                    match = prefixStart;
                }

                /* match copy - slow variant, supporting overlap copy */
                {   size_t u;
                    for (u=0; u<ml; u++) {
                        op[u] = match[u];
            }   }   }
            op += ml;
            if ((size_t)(oend-op) < LASTLITERALS) {
                DEBUGLOG(5, "invalid: match ends at distance %zi from end of block", oend-op);
                /* incorrect end of block :
                 * last match must stop at least LASTLITERALS==5 bytes before end of output block */
                return -1;
            }
        } /* match */
    } /* main loop */
    return (int)(ip - istart);
}


/* Read the variable-length literal or match length.
 *
 * @ipPtr : pointer to input pointer, will be advanced
 * @ilimit : read is forbidden beyond this position (must be within input buffer)
**/
typedef size_t Rvl_t;
static const Rvl_t rvl_error = (Rvl_t)(-1);
LZ4_FORCE_INLINE Rvl_t
read_variable_length(const BYTE** ipPtr, const BYTE* ilimit)
{
    Rvl_t s, length = 0;
    assert(ipPtr != NULL);
    assert(*ipPtr !=  NULL);
    assert(ilimit != NULL);
    if (unlikely((*ipPtr) >= ilimit)) {    /* read limit reached */
        return rvl_error;
    }
    s = **ipPtr;
    (*ipPtr)++;
    length += s;
    if (likely(s != 255)) return length;
    do {
        if (unlikely((*ipPtr) >= ilimit)) {    /* read limit reached */
            return rvl_error;
        }
        s = **ipPtr;
        (*ipPtr)++;
        length += s;
        /* accumulator overflow detection (32-bit mode only) */
        if ((sizeof(length) < 8) && unlikely(length > ((Rvl_t)(-1)/2)) ) {
            return rvl_error;
        }
    } while (s == 255);

    return length;
}

/* LZ4_copyMatchExact() :
 * copy a match of length bytes from match to op (match < op, overlap allowed),
 * writing nothing beyond op+length. Invalid offset 0 (match == op) produces 0 bytes.
 * Not inlined : rarely used, its memcpy() calls would burden the decoder's hot loops.
 * @return : output position after the match */
LZ4_FORCE_NOINLINE BYTE*
LZ4_copyMatchExact(BYTE* op, const BYTE* match, size_t length)
{
    if (unlikely(match == op)) {   /* invalid offset 0 : clear the match segment, see doc/lz4_Block_format.md */
        MEM_INIT(op, 0, length);
        return op + length;
    }
    /* bytes from match to op repeat with period (op-match) :
     * copy all of them, so the distance doubles at each round.
     * Beyond 2 KB, keep the distance : larger copies of just written data are slower */
    while (length > (size_t)(op - match)) {
        size_t const available = (size_t)(op - match);
        LZ4_memcpy(op, match, available);
        op += available;
        length -= available;
        if (available >= 2048) match += available;
    }
    LZ4_memcpy(op, match, length);
    return op + length;
}

/* LZ4_copyExtDictMatch() :
 * copy a match starting within the external dictionary (match < lowPrefix),
 * possibly stretching into the current block.
 * @return : output position after the match */
LZ4_FORCE_INLINE BYTE*
LZ4_copyExtDictMatch(BYTE* op, const BYTE* match, size_t length,
                     const BYTE* lowPrefix, const BYTE* dictEnd)
{
    if (length <= (size_t)(lowPrefix-match)) {
        /* match fits entirely within external dictionary : just copy */
        LZ4_memmove(op, dictEnd - (lowPrefix-match), length);
        return op + length;
    } else {
        /* match stretches into both external dictionary and current block */
        size_t const copySize = (size_t)(lowPrefix - match);
        LZ4_memcpy(op, dictEnd - copySize, copySize);
        return LZ4_copyMatchExact(op + copySize, lowPrefix, length - copySize);
    }
}

/*! LZ4_decompress_generic() :
 *  This generic decompression function covers all use cases.
 *  It shall be instantiated several times, using different sets of directives.
 *  Note that it is important for performance that this function really get inlined,
 *  in order to remove useless branches during compilation optimization.
 */
LZ4_FORCE_INLINE int
LZ4_decompress_generic(
                 const char* const src,
                 char* const dst,
                 int srcSize,
                 int outputSize,         /* dstCapacity, or MIN(targetOutputSize, dstCapacity) when partialDecoding */

                 earlyEnd_directive partialDecoding,  /* full, partial */
                 dict_directive dict,                 /* noDict, withPrefix64k, usingExtDict */
                 const BYTE* const lowPrefix,  /* always <= dst, == dst when no prefix */
                 const BYTE* const dictStart,  /* only if dict==usingExtDict */
                 const size_t dictSize         /* note : = 0 if noDict */
                 )
{
    if ((src == NULL) || (outputSize < 0)) { return -1; }

    {   const BYTE* ip = (const BYTE*) src;
        const BYTE* const iend = ip + srcSize;

        BYTE* op = (BYTE*) dst;
        BYTE* const oend = op + outputSize;

        const BYTE* const dictEnd = (dictStart == NULL) ? NULL : dictStart + dictSize;

        /* withPrefix64k : offset <= 65535 can't reach beyond the 64 KB prefix */
        const int checkOffset = (dict != withPrefix64k) && (dictSize < (int)(64 KB));


        /* Set up the "end" pointers for the shortcut. */
        const BYTE* const shortiend = iend - 14 /*maxLL*/ - 2 /*offset*/;
        const BYTE* const shortoend = oend - 14 /*maxLL*/ - 18 /*maxML*/;

        /* shared by both loops : the fast loop can hand over a decoded match (safe_match_copy) */
        const BYTE* match;
        size_t offset;
        size_t ml;   /* match length */


        DEBUGLOG(5, "LZ4_decompress_generic (srcSize:%i, dstSize:%i)", srcSize, outputSize);

        /* Special cases */
        assert(lowPrefix <= op);
        if (unlikely(outputSize==0)) {
            /* Empty output buffer */
            if (partialDecoding) return 0;
            if ((srcSize==1) && (*ip==0)) return 0;   /* empty block */
            return (srcSize > 1) ? -2 : -1;   /* -2 : output buffer too small */
        }
        if (unlikely(srcSize<=0)) { return -1; }

    /* LZ4_FAST_DEC_LOOP:
     * designed for modern OoO performance cpus,
     * where copying reliably 32-bytes is preferable to an unpredictable branch.
     * note : fast loop may show a regression for some client arm chips. */
#if LZ4_FAST_DEC_LOOP
        if ((oend - op) < FASTLOOP_SAFE_DISTANCE) {
            DEBUGLOG(6, "move to safe decode loop");
            goto safe_decode;
        }
        /* Ring buffer restarting at its beginning : the oldest referenced history (max distance behind op)
         * lies in the external dictionary, ahead of op, always at the same distance.
         * Rings sized with the former LZ4_DECODER_RING_BUFFER_SIZE() (margin 14, up to v1.10.0)
         * still work, but leave as little as 16 bytes :
         * the fast loop's wide copies could overwrite it, the safe loop's can't. */
        if ( (dict==usingExtDict)
          && ((uptrval)dictEnd - (uptrval)lowPrefix - LZ4_DISTANCE_ABSOLUTE_MAX < FASTLOOP_SAFE_DISTANCE) ) {
            DEBUGLOG(6, "history just ahead : move to safe decode loop");
            goto safe_decode;
        }

        /* Fast loop : decode sequences as long as output < oend-FASTLOOP_SAFE_DISTANCE,
         * and input is not too close to iend.
         * It hands over to the safe loop either at a sequence start (break),
         * or once a match is fully decoded (goto safe_match_copy). */
        DEBUGLOG(6, "using fast decode loop");
        while (1) {
            unsigned token;
            size_t ll;   /* literal length */
            BYTE* cpy;
            /* Main fastloop assertion: We can always wildcopy FASTLOOP_SAFE_DISTANCE */
            assert(oend - op >= FASTLOOP_SAFE_DISTANCE);
            assert(ip < iend);
            token = *ip++;
            ll = token >> ML_BITS;
            DEBUGLOG(7, "blockPos%6u: litLength token = %u", (unsigned)(op-(BYTE*)dst), (unsigned)ll);

            /* too close to input end : the safe loop restarts this sequence */
            if (ip > iend-(16 + 1/*max lit + offset + nextToken*/)) { ip--; break; }

            /* decode literal length */
            if (ll == RUN_MASK) {
                /* literal length >= RUN_MASK means >= RUN_MASK literal bytes follow the extension bytes,
                 * so extension bytes cannot reach the last RUN_MASK bytes of input */
                const BYTE* const tokenPos = ip-1;
                size_t const addl = read_variable_length(&ip, iend - RUN_MASK);
                if (addl == rvl_error) {
                    DEBUGLOG(6, "error reading long literal length");
                    goto _output_error;
                }
                ll += addl;
                if (unlikely((uptrval)(op)+ll<(uptrval)(op))) { goto _output_error; } /* overflow detection */
                if (unlikely((uptrval)(ip)+ll<(uptrval)(ip))) { goto _output_error; } /* overflow detection */

                /* copy literals, unless too close to the end for a wild copy :
                 * nothing is written yet, so the safe loop can restart this sequence */
                cpy = op+ll;
                LZ4_STATIC_ASSERT(MFLIMIT >= WILDCOPYLENGTH);
              #if LZ4_WILDCOPY64
                if ((cpy>oend-64) || (ip+ll>iend-64)) { ip = tokenPos; break; }
                LZ4_wildCopy64(op, ip, cpy);
              #else
                if ((cpy>oend-32) || (ip+ll>iend-32)) { ip = tokenPos; break; }
                LZ4_wildCopy32(op, ip, cpy);
              #endif
                ip += ll; op = cpy;
            } else {
                DEBUGLOG(7, "copy %u bytes in a 16-bytes stripe", (unsigned)ll);
                /* Literals can only be <= 14, but hope compilers optimize better when copy by a register size */
                LZ4_memcpy(op, ip, 16);
                ip += ll; op += ll;
            }

            /* get offset */
            offset = LZ4_readLE16(ip); ip+=2;
            DEBUGLOG(6, "blockPos%6u: offset = %u", (unsigned)(op-(BYTE*)dst), (unsigned)offset);
            match = op - offset;
            assert(match <= op);  /* overflow check */

            /* get matchlength */
            ml = token & ML_MASK;
            DEBUGLOG(7, "  match length token = %u (len==%u)", (unsigned)ml, (unsigned)ml+MINMATCH);

            if (ml == ML_MASK) {
                /* after match length extension bytes, at least 1 token + LASTLITERALS literals must remain */
                size_t const addl = read_variable_length(&ip, iend - (1 + LASTLITERALS));
                if (addl == rvl_error) {
                    DEBUGLOG(5, "error reading long match length");
                    goto _output_error;
                }
                ml += addl;
                DEBUGLOG(7, "  long match length == %u", (unsigned)ml + MINMATCH);
                if (unlikely((uptrval)(op)+ml<(uptrval)op)) { goto _output_error; } /* overflow detection */
            }
            ml += MINMATCH;

            if (op + ml >= oend - FASTLOOP_SAFE_DISTANCE) {
                DEBUGLOG(7, "moving to safe_match_copy (ml==%u)", (unsigned)ml);
                goto safe_match_copy;
            }

            /* match length from token only (<= 18), no overlap, within prefix : copy 18 bytes */
            if ( ((token & ML_MASK) != ML_MASK) && (offset >= 8)
              && ((dict == withPrefix64k) || (match >= lowPrefix)) ) {
                assert(match >= lowPrefix);
                assert(match <= op);
                assert(op + 18 <= oend);
                LZ4_memcpy(op, match, 8);
                LZ4_memcpy(op+8, match+8, 8);
                LZ4_memcpy(op+16, match+16, 2);
                op += ml;
                continue;
            }

            /* match ends far from oend : no end-of-block condition to check */
            assert(op + ml < oend - FASTLOOP_SAFE_DISTANCE);

            if ( checkOffset && (unlikely(match + dictSize < lowPrefix)) ) {
                DEBUGLOG(5, "Error : pos=%zi, offset=%zi => outside buffers", op-lowPrefix, op-match);
                goto _offset_error;
            }
            /* match starting within external dictionary */
            if ((dict==usingExtDict) && (match < lowPrefix)) {
                assert(dictEnd != NULL);
                op = LZ4_copyExtDictMatch(op, match, ml, lowPrefix, dictEnd);
                continue;
            }

            /* copy match within block */
            cpy = op + ml;

            assert((op <= oend) && (oend-op >= 64));
            if (unlikely(offset<16)) {
                LZ4_memcpy_using_offset(op, match, cpy, offset);
    #if LZ4_WILDCOPY64
            } else if (offset >= 64) {
                LZ4_wildCopy64(op, match, cpy);
    #endif
            } else {
                LZ4_wildCopy32(op, match, cpy);
            }

            op = cpy;   /* wildcopy correction */
        }
    safe_decode:
#endif

        /* Safe loop : decode remaining sequences, without writing beyond oend nor reading beyond iend.
         * With LZ4_FAST_DEC_LOOP, that's the end of the block (or a block too small for the fast loop),
         * entered at a sequence start, or at safe_match_copy with a decoded match.
         * Without it, that's the whole block. */
        DEBUGLOG(6, "using safe decode loop");
        while (1) {
            unsigned token;
            size_t ll;   /* literal length */
            BYTE* cpy;
            assert(ip < iend);
            token = *ip++;
            ll = token >> ML_BITS;
            DEBUGLOG(7, "blockPos%6u: litLength token = %u", (unsigned)(op-(BYTE*)dst), (unsigned)ll);

            /* A two-stage shortcut for the most common case:
             * 1) If the literal length is 0..14, and there is enough space,
             * enter the shortcut and copy 16 bytes on behalf of the literals.
             * 2) Further if the match length is 4..18, copy 18 bytes in a similar
             * manner; but we ensure that there's enough space in the output for
             * those 18 bytes earlier, upon entering the shortcut (in other words,
             * there is a combined check for both stages).
             */
            if ( (ll != RUN_MASK)
                /* strictly "less than" on input, to re-enter the loop with at least one byte */
              && likely((ip < shortiend) & (op <= shortoend)) ) {
                /* Copy the literals */
                LZ4_memcpy(op, ip, 16);
                op += ll; ip += ll;

                /* The second stage: prepare for match copying, decode full info.
                 * If it doesn't work out, the info won't be wasted. */
                offset = LZ4_readLE16(ip); ip += 2;
                match = op - offset;
                assert(match <= op); /* check overflow */
                ml = token & ML_MASK;
                DEBUGLOG(7, "blockPos%6u: matchLength token = %u (len=%u)", (unsigned)(op-(BYTE*)dst), (unsigned)ml, (unsigned)ml + 4);

                /* Do not deal with overlapping matches. */
                if ( (ml != ML_MASK)
                  && (offset >= 8)
                  && (dict==withPrefix64k || match >= lowPrefix) ) {
                    /* Copy the match. */
                    LZ4_memcpy(op + 0, match + 0, 8);
                    LZ4_memcpy(op + 8, match + 8, 8);
                    LZ4_memcpy(op +16, match +16, 2);
                    op += ml + MINMATCH;
                    /* Both stages worked, load the next token. */
                    continue;
                }
                /* The second stage didn't work out, but the info is ready for the match copy below. */
            } else {
                /* decode literal length */
                if (ll == RUN_MASK) {
                    /* literal length >= RUN_MASK means >= RUN_MASK literal bytes follow the extension bytes,
                     * so extension bytes cannot reach the last RUN_MASK bytes of input */
                    size_t const addl = read_variable_length(&ip, iend-RUN_MASK);
                    if (addl == rvl_error) { goto _output_error; }
                    ll += addl;
                    if (unlikely((uptrval)(op)+ll<(uptrval)(op))) { goto _output_error; } /* overflow detection */
                    if (unlikely((uptrval)(ip)+ll<(uptrval)(ip))) { goto _output_error; } /* overflow detection */
                }

                /* copy literals */
                cpy = op+ll;
                LZ4_STATIC_ASSERT(MFLIMIT >= WILDCOPYLENGTH);
                if ((cpy>oend-MFLIMIT) || (ip+ll>iend-(2+1+LASTLITERALS))) {
                    /* close to input or output end : these must be the last literals of the block,
                     * except with partialDecoding, which can also stop within them */
                    if (partialDecoding) {
                        /* stop at input end, or when output is full */
                        ll = MIN(ll, (size_t)(iend-ip));
                        ll = MIN(ll, (size_t)(oend-op));
                    } else if ((ip+ll != iend) || (cpy > oend)) {
                        /* last literals must end exactly at input end, within output */
                        DEBUGLOG(5, "invalid last literals : they end at iend%+zi and oend%+zi", (ip+ll)-iend, cpy-oend);
                        if ((ip+ll <= iend) && (cpy > oend-MFLIMIT)) goto _dst_too_small;   /* output full, input left */
                        goto _output_error;
                    }
                    LZ4_memmove(op, ip, ll);  /* supports overlapping memory regions, for in-place decompression scenarios */
                    ip += ll;
                    op += ll;
                    /* end of block, unless partialDecoding can go on : output not full, input left for a match */
                    if (!partialDecoding || (op == oend) || (ip >= (iend-2))) {
                        break;
                    }
                } else {
                    LZ4_wildCopy8(op, ip, cpy);   /* can overwrite up to 8 bytes beyond cpy */
                    ip += ll; op = cpy;
                }

                /* get offset */
                offset = LZ4_readLE16(ip); ip+=2;
                match = op - offset;

                /* get matchlength */
                ml = token & ML_MASK;
                DEBUGLOG(7, "blockPos%6u: matchLength token = %u", (unsigned)(op-(BYTE*)dst), (unsigned)ml);
            }

            /* literals copied, offset decoded, ml == match length token */
            if (ml == ML_MASK) {
                /* after match length extension bytes, at least 1 token + LASTLITERALS literals must remain */
                size_t const addl = read_variable_length(&ip, iend - (1 + LASTLITERALS));
                if (addl == rvl_error) { goto _output_error; }
                ml += addl;
                if (unlikely((uptrval)(op)+ml<(uptrval)op)) goto _output_error;   /* overflow detection */
            }
            ml += MINMATCH;

#if LZ4_FAST_DEC_LOOP
        safe_match_copy:
#endif
            /* literals copied, match fully decoded (ml includes MINMATCH), offset not validated yet */
            if ((checkOffset) && (unlikely(match + dictSize < lowPrefix))) goto _offset_error;   /* Error : offset outside buffers */

            /* end of block : the last LASTLITERALS bytes must be literals */
            assert(op <= oend);
            if (unlikely(op+ml > oend-LASTLITERALS)) {
                if (partialDecoding) ml = MIN(ml, (size_t)(oend-op));   /* partialDecoding : may end anywhere */
                else goto _dst_too_small;
            }

            /* match starting within external dictionary */
            if ((dict==usingExtDict) && (match < lowPrefix)) {
                assert(dictEnd != NULL);
                op = LZ4_copyExtDictMatch(op, match, ml, lowPrefix, dictEnd);
                continue;
            }
            assert(match >= lowPrefix);

            /* close to block end : no wild copy */
            if (unlikely(op+ml > oend-MATCH_SAFEGUARD_DISTANCE)) {
                op = LZ4_copyMatchExact(op, match, ml);
                if (op == oend) { break; }   /* partialDecoding : output buffer full */
                continue;
            }

            /* copy match within block */
            cpy = op + ml;
            LZ4_memcpy_using_offset_base(op, match, cpy, offset);
            op = cpy;   /* wildcopy correction */
        }

        /* end of decoding */
        DEBUGLOG(5, "decoded %i bytes", (int) (((char*)op)-dst));
        return (int) (((char*)op)-dst);     /* Nb of output bytes decoded */

        /* errors : see LZ4_decompress_safe() in lz4.h */
    _output_error:
        return -1;
    _dst_too_small:
        return -2;
    _offset_error:
        return -3;
    }
}


/*===== Instantiate the API decoding functions. =====*/

LZ4_FORCE_O2
int LZ4_decompress_safe(const char* source, char* dest, int compressedSize, int maxDecompressedSize)
{
    return LZ4_decompress_generic(source, dest, compressedSize, maxDecompressedSize,
                                  decode_full_block, noDict,
                                  (BYTE*)dest, NULL, 0);
}

LZ4_FORCE_O2
int LZ4_decompress_safe_partial(const char* src, char* dst, int compressedSize, int targetOutputSize, int dstCapacity)
{
    dstCapacity = MIN(targetOutputSize, dstCapacity);
    return LZ4_decompress_generic(src, dst, compressedSize, dstCapacity,
                                  partial_decode,
                                  noDict, (BYTE*)dst, NULL, 0);
}

LZ4_FORCE_O2
int LZ4_decompress_fast(const char* source, char* dest, int originalSize)
{
    DEBUGLOG(5, "LZ4_decompress_fast");
    return LZ4_decompress_unsafe_generic(
                (const BYTE*)source, (BYTE*)dest, originalSize,
                0, NULL, 0);
}

/*===== Instantiate a few more decoding cases, used more than once. =====*/

LZ4_FORCE_O2 /* Exported, an obsolete API function. */
int LZ4_decompress_safe_withPrefix64k(const char* source, char* dest, int compressedSize, int maxOutputSize)
{
    return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize,
                                  decode_full_block, withPrefix64k,
                                  (BYTE*)dest - 64 KB, NULL, 0);
}

LZ4_FORCE_O2
static int LZ4_decompress_safe_partial_withPrefix64k(const char* source, char* dest, int compressedSize, int targetOutputSize, int dstCapacity)
{
    dstCapacity = MIN(targetOutputSize, dstCapacity);
    return LZ4_decompress_generic(source, dest, compressedSize, dstCapacity,
                                  partial_decode, withPrefix64k,
                                  (BYTE*)dest - 64 KB, NULL, 0);
}

/* Another obsolete API function, paired with the previous one. */
int LZ4_decompress_fast_withPrefix64k(const char* source, char* dest, int originalSize)
{
    return LZ4_decompress_unsafe_generic(
                (const BYTE*)source, (BYTE*)dest, originalSize,
                64 KB, NULL, 0);
}

LZ4_FORCE_O2
static int LZ4_decompress_safe_withSmallPrefix(const char* source, char* dest, int compressedSize, int maxOutputSize,
                                               size_t prefixSize)
{
    return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize,
                                  decode_full_block, noDict,
                                  (BYTE*)dest-prefixSize, NULL, 0);
}

LZ4_FORCE_O2
static int LZ4_decompress_safe_partial_withSmallPrefix(const char* source, char* dest, int compressedSize, int targetOutputSize, int dstCapacity,
                                               size_t prefixSize)
{
    dstCapacity = MIN(targetOutputSize, dstCapacity);
    return LZ4_decompress_generic(source, dest, compressedSize, dstCapacity,
                                  partial_decode, noDict,
                                  (BYTE*)dest-prefixSize, NULL, 0);
}

LZ4_FORCE_O2
int LZ4_decompress_safe_forceExtDict(const char* source, char* dest,
                                     int compressedSize, int maxOutputSize,
                                     const void* dictStart, size_t dictSize)
{
    DEBUGLOG(5, "LZ4_decompress_safe_forceExtDict");
    return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize,
                                  decode_full_block, usingExtDict,
                                  (BYTE*)dest, (const BYTE*)dictStart, dictSize);
}

LZ4_FORCE_O2
int LZ4_decompress_safe_partial_forceExtDict(const char* source, char* dest,
                                     int compressedSize, int targetOutputSize, int dstCapacity,
                                     const void* dictStart, size_t dictSize)
{
    dstCapacity = MIN(targetOutputSize, dstCapacity);
    return LZ4_decompress_generic(source, dest, compressedSize, dstCapacity,
                                  partial_decode, usingExtDict,
                                  (BYTE*)dest, (const BYTE*)dictStart, dictSize);
}

LZ4_FORCE_O2
static int LZ4_decompress_fast_extDict(const char* source, char* dest, int originalSize,
                                       const void* dictStart, size_t dictSize)
{
    return LZ4_decompress_unsafe_generic(
                (const BYTE*)source, (BYTE*)dest, originalSize,
                0, (const BYTE*)dictStart, dictSize);
}

/* The "double dictionary" mode, for use with e.g. ring buffers: the first part
 * of the dictionary is passed as prefix, and the second via dictStart + dictSize.
 * These routines are used only once, in LZ4_decompress_*_continue().
 */
LZ4_FORCE_INLINE
int LZ4_decompress_safe_doubleDict(const char* source, char* dest, int compressedSize, int maxOutputSize,
                                   size_t prefixSize, const void* dictStart, size_t dictSize)
{
    return LZ4_decompress_generic(source, dest, compressedSize, maxOutputSize,
                                  decode_full_block, usingExtDict,
                                  (BYTE*)dest-prefixSize, (const BYTE*)dictStart, dictSize);
}

/*===== streaming decompression functions =====*/

#if !defined(LZ4_STATIC_LINKING_ONLY_DISABLE_MEMORY_ALLOCATION)
LZ4_streamDecode_t* LZ4_createStreamDecode(void)
{
    LZ4_STATIC_ASSERT(sizeof(LZ4_streamDecode_t) >= sizeof(LZ4_streamDecode_t_internal));
    return (LZ4_streamDecode_t*) ALLOC_AND_ZERO(sizeof(LZ4_streamDecode_t));
}

int LZ4_freeStreamDecode (LZ4_streamDecode_t* LZ4_stream)
{
    if (LZ4_stream == NULL) { return 0; }  /* support free on NULL */
    FREEMEM(LZ4_stream);
    return 0;
}
#endif

/*! LZ4_setStreamDecode() :
 *  Use this function to instruct where to find the dictionary.
 *  This function is not necessary if previous data is still available where it was decoded.
 *  Loading a size of 0 is allowed (same effect as no dictionary).
 * @return : 1 if OK, 0 if error
 */
int LZ4_setStreamDecode (LZ4_streamDecode_t* LZ4_streamDecode, const char* dictionary, int dictSize)
{
    LZ4_streamDecode_t_internal* lz4sd = &LZ4_streamDecode->internal_donotuse;
    if (dictSize < 0) return 0;
    lz4sd->prefixSize = (size_t)dictSize;
    if (dictSize) {
        assert(dictionary != NULL);
        lz4sd->prefixEnd = (const BYTE*) dictionary + dictSize;
    } else {
        lz4sd->prefixEnd = (const BYTE*) dictionary;
    }
    lz4sd->externalDict = NULL;
    lz4sd->extDictSize  = 0;
    return 1;
}

/*! LZ4_decoderRingBufferSize() :
 *  when setting a ring buffer for streaming decompression (optional scenario),
 *  provides the minimum size of this ring buffer
 *  to be compatible with any source respecting maxBlockSize condition.
 *  Note : in a ring buffer scenario,
 *  blocks are presumed decompressed next to each other.
 *  When not enough space remains for next block (remainingSize < maxBlockSize),
 *  decoding resumes from beginning of ring buffer.
 * @return : minimum ring buffer size,
 *           or 0 if there is an error (invalid maxBlockSize).
 */
int LZ4_decoderRingBufferSize(int maxBlockSize)
{
    /* the margin must cover the widest write beyond a copy of the safe loop (16 bytes), minus 2 :
     * LZ4_decompress_generic() avoids the fast loop when history is closer than FASTLOOP_SAFE_DISTANCE,
     * which the current margin (64) never is. The former margin (14) remains valid. */
    LZ4_STATIC_ASSERT(LZ4_DECODER_RING_BUFFER_SIZE(0) - 65536 >= 16 - 2);
    if (maxBlockSize < 0) return 0;
    if (maxBlockSize > LZ4_MAX_INPUT_SIZE) return 0;
    if (maxBlockSize < 16) maxBlockSize = 16;
    return LZ4_DECODER_RING_BUFFER_SIZE(maxBlockSize);
}

/*
*_continue() :
    These decoding functions allow decompression of multiple blocks in "streaming" mode.
    Previously decoded blocks must still be available at the memory position where they were decoded.
    If it's not possible, save the relevant part of decoded data into a safe buffer,
    and indicate where it stands using LZ4_setStreamDecode()
*/
LZ4_FORCE_O2
int LZ4_decompress_safe_continue (LZ4_streamDecode_t* LZ4_streamDecode, const char* source, char* dest, int compressedSize, int maxOutputSize)
{
    LZ4_streamDecode_t_internal* lz4sd = &LZ4_streamDecode->internal_donotuse;
    int result;

    if (lz4sd->prefixSize == 0) {
        /* The first call, no dictionary yet. */
        assert(lz4sd->extDictSize == 0);
        result = LZ4_decompress_safe(source, dest, compressedSize, maxOutputSize);
        if (result <= 0) return result;
        lz4sd->prefixSize = (size_t)result;
        lz4sd->prefixEnd = (BYTE*)dest + result;
    } else if (lz4sd->prefixEnd == (BYTE*)dest) {
        /* They're rolling the current segment. */
        if (lz4sd->prefixSize >= 64 KB - 1)
            result = LZ4_decompress_safe_withPrefix64k(source, dest, compressedSize, maxOutputSize);
        else if (lz4sd->extDictSize == 0)
            result = LZ4_decompress_safe_withSmallPrefix(source, dest, compressedSize, maxOutputSize,
                                                         lz4sd->prefixSize);
        else
            result = LZ4_decompress_safe_doubleDict(source, dest, compressedSize, maxOutputSize,
                                                    lz4sd->prefixSize, lz4sd->externalDict, lz4sd->extDictSize);
        if (result <= 0) return result;
        lz4sd->prefixSize += (size_t)result;
        lz4sd->prefixEnd  += result;
    } else {
        /* The buffer wraps around, or they're switching to another buffer. */
        lz4sd->extDictSize = lz4sd->prefixSize;
        lz4sd->externalDict = lz4sd->prefixEnd - lz4sd->extDictSize;
        result = LZ4_decompress_safe_forceExtDict(source, dest, compressedSize, maxOutputSize,
                                                  lz4sd->externalDict, lz4sd->extDictSize);
        if (result <= 0) return result;
        lz4sd->prefixSize = (size_t)result;
        lz4sd->prefixEnd  = (BYTE*)dest + result;
    }

    return result;
}

LZ4_FORCE_O2 int
LZ4_decompress_fast_continue (LZ4_streamDecode_t* LZ4_streamDecode,
                        const char* source, char* dest, int originalSize)
{
    LZ4_streamDecode_t_internal* const lz4sd =
        (assert(LZ4_streamDecode!=NULL), &LZ4_streamDecode->internal_donotuse);
    int result;

    DEBUGLOG(5, "LZ4_decompress_fast_continue (toDecodeSize=%i)", originalSize);
    assert(originalSize >= 0);

    if (lz4sd->prefixSize == 0) {
        DEBUGLOG(5, "first invocation : no prefix nor extDict");
        assert(lz4sd->extDictSize == 0);
        result = LZ4_decompress_fast(source, dest, originalSize);
        if (result <= 0) return result;
        lz4sd->prefixSize = (size_t)originalSize;
        lz4sd->prefixEnd = (BYTE*)dest + originalSize;
    } else if (lz4sd->prefixEnd == (BYTE*)dest) {
        DEBUGLOG(5, "continue using existing prefix");
        result = LZ4_decompress_unsafe_generic(
                        (const BYTE*)source, (BYTE*)dest, originalSize,
                        lz4sd->prefixSize,
                        lz4sd->externalDict, lz4sd->extDictSize);
        if (result <= 0) return result;
        lz4sd->prefixSize += (size_t)originalSize;
        lz4sd->prefixEnd  += originalSize;
    } else {
        DEBUGLOG(5, "prefix becomes extDict");
        lz4sd->extDictSize = lz4sd->prefixSize;
        lz4sd->externalDict = lz4sd->prefixEnd - lz4sd->extDictSize;
        result = LZ4_decompress_fast_extDict(source, dest, originalSize,
                                             lz4sd->externalDict, lz4sd->extDictSize);
        if (result <= 0) return result;
        lz4sd->prefixSize = (size_t)originalSize;
        lz4sd->prefixEnd  = (BYTE*)dest + originalSize;
    }

    return result;
}


/*
Advanced decoding functions :
*_usingDict() :
    These decoding functions work the same as "_continue" ones,
    the dictionary must be explicitly provided within parameters
*/

int LZ4_decompress_safe_usingDict(const char* source, char* dest, int compressedSize, int maxOutputSize, const char* dictStart, int dictSize)
{
    if (dictSize < 0) return -1;
    if (dictSize==0)
        return LZ4_decompress_safe(source, dest, compressedSize, maxOutputSize);
    if (dictStart+dictSize == dest) {
        if (dictSize >= 64 KB - 1) {
            return LZ4_decompress_safe_withPrefix64k(source, dest, compressedSize, maxOutputSize);
        }
        assert(dictSize >= 0);
        return LZ4_decompress_safe_withSmallPrefix(source, dest, compressedSize, maxOutputSize, (size_t)dictSize);
    }
    assert(dictSize >= 0);
    return LZ4_decompress_safe_forceExtDict(source, dest, compressedSize, maxOutputSize, dictStart, (size_t)dictSize);
}

int LZ4_decompress_safe_partial_usingDict(const char* source, char* dest, int compressedSize, int targetOutputSize, int dstCapacity, const char* dictStart, int dictSize)
{
    if (dictSize < 0) return -1;
    if (dictSize==0)
        return LZ4_decompress_safe_partial(source, dest, compressedSize, targetOutputSize, dstCapacity);
    if (dictStart+dictSize == dest) {
        if (dictSize >= 64 KB - 1) {
            return LZ4_decompress_safe_partial_withPrefix64k(source, dest, compressedSize, targetOutputSize, dstCapacity);
        }
        assert(dictSize >= 0);
        return LZ4_decompress_safe_partial_withSmallPrefix(source, dest, compressedSize, targetOutputSize, dstCapacity, (size_t)dictSize);
    }
    assert(dictSize >= 0);
    return LZ4_decompress_safe_partial_forceExtDict(source, dest, compressedSize, targetOutputSize, dstCapacity, dictStart, (size_t)dictSize);
}

int LZ4_decompress_fast_usingDict(const char* source, char* dest, int originalSize, const char* dictStart, int dictSize)
{
    if (dictSize < 0) return -1;
    if (dictSize==0 || dictStart+dictSize == dest)
        return LZ4_decompress_unsafe_generic(
                        (const BYTE*)source, (BYTE*)dest, originalSize,
                        (size_t)dictSize, NULL, 0);
    assert(dictSize >= 0);
    return LZ4_decompress_fast_extDict(source, dest, originalSize, dictStart, (size_t)dictSize);
}
#ifndef STATIC
EXPORT_SYMBOL(LZ4_decompress_safe);
EXPORT_SYMBOL(LZ4_decompress_safe_partial);
EXPORT_SYMBOL(LZ4_setStreamDecode);
EXPORT_SYMBOL(LZ4_decompress_safe_continue);

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("LZ4 decompressor");
#endif
