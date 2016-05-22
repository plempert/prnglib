/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */

#include "new_well.h"





void WELL::InitWELLRNG1024a (unsigned int *init){
    int j;
    state_i = 0;
    for (j = 0; j < R; j++)
        STATE[j] = init[j];
}

double WELL::WELLRNG1024a (void){
    z0    = VRm1;
    z1    = Identity(V0)       ^ MAT0POS (8, VM1);
    z2    = MAT0NEG (-19, VM2) ^ MAT0NEG(-14,VM3);
    newV1 = z1                 ^ z2; 
    newV0 = MAT0NEG (-11,z0)   ^ MAT0NEG(-7,z1)    ^ MAT0NEG(-13,z2) ;
    state_i = (state_i + 31) & 0x0000001fU;
    return ((double) STATE[state_i]  * FACT);
}
