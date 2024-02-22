#include "take/object.h"
#include "take/sfxbios.h"
#include "take/motsys.h"
#include "harada/hr_tchr.h"

ACTTBL DfMot[128] = {}; // TODO: This needs to be initialized
static char OutLineStatusDef[64] = {};
static char OutLineStatusErase[64] = {
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4
};
u128 *SfxPacketBuff[2] = {};
s32 SpActive = 0;
DMAPTR pDma = {};
SCRENV Scr = {};
LIGHT3 Light3 = {};
char MemFlag[512] = {};
char *SfxDataPtr = NULL;
char *SfxWorkBuff = NULL;
sceVu0FMATRIX SfxLsMtx[64] = {};
sceVu0FMATRIX SfxLvMtx[64] = {};
sceVu0FMATRIX SfxLvSpecMtx[64] = {};
sceVu0FMATRIX SfxLcLightMtx[64] = {};
sceVu0FMATRIX SfxSkinMtx[64] = {};
sceVu0IVECTOR SfxVertexI[2048] = {};
sceVu0IVECTOR SfxColorI[2048] = {};
sceVu0IVECTOR SfxSpecUVI[2048] = {};
GIMINFO SfxTexInfo[64] = {};
u32 SfxTexInfoIndex = 0;
qword_uni SpecGsEnv[64] = {};
s32 SpecGsEnvInd = 0;
qword_uni *SpecGsEnvAlpha = NULL; // ?
kitOutlineDrawEnv OutlineEnv = {};
sceVu0IVECTOR bboxTmp[2] = {};
u32 *DataBuffPtr = NULL;
u32 SfxFrame = 0;
u32 SfxInter = 0;
f32 ActCntSpd = 0.0f;
f32 *pSfxClip = NULL;
s32 SfxAllPause = 0;
s32 SfxDebugCnt = 0;

void ModelDraw(SFXOBJ *pObj) {
    SFXOBJ *pObjTmp;
    u32 count;
    s16 Condition;
    MOTION *pMot;
    MIME *pMime;

    count = *T0_COUNT;
    Condition = pObj->Condition;
    pMot = NULL;
    pMime = NULL;

    pObj->OutFlag = 0;
    if (pObj->SvxAdrs != NULL) {
        if (pObj->ClipFlag != 0 && SfxObjBallClipCheck(pObj) != 0 && pObj->MotionSyncFlag == 0) {
            pObj->OutFlag = 1;
        } else {
            GetMotion(pObj);
            hr_draw_tkVU1(pObj->SvxWorkAdrs, pObj->pMot->CoordNum, SfxLsMtx, SfxLcLightMtx, pObj->pLightColor);
        }
    } else {
        float f = (*pObj->pLightColor)[3][0] + (*pObj->pLightColor)[3][1] + (*pObj->pLightColor)[3][2];
        if (f > 0.8) {
            f = 0.8f;
        }
        f = (f * 127.0f) / 0.8;
        SpecGsEnvAlpha->u_u64[0] = SCE_GS_SET_ALPHA_2(0, 2, 2, 1, (s32)f & 0xFF);
        if (pObj->pMot->CoordNum != 0) {
            pObjTmp = pObj;

            while (pObjTmp != NULL) {
                if ((Condition & pObjTmp->Flag) == 0) {
                    pObjTmp = pObjTmp->pObjSub;
                } else {
                    if (pObjTmp->ClipFlag != 0 && SfxObjBallClipCheck(pObjTmp) != 0 && pObj->MotionSyncFlag == 0) {
                        pObj->OutFlag = 1;
                    } else {
                        SfxDebugCnt += 1;

                        if (pMot != pObjTmp->pMot) {
                            pMot = pObjTmp->pMot;
                            GetMotion(pObjTmp);
                        }

                        if (pMime != pObjTmp->pMime) {
                            pMime = pObjTmp->pMime;
                            MimeWork(pObj);
                        }

                        if (pObjTmp->GmsTransType == 1) {
                            if (*(u32 *)pObjTmp->GmsAdrs < 2) {
                                sceDmaSync(pDma.Gif, 0, 0);
                                sceDmaSend(pDma.Gif, Fadr((u32 *)pObjTmp->GmsAdrs, 0));
                            } else {
                                sceDmaSync(pDma.Gif, 0, 0);
                                sceDmaSend(pDma.Gif, Fadr((u32 *)pObjTmp->GmsAdrs, 0));
                                SetSyncTex(pObjTmp);
                                if ((*Fadr((u32 *)pObjTmp->GmsAdrs, 1)) != 0 && pObj->MotionSyncFlag != 1) {
                                    sceDmaSync(pDma.Gif, 0, 0);
                                    sceDmaSend(pDma.Gif, Fadr((u32 *)pObjTmp->GmsAdrs, 1));
                                    MixClut(pObjTmp->Cbp, pObjTmp->ClutNum0, pObjTmp->ClutNum1, pObjTmp->ClutWeight);
                                }
                            }
                        }

                        SfxDrawOutLine(pObjTmp);
                    }

                    pObjTmp = pObjTmp->pObjSub;
                }
            }
        } else {
            if (pObj->GmsTransType == 1) {
                sceDmaSync(pDma.Gif, 0, 0);
                sceDmaSend(pDma.Gif, Fadr((u32 *)pObj->GmsAdrs, 0));
            }
            GetSimpleSfxMatrix(pObj);
            SfxDrawOutLine(pObj);
        }
        pObj->DrawWorkCnt = *T0_COUNT - count;
        func_00205E38(pObj);
    }
}

