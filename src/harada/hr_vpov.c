#include "harada/hr_vpov.h"
#include "harada/hr_mapdr.h"
#include "harada/hr_mapv.h"
#include "harada/hr_anmvp.h"
#include "harada/hr_nak.h"
#include "harada/hr_pall.h"
#include "harada/hr_pefc.h"
#include "nakano/game.h"
#include "okanoyo/okio.h"
#include "okanoyo/okprint.h"
#include "nakano/main.h"
#include "nakano/dma.h"
#include "nakano/nkpad.h"

static s32 VPOvInit();
static s32 VPOvMain();

s32 (*hrVpoVFuncTbl[2])() = {
    VPOvInit,
    VPOvMain
};
static HRVPOV *hrvpov;

static void hr_set_vpo(s32 no) {
    hrvpov->info = hrget_vpoinfo(no);
    hrvpov->infono = no;
    hrvpov->objmax = hrvpov->info->ntag;
    hrvpov->objno = 0;
}

static s32 VPOvInit() {
    vpmINFO *vpmi;
    u32 i;

    vpmi = &VpmInfo;
    nkInitPS2();
    hr_okmem_reset();
    {
        s32 i;
        hrvpov = (HRVPOV *)getBuff(1, sizeof(HRVPOV), "deb", &i);
    }
    hrGetMapBuf();
    hr_nak_init_allobj();
    nkVT_InitWave();
    MapConfFileRead();
    hrDataLoad();
    hr_nak_dummyVK();
    hrInitWorkDeb();
    hr_anmVPM_allinit();
    hfmcam.posmode = 1;
    hrMapFileRead();
    OkSetIDprofile();
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 16, 8);
    OkPrintSysInit(4, SCR_WIDTH, SCR_HEIGHT, 10, 6);
    CamInit(&GameGbl.cam);
    GameGbl.cam.posi[2] = -640.0f;
    draw_id = 0;

    vpmi->block_list[0] = 0;
    for (i = 0; i < 2048; i++) {
        vpmi->block_list[i + 1] = 0;
    }

    nkPathReset();
    hrInitAtrMain();
    sceGsSyncPath(0,0);
    nkDebFlush();
    nkPathFlush();
    GameGbl.fr = 0;
    hr_set_vpmfog(&VpmInfo);
    hr_pfade_init(&ppsys.fade);
    memset(hrvpov, 0, sizeof(HRVPOV));

    hrvpov->info = hrget_vpoinfo(0);
    if (hrvpov->info == NULL) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    } else {
        hrvpov->infomax = *hr_obcvpf;
        hr_set_vpo(0);
        SysGbl.smode++;
        printf("VPOviewer init end\n");
    }

    return 0;
}

static void hr_vpov_menu(kPadDATA *kpd0) {
    s32 irep;
    s32 sank;
    s32 batu;
    s32 mno;
    f32 frep;

    if (pPAD_REP_UP(kpd0)) {
        hrvpov->mno--;
        if (hrvpov->mno < 0) {
            hrvpov->mno = 7;
        }
    } else if (pPAD_REP_DOWN(kpd0)) {
        hrvpov->mno++;
        if (hrvpov->mno > 7) {
            hrvpov->mno = 0;
        }
    }

    mno = hrvpov->mno;

    if (pPAD_REP_LEFT(kpd0)) {
        irep = -1;
        frep = -0.125f;
    } else if (pPAD_REP_RIGHT(kpd0)) {
        irep = 1;
        frep = 0.125f;
    } else {
        irep = 0;
        frep = 0.0f;
    }

    if (pPAD_LVL_TRIANG(kpd0)) {
        sank = 1;
    } else {
        sank = 0;
    }

    if (pPAD_LVL_CROSS(kpd0)) {
        batu = 1;
    } else {
        batu = 0;
    }

    if (irep != 0) {
        switch (hrvpov->mno) {
            case 0:
                hrvpov->infono += irep;
                if (hrvpov->infono < 0) {
                    hrvpov->infono = 0;
                } else if (hrvpov->infono >= hrvpov->infomax) {
                    hrvpov->infono = hrvpov->infomax - 1;
                } else {
                    if (irep != 0) {
                        hr_set_vpo(hrvpov->infono);
                    }
                }
                break;
            case 1:
                hrvpov->objno += irep;
                if (hrvpov->objno < 0) {
                    hrvpov->objno = 0;
                } else if (hrvpov->objno >= hrvpov->objmax) {
                    hrvpov->objno = hrvpov->objmax - 1;
                }
                break;
            case 2:
            case 3:
            case 4:
                if (batu) {
                    frep *= 80.0f;
                } else if (sank) {
                    frep *= 8.0f;
                }
                hrvpov->pos[mno - 2] += frep;
                break;
            case 5:
            case 6:
            case 7:
                if (batu) {
                    frep *= 80.0f;
                } else if (sank) {
                    frep *= 8.0f;
                }
                mno -= 5;
                hrvpov->rot[mno] += frep;

                if (hrvpov->rot[mno] > 180.0f) {
                    hrvpov->rot[mno] -= 360.0f;
                } else if (hrvpov->rot[mno] < -180.0f) {
                    hrvpov->rot[mno] += 360.0f;
                }
                break;
        }
    }
}

