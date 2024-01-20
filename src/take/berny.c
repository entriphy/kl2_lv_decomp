#include "take/berny.h"
#include "take/object.h"
#include "take/motip.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"
#include "okanoyo/okmain.h"

static void BernyEnvInit(BERNY *pBe);
static void DrawBerny(OBJWORK *pObjw);
static void DrawBernySub(sceVu0IVECTOR *Pos, u64 tex0, s32 px, s32 py, s32 pw, s32 ph, u8 alpha);

OBJWORK* SetBernyFire(SFXOBJ *pObj) {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    BERNY *pBe;
    s32 i, j;
    s32 cnt;
    f32 clip;
    SFXOBJ *pObjTmp;

    InitObjWork(objw);
    objw->stat0 = 9;
    objw->dreff = DrawBerny;
    objw->drmir = DrawBerny;
    pBe = (BERNY *)getmem(0x1000);
    objw->reg0 = (s32)pBe;
    pBe->pObj = pObj;
    pBe->Radius = 35.0f;
    pBe->Speed = M_PIf / 40.0f;
    pBe->Angle[0] = 0.0f;

    for (i = 0, cnt = 0; i < 5; i++) {
        pBe->Scale[i] = 1.0f;
        for (j = 0; j < 4; j++) {
            pBe->Cnt[i][j] = cnt;
            cnt = (cnt + 32) % 128;
        }
    }

    clip = pObj->ClipZone * 2.0f;
    for (pObjTmp = pObj; pObjTmp != NULL; pObjTmp = pObjTmp->pObjSub) {
        pObjTmp->ClipZone = clip;
    }

    BernyEnvInit(pBe);
    return objw;
}

