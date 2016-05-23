/* xorgens.h for xorgens version 3.04, R. P. Brent 20060628 */

typedef unsigned long UINT; /* Type for random 32 or 64-bit integer, 
                               e.g. unsigned long, unsigned long long,
                               uint64_t, unsigned int or uint32_t */
                                   
typedef double UREAL;       /* Type for random 32 or 64-bit real,
                               e.g. double or float */

typedef struct {
    UINT w;
    UINT weyl;
    UINT x[(64*(sizeof(UINT)>>3) + 128*(1 - (sizeof(UINT)>>3)))];
    int i;              /* i < 0 indicates first call */
} xorgens;


UINT  rng_xor4096i(xorgens *state, UINT seed);  /* integer random number generator */

UREAL rng_xor4096r(xorgens *state, UINT seed);  /* real random number generator */
