#include "common.h"

static s32 GameInit();
static s32 GameLoad();
static s32 GameMain();

static s32 nkGamePauseFlag = 0;
static s32 nkLoadTimer = 0;
static s32 nkLoadStat = 0;
static s32 nkLoadBun;
s32 (*GameFuncTbl[3])() = {
    GameInit,
    GameLoad,
    GameMain
};
GAME_WORK GameGbl = {};
s16 obj_id[129] = {};
static s32 nkGamePauseRepTimer;
static s32 nkGamePauseRepTimerB;
OBJWORK ObjWorkBuff[128] = {};
qword *nkDstAdr = NULL;
static u32 *nkLoadBuff;

// Unknown
u8 *DAT_003fb93c = NULL;

void nkGoPlayDemo() {
    GameGbl.kpd[0].cnt = 1;
    hrpt_deb = 0;
}

void _nkGetMapBuf() {
    // Empty function
}

void nkDrawFilter(s32 prty) {
    sceGsDrawEnv1 *env1;
	sceGsDrawEnv1 *env1_d;
    
    if (GameGbl.filter_prty != prty || GameGbl.filter_cnt == 0.0f)
        return;

    if ((GameGbl.fr + 1 & 1) != 0) {
        env1 = &GameGbl.db.draw11;
        env1_d = &GameGbl.db.draw01;
    } else {
        env1 = &GameGbl.db.draw01;
        env1_d = &GameGbl.db.draw11;
    }

    switch (GameGbl.filter_type) {
        case 1:
            nkDrawFilterShadeOff(env1, env1_d, GameGbl.filter_cnt);
            break;
        case 2:
            nkDrawBlackIn(GameGbl.filter_cnt);
            break;
        case 3:
            nkDrawBlackIn(1.0f - GameGbl.filter_cnt);
            break;
    }

    sceGsSyncPath(0, 0);
}

void nkInitPS2() {
    SysGbl.objwork = ObjWorkBuff;
    SysGbl.n_objw = 128;
    nkDG.cam_debug_mode = 0;
    nkDG.flag = 0;
    sceGsResetPath();
    sceDevGifReset();
    sceDevVif1Reset();
    sceDevVu1Reset();
    sceDevVif0Reset();
    sceDevVu0Reset();
    nkInitDma();
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    nkGsInitEnv();
}

