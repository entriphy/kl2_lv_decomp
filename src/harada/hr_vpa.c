#include "harada/hr_vpa.h"
#include "harada/hr_bgwk.h"
#include "harada/hr_mapdr.h"
#include "okanoyo/okio.h"
#include "nakano/main.h"
#include "nakano/dma.h"

extern u32 hr_vu1_vpa __attribute__((section(".vudata")));
extern u32 hr_vu1_vpa_v __attribute__((section(".vudata")));

static void read_anime_end(ANMINFO *anm);
static void read_anime_jmp(ANMINFO *anm);
static void read_anime_pat(ANMINFO *anm);
static void read_anime_time(ANMINFO *anm);
static void read_anime_draw(ANMINFO *anm);
static void read_anime_vcl(ANMINFO *anm);
static void read_anime_vclr(ANMINFO *anm);
static void read_anime_vclg(ANMINFO *anm);
static void read_anime_vclb(ANMINFO *anm);
static void read_anime_scl(ANMINFO *anm);
static void read_anime_sclx(ANMINFO *anm);
static void read_anime_scly(ANMINFO *anm);
static void read_anime_sclz(ANMINFO *anm);
static void read_anime_scls(ANMINFO *anm);
static void read_anime_sclsx(ANMINFO *anm);
static void read_anime_sclsy(ANMINFO *anm);
static void read_anime_sclsz(ANMINFO *anm);
static void read_anime_rot(ANMINFO *anm);
static void read_anime_rotx(ANMINFO *anm);
static void read_anime_roty(ANMINFO *anm);
static void read_anime_rots(ANMINFO *anm);
static void read_anime_rotsx(ANMINFO *anm);
static void read_anime_rotsy(ANMINFO *anm);
static void read_anime_rota(ANMINFO *anm);
static void read_anime_rotax(ANMINFO *anm);
static void read_anime_rotay(ANMINFO *anm);
static void read_anime_camy(ANMINFO *anm);

static void (*read_animetbl[26])(ANMINFO *anm) = {
    read_anime_end,
    read_anime_jmp,
    read_anime_pat,
    read_anime_time,
    read_anime_draw,
    read_anime_vcl,
    read_anime_vclr,
    read_anime_vclg,
    read_anime_vclb,
    read_anime_scl,
    read_anime_sclx,
    read_anime_scly,
    read_anime_sclz,
    read_anime_scls,
    read_anime_sclsx,
    read_anime_sclsy,
    read_anime_sclsz,
    read_anime_rot,
    read_anime_rotx,
    read_anime_roty,
    read_anime_rotsx,
    read_anime_rotsy,
    read_anime_rota,
    read_anime_rotax,
    read_anime_rotay,
    read_anime_camy
};
VPAINFO infovpa = {};
ANMINFO *hri_anm = NULL;
ANMINFO *hri_bg = NULL;

void DecodeVpo2(/* a0 4 */ u32 *vpo_top, /* a1 5 */ VPOINFO *vpoi) {
    /* a2 6 */ u32 addr;
    /* v1 3 */ u32 i;
    /* a0 4 */ qword *tagptr;

    vpoi->ntag = *vpo_top;
    addr = (u32)vpo_top;
    vpoi->data_top = (u32 *)addr;
    tagptr = (qword *)vpo_top + 1;
    vpoi->dmatag = tagptr;
    for (i = 0; i < vpoi->ntag; i++, tagptr++) {
        (*tagptr)[1] += addr;
    }
}

void DecodeVpo2Tag(/* a0 4 */ u32 *vpo_top) {
    /* a2 6 */ u32 addr;
    /* v1 3 */ u32 i;
    /* a1 5 */ u32 ntag;
    /* a0 4 */ qword *tagptr;

    addr = (u32)vpo_top;
    ntag = *vpo_top;
    vpo_top += 4;
    tagptr = (qword *)vpo_top;
    for (i = 0; i < ntag; (*tagptr)[1] += addr, i++, tagptr++);
}

