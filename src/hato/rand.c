#include "hato.h"

int ht_g_rand;

void htInitRand(int seed) {
    if (seed == 0) {
        ht_g_rand = 1;
    } else {
        ht_g_rand = seed;
    }
}

int htGetRand(void) {
    ht_g_rand &= 0x7FFF;
    int odd = ht_g_rand & 1;
    ht_g_rand >>= 1;
    ht_g_rand |= odd << 14;
    ht_g_rand ^= odd << 7;
    return ht_g_rand - 1;
}