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

#ifndef mad0_h
#define mad0_h

/* Exact-width integer types */

#ifndef __int8_t_defined
    #define __int8_t_defined
    typedef signed char int8_t;
    typedef short int16_t;
    typedef int int32_t;
    typedef long long int64_t;
#endif

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
#ifndef __uint32_t_defined
    #define __uint32_t_defined
    typedef unsigned int uint32_t;
#endif
typedef unsigned long long uint64_t;

#if defined(_MSC_VER)   //MSFT Virtual C++
    #include <stdlib.h>
    #define strtoull _strtoui64
#endif

/* generator struct */
typedef struct {
    uint64_t a,b,c,d;
    uint8_t S[256];
    uint8_t T[512];
    uint16_t pos;   //current output position w.r.t. T (0 means the output buffer T is empty)
} mad0;

/*
 * It takes 2 steps to use the generator.
 * 1) Initialize a generator variable, e.g.,
 *      mad0 gen;                          //declare a generator variable gen
 *      initGenFrKey(&gen, 5, "12345");    //initialize gen using the key "12345", whose size is 5
 *    Note that each generator instance is uniquely identified by a generator
 *    variable of the type mad0. You can instantiate multiple generators by
 *    initializing multiple generator variables, e.g.,
 *      mad0 gen1, gen2;
 *      initGenFrKey(&gen1, 5, "12345");
 *      initGenFrKey(&gen2, 4, "abcd");
 * 2) Call one of the four generate functions and pass the address of the initialized
 *    generator variable as the first parameter to tell which generator instance should
 *    be used in the pseudorandom number generation.
 */

void initGenFrKey(mad0 *gen, const uint8_t szKey, const uint8_t *key);
//generator initialization function
//input:
//  gen   -- a pointer of type mad0
//  szKey -- size of key (in byte), must be <= 64
//  key   -- key string
//return:
//  none

void generate(mad0 *gen, uint16_t szRnd, uint8_t *pOut);
//This function generates a pseudorandom sequence.
//input:
//  gen   -- a pointer of type mad0
//  szRnd -- size (in byte) of the pseudorandom sequence to be generated
//  pOut  -- the output buffer for saving the pseudorandom sequence
//return:
//  none

uint8_t generateByte(mad0 *gen);
//This function generates one pseudorandom byte.
//input:
//  gen   -- a pointer of type mad0
//return:
//  a pseudorandom byte

uint32_t generateUint32(mad0 *gen);
//This function generates one pseudorandom unsigned 32-bit integer.
//input:
//  gen   -- a pointer of type mad0
//return:
//  a pseudorandom unsigned 32-bit integer

double generateDouble(mad0 *gen);
//This function generates one pseudorandom double value on [0,1].
//input:
//  gen   -- a pointer of type mad0
//return:
//  a pseudorandom double value on [0,1]

const char *engineInfo();
//a helper function for identifying the generator engine in the MaD family

#endif
