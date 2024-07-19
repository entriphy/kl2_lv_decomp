#include <eetypes.h>
#include <libdev.h>
#include "take/t_sample.h"
#include "nakano/camera.h"
#include "nakano/dma.h"
#include "nakano/drawof.h"
#include "nakano/nkpad.h"
#include "nakano/objfunc.h"
#include "nakano/wmeter.h"
#include "okanoyo/okprint.h"

static s32 SampleInit();
static s32 SampleMain();

s32 (*tkSampleFuncTbl[])() = {
    SampleInit,
    SampleMain
};

static s32 SampleInit() {
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
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ32, SCE_GS_CLEAR);
    FlushCache(0);
    nkInitDma();
    OkSetIDprofile();
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 12, 6);
    OkPrintSysInit(7, SCR_WIDTH, SCR_HEIGHT, 12, 6);
    CamInit(&GameGbl.cam);
    SysGbl.smode++;
    // Return intentionally left blank
}

static s32 SampleMain() {
    s32 inter;
    kPadDATA *kpd0;
    kPadDATA *kpd1;
    s32 lp;

    kpd0 = &GameGbl.kpd[0];
    nkGetPad();
    kpd0->lvl = nkGetPlvl(0);
    kpd0->trg = nkGetPtrg(0);
    kpd0->rep = nkGetPrep(0);

    kpd1 = &GameGbl.kpd[1];
    kpd1->lvl = nkGetPlvl(1);
    kpd1->trg = nkGetPtrg(1);
    kpd1->rep = nkGetPrep(1);

    for (lp = 0; lp < 100000; lp++) {
        
    }

    nkSetMeter();
    for (lp = 0; lp < 100000; lp++) {
        
    }

    CamDebug(&GameGbl.cam);
    CamCalMatrix(&GameGbl.cam);
    DrawObjFunc(SysGbl.objwork);
    sceGsSyncPath(0, 0);
    nkSetMeter();
    nkDrawWorkMeter();
    OkPFontFlush(pPAD_TRG_SQUARE(kpd0) && pPAD_LVL_CIRCLE(kpd0));
    inter = sceGsSyncV(0) ^ 1;
    if (GameGbl.fr & 1) {
        sceGsSetHalfOffset(&GameGbl.db.draw11, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw12, 0x800, 0x800, inter);
    } else {
        sceGsSetHalfOffset(&GameGbl.db.draw01, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw02, 0x800, 0x800, inter);
    }
    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.fr++;
    nkResetMeter();
    if (pPAD_TRG_SELECT(kpd0)) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }

    // Return intentionally left blank
}