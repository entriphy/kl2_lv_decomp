#include "harada/hr_mapdr.h"
#include "harada/hr_anmdt.h"
#include "harada/hr_bgwk.h"
#include "harada/hr_mirr.h"
#include "nakano/main.h"
#include "okanoyo/okio.h"
#include "nakano/dma.h"

extern u32 hr_vu1m_iv __attribute__((section(".vudata")));
extern u32 hr_vu1_vpo __attribute__((section(".vudata")));
extern u32 hr_vu1m_bg __attribute__((section(".vudata")));
extern u32 hr_vu1m_mirr __attribute__((section(".vudata")));
extern u32 hr_vu1m_avi __attribute__((section(".vudata")));

u32 hrmaxlcnt = 0;

void DecodeVpm(u32 *vpm_top) {
    u32 i;
    u32 *headptr;
    qword *tagptr;
    vpmINFO *vpmi;

    vpmi = &VpmInfo;
    vpmi->vpm_zone_num = vpm_top[0];
    vpmi->vpm_block_num = vpm_top[1];
    vpm_top[2] += (u32)vpm_top;
    vpmi->block_head_ptr = (qword *)vpm_top[2];

    headptr = vpm_top + 3;
    vpmi->clip_head_ptr = headptr;
    for (i = 0; i < vpmi->vpm_zone_num << 3; i++) {
        *headptr++ += (u32)vpm_top;
    }

    tagptr = vpmi->block_head_ptr;
    for (i = 0; i < vpmi->vpm_block_num << 1; i++) {
        (*tagptr++)[1] += (u32)vpm_top;
    }
}

void DecodeVpm2(u32 *vpm_top, vpmINFO *vpmi) {
    u32 i;
    u32 *headptr;
    qword *tagptr;

    vpmi->vpm_zone_num = vpm_top[0];
    vpmi->vpm_block_num = vpm_top[1];
    vpm_top[2] += (u32)vpm_top;
    vpmi->block_head_ptr = (qword *)vpm_top[2];

    headptr = vpm_top + 3;
    vpmi->clip_head_ptr = headptr;
    for (i = 0; i < vpmi->vpm_zone_num << 3; i++) {
        *headptr++ += (u32)vpm_top;
    }

    tagptr = vpmi->block_head_ptr;
    for (i = 0; i < vpmi->vpm_block_num << 1; i++) {
        (*tagptr++)[1] += (u32)vpm_top;
    }

    vpmi->vpm_data_top = vpm_top;
}

void DecodeVpmMini(u32 *vpm_top, mINFO *vpmi) {
    u32 i;
    u32 *headptr;
    qword *tagptr;

    vpmi->vpm_zone_num = vpm_top[0];
    vpmi->vpm_block_num = vpm_top[1];
    vpm_top[2] += (u32)vpm_top;
    vpmi->block_head_ptr = (qword *)vpm_top[2];

    headptr = vpm_top + 3;
    vpmi->clip_head_ptr = headptr;
    for (i = 0; i < vpmi->vpm_zone_num << 3; i++) {
        *headptr++ += (u32)vpm_top;
    }

    tagptr = vpmi->block_head_ptr;
    for (i = 0; i < vpmi->vpm_block_num << 1; i++) {
        (*tagptr++)[1] += (u32)vpm_top;
    }

    vpmi->vpm_data_top = vpm_top;
}

void DecodeVpmTag(u32 *vpm_top) {
    u32 *headptr;
    u32 zone_num;
    u32 block_num;
    qword *tagptr;
    s32 i;

    zone_num = vpm_top[0];
    block_num = vpm_top[1];
    vpm_top[2] += (u32)vpm_top;
    tagptr = (qword *)vpm_top[2];

    for (i = 0, headptr = vpm_top + 3; i < zone_num << 3; i++, headptr++) {
        *headptr += (u32)vpm_top;
    }

    for (i = 0; i < block_num << 1; i++, tagptr++) {
        (*tagptr)[1] += (u32)vpm_top;
    }
}

void DecodeVpmS(u32 *vpm_top, vpmINFO *vpmi) {
    u32 *headptr;

    vpmi->vpm_zone_num = vpm_top[0];
    vpmi->vpm_block_num = vpm_top[1];
    vpmi->block_head_ptr = (qword *)vpm_top[2];
    headptr = vpm_top + 3;
    vpmi->clip_head_ptr = headptr;
    vpmi->vpm_data_top = vpm_top;
}

void DecodeVpmMiniS(u32 *vpm_top, mINFO *vpmi) {
    u32 *headptr;

    vpmi->vpm_zone_num = vpm_top[0];
    vpmi->vpm_block_num = vpm_top[1];
    vpmi->block_head_ptr = (qword *)vpm_top[2];
    headptr = vpm_top + 3;
    vpmi->clip_head_ptr = headptr;
    vpmi->vpm_data_top = vpm_top;
}


void hr_dmavif(u32 fsh2, u32 fsh3) {
    HR_VIFWAIT *pp;

    pp = (HR_VIFWAIT *)p1_packet;
    pp->dmatag[0] = 0x10000001;
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    pp->viftag[0] = pp->viftag[1] = 0;
    pp->viftag[2] = fsh2;
    pp->viftag[3] = fsh3;

    p1_packet += 2;
}

void hrSetObjAttrUnpack(qword tag, s32 vu_addr, s32 qwc) {
    tag[0] = tag[1] = tag[2] = 0;
    tag[3] = SCE_VIF1_SET_UNPACK(vu_addr, qwc, 0xC, 0);
}

void hrSetObjAttrMscal(qword tag, s32 vu_addr) {
    tag[0] = tag[1] = tag[2] = 0;
    tag[3] = SCE_VIF1_SET_MSCAL(vu_addr, 0);
}

void hrSetObjAttrDBlbuf(qword tag, u32 base, u32 offset) {
    // Is there a macro for this?
    tag[0] = 0;
    tag[1] = 0x01000404;
    tag[2] = base | 0x3000000;
    tag[3] = offset | 0x2000000;
}

