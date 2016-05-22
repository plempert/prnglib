

#ifndef xorgens_new_
#define xorgens_new_

/* xorgens.h for xorgens version 3.04, R. P. Brent 20060628 */

typedef unsigned long UINT; /* Type for random 32 or 64-bit integer, 
                               e.g. unsigned long, unsigned long long,
                               uint64_t, unsigned int or uint32_t */
                                   
typedef double UREAL;       /* Type for random 32 or 64-bit real,
                               e.g. double or float */

// UINT  xor4096i(UINT seed);  /* integer random number generator */

// UREAL xor4096r(UINT seed);  /* real random number generator */




class XORGENS{
private:
    // 'static' state variables from xor4096i()
    UINT w, weyl, zero, *x;
    int i;              /* i < 0 indicates first call */

public:

    XORGENS(){
        #define UINT64 (sizeof(UINT)>>3)
        #define UINT32 (1 - UINT64) 
        #define r    (64*UINT64 + 128*UINT32)
        zero = 0;
        i = -1;
        x = new UINT[r];
        #undef r
        #undef UINT64
        #undef UINT32
    }





UINT xor4096i(UINT seed)

  {
  /* 32-bit or 64-bit integer random number generator 
     with period at least 2**4096-1.
     
     It is assumed that "UINT" is a 32-bit or 64-bit integer 
     (see typedef statements in xorgens.h).
     
     xor4096i should be called exactly once with nonzero seed, and
     thereafter with zero seed.  
     
     One random number uniformly distributed in [0..2**wlen) is returned,
     where wlen = 8*sizeof(UINT) = 32 or 64.

     R. P. Brent, 20060628.
  */

  /* UINT64 is TRUE if 64-bit UINT,
     UINT32 is TRUE otherwise (assumed to be 32-bit UINT). */
     
#define UINT64 (sizeof(UINT)>>3)
#define UINT32 (1 - UINT64) 

#define wlen (64*UINT64 +  32*UINT32)
#define r    (64*UINT64 + 128*UINT32)

#define s    (53*UINT64 +  95*UINT32)
#define a    (33*UINT64 +  17*UINT32)
#define b    (26*UINT64 +  12*UINT32)
#define c    (27*UINT64 +  13*UINT32)
#define d    (29*UINT64 +  15*UINT32)
#define ws   (27*UINT64 +  16*UINT32) 


  UINT t, v;
  int k;
  
  if ((i < 0) || (seed != zero)) { /* Initialisation necessary */
  
  /* weyl = odd approximation to 2**wlen*(3-sqrt(5))/2. */

    if (UINT32) 
      weyl = 0x61c88647;
    else 
      weyl = ((((UINT)0x61c88646)<<16)<<16) + (UINT)0x80b583eb;
                 
    v = (seed!=zero)? seed:~seed;  /* v must be nonzero */

    for (k = wlen; k > 0; k--) {   /* Avoid correlations for close seeds */
      v ^= v<<10; v ^= v>>15;      /* Recurrence has period 2**wlen-1 */ 
      v ^= v<<4;  v ^= v>>13;      /* for wlen = 32 or 64 */
      }
    for (w = v, k = 0; k < r; k++) { /* Initialise circular array */
      v ^= v<<10; v ^= v>>15; 
      v ^= v<<4;  v ^= v>>13;
      x[k] = v + (w+=weyl);                
      }
    for (i = r-1, k = 4*r; k > 0; k--) { /* Discard first 4*r results */ 
      t = x[i = (i+1)&(r-1)];   t ^= t<<a;  t ^= t>>b; 
      v = x[(i+(r-s))&(r-1)];   v ^= v<<c;  v ^= v>>d;          
      x[i] = t^v;       
      }
    }
    
  /* Apart from initialisation (above), this is the generator */

  t = x[i = (i+1)&(r-1)];            /* Assumes that r is a power of two */
  v = x[(i+(r-s))&(r-1)];            /* Index is (i-s) mod r */
  t ^= t<<a;  t ^= t>>b;             /* (I + L^a)(I + R^b) */
  v ^= v<<c;  v ^= v>>d;             /* (I + L^c)(I + R^d) */
  x[i] = (v ^= t);                   /* Update circular array */
  w += weyl;                         /* Update Weyl generator */
  return (v + (w^(w>>ws)));          /* Return combination */

#undef UINT64
#undef UINT32
#undef wlen
#undef s
#undef a
#undef b
#undef c
#undef d
#undef ws
#undef r
  }

UREAL xor4096r(UINT seed)

  {
  /* 64-bit or 32-bit real random number generator 
     with period at least 2**4096-1.
     
     It is assumed that "UINT" is a 32-bit or 64-bit integer and "UREAL"
     is "double" or "float". If "double" this is an IEEE standard
     floating-point number with 53 bits in the fraction; if "single" it 
     has 24 bits in the fraction (including 1 implicit bit in each case).
     
     In the 64-bit integer case, the method used is to call xor4096i to get
     64 random bits, then the high 53 (for double) or 24 bits (for float)
     are scaled to the open interval (0.0, 1.0), except that they are
     discarded if all zero.

     In the 32-bit integer case, one or two calls to xor4096i are made to
     get 32 or 64 random bits, some are discarded, and the remaining bits
     (if nonzero) are scaled to the open interval (0.0, 1.0).  

     xor4096r should be called exactly once with nonzero seed, and
     thereafter with zero seed.

     One random number of type UREAL is returned per call.
     
     The results be should uniformly distributed in (0.0, 1.0) to the
     resolution of the floating-point system (0.5**53 or 0.5**24).  

     The results are never exactly 0.0 or 1.0.

     R. P. Brent, 20060628.
     */
     
#define UINT64 (sizeof(UINT)>>3)
#define UINT32 (1 - UINT64) 
#define UREAL64 (sizeof(UREAL)>>3)
#define UREAL32 (1 - UREAL64)

/* sr = number of bits discarded = 11 for double, 40 or 8 for float */
 
#define sr (11*UREAL64 +(40*UINT64 + 8*UINT32)*UREAL32)

/* ss (used for scaling) is 53 or 21 for double, 24 for float */

#define ss ((53*UINT64 + 21*UINT32)*UREAL64 + 24*UREAL32)

/* SCALE is 0.5**ss, SC32 is 0.5**32 */

#define SCALE ((UREAL)1/(UREAL)((UINT)1<<ss)) 
#define SC32  ((UREAL)1/((UREAL)65536*(UREAL)65536)) 

  UREAL res;
  
  res = (UREAL)0; 
  while (res == (UREAL)0)               /* Loop until nonzero result.   */
    {                                   /* Usually only one iteration . */
    res = (UREAL)(xor4096i(seed)>>sr);  /* Discard sr random bits.  */
    seed = (UINT)0;                     /* Zero seed for next time. */
    if (UINT32 && UREAL64)              /* Need another call to xor4096i. */
      res += SC32*(UREAL)xor4096i(seed);/* Add low-order 32 bits. */
    }
  return (SCALE*res);                   /* Return result in (0.0, 1.0). */  

#undef UINT64
#undef UINT32
#undef UREAL64
#undef UREAL32
#undef SCALE
#undef SC32
#undef sr
#undef ss
  }


};


#endif