void DecodeVpo2S(u32 *vpo_top, VPOINFO *vpoi) {
    u32 *headptr;

    vpoi->ntag = *vpo_top;
    headptr = vpo_top + 4;
    vpoi->data_top = vpo_top;
    vpoi->dmatag = (qword *)headptr;
}

static void hr_anmVPA_init_s(ANMINFO *anm) {
    anm->scalex = anm->scaley = anm->scalez = 1.0f;
    anm->scsx = anm->scsy = anm->scsz = 0.0f;
    anm->vcolr = anm->vcolg = anm->vcolb = 0.0f;
    anm->vcsr = anm->vcsg = anm->vcsb = 0.0f;
    anm->rotx = anm->rotsx = anm->rotax = 0;
    anm->roty = anm->rotsy = anm->rotay = 0;
}

void hr_anmVPA_init_M(ANMINFO *anm, u32 *top, u32 *addr, qword *tag) {
    u32 *hp;

    anm->flag = 1;
    anm->time = 0;
    anm->dmatag = tag;
    if ((s32)top != -1) {
        hp = top + (*addr >> 2);
        anm->npos = *hp++;
        anm->pos = (qword *)(top + (*hp >> 2));
        hp++;
        anm->blk = hp;
        anm->code = hp + anm->npos;
    } else {
        anm->flag ^= 1;
        anm->npos = 0;
        anm->pos = NULL;
        anm->blk = NULL;
        anm->code = NULL;
    }

    hr_anmVPA_init_s(anm);
}

void hr_anmVPA_init_M1(ANMINFO *anm, u32 *top, u32 *addr, qword *tag) {
    u32 *hp;

    anm->flag = 1;
    anm->time = 0;
    anm->dmatag = tag;
    if ((s32)top != -1) {
        hp = top + (*addr >> 2);
        anm->npos = *hp++;
        anm->pos = (qword *)(top + (*hp >> 2));
        hp++;
        anm->blk = hp;
        anm->code = hp + anm->npos * 2;
    } else {
        anm->flag ^= 1;
        anm->npos = 0;
        anm->pos = NULL;
        anm->blk = NULL;
        anm->code = NULL;
    }

    hr_anmVPA_init_s(anm);
}

void hr_anmVPA_init() {
    u32 *headptr;
    s32 i;
    u32 d;
    ANMINFO *anm;

    if (infovpa.vpoi.data_top == NULL) {
        infovpa.n = 0;
        return;
    }

    headptr = infovpa.vpoi.data_top;
    infovpa.ver = headptr[2];
    d = headptr[3];
    headptr += d >> 2;
    infovpa.n = *headptr++;
    infovpa.anm = headptr;

    hri_anm = getBuff(1, infovpa.n * sizeof(ANMINFO), "hri_anm", &i);
    if ((s32)hri_anm == -1) {
        infovpa.n = 0;
        return;
    }

    if (infovpa.ver == 0) {
        for (i = 0, anm = hri_anm; i < infovpa.n; i++, anm++) {
            hr_anmVPA_init_M(anm, infovpa.vpoi.data_top, headptr++, infovpa.vpoi.dmatag);
        }
    } else {
        for (i = 0, anm = hri_anm; i < infovpa.n; i++, anm++) {
            hr_anmVPA_init_M1(anm, infovpa.vpoi.data_top, headptr++, infovpa.vpoi.dmatag);
        }
    }
}

void hr_anmVPA_initBG() {
    ANMINFO *anm;
    s32 i;

    hri_bg = getBuff(1, hrcntbg * sizeof(ANMINFO), "hri_bg", &i);
    if ((s32)hri_bg == -1) {
        return;
    }

    for (i = 0, anm = hri_bg; i < hrcntbg; i++, anm++) {
        hr_anmVPA_init_M(anm, hrbgbin[i], hrbgbin[i] + 1, NULL);
    }
}

static s32 check_vpmblock(u32 no) {
    u32 *list;
    u32 i;
    u32 max;

    max = VpmInfo.block_list[0];
    for (i = 0, list = &VpmInfo.block_list[1]; i < max; i++, list++) {
        if (*list == no) {
            return 1;
        }
    }
    return 0;
}

