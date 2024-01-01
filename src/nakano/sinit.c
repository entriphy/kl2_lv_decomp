#include "nakano/sinit.h"
#include "nakano/dma.h"
#include "nakano/objfunc.h"
#include "nakano/game.h"
#include "nakano/main.h"
#include "nakano/camera.h"
#include "nakano/light.h"
#include "nakano/nk2pcon.h"
#include "nakano/gscreen.h"
#include "nakano/hlzdeco.h"
#include "nakano/route.h"
#include "nakano/nkgf.h"
#include "nakano/wipe.h"
#include "nakano/gssub.h"
#include "harada/hr_main.h"
#include "harada/hr_mapdr.h"
#include "hoshino/h_file.h"
#include "hoshino/h_menu.h"
#include "hoshino/h_game.h"
#include "okanoyo/okio.h"
#include "okanoyo/okmain.h"
#include "okanoyo/okprint.h"
#include "take/object.h"

void InitAllObjWork2(OBJWORK *objwork, s32 n_objw) {
    OBJWORK *objw;
    s32 lp1;

    for (lp1 = 0, objw = objwork; lp1 < n_objw; lp1++, objw++) {
        if (objw->stat0 == 1) {
            if (objw->stat1 == 1) {
                objw->bun0 = 0;
                objw->bun1 = 0;
                continue;
            }
            if (objw->stat1 == 2) {
                objw->bun0 = 0;
                continue;
            }
        }
        InitObjWork(objw);
    }
}



void nkStageInit0() {
    u32 *adr;
    s32 Stage;

    Stage = GameGbl.vision >> 8;
    if (Stage - 30U > 5) {
        adr = (u32 *)hGetDataAddr(0);
        if (adr == NULL)
            return;
        nkLoadGms((qword *)((u32)adr + adr[1]));
        sceGsSyncPath(0, 0);
    }
}

// TODO: remove these once implemented
extern void NkDeb2PTbl();
extern void OkDebugGame();
extern void HsDeb2PTbl();
extern void camtest();
extern void HtDeb2PTbl();

void nkAreaInit() {
    qword *adr;
    s32 ret;

    GameGbl.flag = 0;
    memoryAreaFormat();
    SfxSystemInit(&PsfxWorkBuff);
    if (!(GameGbl.flag & 4)) {
        adr = (qword *)getBuff(1, 0x10, NULL, &ret);
        nkDstAdr = adr;
        ret = (u32)nkDecodeData(((u32)NakanoPackAdr + NakanoPackAdr[1]), nkDstAdr) - (u32)adr;
        getBuff(1, ret - 0x10, NULL, &ret);
    }
    FlushCache(0);
    FlushCache(0);
    InitHitPolyIndex();
    CamInit(&GameGbl.cam);
    if (!(nkDG.flag & 8)) {
        GameGbl.cam.mode = 1;
    }
    memset(&hG, 0, sizeof(hGLOBAL));
    hG.dmaGif = sceDmaGetChan(2);
    hPkInit(&hG);
    hPkSetBp(10000);
    MapConfFileRead();
    _nkLightInit();
    GameGbl.fr = 0;
    func_00104508();
    hrInitWork();
    OkZakoInitArea();
    abAreaInit();
    ht_InitGblArea();
    kzAreaInit();
    OkSetIDprofile();
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 12, 6);
    OkPrintSysInit(1, SCR_WIDTH, SCR_HEIGHT, 12, 6);
    OkPrintSysInit(2, SCR_WIDTH, SCR_HEIGHT, 12, 8);
    okzcom_mode = 0;
    OkDebugGameMode = 0;
    NkDeb2PTblInit();
    OkDebugGame2pConSet(1, NkDeb2PTbl);
    OkDebugGame2pConSet(2, OkDebugGame);
    HsDeb2PTblInit();
    OkDebugGame2pConSet(3, HsDeb2PTbl);
    OkDebugGame2pConSet(4, camtest);
    OkDebugGame2pConSet(5, camtest);
    HtDeb2PTblInit();
    OkDebugGame2pConSet(6, HtDeb2PTbl);
    GameGbl.pause_flag = 0;
    GameGbl.non_pause_flag = 0;
    GameGbl.pause_flag2 = 0;
    GameGbl.init_vision = 0;
    GameGbl.wipe.outtime = 0;
    GameGbl.wipe.next_vision = 0;
    GameGbl.area_cnt = 0;
    GameGbl.tokurom_cnt = 0xFFFF;
    GimAreaInit(GameGbl.vision);
    if (!(GameGbl.flag & 4) || GameGbl.vision > 0x1E04) {
        InitGScreen();
    }
    nkVT_InitWave();
    hGameAreaStart(0);
    if (GameGbl.vision >> 8 < 30) {
        nkSetPopuka();
        nkRtConnMng(GameGbl.klonoa);
    }
    nkSetFilterObj(2, 30, 0);
    nkPathReset();
    FlushCache(0);
    hrInitAtrMain();
    nkPathFlush();
    sceDmaSync(DmaChVIF1, 0, 0);
    nkPathReset();
    InitStageGScreen();
}

