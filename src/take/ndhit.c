#include "ndhit.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"
#include "okanoyo/okmain.h"
#include "object.h"

static sceVu0FVECTOR StarVec[12] = {
    { 0.0f, 0.0f, 1.0f, 1.0f },
    { 0.8944272f, 0.0f, 0.4472136f, 1.0f },
    { 0.2763932f, 0.8506508f, 0.4472136f, 1.0f },
    { -0.7236069f, 0.5257311f, 0.4472136f, 1.0f },
    { -0.7236068f, -0.5257311f, 0.4472136f, 1.0f },
    { 0.2763932f, -0.8506508f, 0.4472136f, 1.0f },
    { 0.7236067f, 0.5257311f, -0.4472137f, 1.0f },
    { -0.2763932f, 0.8506508f, -0.4472137f, 1.0f },
    { -0.8944271f, 0.0f, -0.4472137f, 1.0f },
    { -0.2763933f, -0.8506507f, -0.4472137f, 1.0f },
    { 0.7236069f, -0.5257308f, -0.4472137f, 1.0f },
    { 0.0f, 0.0f, -1.0f, 1.0f }
};
static f32 StarPlus[12] = { 4.0f, 5.0f, 6.0f, 4.0f, 5.0f, 6.0f, 4.0f, 5.0f, 6.0f, 4.0f, 5.5f, 6.0f };

OBJWORK* SetNdhit(sceVu0FVECTOR Vec) {
    OBJWORK *pObjw;
    NDHIT *ndhit;
    sceVu0FMATRIX m0;
    s32 i;

    pObjw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    InitObjWork(pObjw);
    pObjw->stat0 = 9;
    pObjw->dreff = DrawNdhit;
    ndhit = (NDHIT *)getmem(0x2000);
    pObjw->reg0 = (s32)ndhit;
    sceVu0CopyVector(ndhit->BaseVec, Vec);
    ndhit->Cnt = 32;
    sceVu0UnitMatrix(m0);
    sceVu0RotMatrixX(m0, m0, M_TWOPIf / 10.0f);
    sceVu0RotMatrixY(m0, m0, M_TWOPIf / 10.0f);
    sceVu0RotMatrixZ(m0, m0, M_TWOPIf / 10.0f);
    ndhit->SpinCnt = 0.0f;

    for (i = 0; i < 12; i++) {
        ndhit->Alpha[i] = 0x80;
        ndhit->Radius[i] = 0.0f;
        sceVu0ApplyMatrix(ndhit->Vec[i], m0, StarVec[i]);
    }
    NdhitEnvInit(ndhit);
    return pObjw;
}

void NdhitEnvInit(NDHIT *pNd) {
    s32 tex;
    s32 qword;

    tex = abGetTexIndex(260);
    pNd->px = texinfo[tex].px;
    pNd->py = texinfo[tex].py;
    pNd->pw = texinfo[tex].pw;
    pNd->ph = texinfo[tex].ph;
    pNd->tex0 = (texinfo[tex].tex0 | 0x400000000) & 0xffffffe7ffffffff;
    pNd->GsEnvInd = 2;
    pNd->GsEnv[pNd->GsEnvInd].u_u64[0] = SCE_GS_SET_CLAMP_2(0, 0, 0, 0, 0, 0);
    pNd->GsEnv[pNd->GsEnvInd].u_u64[1] = SCE_GS_CLAMP_2;
    pNd->GsEnvInd++;
    pNd->GsEnv[pNd->GsEnvInd].u_u64[0] = SCE_GS_SET_TEST_2(1, 6, 96, 1, 0, 0, 1, 2);
    pNd->GsEnv[pNd->GsEnvInd].u_u64[1] = SCE_GS_TEST_2;
    pNd->GsEnvInd++;
    pNd->GsEnv[pNd->GsEnvInd].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 1, 0, 1, 128);
    pNd->GsEnv[pNd->GsEnvInd].u_u64[1] = SCE_GS_ALPHA_2;
    pNd->GsEnvInd++;
    pNd->GsEnv[pNd->GsEnvInd].u_u64[0] = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMZ16S, 0);
    pNd->GsEnv[pNd->GsEnvInd].u_u64[1] = SCE_GS_ZBUF_2;
    qword = pNd->GsEnvInd;
    pNd->GsEnvInd++;

    SCE_GIF_CLEAR_TAG(pNd->GsEnv);
    pNd->GsEnv[0].u_u32[0] = 0x70000000 | qword;
    pNd->GsEnv[1].u_u64[0] = SCE_GIF_SET_TAG(qword - 1, 1, 0, 0, 0, 1);
    pNd->GsEnv[1].u_u64[1] = SCE_GIF_PACKED_AD;
}

