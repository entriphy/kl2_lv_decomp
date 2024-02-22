#include "take/sfxcalc.h"
#include "take/object.h"
#include "take/sfxdraw.h"

sceVu0FVECTOR vmin = {};
sceVu0FVECTOR vmax = {};

static void Vu0RegisterInit(sceVu0FMATRIX lc, sceVu0FVECTOR *c, sceVu0FVECTOR *sc, sceVu0FVECTOR *clip, sceVu0FVECTOR *vmin, sceVu0FVECTOR *vmax, u16 adc, u16 mac);
static void SetMatrix(sceVu0FMATRIX m0);
static void SetMatrix4(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FMATRIX m2, sceVu0FMATRIX m3);
static void RotTransPersWeight(u128 *v0, s32 vt, s32 wt, s32 num);
static void RotTransPers(u128 *v0, s32 vadrs, s32 num);
static void GetRgb(u128 *vt, s32 nadrs, s32 num);
static void GetRgbWeight(u128 *vt, s32 nadrs, s32 nwadrs, s32 num);
static void RotTransPersMime(u128 *v0, s32 vt, s32 vt2, s32 wt, s32 num);
static void GetRgbMime(u128 *vt, s32 nm, s32 nm2, s32 nw, s32 num);
static void GetSpecUV(u128 *uv, s32 nadrs, s32 num);
static void GetSpecUVWeight(u128 *vt, s32 nadrs, s32 nwadrs, s32 num);
static void GetSpecUVMime(u128 *vt, s32 nm, s32 nm2, s32 nw, s32 num);
static void GetRegister(sceVu0FVECTOR *min, sceVu0FVECTOR *max);


void SfxFixCalc(SFXOBJ *pObj, PARTS *pParts, u128 *pVertexI, u128 *pColorI, u128 *pSpecUVI) {
    s32 l_vnum;
    u128 *pVertex;
    u128 *pNormal;
    qword_uni DmaTag;
    s32 vt_size;
    s32 vtq_size;
    s32 Vu0Mem;
    sceVu0FVECTOR Scale;
    sceVu0FVECTOR clip;
    sceVu0FVECTOR clamp;

    ((qword_uni *)0x70002200)->u_u128 = 0;
    ((qword_uni *)0x70002210)->u_u128 = 0;
    ((qword_uni *)0x70002220)->u_u128 = 0;
    ((qword_uni *)0x70002230)->u_u128 = 0;

    if (pParts->OutLine == 0 || pParts->OutLine == 1 || pParts->OutLine == 4 || pParts->OutLine == 5 || pParts->OutLine == 6) {
        vmin[0] = 2368.0f;
        vmin[1] = 2160.0f;
        vmin[2] = 1048575.0f;
        vmin[3] = 0.0f;
        vmax[0] = 1728.0f;
        vmax[1] = 1936.0f;
        vmax[2] = 1.0f;
        vmax[3] = 0.0f;
    }

    Scale[0] = pObj->scale;
    Scale[1] = 1.0f / 255.0f;
    Scale[2] = 0.0f;
    Scale[3] = 0.0f;

    clip[0] = 4095.0f;
    clip[1] = 4095.0f;
    clip[2] = 32.0f;
    clip[3] = 1048575.0f;

    clamp[0] = 255.0f;
    clamp[1] = 255.0f;
    clamp[2] = 255.0f;
    clamp[3] = 2048.0f;

    Vu0RegisterInit(*pObj->pLightColor, &clamp, &Scale, &clip, &vmin, &vmax, 0x8000, 208);

    pVertex = (u128 *)pParts->vert_adrs;
    pNormal = (u128 *)pParts->norm_adrs;
    Vu0Mem = 0;
    l_vnum = pParts->vert_num;
    vt_size = 128;
    vtq_size = 48;

    while (l_vnum != 0) {
        if (l_vnum <= 128) {
            vt_size = l_vnum;
            vtq_size = (vt_size * 6) / 16;
            if ((vt_size * 6) & 0xF) {
                vtq_size++;
            }
        }

        *(u32 *)0x70002200 = DMArefe | vtq_size;
        *(u32 *)0x70002204 = (u32)pVertex;
        *(u32 *)0x7000220C = SCE_VIF0_SET_UNPACK(Vu0Mem, vt_size, VIF_UNPACK_V3_16, 0);
        DmaSend_Mac(pDma.Vif0, (u128 *)(0x70002200 | 0x80000000));
        SetMatrix(SfxLsMtx[0]);
        DmaSync_Mac(pDma.Vif0);
        RotTransPers(pVertexI, Vu0Mem, vt_size);
        pVertexI += vt_size;

        *(u32 *)0x70002230 = DMArefe | vtq_size;
        *(u32 *)0x70002234 = (u32)pNormal;
        *(u32 *)0x7000223C = SCE_VIF0_SET_UNPACK(Vu0Mem, vt_size, VIF_UNPACK_V3_16, 0);
        DmaSend_Mac(pDma.Vif0, (u128 *)(0x70002230 | 0x80000000));
        SetMatrix(SfxLcLightMtx[0]);
        DmaSync_Mac(pDma.Vif0);

        GetRgb(pColorI, Vu0Mem, vt_size);
        pColorI += vt_size;

        if (pParts->SpecType != 0) {
            SetMatrix(SfxLvSpecMtx[0]);
            GetSpecUV(pSpecUVI, Vu0Mem, vt_size);
            pSpecUVI += vt_size;
        }

        pVertex += 48;
        pNormal += 48;
        l_vnum -= vt_size;
    }

    GetRegister(&vmin, &vmax);
    sceVu0FTOI4Vector(bboxTmp[0], vmax);
    sceVu0FTOI4Vector(bboxTmp[1], vmin);
}

