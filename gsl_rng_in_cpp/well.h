#ifndef well_
#define well_

#include "well/new_well.h"
#include "well/new_well.c"

class well : public PRNG {

public:

    void set(unsigned int * seed){
        state = new WELL();
        ((WELL*)state)->InitWELLRNG1024a(seed);
    }

    double get_double(){
        return ((WELL*)state)->WELLRNG1024a();
    }
};

#endif