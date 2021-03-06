/** 
 * @file dSFMT.c 
 * @brief double precision SIMD-oriented Fast Mersenne Twister (dSFMT)
 * based on IEEE 754 format.
 *
 * @author Mutsuo Saito (Hiroshima University)
 * @author Makoto Matsumoto (Hiroshima University)
 *
 * Copyright (C) 2007 Mutsuo Saito, Makoto Matsumoto and Hiroshima
 * University. All rights reserved.
 *
 * The new BSD License is applied to this software, see LICENSE.txt
 */

#ifdef HAVE_CONFIG_H
#  include "config.h"
#endif

#include <string.h>
#include <assert.h>
#include "dSFMTnew.h"






#if defined(HAVE_ALTIVEC)
#include "dSFMT-alti.h"
#elif defined(HAVE_SSE2)
#include "dSFMT-sse2.h"
#endif

/**
 * This function simulate a 32-bit array index overlapped to 64-bit
 * array of LITTLE ENDIAN in BIG ENDIAN machine.
 */
#if (defined(__BIG_ENDIAN__) || defined(BIG_ENDIAN)) && !defined(__amd64)
inline  int DSFMT::sfmt_idxof(int i) {
    return i ^ 1;
}
#else
inline int DSFMT::sfmt_idxof(int i) {
    return i;
}
#endif

/**
 * This function simulates SIMD 128-bit left shift by the standard C.
 * The 128-bit integer given in \b in is shifted by (shift * 8) bits.
 * This function simulates the LITTLE ENDIAN SIMD.
 * @param out the output of this function
 * @param in the 128-bit data to be shifted
 * @param shift the shift value
 */
inline void DSFMT::lshift128(w128_pg *out, const w128_pg *in, int shift) {
    out->u[0] = in->u[0] << (shift * 8);
    out->u[1] = in->u[1] << (shift * 8);
    out->u[1] |= in->u[0] >> (64 - shift * 8);
}

/**
 * This function represents the recursion formula.
 * @param r output
 * @param a a 128-bit part of the internal state array
 * @param b a 128-bit part of the internal state array
 * @param c a 128-bit part of the internal state array
 * @param lung a 128-bit part of the internal state array
 */
inline void DSFMT::do_recursion(w128_pg *r, w128_pg *a, w128_pg *b, w128_pg *c,
				w128_pg *lung) {
    w128_pg x;

    lshift128(&x, a, SFMT_SL2);
    r->u[0] = a->u[0] ^ x.u[0] ^ ((b->u[0] >> SFMT_SR1) & SFMT_MSK1) 
	^ (c->u[0] >> SFMT_SR2) ^ (c->u[0] << SFMT_SL1) ^ lung->u[1];
    r->u[1] = a->u[1] ^ x.u[1] ^ ((b->u[1] >> SFMT_SR1) & SFMT_MSK2) 
	^ (c->u[1] >> SFMT_SR2) ^ (c->u[1] << SFMT_SL1) ^ lung->u[0];
    r->u[0] &= SFMT_LOW_MASK;
    r->u[1] &= SFMT_LOW_MASK;
    lung->u[0] ^= r->u[0];
    lung->u[1] ^= r->u[1];
    r->u[0] |= SFMT_HIGH_CONST;
    r->u[1] |= SFMT_HIGH_CONST;
}

#if !defined(HAVE_SSE2)
/**
 * This function converts the double precision floating point numbers which
 * distribute uniformly in the range [1, 2) to those which distribute uniformly
 * in the range [0, 1).
 * @param array array of double precision floating point numbers
 * @param size size of the array
 */
inline void DSFMT::convert_co(w128_pg array[], int size) {
    int i;

    for (i = 0; i < size; i++) {
	array[i].d[0] = array[i].d[0] - 1.0L;
	array[i].d[1] = array[i].d[1] - 1.0L;
    }
}

/**
 * This function converts the double precision floating point numbers which
 * distribute uniformly in the range [1, 2) to those which distribute uniformly
 * in the range (0, 1].
 * @param array array of double precision floating point numbers
 * @param size size of the array
 */
inline void DSFMT::convert_oc(w128_pg array[], int size) {
    int i;

    for (i = 0; i < size; i++) {
	array[i].d[0] = 2.0L - array[i].d[0];
	array[i].d[1] = 2.0L - array[i].d[1];
    }
}

/**
 * This function converts the double precision floating point numbers which
 * distribute uniformly in the range [1, 2) to those which distribute uniformly
 * in the range (0, 1).
 * @param array array of double precision floating point numbers
 * @param size size of the array
 */
