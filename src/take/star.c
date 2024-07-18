#include "take/star.h"
#include "take/object.h"
#include "take/motip.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"
#include "okanoyo/okmain.h"

static void StarEnvInit(STAR *pSt);
static void DrawStar(OBJWORK *pObjw);
static void DrawStarSub(sceVu0IVECTOR *Xyz, s32 px, s32 py, s32 pw, s32 ph);

OBJWORK* SetStar(SFXOBJ *pObj) {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    STAR *pSt;
    s32 i;

    InitObjWork(objw);
    objw->stat0 = 9;
    objw->dreff = DrawStar;
    objw->drmiref = DrawStar;
    pSt = (STAR *)getmem(0x1000);
    objw->reg0 = (s32)pSt;
    pSt->pObj = pObj;
    pSt->Radius = 15.0f;
    pSt->Angle[0] = 0.0f;
    pSt->SpinAngle[0] = 0.0f;
    pSt->PartsNum = 18;
    for (i = 0; i < 5; i++) {
        pSt->Scale[i] = 1.0f;
    }
    StarEnvInit(pSt);
    
    return objw;
}

OBJWORK* SetPopStar(SFXOBJ *pObj) {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    STAR *pSt;
    s32 i;

    InitObjWork(objw);
    objw->stat0 = 9;
    objw->dreff = DrawStar;
    objw->drmiref = DrawStar;
    pSt = (STAR *)getmem(0x1000);
    objw->reg0 = (s32)pSt;
    pSt->pObj = pObj;
    pSt->Radius = 15.0f;
    pSt->Angle[0] = 0.0f;
    pSt->SpinAngle[0] = 0.0f;
    pSt->PartsNum = 1;
    for (i = 0; i < 5; i++) {
        pSt->Scale[i] = 1.0f;
    }
    StarEnvInit(pSt);
    
    return objw;
}

static void StarEnvInit(STAR *pSt) {
    s32 idx = abGetTexIndex(260);
    u64 tex0;
    s32 qword;
    
    pSt->px = texinfo[idx].px;
    pSt->py = texinfo[idx].py;
    pSt->pw = texinfo[idx].pw;
    pSt->ph = texinfo[idx].ph;
    tex0 = texinfo[idx].tex0;
    
    pSt->GsEnvInd = 2;
    pSt->GsEnv[pSt->GsEnvInd].u_u64[0] = (tex0 | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);
    pSt->GsEnv[pSt->GsEnvInd].u_u64[1] = SCE_GS_TEX0_2;
    pSt->GsEnvInd++;
    pSt->GsEnv[pSt->GsEnvInd].u_u64[0] = SCE_GS_SET_CLAMP_2(0, 0, 0, 0, 0, 0);
    pSt->GsEnv[pSt->GsEnvInd].u_u64[1] = SCE_GS_CLAMP_2;
    pSt->GsEnvInd++;
    pSt->GsEnv[pSt->GsEnvInd].u_u64[0] = SCE_GS_SET_TEST_2(1, 6, 96, 1, 0, 0, 1, 2);
    pSt->GsEnv[pSt->GsEnvInd].u_u64[1] = SCE_GS_TEST_2;
    pSt->GsEnvInd++;
    pSt->GsEnv[pSt->GsEnvInd].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 1, 0, 1, 128);
    pSt->GsEnv[pSt->GsEnvInd].u_u64[1] = SCE_GS_ALPHA_2;
    pSt->GsEnvInd++;
    pSt->GsEnv[pSt->GsEnvInd].u_u64[0] = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMZ16S, 0);
    pSt->GsEnv[pSt->GsEnvInd].u_u64[1] = SCE_GS_ZBUF_2;
    
    qword = pSt->GsEnvInd;
    pSt->GsEnvInd++;
    pSt->GsEnv[0].u_u128 = 0;
    pSt->GsEnv[0].u_u32[0] = 0x70000000 | qword;
    pSt->GsEnv[1].u_u64[0] = SCE_GIF_SET_TAG(qword - 1, 0, 0, 0, 0, 1);
    pSt->GsEnv[1].u_u64[1] = SCE_GIF_PACKED_AD;
}

