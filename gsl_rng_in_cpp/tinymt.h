#ifndef tinymt_
#define tinymt_

#include "tinymt/tinymt32.h"
#include "tinymt/tinymt32.c"

/*
    This class is a wrapper around the tinymt32
    random number generator. The set() function
    takes an integer seed and the get_int()
    function returns a randomly generated 
    unsigned int. Since the state is dynamically
    allocated and passed directly to the generator,
    there was no need to edit the header files to
    allow for multi-threaded functionality.  
*/
class tinymt : public PRNG {
public:
    void set(int seed){
        state = new tinymt32_t();
        tinymt32_init((tinymt32_t*)state, seed);
    }
    unsigned int get_int(){
        // The state, which is initially void*, needs to
        // be typecast as a tinymt32_t*
        return tinymt32_generate_uint32((tinymt32_t*)state);
    }
    double get_double(){
        return tinymt32_generate_float((tinymt32_t*)state);
    }
};

#endif