static s32 vpa_mpg(ANMINFO *anm, s32 fmpg) {
    if (anm->draw != fmpg) {
        if (anm->draw == 0) {
            nkAddRef(&hr_vu1_vpa);
        } else if (anm->draw >= 0) {
            if (anm->draw <= 3) {
                nkAddRef(&hr_vu1_vpa_v);
            }
        }
    }

    return anm->draw;
}

static void vpa_dbmpg(ANMINFO *anm) {
    nkMakeUnpackVif1(&p1_packet, GameGbl.wsm, 0x390,4);
    (*p1_packet)[0] = 0x10000001;
    (*p1_packet)[1] = (*p1_packet)[2] = (*p1_packet)[3] = 0;
    p1_packet++;
    hrSetObjAttrDBlbuf(*p1_packet, 0,0x1c0);
    p1_packet++;
    FlushCache(0);
}

static void vpa_vcolscale(ANMINFO *anm) {
    sceVu0FMATRIX m;
    ATR_VPAVS *pp;

    sceVu0UnitMatrix(m);
    if (anm->flag & 0x100) {
        sceVu0RotMatrixY(m, m, -GameGbl.cam.ang[1]);
    }
    nkMakeUnpackVif1(&p1_packet, m, 0x38C, 4);

    pp = (ATR_VPAVS *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_VPAVS) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    pp->vcol[0] = SCE_VIF1_SET_UNPACK(0x3AF, 1, 8, 0);
    pp->vcol[1] = *(s32 *)&anm->vcolr;
    pp->vcol[2] = *(s32 *)&anm->vcolg;
    pp->vcol[3] = *(s32 *)&anm->vcolb;
    pp->scale[0] = SCE_VIF1_SET_UNPACK(0x3B0, 1, 12, 0);
    pp->scale[1] = *(s32 *)&anm->scalex;
    pp->scale[2] = *(s32 *)&anm->scaley;
    pp->scale[3] = *(s32 *)&anm->scalez;
    pp->mpg[0] = *(s32 *)&GameGbl.cam.scr_z;
    pp->mpg[1] = 0;
    pp->mpg[2] = 0x10000000;
    pp->mpg[3] = SCE_VIF1_SET_MSCAL(2, 0);

    FlushCache(0);
    pp++;
    p1_packet = (qword *)pp;
}

static void hr_anmVPA_draw_M() {
    ANMINFO *anm;
    s32 i;
    s32 j;
    s32 flag;
    s32 flag2;
    s32 fmpg;
    qword *pos;
    u32 *blk;

    flag = 0;
    fmpg = -1;

    for (i = 0, anm = hri_anm; i < infovpa.n; i++, anm++) {
        if (!(anm->flag & 2)) {
            fmpg = vpa_mpg(anm, fmpg);
            blk = anm->blk;
            flag2 = 0;

            for (j = 0, pos = anm->pos; j < anm->npos; j++, pos++) {
                if (check_vpmblock(*blk++)) {
                    if (flag == 0) {
                        vpa_dbmpg(anm);
                    }
                    flag++;

                    if (flag2 == 0) {
                        vpa_vcolscale(anm);
                    }
                    flag2++;

                    (*p1_packet)[0] = 0x30000001;
                    (*p1_packet)[1] = (s32)pos;
                    (*p1_packet)[2] = (*p1_packet)[3] = 0;
                    p1_packet++;
                    nkAddRef(anm->dmatag);
                }
            }
        }
    }
}

