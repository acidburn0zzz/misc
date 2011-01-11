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
#ifndef __MD5_H__
#define __MD5_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        uint32_t A, B, C, D;
        uint32_t nblocks;
        unsigned char buf[64];
        int count;
    } md5_ctx;

#if defined(__GNUC__) && defined(__i386__)
    static uint32_t rol(uint32_t x, int n) {
        __asm__("roll %%cl,%0"
        :"=r" (x)
                        :"0" (x),"c" (n));
        return x;
    }
#else
#define rol(x,n) ( ((x) << (n)) | ((x) >> (32-(n))) )
#endif

#define FF(b, c, d) (d ^ (b & (c ^ d)))
#define FG(b, c, d) FF (d, b, c)
#define FH(b, c, d) (b ^ c ^ d)
#define FI(b, c, d) (c ^ (b | ~d))

    void md5_begin(md5_ctx *ctx);
    void md5_transform(unsigned char *data, md5_ctx *ctx);
    void md5_hash(unsigned char *inbuf, unsigned int inlen, md5_ctx *ctx);
    void md5_end(md5_ctx *ctx);

#ifdef __cplusplus
}
#endif

#endif /*__MD5_H__*/
