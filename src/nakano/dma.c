#include "common.h"

void *DAT_003fb93c;

void FUN_0018dc78(void *tag) {
    sceDmaSync(DmaChGIF, 0, 0);
    sceDmaSend(DmaChGIF, tag);
}

void FUN_0018dcb0(void *tag) {
    FlushCache(WRITEBACK_DCACHE);
    FUN_0018dc78(tag);
    sceGsSyncPath(0, 0);
}