s32 GetSfxOutFlag(SFXOBJ *pObj) {
    return pObj->OutFlag;
}

SFXOBJ* GetSfxObjectData(s32 ChrNum) {
    SFXOBJ *pObj;
    s32 ShType;
    f32 ShSize;
    f32 ShOffset;
    f32 ShRange;
    f32 *Sh; // ?

    if (Fadr((u32 *)SfxDataPtr, ChrNum) == Fadr((u32 *)SfxDataPtr, ChrNum + 1)) {
        ChrNum = 820;
    }

    pObj = SetSfxObject(Fadr((u32 *)SfxDataPtr, ChrNum));
    pObj->ObjNum = ChrNum;

    Sh = (f32 *)Fadr((u32 *)SfxDataPtr, 936);
    ShType = ((s32 *)Sh + ChrNum * 6)[2];
    ShSize = (Sh + ChrNum * 6)[3];
    ShOffset = (Sh + ChrNum * 6)[4];
    ShRange = (Sh + ChrNum * 6)[5];
    SetObjClipZone(pObj, (Sh + ChrNum * 6)[0], (Sh + ChrNum * 6)[1]);
    SetObjShadow(pObj, ShType, ShSize, ShOffset, ShRange);

    return pObj;
}

void SetSfxData(char *pSfxData) {
    SfxDataPtr = pSfxData;
}

s32 SetSfxWork(u32 *DataBuff, u32 *SfxAdrs) {
    // TODO
}

s32 SetSfxAct(u32 *DataBuff, SFXOBJ *pObj, u32 *ActAdrs) {
    s32 *TmpDataBuff;
    s32 CoordBuffSize;
    MOTION *m;

    if (*ActAdrs == 0) {
        pObj->pMot = pObj->pObjTop->pMot;
        return 0;
    }

    TmpDataBuff = (s32 *)ALIGN(DataBuff);
    pObj->pMot = (MOTION *)TmpDataBuff;
    m = pObj->pMot;
    TmpDataBuff = (s32 *)ALIGN_ALT(TmpDataBuff, sizeof(MOTION));
    m->ActAdrs = (u8 *)ActAdrs;
    m->ActNum = 1;
    m->ActNumMax = *(u16 *)ActAdrs;
    InitSfxCoord(pObj->pMot, (u8 *)Fadr(ActAdrs, 0), (tagCOORD *)TmpDataBuff);
    m->pActtbl = DfMot;
    m->pBaseCoord = pObj->pObjTop->pMot->pBaseCoord;
    CoordBuffSize = sizeof(tagCOORD);
    TmpDataBuff = (s32 *)((u32)TmpDataBuff + CoordBuffSize * (m->CoordNum + 1));
    TmpDataBuff = (s32 *)ALIGN(TmpDataBuff);
    m->pBaseCoord->Trans[0] = 0.0f;
    m->pBaseCoord->Trans[1] = 0.0f;
    m->pBaseCoord->Trans[2] = 0.0f;
    m->pBaseCoord->Trans[3] = 1.0f;
    m->pBaseCoord->Rot[0] = 0.0f;
    m->pBaseCoord->Rot[1] = 0.0f;
    m->pBaseCoord->Rot[2] = 0.0f;
    m->pBaseCoord->Rot[3] = 0.0f;
    SetActSub(m, m->ActNum);
    return (s32)((u32)TmpDataBuff - (u32)DataBuff);
}