void DrawNdhit(OBJWORK *pObjw) {
    NDHIT *ndhit;
    sceVu0FMATRIX m0;
    sceVu0FMATRIX m1;
    sceVu0FMATRIX m2;
    sceVu0IVECTOR v1[4];
    sceVu0FVECTOR v2;
    sceVu0FMATRIX m3;
    sceVu0FVECTOR v3[8];
    sceVu0FVECTOR pad[12];
    s32 i;
    f32 fVar1;
    f32 fVar2;

    ndhit = (NDHIT *)pObjw->reg0;
    sceVu0UnitMatrix(m0);
    sceVu0TransMatrix(m0, m0, ndhit->BaseVec);
    sceVu0MulMatrix(m0, Scr.WvMtx, m0);

    m0[0][0] = 1.0f;
    m0[0][1] = 0.0f;
    m0[0][2] = 0.0f;
    m0[0][3] = 0.0f;
    m0[1][0] = 0.0f;
    m0[1][1] = 1.0f;
    m0[1][2] = 0.0f;
    m0[1][3] = 0.0f;
    m0[2][0] = 0.0f;
    m0[2][1] = 0.0f;
    m0[2][2] = 1.0f;
    m0[2][3] = 0.0f;

    sceVu0MulMatrix(m0, Scr.VsMtx, m0);
    sceDmaSync(pDma.Gif, 0, 0);
    pDma.Gif->chcr.TTE = 0;
    FlushCache(0);
    sceDmaSend(pDma.Gif, ndhit->GsEnv);
    sceDmaSync(pDma.Gif, 0, 0);

    m3[0][0] = -12.0f;
    m3[0][1] = 12.0f;
    m3[0][2] = 0.0f;
    m3[0][3] = 0.0f;
    m3[1][0] = -12.0f;
    m3[1][1] = -12.0f;
    m3[1][2] = 0.0f;
    m3[1][3] = 0.0f;
    m3[2][0] = 12.0f;
    m3[2][1] = 12.0f;
    m3[2][2] = 0.0f;
    m3[2][3] = 0.0f;
    m3[3][0] = 12.0f;
    m3[3][1] = -12.0f;
    m3[3][2] = 0.0f;
    m3[3][3] = 0.0f;
    sceVu0UnitMatrix(m1);
    sceVu0RotMatrixZ(m1, m1, ndhit->SpinCnt);

    if (GameGbl.pause_flag == 0) {
        ndhit->SpinCnt += 0.448799;
        fVar1 = ndhit->Radius[i]; // i (s7) is unset tho lmao
        fVar2 = fVar1 - 10.0f;
        fVar1 -= 30.0f;
        if (fVar2 < 4.0f)
            fVar2 = 4.0f;
        if (fVar1 < 4.0f)
            fVar1 = 4.0f;
        if (ndhit->SpinCnt > 3.141592653589793)
            ndhit->SpinCnt -= 6.283185;

        for (i = 0; i < 12; i++) {
            ndhit->Radius[i] += StarPlus[i];
            ndhit->Alpha[i] -= 4;
        }

        ndhit->Cnt--;
        if (ndhit->Cnt < 1) {
            pObjw->stat0 = 0;
            freemem(pObjw->reg0);
            return;
        }
    }

    sceVu0ApplyMatrix(v3[0], m1, m3[0]);
    sceVu0ApplyMatrix(v3[1], m1, m3[1]);
    sceVu0ApplyMatrix(v3[2], m1, m3[2]);
    sceVu0ApplyMatrix(v3[3], m1, m3[3]);

    for (i = 0; i < 12; i++) {
        sceVu0ScaleVectorXYZ(v2, ndhit->Vec[i], ndhit->Radius[i]);
        sceVu0AddVector(v3[4], v3[0], v2);
        sceVu0AddVector(v3[5], v3[1], v2);
        sceVu0AddVector(v3[6], v3[2], v2);
        sceVu0AddVector(v3[7], v3[3], v2);
        sceVu0RotTransPers(v1[0], m0, v3[4], 0);
        if (v1[0][2] < 0x1000000U && v1[0][0] > 0x57FF && v1[0][0] < 0xA801 && v1[0][1] > 0x57FF && v1[0][1] < 0xA801) {
            sceVu0RotTransPers(v1[1], m0, v3[5], 0);
            sceVu0RotTransPers(v1[2], m0, v3[6], 0);
            sceVu0RotTransPers(v1[3], m0, v3[7], 0);
            DrawNdhitSub(v1, ndhit->px, ndhit->py, ndhit->pw, ndhit->ph, ndhit->tex0, ndhit->Alpha[i]);
            sceVu0ScaleVectorXYZ(v3[4], ndhit->Vec[i], fVar2);
            sceVu0ScaleVectorXYZ(v3[5], ndhit->Vec[i], fVar1);
            sceVu0RotTransPers(v1[0], m0, v3[4], 0);
            sceVu0RotTransPers(v1[1], m0, v3[5], 0);
            DrawNdhitSub2(v1[0], v1[1], ndhit->Alpha[i]);
        }
    }
}

