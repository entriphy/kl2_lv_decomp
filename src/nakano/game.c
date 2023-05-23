#include "common.h"

int nkLoadTimer;
int nkLoadStat;
int DAT_003fb8f4;
u32 *NakanoPackAddr;
u32 *nkLoadBuff;
s16 obj_id[129] = {};

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
    nkLoadBuff = (u32 *)getBuff(1, hGameDataSize(1), NULL, &ret);
    kzInitNowload();
    nkKeyFrameStart();
    SysGbl.smode++;
    return 0;
}

int GameLoad() {
    if (DAT_003fb8f4 == 0) {
        if (nkLoadStat == 0) {
            hGameRead(0, (s32)nkLoadBuff);
        }
        else if (nkLoadStat == 1) {
            hGameRead(1, (s32)nkLoadBuff);
        }
        DAT_003fb8f4++;
    }
    if (GameGbl.vision != 0x106 && DAT_003fb93c != NULL) {
        FUN_0018dcb0(DAT_003fb93c);
        sceGsSyncPath(0,0);
    }
    TkMainLoop(1);
    sceGsSyncPath(0,0);
    FUN_001dabd0(GameGbl.kpd[1].rep & 0x100);
    hLoopBottom();
    GameGbl.inter = sceGsSyncV(0) ^ 1;
    if ((GameGbl.fr & 1) == 0) {
        sceGsSetHalfOffset(&GameGbl.db.draw01, 2048, 2048, GameGbl.inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw02, 2048, 2048, GameGbl.inter);
    } else {
        sceGsSetHalfOffset(&GameGbl.db.draw11, 2048, 2048, GameGbl.inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw12, 2048, 2048, GameGbl.inter);
    }
    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.fr++;

    if (kzDrawNowload() == 1) {
        if (nkLoadStat == 0) {
            abStageInitA();
            hInitStage0();
            kzStageInit0();
            nkStageInit0();
        } else if (nkLoadStat == 1) {
            kzSetDispMaskOn();
            nkInitPS2();
            NakanoPackAddr = (u32 *)hGetDataAddr(0);
            nkStageInit1();
            SysGbl.smode++;
            nkGsSetNormalFZ2_0();
            nkWipeEffBClear();
            nkGsSetNormalFZ2_1();
            nkWipeEffBClear();
        }
        DAT_003fb8f4 = 0;
        nkLoadStat++;
    }

    return 0;
}

int GameMain() {
    // TODO
    return 0;
}
