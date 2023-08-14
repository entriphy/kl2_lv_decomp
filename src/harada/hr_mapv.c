#include <sifdev.h>
#include <eeregs.h>
#include "harada/hr_mapv.h"
#include "harada/hr_mapdr.h"
#include "harada/hr_anmdt.h"
#include "harada/hr_anmvp.h"
#include "harada/hr_main.h"
#include "harada/hr_pall.h"
#include "hoshino/h_sound.h"
#include "hoshino/h_game.h"
#include "nakano/main.h"
#include "nakano/dma.h"
#include "nakano/game.h"
#include "okanoyo/okio.h"
#include "okanoyo/okprint.h"

extern u32 hr_vu1m_wire __attribute__((section(".vudata")));
extern u32 hr_vu1m_ioff __attribute__((section(".vudata")));
extern u32 hr_vu1m_fogc __attribute__((section(".vudata")));

static s32 MapvInit();
static s32 MapvMain();
static void hr_mv_m_slint(s32 irep, f32 frep);
static void hr_mv_m_slint2(s32 irep, f32 frep);

static char *lookname[4] = {"normal", "side", "down", "up"};
static char *mapbg_name[3] = {"map", "bg", "mapbg"};
static char *avt_name[3] = {"NoAnm", "SLint", "SLint2"};
static HRANMVS hr_avt_tbl[2] = { {NULL, hr_avw_sint, 3, &VpmInfo}, {NULL, hr_avw_sint2, 4, &VpmInfo} };
static void (*hr_mv_avt_tbl[2])(s32, f32) = {hr_mv_m_slint, hr_mv_m_slint2};
s32 (*hrMapVFuncTbl[2])() = {MapvInit, MapvMain};
s32 (*HrPtFuncTbl[2])() = {hrPtInit, hrPtMain};
static u8 *hr_gmsdata;
static u8 *hr_gmsdatab;
static HR_MAPVS *hrmvb;
static s32 MapVMenu;
static s32 MapVMsw;
static s32 pt_pause;
static s32 pt_koma;
static s32 pt_posmv;
static PCAMSC ptmp_wp;
static PCAMSC ptmp_wi;
static PCAMSC ptmp_wa;
static PCAMSC ptmp_mpr;
static PCAMSC ptmp_mir;
static PCAMSL ptmp_len;
static HPVSEL pcsel;
static s32 pt_lightno;
static s32 *Dmir_addr;
static char MapName[256];

void hr_avw_sint(HRANMV *av) {
    hr_set_vlight(&hrvlight[0], 0.0f, 0.0f, -3000.0f, 3.0f, 3.5f, 4.0f, 200.0f, 1000.0f);
    hr_set_vlightMini(&hrvlight[0], 0.1f, 0.2f, 0.3f);
    av->flag |= 2;
}

void hr_avw_sint2(HRANMV *av) {
    hr_set_vlight(&hrvlight[0], 0.0f, 0.0f, -2000.0f, 3.0f, 1.0f, 1.0f, 1.0f, 1000.0f);
    hr_set_vlight(&hrvlight[1], 0.0f, 0.0f, -3000.0f, 1.0f, 1.0f, 3.0f, 1.0f, 1500.0f);
    hr_set_vlightMini(&hrvlight[0], 0.1f, 0.2f, 0.3f);
    hr_set_vlightMini(&hrvlight[1], 0.3f, 0.2f, 0.1f);
    av->flag = av->flag | 2;
}

