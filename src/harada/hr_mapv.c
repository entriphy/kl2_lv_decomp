#include <sifdev.h>
#include <eeregs.h>
#include "harada/hr_mapv.h"
#include "harada/hr_mapdr.h"
#include "harada/hr_anmdt.h"
#include "harada/hr_anmvp.h"
#include "harada/hr_main.h"
#include "harada/hr_pall.h"
#include "harada/hr_pefc.h"
#include "harada/hr_bgwk.h"
#include "harada/hr_nak.h"
#include "harada/hr_take.h"
#include "harada/hr_vpa.h"
#include "harada/hr_obcon2.h"
#include "harada/hr_prm.h"
#include "harada/hr_pread.h"
#include "hoshino/h_sound.h"
#include "hoshino/h_game.h"
#include "nakano/main.h"
#include "nakano/dma.h"
#include "nakano/game.h"
#include "okanoyo/okio.h"
#include "okanoyo/okprint.h"
#include "nakano/nkpad.h"
#include "hoshino/h_init.h"

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
static void (*hr_mv_avt_tbl[])(s32, f32) = {hr_mv_m_slint, hr_mv_m_slint2};
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

void hr_crossline(sceVu0FVECTOR v, u32 wh, u32 r, u32 g, u32 b, u32 fg) {
    ATR_HRCL *pp;
    sceVu0IVECTOR iv;
    u32 t1;
    u32 h;
    u32 sz;
    f32 f;

    sceVu0RotTransPers(iv, GameGbl.wsm, v, 0);
    wh = (wh / 2) << 4;
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
    DecodeVpm((u32 *)vpmi->data_buff);
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
    sceVu0FVECTOR ang;

    ang[0] = (cam->ang[0] * 360.0f) / M_TWOPIf;
    ang[1] = (cam->ang[1] * 360.0f) / M_TWOPIf;
    ang[2] = (cam->ang[2] * 360.0f) / M_TWOPIf;
}

static void h_oneblock_tagcp(vpmINFO *vpmi) {
    vpmi->vpm_block_num = 1;
    vpmi->block_head_ptr = &hrmvb->ob_blockhead[hrmvb->ob_no * 2];
}

static void h_oneblock_back(vpmINFO *vpmi) {
    vpmi->vpm_block_num = hrmvb->ob_nblock;
    vpmi->block_head_ptr = hrmvb->ob_blockhead;
}

static void h_looktype_back() {
    hrmvb->lookmode = 0;
}