void SfxSkinCalc(SFXOBJ *pObj, PARTS *pParts, u128 *pVertexI, u128 *pColorI, u128 *pSpecUVI) {
    s32 i;
    s32 ii;
    s32 l_vnum;
    u128 *pVertex;
    u128 *pNormal;
    u128 *pVertWt;
    TYPE_JOINT_BLK *pBlock;
    qword_uni DmaTag;
    s32 vt_size;
    s32 vtq_size;
    s32 vtwq_size;
    s32 Vu0Mem;
    sceVu0FVECTOR Scale;
    sceVu0FVECTOR clip;
    sceVu0FVECTOR clamp;
    s32 mi[4];

    ((qword_uni *)0x70002200)->u_u128 = 0;
    ((qword_uni *)0x70002210)->u_u128 = 0;
    ((qword_uni *)0x70002220)->u_u128 = 0;
    ((qword_uni *)0x70002230)->u_u128 = 0;

    if (pParts->OutLine == 0 || pParts->OutLine == 1 || pParts->OutLine == 4 || pParts->OutLine == 5 || pParts->OutLine == 6) {
        vmin[0] = 2368.0f;
        vmin[1] = 2160.0f;
        vmin[2] = 1048575.0f;
        vmin[3] = 0.0f;
        vmax[0] = 1728.0f;
        vmax[1] = 1936.0f;
        vmax[2] = 1.0f;
        vmax[3] = 0.0f;
    }

    Scale[0] = pObj->scale;
    Scale[1] = 1.0f / 255.0f;
    Scale[2] = 0.0f;
    Scale[3] = 0.0f;

    clip[0] = 4095.0f;
    clip[1] = 4095.0f;
    clip[2] = 32.0f;
    clip[3] = 1048575.0f;

    clamp[0] = 255.0f;
    clamp[1] = 255.0f;
    clamp[2] = 255.0f;
    clamp[3] = 2048.0f;

    Vu0RegisterInit(*pObj->pLightColor, &clamp, &Scale, &clip, &vmin, &vmax, 0x8000, 208);
    
    for (i = 0, pBlock = (TYPE_JOINT_BLK *)pParts->jblock_adrs; i < pParts->jblock_num; i++, pBlock++) {
        pVertex = (u128 *)((u32)pParts->vert_adrs + (u32)pBlock->vert_ofs);
        pNormal = (u128 *)((u32)pParts->norm_adrs + (u32)pBlock->norm_ofs);
        pVertWt = (u128 *)((u32)pBlock->v_wt_adrs + (u32)pParts->sfx_adrs);

        for (ii = 0; ii < 4; ii++) {
            if (pBlock->joint_id[ii] < 0) {
                mi[ii] = 0;
            } else {
                mi[ii] = pBlock->joint_id[ii];
            }
        }

        Vu0Mem = 0;
        l_vnum = pBlock->vert_n;
        vt_size = 64;
        vtq_size = 24;
        vtwq_size = 16;

        while (l_vnum != 0) {
            if (l_vnum <= 64) {
                vt_size = l_vnum;
                vtq_size = (vt_size * 6) / 16;
                if ((vt_size * 6) & 0xF) {
                    vtq_size++;
                }
                vtwq_size = vt_size / 4;
                if ((vt_size * 4) & 0xF) {
                    vtwq_size++;
                }
            }

            *(u32 *)0x70002200 = DMAref | vtq_size;
            *(u32 *)0x70002204 = (u32)pVertex;
            *(u32 *)0x7000220C = SCE_VIF0_SET_UNPACK(Vu0Mem, vt_size, VIF_UNPACK_V3_16, 0);
            *(u32 *)0x70002210 = DMArefe | vtwq_size;
            *(u32 *)0x70002214 = (u32)pVertWt;
            *(u32 *)0x7000221C = SCE_VIF0_SET_UNPACK(0x4000 | Vu0Mem + 0x80, vt_size, VIF_UNPACK_V4_8, 0);
            DmaSend_Mac(pDma.Vif0, (u128 *)(0x70002200 | 0x80000000));
            SetMatrix4(SfxLsMtx[mi[0]], SfxLsMtx[mi[1]], SfxLsMtx[mi[2]], SfxLsMtx[mi[3]]);
            DmaSync_Mac(pDma.Vif0);
            RotTransPersWeight(pVertexI, Vu0Mem, Vu0Mem + 0x80, vt_size);
            pVertexI += vt_size;

            ((qword_uni *)0x70002230)->u_u32[0] = DMArefe | vtq_size;
            ((qword_uni *)0x70002230)->u_u32[1] = (u32)pNormal;
            ((qword_uni *)0x70002230)->u_u32[3] = SCE_VIF0_SET_UNPACK(Vu0Mem, vt_size, VIF_UNPACK_V3_16, 0);
            DmaSend_Mac(pDma.Vif0, (u128 *)(0x70002230 | 0x80000000));
            SetMatrix4(SfxLcLightMtx[mi[0]], SfxLcLightMtx[mi[1]], SfxLcLightMtx[mi[2]], SfxLcLightMtx[mi[3]]);
            sceDmaSync(pDma.Vif0, 0, 0);

            GetRgbWeight(pColorI, Vu0Mem, Vu0Mem + 0x80, vt_size);
            pColorI += vt_size;

            if (pParts->SpecType != 0) {
                SetMatrix4(SfxLvSpecMtx[mi[0]], SfxLvSpecMtx[mi[1]], SfxLvSpecMtx[mi[2]], SfxLvSpecMtx[mi[3]]);
                GetSpecUVWeight(pSpecUVI, Vu0Mem, Vu0Mem + 0x80, vt_size);
                pSpecUVI += vt_size;
            }

            pVertex += 24;
            pVertWt += 16;
            pNormal += 24;
            l_vnum -= vt_size;
        }
    }

    GetRegister(&vmin, &vmax);
    sceVu0FTOI4Vector(bboxTmp[0], vmax);
    sceVu0FTOI4Vector(bboxTmp[1], vmin);
}