qword* nkDecodeData(qword *src, qword *dst) {
    qword *adr;
    qword *radr;
    qword *area_adr;
    vpmINFO *vpmi = &VpmInfo;
    s32 area;

    area = GameGbl.vision & 0xFF;
    if (area < GameGbl.area_max) {
        area_adr = (qword *)((u32)src + (*src)[area + 2]);

        adr = (qword *)((u32)area_adr + area_adr[0][1]);
        HLZDecode((u8 *)adr, (u8 *)dst, (HLZWork *)0x70000000);
        nkLoadGms(dst);
        sceGsSyncPath(0, 0);

        adr = (qword *)((u32)area_adr + area_adr[0][2]);
        radr = (qword *)HLZDecode((u8 *)adr, (u8 *)dst, (HLZWork *)0x70000000);
        vpmi->data_buff = (u8 *)dst;
        DecodeVpm((u32 *)dst);
        vpmi->vpm_data_top = (u32 *)vpmi->data_buff;
        dst = radr;

        adr = (qword *)((u32)area_adr + area_adr[0][3]);
        GameGbl.route_max = InitMichi((u32 *)adr);

        adr = (qword *)((u32)area_adr + area_adr[1][0]);
        radr = (qword *)HLZDecode((u8 *)adr, (u8 *)radr, (HLZWork *)0x70000000);
        InitHitPoly(dst);

        adr = (qword *)((u32)area_adr + area_adr[1][1]);
        if (adr[0][0] == -1 && adr[1][0] == -1 && adr[2][0] == -1 && adr[3][0] == -1) {
            CamDataNone();
            nkDG.flag |= 8;
        } else {
            CamDataInit((s32 *)adr);
            nkDG.flag &= -9;
        }

        adr = (qword *)((u32)area_adr + area_adr[1][2]);
        if (adr[0][0] == -1 && adr[1][0] == -1 && adr[2][0] == -1 && adr[3][0] == -1) {
            LgtDataNone();
            nkDG.flag |= 0x10;
        } else {
            LgtDataInit((s32 *)adr);
            nkDG.flag &= -0x11;
        }

        return radr;
    }
}

