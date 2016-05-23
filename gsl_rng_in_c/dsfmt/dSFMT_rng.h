/** 
 * @file dSFMT.h 
 *
 * @brief double precision SIMD oriented Fast Mersenne Twister(dSFMT)
 * pseudorandom number generator based on IEEE 754 format.
 *
 * @author Mutsuo Saito (Hiroshima University)
 * @author Makoto Matsumoto (Hiroshima University)
 *
 * Copyright (C) 2007 Mutsuo Saito, Makoto Matsumoto and Hiroshima
 * University. All rights reserved.
 *
 * The new BSD License is applied to this software.
 * see LICENSE.txt
 *
 * @note We assume that your system has inttypes.h.  If your system
 * doesn't have inttypes.h, you have to typedef uint32_t and uint64_t,
 * and you have to define PRIu64 and PRIx64 in this file as follows:
 * @verbatim
 typedef unsigned int uint32_t
 typedef unsigned long long uint64_t  
 #define PRIu64 "llu"
 #define PRIx64 "llx"
@endverbatim
 * uint32_t must be exactly 32-bit unsigned integer type (no more, no
 * less), and uint64_t must be exactly 64-bit unsigned integer type.
 * PRIu64 and PRIx64 are used for printf function to print 64-bit
 * unsigned int and 64-bit unsigned int in hexadecimal format.
 */

#ifndef DSFMT_H
#define DSFMT_H

#include <stdio.h>
#include "dSFMT-params.h"

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)
  #include <inttypes.h>
#elif defined(_MSC_VER)
  typedef unsigned int uint32_t;
  typedef unsigned long long uint64_t;
  #define inline
#else
  #include <inttypes.h>
  #if defined(__GNUC__)
    #define inline __inline__
  #else
    #define inline
  #endif
#endif

#ifndef PRIu64
  #if defined(_MSC_VER)
    #define PRIu64 "I64u"
    #define PRIx64 "I64x"
  #else
    #define PRIu64 "llu"
    #define PRIx64 "llx"
  #endif
#endif

#ifndef UINT64_C
  #define UINT64_C(v) (v ## ULL) 
#endif

/*------------------------------------------
  128-bit SIMD like data type for standard C
  ------------------------------------------*/

/** 128-bit data structure */
union W128_T {
    uint64_t u[2];
    uint32_t u32[4];
    double d[2];
};

/** 128-bit data type */
typedef union W128_T w128_t;

typedef struct{
    /*
     * Stores the state of the dsfmt generator.
     * Designed to model the MaD0 and tinymt generators.
     */
    /** the 128-bit internal state array */
    w128_t sfmt[SFMT_N + 1];

    /** the double pointer to the 128-bit internal state array */
    double *psfmt64;
    /** index counter to the internal state array as double */
    int sfmt_idx;
    /** a flag: it is 0 if and only if the internal state is not yet
     * initialized. */
    int is_sfmt_initialized;
    /** a period certification vector which certificate the period of
     * 2^{SFMT_MEXP}-1. */
    uint64_t sfmt_pcv[2];
} dsfmt;


double dsfmt_genrand_close1_open2(dsfmt *state);
static double genrand_close_open(dsfmt *state);
//static double genrand_open_close(void);
//static double genrand_open_open(void);

//void fill_array_open_close(double array[], int size);
//void fill_array_close_open(double array[], int size);
//void fill_array_open_open(double array[], int size);
//void fill_array_close1_open2(double array[], int size);
//char *get_idstring(void);
//int get_min_array_size(void);
void dsfmt_init_gen_rand(dsfmt *state, uint32_t seed);
//void init_by_array(uint32_t init_key[], int key_length);

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range [0, 1).
 * init_gen_rand() or init_by_array() must be called before this
 * function.  
 * @return double precision floating point pseudorandom number
 */
inline static double genrand_close_open(dsfmt *state) {
    return dsfmt_genrand_close1_open2(state) - 1.0L;
}

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range (0, 1].
 * init_gen_rand() or init_by_array() must be called before this
 * function.
 * @return double precision floating point pseudorandom number
 */
//inline static double genrand_open_close(void) {
//    return 2.0L - genrand_close1_open2();
//}

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range (0, 1).
 * init_gen_rand() or init_by_array() must be called before this
 * function.
 * @return double precision floating point pseudorandom number
 */
//inline static double genrand_open_open(void) {
//    union {
//	uint64_t u;
//	double d;
//    } conv;
//
//    conv.d = genrand_close1_open2();
//    conv.u |= 1;
//    return conv.d - 1.0L;
//}

#endif /* DSFMT_H */
