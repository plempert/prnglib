//
// Created by Patrick Lempert on 5/18/16.
//

#ifndef pgl_rng_dsfmt_c
#define pgl_rng_dsfmt_c

#include "../pgl_rng.h"
#include "dSFMT_rng.h"
#include "dSFMT_rng.c"

static const char * name = "dSFMT";
static const int ran_max = 1000;
static const int ran_min = 1;
static inline unsigned long int ran_get (void *vstate);
static double ran_get_double (void *vstate);
static void ran_set (void *state, unsigned long int s);

static inline unsigned long int ran_get (void *vstate){
    printf("Cannot generate integers with dsfmt generator.\n");
    exit(1);
}
static double ran_get_double (void *vstate){
    return genrand_close_open((dsfmt *)vstate);
}
static void ran_set1 (void *vstate, unsigned long int s){
    dsfmt_init_gen_rand((dsfmt *)vstate, (uint32_t)s);
}
static void ran_set2 (void *vstate, int n, const unsigned char * s){
    printf("%s generator does not support a seed string\n", name);
    exit(1);
}
static void ran_set3 (void *vstate, unsigned int * s){
    printf("%s generator does not support a seed array\n", name);
    exit(1);
}
static const gsl_rng_type ran_type = {
        "dSFMT",                /* const char * name; */
        ran_max,                /* unsigned long int max; */
        ran_min,                /* unsigned long int min; */
        sizeof(dsfmt),          /* int size; */
        &ran_set1,              /* void (*set) (void * state, unsigned long int seed); */
        &ran_set2,
        &ran_set3,
        &ran_get,               /* unsigned long int (*get) (void * state); */
        &ran_get_double         /* double (*get_double) (void *state); */
};

const gsl_rng_type *gsl_rng_dsfmt = &ran_type;

#endif