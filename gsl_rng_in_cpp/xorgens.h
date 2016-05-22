#ifndef xorgens_
#define xorgens_

#include "xorgens/xorgens_new.h"
// #include "xorgens/xorgens.c"

#include <string>
using std::string;

class xorgens : public PRNG {
private:
    string type;
public:
    xorgens(){
        type = "unset";
    }
    void set_type(string type){
        this->type = type;
    }
    void set(int seed){
        state = new XORGENS();
        if(type=="int") ((XORGENS*)state)->xor4096i(seed);
        else if(type=="real") ((XORGENS*)state)->xor4096r(seed);
        else throw string("This generator requires that a type be set via set_type(string). Available types are 'int' and 'real'.");
    }
    void set(unsigned int * seed){}
    void set(const char * seed){}
    unsigned long get_long(){
        return ((XORGENS*)state)->xor4096i(0);
    }
    double get_double(){
        return ((XORGENS*)state)->xor4096r(0);
    }
};

#endif