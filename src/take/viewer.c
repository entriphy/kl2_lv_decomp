#include <eetypes.h>
#include <libdev.h>
#include "take/viewer.h"
#include "nakano/camera.h"
#include "nakano/drawof.h"
#include "nakano/nkpad.h"
#include "nakano/wmeter.h"
#include "okanoyo/okio.h"
#include "take/view.h"
#include "nakano/dma.h"
#include "nakano/objfunc.h"
#include "okanoyo/okprint.h"

static s32 ViewerInit() {
	sceVu0FVECTOR vf;

    SysGbl.objwork = malloc(sizeof(OBJWORK) * 128);
    SysGbl.n_objw = 128;
    InitAllObjWork(SysGbl.objwork, 128);
    sceGsResetPath();
    sceDevGifReset();
    sceDevVif1Reset();
    sceDevVu1Reset();
    sceDevVif0Reset();
    sceDevVu0Reset();
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ16S, SCE_GS_CLEAR);
    FlushCache(WRITEBACK_DCACHE);
    nkInitDma();
    OkSetIDprofile();
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 12, 6);
    OkPrintSysInit(7, SCR_WIDTH, SCR_HEIGHT, 12, 6);
    ViewInit();
    SysGbl.smode++;
}

static s32 ViewerMain() {
    s32 inter;
    kPadDATA *kpd0;
    kPadDATA *kpd1;

    kpd0 = &GameGbl.kpd[0];
    nkGetPad();
    kpd0->lvl = nkGetPlvl(0);
    kpd0->trg = nkGetPtrg(0);
    kpd0->rep = nkGetPrep(0);

    kpd1 = &GameGbl.kpd[1];
    kpd1->lvl = nkGetPlvl(1);
    kpd1->trg = nkGetPtrg(1);
    kpd1->rep = nkGetPrep(1);

    nkSetMeter();
    View();
    SortObjWork(SysGbl.objwork, SysGbl.n_objw);
    ObjFunc(SysGbl.objwork);
    
    DrawObjFunc(SysGbl.objwork);
    CamDebug(&GameGbl.cam);
    CamCalMatrix(&GameGbl.cam);
    sceGsSyncPath(0, 0);
    DrEffObjFunc(SysGbl.objwork);
    sceGsSyncPath(0, 0);
    OkPFontFlush(PAD_TRG_SELECT(GameGbl.kpd[1]));

    inter = sceGsSyncV(0) ^ 1;
    if (GameGbl.fr & 1) {
        sceGsSetHalfOffset(&GameGbl.db.draw11, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw12, 0x800, 0x800, inter);
    } else {
        sceGsSetHalfOffset(&GameGbl.db.draw01, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw02, 0x800, 0x800, inter);
    }
    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.inter = inter;
    GameGbl.fr++;
    

    if (pPAD_TRG_SELECT(kpd0) && pPAD_LVL_START(kpd0)) {
        DebmemFormat(7);
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }

    // Return intentionally left blank
}

s32 (*tkViewerFuncTbl[])() = {
    ViewerInit,
    ViewerMain
};
