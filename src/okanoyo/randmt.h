#ifndef RANDMT_H
#define RANDMT_H

#include "okanoyo.h"

// http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/VERSIONS/C-LANG/980409/mt19937-1.c
// https://github.com/cslarsen/mersenne-twister/blob/master/mersenne-twister.cpp

/* Period parameters */
#define N 246
#define M 120
#define MATRIX_A 0x9908b0df   /* constant vector a */
#define UPPER_MASK 0x80000000 /* most significant w-r bits */
#define LOWER_MASK 0x7fffffff /* least significant r bits */

/* Tempering parameters */
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y)  (y >> 11)
#define TEMPERING_SHIFT_S(y)  (y << 7)
#define TEMPERING_SHIFT_T(y)  (y << 15)
#define TEMPERING_SHIFT_L(y)  (y >> 18)

class randMT {
private:
    s32 index;
    u32 seed;
    u32 mtr[N];
    u32 mt[N + 1];

protected:
    void generateMT();
public:
    randMT() {
        this->srand(4357);
    }
    void srand(u32 s);
    u32 rand();
    u32 randRange(u32 max);
    f32 GetNormalDistribution(u32 i1, u32 i2);
};

#endif