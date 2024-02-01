#include "take/kurosuke.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"
#include "okanoyo/okmain.h"
#include "object.h"
#include "effsub.h"

static void KurosukeEnvInit(KUROSUKE *pKr);
static void DrawKurosuke(OBJWORK *pObjw);
static void DrawKurosukeSub(sceVu0IVECTOR *Xyz, s32 px, s32 py, s32 pw, s32 ph, u64 tex0, u8 alpha);

OBJWORK* SetKurosuke(SFXOBJ *pObj) {
    s32 i;
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    KUROSUKE *pKr;
    sceVu0FVECTOR v0, v1;
    sceVu0FMATRIX m;

    InitObjWork(objw);
    objw->stat0 = 9;
    objw->dreff = DrawKurosuke;

    pKr = getmem(0x1000);
    objw->reg0 = (s32)pKr;
    pKr->pObj = pObj;
    pKr->InRange = 100.0f;
    pKr->OutRange = 200.0f;
    pKr->Index = 0;
    pKr->TotalMode = 0;
    sceVu0CopyVector(v0, pKr->pObj->pMot->pBaseCoord->Mtx[3]);
    v0[1] -= 20.0f;

    for (i = 0; i < 24; i++) {
        pKr->Vector[i][0] = 0.0f;
        pKr->Vector[i][1] = 0.0f;
        pKr->Vector[i][2] = 0.0f;
        pKr->Vector[i][3] = 0.0f;
        pKr->Cnt[i] = 1;
        pKr->Xrot[i] = ((rand() >> 3) % 360) / 360.0f * M_TWOPI;
        pKr->Xrot[i] -= (f64)M_PI;
        pKr->Yrot[i] = ((rand() >> 3) % 360) / 360.0f * M_TWOPI;
        pKr->Yrot[i] -= (f64)M_PI;
        pKr->Zrot[i] = ((rand() >> 3) % 360) / 360.0f * M_TWOPI;
        pKr->Zrot[i] -= (f64)M_PI;
        pKr->Range[i] = 600.0f;
        pKr->Mode[i] = 0;

        sceVu0UnitMatrix(m);
        sceVu0RotMatrixX(m, m, pKr->Xrot[i]);
        sceVu0RotMatrixY(m, m, pKr->Yrot[i]);
        sceVu0RotMatrixZ(m, m, pKr->Zrot[i]);
        sceVu0TransMatrix(m, m, v0);

        v1[0] = pKr->Range[i];
        v1[1] = 0.0f;
        v1[2] = 0.0f;
        v1[3] = 1.0f;
        sceVu0ApplyMatrix(pKr->TargetPos[i], m, v1);
        sceVu0CopyVector(pKr->Pos[i], pKr->TargetPos[i]);
    }

    KurosukeEnvInit(pKr);
    return objw;
}

static void KurosukeEnvInit(KUROSUKE *pKr) {
    s32 idx;
    s32 qword;

    idx = abGetTexIndex(265);
    pKr->px = texinfo[idx].px;
    pKr->py = texinfo[idx].py;
    pKr->pw = texinfo[idx].pw;
    pKr->ph = texinfo[idx].ph;

    pKr->GsEnvInd = 2;
    pKr->GsEnv[pKr->GsEnvInd].u_u64[0] = SCE_GS_SET_CLAMP_2(0, 0, 0, 0, 0, 0);
    pKr->GsEnv[pKr->GsEnvInd].u_u64[1] = SCE_GS_CLAMP_2;
    pKr->GsEnvInd++;
    pKr->GsEnv[pKr->GsEnvInd].u_u64[0] = 0x5110D;
    pKr->GsEnv[pKr->GsEnvInd].u_u64[1] = SCE_GS_TEST_2;
    pKr->GsEnvInd++;
    pKr->GsEnv[pKr->GsEnvInd].u_u64[0] = 0x8000000044;
    pKr->GsEnv[pKr->GsEnvInd].u_u64[1] = SCE_GS_ALPHA_2;
    pKr->GsEnvInd++;
    pKr->GsEnv[pKr->GsEnvInd].u_u64[0] = 0x3A00008C;
    pKr->GsEnv[pKr->GsEnvInd].u_u64[1] = SCE_GS_ZBUF_2;
    qword = pKr->GsEnvInd;
    pKr->GsEnvInd++;

    pKr->GsEnv[0].u_u128 = 0;
    pKr->GsEnv[0].u_u32[0] = qword | 0x70000000;
    pKr->GsEnv[1].u_u64[0] = SCE_GIF_SET_TAG(qword - 1, 1, 0, 0, 0, 1);
    pKr->GsEnv[1].u_u64[1] = SCE_GIF_PACKED_AD;
}