void hrInitAtrMain() {
    ATR_MAIN *pp;

    pp = (ATR_MAIN *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_MAIN) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    hrSetObjAttrUnpack(pp->t00, 0x384, 4);
    pp->lc[0][0] = pp->lc[0][1] = pp->lc[0][2] = \
    pp->lc[1][0] = pp->lc[1][1] = pp->lc[1][2] = \
    pp->lc[2][0] = pp->lc[2][1] = pp->lc[2][2] = \
    pp->lc[3][0] = pp->lc[3][1] = pp->lc[3][2] = 1.0f;
    pp->lc[0][3] = pp->lc[1][3] = pp->lc[2][3] = 0.0f;
    pp->lc[3][3] = 1.0f;
    hrSetObjAttrUnpack(pp->t01, 0x388, 4);
    pp->ll[0][0] = pp->ll[0][1] = pp->ll[0][2] = \
    pp->ll[2][0] = pp->ll[2][1] = pp->ll[2][2] = -0.577f;
    pp->ll[1][0] = pp->ll[1][1] = pp->ll[1][2] = 0.577f;
    pp->ll[0][3] = pp->ll[1][3] = pp->ll[2][3] = 0.0f;
    pp->ll[3][0] = pp->ll[3][1] = pp->ll[3][2] = 0.0f;
    pp->ll[3][3] = 1.0f;
    hrSetObjAttrUnpack(pp->t02, 0x38C, 4);
    hrSetObjAttrUnpack(pp->t03, 0x390, 4);
    hrSetObjAttrUnpack(pp->t04, 0x394, 1);
    pp->foga[0] = pp->foga[1] = pp->foga[2] = 0.0f;
    pp->foga[3] = -85.0f;
    hrSetObjAttrUnpack(pp->t05, 0x395, 1);
    pp->fogb[0] = pp->fogb[1] = pp->fogb[2] = 0.0f;
    pp->fogb[3] = 170000.0f;
    hrSetObjAttrUnpack(pp->t06, 0x396, 1);
    pp->defcol[0] = pp->defcol[1] = pp->defcol[2] = 128.0f;
    pp->defcol[3] = 1.0f;
    hrSetObjAttrUnpack(pp->t07, 0x397, 1);
    pp->gtag_tex0[0] = 1;
    pp->gtag_tex0[1] = 0x10000000;
    pp->gtag_tex0[2] = SCE_GIF_PACKED_AD;
    pp->gtag_tex0[3] = 0;
    hrSetObjAttrUnpack(pp->t08, 0x398, 1);
    pp->tex0_n[0] = pp->tex0_n[1] = pp->tex0_n[2] = pp->tex0_n[3] = 0;
    hrSetObjAttrUnpack(pp->t09, 0x399, 1);
    ((u64 *)pp->tex0_r)[0] = SCE_GS_SET_TEX0(0x2300, 4, SCE_GS_PSMT8, 8, 8, 0, 1, 0x3E00, 0, 0, 0, 1);
    ((u64 *)pp->tex0_r)[1] = SCE_GS_TEX0_1;
    hrSetObjAttrUnpack(pp->t10, 0x39A, 1);
    pp->alpha[0] = 0;
    pp->alpha[1] = 0;
    pp->alpha[2] = 64;
    pp->alpha[3] = 128;
    hrSetObjAttrUnpack(pp->t11, 0x39B, 1);
    pp->block_offset[0] = pp->block_offset[1] = pp->block_offset[2] = pp->block_offset[3] = 0;
    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

void hrSetMapREG() {
    ATR_HRMPREG *pp;

    pp = (ATR_HRMPREG *)p1_packet;
    pp->dma[0] = 0x10000000 | (sizeof(ATR_HRMPREG) / sizeof(qword) - 1);
    pp->dma[1] = pp->dma[2] = pp->dma[3] = 0;
    pp->vif[0] = pp->vif[1] = 0;
    pp->vif[2] = SCE_VIF1_SET_FLUSH(0);
    pp->vif[3] = SCE_VIF1_SET_DIRECT(4, 0);
    pp->gif[0] = 0x8003;
    pp->gif[1] = 0x10000000;
    pp->gif[2] = SCE_GIF_PACKED_AD;
    pp->gif[3] = 0;
    ((u64 *)pp->test)[0] = SCE_GS_SET_TEST_1(1, 6, 0, 0, 0, 0, 5, 0);
    ((u64 *)pp->test)[1] = SCE_GS_TEST_1;
    ((u64 *)pp->alpha)[0] = SCE_GS_SET_ALPHA_1(0, 1, 0, 1, 0);
    ((u64 *)pp->alpha)[1] = SCE_GS_ALPHA_1;
    ((u64 *)pp->tex1)[0] = SCE_GS_SET_TEX1_1(0, 0, 1, 0, 0, 0, 0);
    ((u64 *)pp->tex1)[1] = SCE_GS_TEX1_1;
    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

void hrInitAtrVu1mIv(u64 tag0, u64 tag2, s32 mpgno, HRSCRST *scrst) {
    ATR_VU1_MAPS *pp;

    pp = (ATR_VU1_MAPS *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_VU1_MAPS) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    hrSetObjAttrUnpack(pp->t00, 0x3AC, 1);
    pp->fixednum[0] = 0.5f;
    ((u32 *)pp->fixednum)[1] = 0x8000;
    pp->fixednum[2] = 1.0f;
    pp->fixednum[3] = 1.0f;
    hrSetObjAttrUnpack(pp->t01, 0x3AD, 1);
    pp->clipnum[0] = 2048.0f;
    pp->clipnum[1] = 0;
    pp->clipnum[2] = 0;
    pp->clipnum[3] = 2047.0f;
    hrSetObjAttrUnpack(pp->t02, 0x3AE, 1);
    ((u64 *)pp->giftag_tmp)[0] = tag0;
    ((u64 *)pp->giftag_tmp)[1] = tag2;
    hrSetObjAttrDBlbuf(pp->dblbuf, 0, 0x1C0);
    if (mpgno != -1) {
        hrSetObjAttrMscal(pp->mscal, mpgno);
    } else {
        pp->mscal[0] = pp->mscal[1] = pp->mscal[2] = pp->mscal[3] = 0;
    }
    hrSetObjAttrUnpack(pp->t10, 0x39A, 1);
    pp->alpha[0] = *(u32 *)&scrst->s;
    pp->alpha[1] = *(u32 *)&scrst->t;
    pp->alpha[2] = 64;
    pp->alpha[3] = 128;
    hrSetObjAttrUnpack(pp->t07, 0x397, 1);
    pp->gtag_tex0[0] = 1;
    pp->gtag_tex0[1] = 0x10000000;
    pp->gtag_tex0[2] = SCE_GIF_PACKED_AD;
    pp->gtag_tex0[3] = 0;
    FlushCache(0);
    pp++;
    p1_packet = (qword *)pp;
}

void hrSetBlockList(vpmINFO *vpmi, VPCLIP *vpc, sceVu0FMATRIX wvm) {
    u32 i;
    u32 lcnt;
    u32 *list;
    qword *tagptr;

    list = &vpmi->block_list[1];
    hrmaxlcnt = 0;
    for (i = 0, tagptr = vpmi->block_head_ptr, lcnt = 0; i < vpmi->vpm_block_num; i++, tagptr += 2) {
        if (h_vpm_bclip(vpc, (s32 *)(*tagptr)[1], wvm) == 0) {
            if (lcnt < 0x800) {
                *list++ = i;
            }
            lcnt++;
        }
    }

    hrmaxlcnt = lcnt;
    lcnt = lcnt > 0x800 ? 0x800 : lcnt;
    vpmi->block_list[0] = lcnt;
}

static s32 hr_sort_len(const void *t0, const void *t1) {
    HRSORT_LEN *d0;
    HRSORT_LEN *d1;

    if (*(f32 *)t0 > *(f32 *)t1) {
        return 1;
    }

    if (*(f32 *)t0 < *(f32 *)t1) {
        return -1;
    }

    if (*((u32 *)t0 + 1) > *((u32 *)t1 + 1)) {
        return 1;
    } else {
        return -1;
    }
}

u32 hrSetBlockListSL(vpmINFO *vpmi, VPCLIP *vpc, sceVu0FMATRIX wvm, f32 len, sceVu0FVECTOR pos) {
    u32 i;
    u32 lcnt;
    u32 *list;
    qword *tagptr;
    u32 *adr;
    u32 in;
    HRSORT_LEN *hs;
    HRSORT_LEN *hs0;
    f32 *f;
    sceVu0FVECTOR v;

    if (vpmi->vpm_block_num < 0x800) {
        i = vpmi->vpm_block_num;
    } else {
        i = 0x800;
    }
    hs0 = (HRSORT_LEN *)getmem(i << 3);
    if ((s32)hs0 == -1) {
        hrSetBlockList(vpmi, vpc, wvm);
        return vpmi->block_list[0];
    }
    hs = hs0;

    list = vpmi->block_list + 1;
    lcnt = 0;
    for (i = 0, tagptr = vpmi->block_head_ptr, in = 0; i < vpmi->vpm_block_num; i++, tagptr += 2) {
        adr = (u32 *)(*tagptr)[1];
        if (h_vpm_bclip(vpc, (s32 *)adr, wvm) == 0) {
            if (lcnt < 0x800) {
                f = (f32 *)adr + 2;
                hs->no = i;
                v[0] = (f[0] - pos[0]);
                v[1] = (f[1] - pos[1]);
                v[2] = (f[2] - pos[2]);
                hs->len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
                if (hs->len <= len) {
                    in++;
                }
                hs++;
            }
            lcnt++;
        }
    }

    hrmaxlcnt = lcnt;
    lcnt = lcnt > 0x800 ? 0x800 : lcnt;
    vpmi->block_list[0] = lcnt;
    qsort(hs0, lcnt, 8, hr_sort_len);

    for (i = 0, hs = hs0; i < lcnt; i++, hs++) {
        *list++ = hs->no;
    }

    freemem(hs0);
    return in;
}

u32 hrSetBlockListSL2(vpmINFO *vpmi, VPCLIP *vpc, sceVu0FMATRIX wvm, f32 len, sceVu0FVECTOR pos, sceVu0FVECTOR pos2) {
    u32 i;
    u32 lcnt;
    u32 *list;
    qword *tagptr;
    u32 *adr;
    u32 in;
    HRSORT_LEN *hs;
    HRSORT_LEN *hs0;
    f32 *f;
    sceVu0FVECTOR v;

    if (vpmi->vpm_block_num < 0x800) {
        i = vpmi->vpm_block_num;
    } else {
        i = 0x800;
    }
    hs0 = (HRSORT_LEN *)getmem(i << 3);
    if ((s32)hs0 == -1) {
        hrSetBlockList(vpmi, vpc, wvm);
        return vpmi->block_list[0];
    }
    hs = hs0;

    list = vpmi->block_list + 1;
    lcnt = 0;
    for (i = 0, tagptr = vpmi->block_head_ptr, in = 0; i < vpmi->vpm_block_num; i++, tagptr += 2) {
        adr = (u32 *)(*tagptr)[1];
        if (h_vpm_bclip(vpc, (s32 *)adr, wvm) == 0) {
            if (lcnt < 0x800) {
                f = (f32 *)adr + 2;
                hs->no = i;
                v[0] = (f[0] - pos[0]);
                v[1] = (f[1] - pos[1]);
                v[2] = (f[2] - pos[2]);
                hs->len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
                if (hs->len <= len) {
                    in++;
                } else {
                    v[0] = (f[0] - pos2[0]);
                    v[1] = (f[1] - pos2[1]);
                    v[2] = (f[2] - pos2[2]);
                    hs->len = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
                    if (hs->len <= len) {
                        in++;
                    }
                }
                hs++;
            }
            lcnt++;
        }
    }

    hrmaxlcnt = lcnt;
    lcnt = lcnt > 0x800 ? 0x800 : lcnt;
    vpmi->block_list[0] = lcnt;
    qsort(hs0, lcnt, 8, hr_sort_len);

    for (i = 0, hs = hs0; i < lcnt; i++, hs++) {
        *list++ = hs->no;
    }

    freemem(hs0);
    return in;
}

static void hrSetFogDist(s32 near,s32 far) {
    KVM_FOGDIST *pp;
    f32 f_near;
    f32 f_far;

    pp = (KVM_FOGDIST *)p1_packet;
    pp->DMAtag[0] = 0x10000000 | (sizeof(KVM_FOGDIST) / sizeof(qword) - 1);
    pp->DMAtag[1] = pp->DMAtag[2] = pp->DMAtag[3] = 0;
    pp->VIFtag0[0] = pp->VIFtag0[1] = pp->VIFtag0[2] = 0;
    pp->VIFtag0[3] = SCE_VIF1_SET_UNPACK(0x394, 1, 0xC, 0);
    pp->foga[0] = pp->foga[1] = pp->foga[2] = 0;
    if (near != far) {
        f_near = (f32)near;
        f_far = (f32)far;
        pp->foga[3] = (f_near * 255.0f) / (f_near - f_far);
    } else {
        pp->foga[3] = 0;
    }
    pp->VIFtag1[0] = pp->VIFtag1[1] = pp->VIFtag1[2] = 0;
    pp->VIFtag1[3] = SCE_VIF1_SET_UNPACK(0x395, 1, 0xC, 0);
    pp->fogb[0] = pp->fogb[1] = pp->fogb[2] = 0;
    if (near != far) {
        f_near = (f32)near;
        f_far = (f32)far;
        pp->fogb[3] = -(f_far * 255.0f * f_near) / (f_near - f_far);
    } else {
        pp->fogb[3] = 0;
    }
    pp++;
    p1_packet = (qword *)pp;
}

static void hrSetFogCol(qword col) {
    KGS_FOGCOL *pp;

    pp = (KGS_FOGCOL *)p1_packet;
    pp->DMAtag[0] = 0x10000000 | (sizeof(KGS_FOGCOL) / sizeof(qword) - 1);
    pp->DMAtag[1] = pp->DMAtag[2] = pp->DMAtag[3] = 0;
    pp->VIFtag[0] = pp->VIFtag[1] = pp->VIFtag[2] = 0;
    pp->VIFtag[3] = SCE_VIF1_SET_DIRECT(2, 0);
    pp->GIFtag[0] = 0x8001;
    pp->GIFtag[1] = 0x10000000;
    pp->GIFtag[2] = SCE_GIF_PACKED_AD;
    pp->GIFtag[3] = 0;
    pp->GSreg = SCE_GS_FOGCOL;
    pp->fogcol = (col[2] << 16) + (col[1] << 8) + col[0];
    pp++;
    p1_packet = (qword *)pp;
}

void hrSetClipRoadParams() {
     vpmINFO *vpmi;

    vpmi = &VpmInfo;
    hrSetFogDist(vpmi->fog_near, vpmi->fog_far);
    hrSetFogCol(vpmi->fog_col);
}

void hr_set_vpmblock(vpmINFO *vpmi) {
    u32 i;
    u32 nblock;

    nblock = vpmi->block_list[0];
    for (i = 0; i < nblock; i++) {
        nkAddRef(vpmi->block_head_ptr[vpmi->block_list[i + 1] * 2]);
    }
    for (i = 0; i < nblock; i++) {
        nkAddRef(vpmi->block_head_ptr[vpmi->block_list[i + 1] * 2] + 4);
    }
}

void hr_set_vpmblockM(mINFO *vpmi) {
    u32 i;
    u32 nblock;

    nblock = vpmi->block_list[0];
    for (i = 0; i < nblock; i++) {
        nkAddRef(vpmi->block_head_ptr[vpmi->block_list[i + 1] * 2]);
    }
    for (i = 0; i < nblock; i++) {
        nkAddRef(vpmi->block_head_ptr[vpmi->block_list[i + 1] * 2] + 4);
    }
}

static void hrSetClipRoadPacket(vpmINFO *vpmi, s32 flag) {
    nkMakeUnpackVif1(&p1_packet, GameGbl.wsm, 0x390, 4);
    nkMakeUnpackVif1(&p1_packet, GameGbl.vsm, 0x38C, 4);
    nkAddRef(&hr_vu1m_iv);
    hrInitAtrVu1mIv(SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 10),
                    (u64)SCE_GS_PRIM << 0  | (u64)SCE_GS_RGBAQ << 4  |
                    (u64)SCE_GS_ST   << 8  | (u64)SCE_GS_XYZF2 << 12 |
                    (u64)SCE_GS_ST   << 16 | (u64)SCE_GS_XYZF2 << 20 |
                    (u64)SCE_GS_ST   << 24 | (u64)SCE_GS_XYZF2 << 28 |
                    (u64)SCE_GS_ST   << 32 | (u64)SCE_GS_XYZF2 << 36,
                    6, &hrmapst);
    if (flag != 0 || hrmapoff == 0) {
        hr_set_vpmblock(vpmi);
    }
    FlushCache(WRITEBACK_DCACHE);
}

void hrDrawMap() {
    hrSetBlockList(&VpmInfo, &cvpm, GameGbl.wvm);
    hrSetClipRoadParams();
    hrSetClipRoadPacket(&VpmInfo,0);
}

void hrDrawMTex() {
    if ((s32)hr_mt_addr == -1) {
        return;
    }

    hrSetBlockList(&hr_mtexi,&cvpm, GameGbl.wvm);
    hr_dmavif(0, (u32)VU0_MICRO);
    hrSetClipRoadParams();
    hrSetClipRoadPacket(&hr_mtexi,1);
}

void hrDrawVpo_st() {
    hr_objtype = 0;
    nkAddRef(&hr_vu1_vpo);
    hrSetClipRoadParams();
    hrInitAtrVu1mIv(SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 10),
                    (u64)SCE_GS_PRIM << 0  | (u64)SCE_GS_RGBAQ << 4  |
                    (u64)SCE_GS_ST   << 8  | (u64)SCE_GS_XYZF2 << 12 |
                    (u64)SCE_GS_ST   << 16 | (u64)SCE_GS_XYZF2 << 20 |
                    (u64)SCE_GS_ST   << 24 | (u64)SCE_GS_XYZF2 << 28 |
                    (u64)SCE_GS_ST   << 32 | (u64)SCE_GS_XYZF2 << 36,
                    2, &hrmapst);
}

