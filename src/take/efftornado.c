#include <eetypes.h>
#include <libdev.h>
#include "take/efftornado.h"
#include "take/camera.h"
#include "take/object.h"
#include "nakano/drawof.h"
#include "nakano/nkpad.h"
#include "nakano/wmeter.h"

#include "abe/ab_init.h"
#include "nakano/dma.h"
#include "nakano/map00.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"
#include "okanoyo/okprint.h"

// i don't know the actual name of this file but it does stuff with tornados
// so efftornado.c it is (to put it after effsub.c)

s32 (*tkTornadoFuncTbl[])() = {
    tkTornadoInit,
    tkTornadoMain
};
static OBJWORK *TornadoObj = NULL;
static PsfxMODEL TornadoModel = {};

static void func_002064A0();
static void func_002064D8(sceVu0FVECTOR Trans);

s32 tkTornadoInit() {
    s32 i;

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
    OkDefaultSysInit(0x280, 0xE0, 12, 6);
    OkPrintSysInit(7, 0x280, 0xE0, 12, 6);
    DataBuffPtr = getBuff(1, 0x100000, NULL, &i);
    pDma.Vif0 = sceDmaGetChan(DMAC_VIF0);
    pDma.Vif1 = sceDmaGetChan(DMAC_VIF1);
    pDma.Gif = sceDmaGetChan(DMAC_GIF);
    pDma.ToSpr = sceDmaGetChan(DMAC_TO_SPR);
    pDma.FromSpr = sceDmaGetChan(DMAC_FROM_SPR);
    ScreenInit();
    LightInit();
    nkPathReset();
    nkInitAtrMain();
    sceGsSyncPath(0, 0);
    nkDebFlush();
    nkPathFlush();
    nkPathReset();
    nkPathReset();
    nkInitAtrMain();
    sceGsSyncPath(0, 0);
    nkDebFlush();
    nkPathFlush();
    nkPathReset();
    memoryStageFormat();
    memoryAreaptrSet();
    memoryAreaFormat();
    GameGbl.vision = 0x1400;
    abGimInit();
    GetWsMtx();
    TornadoObj = SetTornado();
    TornadoModel.Trans[0] = 0.0f;
    TornadoModel.Trans[1] = 25.0f;
    TornadoModel.Trans[2] = 200.0f;
    TornadoModel.Trans[3] = 1.0f;
    SysGbl.smode++;
}

s32 tkTornadoMain() {
    s32 inter;
    kPadDATA *kpad0;
    kPadDATA *kpad1;

    kpad0 = &GameGbl.kpd[0];
    nkGetPad();
    kpad0->lvl = nkGetPlvl(0);
    kpad0->trg = nkGetPtrg(0);
    kpad0->rep = nkGetPrep(0);

    kpad1 = &GameGbl.kpd[1];
    kpad1->lvl = nkGetPlvl(1);
    kpad1->trg = nkGetPtrg(1);
    kpad1->rep = nkGetPrep(1);

    nkSetMeter();
    func_002064A0();
    SortObjWork(SysGbl.objwork, SysGbl.n_objw);
    ObjFunc(SysGbl.objwork);
    DrawObjFunc(SysGbl.objwork);
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
    if (pPAD_TRG_SELECT(kpad0) && pPAD_LVL_START(kpad0)) {
        DebmemFormat(7);
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }
}

static void func_002064A0() {
    BgDraw();
    func_002064D8(TornadoModel.Trans);
    SetTornadoTrans(TornadoObj, TornadoModel.Trans);
}

static void func_002064D8(sceVu0FVECTOR Trans) {
    // double-precision math...
    if (PAD_LVL_DOWN(GameGbl.kpd[0])) {
        Trans[1] += 2.5;
    }
    if (PAD_LVL_UP(GameGbl.kpd[0])) {
        Trans[1] -= 2.5;
    }
    if (PAD_LVL_RIGHT(GameGbl.kpd[0])) {
        Trans[0] += 2.5;
    }
    if (PAD_LVL_LEFT(GameGbl.kpd[0])) {
        Trans[0] += 2.5;
    }
    if (PAD_LVL_R1(GameGbl.kpd[0])) {
        Trans[2] += 2.5;
    }
    if (PAD_LVL_R2(GameGbl.kpd[0])) {
        Trans[2] -= 2.5;
    }
}
