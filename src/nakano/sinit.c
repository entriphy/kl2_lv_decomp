#include "common.h"

void nkStageInit0() {
    if ((GameGbl.vision >> 8) - 15 > 5) {
        u8 *gms = (u8 *)hGetDataAddr(0);
        if (gms != NULL) {
            nkLoadGms(gms + ((s32 *)gms)[1]);
            sceGsSyncPath(0, 0);
        }
    }
}

void nkStageInit1() {
    // TODO
}
