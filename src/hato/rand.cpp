#include "hato.h"

void htInitRand(int seed) {
    if (seed == 0) {
        ht_g_rand = 1;
    } else {
        ht_g_rand = seed;
    }
}

int htGetRand() {
    ht_g_rand = (ht_g_rand & 1U) << 7 ^ ((ht_g_rand & 1U) << 0xe | (int)(ht_g_rand & 0x7fffU) >> 1);
    return ht_g_rand - 1;
}