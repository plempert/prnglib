//
// Created by Patrick Lempert on 5/22/16.
//

#include "pgl_rng.h"

unsigned long int gsl_rng_default_seed = 0;

gsl_rng * gsl_rng_alloc(const gsl_rng_type * T){
    /*
     * malloc() allocates sizeof(gsl_rng) bytes of memory
     * and returns a pointer to the allocated memory.
     * Since malloc() return a void* we need to typecast
     * as gsl_rng*
     */
    gsl_rng *r = calloc(1, sizeof(gsl_rng));
    if (r == 0){
        printf("failed to allocate space for rng struct");
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
        printf("failed to allocate space for rng state");
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

