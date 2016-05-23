/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */

#ifndef well_rng_c
#define well_rng_c

#include "well_rng.h"

#define W 32
#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define Identity(v) (v)

#define V0            state->STATE[state->state_i                   ]
#define VM1           state->STATE[(state->state_i+M1) & 0x0000001fU]
#define VM2           state->STATE[(state->state_i+M2) & 0x0000001fU]
#define VM3           state->STATE[(state->state_i+M3) & 0x0000001fU]
#define VRm1          state->STATE[(state->state_i+31) & 0x0000001fU]
#define newV0         state->STATE[(state->state_i+31) & 0x0000001fU]
#define newV1         state->STATE[state->state_i                   ]

#define FACT 2.32830643653869628906e-10



void rngInitWELLRNG1024a (well *state, unsigned int *init){
    int j;
    state->state_i = 0;
    for (j = 0; j < R; j++)
       (state->STATE)[j] = init[j];
}

double rngWELLRNG1024a (well *state){
    state->z0    = VRm1;
    state->z1    = Identity(V0)       ^ MAT0POS (8, VM1);
    state->z2    = MAT0NEG (-19, VM2) ^ MAT0NEG(-14,VM3);
    newV1 = state->z1                 ^ state->z2;
    newV0 = MAT0NEG (-11,state->z0)   ^ MAT0NEG(-7,state->z1)    ^ MAT0NEG(-13,state->z2) ;
    state->state_i = (state->state_i + 31) & 0x0000001fU;
    return ((double) state->STATE[state->state_i]  * FACT);
}


#endif