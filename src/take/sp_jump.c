#include "take/sp_jump.h"
#include "take/object.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"
#include "okanoyo/okmain.h"

static void DrawSpJump(OBJWORK *pObjw);
static void SpJumpEnvInit(SPJUMP *pSj);
static void DrawSpJumpSub(sceVu0IVECTOR *Xyz, u64 tex0, s32 px, s32 py, s32 pw, s32 ph, u8 alpha);

OBJWORK* SetSpJump(SFXOBJ *pObj) {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    SPJUMP *pSj;
    s32 i;
    char stack[96]; // ?

    InitObjWork(objw);
    objw->stat0 = 9;
    objw->dreff = DrawSpJump;
    objw->drmiref = DrawSpJump;
    pSj = (SPJUMP *)getmem(0x2000);
    objw->reg0 = (s32)pSj;
    pSj->pObj = pObj;
    pSj->TotalCnt = 0;
    for (i = 0; i < 180; i++) {
        SETVEC(pSj->Vector[i], 0.0f, 0.0f, 0.0f, 0.0f);
        pSj->OnFlag[i] = 0;
        pSj->Size[i] = rand() % 7 + 3;
    }
    SpJumpEnvInit(pSj);
    return objw;
}

static void SpJumpEnvInit(SPJUMP *pSj) {
    s32 idx;
    u64 tex;
    u64 tex0[8];
    u16 py[8];
    u16 px[8];
    u16 ph[8];
    u16 pw[8];
    s32 i;

    idx = abGetTexIndex(260);
    px[0] = texinfo[idx].px;
    py[0] = texinfo[idx].py;
    pw[0] = texinfo[idx].pw;
    ph[0] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    tex0[0] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    idx = abGetTexIndex(5);
    px[1] = texinfo[idx].px;
    py[1] = texinfo[idx].py;
    pw[1] = texinfo[idx].pw;
    ph[1] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    tex0[1] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    idx = abGetTexIndex(6);
    px[2] = texinfo[idx].px;
    py[2] = texinfo[idx].py;
    pw[2] = texinfo[idx].pw;
    ph[2] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    tex0[2] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    idx = abGetTexIndex(7);
    px[3] = texinfo[idx].px;
    py[3] = texinfo[idx].py;
    pw[3] = texinfo[idx].pw;
    ph[3] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    tex0[3] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    idx = abGetTexIndex(8);
    px[4] = texinfo[idx].px;
    py[4] = texinfo[idx].py;
    pw[4] = texinfo[idx].pw;
    ph[4] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    tex0[4] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    idx = abGetTexIndex(9);
    px[5] = texinfo[idx].px;
    py[5] = texinfo[idx].py;
    pw[5] = texinfo[idx].pw;
    ph[5] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    tex0[5] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    idx = abGetTexIndex(10);
    px[6] = texinfo[idx].px;
    py[6] = texinfo[idx].py;
    pw[6] = texinfo[idx].pw;
    ph[6] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    tex0[6] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    for (i = 0; i < 7; i++) {
        pSj->tex0[i] = tex0[i];
        pSj->px[i] = px[i];
        pSj->py[i] = py[i];
        pSj->ph[i] = ph[i];
        pSj->pw[i] = pw[i];
    }

    for (i = 0; i < 180; i++) {
        s32 h = rand() % 7;
        pSj->type[i] = h;
    }

    pSj->GsEnvInd = 2;
    pSj->GsEnv[pSj->GsEnvInd].u_u64[0] = SCE_GS_SET_CLAMP_2(0, 0, 0, 0, 0, 0);
    pSj->GsEnv[pSj->GsEnvInd].u_u64[1] = SCE_GS_CLAMP_2;
    pSj->GsEnvInd++;
    pSj->GsEnv[pSj->GsEnvInd].u_u64[0] = SCE_GS_SET_TEST_2(1, 6, 16, 1, 0, 0, 1, 2);
    pSj->GsEnv[pSj->GsEnvInd].u_u64[1] = SCE_GS_TEST_2;
    pSj->GsEnvInd++;
    pSj->GsEnv[pSj->GsEnvInd].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 2, 0, 1, 0x80);
    pSj->GsEnv[pSj->GsEnvInd].u_u64[1] = SCE_GS_ALPHA_2;
    pSj->GsEnvInd++;
    pSj->GsEnv[pSj->GsEnvInd].u_u64[0] = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMZ16S, 1);
    pSj->GsEnv[pSj->GsEnvInd].u_u64[1] = SCE_GS_ZBUF_2;
    pSj->GsEnvInd++;

    pSj->GsEnv[0].u_u128 = 0;
    pSj->GsEnv[0].u_u32[0] = 0x70000000 | pSj->GsEnvInd - 1;
    pSj->GsEnv[1].u_u64[0] = SCE_GIF_SET_TAG(pSj->GsEnvInd - 2, 1, 0, 0, 0, 1);
    pSj->GsEnv[1].u_u64[1] = SCE_GIF_PACKED_AD;
}

