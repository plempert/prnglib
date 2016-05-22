#include "pgl_rng.h"
#include <time.h>   /* time_t, time */
#include <unistd.h> /* sleep */
#include <stdio.h>  /* printf */
#include "time_f.h"
#include "MaD0/mad0.h"
#include "tinymt/tinymt32.h"

void mad0_test();
void tinymt_test();

static const int BIG = 500000000;

int main(){
    mad0_test();
    tinymt_test();
    return 0;
}

void mad0_test(){

    gsl_rng * r = gsl_rng_alloc(gsl_rng_mad0);
    gsl_rng_set(r, 5);

    mad0 * old = malloc(sizeof(mad0));
    initGenFrKey(old, 4, (const unsigned char *)"abcd");
    mad0 old2;
    initGenFrKey(&old2, 4, (const unsigned char *)"abcd");

    printf("mad0_test() started\n--------------------\n");
    for(int i=0; i<10; i++){
        unsigned long int n = gsl_rng_get(r);
        unsigned long int o1 = (unsigned long int)generateUint32(old);
        unsigned long int o2 = (unsigned long int)generateUint32(&old2);
        if(n!=o1 || n!=o2) {
            printf("mad0 test failed:\n");
            fprintf(stdout, "new: %lu\n", n);
            fprintf(stdout, "old: %lu\n", o1);
            fprintf(stdout, "old2: %lu\n", o2);
            exit(1);
        }
        printf("%lu\n", n);
    }


    start();
    printf("old time to generate %d ints: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int o1 = (unsigned long int)generateUint32(old);
    }
    stop();

    start();
    printf("new time to generate %d ints: ", BIG);
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
    for(int i=0; i<10; i++){
        unsigned long int n = gsl_rng_get(r);
        unsigned long int o1 = (unsigned long int)tinymt32_generate_uint32((tinymt32_t*)old);
        if(n!=o1) {
            printf("tinymt test failed:\n");
            printf("new: %lu\n", n);
            printf("old: %lu\n", o1);
            exit(1);
        }
        printf("%lu\n", n);
    }

    start();
    printf("old time to generate %d ints: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int o1 = (unsigned long int)tinymt32_generate_uint32(old);
    }
    stop();

    start();
    printf("new time to generate %d ints: ", BIG);
    for(int i=0; i<BIG; i++){
        unsigned long int n = gsl_rng_get(r);
    }
    stop();

    gsl_rng_free(r);

    fprintf(stdout, "--------------------\n");
}