static void hr_mv_onvram(u64 tex0, u32 x0, u32 y0, u32 x1, u32 y1, f64 s0, f64 t0, f64 s1, f64 t1) {
    f32 f_s0;
    f32 f_t0;
    f32 f_s1;
    f32 f_t1;
    HRVV *pp;
    f32 f;
    u32 rgb;

    f_s0 = (f32)s0;
    f_t0 = (f32)t0;
    f_s1 = (f32)s1;
    f_t1 = (f32)t1;

    pp = (HRVV *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(HRVV) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    pp->viftag[0] = pp->viftag[1] = pp->viftag[2] = 0;
    pp->viftag[3] = SCE_VIF1_SET_DIRECT(9, 0);

    ((u64 *)pp->gif0)[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
    pp->gif0[2] = SCE_GIF_PACKED_AD;
    pp->gif0[3] = 0;
    ((u64 *)pp->tex0)[0] = tex0;
    ((u64 *)pp->tex0)[1] = SCE_GS_TEX0_1;

    ((u64 *)pp->gif1)[0] = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 0, 0, 0, 0, 0), 0, 3);
    ((u64 *)pp->gif1)[1] = SCE_GS_ST << 0 | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;
    rgb = 0x80;
    f = f_s0;
    pp->st0[0] = *(s32 *)&f;
    f = f_t0;
    pp->st0[1] = *(s32 *)&f;
    f = 1.0f;
    pp->st0[2] = *(s32 *)&f;
    pp->st0[3] = 0;
    pp->rgb0[0] = pp->rgb0[1] = pp->rgb0[2] = pp->rgb0[3] = rgb;
    pp->xyz0[0] = x0;
    pp->xyz0[1] = y0;
    pp->xyz0[2] = -1;
    pp->xyz0[3] = 0;
    f = f_s1;
    pp->st1[0] = *(s32 *)&f;
    f = f_t1;
    pp->st1[1] = *(s32 *)&f;
    f = 1.0f;
    pp->st1[2] = *(s32 *)&f;
    pp->st1[3] = 0;
    pp->rgb1[0] = pp->rgb1[1] = pp->rgb1[2] = pp->rgb1[3] = rgb;
    pp->xyz1[0] = x1;
    pp->xyz1[1] = y1;
    pp->xyz1[2] = -1;
    pp->xyz1[3] = 0;

    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

static void hr_mv_mmax(s32 max) {
    if (hrmvb->avtmenu < 0) {
        hrmvb->avtmenu = max - 1;
    } else if (hrmvb->avtmenu >= max) {
        hrmvb->avtmenu = 0;
    }
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
    if (pPAD_REP_UP(kpd)) {
        hrmvb->avtmenu--;
    }
    if (pPAD_REP_DOWN(kpd)) {
        hrmvb->avtmenu++;
    }
    hr_mv_avt_tbl[hrmvb->avtcnt - 1](irep, frep);
}

static void hr_clutsp2(HRGMSH *gmsh) {
    // TODO
}

static void hr_sendclut(HRGMSH *gmsh) {
    u32 size;
    u32 dsize;
    u32 *data;

    size = gmsh->clut[-4] & 0x7FFF;
    size += 6;
    dsize = size + 2;
    (*p1_packet)[0] = 0x10000000 + dsize;
    (*p1_packet)[1] = (*p1_packet)[2] = (*p1_packet)[3] = 0;
    p1_packet++;
    (*p1_packet)[0] = (*p1_packet)[1] = (*p1_packet)[2] = 0;
    (*p1_packet)[3] = SCE_VIF1_SET_DIRECT(size + 1, 0);
    p1_packet++;
    data = (u32 *)gmsh->gif;
    while (--size != -1) {
        (*p1_packet)[0] = *data++;
        (*p1_packet)[1] = *data++;
        (*p1_packet)[2] = *data++;
        (*p1_packet)[3] = *data++;
        p1_packet++;
    }

    ((u64 *)p1_packet)[0] = 0x8000;
    (*p1_packet)[2] = (*p1_packet)[3] = 0;
    p1_packet++;
    FlushCache(0);
}

static void hr_mv_onvram2(HRGMSH *gmsp, HRGMSH *gmsh, u64 tex0) {
    u32 x;
    u32 y;
    f32 s0;
    f32 t0;
    f32 s1;
    f32 t1;

    y = gmsh->rrh * 0x60 + 0x7E60;
    x = 0x8000;
    s0 = (f32)gmsp->dsax / 512.0f;
    t0 = (f32)gmsp->dsay / 512.0f;
    s1 = (f32)(gmsp->dsax + gmsp->rrw) / 512.0f;
    t1 = (f32)(gmsp->dsay + gmsp->rrh) / 512.0f;
    hr_mv_onvram(tex0, x, y, x + (gmsp->rrw << 4), y + (gmsp->rrh << 3), s0, t0, s1, t1);
}

static void menu_clutwork(HR_CLUTS *ct, kPadDATA *kpd, s32 irep) {
    HRGMSH *gmsh;
    HRGMSH *gmsp;
    u32 *rgba;
    u8 *col;
    u64 tex0;

    if (ct->mv_clutc == 0) {
        return;
    }

    if (pPAD_TRG_CIRCLE(kpd)) {
        hrmvb->avtV ^= 1;
    }

    if (hrmvb->avtV == 0) {
        if (irep != 0) {
            ct->mv_clutn += irep;
            if (ct->mv_clutn < 0) {
                ct->mv_clutn = ct->mv_clutc - 1;
            } else if (ct->mv_clutn >= ct->mv_clutc) {
                ct->mv_clutn = 0;
            }
            ct->mvc_rgba = 0;
            ct->mvc_rrh = 0;
            ct->mvc_rrw = 0;
        }
    } else {
        if (pPAD_REP_UP(kpd)) {
            ct->mvc_rgba--;
            if (ct->mvc_rgba < 0) {
                ct->mvc_rgba = 3;
            }
        } else if (pPAD_REP_DOWN(kpd)) {
            ct->mvc_rgba++;
            if (ct->mvc_rgba > 3) {
                ct->mvc_rgba = 0;
            }
        }
    }

    gmsh = &ct->mv_clutd[ct->mv_clutn];
    if (pPAD_TRG_L1(kpd)) {
        ct->mvc_rrw--;
        if (ct->mvc_rrw < 0) {
            ct->mvc_rrw = gmsh->rrw - 1;
        }
    } else if (pPAD_TRG_R1(kpd)) {
        ct->mvc_rrw++;
        if (ct->mvc_rrw >= gmsh->rrw) {
            ct->mvc_rrw = 0;
        }
    }

    if (pPAD_TRG_L2(kpd)) {
        ct->mvc_rrh--;
        if (ct->mvc_rrh < 0) {
            ct->mvc_rrh = gmsh->rrh - 1;
        }
    } else if (pPAD_TRG_R2(kpd)) {
        ct->mvc_rrh++;
        if (ct->mvc_rrh >= gmsh->rrh) {
            ct->mvc_rrh = 0;
        }
    }

    rgba = gmsh->clut + gmsh->rrw * ct->mvc_rrh + ct->mvc_rrw;
    if (pPAD_TRG_TRIANG(kpd)) {
        ct->mv_pixn++;
        if (ct->mv_pixn >= ct->mv_pixc) {
            ct->mv_pixn = 0;
        }
    } else if (pPAD_TRG_CROSS(kpd)) {
        ct->mv_pixn--;
        if (ct->mv_pixn < 0) {
            ct->mv_pixn = ct->mv_pixc - 1;
        }
    }

    gmsp = &ct->mv_pixd[ct->mv_pixn];
    if (hrmvb->avtV != 0) {
        col = (u8 *)rgba + ct->mvc_rgba;
        if (irep >= 1 && *col != 0xFF) {
            *col = *col + 1;
        } else if (irep < 0 && *col != 0) {
            *col = *col - 1;
        }

        if (irep != 0) {
            hr_sendclut(gmsh);
        }
    }

    hr_clutsp2(gmsh);
    tex0 = SCE_GS_SET_TEX0(gmsp->dbp, gmsp->dbw, gmsp->dpsm, 9, 9, 0, 0, gmsh->dbp, 0, 0, 0, 1);
    hr_mv_onvram2(gmsp, gmsh, tex0);
}

static void work_stscr(kPadDATA *kpd) {
    s32 *maps;
    s32 i;
    f32 *st;
    f32 *spd;

    if (pPAD_REP_LEFT(kpd)) {
        hrmvb->stflag--;
        if (hrmvb->stflag < 0) {
            hrmvb->stflag = 3;
        }
    } else if (pPAD_REP_RIGHT(kpd)) {
        hrmvb->stflag++;
        if (hrmvb->stflag > 3) {
            hrmvb->stflag = 0;
        }
    }

    i = pPAD_LVL_CIRCLE(kpd) ? 8 : 1;
    maps = &hrmvb->maps[hrmvb->stflag];
    switch (hrmvb->stflag) {
        case 0:
            spd = &hrmapst.s;
            break;
        case 1:
            spd = &hrmapst.t;
            break;
        case 2:
            spd = &hrbgst.s;
            break;
        default:
            spd = &hrbgst.t;
            break;
    }

    st = spd + 1;
    if (pPAD_TRG_TRIANG(kpd)) {
        *maps += i;
        if (*maps > 0x200) {
            *maps = 0x200;
        }
    } else if (pPAD_TRG_CROSS(kpd)) {
        *maps -= i;
        if (*maps < -0x200) {
            *maps = -0x200;
        }
    }

    switch (hrmvb->stflag) {
        case 0:
            mapvw.map_spds = *maps;
            break;
        case 1:
            mapvw.map_spdt = *maps;
            break;
        case 2:
            mapvw.bg_spds = *maps;
            break;
        default:
            mapvw.bg_spdt = *maps;
            break;
    }

    if (*maps == 0) {
        *spd = *st = 0.0f;
    } else {
        *st = 1.0f / *maps;
    }
}

static void MapVConf(CAM_WORK *cam) {
    kPadDATA *kpd;
    f32 flr;
    s32 ilr;
    f32 frep;
    s32 irep;
    f32 fv;
    f32 fh;
    f32 fv0;
    f32 fh0;
    HCMIR *cmir;
    HR_CLUTS *ct;
    vpmINFO *vpmi;

    kpd = &GameGbl.kpd[0];
    vpmi = &VpmInfo;
    flr = 0.0f;
    ilr = 0;
    frep = 0.0f;
    irep = 0;

    if (kpd->rep & 0x5000) {
        switch (MapVMenu) {
            case 13:
                h_oneblock_back(vpmi);
                break;
            case 15:
                h_looktype_back();
                break;
        }
    }

    if (hrmvb->avtV == 0) {
        if (pPAD_REP_UP(kpd)) {
            MapVMenu--;
        }
        if (pPAD_REP_DOWN(kpd)) {
            MapVMenu++;
        }

        if (MapVMenu < 0) {
            MapVMenu = 21;
        }
        if (MapVMenu > 21) {
            MapVMenu = 0;
        }
    }

    if (pPAD_LVL_RIGHT(kpd)) {
        flr = 1.0f;
        ilr = 1;
    }
    if (pPAD_LVL_LEFT(kpd)) {
        flr = -1.0f;
        ilr = -1;
    }

    if (pPAD_REP_RIGHT(kpd)) {
        frep = 1.0f;
        irep = 1;
    }
    if (pPAD_REP_LEFT(kpd)) {
        frep = -1.0f;
        irep = -1;
    }

    fh = nkGetPl3fh(1);
    fv = nkGetPl3fv(1);
    if (fh < 0.0f) {
        fh0 = -fh;
    } else {
        fh0 = fh;
    }
    if (fv < 0.0f) {
        fv0 = -fv;
    } else {
        fv0 = fv;
    }

    if (fv0 < fh0) {
        if (fh > 100.0f) {
            fh = 16.0f;
        } else if (fh < -100.0f) {
            fh = -16.0f;
        }
        fv = 0.0f;
    } else {
        if (fv > 100.0f) {
            fv = 16.0f;
        } else if (fv < -100.0f) {
            fv = -16.0f;
        }
        fh = 0.0f;
    }

    switch (MapVMenu) {
        case 0:
            if (cam->scr_z > 10.0f) {
                cam->scr_z += flr * 10.0f;
            }
            break;
        case 1:
            if (ilr > 0 && mapvw.bgt.r < 0xFF) {
                mapvw.bgt.r += ilr;
            }
            if (ilr < 0 && mapvw.bgt.r > 0x00) {
                mapvw.bgt.r += ilr;
            }
            break;
        case 2:
            if (ilr > 0 && mapvw.bgt.g < 0xFF) {
                mapvw.bgt.g += ilr;
            }
            if (ilr < 0 && mapvw.bgt.g > 0x00) {
                mapvw.bgt.g += ilr;
            }
            break;
        case 3:
            if (ilr > 0 && mapvw.bgt.b < 0xFF) {
                mapvw.bgt.b += ilr;
            }
            if (ilr < 0 && mapvw.bgt.b > 0x00) {
                mapvw.bgt.b += ilr;
            }
            break;
        case 4:
            if (ilr > 0 && mapvw.bgu.r < 0xFF) {
                mapvw.bgu.r += ilr;
            }
            if (ilr < 0 && mapvw.bgu.r > 0x00) {
                mapvw.bgu.r += ilr;
            }
            break;
        case 5:
            if (ilr > 0 && mapvw.bgu.g < 0xFF) {
                mapvw.bgu.g += ilr;
            }
            if (ilr < 0 && mapvw.bgu.g > 0x00) {
                mapvw.bgu.g += ilr;
            }
            break;
        case 6:
            if (ilr > 0 && mapvw.bgu.b < 0xFF) {
                mapvw.bgu.b += ilr;
            }
            if (ilr < 0 && mapvw.bgu.b > 0x00) {
                mapvw.bgu.b += ilr;
            }
            break;
        case 7:
            if (ilr > 0 && mapvw.fog_col[0] < 0xFF) {
                mapvw.fog_col[0] += ilr;
            }
            if (ilr < 0 && mapvw.fog_col[0] > 0x00) {
                mapvw.fog_col[0] += ilr;
            }
            hr_set_vpmfog(&VpmInfo);
            break;
        case 8:
            if (ilr > 0 && mapvw.fog_col[1] < 0xFF) {
                mapvw.fog_col[1] += ilr;
            }
            if (ilr < 0 && mapvw.fog_col[1] > 0x00) {
                mapvw.fog_col[1] += ilr;
            }
            hr_set_vpmfog(&VpmInfo);
            break;
        case 9:
            if (ilr > 0 && mapvw.fog_col[2] < 0xFF) {
                mapvw.fog_col[2] += ilr;
            }
            if (ilr < 0 && mapvw.fog_col[2] > 0x00) {
                mapvw.fog_col[2] += ilr;
            }
            hr_set_vpmfog(&VpmInfo);
            break;
        case 10:
            mapvw.fog_near += ilr * 10;
            if (mapvw.fog_near < 0) {
                mapvw.fog_near = 0;
            }
            hr_set_vpmfog(&VpmInfo);
            break;
        case 11:
            mapvw.fog_far += ilr * 10;
            if (mapvw.fog_far < 0) {
                mapvw.fog_far = 0;
            }
            hr_set_vpmfog(&VpmInfo);
            break;
        case 12:
            if (pPAD_TRG_CIRCLE(kpd)) {
                MapConfFileWrite();
            }
            break;
        case 13:
            if (irep != 0) {
                hrmvb->ob_no += irep;
                if (hrmvb->ob_no < 0) {
                    hrmvb->ob_no = hrmvb->ob_nblock - 1;
                } else if (hrmvb->ob_no >= hrmvb->ob_nblock) {
                    hrmvb->ob_no = 0;
                }
            }
            h_oneblock_tagcp(vpmi);
            break;
        case 14:
            if (irep != 0) {
                hrmvb->mdm += irep;
                if (hrmvb->mdm < 0) {
                    hrmvb->mdm = 3;
                } else if (hrmvb->mdm > 3) {
                    hrmvb->mdm = 0;
                }
            }

            if (hrmvb->mdm == 3) {
                hrmvb->fogcpos[0] += fh;
                hrmvb->fogcpos[2] += fv;
                memcpy(&hrmvb->mdmbuf[3].tag0, hrmvb->fogcpos, 0x10);
            }
            break;
        case 15:
            if (irep != 0) {
                if (hrmvb->lookmode == 0) {
                    hrmvb->cposy = cam->posi[1];
                    hrmvb->crotx = cam->ang[0];
                }

                hrmvb->lookmode += irep;
                if (hrmvb->lookmode < 0) {
                    hrmvb->lookmode = 3;
                } else if (hrmvb->lookmode > 3) {
                    hrmvb->lookmode = 0;
                }

                switch (hrmvb->lookmode) {
                    case 0:
                        cam->posi[1] = hrmvb->cposy;
                        cam->ang[0] = hrmvb->crotx;
                        break;
                    case 1:
                        cam->posi[1] = hrmvb->cposy;
                        cam->ang[0] = 0.0f;
                        break;
                    case 2:
                        cam->posi[1] = hrmvb->cposy - 4000.0f;
                        cam->ang[0] = 1.570796f;
                        break;
                    case 3:
                        cam->posi[1] = hrmvb->cposy + 4000.0f;
                        cam->ang[0] = -1.570796f;
                        break;
                }
            }
            break;
        case 16:
            if (pPAD_TRG_R1(kpd)) {
                hrmvb->mvm_mtex ^= 1;
            }

            if (hrcntbg != 0) {
                if (irep != 0) {
                    hrmvb->mvm_bg += irep;
                    if (hrmvb->mvm_bg < 0) {
                        hrmvb->mvm_bg = 2;
                    } else if (hrmvb->mvm_bg > 2) {
                        hrmvb->mvm_bg = 0;
                    }
                }

                if (hrmvb->mvm_bg == 1) {
                    if (pPAD_TRG_TRIANG(kpd)) {
                        hrmvb->mvm_bgno++;
                        if (hrmvb->mvm_bgno >= hrcntbg) {
                            hrmvb->mvm_bgno = 0;
                        }
                    } else if (pPAD_TRG_CROSS(kpd)) {
                        hrmvb->mvm_bgno--;
                        if (hrmvb->mvm_bgno < 0) {
                            hrmvb->mvm_bgno = hrcntbg - 1;
                        }
                    } else if (pPAD_TRG_SQUARE(kpd)) {
                        hrmvb->mvm_bgfg ^= 1;
                    }
                }
            }
            break;
        case 17:
            if (hcmircnt != 0 && hfmcam.type == 'C') {
                cmir = (HCMIR *)hfmcam.mir;
                if (cmir != hrmvb->keepcmir) {
                    hrmvb->keepcmir = cmir;
                    if (hr_mirsys_ver(hcm_addr) == 1) {
                        f32 *f = hr_cmirver1_scale(cmir);
                        hfmcam.scalex = *f++;
                        hfmcam.scaley = *f++;
                    }
                }

                if (irep < 0) {
                    hrmvb->mir_fg = 1;
                    hfmcam.scalex -= 0.125f;
                    if (hfmcam.scalex < 0.5f) {
                        hfmcam.scalex = 0.5f;
                    }
                } else if (irep > 0) {
                    hrmvb->mir_fg = 1;
                    hfmcam.scalex += 0.125f;
                    if (hfmcam.scalex > 4.0f) {
                        hfmcam.scalex = 4.0f;
                    }
                }

                if (pPAD_TRG_L2(kpd)) {
                    hrmvb->mir_fg = 1;
                    hfmcam.scaley -= 0.125f;
                    if (hfmcam.scaley < 0.5f) {
                        hfmcam.scaley = 0.5f;
                    }
                } else if (pPAD_TRG_L1(kpd)) {
                    hrmvb->mir_fg = 1;
                    hfmcam.scaley += 0.125f;
                    if (hfmcam.scaley > 4.0f) {
                        hfmcam.scaley = 4.0f;
                    }
                }

                if (hrmvb->mir_fg != 0) {
                    if (hr_mirsys_ver(hcm_addr) == 1) {
                        f32 *f = hr_cmirver1_scale(cmir);
                        *f++ = hfmcam.scalex;
                        *f++ = hfmcam.scaley;
                    }
                }
            }
            break;
        case 18:
            if (pPAD_TRG_CIRCLE(kpd)) {
                if (hrmvb->avtcnt != 0) {
                    hrmvb->avtV ^= 1;
                }
            } else {
                if (irep != 0 && hrmvb->avtV == 0) {
                    hrmvb->avtcnt += irep;
                    if (hrmvb->avtcnt > 2) {
                        hrmvb->avtcnt = 0;
                    } else if (hrmvb->avtcnt < 0) {
                        hrmvb->avtcnt = 2;
                    }

                    if (hrmvb->avtcnt == 0) {
                        hrmvb->avts.flag = 0;
                    } else {
                        hrmvb->avt_tbl = &hr_avt_tbl[hrmvb->avtcnt];
                        hrmvb->avts.flag = 1;
                        hrmvb->avts.info = hrmvb->avt_tbl->info;
                        hrmvb->avts.prog = hrmvb->avt_tbl->prog;
                        hrmvb->avts.drawno = hrmvb->avt_tbl->drawno;
                        if (hrmvb->avt_tbl->init != NULL) {
                            hrmvb->avt_tbl->init(&hrmvb->avts);
                        }
                    }
                }
            }

            if (hrmvb->avtV != 0) {
                hr_mv_avt_menu(kpd, irep, frep);
            }
            break;
        case 19:
            ct = &hrmvb->clsb;
            menu_clutwork(ct, kpd, irep);
            break;
        case 20:
            ct = &hrmvb->cls;
            menu_clutwork(ct, kpd, irep);
            break;
        case 21:
            work_stscr(kpd);
            break;
    }
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
    s32 work;
    vpmINFO *vpmi;
    s32 inter;
    kPadDATA *kpd0;
    kPadDATA *kpd1;
    s32 i;
    u32 psize;
    s32 tfg[6];
    BGWK *bg;

    vpmi = &VpmInfo;

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
    psize = (u32)p1_packet;
    FlushCache(0);
    if (MapVMenu == 17) {
        if (hrmirflush) {
            hrDrawMir(0);
            nkPathFlush();
            sceGsSyncPath(0, 0);
            hrDrawMirT();
            sceGsSyncPath(0, 0);
            hrPathFlushOld();
        }
    } else {
        if (hrmirflush) {
            hrPathFlushOld();
            hrPathClear();
        }
    }
    FlushCache(0);

    if (PAD_LVL_R1(GameGbl.kpd[0])) {
        hfmcam.vsmFg |= 2;
        hfmcam.vsmSy = 1.5f;
    } else {
        hfmcam.vsmFg = 0;
    }
    if (MapVMsw == 0 || MapVMenu != 15) {
        CamDebug(&GameGbl.cam);
    }
    if (PAD_TRG_START(GameGbl.kpd[0])) {
        MapVMsw = ~MapVMsw;
        if (MapVMsw == 0) {
            h_oneblock_back(vpmi);
            h_looktype_back();
        }
    }
    if (MapVMsw != 0) {
        MapVConf(&GameGbl.cam);
    }

    hrCamMonitor(&GameGbl.cam);
    CamCalMatrix(&GameGbl.cam);
    h_vp_init(&cvpm, GameGbl.cam.scr_z, 1.0f, 65536.0f, 1024.0f, 1024.0f, 1024.0f, 0);
    hrMainWork();
    hrSetMapREG();

    if (hrmvb->mvm_bg != 0) {
        for (i = 0, bg = hrbgbuff; i < hrcntbg; i++, bg++) {
            tfg[i] = bg->fg;
            if (hrmvb->mvm_bg == 1 && hrmvb->mvm_bgfg != 0 && i != hrmvb->mvm_bgno) {
                bg->fg = 0;
            }
        }
        hrDrawBG();
        for (i = 0, bg = hrbgbuff; i < hrcntbg; i++, bg++) {
            bg->fg = tfg[i];
        }
    } else {
        hrDrawBack();
    }

    if (hrmvb->mvm_bg != 1) {
        if (hrmvb->avts.flag) {
            for (i = 0; i < 1; i++) {
                hrmvb->pushavbuf[i] = hravbuf[i];
                hravbuf[i].flag = 0;
            }
            hrmvb->pushavcnt = hravcnt;

            memcpy(&hravbuf[0], &hrmvb->avts, sizeof(HRANMV));
            hravcnt = 1;
            hr_anmVPM_work();
            hrDrawAnmMap();
            nkP1Flush(p1_ot);
            memcpy(&hrmvb->avts, &hravbuf[0], sizeof(HRANMV));

            for (i = 0; i < 1; i++) {
                hravbuf[i] = hrmvb->pushavbuf[i];
            }
            hravcnt = hrmvb->pushavcnt;
        } else {
            if (hrmvb->mdm == 0) {
                hrDrawMap();
                if (MapVMenu == 13 && MapVMsw != 0) {
                    VpmInfo.block_list[1] = hrmvb->ob_no;
                }
                hr_anmVPA_draw();
                nkP1Flush(p1_ot);
            } else {
                hrDrawMap_VO();
            }
        }
    } else {
        nkP1Flush(p1_ot);
    }

    if (hrmvb->mvm_mtex != 0) {
        nkVT_ExecMovie();
        nkVT_Exec();
        hrSetMapREG();
        hrDrawMTex();
        nkP1Flush(&p1_ot[1]);
    }

    sceGsSyncPath(0,0);
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
    sceGsSyncPath(0, 0);
    if (!hrmirflush) {
        nkPathFlush();
    }

    if (PAD_TRG_SELECT(GameGbl.kpd[0])) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
        sceGsSyncPath(0, 0);
    }

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
    HR_CALL *ca;
    s32 i;

    if ((s32)ps->call == -1) {
        return;
    }

    if (key == 0) {
        for (i = pcsel.no + 1, ca = pcsel.ca + 1; i < 16; i++, ca++) {
            if (ca ->flag != 0) {
                pcsel.ca = ca;
                pcsel.no = i;
                return;
            }
        }

        for (i = 0, ca = ps->call; i < pcsel.no; i++, ca++) {
            if (ca->flag != 0) {
                pcsel.ca = ca;
                pcsel.no = i;
                return;
            }
        }
    } else {
        for (i = pcsel.no - 1, ca = pcsel.ca - 1; i >= 0; i--, ca--) {
            if (ca ->flag != 0) {
                pcsel.ca = ca;
                pcsel.no = i;
                return;
            }
        }

        for (i = 15, ca = ps->call + 15; i > pcsel.no; i--, ca--) {
            if (ca->flag != 0) {
                pcsel.ca = ca;
                pcsel.no = i;
                return;
            }
        }
    }
}