static void hr_anmVPA_draw_M1() {
    ANMINFO *anm;
    s32 i;
    s32 j;
    s32 k;
    s32 flag;
    s32 flag2;
    s32 fmpg;
    qword *pos;
    u32 *blk;
    u32 top;
    u32 mdcnt;
    u32 mdaddr;

    flag = 0;
    fmpg = -1;
    top = (u32)infovpa.vpoi.data_top;

    for (i = 0, anm = hri_anm; i < infovpa.n; i++, anm++) {
        if (!(anm->flag & 2)) {
            fmpg = vpa_mpg(anm, fmpg);
            blk = anm->blk;
            flag2 = 0;

            for (j = 0, pos = anm->pos; j < anm->npos; blk++, j++) {
                if (!check_vpmblock(*blk++)) {
                    pos += *blk;
                } else {
                    if (flag == 0) {
                        vpa_dbmpg(anm);
                    }
                    flag++;

                    if (flag2 == 0) {
                        vpa_vcolscale(anm);
                    }
                    flag2++;

                    if (*blk != 1 || (*pos)[3] == 0) {
                        for (k = 0; k < *blk; k++, pos++) {
                            (*p1_packet)[0] = (*pos)[0];
                            (*p1_packet)[1] = top + (*pos)[1];
                            (*p1_packet)[2] = (*p1_packet)[3] = 0;
                            p1_packet++;
                            nkAddRef(anm->dmatag);
                        }
                    } else {
                        mdcnt = *(u16 *)pos;
                        mdaddr = top + (*pos)[1];

                        for (k = 0; k < mdcnt; k++) {
                            (*p1_packet)[0] = 0x30000001;
                            (*p1_packet)[1] = mdaddr;
                            mdaddr += 0x10;
                            (*p1_packet)[2] = 0;
                            (*p1_packet)[3] = 0x10000000;
                            p1_packet++;
                            nkAddRef(anm->dmatag);
                        }

                        pos++;
                    }
                }
            }
        }
    }
}

void hr_anmVPA_draw() {
    f32 f;

    if (infovpa.n == 0) {
        return;
    }

    hr_dmavif(0, 0x11000000);
    hrSetClipRoadParams();
    nkMakeUnpackVif1(&p1_packet, GameGbl.wsm, 0x390, 4);

    f = 7.0f / 15.0f;
    (*p1_packet)[0] = 0x10000001;
    (*p1_packet)[1] = (*p1_packet)[2] = (*p1_packet)[3] = 0;
    p1_packet++;
    (*p1_packet)[0] = 0x640103B1;
    (*p1_packet)[1] = (*p1_packet)[2] = *(s32 *)&f;
    (*p1_packet)[3] = 0;
    p1_packet++;

    hrInitAtrVu1mIv(0, 0, -1, &hrmapst);
    if (infovpa.ver == 2) {
        hr_anmVPA_draw_M1();
    }
    FlushCache(0);
}

static void read_anime_end(ANMINFO *anm) {
    anm->flag ^= 1;
}

static void read_anime_jmp(ANMINFO *anm) {
    HVPA_JMP *sjmp = (HVPA_JMP *)anm->code;
    anm->code += sjmp->jmp;
}

static void read_anime_pat(ANMINFO *anm) {
    HVPA_PAT *spat = (HVPA_PAT *)anm->code;
    anm->dmatag = &infovpa.vpoi.dmatag[spat->patno];
    anm->code += 2;
}

static void read_anime_time(ANMINFO *anm) {
    HVPA_TIME *stime = (HVPA_TIME *)anm->code;
    anm->time = stime->wait;
    anm->code += 2;
    anm->flag ^= 1;
}

static void read_anime_draw(ANMINFO *anm) {
    anm->flag ^= 2;
    anm->code += 1;
}

static void vcolscale_flag(ANMINFO *anm, f32 spd, u32 flag) {
    if (spd != 0.0f) {
        anm->flag |= flag;
    } else {
        anm->flag &= ~flag;

    }
}

static void read_anime_vcl(ANMINFO *anm) {
    HVPA_VCL *svcl = (HVPA_VCL *)anm->code;
    anm->vcsr = svcl->red;
    anm->vcsg = svcl->green;
    anm->vcsb = svcl->blue;
    vcolscale_flag(anm, anm->vcsr, 4);
    vcolscale_flag(anm, anm->vcsg, 8);
    vcolscale_flag(anm, anm->vcsb, 16);
    anm->code += 4;
}

static void read_anime_vclr(ANMINFO *anm) {
    HVPA_VCLR *svclr = (HVPA_VCLR *)anm->code;
    anm->vcsr = svclr->red;
    vcolscale_flag(anm, anm->vcsr, 4);
    anm->code += 2;
}

