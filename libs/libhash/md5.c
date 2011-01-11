/***************************************************************************
 * Copyright (C) 2009 Lemay, Mathieu                                       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             *
 *                                                                         *
 * You can contact the original author at acidrain1@gmail.com              *
 ***************************************************************************/
#include <stdio.h>
#include <string.h>

#include "md5.h"

void md5_begin(md5_ctx *ctx) {
    ctx->A = 0x67452301;
    ctx->B = 0xefcdab89;
    ctx->C = 0x98badcfe;
    ctx->D = 0x10325476;

    ctx->nblocks = 0;
    ctx->count = 0;
}

void md5_transform(unsigned char *data, md5_ctx *ctx) {
    uint32_t correct_words[16];
    uint32_t A = ctx->A;
    uint32_t B = ctx->B;
    uint32_t C = ctx->C;
    uint32_t D = ctx->D;
    uint32_t *cwp = correct_words;

#ifdef BIG_ENDIAN_HOST
    {
        int i;
        unsigned char *p2, *p1;

        for (i = 0, p1 = data, p2 = (unsigned char*)correct_words; i < 16; i++, p2 += 4)  {
            p2[3] = *p1++;
            p2[2] = *p1++;
            p2[1] = *p1++;
            p2[0] = *p1++;
        }
    }
#else
    memcpy (correct_words, data, 64);
#endif

#define OP(a, b, c, d, s, T) \
        a += FF (b, c, d) + (*cwp++) + T; \
        a = rol(a, s); \
        a += b;

    /* Round 1.  */
    OP (A, B, C, D,  7, 0xd76aa478);
    OP (D, A, B, C, 12, 0xe8c7b756);
    OP (C, D, A, B, 17, 0x242070db);
    OP (B, C, D, A, 22, 0xc1bdceee);
    OP (A, B, C, D,  7, 0xf57c0faf);
    OP (D, A, B, C, 12, 0x4787c62a);
    OP (C, D, A, B, 17, 0xa8304613);
    OP (B, C, D, A, 22, 0xfd469501);
    OP (A, B, C, D,  7, 0x698098d8);
    OP (D, A, B, C, 12, 0x8b44f7af);
    OP (C, D, A, B, 17, 0xffff5bb1);
    OP (B, C, D, A, 22, 0x895cd7be);
    OP (A, B, C, D,  7, 0x6b901122);
    OP (D, A, B, C, 12, 0xfd987193);
    OP (C, D, A, B, 17, 0xa679438e);
    OP (B, C, D, A, 22, 0x49b40821);

#undef OP
#define OP(f, a, b, c, d, k, s, T) \
        a += f (b, c, d) + correct_words[k] + T; \
        a = rol(a, s); \
        a += b;

    /* Round 2.  */
    OP (FG, A, B, C, D,  1,  5, 0xf61e2562);
    OP (FG, D, A, B, C,  6,  9, 0xc040b340);
    OP (FG, C, D, A, B, 11, 14, 0x265e5a51);
    OP (FG, B, C, D, A,  0, 20, 0xe9b6c7aa);
    OP (FG, A, B, C, D,  5,  5, 0xd62f105d);
    OP (FG, D, A, B, C, 10,  9, 0x02441453);
    OP (FG, C, D, A, B, 15, 14, 0xd8a1e681);
    OP (FG, B, C, D, A,  4, 20, 0xe7d3fbc8);
    OP (FG, A, B, C, D,  9,  5, 0x21e1cde6);
    OP (FG, D, A, B, C, 14,  9, 0xc33707d6);
    OP (FG, C, D, A, B,  3, 14, 0xf4d50d87);
    OP (FG, B, C, D, A,  8, 20, 0x455a14ed);
    OP (FG, A, B, C, D, 13,  5, 0xa9e3e905);
    OP (FG, D, A, B, C,  2,  9, 0xfcefa3f8);
    OP (FG, C, D, A, B,  7, 14, 0x676f02d9);
    OP (FG, B, C, D, A, 12, 20, 0x8d2a4c8a);

    /* Round 3.  */
    OP (FH, A, B, C, D,  5,  4, 0xfffa3942);
    OP (FH, D, A, B, C,  8, 11, 0x8771f681);
    OP (FH, C, D, A, B, 11, 16, 0x6d9d6122);
    OP (FH, B, C, D, A, 14, 23, 0xfde5380c);
    OP (FH, A, B, C, D,  1,  4, 0xa4beea44);
    OP (FH, D, A, B, C,  4, 11, 0x4bdecfa9);
    OP (FH, C, D, A, B,  7, 16, 0xf6bb4b60);
    OP (FH, B, C, D, A, 10, 23, 0xbebfbc70);
    OP (FH, A, B, C, D, 13,  4, 0x289b7ec6);
    OP (FH, D, A, B, C,  0, 11, 0xeaa127fa);
    OP (FH, C, D, A, B,  3, 16, 0xd4ef3085);
    OP (FH, B, C, D, A,  6, 23, 0x04881d05);
    OP (FH, A, B, C, D,  9,  4, 0xd9d4d039);
    OP (FH, D, A, B, C, 12, 11, 0xe6db99e5);
    OP (FH, C, D, A, B, 15, 16, 0x1fa27cf8);
    OP (FH, B, C, D, A,  2, 23, 0xc4ac5665);

    /* Round 4.  */
    OP (FI, A, B, C, D,  0,  6, 0xf4292244);
    OP (FI, D, A, B, C,  7, 10, 0x432aff97);
    OP (FI, C, D, A, B, 14, 15, 0xab9423a7);
    OP (FI, B, C, D, A,  5, 21, 0xfc93a039);
    OP (FI, A, B, C, D, 12,  6, 0x655b59c3);
    OP (FI, D, A, B, C,  3, 10, 0x8f0ccc92);
    OP (FI, C, D, A, B, 10, 15, 0xffeff47d);
    OP (FI, B, C, D, A,  1, 21, 0x85845dd1);
    OP (FI, A, B, C, D,  8,  6, 0x6fa87e4f);
    OP (FI, D, A, B, C, 15, 10, 0xfe2ce6e0);
    OP (FI, C, D, A, B,  6, 15, 0xa3014314);
    OP (FI, B, C, D, A, 13, 21, 0x4e0811a1);
    OP (FI, A, B, C, D,  4,  6, 0xf7537e82);
    OP (FI, D, A, B, C, 11, 10, 0xbd3af235);
    OP (FI, C, D, A, B,  2, 15, 0x2ad7d2bb);
    OP (FI, B, C, D, A,  9, 21, 0xeb86d391);

    ctx->A += A;
    ctx->B += B;
    ctx->C += C;
    ctx->D += D;
}

