#include "common.h"

void hPptWorkClear() {
    int i;

    bD->Command |= 0x80;
    pD->pptMute = 0;
    pD->eeID = 0;
    pD->iopID = 0;
    pD->reqNum = 0;
    pD->reqID = 0;
    pD->listLoad = 0;
    pD->listPlay = 0;
    pD->listPlayIdx = 0;
    for (i = 0; i < 8; i++) {
        pD->listNum[i] = 0;
        pD->listTbl[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        pD->pptPlay[i] = 0;
        pD->eeStat[i] = 0;
    }
}

void hPptReset() {
    if (cD->dataFlag == CDREAD_PPT && cD->dataStat != 2) {
        while (!sceCdBreak());
        cD->dataStat = 0;
    }
    hCdCueFlushPPT();
    hPptWorkClear();
}
