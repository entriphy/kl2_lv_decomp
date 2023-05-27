#include "common.h"

void nkInitDma() {
    sceDmaReset(1);
    sceDevGifPutImtMode(0);
    DmaChVIF0 = sceDmaGetChan(0);
    DmaChVIF0->chcr = (tD_CHCR){ .MOD = 1, .TTE = 1 };
    DmaChVIF1 = sceDmaGetChan(1);
    DmaChVIF1->chcr = (tD_CHCR){ .MOD = 1, .TTE = 1 };
    DmaChGIF = sceDmaGetChan(2);
    DmaChGIF->chcr = (tD_CHCR){ .MOD = 1 };
    DmaChfromSPR = sceDmaGetChan(8);
    DmaChfromSPR->chcr = (tD_CHCR){ };
    DmaChtoSPR = sceDmaGetChan(9);
    DmaChtoSPR->chcr = (tD_CHCR){ };
}

void nkDmaGifSend(void *tag) {
    sceDmaSync(DmaChGIF, 0, 0);
    sceDmaSend(DmaChGIF, tag);
}

void nkLoadGms(void *tag) {
    FlushCache(WRITEBACK_DCACHE);
    nkDmaGifSend(tag);
    sceGsSyncPath(0, 0);
}