// Technically matches
void hr_crossline(sceVu0FVECTOR v, u32 wh, u32 r, u32 g, u32 b, u32 fg) {
    ATR_HRCL *pp;
    sceVu0IVECTOR iv;
    u32 t1;
    u32 h;
    u32 sz;
    f32 f;

    sceVu0RotTransPers(iv, GameGbl.wsm, v, 0);
    wh /= 2;
    wh *= 16;
    h = wh >> 1;
    f = 1.0f;
    t1 = *(u32 *)&f;
    sz = fg != 0 ? iv[2] : -2;

    pp = (ATR_HRCL *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_HRCL) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    pp->viftag[0] = pp->viftag[1] = pp->viftag[2] = 0;
    pp->viftag[3] = SCE_VIF1_SET_DIRECT(13, 0);
    ((u64 *)pp->gif0)[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 0, 0, 0, 0, 0), 0, 3);
    ((u64 *)pp->gif0)[1] = SCE_GS_ST << 0 | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;

    pp->stq0[0] = 0;
    pp->stq0[1] = 0;
    pp->stq0[2] = t1;
    pp->stq0[3] = 0;
    pp->rgb0[0] = r;
    pp->rgb0[1] = g;
    pp->rgb0[2] = b;
    pp->rgb0[3] = 128;
    pp->xyz0[0] = iv[0] - 16;
    pp->xyz0[1] = iv[1] - h;
    pp->xyz0[2] = sz;
    pp->xyz0[3] = 0;

    pp->stq1[0] = t1;
    pp->stq1[1] = t1;
    pp->stq1[2] = t1;
    pp->stq1[3] = 0;
    pp->rgb1[0] = r;
    pp->rgb1[1] = g;
    pp->rgb1[2] = b;
    pp->rgb1[3] = 128;
    pp->xyz1[0] = iv[0] + 16;
    pp->xyz1[1] = iv[1] + h;
    pp->xyz1[2] = sz;
    pp->xyz1[3] = 0;

    pp->stq2[0] = 0;
    pp->stq2[1] = 0;
    pp->stq2[2] = t1;
    pp->stq2[3] = 0;
    pp->rgb2[0] = r;
    pp->rgb2[1] = g;
    pp->rgb2[2] = b;
    pp->rgb2[3] = 128;
    pp->xyz2[0] = iv[0] - wh;
    pp->xyz2[1] = iv[1] - 16;
    pp->xyz2[2] = sz;
    pp->xyz2[3] = 0;

    pp->stq3[0] = t1;
    pp->stq3[1] = t1;
    pp->stq3[2] = t1;
    pp->stq3[3] = 0;
    pp->rgb3[0] = r;
    pp->rgb3[1] = g;
    pp->rgb3[2] = b;
    pp->rgb3[3] = 128;
    pp->xyz3[0] = iv[0] + wh;
    pp->xyz3[1] = iv[1] + 16;
    pp->xyz3[2] = sz;
    pp->xyz3[3] = 0;

    FlushCache(0);
    pp++;
    p1_packet = (qword *)pp;
}

void hrMapFileRead() {
    vpmINFO *vpmi;
    char FileName[128];
    char VpmName[128];
    char GmsName[128];
    char RtName[128];
    char HmpName[128];
    u32 *addr;
    s32 i;
    char BgName[128];

    vpmi = &VpmInfo;
    strcpy(FileName, "host0:./dat/st");
    hr_stage_no(FileName, 1);
    strcpy(BgName, FileName);
    strcat(BgName, "/bg0");
    hr_stage_no(BgName, 0);
    strcat(FileName, "/map");
    hr_stage_no(FileName, 0);
    strcpy(MapName, "Map");
    hr_stage_no(MapName, 0);
    strcpy(GmsName, FileName);
    strcat(GmsName, ".gms");
    hr_gmsdata = (u8 *)getBuff(0, 0, GmsName, &i);
    if ((s32)hr_gmsdata != -1) {
        nkLoadGms((qword *)hr_gmsdata);
        printf("LoadGms....ok\n");
        sceGsSyncPath(0, 0);
    }
    strcat(BgName, ".gms");
    hr_gmsdatab = (u8 *)getBuff(0, 0, BgName, &i);
    strcpy(VpmName, FileName);
    strcat(VpmName, ".vpm");
    read_file(VpmName, vpmi->data_buff);
    DecodeVpm(vpmi->data_buff);
    vpmi->vpm_data_top = (u32 *)vpmi->data_buff;
    printf("DecodeVpm....ok\n");
    strcpy(RtName, FileName);
    strcat(RtName, ".rt");
    addr = (u32 *)getBuff(0, 0, RtName, &i);
    if ((s32)addr != -1) {
        InitMichi(addr);
    }
    strcpy(HmpName, FileName);
    strcat(HmpName, ".hmp");
    addr = (u32 *)getBuff(0, 0, HmpName, &i);
    if ((s32)addr != -1) {
        InitHitPoly(addr);
    }
}

