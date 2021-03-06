//
// Created by Patrick Lempert on 5/18/16.
//

#ifndef pgl_rng_mad0_c
#define pgl_rng_mad0_c

#include "mad0.h"
#include "mad0.c"
#include "../pgl_rng.h"

static const char * name = "MaD0";
static const int ran_max = 100000;
static const int ran_min = 1;
static inline unsigned long int ran_get (void *vstate);
static double ran_get_double (void *vstate);
static void ran_set (void *state, unsigned long int s);

static inline unsigned long int ran_get (void *vstate){
    return (unsigned long int)generateUint32((mad0 *)vstate);
}
static double ran_get_double (void *vstate){
    return generateDouble((mad0 *)vstate);
}
static void ran_set1 (void *vstate, unsigned long int s){
    printf("%s generator does not support a seed int\n", name);
    exit(1);
}
static void ran_set2 (void *vstate, int n, const unsigned char * s){
    initGenFrKey((mad0 *)vstate, n, s);
}
static void ran_set3 (void *vstate, unsigned int * s){
    printf("%s generator does not support a seed array\n", name);
    exit(1);
}
static const gsl_rng_type ran_type = {
    "MaD0",                 /* const char * name; */
    ran_max,                /* unsigned long int max; */
    ran_min,                /* unsigned long int min; */
    sizeof(mad0),           /* int size; */
    &ran_set1,              /* void (*set) (void * state, unsigned long int seed); */
    &ran_set2,
    &ran_set3,
    &ran_get,               /* unsigned long int (*get) (void * state); */
    &ran_get_double         /* double (*get_double) (void *state); */
};

const gsl_rng_type *gsl_rng_mad0 = &ran_type;

#endif
