//
// Created by Patrick Lempert on 5/18/16.
//

#ifndef pgl_rng_tinymt_c
#define pgl_rng_tinymt_c

#include "tinymt32.h"
#include "tinymt32.c"
#include "../pgl_rng.h"

static const int ran_max = 1000;
static const int ran_min = 1;
static inline unsigned long int ran_get (void *vstate);
static double ran_get_double (void *vstate);
static void ran_set (void *state, unsigned long int s);

static inline unsigned long int ran_get (void *vstate){
    return (unsigned long int)tinymt32_generate_uint32((tinymt32_t *)vstate);
}
static double ran_get_double (void *vstate){
    return (double)tinymt32_generate_float((tinymt32_t *)vstate);
}
static void ran_set (void *vstate, unsigned long int s){
    tinymt32_init((tinymt32_t *)vstate, (uint32_t) s);
}
static const gsl_rng_type ran_type = {
        "tinymt",               /* const char * name; */
        ran_max,                /* unsigned long int max; */
        ran_min,                /* unsigned long int min; */
        sizeof(tinymt32_t),     /* int size; */
        &ran_set,               /* void (*set) (void * state, unsigned long int seed); */
        &ran_get,               /* unsigned long int (*get) (void * state); */
        &ran_get_double         /* double (*get_double) (void *state); */
};

const gsl_rng_type *gsl_rng_tinymt = &ran_type;

#endif