static void pt_light_menu(HR_PSYS *ps, kPadDATA *kpd0, kPadDATA *kpd1) {
    HR_CALL *ca;
    s32 irp;
    s32 rgb;
    s32 rgbfg;
    f32 *f;
    f32 max;
    f32 min;

    rgb = 0;
    min = 0.0f;
    max = 0.0f;

    if ((s32)ps->call == -1) {
        return;
    }

    pt_menu_y(kpd0, 9);
    irp = pt_menu_x(kpd0, 8);

    if (pPAD_TRG_L2(kpd0)) {
        pt_lightno--;
        if (pt_lightno < 0) {
            pt_lightno = 2;
        }
    } else if (pPAD_TRG_R2(kpd0)) {
        pt_lightno++;
        if (pt_lightno > 2) {
            pt_lightno = 0;
        }
    }

    if (irp != 0) {
        ca = pcsel.ca;
        rgbfg = 1;
        switch (MapVMenu) {
            case 0:
            case 1:
            case 2:
                f = &ca->nlw[pt_lightno].p[MapVMenu];
                max = 180.0f;
                min = -180.0f;
                rgbfg = 0;
                break;
            case 3:
            case 4:
            case 5:
                f = &ca->lcolor[pt_lightno].p[MapVMenu - 3];
                rgb = *f * 255.0f;
                break;
            case 6:
            case 7:
            case 8:
                f = &ca->ambi[MapVMenu - 6];
                rgb = *f * 255.0f;
                break;
        }

        if (!rgbfg) {
            *f += (f32)irp / 8.0f;
            if (*f > max) {
                *f = max;
            } else if (*f < min) {
                *f = min;
            }
        } else {
            rgb += irp;
            if (rgb > 0xFF) {
                rgb = 0xFF;
            } else if (rgb < 0x00) {
                rgb = 0x00;
            }

            if (rgb == 0xFF) {
                *f = 1.0f;
            } else {
                *f = (f32)rgb / 255.0f;
            }
        }

        hr_pt_light(ca);
    }
}