static void DrawKurosuke(OBJWORK *pObjw) {
    sceVu0IVECTOR vi[4];
    sceVu0FVECTOR vf0[4];
    sceVu0FVECTOR vf1;
    sceVu0FVECTOR vf2;
    sceVu0FMATRIX m;
    sceVu0FVECTOR vf3[4] = {
        { 6.0f, 6.0f, 0.0f, 1.0f },
        { 6.0f, -6.0f, 0.0f, 1.0f },
        { -6.0f, 6.0f, 0.0f, 1.0f },
        { -6.0f, -6.0f, 0.0f, 1.0f },
    };
    KUROSUKE *pKr = (KUROSUKE *)pObjw->reg0;
    s32 i;
    s32 tex;
    u64 tex0;

    for (i = 0; i < 24; i++) {
        switch (pKr->Mode[i]) {
            case 0:
                pKr->InRange = 300.0f;
                pKr->OutRange = 400.0f;
                pKr->TopSpeed = 0.3f;
                pKr->Speed = 0.05f;
                if (pKr->Alpha != 0) {
                    pKr->Alpha--;
                } else {
                    pKr->Range[i] = 500.0f;
                    sceVu0CopyVector(vf1, pKr->pObj->pMot->pBaseCoord->Mtx[3]);
                    vf1[1] += -20.0f;
                    sceVu0UnitMatrix(m);
                    sceVu0RotMatrixX(m, m, pKr->Xrot[i]);
                    sceVu0RotMatrixY(m, m, pKr->Yrot[i]);
                    sceVu0RotMatrixZ(m, m, pKr->Zrot[i]);
                    sceVu0TransMatrix(m, m, vf1);
                    vf2[0] = pKr->Range[i];
                    vf2[1] = 0.0f;
                    vf2[2] = 0.0f;
                    vf2[3] = 1.0f;
                    sceVu0ApplyMatrix(pKr->TargetPos[i], m, vf2);
                    sceVu0CopyVector(pKr->Pos[i], pKr->TargetPos[i]);
                }

                tex = abGetTexIndex(265);
                pKr->px = texinfo[tex].px;
                pKr->py = texinfo[tex].py;
                pKr->pw = texinfo[tex].pw;
                pKr->ph = texinfo[tex].ph;
                tex0 = texinfo[tex].tex0;
                pKr->Tex0 = (tex0 | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

                tex = abGetTexIndex(265);
                pKr->px = texinfo[tex].px;
                pKr->py = texinfo[tex].py;
                pKr->pw = texinfo[tex].pw;
                pKr->ph = texinfo[tex].ph;
                tex0 = texinfo[tex].tex0;
                pKr->Tex0 = (tex0 | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);
                break;
            case 1:
                pKr->InRange = 100.0f;
                pKr->OutRange = 300.0f;
                pKr->TopSpeed = 1.0f;
                pKr->Speed = 0.1f;
                pKr->Alpha = 32;

                tex = abGetTexIndex(265);
                pKr->px = texinfo[tex].px;
                pKr->py = texinfo[tex].py;
                pKr->pw = texinfo[tex].pw;
                pKr->ph = texinfo[tex].ph;
                tex0 = texinfo[tex].tex0;
                pKr->Tex0 = (tex0 | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);
                break;
            case 2:
                pKr->InRange = 30.0f;
                pKr->OutRange = 100.0f;
                pKr->TopSpeed = 5.0f;
                pKr->Speed = 0.2f;
                pKr->Alpha = 48;

                tex = abGetTexIndex(266);
                pKr->px = texinfo[tex].px;
                pKr->py = texinfo[tex].py;
                pKr->pw = texinfo[tex].pw;
                pKr->ph = texinfo[tex].ph;
                tex0 = texinfo[tex].tex0;
                pKr->Tex0 = (tex0 | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);
                break;
            case 3:
                pKr->InRange = 0.0f;
                pKr->OutRange = 50.0f;
                pKr->TopSpeed = 10.0f;
                pKr->Speed = 2.0f;
                pKr->Alpha = 96;

                tex = abGetTexIndex(267);
                pKr->px = texinfo[tex].px;
                pKr->py = texinfo[tex].py;
                pKr->pw = texinfo[tex].pw;
                pKr->ph = texinfo[tex].ph;
                break;
        }
    }

    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, pKr->GsEnv);
    sceDmaSync(pDma.Gif, 0, 0);

    sceVu0CopyVector(vf1, pKr->pObj->pMot->pBaseCoord->Mtx[3]);
    vf1[1] += -20.0f;
    for (i = 0; i < 24; i++) {
        f32 f;

        pKr->Cnt[i]--;
        f = _Sqrt((pKr->Pos[i][0] - vf1[0]) * (pKr->Pos[i][0] - vf1[0]) + (pKr->Pos[i][1] - vf1[1]) * (pKr->Pos[i][1] - vf1[1]) + (pKr->Pos[i][2] - vf1[2]) * (pKr->Pos[i][2] - vf1[2]));
        if (pKr->Cnt[i] < 0 || f > pKr->OutRange || f < pKr->InRange) {
            if (pKr->Cnt[i] < 0) {
                pKr->Cnt[i] = ((rand() >> 3) % 60) + 30;
                pKr->Mode[i] = pKr->TotalMode;
            }

            if (f > pKr->OutRange) {
                pKr->Xrot[i] += ((rand() >> 3) % 21 - 10) / 360.0f * M_TWOPI;
                pKr->Yrot[i] += ((rand() >> 3) % 21 - 10) / 360.0f * M_TWOPI;
                pKr->Zrot[i] += ((rand() >> 3) % 21 - 10) / 360.0f * M_TWOPI;
                if (pKr->Xrot[i] > (f64)M_PI) {
                    pKr->Xrot[i] -= M_TWOPI;
                }
                if (pKr->Xrot[i] < (f64)-M_PI) {
                    pKr->Xrot[i] += M_TWOPI;
                }
                if (pKr->Yrot[i] > (f64)M_PI) {
                    pKr->Yrot[i] -= M_TWOPI;
                }
                if (pKr->Yrot[i] < (f64)-M_PI) {
                    pKr->Yrot[i] += M_TWOPI;
                }
                if (pKr->Zrot[i] > (f64)M_PI) {
                    pKr->Zrot[i] -= M_TWOPI;
                }
                if (pKr->Zrot[i] < (f64)-M_PI) {
                    pKr->Zrot[i] += M_TWOPI;
                }
                pKr->Range[i] = pKr->InRange;
            } else if (f < pKr->InRange) {
                pKr->Xrot[i] += ((rand() >> 3) % 21 - 10) / 360.0f * M_TWOPI;
                pKr->Yrot[i] += ((rand() >> 3) % 21 - 10) / 360.0f * M_TWOPI;
                pKr->Zrot[i] += ((rand() >> 3) % 21 - 10) / 360.0f * M_TWOPI;

                if (pKr->Xrot[i] > (f64)M_PI) {
                    pKr->Xrot[i] -= M_TWOPI;
                }
                if (pKr->Xrot[i] < (f64)-M_PI) {
                    pKr->Xrot[i] += M_TWOPI;
                }
                if (pKr->Yrot[i] > (f64)M_PI) {
                    pKr->Yrot[i] -= M_TWOPI;
                }
                if (pKr->Yrot[i] < (f64)-M_PI) {
                    pKr->Yrot[i] += M_TWOPI;
                }
                if (pKr->Zrot[i] > (f64)M_PI) {
                    pKr->Zrot[i] -= M_TWOPI;
                }
                if (pKr->Zrot[i] < (f64)-M_PI) {
                    pKr->Zrot[i] += M_TWOPI;
                }
                pKr->Range[i] = pKr->OutRange;
            } else {
                pKr->Range[i] = ((rand() >> 3) % 100) / 100.0f * (pKr->OutRange - pKr->InRange) + pKr->InRange;
                pKr->Xrot[i] += ((rand() >> 3) % 61 - 30) / 360.0f * M_TWOPI;
                pKr->Yrot[i] += ((rand() >> 3) % 61 - 30) / 360.0f * M_TWOPI;
                pKr->Zrot[i] += ((rand() >> 3) % 61 - 30) / 360.0f * M_TWOPI;

                if (pKr->Xrot[i] > (f64)M_PI) {
                    pKr->Xrot[i] -= M_TWOPI;
                }
                if (pKr->Xrot[i] < (f64)-M_PI) {
                    pKr->Xrot[i] += M_TWOPI;
                }
                if (pKr->Yrot[i] > (f64)M_PI) {
                    pKr->Yrot[i] -= M_TWOPI;
                }
                if (pKr->Yrot[i] < (f64)-M_PI) {
                    pKr->Yrot[i] += M_TWOPI;
                }
                if (pKr->Zrot[i] > (f64)M_PI) {
                    pKr->Zrot[i] -= M_TWOPI;
                }
                if (pKr->Zrot[i] < (f64)-M_PI) {
                    pKr->Zrot[i] += M_TWOPI;
                }
            }

            sceVu0UnitMatrix(m);
            sceVu0RotMatrixX(m, m, pKr->Xrot[i]);
            sceVu0RotMatrixY(m, m, pKr->Yrot[i]);
            sceVu0RotMatrixZ(m, m, pKr->Zrot[i]);
            sceVu0TransMatrix(m, m, vf1);
            vf2[0] = pKr->Range[i];
            vf2[1] = 0.0f;
            vf2[2] = 0.0f;
            vf2[3] = 1.0f;
            sceVu0ApplyMatrix(pKr->TargetPos[i], m, vf2);
        }

        if (GameGbl.pause_flag == 0) {
            if (pKr->TargetPos[i][0] > pKr->Pos[i][0]) {
                pKr->Vector[i][0] += pKr->Speed;
            }
            if (pKr->Vector[i][0] > pKr->TopSpeed) {
                pKr->Vector[i][0] -= pKr->Speed;
            }
            if (pKr->TargetPos[i][0] < pKr->Pos[i][0]) {
                pKr->Vector[i][0] -= pKr->Speed;
            }
            if (pKr->Vector[i][0] < -pKr->TopSpeed) {
                pKr->Vector[i][0] += pKr->Speed;
            }

            if (pKr->TargetPos[i][1] > pKr->Pos[i][1]) {
                pKr->Vector[i][1] += pKr->Speed;
            }
            if (pKr->Vector[i][1] > pKr->TopSpeed) {
                pKr->Vector[i][1] -= pKr->Speed;
            }
            if (pKr->TargetPos[i][1] < pKr->Pos[i][1]) {
                pKr->Vector[i][1] -= pKr->Speed;
            }
            if (pKr->Vector[i][1] < -pKr->TopSpeed) {
                pKr->Vector[i][1] += pKr->Speed;
            }

            if (pKr->TargetPos[i][2] > pKr->Pos[i][2]) {
                pKr->Vector[i][2] += pKr->Speed;
            }
            if (pKr->Vector[i][2] > pKr->TopSpeed) {
                pKr->Vector[i][2] -= pKr->Speed;
            }
            if (pKr->TargetPos[i][2] < pKr->Pos[i][2]) {
                pKr->Vector[i][2] -= pKr->Speed;
            }
            if (pKr->Vector[i][2] < -pKr->TopSpeed) {
                pKr->Vector[i][2] += pKr->Speed;
            }

            pKr->Pos[i][0] += pKr->Vector[i][0];
            pKr->Pos[i][1] += pKr->Vector[i][1];
            pKr->Pos[i][2] += pKr->Vector[i][2];
            pKr->Pos[i][3] = 1.0f;
        }

        sceVu0UnitMatrix(m);
        sceVu0TransMatrix(m, m, pKr->Pos[i]);
        sceVu0MulMatrix(m, Scr.WvMtx, m);
        SETVEC(m[0], 1.0f, 0.0f, 0.0f, 0.0f);
        SETVEC(m[1], 0.0f, 1.0f, 0.0f, 0.0f);
        SETVEC(m[2], 0.0f, 0.0f, 1.0f, 0.0f);
        sceVu0MulMatrix(m, Scr.VsMtx, m);
        sceVu0RotTransPers(vi[0], m, vf3[0], 0);
        if (vi[0][2] < 0x1000000U && vi[0][0] >= 0x5800 && vi[0][0] <= 0xA800 && vi[0][1] >= 0x5800 && vi[0][1] <= 0xA800) {
            sceVu0RotTransPers(vi[1], m, vf3[1], 0);
            sceVu0RotTransPers(vi[2], m, vf3[2], 0);
            sceVu0RotTransPers(vi[3], m, vf3[3], 0);
            DrawKurosukeSub(vi, pKr->px, pKr->py, pKr->pw, pKr->ph, pKr->Tex0, pKr->Alpha);
        }
    }
}

