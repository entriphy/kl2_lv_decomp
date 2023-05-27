#include "common.h"

u8 *DAT_003fb93c;

void FUN_0018dc78(u8 *tag) {
    sceDmaSync(DmaChGIF, 0, 0);
    sceDmaSend(DmaChGIF, tag);
}

void nkLoadGms(u8 *tag) {
    FlushCache(WRITEBACK_DCACHE);
    FUN_0018dc78(tag);
    sceGsSyncPath(0, 0);
}
