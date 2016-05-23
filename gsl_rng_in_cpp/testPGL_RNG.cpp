#include <iostream>
#include "pgl_rng.h"
#include "time_f.h"

#include <chrono>
using namespace std::chrono;

namespace old_dsfmt{
    #include "dsfmt/dsfmt.h"
    #include "dsfmt/dsfmt.c"
}


using std::cout;
using std::endl;

void pgl_rng_example();
void dsfmt_test(unsigned int);
void mad0_test(const unsigned char *);
void tinymt_test(unsigned int);
void well_test();
void xorgens_test();

static const int BIG = 500000000;



int main(){
    //pgl_rng_example();
    //cout << pgl_rng_mad0 << endl;
    dsfmt_test(5);
//    mad0_test((const unsigned char *)"abcd");
//     tinymt_test(5);
    
    return 0;
}

void pgl_rng_example(){}

void dsfmt_test(unsigned int seed){
    old_dsfmt::init_gen_rand(seed);
    
    pgl_rng * r = pgl_rng_alloc(pgl_rng_dsfmt);
    pgl_rng_set(r, seed);

    for(int i=0; i<10; i++){
        double old_rgn = old_dsfmt::genrand_close_open();
        double new_rng = pgl_rng_uniform(r);
        if(old_rgn!=new_rng) {
            cout << old_rgn << " " << new_rng << endl;
            exit(0);
        }
        if(old_rgn==0.0){
            cout << old_rgn << endl;
            exit(0);
        }
        cout << old_rgn << " " << new_rng << endl;
    }

    start();
    printf("old time to generate %d ints: ", BIG);
    for(int i=0; i<BIG; i++){
        double o1 = (double)old_dsfmt::genrand_close_open();
    }
    stop();

    start();
    printf("new time to generate %d ints: ", BIG);
    for(int i=0; i<BIG; i++){
        double n = (double)pgl_rng_uniform(r);
    }
    stop();

    fprintf(stdout, "--------------------\n");

    pgl_rng_free(r);
}

void mad0_test(const unsigned char * seed){
    mad0* gen = new mad0();
    initGenFrKey(gen, 4, (const unsigned char *)"abcd");

    pgl_rng * r = pgl_rng_alloc(pgl_rng_mad0);
    pgl_rng_set(r, seed);

    for(int i=0; i<10; i++){
        unsigned int old_rgn = generateUint32(gen);
        unsigned int new_rng = pgl_rng_get(r);
        if(old_rgn!=new_rng) {
            cout << old_rgn << " " << new_rng << endl;
            exit(0);
        }
        cout << old_rgn << endl;
        // cout << new_rng << endl;
    }

    start();
    printf("old time to generate %d ints: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int o1 = (unsigned long int)generateUint32(gen);
    }
    stop();

    start();
    printf("new time to generate %d ints: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int n = (unsigned long int)pgl_rng_get(r);
    }
    stop();

    fprintf(stdout, "--------------------\n");

    pgl_rng_free(r);
}

void tinymt_test(unsigned int seed){
    tinymt32_t* generator = new tinymt32_t();
    tinymt32_init(generator, seed);

    pgl_rng * r = pgl_rng_alloc(pgl_rng_tinymt);
    pgl_rng_set(r, seed);
    cout << pgl_rng_name(r) << endl;
    for(int i=0; i<10; i++){
        unsigned int old_rgn = tinymt32_generate_uint32(generator);
        unsigned int new_rng = pgl_rng_get(r);
        if(old_rgn!=new_rng) {
            cout << old_rgn << " " << new_rng << endl;
            exit(1);
        }
        cout << old_rgn << endl;
    }

    start();
    printf("old time to generate %d ints: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int o1 = (unsigned long int)tinymt32_generate_uint32(generator);
    }
    stop();

    start();
    printf("new time to generate %d ints: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int n = (unsigned long int)pgl_rng_get(r);
    }
    stop();

    fprintf(stdout, "--------------------\n");

    pgl_rng_free(r);
}

void well_test(){}
void xorgens_test(){}