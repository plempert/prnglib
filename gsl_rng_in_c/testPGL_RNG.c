#include "pgl_rng.h"
#include <time.h>   /* time_t, time */
#include <unistd.h> /* sleep */
#include <stdio.h>  /* printf */
#include "time_f.h"
#include "MaD0/mad0.h"
#include "tinymt/tinymt32.h"
#include "dsfmt/dSFMT.h"
#include "well/WELL1024a.h"
#include "xorgens/xorgens.h"

void mad0_test();
void tinymt_test();
void dsfmt_test();
void well_test();
void xorgens_test();

static const int BIG = 500000000;
static const int test_values = 5;

int main(){
    mad0_test();
    tinymt_test();
    dsfmt_test();
    well_test();
    xorgens_test();
    return 0;
}

void mad0_test(){

    gsl_rng * r = gsl_rng_alloc(gsl_rng_mad0);
    gsl_rng_set2(r, 4, (const unsigned char *)"abcd");

    mad0 * old = calloc(1, sizeof(mad0));
    initGenFrKey(old, 4, (const unsigned char *)"abcd");

    printf("mad0_test() started\n--------------------\n");
    for(int i=0; i<test_values; i++){
        printf("%lu %lu\n", gsl_rng_get(r), (unsigned long int)generateUint32(old));
    }

    start();
    printf("old time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int o1 = (unsigned long int)generateUint32(old);
    }
    stop();

    start();
    printf("new time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int n = (unsigned long int)gsl_rng_get(r);
    }
    stop();

    gsl_rng_free(r);

    fprintf(stdout, "--------------------\n");
}
void tinymt_test(){
    gsl_rng * r = gsl_rng_alloc(gsl_rng_tinymt);
    gsl_rng_set(r, 5);

    void * old = calloc(1, sizeof(tinymt32_t));
    tinymt32_init((tinymt32_t*)old, 5);

    printf("tinymt_test() started\n--------------------\n");
    for(int i=0; i<test_values; i++){
        printf("%lu %lu\n", gsl_rng_get(r), (unsigned long int)tinymt32_generate_uint32((tinymt32_t*)old));
    }

    start();
    printf("old time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int o1 = (unsigned long int)tinymt32_generate_uint32(old);
    }
    stop();

    start();
    printf("new time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int n = gsl_rng_get(r);
    }
    stop();

    gsl_rng_free(r);

    printf("--------------------\n");
}
void dsfmt_test(){
    gsl_rng * r = gsl_rng_alloc(gsl_rng_dsfmt);
    gsl_rng_set(r, 5);
    init_gen_rand(5);

    printf("dsfmt_test() started\n--------------------\n");


    for(int i=0; i<test_values; i++){
        printf("%f %f\n", genrand_close_open(), gsl_rng_uniform(r));
    }

    start();
    printf("old time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        double o1 = genrand_close_open();
    }
    stop();

    start();
    printf("new time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        double n = gsl_rng_uniform(r);
    }
    stop();

    gsl_rng_free(r);

    printf("--------------------\n");

}
void well_test(){
    gsl_rng * r = gsl_rng_alloc(gsl_rng_well);
    unsigned int seed_value[32];
    for(int i=0; i<32; i++){
        seed_value[i] = i*2;    // Arbitrary seed value
    }

    gsl_rng_set3(r, seed_value);
    InitWELLRNG1024a(seed_value);

    printf("well_test() started\n--------------------\n");

    for(int i=0; i<test_values; i++){
        printf("%f %f\n", WELLRNG1024a(), gsl_rng_uniform(r));
    }

    start();
    printf("old time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        double o1 = WELLRNG1024a();
    }
    stop();

    start();
    printf("new time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        double n = gsl_rng_uniform(r);
    }
    stop();

    gsl_rng_free(r);

    printf("--------------------\n");
}
void xorgens_test(){
    gsl_rng * r = gsl_rng_alloc(gsl_rng_xorgens);
    gsl_rng_set(r, 5);

    xor4096i(5);

    printf("xorgens_test() started\n--------------------\n");

    for(int i=0; i<test_values; i++){
        printf("%lu %lu\n", xor4096i(0), gsl_rng_get(r));
    }

    start();
    printf("old time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int o1 = xor4096i(0);
    }
    stop();

    start();
    printf("new time to generate %d values: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int n = gsl_rng_get(r);
    }
    stop();

    gsl_rng_free(r);

    printf("--------------------\n");
}