static void MapConfFileWrite() {
    s32 wfd;
    char FileName[128];
    char MirName[128];

    strcpy(FileName, "host0:./dat/st");
    hr_stage_no(FileName, 1);
    strcat(FileName, "/map");
    hr_stage_no(FileName, 0);
    strcpy(MirName, FileName);
    strcat(FileName, ".mvc");
    wfd = sceOpen(FileName, SCE_WRONLY | SCE_CREAT | SCE_TRUNC);
    if (wfd < 0) {
        printf("Can't open file \"%s\"\n", FileName);
    } else {
        sceWrite(wfd, &mapvw, sizeof(MAPVWORK));
        sceClose(wfd);
        printf("Writing [%s] file.... ok!\n", FileName);
    }

    if (hrmvb->mir_fg != 0 && hr_mirsys_ver(hcm_addr) == 1) {
        strcat(MirName, ".mrc");
        wfd = sceOpen(MirName, SCE_WRONLY | SCE_TRUNC);
        if (wfd < 0) {
            printf("Can't open file \"%s\"\n", MirName);
        } else {
            sceWrite(wfd, hcm_addr, hcm_size);
            sceClose(wfd);
            printf("Writing [%s] file.... ok!\n", MirName);;
        }
        hrmvb->mir_fg = 0;
    }
}

static u16 hr_read_gmsh(HRGMSH *gmsh, qword *data, u64 bb, u16 psm) {
    gmsh->gif = data - 1;
    gmsh->dpsm = psm;
    gmsh->dbp = (bb >> GS_BITBLTBUF_DBP_O) & (GS_BITBLTBUF_DBP_M >> GS_BITBLTBUF_DBP_O);
    gmsh->dbw = (bb >> GS_BITBLTBUF_DBW_O) & (GS_BITBLTBUF_DBW_M >> GS_BITBLTBUF_DBW_O);
    data++;
    bb = *(u64 *)data;
    gmsh->dsax = (bb >> GS_TRXPOS_DSAX_O) & (GS_TRXPOS_DSAX_M >> GS_TRXPOS_DSAX_O);
    gmsh->dsay = (bb >> GS_TRXPOS_DSAY_O) & (GS_TRXPOS_DSAY_M >> GS_TRXPOS_DSAY_O);
    data++;
    bb = *(u64 *)data;
    gmsh->rrw = (bb >> GS_TRXREG_RRW_O) & (GS_TRXREG_RRW_M >> GS_TRXREG_RRW_O);
    gmsh->rrh = (bb >> GS_TRXREG_RRH_O) & (GS_TRXREG_RRH_M >> GS_TRXREG_RRH_O);
    data += 3;
    gmsh->clut = (u32 *)*data;
    return 4;
}

static void hr_initclut2(HR_CLUTS *ct, u8 *gms) {
    // TODO
}

void hr_okmem_reset() {
    memoryStageFormat();
    memoryAreaptrSet();
    memoryAreaFormat();
}

void hrGetMapBuf() {
    s32 i;

    VpmInfo.data_buff = (u8 *)getBuff(1,0x200000, "map", &i);
}

