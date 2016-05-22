#ifndef pglrng_mad0_h
#define pglrng_mad0_h

#include "MaD0/mad0.h"
#include "MaD0/mad0.c"

struct pglrng_mad0 : public pgl_rng {
    
    void set(const unsigned char * seed)override{
        state = new mad0();
//        initGenFrKey((mad0 *)state, sizeof(seed)-1, seed);
        initGenFrKey((mad0*)state, 4, (const unsigned char *)"abcd");
    }

    unsigned long int get_long()override{
        return (unsigned long int)generateUint32((mad0 *)state);
    }  

    double get_double()override{
        return generateDouble((mad0 *)state);
    }  

};

#endif