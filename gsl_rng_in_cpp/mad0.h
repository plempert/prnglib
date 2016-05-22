#ifndef mad0_
#define mad0_

#include "MaD0/mad0.h"
#include "MaD0/mad0.c"

#include <string>
using std::string;

class MaD0 : public PRNG {
public:
    
    void set(const char * seed, int size){
        state = new mad0();
        const unsigned char * key = (const unsigned char *)seed;
        initGenFrKey((mad0 *)state, size, key);
    }

    unsigned int get_int(){
        return generateUint32((mad0 *)state);
    }  

    double get_double(){
        return generateDouble((mad0 *)state);
    }  

};

#endif