static s32 MapvInit() {
    vpmINFO *vpmi;
    u32 i;

    vpmi = &VpmInfo;
    nkInitPS2();
    hr_okmem_reset();
    {
        s32 i;
        hrmvb = (HR_MAPVS *)getBuff(1, sizeof(HR_MAPVS), "deb", &i);
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
    MapVMenu = 0;
    MapVMsw = 0;
    GameGbl.fr = 0;
    hr_set_vpmfog(&VpmInfo);
    hrmvb->ob_no = 0;
    hrmvb->ob_nblock = vpmi->vpm_block_num;
    hrmvb->ob_blockhead = vpmi->block_head_ptr;
    hrmvb->mdm = 0;

    hrmvb->mdmptr = hrmvb->mdmbuf;
    strcpy(hrmvb->mdmptr->name, "normal");
    hrmvb->mdmptr->mpg = NULL;
    hrmvb->mdmptr->tag0 = hrmvb->mdmptr->tag2 = 0;

    hrmvb->mdmptr++;
    strcpy(hrmvb->mdmptr->name, "wire");
    hrmvb->mdmptr->mpg = (qword *)&hr_vu1m_wire;
    hrmvb->mdmptr->tag0 = 0x2001400000000000;
    hrmvb->mdmptr->tag2 = 0x41;

    hrmvb->mdmptr++;
    strcpy(hrmvb->mdmptr->name, "i off");
    hrmvb->mdmptr->mpg = (qword *)&hr_vu1m_ioff;
    hrmvb->mdmptr->tag0 = 0;
    hrmvb->mdmptr->tag2 = 0;

    hrmvb->mdmptr++;
    strcpy(hrmvb->mdmptr->name, "fogc");
    hrmvb->mdmptr->mpg = (qword *)&hr_vu1m_fogc;
    hrmvb->mdmptr->tag0 = 0;
    hrmvb->mdmptr->tag2 = 0;

    hrmvb->fogcpos[0] = 0.0f;
    hrmvb->fogcpos[1] = 0.0f;
    hrmvb->fogcpos[2] = 0.0f;
    hrmvb->fogcpos[3] = 0.0f;
    hrmvb->lookmode = 0;
    hrmvb->mvm_bg = 0;
    hrmvb->mvm_bgfg = 0;
    hrmvb->mvm_bgno = 0;

    if ((s32)hr_mt_addr != -1) {
        hrmvb->mvm_mtex = 1;
    } else {
        hrmvb->mvm_mtex = 0;
    }

    hrmvb->mir_fg = 0;
    hrmvb->keepcmir = NULL;
    hrmvb->avtcnt = 0;
    hrmvb->avtmenu = 0;
    hrmvb->avtV = 0;
    hrmvb->avts.flag = 0;
    hr_initclut2(&hrmvb->clsb, hr_gmsdatab);
    hrmvb->cltb_fg = 0;
    hr_initclut2(&hrmvb->cls, hr_gmsdata);
    hrmvb->clt_fg = 0;
    hrmvb->maps[0] = mapvw.map_spds;
    hrmvb->maps[1] = mapvw.map_spdt;
    hrmvb->maps[2] = mapvw.bg_spds;
    hrmvb->maps[3] = mapvw.bg_spdt;
    hrmvb->stflag = 0;
    hr_pfade_init(&ppsys.fade);

    SysGbl.smode++;
    printf("Mapviewer init end\n");
    return 0;
}

void hrCamMonitor(CAM_WORK *cam) {
    // TODO
}

static void h_oneblock_tagcp(vpmINFO *vpmi) {
    // TODO
}

static void h_oneblock_back(vpmINFO *vpmi) {
    // TODO
}

static void h_looktype_back() {
    // TODO
}

static void hr_mv_onvram(u64 tex0, u32 x0, u32 y0, u32 x1, u32 y1, f32 s0, f32 t0, f32 s1, f64 t1) {
    // TODO
}

static void hr_mv_mmax(s32 max) {
    // TODO
}

static void hr_mv_m_slint(s32 irep, f32 frep) {
    f32 *f;

    hr_mv_mmax(11);
    if (irep == 0) {
        return;
    }
    if (hrmvb->avtmenu < 8) {
        f = &hrvlight[0].x + hrmvb->avtmenu;
        if (hrmvb->avtmenu - 3 > 2U) {
            frep *= 10.0f;
        } else {
            frep /= 10.0f;
        }
    } else {
        f = &hrvlight[0].x + hrmvb->avtmenu;
        frep /= 10.0f;
    }

    *f += frep;
}

static void hr_mv_m_slint2(s32 irep, f32 frep) {
    f32 *f;

    hr_mv_mmax(22);
    if (irep == 0) {
        return;
    }
    if (hrmvb->avtmenu < 8) {
        f = &hrvlight[0].x + hrmvb->avtmenu;
        if (hrmvb->avtmenu - 3 < 3U) {
            frep /= 10.0f;
        } else {
            frep *= 10.0f;
        }
    } else if (hrmvb->avtmenu < 11) {
        f = &hrvlight[0].x + hrmvb->avtmenu;
        frep /= 10.0f;
    } else if (hrmvb->avtmenu < 19) {
        f = &hrvlight[0].y + hrmvb->avtmenu;
        if (hrmvb->avtmenu - 14 < 3U) {
            frep /= 10.0f;

        } else {
            frep *= 10.0f;
        }
    } else {
        f = &hrvlight[0].y + hrmvb->avtmenu;
        frep /= 10.0f;
    }

    *f += frep;
}

static void hr_mv_avt_menu(kPadDATA *kpd, s32 irep, f32 frep) {
    // TODO
}

static void hr_clutsp2(HRGMSH *gmsh) {
    // TODO
}

static void hr_sendclut(HRGMSH *gmsh) {
    // TODO
}

static void hr_mv_onvram2(HRGMSH *gmsp, HRGMSH *gmsh, u64 tex0) {
    // TODO
}

static void menu_clutwork(HR_CLUTS *ct, kPadDATA *kpd, s32 irep) {
    // TODO
}

static void work_stscr(kPadDATA *kpd) {
    // TODO
}

static void MapVConf(CAM_WORK *cam) {
    // TODO
}

static void hrInitAtrVu1mIv_VO() {
    ATR_VU1M_IV *pp;

    pp = (ATR_VU1M_IV *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_VU1M_IV) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    hrSetObjAttrUnpack(pp->t00, 0x3AC, 1);
    pp->fixednum[0] = 0.5f;
    ((s32 *)pp->fixednum)[1] = 0x8000;
    pp->fixednum[2] = 1.0f;
    pp->fixednum[3] = 1.0f;
    hrSetObjAttrUnpack(pp->t01, 0x3AD, 1);
    pp->clipnum[0] = 2048.0f;
    pp->clipnum[1] = 0;
    pp->clipnum[2] = 0;
    pp->clipnum[3] = 2047.0f;
    hrSetObjAttrUnpack(pp->t02, 0x3AE, 1);
    ((u64 *)pp->giftag_tmp)[0] = hrmvb->mdmptr->tag0;
    ((u64 *)pp->giftag_tmp)[1] = hrmvb->mdmptr->tag2;
    hrSetObjAttrDBlbuf(pp->dblbuf, 0, 0x1C0);
    hrSetObjAttrMscal(pp->mscal, 6);
    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

static void hrSetClipRoadPacket_VO() {
    u32 i;
    u32 nblock;
    vpmINFO *vpmi;

    vpmi = &VpmInfo;
    nblock = vpmi->block_list[0];
    nkMakeUnpackVif1(&p1_packet, GameGbl.wsm, 0x390, 4);
    hrmvb->mdmptr = &hrmvb->mdmbuf[hrmvb->mdm];
    nkAddRef(hrmvb->mdmptr->mpg);
    hrInitAtrVu1mIv_VO();
    for (i = 0; i < nblock; i++) {
        nkAddRef(vpmi->block_head_ptr[vpmi->block_list[i + 1] * 2]);
    }
    FlushCache(WRITEBACK_DCACHE);
}

void hrSetVu1MemImage() {
    u32 i;
    u32 *sp;
    u32 *dp;

    sceGsSyncPath(0, 0);
    sp = (u32 *)VU1_MEM;
    dp = (u32 *)vu1mem_image;
    for (i = 0; i < 1024; i++, sp += 4, dp += 4) {
        dp[0] = sp[0];
        dp[1] = sp[1];
        dp[2] = sp[2];
        dp[3] = sp[3];
    }
}

void hrDrawMap_VO() {
    hrSetBlockList(&VpmInfo, &cvpm, GameGbl.wvm);
    hrSetClipRoadParams();
    hrSetClipRoadPacket_VO();
    nkP1Flush(p1_ot);
}

static s32 MapvMain() {
    // TODO
    return 0;
}

static void init_hoshinoN() {
    hInitStage0();
    hSndSetMVol(1.0f);
}

s32 hrPtInit() {
    vpmINFO *vpmi;
    u32 i;

    vpmi = &VpmInfo;
    nkInitPS2();
    hr_okmem_reset();
    hrGetMapBuf();
    hr_nak_init_allobj();
    nkVT_InitWave();
    abEffectInit();
    ht_InitGbl();
    MapConfFileRead();
    hrPtSeLoad();
    hrDataLoad();
    hr_nak_dummyVK();
    hrInitWorkDeb();
    hr_anmVPM_allinit();
    hfmcam.posmode = 1;
    hrMapFileRead();
    OkSetIDprofile();
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 16, 8);
    OkPrintSysInit(4, SCR_WIDTH, SCR_HEIGHT, 10, 6);
    _nkLightInit();
    CamInit(&GameGbl.cam);
    CamCalMatrix(&GameGbl.cam);
    draw_id = 0;
    init_hoshinoN();
    hr_take_init();
    hrPtDataLoad();
    hr_pall_init();
    vpmi->block_list[0] = 0;
    for (i = 0; i < 2048; i++) {
        vpmi->block_list[i + 1] = 0;
    }
    nkPathReset();
    printf("p1_packet %x\n", (u32)p1_packet);
    hrInitAtrMain();
    sceGsSyncPath(0,0);
    nkDebFlush();
    nkPathFlush();
    printf("-- nkDebFlush\n");
    GameGbl.fr = 0;
    if (hrpt_flag == 0) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
        sceGsSyncPath(0,0);
    }
    else {
        hr_set_vpmfog(&VpmInfo);
        printf("--Fog");
        pt_posmv = 0;
        pt_koma = 0;
        pt_pause = 0;
        MapVMenu = 0;
        MapVMsw = 0;
        pcsel.ca = NULL;
        pcsel.no = 0;
        pt_lightno = 0;
        Dmir_addr = NULL;
        SysGbl.smode++;
        printf("PT init end\n");
    }
    return 0;
}