static void DrawSpJump(OBJWORK *pObjw) {
    SPJUMP *pSj = (SPJUMP *)pObjw->reg0;
    HERO_WORK *herow = GameGbl.klonoa->work;
    s32 i;

    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSync(pDma.Gif, 0, 0);
    sceDmaSend(pDma.Gif, pSj->GsEnv);
    sceDmaSync(pDma.Gif, 0, 0);

    if ((pSj->LastPos[1] > pSj->pObj->pMot->pBaseCoord->Mtx[3][1] && !(herow->gimm_flag & 0x800)) || (pSj->LastPos[1] < pSj->pObj->pMot->pBaseCoord->Mtx[3][1] && herow->gimm_flag & 0x800)) {
        if (pSj->TotalCnt < 60) {
            for (i = 0; i < 180; i++) {
                if (!pSj->OnFlag[i]) {
                    pSj->Alpha[i] = 0x80;
                    sceVu0CopyVector(pSj->Pos[i], pSj->pObj->pMot->pBaseCoord->Mtx[3]);
                    pSj->Pos[i][0] += ((rand() >> 3) % 31) - 15;
                    pSj->Pos[i][2] += ((rand() >> 3) % 31) - 15;
                    pSj->Vector[i][0] = ((rand() >> 3) % 7) - 3;
                    pSj->Vector[i][2] = ((rand() >> 3) % 7) - 3;
                    pSj->OnFlag[i] = 1;
                    break;
                }
            }

            for (i = 0; i < 180; i++) {
                if (!pSj->OnFlag[i]) {
                    pSj->Alpha[i] = 0x80;
                    sceVu0CopyVector(pSj->Pos[i], pSj->pObj->pMot->pBaseCoord->Mtx[3]);
                    pSj->Pos[i][0] += ((rand() >> 3) % 31) - 15;
                    pSj->Pos[i][2] += ((rand() >> 3) % 31) - 15;
                    pSj->Vector[i][0] = ((rand() >> 3) % 7) - 3;
                    pSj->Vector[i][2] = ((rand() >> 3) % 7) - 3;
                    pSj->OnFlag[i] = 1;
                    break;
                }
            }
        }
    }

    sceVu0CopyVector(pSj->LastPos, pSj->pObj->pMot->pBaseCoord->Mtx[3]);
    for (i = 0; i < 180; i++) {
        if (pSj->OnFlag[i]) {
            pSj->Pos[i][0] += pSj->Vector[i][0];
            pSj->Pos[i][1] += pSj->Vector[i][1];
            pSj->Pos[i][2] += pSj->Vector[i][2];
            sceVu0ScaleVectorXYZ(pSj->Vector[i], pSj->Vector[i], 0.95f);
            pSj->Alpha[i] -= 2;
            if (pSj->Alpha[i] == 0) {
                pSj->OnFlag[i] = 0;
            }
        }
    }

    for (i = 0; i < 180; i++) {
        sceVu0IVECTOR vi[4];
        sceVu0FVECTOR vf[5];
        sceVu0FMATRIX m;

        if (pSj->OnFlag[i]) {
            sceVu0UnitMatrix(m);
            sceVu0TransMatrix(m, m, pSj->Pos[i]);
            sceVu0MulMatrix(m, Scr.WvMtx, m);

            SETVEC(m[0], 1.0f, 0.0f, 0.0f, 0.0f);
            SETVEC(m[1], 0.0f, 1.0f, 0.0f, 0.0f);
            SETVEC(m[2], 0.0f, 0.0f, 1.0f, 0.0f);
            sceVu0MulMatrix(m, Scr.VsMtx, m);

            SETVEC(vf[0], -pSj->Size[i], pSj->Size[i], 20.0f, 1.0f);
            SETVEC(vf[1], -pSj->Size[i], -pSj->Size[i], 20.0f, 1.0f);
            SETVEC(vf[2], pSj->Size[i], pSj->Size[i], 20.0f, 1.0f);
            SETVEC(vf[3], pSj->Size[i], -pSj->Size[i], 20.0f, 1.0f);
            sceVu0RotTransPers(vi[0], m, vf[0], 0);

            if (vi[0][2] < 0x1000000U && vi[0][0] >= 0x5800 && vi[0][0] <= 0xA800 && vi[0][1] >= 0x5800 && vi[0][1] <= 0xA800) {
                sceVu0RotTransPers(vi[1], m, vf[1], 0);
                sceVu0RotTransPers(vi[2], m, vf[2], 0);
                sceVu0RotTransPers(vi[3], m, vf[3], 0);
                DrawSpJumpSub(vi, pSj->tex0[pSj->type[i]], pSj->px[pSj->type[i]], pSj->py[pSj->type[i]], pSj->pw[pSj->type[i]], pSj->ph[pSj->type[i]], pSj->Alpha[i]);
            }
        }
    }

    sceDmaSync(pDma.Gif, 0, 0);
    pSj->TotalCnt++;
    if (pSj->TotalCnt > 120) {
        pObjw->stat0 = 0;
        freemem((void *)pObjw->reg0);
    }
}