inline void DSFMT::convert_oo(w128_pg array[], int size) {
    int i;

    for (i = 0; i < size; i++) {
	array[i].u[0] |= 1;
	array[i].u[1] |= 1;
	array[i].d[0] -= 1.0L;
	array[i].d[1] -= 1.0L;
    }
}
#endif

#if (!defined(HAVE_ALTIVEC)) && (!defined(HAVE_SSE2))
/**
 * This function fills the internal state array with double precision
 * floating point pseudorandom numbers of the IEEE 754 format.
 */
inline void DSFMT::gen_rand_all(void) {
    int i;
    w128_pg lung;

    lung = sfmt[SFMT_N];
    do_recursion(&sfmt[0], &sfmt[0], &sfmt[SFMT_POS1], &sfmt[SFMT_N -1], &lung);
    for (i = 1; i < SFMT_N - SFMT_POS1; i++) {
	do_recursion(&sfmt[i], &sfmt[i], &sfmt[i + SFMT_POS1], &sfmt[i - 1],
		     &lung);
    }
    for (; i < SFMT_N; i++) {
	do_recursion(&sfmt[i], &sfmt[i], &sfmt[i + SFMT_POS1 - SFMT_N],
		     &sfmt[i - 1], &lung);
    }
    sfmt[SFMT_N] = lung;
}

/**
 * This function fills the user-specified array with double precision
 * floating point pseudorandom numbers of the IEEE 754 format.
 * @param array an 128-bit array to be filled by pseudorandom numbers.  
 * @param size number of 128-bit pseudorandom numbers to be generated.
 */
inline void DSFMT::gen_rand_array(w128_pg array[], int size) {
    int i, j;
    w128_pg lung;

    lung = sfmt[SFMT_N];
    do_recursion(&array[0], &sfmt[0], &sfmt[SFMT_POS1], &sfmt[SFMT_N - 1],
		 &lung);
    for (i = 1; i < SFMT_N - SFMT_POS1; i++) {
	do_recursion(&array[i], &sfmt[i], &sfmt[i + SFMT_POS1], &array[i - 1],
		     &lung);
    }
    for (; i < SFMT_N; i++) {
	do_recursion(&array[i], &sfmt[i], &array[i + SFMT_POS1 - SFMT_N],
		     &array[i - 1], &lung);
    }
    for (; i < size - SFMT_N; i++) {
	do_recursion(&array[i], &array[i - SFMT_N],
		     &array[i + SFMT_POS1 - SFMT_N], &array[i - 1], &lung);
    }
    for (j = 0; j < 2 * SFMT_N - size; j++) {
	sfmt[j] = array[j + size - SFMT_N];
    }
    for (; i < size; i++, j++) {
	do_recursion(&array[i], &array[i - SFMT_N],
		     &array[i + SFMT_POS1 - SFMT_N], &array[i - 1], &lung);
	sfmt[j] = array[i];
    }
    sfmt[SFMT_N] = lung;
}
#endif
/**
 * This function represents a function used in the initialization
 * by init_by_array
 * @param x 32-bit integer
 * @return 32-bit integer
 */
 uint32_t DSFMT::dSFMT_ini_func1(uint32_t x) {
    return (x ^ (x >> 27)) * (uint32_t)1664525UL;
}

/**
 * This function represents a function used in the initialization
 * by init_by_array
 * @param x 32-bit integer
 * @return 32-bit integer
 */
 uint32_t DSFMT::dSFMT_ini_func2(uint32_t x) {
    return (x ^ (x >> 27)) * (uint32_t)1566083941UL;
}

/**
 * This function initializes the internal state array to fit the IEEE
 * 754 format.
 */
void DSFMT::initial_mask(void) {
    int i;
    uint64_t *psfmt;

    psfmt = &sfmt[0].u[0];
    for (i = 0; i < (SFMT_N + 1) * 2; i++) {
        psfmt[i] = (psfmt[i] & SFMT_LOW_MASK) | SFMT_HIGH_CONST;
    }
}

/**
 * This function certificate the period of 2^{SFMT_MEXP}-1.
 */
 void DSFMT::period_certification() {
    int inner = 0;
    int i, j;
    uint64_t new_lung[2];
    uint64_t work;
    uint64_t fix[2];

    fix[0] = (((SFMT_HIGH_CONST >> SFMT_SR1) & SFMT_MSK2) 
	      ^ (SFMT_HIGH_CONST >> SFMT_SR2)) | SFMT_HIGH_CONST;
    fix[1] = (((SFMT_HIGH_CONST >> SFMT_SR1) & SFMT_MSK1) 
	      ^ (SFMT_HIGH_CONST >> SFMT_SR2)) | SFMT_HIGH_CONST;
    fix[0] = fix[0] ^ (SFMT_HIGH_CONST >> (64 - 8 * SFMT_SL2));
    new_lung[0] = sfmt[SFMT_N].u[0] ^ fix[0];
    new_lung[1] = sfmt[SFMT_N].u[1] ^ fix[1];
    for (i = 0; i < 2; i++) {
	work = new_lung[i] & sfmt_pcv[i];
	for (j = 0; j < 52; j++) {
	    inner ^= work & 1;
	    work = work >> 1;
	}
    }
    /* check OK */
    if (inner == 1) {
	return;
    }
    /* check NG, and modification */
    for (i = 0; i < 2; i++) {
	work = 1;
	for (j = 0; j < 52; j++) {
	    if ((work & sfmt_pcv[i]) != 0) {
		sfmt[SFMT_N].u[i] ^= work;
		return;
	    }
	    work = work << 1;
	}
    }
}

