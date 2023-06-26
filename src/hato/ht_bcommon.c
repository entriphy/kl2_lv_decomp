#include "hato.h"

void htInitRand(s32 seed) {
    if (seed != 0)
        ht_g_rand = seed;
    else
        ht_g_rand = 1;
}

s32 htGetRand() {
    s32 tmp;

    ht_g_rand &= 0x7FFF;
    tmp = ht_g_rand & 1;
    ht_g_rand >>= 1;
    ht_g_rand |= tmp << 14;
    ht_g_rand ^= tmp << 7;
    return ht_g_rand - 1;
}