static s32 pri_chk_vecmat(PCAMSM *pm, char *str) {
    s32 ret;

    ret = 0;
    if (pm->flag & 0x2) {
        strcpy(str, "Mat");
        ret = 1;
    } else if (pm->flag & 0x10) {
        strcpy(str, "Wld");
    } else {
        strcpy(str, "Vec");
    }
    return ret;
}

static void pri_mat2vec(PCAMSC *pcc, PCAMSM *pcm, s32 y) {
    sceVu0FVECTOR v;

    sceVu0ApplyMatrix(v, pcm->hm, pcc->p);
    sceVu0SubVector(v, v, pcm->hm[3]);
}

static void pri_pcam_pos(PCAMS *pcam, s32 y, s32 mv) {
    char str[4];
    s32 ret;

    ret = pri_chk_vecmat(&pcam->mp, str);
    if (mv && ret) {
        pri_mat2vec(&pcam->wp, &pcam->mp, y);
    }
}

static void pri_pcam_inte(PCAMS *pcam, s32 y, s32 mv) {
    char str[4];
    s32 ret;

    ret = pri_chk_vecmat(&pcam->mi, str);
    if (mv && ret) {
        pri_mat2vec(&pcam->wi, &pcam->mi, y);
    }
}

static void pri_pcam_ang(PCAMS *pcam, s32 y) {
    // Empty function
}

