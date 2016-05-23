//
// Created by Patrick Lempert on 5/18/16.
//

#ifndef pgl_rng_well_c
#define pgl_rng_well_c

#include "well_rng.h"
#include "../pgl_rng.h"
#include <stdio.h>
#include <stdlib.h>

static const char * name = "WELL";
static const int ran_max = 1000;
static const int ran_min = 1;
static inline unsigned long int ran_get (void *vstate);
static double ran_get_double (void *vstate);
static void ran_set (void *state, unsigned long int s);

static inline unsigned long int ran_get (void *vstate){
    printf("The well generator cannot generate integers.\n");
    exit(1);
}
static double ran_get_double (void *vstate){
    return rngWELLRNG1024a((well *)vstate);
}
static void ran_set1 (void *vstate, unsigned long int s){
    printf("%s generator does not support an seed int\n", name);
    exit(1);
}
static void ran_set2 (void *vstate, int n, const unsigned char * s){
    printf("%s generator does not support a seed string\n", name);
    exit(1);
}
static void ran_set3 (void *vstate, unsigned int * s){
    rngInitWELLRNG1024a((well *)vstate, s);
}


static const gsl_rng_type ran_type = {
        "well",                 /* const char * name; */
        ran_max,                /* unsigned long int max; */
        ran_min,                /* unsigned long int min; */
        sizeof(well),           /* int size; */
        &ran_set1,              /* void (*set) (void * state, unsigned long int seed); */
        &ran_set2,
        &ran_set3,
        &ran_get,               /* unsigned long int (*get) (void * state); */
        &ran_get_double         /* double (*get_double) (void *state); */
};

const gsl_rng_type *gsl_rng_well = &ran_type;

#endif