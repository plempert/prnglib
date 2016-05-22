#ifndef PGL_RNG_H
#define PGL_RNG_H

#include <stdlib.h>

typedef struct{
    const char * name;
    unsigned long int max;
    unsigned long int min;
    int size;
    void (*set) (void * state, unsigned long int seed);
    unsigned long int (*get) (void * state);
    double (*get_double) (void *state);
} gsl_rng_type;

typedef struct{
    void * state;
    const gsl_rng_type * type;
//    int seeded;
} gsl_rng;

extern const gsl_rng_type * gsl_rng_dsfmt;     
extern const gsl_rng_type * gsl_rng_mad0;
extern const gsl_rng_type * gsl_rng_tinymt;
extern const gsl_rng_type * gsl_rng_well;
extern const gsl_rng_type * gsl_rng_xorgens;

//#include "dsfmt/pgl_rng_dsfmt.c"
#include "mad0/pgl_rng_mad0.c"
//#include "tinymt/pgl_rng_tinymt.c"
//#include "well/pgl_rng_well.c"
//#include "xorgens/pgl_rng_xorgens.c"

unsigned long int gsl_rng_default_seed = 0;
void gsl_rng_set(const gsl_rng * r, unsigned long int seed);


gsl_rng * gsl_rng_alloc(const gsl_rng_type * T){
    /*
     * malloc() allocates sizeof(gsl_rng) bytes of memory
     * and returns a pointer to the allocated memory.
     * Since malloc() return a void* we need to typecast
     * as gsl_rng*
     */
    gsl_rng *r = malloc(sizeof(gsl_rng));
    if (r == 0){
        fprintf(stdout, "failed to allocate space for rng struct");
        exit(1);
    };
    /*
     * calloc() contiguously allocates enough space for 1
     * object that is T->size bytes of memory and returns a pointer to the
     * allocated memory. The allocated memory is filled with bytes of value
     * zero.
     */
    r->state = calloc(1, T->size);
    if (r->state == 0){
        free(r);
        fprintf(stdout, "failed to allocate space for rng state");
        exit(1);
    };

    r->type = T;
    //gsl_rng_set(r, gsl_rng_default_seed);        /* seed the generator */
    return r;

}

void gsl_rng_set (const gsl_rng * r, unsigned long int seed){
    (r->type->set) (r->state, seed);
}

void gsl_rng_free (gsl_rng * r) {
    if(!r) return;
    free (r->state);
    free (r);
}

unsigned long int gsl_rng_get (const gsl_rng * r){
    return (r->type->get) (r->state);
}
double gsl_rng_uniform (const gsl_rng * r){
    return (r->type->get_double) (r->state);
}
double gsl_rng_uniform_pos (const gsl_rng * r){
    double x;

    do{
        x = (r->type->get_double)(r->state);
    }while(x == 0);

    return x;
}

//const char * gsl_rng_name (const gsl_rng_type * T){
//    /*
//        This function returns a pointer to the name of the generator.
//    */
//
//    if(T == gsl_rng_dsfmt){
//        return "dsfmt";
//    } else if(T == gsl_rng_mad0){
//        return "mad0";
//    } else if(T == gsl_rng_tinymt){
//        return "tinymt";
//    } else if(T == gsl_rng_well){
//        return "well";
//    } else if(T == gsl_rng_xorgens){
//        return "xorgens";
//    }
//}

//const char * gsl_rng_name (gsl_rng * r){
//    /*
//        This function returns a pointer to the name of the generator.
//    */
//    return gsl_rng_name(r->type);
//}

#endif