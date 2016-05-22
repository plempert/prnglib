/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *               MaD0 Pseudorandom Number Generator                          *
 * ------------------------------------------------------------------------- *
 * Copyright (c) 2016 Jie Li, Jianliang Zheng, Paula Whitlock,               *
 * All Rights Reserved.                                                      *
 * Contact: jie.li@farmingdale.edu                                           *
 * 																			 *
 * Redistribution and use in source and binary forms, with or without        *
 * modification, are permitted provided that the following conditions        *
 * are met:                                                                  *
 *                                                                           *
 *   1. Redistributions of source code must retain the above copyright       *
 *      notice, this list of conditions and the following disclaimer.        *
 *                                                                           *
 *   2. Redistributions in binary form must reproduce the above copyright    *
 *      notice, this list of conditions and the following disclaimer in the  *
 *      documentation and/or other materials provided with the distribution. *
 *                                                                           *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY        *
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED *
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE    *
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDERS BE LIABLE FOR ANY     *
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL        *
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS   *
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)     *
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,       *
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING     *
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE        *
 * POSSIBILITY OF SUCH DAMAGE.                                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <string.h>
#include "mad0.h"

/* initialize the generator with a key */
void initGenFrKey(mad0 *gen, const uint8_t szKey, const uint8_t *key)
{
    register uint8_t i,j,k,m,n,*S,*p;
    register uint16_t cnt,r;
    uint64_t abcd[4];

    S = gen->S;

    //key scheduling
    i = 0;
    do {
        S[i] = i;
    } while (++i != 0);
    cnt = 320;
    for (j=0,k=0,r=0; r<cnt; i++,r++)
    {
        m = S[i];
        j += m + key[r % szKey];
        k ^= j;
        S[i] = S[j];
        S[j] = S[k];
        S[k] = m;
    }
    i = j + k;

    //populate a, b, c, and d
    p = (uint8_t *)abcd;
    cnt = 8;
    while (cnt-- != 0)
    {
        i++;
        k ^= j += m = S[i];
        S[i] = n = S[j];
        S[j] = m;
        n += m;
        m += S[k];
        *p++ = S[m];
        *p++ = S[n];
        *p++ = S[m^j];
        *p++ = S[n^k];
    }
    gen->a = abcd[0];
    gen->b = abcd[1];
    gen->c = abcd[2];
    gen->d = abcd[3];

    //mark the output buffer as empty
    gen->pos = 0;
}

/* generate a block of pseudorandom bytes */
void generate(mad0 *gen, uint16_t szRnd, uint8_t *pOut)
{
    uint16_t pos,szProc;
    register uint8_t i;
    register uint64_t a,b,c,d,ta,tb,*S,*T;

    S = (uint64_t *)gen->S;
    a = gen->a;
    b = gen->b;
    c = gen->c;
    d = gen->d;

    while (szRnd > 0)
    {
        //refresh the buffer if needed
        if (gen->pos == 0)
        {
            //use user output buffer directly
            T = (uint64_t *)pOut;
            while (szRnd >= 512)
            {
                ta = a += c;
                tb = b += d;
                for (i=0;i<32;i++)
                {
                    *T++ = c ^= S[i] + a;
                    c += ta ^ tb;
                    d ^= c + b;
                    ta = (ta << 3)|(ta >> 61);
                    *T++ = d += ta ^ tb;
                    S[i] = d;
                    tb = (tb >> 5)|(tb << 59);
                }
                szRnd -= 512;
            }
            if (szRnd == 0)
                break;
            pOut = (uint8_t *)T;

            //use internal output buffer
            ta = a += c;
            tb = b += d;
            T = (uint64_t *)gen->T;
            for (i=0;i<32;i++)
            {
                *T++ = c ^= S[i] + a;
                c += ta ^ tb;
                d ^= c + b;
                ta = (ta << 3)|(ta >> 61);
                *T++ = d += ta ^ tb;
                S[i] = d;
                tb = (tb >> 5)|(tb << 59);
            }
        }

        pos = gen->pos;
        if ((szProc = 512 - gen->pos) <= szRnd)
        {
            gen->pos = 0;
            szRnd -= szProc;
        }
        else
        {
            szProc = szRnd;
            gen->pos += szRnd;
            szRnd = 0;
        }

        //process szProc bytes
        memcpy(pOut, gen->T + pos, szProc);
        pOut += szProc;
    }

    gen->a = a;
    gen->b = b;
    gen->c = c;
    gen->d = d;
}

/* generate a pseudorandom byte */
uint8_t generateByte(mad0 *gen)
{
    uint8_t v;
    generate(gen, 1, &v);
    return v;
}

/* generate a pseudorandom 32-bit unsigned integer */
uint32_t generateUint32(mad0 *gen)
{
    uint32_t v;
    generate(gen, 4, (uint8_t *)&v);
    return v;
}

/* generate a pseudorandom double value on [0,1] */
double generateDouble(mad0 *gen)
{
    const double MAXUINT32 = 4294967295.0;
    uint32_t v;
    generate(gen, 4, (uint8_t *)&v);
    return v / MAXUINT32;
}

const char *engineInfo()
{
    return "MaD0";
}