/*----------------
  PUBLIC FUNCTIONS
  ----------------*/
/**
 * This function returns the identification string.  The string shows
 * the Mersenne exponent, and all parameters of this generator.
 * @return id string.
 */
char *DSFMT::get_idstring(void) {
    return SFMT_IDSTR;
}

/**
 * This function returns the minimum size of array used for \b
 * fill_array functions.
 * @return minimum size of array used for fill_array functions.
 */
int DSFMT::get_min_array_size(void) {
    return SFMT_N64;
}

/**
 * This function generates and returns double precision pseudorandom
 * number which distributes uniformly in the range [1, 2).  This is
 * the primitive and faster than generating numbers in other ranges.
 * init_gen_rand() or init_by_array() must be called before this
 * function.
 * @return double precision floating point pseudorandom number
 */
inline double DSFMT::genrand_close1_open2(void) {
    double r;

    assert(is_sfmt_initialized);

    if (sfmt_idx >= SFMT_N * 2) {
	gen_rand_all();
	sfmt_idx = 0;
    }
    r = psfmt64[sfmt_idx++];
    return r;
}

/**
 * This function generates double precision floating point
 * pseudorandom numbers which distribute in the range [1, 2) to the
 * specified array[] by one call. The number of pseudorandom numbers
 * is specified by the argument \b size, which must be at least (SFMT_MEXP
 * / 128) * 2 and a multiple of two.  The function
 * get_min_array_size() returns this minimum size.  The generation by
 * this function is much faster than the following fill_array_xxx functions.
 *
 * For initialization, init_gen_rand() or init_by_array() must be called
 * before the first call of this function. This function can not be
 * used after calling genrand_xxx functions, without initialization.
 *
 * @param array an array where pseudorandom numbers are filled
 * by this function.  The pointer to the array must be "aligned"
 * (namely, must be a multiple of 16) in the SIMD version, since it
 * refers to the address of a 128-bit integer.  In the standard C
 * version, the pointer is arbitrary.
 *
 * @param size the number of 64-bit pseudorandom integers to be
 * generated.  size must be a multiple of 2, and greater than or equal
 * to (SFMT_MEXP / 128) * 2.
 *
 * @note \b memalign or \b posix_memalign is available to get aligned
 * memory. Mac OSX doesn't have these functions, but \b malloc of OSX
 * returns the pointer to the aligned memory block.
 */
void DSFMT::fill_array_close1_open2(double array[], int size) {
    assert(size % 2 == 0);
    assert(size >= SFMT_N64);
    gen_rand_array((w128_pg *)array, size / 2);
}

/**
 * This function generates double precision floating point
 * pseudorandom numbers which distribute in the range (0, 1] to the
 * specified array[] by one call. This function is the same as
 * fill_array_close1_open2() except the distribution range.
 *
 * @param array an array where pseudorandom numbers are filled
 * by this function.
 * @param size the number of pseudorandom numbers to be generated.
 * see also \sa fill_array_close1_open2()
 */
void DSFMT::fill_array_open_close(double array[], int size) {
    assert(size % 2 == 0);
    assert(size >= SFMT_N64);
    gen_rand_array((w128_pg *)array, size / 2);
    convert_oc((w128_pg *)array, size / 2);
}

/**
 * This function generates double precision floating point
 * pseudorandom numbers which distribute in the range [0, 1) to the
 * specified array[] by one call. This function is the same as
 * fill_array_close1_open2() except the distribution range.
 *
 * @param array an array where pseudorandom numbers are filled
 * by this function.
 * @param size the number of pseudorandom numbers to be generated.
 * see also \sa fill_array_close1_open2()
 */
void DSFMT::fill_array_close_open(double array[], int size) {
    assert(size % 2 == 0);
    assert(size >= SFMT_N64);
    gen_rand_array((w128_pg *)array, size / 2);
    convert_co((w128_pg *)array, size / 2);
}

