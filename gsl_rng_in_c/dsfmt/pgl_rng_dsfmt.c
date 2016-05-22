//
// Created by Patrick Lempert on 5/18/16.
//

#ifndef pgl_rng_dsfmt_c
#define pgl_rng_dsfmt_c

#include "../pgl_rng.h"

static int ran_max = 1000;
static int ran_min = 1;
static inline unsigned long int ran_get (void *vstate);
static double ran_get_double (void *vstate);
static void ran_set (void *state, unsigned long int s);

static inline unsigned long int ran_get (void *vstate){}
static double ran_get_double (void *vstate){}
static void ran_set (void *vstate, unsigned long int s){}
static const gsl_rng_type ran_type = {
        "dsfmt",                 /* name */
        ran_max,                /* RAND_MAX */
        ran_min,                /* RAND_MIN */
        sizeof (ran_state_t),
        &ran_set,
        &ran_get,
        &ran_get_double
};

const gsl_rng_type *gsl_rng_dsfmt = &ran_type;

#endif