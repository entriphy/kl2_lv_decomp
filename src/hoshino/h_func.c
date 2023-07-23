#include "h_func.h"
#include "h_menu.h"
#include "h_init.h"
#include "h_test.h"
#include "okanoyo/okprint.h"

static FUNCTBL MainFuncTbl[3] = {
    {hMenuFuncTbl, -1},
    {hTestFuncTbl, -1},
    {NULL,         -1}
};
s32 (*HFuncTbl[2])() = {
    hInit,
    hMain
};

s32 hInit() {
    memset(&hG, 0, sizeof(hGLOBAL));
    FlushCache(WRITEBACK_DCACHE);
    while (!sceGsSyncV(0));
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 16, 8);
    OkPrintSysInit(3, SCR_WIDTH, SCR_HEIGHT, 16, 8);
    hPkInit(&hG.PkData);
    hPkSetBp(10000);
    SysGbl.nsmode++;

    // Return intentionally left blank
}

s32 hMain() {
    s32 (**subfunctbl)();
    s32 inter;

    sceGsSwapDBuffDc(&GameGbl.db, hG.fr);
    hG.fr++;
    sceGsSyncPath(0, 0);
    FlushCache(0);

    subfunctbl = MainFuncTbl[SysGbl.fmode].func;
    if (subfunctbl != NULL) {
        subfunctbl[SysGbl.smode]();
    } else {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }

    inter = SyncV();
    if (TestMode != 3) {
        if (hG.fr & 1) {
            sceGsSetHalfOffset(&GameGbl.db.draw11, 0x800, 0x800, inter);
            sceGsSetHalfOffset2(&GameGbl.db.draw12, 0x800, 0x800, inter);
        } else {
            sceGsSetHalfOffset(&GameGbl.db.draw01, 0x800, 0x800, inter);
            sceGsSetHalfOffset2(&GameGbl.db.draw02, 0x800, 0x800, inter);
        }
    }

    FlushCache(0);
    sceGsSyncPath(0, 0);
    OkPFontFlush(PAD_TRG_SELECT(GameGbl.kpd[1]));

    // Return intentionally left blank
}