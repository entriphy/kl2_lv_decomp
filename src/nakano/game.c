#include "common.h"

s32 nkLoadTimer;
s32 nkLoadStat;
s32 nkLoadBun;
u32 *NakanoPackAddr;
u32 *nkLoadBuff;
s16 obj_id[129] = {};

s32 (*GameFuncTbl[3])() = {
    GameInit,
    GameLoad,
    GameMain
};

s32 GameInit() {
    s32 ret;
    s32 size;

    nkInitPS2();
    nkGsSetNormalFZ2();
    nkWipeEffBClear();
    GameGbl.pause_flag2 = 0;
    memoryStageFormat();
    nkLoadTimer = 0;
    nkLoadStat = 0;
    nkLoadBun = 0;
    size = hGameDataSize(1);
    nkLoadBuff = (u32 *)getBuff(1, size, NULL, &ret);
    kzInitNowload();
    nkKeyFrameStart();
    SysGbl.smode++;
    return 0;
}

s32 GameLoad() {
    if (nkLoadBun == 0) {
        if (nkLoadStat == 0) {
            hGameRead(0, (s32)nkLoadBuff);
        }
        else if (nkLoadStat == 1) {
            hGameRead(1, (s32)nkLoadBuff);
        }
        nkLoadBun++;
    }
    if (GameGbl.vision != 0x106 && DAT_003fb93c != NULL) {
        nkLoadGms(DAT_003fb93c);
        sceGsSyncPath(0,0);
    }
    TkMainLoop(1);
    sceGsSyncPath(0,0);
    OkPFontFlush(GameGbl.kpd[1].rep & 0x100);
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
        nkLoadBun = 0;
        nkLoadStat++;
    }

    return 0;
}

s32 GameMain() {
    // TODO
    return 0;
}
