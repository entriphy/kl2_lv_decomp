#include <eeregs.h>
#include "nakano/map00.h"
#include "nakano/main.h"
#include "nakano/dma.h"
#include "harada/hr_mapdr.h"
#include "harada/hr_mapv.h"

extern u32 p_vu1m_iv __attribute__((section(".vudata")));

void SetVu1MemImage() {
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

void nkSetObjAttrUnpack(qword tag, s32 vu_addr, s32 qwc) {
    tag[0] = tag[1] = tag[2] = SCE_VIF1_SET_NOP(0);
    tag[3] = SCE_VIF1_SET_UNPACK(vu_addr, qwc, 0x6C, 0); // V4_32
}

void nkSetObjAttrMscal(qword tag, s32 vu_addr) {
    tag[0] = tag[1] = tag[2] = SCE_VIF1_SET_NOP(0);
    tag[3] = SCE_VIF1_SET_MSCAL(vu_addr, 0);
}

void nkSetObjAttrDBlbuf(qword tag, u32 base, u32 offset) {
    tag[0] = SCE_VIF1_SET_NOP(0);
    tag[1] = SCE_VIF1_SET_STCYCL(4, 4, 0);
    tag[2] = SCE_VIF1_SET_BASE(base, 0);
    tag[3] = SCE_VIF1_SET_OFFSET(offset, 0);
}

void nkInitAtrMain() {
    ATR_MAIN *pp = (ATR_MAIN *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_MAIN) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;

    nkSetObjAttrUnpack(pp->t00, 0x384, 4);
    pp->lc[0][0] = pp->lc[0][1] = pp->lc[0][2] = \
    pp->lc[1][0] = pp->lc[1][1] = pp->lc[1][2] = \
    pp->lc[2][0] = pp->lc[2][1] = pp->lc[2][2] = \
    pp->lc[3][0] = pp->lc[3][1] = pp->lc[3][2] = 1.0f;
    pp->lc[0][3] = pp->lc[1][3] = pp->lc[2][3] = 0.0f;
    pp->lc[3][3] = 1.0f;

    nkSetObjAttrUnpack(pp->t01, 0x388, 4);
    pp->ll[0][0] = pp->ll[0][1] = pp->ll[0][2] = \
    pp->ll[2][0] = pp->ll[2][1] = pp->ll[2][2] = -0.577f;
    pp->ll[1][0] = pp->ll[1][1] = pp->ll[1][2] = 0.577f;
    pp->ll[0][3] = pp->ll[1][3] = pp->ll[2][3] = 0.0f;
    pp->ll[3][0] = pp->ll[3][1] = pp->ll[3][2] = 0.0f;
    pp->ll[3][3] = 1.0f;

    nkSetObjAttrUnpack(pp->t02, 0x38C, 4);
    // pp->lv

    nkSetObjAttrUnpack(pp->t03, 0x390, 4);
    // pp->ls

    nkSetObjAttrUnpack(pp->t04, 0x394, 1);
    pp->foga[0] = pp->foga[1] = pp->foga[2] = 0.0f;
    pp->foga[3] = -85.0f;

    nkSetObjAttrUnpack(pp->t05, 0x395, 1);
    pp->fogb[0] = pp->fogb[1] = pp->fogb[2] = 0.0f;
    pp->fogb[3] = 170000.0f;

    nkSetObjAttrUnpack(pp->t06, 0x396, 1);
    pp->defcol[0] = pp->defcol[1] = pp->defcol[2] = 128.0f;
    pp->defcol[3] = 1.0f;

    nkSetObjAttrUnpack(pp->t07, 0x397, 1);
    pp->gtag_tex0[0] = 1;
    pp->gtag_tex0[1] = 0x10000000;
    pp->gtag_tex0[2] = SCE_GIF_PACKED_AD;
    pp->gtag_tex0[3] = 0;

    nkSetObjAttrUnpack(pp->t08, 0x398, 1);
    pp->tex0_n[0] = pp->tex0_n[1] = pp->tex0_n[2] = pp->tex0_n[3] = 0;

    nkSetObjAttrUnpack(pp->t09, 0x399, 1);
    ((u64 *)pp->tex0_r)[0] = SCE_GS_SET_TEX0(0x2300, 4, 19, 8, 8, 0, 1, 0x3E00, 0, 0, 0, 5);
    ((u64 *)pp->tex0_r)[1] = SCE_GS_TEX0_1;

    nkSetObjAttrUnpack(pp->t10, 0x39A, 1);
    pp->alpha[0] = 0;
    pp->alpha[1] = 0;
    pp->alpha[2] = 0x40;
    pp->alpha[3] = 0x80;

    nkSetObjAttrUnpack(pp->t11, 0x39B, 1);
    pp->block_offset[0] = pp->block_offset[1] = pp->block_offset[2] = pp->block_offset[3] = 0;

    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

static void nkInitAtrVu1mIv() {
    ATR_VU1M_IV *pp = (ATR_VU1M_IV *)p1_packet;
    pp->dmatag[0] = 0x10000000 | (sizeof(ATR_VU1M_IV) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;

    nkSetObjAttrUnpack(pp->t00, 0x3AC, 1);
    pp->fixednum[0] = 0.5f;
    ((s32 *)pp->fixednum)[1] = 0x8000;
    pp->fixednum[2] = 1.0f;
    pp->fixednum[3] = 1.0f;

    nkSetObjAttrUnpack(pp->t01, 0x3AD, 1);
    pp->clipnum[0] = 2048.0f;
    pp->clipnum[1] = 0.0f;
    pp->clipnum[2] = 0.0f;
    pp->clipnum[3] = 2047.0f;

    nkSetObjAttrUnpack(pp->t02, 0x3AE, 1);
    pp->giftag_tmp[0] = 0x8001;
    pp->giftag_tmp[1] = 0xA0000000;
    pp->giftag_tmp[2] = 0x42424210;
    pp->giftag_tmp[3] = 0x42;

    nkSetObjAttrDBlbuf(pp->dblbuf, 0x000, 0x1C0);
    nkSetObjAttrMscal(pp->mscal, 6);

    FlushCache(WRITEBACK_DCACHE);
    pp++;
    p1_packet = (qword *)pp;
}

static void nkSetFogDist(s32 near, s32 far) {
    KVM_FOGDIST *pp = (KVM_FOGDIST *)p1_packet;
    f32 f_near;
    f32 f_far;
    pp->DMAtag[0] = 0x10000000 | (sizeof(KVM_FOGDIST) / sizeof(qword) - 1);
    pp->DMAtag[1] = pp->DMAtag[2] = pp->DMAtag[3] = 0;

    pp->VIFtag0[0] = pp->VIFtag0[1] = pp->VIFtag0[2] = 0;
    pp->VIFtag0[3] = SCE_VIF1_SET_UNPACK(0x394, 1, 0x6C, 0); // V4_32
    pp->foga[0] = pp->foga[1] = pp->foga[2] = 0.0f;
    if (near != far) {
        f_near = near;
        f_far = far;
        pp->foga[3] = (f_near * 255.0f) / (f_near - f_far);
    } else {
        pp->foga[3] = 0.0f;
    }

    pp->VIFtag1[0] = pp->VIFtag1[1] = pp->VIFtag1[2] = 0;
    pp->VIFtag1[3] = SCE_VIF1_SET_UNPACK(0x395, 1, 0x6C, 0); // V4_32
    pp->fogb[0] = pp->fogb[1] = pp->fogb[2] = 0.0f;
    if (near != far) {
        f_near = near;
        f_far = far;
        pp->fogb[3] = -(f_far * 255.0f * f_near) / (f_near - f_far);
    } else {
        pp->fogb[3] = 0.0f;
    }

    pp++;
    p1_packet = (qword *)pp;
}

static void nkSetFogCol(sceVu0IVECTOR col) {
    KGS_FOGCOL *pp = (KGS_FOGCOL *)p1_packet;
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

static void nkSetClipRoadParams() {
    vpmINFO *vpmi = &VpmInfo;
    nkSetFogDist(vpmi->fog_near, vpmi->fog_far);
    nkSetFogCol(vpmi->fog_col);
}

static void nkSetClipRoadPacket() {
    u32 i;
    u32 nblock;
    vpmINFO *vpmi = &VpmInfo;

    nkMakeUnpackVif1(&p1_packet, GameGbl.wsm, 0x390, 4);
    nkAddRef(&p_vu1m_iv);
    nkInitAtrVu1mIv();

    nblock = vpmi->vpm_block_num;
    if (nblock > 2048) {
        nblock = 2048;
    }
    for (i = 0; i < nblock; i++) {
        nkAddRef(vpmi->block_head_ptr[i * 2]);
    }

    FlushCache(WRITEBACK_DCACHE);
}

void nkDrawMap() {
    nkSetClipRoadParams();
    nkSetClipRoadPacket();
    nkP1Flush(p1_ot);
}