void hrDrawVpo(sceVu0FMATRIX m, u32 *top, u32 objno) {
    nkMakeUnpackVif1(&p1_packet, m, 0x390,4);
    hr_dmavif(SCE_VIF1_SET_FLUSHE(0), SCE_VIF1_SET_MSCAL(0xC, 0));
    nkAddRef(&top[objno * 4 + 4]);
    FlushCache(WRITEBACK_DCACHE);
}

void hrDrawVpoi(sceVu0FMATRIX m, VPOINFO *info, u32 objno) {
    qword *tagptr;

    if (hr_objtype != 0) {
        hrDrawVpo_st();
    }
    nkMakeUnpackVif1(&p1_packet, m, 0x390, 4);
    hr_dmavif(SCE_VIF1_SET_FLUSHE(0), SCE_VIF1_SET_MSCAL(0xE, 0));
    tagptr = info->dmatag;
    nkAddRef(&tagptr[objno]);
    FlushCache(WRITEBACK_DCACHE);
}

void hrDrawVpoLt(sceVu0FMATRIX nlm, sceVu0FMATRIX lcm) {
    if (hr_objtype != 0) {
        hrDrawVpo_st();
    }
    nkMakeUnpackVif1(&p1_packet, nlm, 0x388, 4);
    nkMakeUnpackVif1(&p1_packet, lcm, 0x384, 4);
    hr_dmavif(SCE_VIF1_SET_FLUSHE(0), SCE_VIF1_SET_MSCAL(0x12, 0));
    FlushCache(WRITEBACK_DCACHE);
}