static void pt_pos_menu(HR_PSYS *ps, kPadDATA *kpd0, kPadDATA *kpd1) {
    HR_CALL *ca;
    s32 irp;
    s32 fg;
    f32 *f;
    sceVu0FVECTOR v;

    if ((s32)ps->call == -1) {
        return;
    }

    ca = pcsel.ca;
    pt_menu_y(kpd0, 7);
    irp = pt_menu_x(kpd0, 8);
    fg = 0;
    if (irp != 0) {
        f = NULL;
        switch (MapVMenu) {
            case 0:
            case 1:
            case 2:
                f = &ca->pos.p[MapVMenu];
                break;
            case 3:
            case 4:
            case 5:
                f = &ca->rot.p[MapVMenu - 3];
                break;
            case 6:
                fg++;
                SetMichinori(&ca->rtw, ca->rtw.mcn - (irp << 8));
                MichiPosi(&ca->rtw, ca->rtpos);
                GetMichiVec(&ca->rtw, v);
                GetMichiRotXY(v, ca->rtrot);
                ca->rtrot[0] = ca->rtrot[2] = 0.0f;
                break;
        }

        if (!fg) {
            *f += (f32)irp / 8.0f;
        }
        pt_posmv = 1;
        hr_call_efcworkDeb(ca, ps);
    }
}

