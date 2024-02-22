#include "take/sfxdraw.h"


// These are technically inline but whatever duuuuude
void DmaSend_Mac(sceDmaChan *DmaChan, u128 *Addr) {
    __asm__("sync");
    DmaChan->tadr = (sceDmaTag *)Addr;
    DmaChan->qwc = 0;
    DmaChan->chcr.MOD = 1;
    DmaChan->chcr.STR = 1;
}

void DmaSync_Mac(sceDmaChan *DmaChan) {
    while (*(vs32 *)(&DmaChan->chcr) & D_CHCR_STR_M);
}