void hrDrawVpo_end() {
    nkP1Flush(p1_ot + 1);
}

VPOINFO* hrget_vpoinfo(s32 id) {
    VPOINFO *info; // ?

    if ((s32)hr_obcvpf == -1 || (s32)(info = hr_obcvpo) == -1 || hr_obcvpf == NULL || hr_obcvpo == NULL || id >= *hr_obcvpf) {
        return NULL;
    } else {
        return &info[id];
    }
}

static void hrSetClipRoadPacketBG(sceVu0FMATRIX vsm, s32 mir) {
    sceVu0FMATRIX m;
    sceVu0FMATRIX m2;
    BGWK *bg;
    s32 i;

    nkAddRef(&hr_vu1m_bg);
    hrInitAtrVu1mIv(SCE_GIF_SET_TAG(0, 0, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 0, 0, 0), 0, 3),
                    SCE_GS_ST << 0 | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8, 6, &hrbgst);
    if (mir != 0) {
        sceVu0CopyMatrix(m2, hfmcam.wvm);
        m2[3][0] = m2[3][1] = m2[3][2] = 0.0f;
        sceVu0MulMatrix(m2, vsm, m2);
    }

    for (i = 0, bg = hrbgbuff; i < 6; i++, bg++) {
        if (bg->fg != 0) {
            if (mir == 0) {
                sceVu0MulMatrix(m, vsm, bg->m);
            } else {
                sceVu0MulMatrix(m, m2, bg->m0);
            }
            nkMakeUnpackVif1(&p1_packet, m, 0x390, 4);
            hr_dmavif(SCE_VIF1_SET_FLUSHE(0), SCE_VIF1_SET_MSCAL(0x8, 0));
            hr_set_vpmblockM(bg->info);
            FlushCache(WRITEBACK_DCACHE);
        }
    }

    hr_dmavif(SCE_VIF1_SET_NOP(0), SCE_VIF1_SET_FLUSHE(0));
    FlushCache(WRITEBACK_DCACHE);
}