void SfxMimeCalc(SFXOBJ *pObj, PARTS *pParts, u128 *pVertexI, u128 *pColorI, u128 *pSpecUVI) {
    s32 i;
    s32 l_vnum;
    u128 *pVertex;
    u128 *pNormal;
    u128 *pVertex2;
    u128 *pNormal2;
    u128 *pVertWt;
    TYPE_JOINT_BLK *pBlock;
    qword_uni DmaTag;
    s32 vt_size;
    s32 vtq_size;
    s32 vtwq_size;
    s32 Vu0Mem;
    sceVu0FVECTOR Scale;
    sceVu0FVECTOR clip;
    sceVu0FVECTOR clamp;
    s32 mi[4];
    s32 ii;

    ((qword_uni *)0x70002200)->u_u128 = 0;
    ((qword_uni *)0x70002210)->u_u128 = 0;
    ((qword_uni *)0x70002220)->u_u128 = 0;
    ((qword_uni *)0x70002230)->u_u128 = 0;

    if (pParts->OutLine == 0 || pParts->OutLine == 1 || pParts->OutLine == 4 || pParts->OutLine == 5 || pParts->OutLine == 6) {
        vmin[0] = 2368.0f;
        vmin[1] = 2160.0f;
        vmin[2] = 1048575.0f;
        vmin[3] = 0.0f;
        vmax[0] = 1728.0f;
        vmax[1] = 1936.0f;
        vmax[2] = 1.0f;
        vmax[3] = 0.0f;
    }

    Scale[0] = pObj->scale;
    Scale[1] = 1.0f / 255.0f;
    Scale[2] = 1.0 - pParts->MimeWeight;
    Scale[3] = pParts->MimeWeight;

    clip[0] = 4095.0f;
    clip[1] = 4095.0f;
    clip[2] = 32.0f;
    clip[3] = 1048575.0f;

    clamp[0] = 255.0f;
    clamp[1] = 255.0f;
    clamp[2] = 255.0f;
    clamp[3] = 2048.0f;

    Vu0RegisterInit(*pObj->pLightColor, &clamp, &Scale, &clip, &vmin, &vmax, 0x8000, 208);

    for (i = 0, pBlock = (TYPE_JOINT_BLK *)pParts->jblock_adrs; i < pParts->jblock_num; i++, pBlock++) {
        pVertex = (u128 *)((u32)pParts->vert_adrs_mime0 + (u32)pBlock->vert_ofs);
        pVertex2 = (u128 *)((u32)pParts->vert_adrs_mime1 + (u32)pBlock->vert_ofs);
        pNormal = (u128 *)((u32)pParts->norm_adrs_mime0 + (u32)pBlock->norm_ofs);
        pNormal2 = (u128 *)((u32)pParts->norm_adrs_mime1 + (u32)pBlock->norm_ofs);
        pVertWt = (u128 *)((u32)pBlock->v_wt_adrs + (u32)pParts->sfx_adrs);

        for (ii = 0; ii < 4; ii++) {
            if (pBlock->joint_id[ii] < 0) {
                mi[ii] = 0;
            } else {
                mi[ii] = pBlock->joint_id[ii];
            }
        }

        Vu0Mem = 0;
        l_vnum = pBlock->vert_n;
        vt_size = 64;
        vtq_size = 24;
        vtwq_size = 16;

        while (l_vnum != 0) {
            if (l_vnum <= 64) {
                vt_size = l_vnum;
                vtq_size = (vt_size * 6) / 16;
                if ((vt_size * 6) & 0xF) {
                    vtq_size++;
                }
                vtwq_size = vt_size / 4;
                if ((vt_size * 4) & 0xF) {
                    vtwq_size++;
                }
            }

            *(u32 *)0x70002200 = DMAref | vtq_size;
            *(u32 *)0x70002204 = (u32)pVertex;
            *(u32 *)0x7000220C = SCE_VIF0_SET_UNPACK(Vu0Mem, vt_size, VIF_UNPACK_V3_16, 0);
            *(u32 *)0x70002210 = DMAref | vtq_size;
            *(u32 *)0x70002214 = (u32)pVertex2;
            *(u32 *)0x7000221C = SCE_VIF0_SET_UNPACK(Vu0Mem + 0x40, vt_size, VIF_UNPACK_V3_16, 0);
            *(u32 *)0x70002220 = DMArefe | vtwq_size;
            *(u32 *)0x70002224 = (u32)pVertWt;
            *(u32 *)0x7000222C = SCE_VIF0_SET_UNPACK(0x4000 | Vu0Mem + 0x80, vt_size, VIF_UNPACK_V4_8, 0);
            DmaSend_Mac(pDma.Vif0, (u128 *)(0x70002200 | 0x80000000));
            SetMatrix4(SfxLsMtx[mi[0]], SfxLsMtx[mi[1]], SfxLsMtx[mi[2]], SfxLsMtx[mi[3]]);
            sceDmaSync(pDma.Vif0, 0, 0);
            RotTransPersMime(pVertexI, Vu0Mem, Vu0Mem + 0x40, Vu0Mem + 0x80, vt_size);
            pVertexI += vt_size;

            ((qword_uni *)0x70002200)->u_u32[0] = DMAref | vtq_size;
            ((qword_uni *)0x70002200)->u_u32[1] = (u32)pNormal;
            ((qword_uni *)0x70002200)->u_u32[3] = SCE_VIF0_SET_UNPACK(Vu0Mem, vt_size, VIF_UNPACK_V3_16, 0);
            ((qword_uni *)0x70002210)->u_u32[0] = DMArefe | vtq_size;
            ((qword_uni *)0x70002210)->u_u32[1] = (u32)pNormal2;
            ((qword_uni *)0x70002210)->u_u32[3] = SCE_VIF0_SET_UNPACK(Vu0Mem + 0x40, vt_size, VIF_UNPACK_V3_16, 0);
            DmaSend_Mac(pDma.Vif0, (u128 *)(0x70002200 | 0x80000000));
            SetMatrix4(SfxLcLightMtx[mi[0]], SfxLcLightMtx[mi[1]], SfxLcLightMtx[mi[2]], SfxLcLightMtx[mi[3]]);
            sceDmaSync(pDma.Vif0, 0, 0);

            GetRgbMime(pColorI, Vu0Mem, Vu0Mem + 0x40, Vu0Mem + 0x80, vt_size);
            pColorI += vt_size;

            if (pParts->SpecType != 0) {
                SetMatrix4(SfxLvSpecMtx[mi[0]], SfxLvSpecMtx[mi[1]], SfxLvSpecMtx[mi[2]], SfxLvSpecMtx[mi[3]]);
                GetSpecUVMime(pSpecUVI, Vu0Mem, Vu0Mem + 0x40, Vu0Mem + 0x80, vt_size);
                pSpecUVI += vt_size;
            }

            pVertex += 24;
            pVertex2 += 24;
            pNormal += 24;
            pNormal2 += 24;
            pVertWt += 16;

            l_vnum -= vt_size;
        }
    }

    GetRegister(&vmin, &vmax);
    sceVu0FTOI4Vector(bboxTmp[0], vmax);
    sceVu0FTOI4Vector(bboxTmp[1], vmin);
}