static s32 GameInit() {
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

static s32 GameLoad() {
    s32 ret;
    
    if (nkLoadBun == 0) {
        switch (nkLoadStat) {
            case 0:
                hGameRead(0, (s32)nkLoadBuff);
                break;
            case 1:
                hGameRead(1, (s32)nkLoadBuff);
                break;
        }
        
        nkLoadBun++;
    }

    if (GameGbl.vision != 0x0106 && DAT_003fb93c != NULL) {
        nkLoadGms(DAT_003fb93c);
        sceGsSyncPath(0, 0);
    }

    TkMainLoop(1);
    sceGsSyncPath(0, 0);
    OkPFontFlush(GameGbl.kpd[1].rep & 0x100);
    hLoopBottom();
    GameGbl.inter = sceGsSyncV(0) ^ 1;
    if (GameGbl.fr & 1) {
        sceGsSetHalfOffset(&GameGbl.db.draw11, 2048, 2048, GameGbl.inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw12, 2048, 2048, GameGbl.inter);
    } else {
        sceGsSetHalfOffset(&GameGbl.db.draw01, 2048, 2048, GameGbl.inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw02, 2048, 2048, GameGbl.inter);
    }
    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.fr++;
    
    if (kzDrawNowload() == 1) {
        switch (nkLoadStat) {
            case 0:
                abStageInitA();
                hInitStage0();
                kzStageInit0();
                nkStageInit0();
                break;
            case 1:
                kzSetDispMaskOn();
                nkInitPS2();
                NakanoPackAdr = (u32 *)hGetDataAddr(0);
                nkStageInit1();
                SysGbl.smode++;
                nkGsSetNormalFZ2_0();
                nkWipeEffBClear();
                nkGsSetNormalFZ2_1();
                nkWipeEffBClear();
                break;
        }

        nkLoadBun = 0;
        nkLoadStat++;
    }

    return 0;
}

static s32 GameMain() {
    kPadDATA *kpd0;
	kPadDATA *kpd1;
	OBJWORK *klo;
	CAM_WORK camDummy;
	HERO_WORK *herow;
    
    nkResetMeter();
    if (hrmirflush == 0)
        nkPathFlush();
    
    kpd0 = &GameGbl.kpd[0];
    nkGetPad();
    kpd0->lvl = nkGetPlvl(0);
    kpd0->trg = nkGetPtrg(0);
    kpd0->rep = nkGetPrep(0);
    kpd0->looks = nkGetPlooks(0);
    kpd1 = &GameGbl.kpd[1];
    kpd1->lvl = nkGetPlvl(1);
    kpd1->trg = nkGetPtrg(1);
    kpd1->rep = nkGetPrep(1);
    kpd1->looks = nkGetPlooks(1);

    nkPathReset();
    FlushCache(0);
    nkVisionStartMain();
    MapVDrawBG();
    kzGameMainCL();
    if (GameGbl.pause_flag == 0) {
        SortObjWork(SysGbl.objwork,SysGbl.n_objw);
        ObjFunc(SysGbl.objwork);
        SetObjAllPause(0);
    } else {
        SetObjAllPause(1);
    }
    if (GameGbl.playdemo_flag) {
        nkKeyframePlay();
        GameGbl.playdemo_time--;
        if (GameGbl.playdemo_time > 60 && PAD_TRG_START(GameGbl.kpd[0])) {
            GameGbl.playdemo_time = 60;
            GameGbl.playdemo_flag = 2;
        }
        if (GameGbl.playdemo_time == 60) {
            nkSetFilterObj(3, 60, 0);
            hGameAreaEnd(-1);
        }
        if (GameGbl.playdemo_time <= 0) {
            if (GameGbl.playdemo_flag == 2)
                kzReturnTitle(3);
            else
                kzReturnTitle(1);
            hrpt_deb = 1;
            GameGbl.playdemo_flag = 0;
        }
    }

    nkSetMeter();
    if (OkZakoFuncOn)
        OkZakoJobMain();
    ht_MainGbl();
    abEffectMain(kpd0, kpd1);
    if (GameGbl.vision >> 8 < 0x1E && GameGbl.pause_flag == 0)
        CamControl();
    CamCalMatrix(&GameGbl.cam);

    if (!(GameGbl.flag & 1)) {
        if (nkDG.map_draw_flag == 0) {
            hrMainWork();
            nkMirDraw();
            if (hrmirflush) {
                if (!hr_check_mir()) {
                    hrPathClear();
                }
                hrPathFlushOld();
            }
            nkVT_ExecMovie();
            hrMainDraw();
        } else {
            nkDrawHitModel();
        }
    }

    nkSetMeter();
    if (hr_abeoff == 0) {
        Vu0ProgTrans();
        OutLineEnvInit(GameGbl.fr, GameGbl.inter);
    }
    sceVu0CopyMatrix(Scr.WvMtx, GameGbl.wvm);
    sceVu0CopyMatrix(Scr.VsMtx, GameGbl.vsm);
    sceVu0CopyMatrix(Scr.WsMtx, GameGbl.wsm);

    if (!(GameGbl.flag & 2)) {
        hrDrawVpo_st();
        DrawObjFunc(SysGbl.objwork);
        nkSetMeter();
        hEvInitFrame();
        hEvObjMain();
        kzGameMainWM();
        hrDrawVpo_end();
        nkSetMeter();
    }

    hGameMain();
    sceGsSyncPath(0, 0);
    if (hr_abeoff == 0 && !(GameGbl.flag & 2U)) {
        abEff2Begin();
        DrEffObjFunc(SysGbl.objwork);
        abEff2End();
    }

    nkSetMeter();
    func_0021D9F8();
    nkSetMeter();
    nkDrawFilter(1);
    if (GameGbl.vision >> 8 < 0x1E)
        nkKloMissMain();
    DrawGameScreen();
    ht_GblDrawScreen();
    if (GameGbl.playdemo_flag)
        kzDrawPressStartDemo();
    nkSetMeter();
    kzGameMainTD();
    nkDrawFilter(0);
    SysGbl.proc_hcnt = *T0_COUNT;
    sceGsSyncPath(0, 0);
    SysGbl.proc_hcnt = *T0_COUNT;
    nkSetMeter();

    GameGbl.fcnt0 += M_PI_2f / 10.0f;
    GameGbl.fcnt0 = nkRadMask(GameGbl.fcnt0);

    nkSetMeter();
    if (GameGbl.vision != 0x1B01 && GameGbl.wipe.outtime > 0)
        nkDrawWipe();
    nkSetMeter();
    if (fontDispID != 0)
        nkDrawWorkMeter();
    
    if (GameGbl.reverse_cnt > 0.0f) {
        klo = GameGbl.klonoa;
        GameGbl.reverse_cnt -= 1.0f;
        if (GameGbl.reverse_cnt == 0.0f) {
            herow = (HERO_WORK *)klo->work;
            SetRouteNo(&klo->rtw, GameGbl.wipe.rtn);
            SetMichinori(&klo->rtw, GameGbl.wipe.mcn);
            MichiPosi(&klo->rtw, klo->posi);
            if ((klo->reg0 & 1024) != 0) {
                herow->gimm_flag &= 4294965247U;
                klo->reg0 &= -1025;
            } else {
                herow->gimm_flag |= 2048U;
                klo->reg0 |= 1024;
            }
            CamVWorkInit(klo, &camDummy);
            CamReverseInit();
        }
    }

    hLoopBottom();
    GameGbl.inter = sceGsSyncV(0) ^ 1;
    if (GameGbl.fr & 1) {
        sceGsSetHalfOffset(&GameGbl.db.draw11, offset_X, offset_Y, GameGbl.inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw12, offset_X, offset_Y, GameGbl.inter);
    } else {
        sceGsSetHalfOffset(&GameGbl.db.draw01, offset_X, offset_Y, GameGbl.inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw02, offset_X, offset_Y, GameGbl.inter);
    }
    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.fr++;

    kzSetDispMaskOff();
    GameFlow();

    if ((PAD_TRG_START(GameGbl.kpd[0]) || GameGbl.pause_flag == 0 && kpd0->looks == 0) && hr_pt_check() != 1 && GameGbl.kloLife[0] > 0 && GameGbl.playdemo_flag == 0 && GameGbl.wipe.outtime == 0 && GameGbl.wipe.intime == 0 && GameGbl.area_cnt > 60)
        kzPauseOn();

    if (GameGbl.pause_flag2 && GameGbl.non_pause_flag == 0)
        GameGbl.pause_flag = 1;
    else
        GameGbl.pause_flag = 0;
    GameGbl.retry_bun1++;
    
    return 0;
}
