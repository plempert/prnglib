
#ifndef PRNGFactory_H
#define PRNGFactory_H

#include "PRNG_LIBRARY.h"
#include "xorgens.h"
#include "well.h"
#include "dsfmt.h"
#include "mad0.h"
#include "tinymt.h"

#include <string>
using std::string;

/*
    This class allows the programmer to select from an 
    assortment of pseudo-random number generators. There
    is one static function, generatePRNG(), which takes 
    as its parameter the name of the desired generator.
    It then returns a new instance of that generator as
    a pointer to an abstract class PRNG, which allows
    access to the method set() to set the seed as well
    as the functions get_int() and get_double() to get
    an unsigned int or double value, respectively.
*/

class PRNGFactory{
public:
    static PRNG* generatePRNG(string name){
        if(name=="dsfmt"){
            return new dsfmt();
        } else if(name=="mad0"){
            return new MaD0();
        } else if(name=="tinymt"){
            return new tinymt();
        } else if(name=="well"){
            return new well();
        } else if(name=="xorgens"){
            return new xorgens();
        } else {
            throw new string("PRNGFactory generatePRNG(): Invalid name. Acceptable names are dsfmt, mad0, tinymt, well, and xorgens");
        }
    } 
};

#endif