static void DrawKurosukeSub(sceVu0IVECTOR *Xyz, s32 px, s32 py, s32 pw, s32 ph, u64 tex0, u8 alpha) {
    sceVu0IVECTOR uv[4];
    sceVu0IVECTOR rgba;
    qword_uni *spr = (qword_uni *)0x70000000;

    SETVEC(rgba, 0x40, 0x40, 0x40, alpha);
    SETVEC(uv[0], (px << 4) + 8, (py << 4) + 8, 0, 0);
    SETVEC(uv[1], (px << 4) + 8, ((py + ph) << 4) - 8, 0, 0);
    SETVEC(uv[2], ((px + pw) << 4) - 8, (py << 4) + 8, 0, 0);
    SETVEC(uv[3], ((px + pw) << 4) - 8, ((py + ph) << 4) - 8, 0, 0);

    spr->u_u128 = 0;
    spr->u_u32[0] = 0x70000000 | 11;
    ((qword_uni *)0x70000010)->u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 10);
    ((qword_uni *)0x70000010)->u_u64[1] = (u64)SCE_GIF_PACKED_AD | (u64)SCE_GS_RGBAQ << 4 \
        | (u64)SCE_GS_UV << 8 | (u64)SCE_GS_XYZF2 << 12 \
        | (u64)SCE_GS_UV << 16 | (u64)SCE_GS_XYZF2 << 20 \
        | (u64)SCE_GS_UV << 24 | (u64)SCE_GS_XYZF2 << 28 \
        | (u64)SCE_GS_UV << 32 | (u64)SCE_GS_XYZF2 << 36;
    ((qword_uni *)0x70000020)->u_u64[0] = tex0;
    ((qword_uni *)0x70000020)->u_u64[1] = SCE_GS_TEX0_2;
    ((qword_uni *)0x70000020)->u_u64[1] = SCE_GS_TEX0_2;
    ((qword_uni *)0x70000030)->u_u128 = ((qword_uni *)rgba)->u_u128;
    ((qword_uni *)0x70000040)->u_u128 = ((qword_uni *)uv[0])->u_u128;
    ((qword_uni *)0x70000050)->u_u128 = ((qword_uni *)Xyz[0])->u_u128;
    ((qword_uni *)0x70000060)->u_u128 = ((qword_uni *)uv[1])->u_u128;
    ((qword_uni *)0x70000070)->u_u128 = ((qword_uni *)Xyz[1])->u_u128;
    ((qword_uni *)0x70000080)->u_u128 = ((qword_uni *)uv[2])->u_u128;
    ((qword_uni *)0x70000090)->u_u128 = ((qword_uni *)Xyz[2])->u_u128;
    ((qword_uni *)0x700000A0)->u_u128 = ((qword_uni *)uv[3])->u_u128;
    ((qword_uni *)0x700000B0)->u_u128 = ((qword_uni *)Xyz[3])->u_u128;

    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(pDma.Gif, 0, 0);
}

static void EraseKurosuke(OBJWORK *pObjw) {
    pObjw->stat0 = 0;
    freemem((void *)pObjw->reg0);
}

void SetKurosukeMode(OBJWORK *pObjw, s32 mode) {
    ((KUROSUKE *)pObjw->reg0)->TotalMode = mode;
}