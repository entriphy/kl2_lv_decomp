#include "common.h"

void hInitStage0() {
    gD = &GameData;
    hSndReset();
    hSeLock(0);
    hSndBankSetStage();
    hSndSetMVol(0.0f);
    gD->BGMmode = BGMMODE_TOP;
    gD->BGMppt = 0;
    gD->resFlag = 0;
}


