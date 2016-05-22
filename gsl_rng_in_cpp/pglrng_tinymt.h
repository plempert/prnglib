#ifndef pglrng_tinymt_h
#define pglrng_tinymt_h

#include "tinymt/tinymt32.h"
#include "tinymt/tinymt32.c"
#include "pgl_rng.h"

#include <string>
using std::string;

class pglrng_tinymt : public pgl_rng {
public:
    void set(unsigned int seed){
        state = new tinymt32_t();
        tinymt32_init((tinymt32_t*)state, seed);
    }
    unsigned long int get_long(){
        return tinymt32_generate_uint32((tinymt32_t*)state);
    }
    double get_double(){
        return tinymt32_generate_float((tinymt32_t*)state);
    }
};

#endif