void PartsEnvInit(SFXOBJ *pObj) {
    s32 i;

    for (i = 0; i < pObj->PartsNum; i++) {
        pObj->pParts[i].GsEnvInd = 1;

        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[0] = *(u64 *)&pObj->pParts[i].gs_tex0;
        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[1] = SCE_GS_TEX0_2;
        pObj->pParts[i].GsEnvInd++;

        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[0] = SCE_GS_SET_TEX1_2(0, 0, 3, 0, 0, 0, 0);
        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[1] = SCE_GS_TEX1_2;
        pObj->pParts[i].GsEnvInd++;

        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[0] = SCE_GS_SET_TEST_2(1, 1, 0, 0, 0, 0, 1, 2);
        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[1] = SCE_GS_TEST_2;
        pObj->pParts[i].GsEnvInd++;

        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 1, 2, 1, 127);
        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[1] = SCE_GS_ALPHA_2;
        pObj->pParts[i].GsEnvInd++;

        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[0] = SCE_GS_SET_ZBUF_2(140, SCE_GS_PSMZ16S, 0);
        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[1] = SCE_GS_ZBUF_2;
        pObj->pParts[i].GsEnvInd++;

        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[0] = SCE_GS_SET_PABE(0);
        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[1] = SCE_GS_PABE;
        pObj->pParts[i].GsEnvInd++;

        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[0] = SCE_GS_SET_FBA_2(1);
        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[1] = SCE_GS_FBA_2;
        pObj->pParts[i].GsEnvInd++;

        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[0] = SCE_GS_SET_CLAMP_2(0, 0, 0, 0, 0, 0);
        pObj->pParts[i].GsEnv[pObj->pParts[i].GsEnvInd].u_u64[1] = SCE_GS_CLAMP_2;
        pObj->pParts[i].GsEnvInd++;

        pObj->pParts[i].GsEnv[0].u_u64[0] = SCE_GIF_SET_TAG(pObj->pParts[i].GsEnvInd - 1, 0, 0, 0, 0, 1);
        pObj->pParts[i].GsEnv[0].u_u64[1] = SCE_GIF_PACKED_AD;
    }
}

void SetObjSubScale(SFXOBJ *pObj, f32 Scale) {
    // Empty function
}

void SetObjScale(SFXOBJ *pObj, sceVu0FVECTOR ScaleVector) {
    SFXOBJ *pObjTmp;

    for (pObjTmp = pObj; pObjTmp != NULL; pObjTmp = pObjTmp->pObjSub) {
        *(u128 *)pObjTmp->ScaleVector = *(u128 *)ScaleVector;
    }
}

void SetObjGmsTransType(SFXOBJ *pObj, s32 type) {
    SFXOBJ *pObjTmp;

    for (pObjTmp = pObj; pObjTmp != NULL; pObjTmp = pObjTmp->pObjSub) {
        pObjTmp->GmsTransType = type;
    }
}

void SetObjClipFlag(SFXOBJ *pObj, s32 flag) {
    SFXOBJ *pObjTmp;

    for (pObjTmp = pObj; pObjTmp != NULL; pObjTmp = pObjTmp->pObjSub) {
        pObjTmp->ClipFlag = flag;
    }
}