static void Vu0RegisterInit(sceVu0FMATRIX lc, sceVu0FVECTOR *c, sceVu0FVECTOR *sc, sceVu0FVECTOR *clip, sceVu0FVECTOR *vmin, sceVu0FVECTOR *vmax, u16 adc, u16 mac) {
    __asm__ volatile(
        "lqc2        $vf12, 0x0(%0)\n"
        "lqc2        $vf13, 0x10(%0)\n"
        "lqc2        $vf14, 0x20(%0)\n"
        "lqc2        $vf15, 0x30(%0)\n"
        "lqc2        $vf11, 0x0(%1)\n"
        "lqc2        $vf10, 0x0(%2)\n"
        "lqc2        $vf9, 0x0(%3)\n"
        "lqc2        $vf7, 0x0(%4)\n"
        "lqc2        $vf8, 0x0(%5)\n"
        "ctc2        %6, $vi4\n"
        "ctc2        %7, $vi5\n"
    : : "r" (lc), "r" (c), "r" (sc), "r" (clip), "r" (vmin), "r" (vmax), "r" (adc), "r" (mac));
}

static void SetMatrix(sceVu0FMATRIX m0) {
    __asm__ volatile(
        "lqc2        $vf16, 0x0(%0)\n"
        "lqc2        $vf17, 0x10(%0)\n"
        "lqc2        $vf18, 0x20(%0)\n"
        "lqc2        $vf19, 0x30(%0)\n"
    : : "r" (m0));
}

