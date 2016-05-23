/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */

#ifndef well_rng_h
#define well_rng_h

typedef struct {
    unsigned int state_i;
    unsigned int STATE[32];
    unsigned int z0, z1, z2;
} well;


void rngInitWELLRNG1024a (well *state, unsigned int *init);
double rngWELLRNG1024a (well *state);

#endif