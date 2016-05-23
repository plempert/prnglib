#ifndef dsfmt_
#define dsfmt_

#include "dsfmt/dsfmtnew.h"
#include "dsfmt/dsfmtnew.c"
#include "PRNGFactory.h"

#include <string>
using std::string;

class dsfmt : public PRNG {
public:
    void set(int seed) override {
        state = new DSFMT();
        ((DSFMT*)state)->init_gen_rand(seed);
    } 
    double get_double() override {
        return ((DSFMT*)state)->genrand_close_open();
    }
};

#endif