void nkStageInit1() {
    qword *adr;
    OBJWORK *objw;

    nkGsSetNormalFZ2_0();
    nkWipeEffBClear();
    nkGsSetNormalFZ2_1();
    nkWipeEffBClear();
    nkGsSetEffectFZ2();
    nkWipeEffBClear();
    GameGbl.stage_flag = 0;
    InitAllObjWork(SysGbl.objwork, SysGbl.n_objw);
    if (NakanoPackAdr != NULL) {
        adr = (qword *)((u32)NakanoPackAdr + NakanoPackAdr[1]);
        GameGbl.area_max = (*adr)[0] - 1;
        adr = (qword *)((u32)adr + (*adr)[1]);
        if (adr[0][0] == -1 && adr[1][0] == -1 && adr[2][0] == -1 && adr[3][0] == -1) {
            nkDG.flag |= 4;
        } else {
            nkDG.flag &= -5;
            nkRtConnInit(adr);
        }
        SetSfxData((char *)((u32)NakanoPackAdr + NakanoPackAdr[2]));
    }
    OkZakoInitStage();
    GameGbl.kloZanki[0] = kzLoadRemain();
    hInitStage1();
    kzStageInit1();
    if (NakanoPackAdr != NULL) {
        GimStageInit(GameGbl.vision);
    }
    hrStageDataLoad(0);
    abStageInitB();
    ht_InitGbl();
    nkGfInit();
    if (GameGbl.vision >> 8 < 30) {
        objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
        objw->stat0 = 1;
        objw->stat1 = 1;
        objw->bun0 = 0;
        objw->bun1 = 0;
        objw->prty = 128;
        GameGbl.klonoa = objw;
        GameGbl.kloLife[0] = 3;

        objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
        objw->stat0 = 1;
        objw->stat1 = 2;
        objw->prty = 128;
        objw->bun0 = 0;
        GameGbl.kazd = objw;
    }

    memoryAreaptrSet();
    nkAreaInit();
    if (GameGbl.vision >> 8 < 30) {
        nkSetRtConn(GameGbl.klonoa);
    }
    InitStageGScreen();
    nkVisionStartInit();

    GameGbl.Re_st_rtn = -1;
    GameGbl.Re_st_mcn = 0;
    GameGbl.Re_st_klomode = 0;
    GameGbl.Re_st_height = 0.0f;
    GameGbl.filter_cnt = 0.0f;
    GameGbl.filter_type = 1;
    GameGbl.reverse_cnt = 0.0f;
    GameGbl.time_atack_cnt = 0;
    GameGbl.vs_clear_cnt = 0;
    GameGbl.fcnt0 = 0.0f;
    GameGbl.fcnt1 = 0.0f;
    GameGbl.fcnt2 = 0.0f;
    GameGbl.popuka_flag = 0;

    switch (GameGbl.vision >> 8) {
        case 18:
            GameGbl.time_atack_best = kzLoadExtraTime(0);
            break;
        case 19:
            GameGbl.time_atack_best = kzLoadExtraTime(1);
            break;
    }

    if (kzCheckGenju() == 1) {
        switch (GameGbl.vision >> 8) {
            case 21:
                GameGbl.time_atack_best = kzLoadGenjuTime(0);
                break;
            case 22:
                GameGbl.time_atack_best = kzLoadGenjuTime(1);
                break;
            case 23:
                GameGbl.time_atack_best = kzLoadGenjuTime(2);
                break;
            case 24:
                GameGbl.time_atack_best = kzLoadGenjuTime(3);
                break;
            case 26:
                GameGbl.time_atack_best = kzLoadGenjuTime(4);
                break;
            case 27:
                GameGbl.time_atack_best = kzLoadGenjuTime(5);
                break;
        }
    }

    __asm__ volatile(
        "mfc1 $6, %0\n"
        "qmtc2 $6, $vf4\n"
        "vrinit R, $vf4x\n"
    : : "f" (123.4f));
}

void nkChangeArea(u32 vision, s32 rtn, s32 mcn, f32 height) {
    GameGbl.init_vision = vision;
    GameGbl.st_rtn = rtn;
    GameGbl.st_mcn = mcn;
    GameGbl.st_height = height;
    GameGbl.st_flag |= 8;
}

void nkChangeArea2(u32 vision, s32 rtn, s32 mcn, s32 muki, f32 height) {
    GameGbl.init_vision = vision;
    GameGbl.st_rtn = rtn;
    GameGbl.st_mcn = mcn;
    GameGbl.st_height = height;
    GameGbl.st_flag |= 8;
}
