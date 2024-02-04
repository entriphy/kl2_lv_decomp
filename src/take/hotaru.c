#include "take/hotaru.h"
#include "take/object.h"
#include "take/motip.h"
#include "nakano/objfunc.h"
#include "okanoyo/okio.h"
#include "okanoyo/okmain.h"

static void DrawHotaru(OBJWORK *pObjw);
static void HotaruEnvInit(HOTARU *pHt);
static void DrawHotaruSub(sceVu0IVECTOR *Xyz, u64 tex0, s32 px, s32 py, s32 pw, s32 ph, u8 alpha);

OBJWORK* SetHotaruLight(SFXOBJ *pObj1, SFXOBJ *pObj2) {
    OBJWORK *objw = SearchOBJWORK(SysGbl.objwork, SysGbl.n_objw);
    HOTARU *pHt;
    sceVu0FVECTOR pad[6];

    InitObjWork(objw);
    objw->stat0 = 9;
    objw->dreff = DrawHotaru;
    pHt = (HOTARU *)getmem(0x2000);
    objw->reg0 = (s32)pHt;
    pHt->pObj1 = pObj1;
    pHt->pObj2 = pObj2;
    pHt->Mode = 0;
    pHt->TargetAlpha1 = 0.0f;
    pHt->TargetAlpha2 = 0.0f;
    HotaruEnvInit(pHt);
    return objw;
}

static void HotaruEnvInit(HOTARU *pHt) {
    s32 idx;
    s32 qword;
    u64 tex0;

    idx = abGetTexIndex(268);
    pHt->px = texinfo[idx].px;
    pHt->py = texinfo[idx].py;
    pHt->pw = texinfo[idx].pw;
    pHt->ph = texinfo[idx].ph;
    tex0 = texinfo[idx].tex0;
    pHt->tex0 = (tex0 | (1L << GS_TEX0_TCC_O)) & ~(3L << GS_TEX0_TFX_O);

    pHt->GsEnvInd = 2;
    pHt->GsEnv[pHt->GsEnvInd].u_u64[0] = SCE_GS_SET_CLAMP_2(0, 0, 0, 0, 0, 0);
    pHt->GsEnv[pHt->GsEnvInd].u_u64[1] = SCE_GS_CLAMP_2;
    pHt->GsEnvInd++;
    pHt->GsEnv[pHt->GsEnvInd].u_u64[0] = SCE_GS_SET_TEST_2(1, 6, 16, 1, 0, 0, 1, 2);
    pHt->GsEnv[pHt->GsEnvInd].u_u64[1] = SCE_GS_TEST_2;
    pHt->GsEnvInd++;
    pHt->GsEnv[pHt->GsEnvInd].u_u64[0] = SCE_GS_SET_TEX1_2(0, 0, 1, 0, 0, 0, 0);
    pHt->GsEnv[pHt->GsEnvInd].u_u64[1] = SCE_GS_TEX1_2;
    pHt->GsEnvInd++;
    pHt->GsEnv[pHt->GsEnvInd].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 2, 0, 1, 0x80);
    pHt->GsEnv[pHt->GsEnvInd].u_u64[1] = SCE_GS_ALPHA_2;
    pHt->GsEnvInd++;
    pHt->GsEnv[pHt->GsEnvInd].u_u64[0] = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMZ16S, 1);
    pHt->GsEnv[pHt->GsEnvInd].u_u64[1] = SCE_GS_ZBUF_2;
    qword = pHt->GsEnvInd;
    pHt->GsEnvInd++;

    pHt->GsEnv[0].u_u128 = 0;
    pHt->GsEnv[0].u_u32[0] = qword | 0x70000000;
    pHt->GsEnv[1].u_u64[0] = SCE_GIF_SET_TAG(qword - 1, 1, 0, 0, 0, 1);
    pHt->GsEnv[1].u_u64[1] = SCE_GIF_PACKED_AD;
}