static void read_anime_vclg(ANMINFO *anm) {
    HVPA_VCLG *svclg = (HVPA_VCLG *)anm->code;
    anm->vcsg = svclg->green;
    vcolscale_flag(anm, anm->vcsg, 8);
    anm->code += 2;
}

static void read_anime_vclb(ANMINFO *anm) {
    HVPA_VCLB *svclb = (HVPA_VCLB *)anm->code;
    anm->vcsb = svclb->blue;
    vcolscale_flag(anm, anm->vcsb, 16);
    anm->code += 2;
}

static void read_anime_scl(ANMINFO *anm) {
    HVPA_SCL *sscl = (HVPA_SCL *)anm->code;
    anm->scalex = sscl->sclx;
    anm->scaley = sscl->scly;
    anm->scalez = sscl->sclz;
    anm->scsx = anm->scsy = anm->scsz = 0.0f;
    vcolscale_flag(anm, anm->scsx, 0x20);
    vcolscale_flag(anm, anm->scsy, 0x40);
    vcolscale_flag(anm, anm->scsz, 0x80);
    anm->code += 4;
}

static void read_anime_sclx(ANMINFO *anm) {
    HVPA_SCLX *ssclx = (HVPA_SCLX *)anm->code;
    anm->scalex = ssclx->sclx;
    anm->scsx = 0.0f;
    vcolscale_flag(anm, anm->scsx, 0x20);
    anm->code += 2;
}

static void read_anime_scly(ANMINFO *anm) {
    HVPA_SCLY *sscly = (HVPA_SCLY *)anm->code;
    anm->scaley = sscly->scly;
    anm->scsy = 0.0f;
    vcolscale_flag(anm, anm->scsy, 0x40);
    anm->code += 2;
}

static void read_anime_sclz(ANMINFO *anm) {
    HVPA_SCLZ *ssclz = (HVPA_SCLZ *)anm->code;
    anm->scalez = ssclz->sclz;
    anm->scsz = 0.0f;
    vcolscale_flag(anm, anm->scsz, 0x80);
    anm->code += 2;
}

static void read_anime_scls(ANMINFO *anm) {
    HVPA_SCLS *sscls = (HVPA_SCLS *)anm->code;
    anm->scsx = sscls->sclsx;
    anm->scsy = sscls->sclsy;
    anm->scsz = sscls->sclsz;
    vcolscale_flag(anm, anm->scsx, 0x20);
    vcolscale_flag(anm, anm->scsy, 0x40);
    vcolscale_flag(anm, anm->scsz, 0x80);
    anm->code += 4;
}

static void read_anime_sclsx(ANMINFO *anm) {
    HVPA_SCLSX *ssclsx = (HVPA_SCLSX *)anm->code;
    anm->scsx = ssclsx->sclsx;
    vcolscale_flag(anm, anm->scsx, 0x20);
    anm->code += 2;
}

static void read_anime_sclsy(ANMINFO *anm) {
    HVPA_SCLSY *ssclsy = (HVPA_SCLSY *)anm->code;
    anm->scsy = ssclsy->sclsy;
    vcolscale_flag(anm, anm->scsy, 0x40);
    anm->code += 2;
}

static void read_anime_sclsz(ANMINFO *anm) {
    HVPA_SCLSZ *ssclsz = (HVPA_SCLSZ *)anm->code;
    anm->scsz = ssclsz->sclsz;
    vcolscale_flag(anm, anm->scsz, 0x80);
    anm->code += 2;
}

static void read_anime_rot(ANMINFO *anm) {
    HVPA_ROT *srot = (HVPA_ROT *)anm->code;
    anm->rotx = srot->rx;
    anm->roty = srot->ry;
    anm->rotsx = anm->rotax = 0;
    anm->rotsy = anm->rotay = 0;
    anm->code += 3;
}

static void read_anime_rotx(ANMINFO *anm) {
    HVPA_ROTX *srotx = (HVPA_ROTX *)anm->code;
    anm->rotx = srotx->rx;
    anm->rotsx = anm->rotax = 0;
    anm->code += 2;
}