static void SetMatrix4(sceVu0FMATRIX m0, sceVu0FMATRIX m1, sceVu0FMATRIX m2, sceVu0FMATRIX m3) {
    __asm__ volatile(
        "lqc2        $vf16, 0x0(%0)\n"
        "lqc2        $vf17, 0x10(%0)\n"
        "lqc2        $vf18, 0x20(%0)\n"
        "lqc2        $vf19, 0x30(%0)\n"
        "lqc2        $vf20, 0x0(%1)\n"
        "lqc2        $vf21, 0x10(%1)\n"
        "lqc2        $vf22, 0x20(%1)\n"
        "lqc2        $vf23, 0x30(%1)\n"
        "lqc2        $vf24, 0x0(%2)\n"
        "lqc2        $vf25, 0x10(%2)\n"
        "lqc2        $vf26, 0x20(%2)\n"
        "lqc2        $vf27, 0x30(%2)\n"
        "lqc2        $vf28, 0x0(%3)\n"
        "lqc2        $vf29, 0x10(%3)\n"
        "lqc2        $vf30, 0x20(%3)\n"
        "lqc2        $vf31, 0x30(%3)\n"
    : : "r" (m0), "r" (m1), "r" (m2), "r" (m3));
}

static void RotTransPersWeight(u128 *v0, s32 vt, s32 wt, s32 num) {
    __asm__ volatile(
        "ctc2        %1, $vi1\n"
        "ctc2        %2, $vi3\n"
    "_rtpw_loop:\n"
        "vcallms     0x10\n"
        "qmfc2.I     $2, $vf1\n"
        "sqc2        $vf1, 0x0(%0)\n"
        "addi        %0, %0, 0x10\n"
        "addi        %3, %3, -0x1\n"
        "bne         $0, %3, _rtpw_loop\n"
    : : "r" (v0), "r" (vt), "r" (wt), "r" (num));
}

