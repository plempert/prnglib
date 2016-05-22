#ifndef PGL_RNG_H
#define PGL_RNG_H

#include <stdlib.h> /* malloc, exit, calloc, free, */
#include <stdio.h>  /* printf */

typedef struct{
    const char * name;
    unsigned long int max;
    unsigned long int min;
    int size;
    void (*set) (void * state, unsigned long int seed);
    unsigned long int (*get) (void * state);
    double (*get_double) (void *state);
} gsl_rng_type;

typedef struct{
    void * state;
    const gsl_rng_type * type;
//    int seeded;
} gsl_rng;

extern const gsl_rng_type * gsl_rng_dsfmt;     
extern const gsl_rng_type * gsl_rng_mad0;
extern const gsl_rng_type * gsl_rng_tinymt;
extern const gsl_rng_type * gsl_rng_well;
extern const gsl_rng_type * gsl_rng_xorgens;

//#include "dsfmt/pgl_rng_dsfmt.c"
//#include "mad0/pgl_rng_mad0.c"
//#include "tinymt/pgl_rng_tinymt.c"
//#include "well/pgl_rng_well.c"
//#include "xorgens/pgl_rng_xorgens.c"

unsigned long int gsl_rng_default_seed;
void gsl_rng_set(const gsl_rng * r, unsigned long int seed);


gsl_rng * gsl_rng_alloc(const gsl_rng_type * T);
void gsl_rng_set (const gsl_rng * r, unsigned long int seed);
void gsl_rng_free (gsl_rng * r);
unsigned long int gsl_rng_get (const gsl_rng * r);
double gsl_rng_uniform (const gsl_rng * r);
double gsl_rng_uniform_pos (const gsl_rng * r);

#endif