/**
 * This function generates double precision floating point
 * pseudorandom numbers which distribute in the range (0, 1) to the
 * specified array[] by one call. This function is the same as
 * fill_array_close1_open2() except the distribution range.
 *
 * @param array an array where pseudorandom numbers are filled
 * by this function.
 * @param size the number of pseudorandom numbers to be generated.
 * see also \sa fill_array_close1_open2()
 */
void DSFMT::fill_array_open_open(double array[], int size) {
    assert(size % 2 == 0);
    assert(size >= SFMT_N64);
    gen_rand_array((w128_pg *)array, size / 2);
    convert_oo((w128_pg *)array, size / 2);
}

/**
 * This function initializes the internal state array with a 32-bit
 * integer seed.
 * @param seed a 32-bit integer used as the seed.
 */
void DSFMT::init_gen_rand(uint32_t seed) {
    int i;
    uint32_t *psfmt;

    psfmt = &sfmt[0].u32[0];
    psfmt[sfmt_idxof(0)] = seed;
    for (i = 1; i < (SFMT_N + 1) * 4; i++) {
        psfmt[sfmt_idxof(i)] = 1812433253UL 
	    * (psfmt[sfmt_idxof(i - 1)] 
	       ^ (psfmt[sfmt_idxof(i - 1)] >> 30)) + i;
    }
    initial_mask();
    period_certification();
    sfmt_idx = SFMT_N64;
    is_sfmt_initialized = 1;
#ifdef HAVE_SSE2
    setup_const();
#endif
}

/**
 * This function initializes the internal state array,
 * with an array of 32-bit integers used as the seeds
 * @param init_key the array of 32-bit integers, used as a seed.
 * @param key_length the length of init_key.
 */
void DSFMT::init_by_array(uint32_t init_key[], int key_length) {
    int i, j, count;
    uint32_t r;
    uint32_t *psfmt32;
    int lag;
    int mid;
    int size = (SFMT_N + 1) * 4;	/* pulmonary */


    if (size >= 623) {
	lag = 11;
    } else if (size >= 68) {
	lag = 7;
    } else if (size >= 39) {
	lag = 5;
    } else {
	lag = 3;
    }
    mid = (size - lag) / 2;

    psfmt32 = &sfmt[0].u32[0];
    memset(sfmt, 0x8b, sizeof(sfmt));
    if (key_length + 1 > size) {
	count = key_length + 1;
    } else {
	count = size;
    }
    r = dSFMT_ini_func1(psfmt32[sfmt_idxof(0)] ^ psfmt32[sfmt_idxof(mid % size)] 
		  ^ psfmt32[sfmt_idxof((size - 1) % size)]);
    psfmt32[sfmt_idxof(mid % size)] += r;
    r += key_length;
    psfmt32[sfmt_idxof((mid + lag) % size)] += r;
    psfmt32[sfmt_idxof(0)] = r;
    i = 1;
    count--;
    for (i = 1, j = 0; (j < count) && (j < key_length); j++) {
	r = dSFMT_ini_func1(psfmt32[sfmt_idxof(i)] 
		      ^ psfmt32[sfmt_idxof((i + mid) % size)] 
		      ^ psfmt32[sfmt_idxof((i + size - 1) % size)]);
	psfmt32[sfmt_idxof((i + mid) % size)] += r;
	r += init_key[j] + i;
	psfmt32[sfmt_idxof((i + mid + lag) % size)] += r;
	psfmt32[sfmt_idxof(i)] = r;
	i = (i + 1) % size;
    }
    for (; j < count; j++) {
	r = dSFMT_ini_func1(psfmt32[sfmt_idxof(i)] 
		      ^ psfmt32[sfmt_idxof((i + mid) % size)] 
		      ^ psfmt32[sfmt_idxof((i + size - 1) % size)]);
	psfmt32[sfmt_idxof((i + mid) % size)] += r;
	r += i;
	psfmt32[sfmt_idxof((i + mid + lag) % size)] += r;
	psfmt32[sfmt_idxof(i)] = r;
	i = (i + 1) % size;
    }
    for (j = 0; j < size; j++) {
	r = dSFMT_ini_func2(psfmt32[sfmt_idxof(i)] 
		      + psfmt32[sfmt_idxof((i + mid) % size)] 
		      + psfmt32[sfmt_idxof((i + size - 1) % size)]);
	psfmt32[sfmt_idxof((i + mid) % size)] ^= r;
	r -= i;
	psfmt32[sfmt_idxof((i + mid + lag) % size)] ^= r;
	psfmt32[sfmt_idxof(i)] = r;
	i = (i + 1) % size;
    }
    initial_mask();
    period_certification();
    sfmt_idx = SFMT_N64;
    is_sfmt_initialized = 1;
#ifdef HAVE_SSE2
    setup_const();
#endif
}