static void DrawSpJumpSub(sceVu0IVECTOR *Xyz, u64 tex0, s32 px, s32 py, s32 pw, s32 ph, u8 alpha) {
    sceVu0IVECTOR uv[4];
    sceVu0IVECTOR rgba;
    qword_uni *spr;
    s32 ind = 0;

    sceDmaSync(pDma.Gif, 0, 0);
    spr = (qword_uni *)0x70000000;
    SETVEC(rgba, 0xFF, 0xFF, 0x80, alpha);
    SETVEC(uv[0], (px << 4) + 8, (py << 4) + 8, 0, 0);
    SETVEC(uv[1], (px << 4) + 8, ((py + ph) << 4) - 8, 0, 0);
    SETVEC(uv[2], ((px + pw) << 4) - 8, (py << 4) + 8, 0, 0);
    SETVEC(uv[3], ((px + pw) << 4) - 8, ((py + ph) << 4) - 8, 0, 0);

    spr[ind].u_u128 = 0;
    spr[ind++].u_u32[0] = 0x70000000 | 11;
    spr[ind].u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 10);
    spr[ind++].u_u64[1] = (u64)SCE_GIF_PACKED_AD | (u64)SCE_GS_RGBAQ << 4
                          | (u64)SCE_GS_UV << 8 | (u64)SCE_GS_XYZF2 << 12
                          | (u64)SCE_GS_UV << 16 | (u64)SCE_GS_XYZF2 << 20
                          | (u64)SCE_GS_UV << 24 | (u64)SCE_GS_XYZF2 << 28
                          | (u64)SCE_GS_UV << 32 | (u64)SCE_GS_XYZF2 << 36;
    spr[ind].u_u64[0] = tex0;
    spr[ind++].u_u64[1] = SCE_GS_TEX0_2;
    spr[ind++].u_u128 = ((qword_uni *)rgba)->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)uv[0])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)Xyz[0])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)uv[1])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)Xyz[1])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)uv[2])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)Xyz[2])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)uv[3])->u_u128;
    spr[ind++].u_u128 = ((qword_uni *)Xyz[3])->u_u128;

    sceDmaSend(pDma.Gif, (void *)((u32)spr | 0x80000000));
    sceDmaSync(pDma.Gif, 0, 0);
}

void EraseSpJump(OBJWORK *pObjw, s32 Mode) {
    pObjw->stat0 = 0;
    freemem((void *)pObjw->reg0);
}