void GetObjMatrixTrans(SFXOBJ *pObj, s32 num, sceVu0FVECTOR DistVector) {
    sceVu0FMATRIX ScaleMtx;
    sceVu0FMATRIX TmpMtx;
    sceVu0FVECTOR TmpVec;

    GetScaleMtx(ScaleMtx, pObj->ScaleVector);
    sceVu0InversMatrix(TmpMtx, pObj->pMot->pBaseCoord->Mtx);
    sceVu0MulMatrix(ScaleMtx, ScaleMtx, TmpMtx);
    sceVu0MulMatrix(ScaleMtx, pObj->pMot->pBaseCoord->Mtx, ScaleMtx);
    sceVu0ApplyMatrix(TmpVec, ScaleMtx, pObj->pMot->pCoord[num].Mtx[3]);
    DistVector[0] = TmpVec[0];
    DistVector[1] = TmpVec[1];
    DistVector[2] = TmpVec[2];
    DistVector[3] = 1.0f;
}

void SetObjNormalLight(SFXOBJ *pObj, sceVu0FMATRIX *NormalLight) {
    SFXOBJ *pObjTmp;

    pObjTmp = pObj;
    do {
        pObjTmp->pNormalLight = NormalLight;
        pObjTmp = pObjTmp->pObjSub;
    } while (pObjTmp != NULL);
}

void SetObjOutlineOff(SFXOBJ *pObj) {
    SFXOBJ *pObjSub;
    s32 i;

    for (pObjSub = pObj; pObjSub != NULL; pObjSub = pObjSub->pObjSub) {
        for (i = 0; i < pObjSub->PartsNum; i++) {
            pObjSub->pParts[i].OutLine = 4;
        }
        pObjSub->LineEnv.status = (u8 *)OutLineStatusErase;
    }
}

void SetObjEffDraw(SFXOBJ *pObj) {
    SFXOBJ *pObjSub;

    for (pObjSub = pObj; pObjSub != NULL; pObjSub = pObjSub->pObjSub) {
        pObjSub->OutLineFlag = 0;
    }
}

void SetObjLightColor(SFXOBJ *pObj, sceVu0FMATRIX *LightColor) {
    SFXOBJ *pObjTmp;

    pObjTmp = pObj;
    do {
        pObjTmp->pLightColor = LightColor;
        pObjTmp = pObjTmp->pObjSub;
    } while (pObjTmp != NULL);
}

void OutLineEnvInit(u32 frame, u32 inter) {
    u128 *pPacket;

    if (kzCheckSRoll() == 0) {
        SfxDebugCnt = 0;
        SfxFrame = frame;
        SfxInter = inter;
        pPacket = (u128 *)0x70002200;
        PacketKickPath3((u128 *)0x70002200, (kitOutlineDrawEnv *)((OutLineAllInit(pPacket, &OutlineEnv, frame, inter) - (u32)pPacket) >> 4));
        sceDmaSync(pDma.Gif, 0, 0);
    }
}

void LinkActTbl(MOTION *m, ACTTBL *pActtbl) {
    m->pActtbl = pActtbl;
}

void SetObjCondition(SFXOBJ *pObj, s16 Condition) {
    pObj->Condition = Condition;
}

void SetObjPause(SFXOBJ *pObj, u32 flag) {
    pObj->Pause = flag;
}

SFXOBJ* GetActiveSfx(SFXOBJ *pObj) {
    SFXOBJ *pObjTmp;

    for (pObjTmp = pObj->pObjTop; pObjTmp != NULL; pObjTmp = pObjTmp->pObjSub) {
        if (pObjTmp->Flag == (pObjTmp->Flag & pObjTmp->pObjTop->Condition)) {
            return pObjTmp;
        }
    }

    return NULL;
}

SFXOBJ* GetFlagObjPtr(SFXOBJ *pObj, u32 Condition) {
    if (pObj->Flag != (Condition & pObj->Flag)) {
        return GetFlagObjPtr(pObj->pObjSub, Condition);
    } else {
        return pObj;
    }
}

void EraseSfxObject(SFXOBJ *pObj) {
    if (pObj != NULL) {
        OpenMem((u32 *)pObj);
    }
}

void MixClut(u16 Cbp, s32 Num0, s32 Num1, f32 Weight) {
    // TODO
}

