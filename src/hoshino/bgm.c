#include "common.h"

void hBgmReset() {
    if (cD->dataFlag == CDREAD_BGM && cD->dataStat != 2) {
        while (!sceCdBreak());
        cD->dataStat = 0;
    }
    hBgmWorkClear();
    bD->Command |= 0x4;
    hCdCueFlushBGM();
    if (sD->Stereo == SND_MODE_5P1CH && aD->field_0x80 != 0) {
        hStr_0016f6e8();
    }
}

int hBgmGetStat() {
    if (cD->BGMplay != 0) {
        switch (bD->fadeFlag) {
            case 1:
                return 3;
            case 2:
                return 2;
            case 3:
                return 3;
            default:
                return 1;
        }
    } else {
        return 0;
    }    
}

void hBgmWorkClear() {
    bD->nextNo = -1;
    bD->bgmCh = 0;
    bD->bgmChMax = 2;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
    bD->iopID = 0;
    bD->cdReq = 0;
    bD->fadeFlag = 0;
    bD->bgmVol = 0.0;
    bD->bgmMute = 0;
    cD->BGMplay = 0;
}
