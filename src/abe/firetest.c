#include "common.h"

static s32 FiretestInit() {
    vpmINFO *vpmi;
    u32 i;

    vpmi = &VpmInfo;
    SysGbl.objwork = (OBJWORK *)malloc(sizeof(OBJWORK) * 128);
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
    memoryStageFormat();
    memoryAreaptrSet();
    memoryAreaFormat();
    nkInitDma();
    abMfifoInit();
    _nkGetMapBuf();
    nkMapFileRead();
    draw_id = 0;
    vpmi->block_list[0] = 0;
    for (i = 0; i < 2048; i++) {
        vpmi->block_list[i + 1] = 0;
    }
    nkPathReset();
    nkInitAtrMain();
    sceGsSyncPath(0,0);
    nkDebFlush();
    nkPathFlush();
    hr_nak_init_allobj();
    MapConfFileRead();
    hrMapFileRead();
    hr_set_vpmfog(&VpmInfo);
    OkSetIDprofile();
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 12, 6);
    OkPrintSysInit(5, SCR_WIDTH, SCR_HEIGHT, 12, 6);
    CamInit(&GameGbl.cam);
    GameGbl.cam.posi[0] = 200.0f;
    GameGbl.cam.posi[1] = -500.0f;
    SysGbl.smode++;
    abEffectInit();

    return 0;
}

static s32 FiretestMain() {
    s32 inter;
    kPadDATA *kpd0;
    kPadDATA *kpd1;
    volatile u32 *cnt;
    s32 hcnt0;
    s32 hcnt1;

    kpd0 = &GameGbl.kpd[0];
    nkGetPad();
    kpd0->lvl = nkGetPlvl(0);
    kpd0->trg = nkGetPtrg(0);
    kpd0->rep = nkGetPrep(0);

    kpd1 = &GameGbl.kpd[1];
    kpd1->lvl = nkGetPlvl(1);
    kpd1->trg = nkGetPtrg(1);
    kpd1->rep = nkGetPrep(1);

    nkPathReset();
    FlushCache(0);
    SortObjWork(SysGbl.objwork, SysGbl.n_objw);
    ObjFunc(SysGbl.objwork);
    CamCalMatrix(&GameGbl.cam);
    MapVDrawBG();
    hrMainWork();
    hrMainDraw();
    DrawObjFunc(SysGbl.objwork);
    nkSetMeter();
    sceGsSyncPath(0, 0);
    nkSetMeter();
    if (MfifoOn != 0)
        abMfifoBegin();

    cnt = T0_COUNT;
    hcnt0 = *cnt;
    DrEffObjFunc(SysGbl.objwork);
    hcnt1 = *cnt;

    nkSetMeter();
    if (MfifoOn != 0)
        abMfifoEnd((void *)0x70000000);
    sceGsSyncPath(0, 0);
    hcnt0 = *T0_COUNT;
    nkSetMeter();
    nkDrawWorkMeter();
    OkPFontFlush((kpd0->trg & 0x80) && (kpd0->lvl & 0x20));
    inter = sceGsSyncV(0) ^ 1;
    if ((GameGbl.fr & 1) != 0) {
        sceGsSetHalfOffset(&GameGbl.db.draw11, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw12, 0x800, 0x800, inter);
    } else {
        sceGsSetHalfOffset(&GameGbl.db.draw01, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw02, 0x800, 0x800, inter);
    }
    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.fr++;
    nkResetMeter();
    FlushCache(0);
    nkPathFlush();
    if ((kpd0->trg & 0x100) != 0) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }
    return 0;
}

s32 (*abFiretestFuncTbl[2])() = {
    FiretestInit,
    FiretestMain
};
