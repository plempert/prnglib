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
#include "mad0.h"

int main(int argc, char *argv[])
{
    uint8_t out[256];
    mad0 gen;
    int i;
    uint8_t rb;
    uint32_t ri;
    double rd;

    //initialize the generator using the key "abcd"
    initGenFrKey(&gen, 4, "abcd");

    //generate 256 pseudorandom bytes
    generate(&gen, 256, out);
    printf("\n");
    for (i=0; i<256; i++)
        printf("%02x ",out[i]);
    printf("\n\n");

    //generate 32 pseudorandom bytes
    for (i=0; i<32; i++)
    {
        rb = generateByte(&gen);
        printf("%02x ", rb);
    }
    printf("\n\n");

    //generate 32 pseudorandom 32-bit unsigned integers
    for (i=0; i<32; i++)
    {
        ri = generateUint32(&gen);
        printf("%lu ", ri);
    }
    printf("\n\n");

    //generate 32 pseudorandom double values
    for (i=0; i<32; i++)
    {
        rd = generateDouble(&gen);
        printf("%1.6f ", rd);
    }
    printf("\n\n");

    return 0;
}