void DrawNdhitSub(sceVu0IVECTOR *Xyz, s32 px, s32 py, s32 pw, s32 ph, u64 tex0, u8 alpha) {
    qword buff[5];
    qword_uni *packet;
    qword_uni *spr;

    spr = (qword_uni *)0x70000000;
    packet = (qword_uni *)buff;

    packet[4].u_u32[0] = 0xC0;
    packet[4].u_u32[1] = 0xC0;
    packet[4].u_u32[2] = 0xC0;
    packet[4].u_u32[3] = alpha;

    packet[0].u_u32[0] = px * 16 + 8;
    packet[0].u_u32[1] = py * 16 + 8;
    packet[0].u_u32[2] = 0;
    packet[0].u_u32[3] = 0;

    packet[1].u_u32[0] = px * 16 + 8;
    packet[1].u_u32[1] = (py + ph) * 16 - 8;
    packet[1].u_u32[2] = 0;
    packet[1].u_u32[3] = 0;

    packet[2].u_u32[0] = (px + pw) * 16 - 8;
    packet[2].u_u32[1] = py * 16 + 8;
    packet[2].u_u32[2] = 0;
    packet[2].u_u32[3] = 0;

    packet[3].u_u32[0] = (px + pw) * 16 - 8;
    packet[3].u_u32[1] = (py + ph) * 16 - 8;
    packet[3].u_u32[2] = 0;
    packet[3].u_u32[3] = 0;

    SCE_GIF_CLEAR_TAG(spr);
    spr->u_u32[0] = 0x7000000B;

    ((qword_uni *)0x70000010)->u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(4, 1, 1, 0, 1, 0, 1, 1, 0), 0, 10);
    ((qword_uni *)0x70000010)->u_u64[1] = (u64)SCE_GIF_PACKED_AD | (u64)SCE_GS_RGBAQ << 4 \
        | (u64)SCE_GS_UV << 8 | (u64)SCE_GS_XYZF2 << 12 \
        | (u64)SCE_GS_UV << 16 | (u64)SCE_GS_XYZF2 << 20 \
        | (u64)SCE_GS_UV << 24 | (u64)SCE_GS_XYZF2 << 28 \
        | (u64)SCE_GS_UV << 32 | (u64)SCE_GS_XYZF2 << 36;

    ((qword_uni *)0x70000020)->u_u64[0] = tex0;
    ((qword_uni *)0x70000020)->u_u64[1] = SCE_GS_TEX0_2;
    ((qword_uni *)0x70000030)->u_u128 = packet[4].u_u128;
    ((qword_uni *)0x70000040)->u_u128 = packet[0].u_u128;
    ((qword_uni *)0x70000050)->u_u128 = *(u128 *)Xyz[0];
    ((qword_uni *)0x70000060)->u_u128 = packet[1].u_u128;
    ((qword_uni *)0x70000070)->u_u128 = *(u128 *)Xyz[1];
    ((qword_uni *)0x70000080)->u_u128 = packet[2].u_u128;
    ((qword_uni *)0x70000090)->u_u128 = *(u128 *)Xyz[2];
    ((qword_uni *)0x700000A0)->u_u128 = packet[3].u_u128;
    ((qword_uni *)0x700000B0)->u_u128 = *(u128 *)Xyz[3];

    sceDmaSend(pDma.Gif, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(pDma.Gif, 0, 0);
}

extern void DrawNdhitSub2(sceVu0IVECTOR Xyz, sceVu0IVECTOR Xyz2, u8 alpha) {
    qword_uni RgbaQ;
    qword_uni *spr;

    spr = (qword_uni *)0x70000000;

    RgbaQ.u_u32[0] = 0xFF;
    RgbaQ.u_u32[1] = 0xFF;
    RgbaQ.u_u32[2] = 0x80;
    RgbaQ.u_u32[3] = alpha;

    SCE_GIF_CLEAR_TAG(spr);
    spr->u_u32[0] = 0x70000004;

    ((qword_uni *)0x70000010)->u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(2, 0, 0, 0, 1, 0, 0, 1, 0), 0, 3);
    ((qword_uni *)0x70000010)->u_u64[1] = (u64)SCE_GS_RGBAQ | (u64)SCE_GS_XYZF2 << 4 | (u64)SCE_GS_XYZF2 << 8;

    ((qword_uni *)0x70000020)->u_u128 = RgbaQ.u_u128;
    ((qword_uni *)0x70000030)->u_u128 = *(u128 *)Xyz;
    ((qword_uni *)0x70000040)->u_u128 = *(u128 *)Xyz2;

    sceDmaSend(pDma.Gif, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(pDma.Gif, 0, 0);
}