static void pri_pcam_lr(PCAMSC *pcc, s32 y) {
    // Empty function
}

static void pri_pcam_proj(PCAMS *pcam, s32 y) {
    // Empty function
}

static void pt_menu_y(kPadDATA *kpd0, s32 menu) {
    if (pPAD_REP_UP(kpd0)) {
        if (--MapVMenu < 0) {
            MapVMenu = menu - 1;
        }
    } else if (pPAD_REP_DOWN(kpd0)) {
        if (++MapVMenu >= menu) {
            MapVMenu = 0;
        }
    }
}

static s32 pt_menu_x(kPadDATA *kpd0, s32 kk) {
    s32 irp;

    irp = 0;
    if (pPAD_REP_RIGHT(kpd0)) {
        irp = 1;
    } else if (pPAD_REP_LEFT(kpd0)) {
        irp = -1;
    }

    if (pPAD_LVL_TRIANG(kpd0)) {
        irp *= kk;
    } else if (pPAD_LVL_CROSS(kpd0)) {
        irp *= kk * kk;
    }

    return irp;
}

static void pt_camera_menu(PCAMS *pcam, kPadDATA *kpd0, kPadDATA *kpd1) {
    // TODO
}

static void pt_used_call_all(HR_PSYS *ps) {
    HR_CALL *ca;
    s32 i;

    ca = ps->call;
    i = 0;
    if ((s32)ca == -1) {
        return;
    }

    while (1) {
        if (ca->flag != 0) {
            pcsel.ca = ca;
            pcsel.no = i;
            return;
        } else {
            i++;
            ca++;
            if (i > 15) {
                return;
            }
        }
    }
}

static void pt_used_call(HR_PSYS *ps, s32 key) {
    // TODO
}

static void pt_light_menu(HR_PSYS *ps, kPadDATA *kpd0, kPadDATA *kpd1) {
    // TODO
}

static void pt_pos_menu(HR_PSYS *ps, kPadDATA *kpd0, kPadDATA *kpd1) {
    // TODO
}

static void hr_packet_afure() {
    u32 size;

    size = ((u32)p1_packet & 0x0FFFFFFF) - (u32)p1_packet_top;
    if (size > 0x10000) {
        printf("!!!! Packet Buff Over %x > %x\n", size, 0x10000);;
    }
}

s32 hrPtMain() {
    // TODO
    return 0;
}