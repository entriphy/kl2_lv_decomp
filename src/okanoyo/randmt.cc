#include "okanoyo/randmt.h"

void randMT::generateMT() {
    u32 mag01[] = { 0, MATRIX_A };
    s32 i;
    u32 u;

    for (i = 0; i < N - M; i++) {
        u = this->mt[i] & UPPER_MASK | this->mt[i + 1] & LOWER_MASK;
        this->mt[i] = this->mt[i + M] ^ u >> 1 ^ mag01[u & 1];
    }

    this->mt[N] = this->mt[0];

    for (; i < N; i++) {
        u = this->mt[i] & UPPER_MASK | this->mt[i + 1] & LOWER_MASK;
        this->mt[i] = this->mt[i + (M - N)] ^ u >> 1 ^ mag01[u & 1];
    }

    for (i = 0; i < N; i++) {
        u = this->mt[i];
        u ^= TEMPERING_SHIFT_U(u);
        u ^= TEMPERING_SHIFT_S(u) & TEMPERING_MASK_B;
        u ^= TEMPERING_SHIFT_T(u) & TEMPERING_MASK_C;
        u ^= TEMPERING_SHIFT_L(u);
        this->mtr[i] = u;
    }

    this->index = 0;
}

void randMT::srand(u32 s) {
    this->seed = s;
    for (s32 i = 0; i < N; i++) {
        s = 69069 * s + 1;
        u32 v = s & 0xffff0000;
        s = 69069 * s + 1;
        this->mt[i] = v | (s & 0xffff0000) >> 16;
    }
    this->generateMT();
}

u32 randMT::rand() {
    if (this->index >= N) {
        this->generateMT();
    }
    return this->mtr[this->index++];
}

u32 randMT::randRange(u32 max) {
    return this->rand() % max;
}

f32 randMT::GetNormalDistribution(u32 i1, u32 i2) {
    f64 d1 = (i1 & 0xFFFFFF) / (f64)0xFFFFFF;
    f64 d2 = (i2 & 0xFFFFFF) / (f64)0xFFFFFF;
    return (sqrt(log(d1) * -2.0)) * sin(d2 * 6.283184);
}