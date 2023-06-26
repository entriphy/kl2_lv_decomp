#include "common.h"

void nkStageInit0() {
    u32 *adr;
    s32 Stage;

    Stage = GameGbl.vision >> 8;
    if (Stage - 15U > 5) {
        adr = (u32 *)hGetDataAddr(0);
        if (adr == NULL)
            return;
        nkLoadGms((qword *)((u32)adr + adr[1]));
        sceGsSyncPath(0, 0);
    }
}

void nkStageInit1() {
    // TODO
}