void hrDrawBG() {
    if (hrcntdrawbg == 0) {
        hrDrawBack();
    } else {
        hrSetClipRoadPacketBG(GameGbl.vsm, 0);
    }
}

void hrDrawBack() {
    ATR_VU1BACK *pp;

    pp = (ATR_VU1BACK *)p1_packet;
    pp->dma[0] = 0x10000000 | (sizeof(ATR_VU1BACK) / sizeof(qword) - 1);
    pp->dma[1] = pp->dma[2] = pp->dma[3] = 0;
    pp->vif[0] = pp->vif[1] = 0;
    pp->vif[2] = (u32)VU0_MICRO;
    pp->vif[3] = SCE_VIF1_SET_DIRECT(9, 0);
    ((u64 *)pp->gif)[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 0, 1, 0, 0, 0), 0, 2);
    ((u64 *)pp->gif)[1] = (u64)SCE_GS_RGBAQ << 0 | (u64)SCE_GS_XYZF2 << 4;
    pp->rgb0[0] = pp->rgb1[0] = mapvw.bgt.r;
    pp->rgb0[1] = pp->rgb1[1] = mapvw.bgt.g;
    pp->rgb0[2] = pp->rgb1[2] = mapvw.bgt.b;
    pp->rgb0[3] = pp->rgb1[3] = mapvw.bgt.a;
    pp->rgb2[0] = pp->rgb3[0] = mapvw.bgu.r;
    pp->rgb2[1] = pp->rgb3[1] = mapvw.bgu.g;
    pp->rgb2[2] = pp->rgb3[2] = mapvw.bgu.b;
    pp->rgb2[3] = pp->rgb3[3] = mapvw.bgu.a;
    pp->xyz0[0] = pp->xyz2[0] = 0x6C00;
    pp->xyz1[0] = pp->xyz3[0] = 0x9400;
    pp->xyz0[1] = pp->xyz1[1] = 0x7900;
    pp->xyz2[1] = pp->xyz3[1] = 0x8700;
    pp->xyz0[2] = pp->xyz1[2] = pp->xyz2[2] = pp->xyz3[2] = 0x0000;
    pp->xyz0[3] = pp->xyz1[3] = pp->xyz2[3] = pp->xyz3[3] = 0x0000;
    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

