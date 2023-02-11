#include "common.h"

int nkLoadTimer;
int nkLoadStat;
int DAT_003fb8f4;
void *DAT_003fc468;

int (*GameFuncTbl[3])() = {
    GameInit,
    GameLoad,
    GameMain
};

int GameInit() {
    int ret;

    nkInitPS2();
    nkGsSetNormalFZ2();
    nkWipeEffBClear();
    GameGbl.pause_flag2 = 0;
    memoryStageFormat();
    nkLoadTimer = 0;
    nkLoadStat = 0;
    DAT_003fb8f4 = 0;
    DAT_003fc468 = getBuff(1, FUN_00167bd0(1), NULL, &ret);
    kzInitNowload();
    nkKeyFrameStart();
    SysGbl.smode++;
    return 0;
}

int GameLoad() {
    return 0;
}

int GameMain() {
    return 0;
}