static void make_lightm() {
    sceVu0FMATRIX nlm;
    sceVu0FMATRIX lcm;
    sceVu0FVECTOR v0;
    sceVu0FVECTOR v1;
    sceVu0FVECTOR v2;

    v0[0] = v0[1] = v0[2] = v0[3] = 0.0f;
    v1[0] = v1[1] = v1[3] = 0.0f;
    v1[2] = 1.0f;
    sceVu0NormalLightMatrix(nlm, v1, v0, v0);
    v0[0] = v0[1] = v0[2] = v0[3] = 1.0f;
    v1[0] = v1[1] = v1[2] = 0.0f;
    v1[3] = 1.0f;
    v2[0] = v2[1] = v2[2] = 0.0f;
    v2[3] = 1.0f;
    sceVu0LightColorMatrix(lcm, v0, v1, v1, v2);
    hrDrawVpoLt(nlm, lcm);
}

static s32 VPOvMain() {
    s32 work;
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

    nkPathReset();
    FlushCache(0);
    hrCamMonitor(&GameGbl.cam);
    CamDebug(&GameGbl.cam);
    CamCalMatrix(&GameGbl.cam);

    if (pPAD_TRG_START(kpd0)) {
        hrvpov->mon ^= 1;
    }
    if (hrvpov->mon != 0) {
        hr_vpov_menu(kpd0);
    }

    hrSetMapREG();
    nkP1Flush(p1_ot);

    sceVu0ScaleVectorXYZ(hrvpov->rotp, hrvpov->rot, M_PIf);
    sceVu0DivVectorXYZ(hrvpov->rotp, hrvpov->rotp, 180.0f);
    hrvpov->rotp[3] = 1.0f;
    hrvpov->pos[3] = 1.0f;

    sceVu0UnitMatrix(hrvpov->m);
    sceVu0RotMatrixZ(hrvpov->m, hrvpov->m, hrvpov->rotp[2]);
    sceVu0RotMatrixX(hrvpov->m, hrvpov->m, hrvpov->rotp[0]);
    sceVu0RotMatrixY(hrvpov->m, hrvpov->m, hrvpov->rotp[1]);
    sceVu0TransMatrix(hrvpov->m, hrvpov->m, hrvpov->pos);
    sceVu0MulMatrix(hrvpov->m, GameGbl.wsm, hrvpov->m);

    hrDrawVpo_st();
    make_lightm();
    hrDrawVpoi(hrvpov->m, hrvpov->info, hrvpov->objno);
    hrDrawVpo_end();

    sceGsSyncPath(0, 0);
    work = *T0_COUNT;
    nkSetMeter();
    nkDrawWorkMeter();
    OkPFontFlush(PAD_TRG_SELECT(GameGbl.kpd[1]));
    inter = sceGsSyncV(0) ^ 1;
    GameGbl.inter = inter;
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
    FlushCache(0);
    sceGsSyncPath(0,0);
    nkPathFlush();
    if (PAD_TRG_SELECT(GameGbl.kpd[0])) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
        sceGsSyncPath(0,0);
    }

    return 0;
}