void SetSfxVariationClut(SFXOBJ *pObj, u32 ClutNum0, u32 ClutNum1, f32 Weight) {
    SFXOBJ *pObjTmp;

    for (pObjTmp = pObj; pObjTmp != NULL; pObjTmp = pObjTmp->pObjSub) {
        pObjTmp->ClutNum0 = ClutNum0;
        pObjTmp->ClutNum1 = ClutNum1;
        pObjTmp->ClutWeight = Weight;
    }
}

void SetSfxVariationGms(SFXOBJ *pObj, u32 GmsNum) {
    SFXOBJ *pObjTmp;

    for (pObjTmp = pObj; pObjTmp != NULL; pObjTmp = pObjTmp->pObjSub) {
        pObjTmp->GmsNum = GmsNum;
    }
}

s32 SetSyncTex(SFXOBJ *pObj) {
    s32 ii;
    u8 *pTex;
    ACT_HEADER *pActHeader;
    s16 TexNum;
    s32 MotionCnt;
    MOTION *m;

    if (pObj->GmsNum != 0) {
        sceDmaSync(pDma.Gif, 0, 0);
        sceDmaSend(pDma.Gif, Fadr(pObj->GmsAdrs, pObj->GmsNum + 1));
        pObj->GmsNum = 0;
    } else {
        m = pObj->pMot;
        MotionCnt = (u32)m->Mb[m->BaseIndex].MotionCnt;
        pActHeader = (ACT_HEADER *)m->Mb[m->BaseIndex].pAct;
        if (pActHeader->TexAddrs != NULL) {
            pTex = (u8 *)pActHeader + pActHeader->TexAddrs;
            TexNum = *pTex;
            pTex = pTex + (MotionCnt * TexNum + 1);
            for (ii = 0; ii < TexNum; ii++) {
                s32 yeet = *pTex++; // ?
                if (yeet > 0) {
                    sceDmaSync(pDma.Gif, 0, 0);
                    sceDmaSend(pDma.Gif, Fadr(pObj->GmsAdrs, yeet + 1));
                }
            }
        }
    }
}

void SetSfxMotionSync(SFXOBJ *pObj0, SFXOBJ *pObj1) {
    SFXOBJ *pObjTmp0;
    SFXOBJ *pObjTmp1;

    if (pObj0->pMot->CoordNum == pObj1->pMot->CoordNum && pObj0->PartsNum == pObj1->PartsNum) {
        for (pObjTmp0 = pObj0, pObjTmp1 = pObj1; pObjTmp0 != NULL; pObjTmp0 = pObjTmp0->pObjSub, pObjTmp1 = pObjTmp1->pObjSub) {
            pObjTmp0->MotionSyncFlag = 1;
            pObjTmp1->MotionSyncFlag = 2;
            pObjTmp0->pMot = pObjTmp1->pMot;
        }
    }
}

s32 SetSfxActSimple(u32 *DataBuff, SFXOBJ *pObj) {
    s32 *TmpDataBuff;
    MOTION *m; // ?

    TmpDataBuff = (s32 *)ALIGN(DataBuff);
    m = (MOTION *)TmpDataBuff;
    pObj->pMot = m;
    TmpDataBuff = (s32 *)ALIGN_ALT(TmpDataBuff, sizeof(MOTION));
    m->CoordNum = 0;
    m->pBaseCoord = (tagCOORD *)TmpDataBuff;
    TmpDataBuff = (s32 *)((u32)TmpDataBuff + sizeof(tagCOORD));
    m->pBaseCoord->Trans[0] = 0.0f;
    m->pBaseCoord->Trans[1] = 0.0f;
    m->pBaseCoord->Trans[2] = 0.0f;
    m->pBaseCoord->Trans[3] = 1.0f;
    m->pBaseCoord->Rot[0] = 0.0f;
    m->pBaseCoord->Rot[1] = 0.0f;
    m->pBaseCoord->Rot[2] = 0.0f;
    m->pBaseCoord->Rot[3] = 0.0f;

    return (s32)((u32)TmpDataBuff - (u32)DataBuff);
}