static void RotTransPers(u128 *v0, s32 vadrs, s32 num) {
    __asm__ volatile(
        "ctc2        %1, $vi1\n"
    "_rtp_l:\n"
        "vcallms     0x00\n"
        "qmfc2.I     $2, $vf1\n"
        "sqc2        $vf1, 0x0(%0)\n"
        "addi        %0, %0, 0x10\n"
        "addi        %2, %2, -0x1\n"
        "bne         $0, %2, _rtp_l\n"
    : : "r" (v0), "r" (vadrs), "r" (num));
}

static void GetRgb(u128 *vt, s32 nadrs, s32 num) {
    __asm__ volatile(
        "ctc2        %1, $vi1\n"
    "_rgb_loop:\n"
        "vcallms     0x40\n"
        "qmfc2.I     $2, $vf1\n"
        "sqc2        $vf1, 0x0(%0)\n"
        "addi        %0, %0, 0x10\n"
        "addi        %2, %2, -0x1\n"
        "bne         $0, %2, _rgb_loop\n"
        "nop\n"
    : : "r" (vt), "r" (nadrs), "r" (num));
}

static void GetRgbWeight(u128 *vt, s32 nadrs, s32 nwadrs, s32 num) {
    __asm__ volatile(
        "ctc2        %1, $vi1\n"
        "ctc2        %2, $vi3\n"
    "_rgbw_loop:\n"
        "vcallms     0x50\n"
        "qmfc2.I     $2, $vf1\n"
        "sqc2        $vf1, 0x0(%0)\n"
        "addi        %0, %0, 0x10\n"
        "addi        %3, %3, -0x1\n"
        "bne         $0, %3, _rgbw_loop\n"
        "nop\n"
    : : "r" (vt), "r" (nadrs), "r" (nwadrs), "r" (num));
}