static void DrawStar(OBJWORK *pObjw) {
    sceVu0FMATRIX m;
    sceVu0FMATRIX mtx[5];
    sceVu0IVECTOR vi[4];
    sceVu0FVECTOR local_170[4];
    sceVu0FVECTOR vf[4];
    sceVu0FVECTOR angle[5];
    STAR *pSt;
    s32 i;

    SETVEC(local_170[0], -4.0f, 4.0f, 0.0f, 1.0f);
    SETVEC(local_170[1], -4.0f, -4.0f, 0.0f, 1.0f);
    SETVEC(local_170[2], 4.0f, 4.0f, 0.0f, 1.0f);
    SETVEC(local_170[3], 4.0f, -4.0f, 0.0f, 1.0f);
    pSt = (STAR* )pObjw->reg0;
    
    sceVu0UnitMatrix(m);
    sceVu0CopyVector(m[3], pSt->pObj->pMot->pCoord[pSt->PartsNum].Mtx[3]);
    
    if (!GameGbl.pause_flag) {
        pSt->Angle[0] += M_PI / 40.0f;
        if (pSt->Angle[0] > M_PI) {
            pSt->Angle[0] -= M_PI * 2.0f;
        }
        if (pSt->Angle[0] < -M_PI) {
            pSt->Angle[0] += M_PI * 2.0f;
        }
        
        pSt->SpinAngle[0] += M_PI / 30.0f;
        if (pSt->SpinAngle[0] > M_PI) {
            pSt->SpinAngle[0] -= M_PI * 2.0f;
        }
        if (pSt->SpinAngle[0] < -M_PI) {
            pSt->SpinAngle[0] += M_PI * 2.0f;
        }
    }

    for (i = 0; i < 5; i++) {
        pSt->Angle[i] = pSt->Angle[0] + (M_PI * 2.0f / 5.0f * (f32)i);
        if (pSt->Angle[i] > M_PI) {
            pSt->Angle[i] -= M_PI * 2.0f;
        }
        if (pSt->Angle[i] < -M_PI) {
            pSt->Angle[i] += M_PI * 2.0f;
        }
        
        pSt->SpinAngle[i] = pSt->SpinAngle[0] + (M_PI * 2.0f / 5.0f * (f32)i);
        if (pSt->SpinAngle[i] > M_PI) {
            pSt->SpinAngle[i] -= M_PI * 2.0f;
        }
        if (pSt->SpinAngle[i] < -M_PI) {
            pSt->SpinAngle[i] += M_PI * 2.0f;
        }

        // KPRS does += instead of =, hmm...
        angle[i][0] = __cos(pSt->Angle[i]) * pSt->Radius;
        angle[i][1] = -18.0f;
        angle[i][2] = __sin(pSt->Angle[i]) * pSt->Radius;
        angle[i][3] = 1.0f;
    }

    for (i = 0; i < 5; i++) {
        sceVu0UnitMatrix(mtx[i]);
        sceVu0RotMatrixZ(mtx[i], mtx[i], pSt->SpinAngle[i]);
        sceVu0ApplyMatrix(vf[0], mtx[i], local_170[0]);
        sceVu0ApplyMatrix(vf[1], mtx[i], local_170[1]);
        sceVu0ApplyMatrix(vf[2], mtx[i], local_170[2]);
        sceVu0ApplyMatrix(vf[3], mtx[i], local_170[3]);
        sceVu0UnitMatrix(mtx[i]);
        sceVu0TransMatrix(mtx[i], mtx[i], angle[i]);
        sceVu0MulMatrix(mtx[i], m, mtx[i]);
        sceVu0MulMatrix(mtx[i], Scr.WvMtx, mtx[i]);
        SETVEC(mtx[i][0], pSt->Scale[i], 0.0f, 0.0f, 0.0f);
        SETVEC(mtx[i][1], 0.0f, pSt->Scale[i], 0.0f, 0.0f);
        SETVEC(mtx[i][2], 0.0f, 0.0f, pSt->Scale[i], 0.0f);
        sceVu0MulMatrix(mtx[i], Scr.VsMtx, mtx[i]);
        sceVu0RotTransPers(vi[0], mtx[i], vf[0], 0);
        if (vi[0][2] < 0x1000000U && vi[0][0] >= 0x5800 && vi[0][0] <= 0xA800 && vi[0][1] >= 0x5800 && vi[0][1] <= 0xA800) {
            sceVu0RotTransPers(vi[1], mtx[i], vf[1], 0);
            sceVu0RotTransPers(vi[2], mtx[i], vf[2], 0);
            sceVu0RotTransPers(vi[3], mtx[i], vf[3], 0);
            FlushCache(0);
            pDma.Gif->chcr.TTE = 0;
            sceDmaSend(pDma.Gif, pSt->GsEnv);
            sceDmaSync(pDma.Gif, 0, 0);
            DrawStarSub(vi, pSt->px, pSt->py, pSt->pw, pSt->ph);
        }
    }
}

static void DrawStarSub(sceVu0IVECTOR *Xyz, s32 px, s32 py, s32 pw, s32 ph) {
    sceVu0IVECTOR uv[4];
    sceVu0IVECTOR rgba;
    qword_uni *spr;
    qword_uni *spr2;
    s32 ind = 0;
    
    spr = SPR_MEM;
    SETVEC(rgba, 0x80, 0x80, 0x80, 0x80);
    SETVEC(uv[0], (px << 4) + 8, (py << 4) + 8, 0, 0);
    SETVEC(uv[1], (px << 4) + 8, ((py + ph) << 4) - 8, 0, 0);
    SETVEC(uv[2], ((px + pw) << 4) - 8, (py << 4) + 8, 0, 0);
    SETVEC(uv[3], ((px + pw) << 4) - 8, ((py + ph) << 4) - 8, 0, 0);

    spr[ind].u_u128 = 0;
    spr[ind++].u_u32[0] = 0x70000000 | 13;
    spr[ind].u_u64[0] = SCE_GIF_SET_TAG(4, 0, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    spr[ind++].u_u64[1] = (u64)SCE_GS_UV | (u64)SCE_GS_RGBAQ << 4 | (u64)SCE_GS_XYZF2 << 8;
    spr[ind++].u_u128 = ((qword_uni *)uv[0])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)rgba)->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)Xyz[0])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)uv[1])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)rgba)->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)Xyz[1])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)uv[2])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)rgba)->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)Xyz[2])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)uv[3])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)rgba)->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)Xyz[3])->u_u128;

    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, SPR_SRC(spr));
    sceDmaSync(pDma.Gif, 0, 0);

    spr2 = SPR_MEM_IDX(0x230);
    ind = 0;
    spr2[ind].u_u128 = 0;
    spr2[ind++].u_u32[0] = 0x70000000 | 1;
    spr2[ind].u_u64[0] = SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1);
    spr2[ind++].u_u64[1] = 0xF;
    sceDmaSync(pDma.Gif, 0, 0);
    sceDmaSend(pDma.Gif, SPR_SRC(spr2));
}