void md5_hash(unsigned char *inbuf, unsigned int inlen, md5_ctx *ctx) {
    if (ctx->count == 64) { /* flush the buffer */
        md5_transform(ctx->buf, ctx);
        ctx->count = 0;
        ctx->nblocks++;
    }

    if (!inbuf) {
        return;
    }

    if (ctx->count) {
        for (; inlen && ctx->count < 64; inlen--) {
            ctx->buf[ctx->count++] = *inbuf++;
        }

        md5_hash(NULL, 0, ctx);
        if (!inlen) {
            return;
        }
    }

    while (inlen >= 64)  {
        md5_transform(inbuf, ctx);
        ctx->count = 0;
        ctx->nblocks++;
        inlen -= 64;
        inbuf += 64;
    }

    for (; inlen && ctx->count < 64; inlen--) {
        ctx->buf[ctx->count++] = *inbuf++;
    }
}

void md5_end(md5_ctx *ctx) {
    uint32_t t, msb, lsb;
    unsigned char *p;

    md5_hash(NULL, 0, ctx); /* flush */;

    msb = 0;
    t = ctx->nblocks;

    if ((lsb = t << 6) < t) { /* multiply by 64 to make a unsigned char count */
        msb++;
    }

    msb += t >> 26;
    t = lsb;

    if ((lsb = t + ctx->count) < t) { /* add the count */
        msb++;
    }

    t = lsb;

    if ((lsb = t << 3) < t) { /* multiply by 8 to make a bit count */
        msb++;
    }

    msb += t >> 29;

    if (ctx->count < 56) { /* enough room */
        ctx->buf[ctx->count++] = 0x80; /* pad */

        while (ctx->count < 56) {
            ctx->buf[ctx->count++] = 0; /* pad */
        }
    } else {/* need one extra block */
        ctx->buf[ctx->count++] = 0x80; /* pad character */

        while (ctx->count < 64) {
            ctx->buf[ctx->count++] = 0;
        }

        md5_hash(NULL, 0, ctx);  /* flush */;

        memset(ctx->buf, 0, 56); /* fill next block with zeroes */
    }

    /* append the 64 bit count */
    ctx->buf[56] = lsb;
    ctx->buf[57] = lsb >>  8;
    ctx->buf[58] = lsb >> 16;
    ctx->buf[59] = lsb >> 24;
    ctx->buf[60] = msb;
    ctx->buf[61] = msb >>  8;
    ctx->buf[62] = msb >> 16;
    ctx->buf[63] = msb >> 24;

    md5_transform(ctx->buf, ctx);

    p = ctx->buf;

#ifdef BIG_ENDIAN_HOST
#define X(a) do { *p++ = ctx->##a; *p++ = ctx->##a >> 8;      \
              *p++ = ctx->##a >> 16; *p++ = ctx->##a >> 24; } while(0)
#else
#define X(a) do { *(uint32_t*)p = (*ctx).a ; p += 4; } while(0)
#endif

    X(A);
    X(B);
    X(C);
    X(D);

#undef X
}