static void hr_setp_fxz(ATR_HRFXZ *fxz, u64 frame1, u64 xyofs1) {
    fxz->vif[0] = fxz->vif[1] = 0;
    fxz->vif[2] = (u32)VU0_MICRO;
    fxz->vif[3] = SCE_VIF1_SET_DIRECT(2, 0);
    ((u64 *)fxz->gif)[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    fxz->gif[2] = SCE_GIF_PACKED_AD;
    fxz->gif[3] = 0;
    ((u64 *)fxz->frame1)[0] = frame1;
    ((u64 *)fxz->frame1)[1] = SCE_GS_FRAME_1;
}

static void hr_set_gsclear(ATR_HRCLR *pp) {
    pp->vif[0] = pp->vif[1] = 0;
    pp->vif[2] = (u32)VU0_MICRO;
    pp->vif[3] = SCE_VIF1_SET_DIRECT(2, 0);
    ((u64 *)pp->gif)[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->gif[2] = SCE_GIF_PACKED_AD;
    pp->gif[3] = 0;
    sceGsSetDefClear(&pp->clear, 3, 0x6C0, 0x790, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 0x80, 0);
}

static void hrInitAtrVu1mIvMir0() {
    ATR_MIRR_0 *pp;

    pp = (ATR_MIRR_0 *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_VU1BACK) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    hr_setp_fxz(&pp->fxz, SCE_GS_SET_FRAME_1(0xB4, 10, 0, 0), SCE_GS_SET_XYOFFSET_2(0x6C00, 0x7900));
    hr_set_gsclear(&pp->clr);
    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

static void hrInitAtrVu1mIvMir1() {
    ATR_MIRR_1 *pp;

    pp = (ATR_MIRR_1 *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_MIRR_1) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    hrSetObjAttrUnpack(pp->t02, 0x3AF, 1);
    sceVu0CopyVector(pp->smax, hfmcam.smax);
    hrSetObjAttrUnpack(pp->t03, 0x3B0, 1);
    sceVu0CopyVector(pp->smin, hfmcam.smin);
    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

static void hrBackAtrVu1mIvMir() {
    ATR_MIRR_RET *pp;
    sceGsDrawEnv1 *env1;

    pp = (ATR_MIRR_RET *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_MIRR_RET) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    env1 = (GameGbl.fr & 1) ? &GameGbl.db.draw11 : &GameGbl.db.draw01;
    hr_setp_fxz(&pp->fxz, *(u64 *)&env1->frame1, *(u64 *)&env1->xyoffset1);
    pp->vifwait[0] = pp->vifwait[1] = pp->vifwait[2] = 0;
    pp->vifwait[3] = (u32)VU0_MICRO;
    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

static void hrSetClipRoadPacketMir(vpmINFO *info) {
    hrInitAtrVu1mIvMir0();
    hrSetMapREG();
    if (hrcntdrawbg != 0) {
        hrSetClipRoadPacketBG(hfmcam.vsm, 1);
    }
    nkMakeUnpackVif1(&p1_packet, hfmcam.wsm, 0x390, 4);
    nkAddRef(&hr_vu1m_mirr);
    hrInitAtrVu1mIvMir1();
    if (hrcntdrawbg != 0) {
        hr_dmavif(SCE_VIF1_SET_NOP(0), SCE_VIF1_SET_MSCAL(6, 0));
    } else {
        hrInitAtrVu1mIv(SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 10),
                        (u64)SCE_GS_PRIM << 0  | (u64)SCE_GS_RGBAQ << 4  |
                        (u64)SCE_GS_ST   << 8  | (u64)SCE_GS_XYZF2 << 12 |
                        (u64)SCE_GS_ST   << 16 | (u64)SCE_GS_XYZF2 << 20 |
                        (u64)SCE_GS_ST   << 24 | (u64)SCE_GS_XYZF2 << 28 |
                        (u64)SCE_GS_ST   << 32 | (u64)SCE_GS_XYZF2 << 36,
                        6, &hrmapst);

    }
    hr_set_vpmblock(info);
    FlushCache(WRITEBACK_DCACHE);
    hrBackAtrVu1mIvMir();
}

static void hr_mirpack_sub3(ATR_MIRR_FAN3 *pp, u32 count, u32 prim) {
    u32 size;

    size = count * 3;
    pp->DMAtag[0] = 0x70000000 + size + (sizeof(ATR_MIRR_FAN3) / sizeof(qword) - 1);
    pp->DMAtag[1] = pp->DMAtag[2] = pp->DMAtag[3] = 0;
    ((u64 *)pp->gifc)[0] = SCE_GIF_SET_TAG(6, 0, 0, 0, 0, 1);
    pp->gifc[2] = SCE_GIF_PACKED_AD;
    pp->gifc[3] = 0;
    sceGsSetDefClear(&pp->clear, 3, 0x6C0, 0x790, SCR_WIDTH, SCR_HEIGHT, 0, 0, 0, 0, 0);
    ((u64 *)pp->gif0)[0] = SCE_GIF_SET_TAG(5, 0, 0, 0, 0, 1);
    ((u64 *)pp->gif0)[1] = SCE_GIF_PACKED_AD;
    ((u64 *)pp->tex0)[0] = SCE_GS_SET_TEX0_2(0x1680, 10, SCE_GS_PSMCT32, 10, 8, 0, 0, 0, 0, 0, 0, 0);
    ((u64 *)pp->tex0)[1] = SCE_GS_TEX0_2;
    ((u64 *)pp->clamp1)[0] = SCE_GS_SET_CLAMP_1(5, 0, 0, 0, 0, 0);
    ((u64 *)pp->clamp1)[1] = SCE_GS_CLAMP_1;
    ((u64 *)pp->test)[0] = SCE_GS_SET_TEST_2(1, 6, 0, 0, 0, 0, 5, 0);
    ((u64 *)pp->test)[1] = SCE_GS_TEST_2;
    ((u64 *)pp->alpha)[0] = SCE_GS_SET_ALPHA_2(0, 1, 0, 1, 0);
    ((u64 *)pp->alpha)[1] = SCE_GS_ALPHA_2;
    ((u64 *)pp->texa)[0] = SCE_GS_SET_TEXA(0x7F, 0, 0x7F);
    ((u64 *)pp->texa)[1] = SCE_GS_TEXA;
    ((u64 *)pp->gif1)[0] = SCE_GIF_SET_TAG(count, 1, 1, prim, 0, 3);
    ((u64 *)pp->gif1)[1] = SCE_GS_ST << 0 | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;
}

static f32 hr_mir_getS(f32 x) {
    return (x - 1728.0f) / 1024.0f;
}

static f32 hr_mir_getT(f32 y) {
    return (y - 1936.0f) / 256.0f;
}

static void hr_mirpack_xyzst(qword *ptrv, qword *st, sceVu0FMATRIX m, sceVu0FVECTOR *v, u32 dy) {
    f32 x;
    f32 y;

    sceVu0RotTransPers(*ptrv, m, *v, 0);
    x = hr_mir_getS((*ptrv)[0] / 16.0f);
    y = hr_mir_getT((dy + (*ptrv)[1]) / 16.0f);
    (*st)[0] = *(s32 *)&x;
    (*st)[1] = *(s32 *)&y;
    y = 1.0f;
    (*st)[2] = *(s32 *)&y;
    (*st)[3] = 0;
}

static qword* hr_setp_mirfan(HFMIR *fmir, qword *pp, u32 count) {
    qword *rgb;
    qword *ptrv;
    qword *st;
    qword *keep;
    qword *kst;
    f32 *f;
    f32 *f0;
    f32 df;
    s32 i;
    sceVu0FVECTOR *v;
    u32 dy;

    dy = hr_get_draw2()[9] & 0xF;
    if (dy) {
        dy = 0;
    } else {
        dy = 8;
    }

    kst = st = pp;
    rgb = pp + 1;
    ptrv = pp + 2;
    keep = pp + 3;
    for (i = 0, v = fmir->center; i < count - 1; i++, v++) {
        hr_mirpack_xyzst(ptrv, st, GameGbl.d_wsm, v, dy);
        if ((*ptrv)[0] < 0) {
            (*ptrv)[0] = 0;
        } else if ((*ptrv)[0] > 0xFFFF) {
            (*ptrv)[0] = 0xFFFF;
        }

        if ((*ptrv)[1] < 0) {
            (*ptrv)[1] = 0;
        } else if ((*ptrv)[1] > 0xFFFF) {
            (*ptrv)[1] = 0xFFFF;
        }

        if ((*ptrv)[2] > 0xFFFFFF) {
            (*ptrv)[2] = 0xFFFFFF;
        }
        if ((*ptrv)[2] < 0) {
            (*ptrv)[3] = 0x8000;
        }

        (*rgb)[0] = (*rgb)[1] = (*rgb)[2] = (*rgb)[3] = 128;
        st += 3;
        rgb += 3;
        ptrv += 3;
    }

    f0 = (f32 *)*kst;
    if (hfmcam.vsmFg & 1) {
        f = (f32 *)f0 + 12;
        for (i = 0; i < count - 2; i++, f += 12) {
            df = (*f - *f0);
            *f = df / hfmcam.vsmSx + *f0;
        }
    } else {
        if (hfmcam.vsmFg & 2) {
            f0 = (f32 *)*kst;
            f0++;
            f = (f32 *)f0 + 12;
            for (i = 0; i < count - 2; i++, f += 12) {
                df = (*f - *f0);
                *f = df / hfmcam.vsmSy + *f0;
            }
        }
    }

    sceVu0CopyVector((f32 *)st, (f32 *)keep);
    keep++;
    sceVu0CopyVector((f32 *)rgb, (f32 *)keep);
    keep++;
    sceVu0CopyVector((f32 *)ptrv, (f32 *)keep);
    return ptrv + 1;
}

static void hrSetPacketMirFan3(HFMIR *fmir, ATR_MIRR_FAN3 *pp) {
    u32 count;

    count = fmir->count + 2;
    hr_mirpack_sub3(pp, count, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRIFAN, 1, 1, 0, 1, 1, 0, 1, 0));
    hr_setp_mirfan(fmir, &pp[1].DMAtag, count);
    FlushCache(WRITEBACK_DCACHE);
}

static void hr_make_scaleST(HCMIR *cmir, f32 *st, f64 stc, f64 scale) {
    s32 i;
    f32 f_stc = stc; // ?
    f32 f_scale = scale; // ?

    for (i = 0; i < cmir->count; i++, st += 12) {
        *st = f_stc + (*st - f_stc) / f_scale;
    }
}

// Technically matches
static qword* hr_setp_mirstr(HCMIR *cmir, qword *pp, u32 count) {
    qword *rgb;
    qword *ptrv;
    qword *st;
    s32 i;
    sceVu0FVECTOR *v;
    f32 q;
    f32 cx;
    f32 cy;
    u32 dy;
    sceVu0IVECTOR iv;
    sceVu0FVECTOR fv;
    OBJWORK *ptr;
    f32 *f; // ?
    f32 *f0; // ?

    dy = hr_get_draw2()[9] & 0xF;
    if (dy) {
        dy = 0;
    } else {
        dy = 8;
    }

    st = pp;
    rgb = pp + 1;
    ptrv = pp + 2;

    hr_make_dum(hfmcam.cent, cmir->dst, cmir->men, pp, cmir->count, 3);
    for (i = 0, v = cmir->vert; i < count; i++, v++) {
        hr_mirpack_xyzst(ptrv, st, GameGbl.d_wsm, (sceVu0FVECTOR *)st, dy);
        sceVu0RotTransPers(*ptrv, GameGbl.d_wsm, *v, 0);
        q = 16.0f / (*ptrv)[3];
        (*st)[2] = *(s32 *)&q;

        if ((*ptrv)[0] < 0) {
            (*ptrv)[0] = 0;
        } else if ((*ptrv)[0] > 0xFFFF) {
            (*ptrv)[0] = 0xFFFF;
        }

        if ((*ptrv)[1] < 0) {
            (*ptrv)[1] = 0;
        } else if ((*ptrv)[1] > 0xFFFF) {
            (*ptrv)[1] = 0xFFFF;
        }

        if ((*ptrv)[2] > 0xFFFFFF) {
            (*ptrv)[2] = 0xFFFFFF;
        }
        if ((*ptrv)[2] < 0) {
            (*ptrv)[3] = 0x8000;
        } else {
            (*ptrv)[3] = 0x0000;
        }

        (*rgb)[0] = (*rgb)[1] = (*rgb)[2] = (*rgb)[3] = 128;
        st += 3;
        rgb += 3;
        ptrv += 3;
    }

    if (hfmcam.posmode == 0) {
        ptr = GameGbl.klonoa;
        sceVu0CopyVectorXYZ(fv, ptr->posi);
        fv[3] = 1.0f;
        sceVu0RotTransPers(iv, hfmcam.wsm, fv, 0);
        cx = iv[0] / 16.0f;
        cy = iv[1] / 16.0f;
    } else {
        cx = cmir->cx;
        cy = cmir->cy;
    }

    f = (f32 *)pp;
    if (hfmcam.scalex != 1.0f) {
        f0 = f;
        cx = hr_mir_getS(cx);
        hr_make_scaleST(cmir, f0, cx, hfmcam.scalex);
    }
    if (hfmcam.scaley != 1.0f) {
        f0 = f + 1;
        cy = hr_mir_getT(cy);
        hr_make_scaleST(cmir, f0, cy, hfmcam.scaley);
    }

    for (i = 0; i < count; i++, f += 12) {
        q = f[2];
        f[0] *= q;
        f[1] *= q;
    }

    return st;
}

static void hrSetPacketMirS3(HCMIR *cmir, ATR_MIRR_FAN3 *pp) {
    u32 count;

    count = cmir->count;
    hr_mirpack_sub3(pp, count, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 1, 0, 1, 0));
    hr_setp_mirstr(cmir, &pp[1].DMAtag, count);
    FlushCache(WRITEBACK_DCACHE);
}

static void hrDrawMirM(s32 no) {
    vpmINFO *info;

    info = func_0010DBC0();
    hrSetBlockList(info, &hfmcam.vc, hfmcam.wvm);
    hrSetFogDist(info->fog_near, info->fog_far);
    hrSetFogCol(info->fog_col);
    hrSetClipRoadPacketMir(info);
}

void hrDrawMir(s32 no) {
    if (!hr_check_mir()) {
        return;
    }

    hrDrawMirM(no);
    nkP1Flush(&p1_ot[3]);
}

void hrPathFlushOld() {
    s32 id;

    FlushCache(WRITEBACK_DCACHE);
    if (hroldflush == 0) {
        hroldflush = 1;
    } else {
        id = draw_id == 0;
        nkDmaSendVif1(p1_ot_buff[id]);
    }
}

void hrPathClear() {
    qword *pp1;

    p1_ot = p1_ot_buff[draw_id];
    p1_packet_top = p1_packet_buff[draw_id];
    p1_packet = (qword *)((u32)p1_packet_top | 0x20000000);
    p1_packet_cnt = p1_packet;
    nkClearOT(p1_ot, 4, -1);
}

void hrDrawMirT() {
    sceDmaChan *d2;

    if (!hr_check_mir()) {
        return;
    }

    if (hfmcam.type == 0x46) {
        hrSetPacketMirFan3(hfmcam.mir, (ATR_MIRR_FAN3 *)0x70000000);
    } else if (hfmcam.type == 0x43) {
        hrSetPacketMirS3(hfmcam.mir, (ATR_MIRR_FAN3 *)0x70000000);
    }

    d2 = sceDmaGetChan(SCE_DMA_GIF);
    d2->chcr.TTE = 1;
    sceDmaSend(d2, (void *)(0x70000000 | 0x80000000));
    hrPathClear();
    sceDmaSync(d2, 0, 0);
}

static void hrSetCommonPacketAV() {
    nkMakeUnpackVif1(&p1_packet, GameGbl.wsm, 0x390, 4);
    nkAddRef(&hr_vu1m_avi);
    hrInitAtrVu1mIv(SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 10),
                    (u64)SCE_GS_PRIM << 0  | (u64)SCE_GS_RGBAQ << 4  |
                    (u64)SCE_GS_ST   << 8  | (u64)SCE_GS_XYZF2 << 12 |
                    (u64)SCE_GS_ST   << 16 | (u64)SCE_GS_XYZF2 << 20 |
                    (u64)SCE_GS_ST   << 24 | (u64)SCE_GS_XYZF2 << 28 |
                    (u64)SCE_GS_ST   << 32 | (u64)SCE_GS_XYZF2 << 36,
                    6, &hrmapst);
}

void hrDrawAnmMap() {
    HRANMV *av;
    s32 mno;
    s32 i;
    s32 res;

    if (hravcnt == 0) {
        return;
    }

    mno = -1;
    hrSetClipRoadParams();
    hrSetCommonPacketAV();
    for (i = 0, av = hravbuf; i < 1; i++, av++) {
        if (av->flag != 0) {
            hr_dmavif(SCE_VIF1_SET_NOP(0), SCE_VIF1_SET_FLUSHE(0));
            res = hrAnmVpmTbl[av->drawno](av, mno);
            mno = av->drawno;
            if (res) {
                hr_set_vpmblock(av->info);
                FlushCache(WRITEBACK_DCACHE);
            }
        }
    }
}