static void read_anime_roty(ANMINFO *anm) {
    HVPA_ROTY *sroty = (HVPA_ROTY *)anm->code;
    anm->roty = sroty->ry;
    anm->rotsy = anm->rotay = 0;
    anm->code += 2;
}

static void read_anime_rots(ANMINFO *anm) {
    HVPA_ROTS *srots = (HVPA_ROTS *)anm->code;
    anm->rotsx = srots->srx;
    anm->rotsy = srots->sry;
    anm->rotax = 0;
    anm->rotay = 0;
    anm->code += 3;
}

static void read_anime_rotsx(ANMINFO *anm) {
    HVPA_ROTSX *srotsx = (HVPA_ROTSX *)anm->code;
    anm->rotsx = srotsx->srx;
    anm->rotax = 0;
    anm->code += 2;
}

static void read_anime_rotsy(ANMINFO *anm) {
    HVPA_ROTSY *srotsy = (HVPA_ROTSY *)anm->code;
    anm->rotsy = srotsy->sry;
    anm->rotay = 0;
    anm->code += 2;
}

static void read_anime_rota(ANMINFO *anm) {
    HVPA_ROTA *s = (HVPA_ROTA *)anm->code;
    anm->rotax = s->arx;
    anm->rotay = s->ary;
    anm->code += 3;
}

static void read_anime_rotax(ANMINFO *anm) {
    HVPA_ROTAX *s = (HVPA_ROTAX *)anm->code;
    anm->rotax = s->arx;
    anm->code += 2;
}

static void read_anime_rotay(ANMINFO *anm) {
    HVPA_ROTAY *s = (HVPA_ROTAY *)anm->code;
    anm->rotay = s->ary;
    anm->code += 2;
}

static void read_anime_camy(ANMINFO *anm) {
    anm->flag ^= 0x100;
    anm->code++;
}

void hr_anmVPA_work_M(ANMINFO *anm) {
    while (anm->flag & 1) {
        read_animetbl[*anm->code](anm);
    }

    if (anm->time != 0 && --anm->time == 0) {
        anm->flag |= 1;
    }

    if (anm->flag & 0x4 || anm->flag & 0x8 || anm->flag & 0x10) {
        anm->vcolr += anm->vcsr;
        anm->vcolg += anm->vcsg;
        anm->vcolb += anm->vcsb;
    }

    if (anm->vcolr != 0.0f || anm->vcolg != 0.0f || anm->vcolb != 0.0f) {
        anm->draw = 1;
    } else {
        anm->draw = 0;
    }

    if (anm->flag & 0x20 || anm->flag & 0x40 || anm->flag & 0x80) {
        anm->scalex += anm->scsx;
        if (anm->scalex < 0.0f) {
            anm->scalex = 0.0f;
        }

        anm->scaley += anm->scsy;
        if (anm->scaley < 0.0f) {
            anm->scaley = 0.0f;
        }

        anm->scalez += anm->scsz;
        if (anm->scalez < 0.0f) {
            anm->scalez = 0.0f;
        }
    }

    if (anm->scalex != 1.0f || anm->scaley != 1.0f || anm->scalez != 1.0f) {
        anm->draw |= 2;
    }

    anm->rotsx += anm->rotax;
    anm->rotx += anm->rotsx;
    anm->rotx &= 0xFFFF;
    anm->rotsy += anm->rotay;
    anm->roty += anm->rotsy;
    anm->roty &= 0xFFFF;
}

void hr_anmVPA_work() {
    ANMINFO *anm;
    s32 i;

    if (infovpa.n == 0) {
        return;
    }

    for (i = 0, anm = hri_anm; i < infovpa.n; i++, anm++) {
        hr_anmVPA_work_M(anm);
    }
}

void hr_anmVPA_workBG() {
    ANMINFO *anm;
    BGWK *bg;
    s32 i;

    if ((s32)hri_bg == -1) {
        return;
    }

    for (i = 0, anm = hri_bg, bg = hrbgbuff; i < hrcntbg; i++, anm++, bg++) {
        hr_anmVPA_work_M(anm);
        bg->rx = anm->rotx;
        bg->ry = anm->roty;
    }
}
