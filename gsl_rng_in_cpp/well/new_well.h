/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */


#ifndef WELL_H
#define WELL_H

#define W 32
#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t,v) (v^(v>>t))
#define MAT0NEG(t,v) (v^(v<<(-(t))))
#define Identity(v) (v)

#define V0            STATE[state_i                   ]
#define VM1           STATE[(state_i+M1) & 0x0000001fU]
#define VM2           STATE[(state_i+M2) & 0x0000001fU]
#define VM3           STATE[(state_i+M3) & 0x0000001fU]
#define VRm1          STATE[(state_i+31) & 0x0000001fU]
#define newV0         STATE[(state_i+31) & 0x0000001fU]
#define newV1         STATE[state_i                   ]

#define FACT 2.32830643653869628906e-10

class WELL{
private:
    unsigned int state_i = 0;
    unsigned int STATE[R];
    unsigned int z0, z1, z2;
public:
    void InitWELLRNG1024a (unsigned int *init);
    double WELLRNG1024a (void);
};
#endif