static void BernyEnvInit(BERNY *pBe) {
    s32 idx;
    u64 tex;
    s32 qword;

    idx = abGetTexIndex(261);
    pBe->px[0] = texinfo[idx].px;
    pBe->py[0] = texinfo[idx].py;
    pBe->pw[0] = texinfo[idx].pw;
    pBe->ph[0] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    pBe->tex0[0] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    idx = abGetTexIndex(262);
    pBe->px[1] = texinfo[idx].px;
    pBe->py[1] = texinfo[idx].py;
    pBe->pw[1] = texinfo[idx].pw;
    pBe->ph[1] = texinfo[idx].ph;\
    tex = texinfo[idx].tex0;
    pBe->tex0[1] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    idx = abGetTexIndex(263);
    pBe->px[2] = texinfo[idx].px;
    pBe->py[2] = texinfo[idx].py;
    pBe->pw[2] = texinfo[idx].pw;
    pBe->ph[2] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    pBe->tex0[2] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    idx = abGetTexIndex(264);
    pBe->px[3] = texinfo[idx].px;
    pBe->py[3] = texinfo[idx].py;
    pBe->pw[3] = texinfo[idx].pw;
    pBe->ph[3] = texinfo[idx].ph;
    tex = texinfo[idx].tex0;
    pBe->tex0[3] = (tex | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    pBe->GsEnvInd = 2;
    pBe->GsEnv[pBe->GsEnvInd].u_u64[0] = SCE_GS_SET_CLAMP_2(0, 0, 0, 0, 0, 0);
    pBe->GsEnv[pBe->GsEnvInd].u_u64[1] = SCE_GS_CLAMP_2;
    pBe->GsEnvInd++;
    pBe->GsEnv[pBe->GsEnvInd].u_u64[0] = 0x5160D;
    pBe->GsEnv[pBe->GsEnvInd].u_u64[1] = SCE_GS_TEST_2;
    pBe->GsEnvInd++;
    pBe->GsEnv[pBe->GsEnvInd].u_u64[0] = 0x8000000048;
    pBe->GsEnv[pBe->GsEnvInd].u_u64[1] = SCE_GS_ALPHA_2;
    pBe->GsEnvInd++;
    pBe->GsEnv[pBe->GsEnvInd].u_u64[0] = 0x3A00008C;
    pBe->GsEnv[pBe->GsEnvInd].u_u64[1] = SCE_GS_ZBUF_2;
    qword = pBe->GsEnvInd;
    pBe->GsEnvInd++;

    pBe->GsEnv[0].u_u128 = 0;
    pBe->GsEnv[0].u_u32[0] = qword | 0x70000000;
    pBe->GsEnv[1].u_u64[0] = SCE_GIF_SET_TAG(qword - 1, 1, 0, 0, 0, 1);
    pBe->GsEnv[1].u_u64[1] = SCE_GIF_PACKED_AD;
}

static void DrawBerny(OBJWORK *pObjw) {
    sceVu0FMATRIX m;
    sceVu0FMATRIX mat[5];
    sceVu0IVECTOR vi[4];
    sceVu0FVECTOR vf0[4];
    sceVu0FVECTOR vf1[4];
    sceVu0FVECTOR vf2[5];
    BERNY *pBe;
    s32 i, j;

    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;

    SETVEC(vf0[0], -16.0f, -32.0f, 0.0f, 1.0f);
    SETVEC(vf0[1], -16.0f, 16.0f, 0.0f, 1.0f);
    SETVEC(vf0[2], 16.0f, -32.0f, 0.0f, 1.0f);
    SETVEC(vf0[3], 16.0f, 16.0f, 0.0f, 1.0f);

    pBe = (BERNY *)pObjw->reg0;
    if (pBe->pObj->OutFlag != 0) {
        return;
    }

    sceVu0UnitMatrix(m);
    sceVu0CopyVector(m[3], pBe->pObj->pMot->pCoord[14].Mtx[3]); // ?

    if (!GameGbl.pause_flag) {
        pBe->Angle[0] += pBe->Speed;
        if (pBe->Angle[0] > M_PI) {
            pBe->Angle[0] -= M_PI * 2.0;
        }
        if (pBe->Angle[0] < -M_PI) {
            pBe->Angle[0] += M_PI * 2.0;
        }
    }

    for (i = 0; i < 5; i++) {
        pBe->Angle[i] = pBe->Angle[0] + (M_PI * 2.0 / 5.0 * (f32)i);
        if (pBe->Angle[i] > M_PI) {
            pBe->Angle[i] -= M_PI * 2.0;
        }
        if (pBe->Angle[i] < -M_PI) {
            pBe->Angle[i] += M_PI * 2.0;
        }
        vf2[i][0] = __cos(pBe->Angle[i]) * pBe->Radius;
        vf2[i][1] = 0.0f;
        vf2[i][2] = __sin(pBe->Angle[i]) * pBe->Radius;
        vf2[i][3] = 1.0f;
    }

    sceDmaSync(pDma.Gif, 0, 0);
    sceDmaSend(pDma.Gif, pBe->GsEnv);
    sceDmaSync(pDma.Gif, 0, 0);

    for (i = 0; i < 5; i++) {
        if (!GameGbl.pause_flag) {
            pBe->Cnt[i][0] = (pBe->Cnt[i][0] + ((rand() >> 3) % 20)) % 128;
        }
        pBe->Cnt[i][1] = (pBe->Cnt[i][0] + 32) % 128;
        pBe->Cnt[i][2] = (pBe->Cnt[i][0] + 64) % 128;
        pBe->Cnt[i][3] = (pBe->Cnt[i][0] + 96) % 128;

        for (j = 0; j < 4; j++) {
            if ((s16)pBe->Cnt[i][j] < 64) {
                pBe->Alpha[i][j] = 0x00;
            } else if ((s16)pBe->Cnt[i][j] < 96) {
                pBe->Alpha[i][j] = pBe->Cnt[i][j] - 64;
            } else {
                pBe->Alpha[i][j] = 128 - pBe->Cnt[i][j];
            }
            pBe->Alpha[i][j] <<= 1;
        }

        sceVu0UnitMatrix(mat[i]);
        sceVu0ApplyMatrix(vf1[0], mat[i], vf0[0]);
        sceVu0ApplyMatrix(vf1[1], mat[i], vf0[1]);
        sceVu0ApplyMatrix(vf1[2], mat[i], vf0[2]);
        sceVu0ApplyMatrix(vf1[3], mat[i], vf0[3]);
        sceVu0UnitMatrix(mat[i]);
        sceVu0TransMatrix(mat[i], mat[i], vf2[i]);
        sceVu0MulMatrix(mat[i], m, mat[i]);
        sceVu0MulMatrix(mat[i], Scr.WvMtx, mat[i]);

        SETVEC(mat[i][0], pBe->Scale[i], 0.0f, 0.0f, 0.0f);
        SETVEC(mat[i][1], 0.0f, pBe->Scale[i], 0.0f, 0.0f);
        SETVEC(mat[i][2], 0.0f, 0.0f, pBe->Scale[i], 0.0f);

        sceVu0MulMatrix(mat[i], Scr.VsMtx, mat[i]);
        sceVu0RotTransPers(vi[0], mat[i], vf1[0], 0);

        if (vi[0][2] < 0x1000000U && vi[0][0] >= 0x5800 && vi[0][0] <= 0xA800 && vi[0][1] >= 0x5800 && vi[0][1] <= 0xA800) {
            sceVu0RotTransPers(vi[1], mat[i], vf1[1], 0);
            sceVu0RotTransPers(vi[2], mat[i], vf1[2], 0);
            sceVu0RotTransPers(vi[3], mat[i], vf1[3], 0);
            DrawBernySub(vi, pBe->tex0[0], pBe->px[0], pBe->py[0], pBe->pw[0], pBe->ph[0], pBe->Alpha[i][0]);
            DrawBernySub(vi, pBe->tex0[1], pBe->px[1], pBe->py[1], pBe->pw[1], pBe->ph[1], pBe->Alpha[i][1]);
            DrawBernySub(vi, pBe->tex0[2], pBe->px[2], pBe->py[2], pBe->pw[2], pBe->ph[2], pBe->Alpha[i][2]);
            DrawBernySub(vi, pBe->tex0[3], pBe->px[3], pBe->py[3], pBe->pw[3], pBe->ph[3], pBe->Alpha[i][3]);
        }
    }
}

void SetBernyData(OBJWORK *pObjw, f32 Radius, f32 Speed) {
    BERNY *pBe = (BERNY *)pObjw->reg0;
    pBe->Speed = Speed;
    pBe->Radius = Radius;
}

static void DrawBernySub(sceVu0IVECTOR *Pos, u64 tex0, s32 px, s32 py, s32 pw, s32 ph, u8 alpha) {
    sceVu0IVECTOR uv[4];
    sceVu0IVECTOR rgba;
    sceVu0IVECTOR xyz[8];
    qword_uni *spr = (qword_uni *)0x70000000;

    sceVu0CopyVector(xyz[0], Pos[0]);
    sceVu0CopyVector(xyz[1], Pos[1]);
    sceVu0CopyVector(xyz[2], Pos[2]);
    sceVu0CopyVector(xyz[3], Pos[3]);
    sceVu0CopyVector(xyz[4], Pos[0]);
    sceVu0CopyVector(xyz[5], Pos[1]);
    sceVu0CopyVector(xyz[6], Pos[2]);
    sceVu0CopyVector(xyz[7], Pos[3]);

    SETVEC(rgba, 128, 128, 128, alpha);
    SETVEC(uv[0], (px << 4) + 8, (py << 4) + 8, 0, 0);
    SETVEC(uv[1], (px << 4) + 8, ((py + ph) << 4) - 8, 0, 0);
    SETVEC(uv[2], ((px + pw) << 4) - 8, (py << 4) + 8, 0, 0);
    SETVEC(uv[3], ((px + pw) << 4) - 8, ((py + ph) << 4) - 8, 0, 0);

    spr->u_u128 = 0;
    spr->u_u32[0] = 0x70000000 | 11;
    ((qword_uni *)0x70000010)->u_u64[0] = SCE_GIF_SET_TAG(1, 0, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 10);
    ((qword_uni *)0x70000010)->u_u64[1] = (u64)SCE_GIF_PACKED_AD | (u64)SCE_GS_RGBAQ << 4 \
        | (u64)SCE_GS_UV << 8 | (u64)SCE_GS_XYZF2 << 12 \
        | (u64)SCE_GS_UV << 16 | (u64)SCE_GS_XYZF2 << 20 \
        | (u64)SCE_GS_UV << 24 | (u64)SCE_GS_XYZF2 << 28 \
        | (u64)SCE_GS_UV << 32 | (u64)SCE_GS_XYZF2 << 36;
    ((qword_uni *)0x70000020)->u_u64[0] = tex0;
    ((qword_uni *)0x70000020)->u_u64[1] = SCE_GS_TEX0_2;
    ((qword_uni *)0x70000030)->u_u128 = ((qword_uni *)rgba)->u_u128;
    ((qword_uni *)0x70000040)->u_u128 = ((qword_uni *)uv[0])->u_u128;
    ((qword_uni *)0x70000050)->u_u128 = ((qword_uni *)xyz[0])->u_u128;
    ((qword_uni *)0x70000060)->u_u128 = ((qword_uni *)uv[1])->u_u128;
    ((qword_uni *)0x70000070)->u_u128 = ((qword_uni *)xyz[1])->u_u128;
    ((qword_uni *)0x70000080)->u_u128 = ((qword_uni *)uv[2])->u_u128;
    ((qword_uni *)0x70000090)->u_u128 = ((qword_uni *)xyz[2])->u_u128;
    ((qword_uni *)0x700000A0)->u_u128 = ((qword_uni *)uv[3])->u_u128;
    ((qword_uni *)0x700000B0)->u_u128 = ((qword_uni *)xyz[3])->u_u128;

    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(pDma.Gif, 0, 0);

    ((qword_uni *)0x70002300)->u_u128 = 0;
    ((qword_uni *)0x70002300)->u_u32[0] = 0x70000000 | 1;
    ((qword_uni *)0x70002310)->u_u64[0] = SCE_GIF_SET_TAG(0, 1, 0, 0, 0, 1);
    ((qword_uni *)0x70002310)->u_u64[1] = 0xF; // NOP
    sceDmaSend(pDma.Gif, (void *)(0x70002300 | 0x80000000));
    sceDmaSync(pDma.Gif, 0, 0);
}

void EraseBernyFire(OBJWORK *pObjw) {
    pObjw->stat0 = 0;
    freemem((BERNY *)pObjw->reg0);
}
