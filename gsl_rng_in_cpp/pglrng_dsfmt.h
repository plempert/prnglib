#ifndef pglrng_dsfmt_h
#define pglrng_dsfmt_h

#include "dsfmt/dsfmt_pgl.h"
#include "dsfmt/dsfmt_pgl.c"
#include "pgl_rng.h"

class pglrng_dsfmt : public pgl_rng {
public:
    void set(unsigned int seed) override {
        state = new DSFMT();
        ((DSFMT*)state)->init_gen_rand(seed);
    } 
    double get_double() override {
        return ((DSFMT*)state)->genrand_close_open();
    }
};




// static inline unsigned long int ran_get (void *vstate){
//     exit(1);
// }
// static double ran_get_double (void *vstate){
//     return ((DSFMT*)vstate)->genrand_close_open();
// }
// static void ran_set (void *state, unsigned long int s){
//     state = new DSFMT();
//     ((DSFMT*)state)->init_gen_rand(s);
// }
// static const pgl_rng_type ran_type = {
//     "dsfmt",                  /* name */
//     999,                       /* RAND_MAX */
//     111,                            /* RAND_MIN */
//     sizeof (5),
//     &ran_set,
//     &ran_get,
//     &ran_get_double
// };   
// const pgl_rng_type *pgl_rng_dsfmt = &ran_type;

#endif