static void hr_packet_afure() {
    u32 size;

    size = ((u32)p1_packet & 0x0FFFFFFF) - (u32)p1_packet_top;
    if (size > 0x10000) {
        printf("!!!! Packet Buff Over %x > %x\n", size, 0x10000);;
    }
}

s32 hrPtMain() {
    s32 work;
    s32 inter;
    kPadDATA *kpd0;
    kPadDATA *kpd1;
    PCAMS *pcam;
    s32 retw;
    s32 pk;
    sceVu0FVECTOR stkv;

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
    hLoopTop();

    if (hr_abeoff == 0) {
        hr_take_initF();
    }
    abEffectMain(kpd0, kpd1);

    pcam = hrpcam;
    pt_posmv = 0;
    retw = 0;
    if (pt_pause == 0 || pt_koma != 0) {
        hr_nak_work_allobj();
        retw = hr_pall_work();
    } else {
        if (pPAD_TRG_SQUARE(kpd0)) {
            MapVMsw++;
            MapVMenu = 0;
            if (MapVMsw > 3) {
                MapVMsw = 0;
            }
        }

        if (MapVMsw == 1) {
            pt_camera_menu(pcam, kpd0, kpd1);
        } else if (MapVMsw == 2) {
            if (kpd0->trg & 0xC) {
                pt_used_call(&ppsys, pPAD_TRG_L1(kpd0));
            }
            pt_light_menu(&ppsys, kpd0, kpd1);
        } else if (MapVMsw == 3) {
            if (kpd0->trg & 0xC) {
                pt_used_call(&ppsys, pPAD_TRG_L1(kpd0));
            }
            pt_pos_menu(&ppsys, kpd0, kpd1);
        }
    }

    CamCalMatrix(&GameGbl.cam);
    hr_take_setM();
    if (pt_pause == 0 || pt_koma != 0) {
        hrMainWork();
    } else {
        hr_psys_debug(&ppsys, MapVMsw, &pcsel, pcam);
        hrMainWork();
    }

    Dmir_addr = hr_ptmir_debobc(Dmir_addr);
    nkMirDraw();
    if (hrmirflush) {
        if (!hr_check_mir()) {
            hrPathClear();
        }
        hrPathFlushOld();
    }

    if (!hr_pt_check() || hrpt_vt) {
        nkVT_ExecMovie();
    }

    pk = 0;
    hrMainDraw();
    if (pt_pause != 0 && pt_koma == 0) {
        pk = 1;
    }
    hr_nak_draw_allobj();
    hr_psys_motdraw(&ppsys, pk);
    sceGsSyncPath(0, 0);
    hr_nak_draw_effobj();

    if (hrpt_flag != 4 && ppsys.mesp != NULL) {
        hr_mesp_draw(ppsys.mesp, &ppmes);
    }
    hr_pfade_draw(&ppsys.fade);
    hr_pmes_draw(&ppmes);
    hr_pt_skipdraw(&ppsys);
    hr_packet_afure();

    work = *T0_COUNT;
    nkSetMeter();
    OkPFontFlush(PAD_TRG_SELECT(GameGbl.kpd[1]));
    hLoopBottom();
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
    sceGsSyncPath(0, 0);
    nkPathFlush();

    if (retw) {
        printf("Ret: %x\n", retw);
    }
    if ((retw & -0x101) == 3) {
        hr_pt_set(1, ppsys.ncSc, 1, 1);
        GameGbl.vision = ppsys.ncVi;
        SysGbl.smode = 0;
        GameGbl.pause_flag = 0;
        sceGsSyncPath(0, 0);
        DisableDmac(1);
        hExitStage();
        hSndReset();
    } else {
        if (PAD_TRG_SELECT(GameGbl.kpd[0]) || retw) {
            SysGbl.fmode = 0;
            SysGbl.smode = 0;
            GameGbl.pause_flag = 0;
            sceGsSyncPath(0, 0);
            DisableDmac(1);
            TkRemoveAllEffect();
            hExitStage();
            hSndReset();
        } else if (PAD_TRG_START(GameGbl.kpd[0])) {
            pt_pause ^= 1;
            if (pt_pause == 0) {
                pt_koma = 0;
                MapVMenu = 0;
                MapVMsw = 0;

                sceVu0CopyVector(stkv, pcam->wp.p);
                pcam->wp = ptmp_wp;
                sceVu0CopyVector(pcam->wp.p, stkv);

                sceVu0CopyVector(stkv, pcam->wi.p);
                pcam->wi = ptmp_wi;
                sceVu0CopyVector(pcam->wi.p, stkv);

                sceVu0CopyVector(stkv, pcam->wa.p);
                pcam->wa = ptmp_wa;
                sceVu0CopyVector(pcam->wa.p, stkv);

                sceVu0CopyVector(stkv, pcam->mp.r.p);
                pcam->mp.r = ptmp_mpr;
                sceVu0CopyVector(pcam->mp.r.p, stkv);

                sceVu0CopyVector(stkv, pcam->mi.r.p);
                pcam->mi.r = ptmp_mir;
                sceVu0CopyVector(pcam->mi.r.p, stkv);

                pcam->len = ptmp_len;
            } else {
                ptmp_wp = pcam->wp;
                ptmp_wi = pcam->wi;
                ptmp_wa = pcam->wa;
                ptmp_mpr = pcam->mp.r;
                ptmp_mir = pcam->mi.r;
                ptmp_len = pcam->len;

                pcam->wp.spd[0] = pcam->wp.spd[1] = pcam->wp.spd[2] = 0.0f;
                sceVu0CopyVectorXYZ(pcam->wp.acc, pcam->wp.spd);
                sceVu0CopyVectorXYZ(pcam->wi.spd, pcam->wp.spd);
                sceVu0CopyVectorXYZ(pcam->wi.acc, pcam->wp.spd);
                sceVu0CopyVectorXYZ(pcam->wa.spd, pcam->wp.spd);
                sceVu0CopyVectorXYZ(pcam->wa.acc, pcam->wp.spd);
                sceVu0CopyVectorXYZ(pcam->mp.r.spd, pcam->wp.spd);
                sceVu0CopyVectorXYZ(pcam->mp.r.acc, pcam->wp.spd);
                sceVu0CopyVectorXYZ(pcam->mi.r.spd, pcam->wp.spd);
                sceVu0CopyVectorXYZ(pcam->mi.r.acc, pcam->wp.spd);
                hr_pcamsl_spdclr(&pcam->len);
                hr_pcamsl_spdclr(&pcam->proj);
                pt_used_call_all(&ppsys);
            }
        } else {
            if (pt_pause != 0) {
                if (PAD_TRG_CIRCLE(GameGbl.kpd[0])) {
                    pt_koma = 1;
                } else {
                    pt_koma = 0;
                }
            }
        }
    }

    return 0;
}