static void DrawHotaru(OBJWORK *pObjw) {
    sceVu0IVECTOR vi[4];
    sceVu0FVECTOR vf0[4];
    sceVu0FVECTOR vf1;
    HOTARU *pHt = (HOTARU *)pObjw->reg0;
    s32 i;

    pHt->TopSpeed = 2.0f;

    switch (pHt->Mode) {
        case 0:
            for (i = 0; i < 32; i++) {
                SETVEC(pHt->Vector[i], 0.0f, 0.0f, 0.0f, 0.0f);
                pHt->OnFlag[i] = 0;
                sceVu0CopyVector(pHt->Pos[i], pHt->pObj1->pMot->pBaseCoord->Mtx[3]);
                sceVu0CopyVector(pHt->TargetPos[i], pHt->pObj1->pMot->pBaseCoord->Mtx[3]);
                pHt->Alpha[i] = 0;
            }
            pHt->OnFlag[0] = 1;
            if (GameGbl.pause_flag == 0) {
                if (pHt->TargetAlpha1 < 1.0f) {
                    pHt->TargetAlpha1 += 0.05f;
                }
                pHt->Cnt[0] = (pHt->Cnt[0] + 1) % 120;
            }
            pHt->Alpha[0] = pHt->TargetAlpha1 * (16.0f + __sin(pHt->Cnt[0] / 120.0f * (f64)M_PI) * 16.0f);
            pHt->Size[0] = 44.0f + __sin(pHt->Cnt[0] / 120.0f * (f64)M_PI) * 8.0f;
            pHt->Size[1] = 0.0f;
            pHt->Cnt[1] = 0;
            break;
        case 1:
            pHt->OnFlag[0] = 1;
            pHt->OnFlag[1] = 1;
            pHt->Alpha[1] = 0;
            pHt->TotalCnt = 0;
            pHt->TargetAlpha2 = 0.0f;

            for (i = 2; i < 32; i++) {
                SETVEC(pHt->Vector[i], 0.0f, 0.0f, 0.0f, 0.0f);
                pHt->OnFlag[i] = 1;
                pHt->TargetPos[i][0] = pHt->pObj2->pMot->pBaseCoord->Mtx[3][0] + (((rand() >> 3) % 129) - 72);
                pHt->TargetPos[i][1] = pHt->pObj2->pMot->pBaseCoord->Mtx[3][1] + (((rand() >> 3) % 129) - 72);
                pHt->TargetPos[i][2] = pHt->pObj2->pMot->pBaseCoord->Mtx[3][2] + (((rand() >> 3) % 129) - 72);
                pHt->Size[i] = 6.0f + (rand() % 6);
                pHt->Alpha[i] = 32;
            }

            pHt->TargetAlpha2 = 0.0f;
            pHt->Mode++;
        case 2:
            SETVEC(pHt->Vector[0], 0.0f, 0.0f, 0.0f, 0.0f);
            SETVEC(pHt->Vector[1], 0.0f, 0.0f, 0.0f, 0.0f);
            sceVu0CopyVector(pHt->Pos[1], pHt->pObj2->pMot->pCoord[15].Mtx[3]);

            if (GameGbl.pause_flag == 0) {
                if (pHt->TargetAlpha1 > 0.0f) {
                    pHt->TargetAlpha1 -= 0.05f;
                }
                pHt->Alpha[0] = pHt->TargetAlpha1 * (16.0f + __sin(pHt->Cnt[0] / 120.0f * (f64)M_PI) * 16.0f);
                if (pHt->TotalCnt > 60 && pHt->TargetAlpha2 < 1.0f) {
                    pHt->TargetAlpha2 += 0.02f;
                }
                pHt->TopSpeed = 6.0f;
                if (pHt->TotalCnt > 60) {
                    pHt->TopSpeed = 6.0f;
                }

                if (pHt->TotalCnt > 30) {
                    for (i = 2; i < 32; i++) {
                        pHt->OnFlag[i] = 1;
                        sceVu0CopyVector(pHt->TargetPos[i], pHt->pObj2->pMot->pCoord[15].Mtx[3]);
                        if (pHt->TargetPos[i][0] > pHt->Pos[i][0] - 32.0f && pHt->TargetPos[i][0] < pHt->Pos[i][0] + 32.0f &&
                            pHt->TargetPos[i][1] > pHt->Pos[i][1] - 32.0f && pHt->TargetPos[i][1] < pHt->Pos[i][1] + 32.0f &&
                            pHt->TargetPos[i][2] > pHt->Pos[i][2] - 32.0f && pHt->TargetPos[i][2] < pHt->Pos[i][2] + 32.0f) {
                            if (pHt->Alpha[i] != 0) {
                                pHt->Alpha[i] -= 4;
                            }
                            sceVu0ScaleVector(pHt->Vector[i], pHt->Vector[i], 0.66f);
                        }
                    }
                }

                if (pHt->TotalCnt > 90) {
                    for (i = 2; i < 32; i++) {
                        if (pHt->Alpha[i] != 0) {
                            pHt->Alpha[i] -= 2;
                        }
                    }
                }

                pHt->Alpha[1] = pHt->TargetAlpha2 * 16.0f;
                pHt->Size[1] = 44.0f + __sin(pHt->Cnt[1] / 120.0f * (f64)M_PI) * 8.0f;
                pHt->TotalCnt++;
            }

            if (pHt->TotalCnt > 110) {
                pHt->Mode++;
            }
            break;
        case 3:
            for (i = 0; i < 32; i++) {
                pHt->OnFlag[i] = 0;
            }
            pHt->OnFlag[1] = 1;
            SETVEC(pHt->Vector[1], 0.0f, 0.0f, 0.0f, 0.0f);
            sceVu0CopyVector(pHt->Pos[1], pHt->pObj2->pMot->pCoord[15].Mtx[3]);
            pHt->Alpha[1] = 16.0f + __sin(pHt->Cnt[1] / 120.0f * (f64)M_PI) * 16.0f;
            pHt->Size[1] = 44.0f + __sin(pHt->Cnt[1] / 120.0f * (f64)M_PI) * 8.0f;
            if (GameGbl.pause_flag == 0) {
                pHt->Cnt[1] = (pHt->Cnt[1] + 1) % 120;
            }
            break;
        case 4:
            for (i = 0; i < 32; i++) {
                pHt->OnFlag[i] = 0;
            }
            pHt->OnFlag[1] = 1;
            SETVEC(pHt->Vector[1], 0.0f, 0.0f, 0.0f, 0.0f);
            sceVu0CopyVector(pHt->Pos[1], pHt->pObj2->pMot->pCoord[15].Mtx[3]);
            pHt->Alpha[1] = pHt->TargetAlpha2 * (16.0f + __sin(pHt->Cnt[1] / 120.0f * (f64)M_PI) * 16.0f);
            pHt->Size[1] = 44.0f + __sin(pHt->Cnt[1] / 120.0f * (f64)M_PI) * 8.0f;
            pHt->Cnt[1] = (pHt->Cnt[1] + 1) % 120;
            if (GameGbl.pause_flag == 0) {
                if (pHt->TargetAlpha2 > 0.0f) {
                    pHt->TargetAlpha2 -= 0.02;
                } else {
                    pObjw->stat0 = 0;
                    freemem((void *)pObjw->reg0);
                    return;
                }
            }
            break;
    }

    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSync(pDma.Gif, 0, 0);
    sceDmaSend(pDma.Gif, pHt->GsEnv);
    sceDmaSync(pDma.Gif, 0, 0);

    for (i = 0; i < 32; i++) {
        if (pHt->OnFlag[i] != 0 && GameGbl.pause_flag == 0) {
            if (pHt->TargetPos[i][0] > pHt->Pos[i][0]) {
                pHt->Vector[i][0] += pHt->TopSpeed / 10.0f;
            }
            if (pHt->Vector[i][0] > pHt->TopSpeed) {
                pHt->Vector[i][0] -= pHt->TopSpeed / 10.0f;
            }
            if (pHt->TargetPos[i][0] < pHt->Pos[i][0]) {
                pHt->Vector[i][0] -= pHt->TopSpeed / 10.0f;
            }
            if (pHt->Vector[i][0] < -pHt->TopSpeed) {
                pHt->Vector[i][0] += pHt->TopSpeed / 10.0f;
            }

            if (pHt->TargetPos[i][1] > pHt->Pos[i][1]) {
                pHt->Vector[i][1] += pHt->TopSpeed / 10.0f;
            }
            if (pHt->Vector[i][1] > pHt->TopSpeed) {
                pHt->Vector[i][1] -= pHt->TopSpeed / 10.0f;
            }
            if (pHt->TargetPos[i][1] < pHt->Pos[i][1]) {
                pHt->Vector[i][1] -= pHt->TopSpeed / 10.0f;
            }
            if (pHt->Vector[i][1] < -pHt->TopSpeed) {
                pHt->Vector[i][1] += pHt->TopSpeed / 10.0f;
            }

            if (pHt->TargetPos[i][2] > pHt->Pos[i][2]) {
                pHt->Vector[i][2] += pHt->TopSpeed / 10.0f;
            }
            if (pHt->Vector[i][2] > pHt->TopSpeed) {
                pHt->Vector[i][2] -= pHt->TopSpeed / 10.0f;
            }
            if (pHt->TargetPos[i][2] < pHt->Pos[i][2]) {
                pHt->Vector[i][2] -= pHt->TopSpeed / 10.0f;
            }
            if (pHt->Vector[i][2] < -pHt->TopSpeed) {
                pHt->Vector[i][2] += pHt->TopSpeed / 10.0f;
            }

            pHt->Pos[i][0] += pHt->Vector[i][0];
            pHt->Pos[i][1] += pHt->Vector[i][1];
            pHt->Pos[i][2] += pHt->Vector[i][2];
        }
    }

    for (i = 0; i < 32; i++) {
        if (pHt->OnFlag[i] != 0) {
            sceVu0FMATRIX m;
            sceVu0UnitMatrix(m);
            sceVu0TransMatrix(m, m, pHt->Pos[i]);
            sceVu0MulMatrix(m, Scr.WvMtx, m);
            SETVEC(m[0], 1.0f, 0.0f, 0.0f, 0.0f);
            SETVEC(m[1], 0.0f, 1.0f, 0.0f, 0.0f);
            SETVEC(m[2], 0.0f, 0.0f, 1.0f, 0.0f);
            sceVu0MulMatrix(m, Scr.VsMtx, m);
            SETVEC(vf0[0], -pHt->Size[i], pHt->Size[i], 10.0f, 1.0f);
            SETVEC(vf0[1], -pHt->Size[i], -pHt->Size[i], 10.0f, 1.0f);
            SETVEC(vf0[2], pHt->Size[i], pHt->Size[i], 10.0f, 1.0f);
            SETVEC(vf0[3], pHt->Size[i], -pHt->Size[i], 10.0f, 1.0f);

            sceVu0RotTransPers(vi[0], m, vf0[0], 0);
            if (vi[0][2] < 0x1000000U && vi[0][0] >= 0x5800 && vi[0][0] <= 0xA800 && vi[0][1] >= 0x5800 && vi[0][1] <= 0xA800) {
                sceVu0RotTransPers(vi[1], m, vf0[1], 0);
                sceVu0RotTransPers(vi[2], m, vf0[2], 0);
                sceVu0RotTransPers(vi[3], m, vf0[3], 0);
                DrawHotaruSub(vi, pHt->tex0, pHt->px, pHt->py, pHt->pw, pHt->ph, pHt->Alpha[i]);
            }
        }
    }

    sceDmaSync(pDma.Gif, 0, 0);
}

static void DrawHotaruSub(sceVu0IVECTOR *Xyz, u64 tex0, s32 px, s32 py, s32 pw, s32 ph, u8 alpha) {
    sceVu0IVECTOR uv[4];
    sceVu0IVECTOR rgba;
    qword_uni *spr;

    sceDmaSync(pDma.Gif, 0, 0);

    spr = (qword_uni *)0x70000000;
    SETVEC(rgba, 0x60, 0x60, 0x80, alpha);
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

    sceDmaSend(pDma.Gif, (void *)(0x70000000 | 0x80000000));
    sceDmaSync(pDma.Gif, 0, 0);
}

void SetHotaruLightMode(OBJWORK *pObjw, s32 mode) {
    ((HOTARU *)pObjw->reg0)->Mode = mode;
}

void EraseHotaruLight(OBJWORK *pObjw) {
    pObjw->stat0 = 0;
    freemem((void *)pObjw->reg0);
}

void SetHotaruLightModeTest(OBJWORK *pObjw) {
    HOTARU *pHt = (HOTARU *)pObjw->reg0;
    pHt->Mode = (pHt->Mode + 1) % 5;
}