#include "common.h"

sceDmaChan* DmaChVIF0;
sceDmaChan* DmaChVIF1;
sceDmaChan* DmaChGIF;
sceDmaChan* DmaChfromSPR;
sceDmaChan* DmaChtoSPR;

void nkInitDma() {
    sceDmaReset(1);
    FUN_00302df0(0);
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

void nkInitSys() {
    // TODO
    nkDG.capture_num = 0;
    nkDG.map_draw_flag = 0;
    nkDG.opflag = 0;
    hrpt_deb = 0;
}

int FUN_00302e58() {
    return (*GIF_STAT & 4) >> 2;
}

void FUN_00302df0(int param_1) {
    if (param_1 == 0) {
        *GIF_MODE = FUN_00302e58() & 0xfffffffb;
    } else {
        *GIF_MODE = FUN_00302e58() | 4;
    }
}