static void RotTransPersMime(u128 *v0, s32 vt, s32 vt2, s32 wt, s32 num) {
    __asm__ volatile(
        "ctc2        %1, $vi1\n"
        "ctc2        %2, $vi2\n"
        "ctc2        %3, $vi3\n"
    "_rtpm_loop:\n"
        "vcallms     0x30\n"
        "qmfc2.I     $2, $vf1\n"
        "sqc2        $vf1, 0x0(%0)\n"
        "addi        %0, %0, 0x10\n"
        "addi        %4, %4, -0x1\n"
        "bne         $0, %4, _rtpm_loop\n"
        "nop\n"
    : : "r" (v0), "r" (vt), "r" (vt2), "r" (wt), "r" (num));
}

static void GetRgbMime(u128 *vt, s32 nm, s32 nm2, s32 nw, s32 num) {
    __asm__ volatile(
        "ctc2        %1, $vi1\n"
        "ctc2        %2, $vi2\n"
        "ctc2        %3, $vi3\n"
    "_rgbm_loop:\n"
        "vcallms     0x70\n"
        "qmfc2.I     $2, $vf1\n"
        "sqc2        $vf1, 0x0(%0)\n"
        "addi        %0, %0, 0x10\n"
        "addi        %4, %4, -0x1\n"
        "bne         $0, %4, _rgbm_loop\n"
        "nop\n"
    : : "r" (vt), "r" (nm), "r" (nm2), "r" (nw), "r" (num));
}

static void GetSpecUV(u128 *uv, s32 nadrs, s32 num) {
    __asm__ volatile(
        "ctc2        %1, $vi1\n"
    "_spec_loop:\n"
        "vcallms     0x80\n"
        "qmfc2.I     $2, $vf1\n"
        "sqc2        $vf1, 0x0(%0)\n"
        "addi        %0, %0, 0x10\n"
        "addi        %2, %2, -0x1\n"
        "bne         $0, %2, _spec_loop\n"
        "nop\n"
    : : "r" (uv), "r" (nadrs), "r" (num));
}

static void GetSpecUVWeight(u128 *vt, s32 nadrs, s32 nwadrs, s32 num) {
    __asm__ volatile(
        "ctc2        %1, $vi1\n"
        "ctc2        %2, $vi3\n"
    "_specw_loop:\n"
        "vcallms     0x90\n"
        "qmfc2.I     $2, $vf1\n"
        "sqc2        $vf1, 0x0(%0)\n"
        "addi        %0, %0, 0x10\n"
        "addi        %3, %3, -0x1\n"
        "bne         $0, %3, _specw_loop\n"
        "nop\n"
    : : "r" (vt), "r" (nadrs), "r" (nwadrs), "r" (num));
}

static void GetSpecUVMime(u128 *vt, s32 nm, s32 nm2, s32 nw, s32 num) {
    __asm__ volatile(
        "ctc2        %1, $vi1\n"
        "ctc2        %2, $vi2\n"
        "ctc2        %3, $vi3\n"
    "_specm_loop:\n"
        "vcallms     0xB0\n"
        "qmfc2.I     $2, $vf1\n"
        "sqc2        $vf1, 0x0(%0)\n"
        "addi        %0, %0, 0x10\n"
        "addi        %4, %4, -0x1\n"
        "bne         $0, %4, _specm_loop\n"
        "nop\n"
    : : "r" (vt), "r" (nm), "r" (nm2), "r" (nw), "r" (num));
}

static void GetRegister(sceVu0FVECTOR *min, sceVu0FVECTOR *max) {
    __asm__ volatile(
        "sqc2 $vf7, 0x0(%0)\n"
        "sqc2 $vf8, 0x0(%1)\n"
        "nop\n"
    : : "r" (min), "r" (max));
}
