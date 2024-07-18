#include "take/object.h"
#include "take/effsub.h"
#include "take/mimesys.h"
#include "take/sfxbios.h"
#include "take/outline.h"
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
            hr_draw_tkVU1((HRVU1OBJ *)pObj->SvxWorkAdrs, pObj->pMot->CoordNum, SfxLsMtx, SfxLcLightMtx, pObj->pLightColor);
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
	s32 i;
	SFXOBJ *pObj;
	char *TmpDataBuff;
	TYPE_SFX_HEADER *pSfx;
	s32 PartsBuffSize;
	TYPE_PARTS_TBL *pParts_Skin;
	TYPE_PARTS_TBL2 *pParts_Fix;
	sceGsTex0 MinTex0;
    
    pObj = (SFXOBJ *)DataBuff;
    pSfx = (TYPE_SFX_HEADER *)SfxAdrs;
    TmpDataBuff = (char *)ALIGN((u32)DataBuff + sizeof(SFXOBJ));
    pObj->pParts = (PARTS *)TmpDataBuff;
    pObj->PartsNum = pSfx->parts_num;
    pObj->scale = pSfx->scale;
    pObj->ClipFlag = 1;
    pObj->ScaleVector[0] = 1.0f;
    pObj->ScaleVector[1] = 1.0f;
    pObj->ScaleVector[2] = 1.0f;
    pObj->ScaleVector[3] = 1.0f;
    pObj->pLightColor = &Light3.LightColor;
    pObj->pNormalLight = &Light3.NormalLight;
    pObj->LineEnv.partsmax = pObj->PartsNum - 1;
    pObj->OutLineFlag = 1;
    pObj->GmsTransType = 1;
    pParts_Skin = (TYPE_PARTS_TBL *)(pSfx + 1);
    pParts_Fix = (TYPE_PARTS_TBL2 *)pParts_Skin;
    MinTex0.CBP = 0x3FFF;

    for (i = 0; i < pObj->PartsNum; i++, pParts_Skin++, pParts_Fix++) {
        switch (pParts_Skin->type) {
            case 0:
                pObj->pParts[i].type = pParts_Fix->type;
                pObj->pParts[i].vert_num = pParts_Fix->vert_num;
                pObj->pParts[i].norm_num = pParts_Fix->norm_num;
                pObj->pParts[i].uv_num = pParts_Fix->uv_num;
                pObj->pParts[i].prim_num = pParts_Fix->strip_num;
                pObj->pParts[i].sfx_adrs = (s32 *)pSfx;
                pObj->pParts[i].uv_adrs = (s32 *)((u32)pSfx + (u32)pParts_Fix->uv_adrs);
                pObj->pParts[i].prim_adrs = (s32 *)((u32)pSfx + (u32)pParts_Fix->strip_adrs);
                pObj->pParts[i].vert_adrs = (s32 *)((u32)pSfx + (u32)pParts_Fix->vert_adrs);
                pObj->pParts[i].norm_adrs = (s32 *)((u32)pSfx + (u32)pParts_Fix->norm_adrs);
                pObj->pParts[i].gs_tex0 = pParts_Fix->gs_tex0;
                break;
            case 1:
            case 3:
                pObj->pParts[i].type = pParts_Skin->type;
                pObj->pParts[i].jblock_num = pParts_Skin->jblock_num;
                pObj->pParts[i].vert_num = pParts_Skin->vert_num;
                pObj->pParts[i].norm_num = pParts_Skin->norm_num;
                pObj->pParts[i].uv_num = pParts_Skin->uv_num;
                pObj->pParts[i].prim_num = pParts_Skin->strip_num;
                pObj->pParts[i].sfx_adrs = (s32 *)pSfx;
                pObj->pParts[i].jblock_adrs = (s32 *)((u32)pSfx + (u32)pParts_Skin->jblock_adrs);
                pObj->pParts[i].uv_adrs = (s32 *)((u32)pSfx + (u32)pParts_Skin->uv_adrs);
                pObj->pParts[i].prim_adrs = (s32 *)((u32)pSfx + (u32)pParts_Skin->strip_adrs);
                pObj->pParts[i].vert_adrs = (s32 *)((u32)pSfx + (u32)pParts_Skin->vert_adrs);
                pObj->pParts[i].norm_adrs = (s32 *)((u32)pSfx + (u32)pParts_Skin->norm_adrs);
                pObj->pParts[i].gs_tex0 = pParts_Skin->gs_tex0;
                break;
            default:
                break;
        }

        pObj->pParts[i].coord_id = 0;
        pObj->pParts[i].GmsAdr = NULL;
        {
            PARTS *part = &pObj->pParts[i];
            if (MinTex0.CBP > part->gs_tex0.CBP) {
                MinTex0.CBP = part->gs_tex0.CBP;
            }
        }
        
    }

    pObj->Cbp = MinTex0.CBP;
    pObj->ClutNum0 = 0;
    pObj->ClutNum1 = 0;
    pObj->ClutWeight = 0.0f;
    pObj->MotionSyncFlag = 0;
    pObj->SvxAdrs = NULL;
    pObj->SvxWorkAdrs = NULL;
    pObj->GmsNum = 0;
    PartsEnvInit(pObj);

    {
        u32 var_a0 = ~0xF;
        u32 var_v1 = sizeof(PARTS);
        TmpDataBuff = (char *)((u32)TmpDataBuff + pObj->PartsNum * var_v1);
        TmpDataBuff = (char *)(((u32)TmpDataBuff + 0xF) & var_a0);
    }
    
    return (s32)((u32)TmpDataBuff - (u32)DataBuff);
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
    u128 *pOutline;

    if (kzCheckSRoll()) {
        return;
    }

    SfxDebugCnt = 0;
    SfxFrame = frame;
    SfxInter = inter;
    pPacket = SPR_MEM_IDX(0x220);
    pOutline = OutlineAllInit(SPR_MEM_IDX(0x220), &OutlineEnv, frame, inter);
    PacketKickPath3((u32)pPacket, ((size_t)pOutline - (size_t)pPacket) / 16);
    sceDmaSync(pDma.Gif, 0, 0);
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

// Not matching: https://decomp.me/scratch/EBEzb
void MixClut(u16 Cbp, s32 Num0, s32 Num1, f32 Weight) {
	qword_uni PacketBuff[64]; // type_override: u128
	s32 Alpha0 = 128;
	s32 Alpha1 = Weight * 128.0f;
	qword_uni RGBA[4];
	qword_uni UV[4];
	qword_uni XYZ[4];
    u32 idx = 2;
    
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_FRAME_2(Cbp >> 5, 10, 0, 0);
    PacketBuff[idx++].u_u64[1] = SCE_GS_FRAME_2;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_XYOFFSET_2(0, 0);
    PacketBuff[idx++].u_u64[1] = SCE_GS_XYOFFSET_2;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_SCISSOR_2(0x0000, 0x0FFF, 0x0000, 0x0FFF);
    PacketBuff[idx++].u_u64[1] = SCE_GS_SCISSOR_2;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_TEX0_2(Cbp + 0x20, 1, SCE_GS_PSMCT32, 9, 5, 1, 0, 0, SCE_GS_PSMCT32, 0, 0, 1);
    PacketBuff[idx++].u_u64[1] = SCE_GS_TEX0_2;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_TEX1_2(0, 0, 0, 0, 0, 0, 0);
    PacketBuff[idx++].u_u64[1] = SCE_GS_TEX1_2;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_TEST_2(1, 1, 0, 0, 0, 0, 1, 1);
    PacketBuff[idx++].u_u64[1] = SCE_GS_TEST_2;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 2, 2, 2, 128);
    PacketBuff[idx++].u_u64[1] = SCE_GS_ALPHA_2;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_TEXA(64, 0, 64);
    PacketBuff[idx++].u_u64[1] = SCE_GS_TEXA;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_FBA_2(0);
    PacketBuff[idx++].u_u64[1] = SCE_GS_FBA_2;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_PABE(0);
    PacketBuff[idx++].u_u64[1] = SCE_GS_PABE;
    PacketBuff[idx].u_u64[0] = SCE_GS_SET_ZBUF_2(0x8C, SCE_GS_PSMZ16S, 1);
    PacketBuff[idx++].u_u64[1] = SCE_GS_ZBUF_2;

    pDma.Gif->chcr.TTE = 0;
    SCE_GIF_CLEAR_TAG(PacketBuff);
    PacketBuff[0].u_u32[0] = 0x70000000 | 12;
    PacketBuff[1].u_u64[0] = SCE_GIF_SET_TAG(11, 1, 0, 0, 0, 1);
    PacketBuff[1].u_u64[1] = SCE_GIF_PACKED_AD;
    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, PacketBuff);
    sceDmaSync(pDma.Gif, 0, 0);

    SETRGBA(RGBA[0], Alpha0, Alpha0, Alpha0, Alpha0);
    SETRGBA(RGBA[1], Alpha0, Alpha0, Alpha0, Alpha0);
    SETRGBA(RGBA[2], Alpha0, Alpha0, Alpha0, Alpha0);
    SETRGBA(RGBA[3], Alpha0, Alpha0, Alpha0, Alpha0);

    SETUV(UV[0], Num0 * 0x400 + 0x008,       0x000 + 0x008);
    SETUV(UV[1], (Num0 + 1) * 0x400 + 0x008, 0x000 + 0x008);
    SETUV(UV[2], Num0 * 0x400 + 0x008,       0x200 + 0x008);
    SETUV(UV[3], (Num0 + 1) * 0x400 + 0x008, 0x200 + 0x008);

    SETXYZ(XYZ[0], 0x000, 0x000, 0x000);
    SETXYZ(XYZ[1], 0x400, 0x000, 0x000);
    SETXYZ(XYZ[2], 0x000, 0x200, 0x000);
    SETXYZ(XYZ[3], 0x400, 0x200, 0x000);

    SCE_GIF_CLEAR_TAG(PacketBuff);
    PacketBuff[0].u_u32[0] = 0x70000000 | 13;
    PacketBuff[1].u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 12);
    PacketBuff[1].u_u64[1] = \
          (u64)SCE_GS_RGBAQ       | (u64)SCE_GS_UV << 4  | (u64)SCE_GS_XYZ2 << 8  \
        | (u64)SCE_GS_RGBAQ << 12 | (u64)SCE_GS_UV << 16 | (u64)SCE_GS_XYZ2 << 20 \
        | (u64)SCE_GS_RGBAQ << 24 | (u64)SCE_GS_UV << 28 | (u64)SCE_GS_XYZ2 << 32 \
        | (u64)SCE_GS_RGBAQ << 36 | (u64)SCE_GS_UV << 40 | (u64)SCE_GS_XYZ2 << 44;
    PacketBuff[2].u_u128 = RGBA[0].u_u128;
    PacketBuff[3].u_u128 = UV[0].u_u128;
    PacketBuff[4].u_u128 = XYZ[0].u_u128;
    PacketBuff[5].u_u128 = RGBA[1].u_u128;
    PacketBuff[6].u_u128 = UV[1].u_u128;
    PacketBuff[7].u_u128 = XYZ[1].u_u128;
    PacketBuff[8].u_u128 = RGBA[2].u_u128;
    PacketBuff[9].u_u128 = UV[2].u_u128;
    PacketBuff[10].u_u128 = XYZ[2].u_u128;
    PacketBuff[11].u_u128 = RGBA[3].u_u128;
    PacketBuff[12].u_u128 = UV[3].u_u128;
    PacketBuff[13].u_u128 = XYZ[3].u_u128;
    FlushCache(0);
    pDma.Gif->chcr.TTE = 0;
    sceDmaSend(pDma.Gif, PacketBuff);
    sceDmaSync(pDma.Gif, 0, 0);
    
    if (Weight == 0.0f) {
        SetDefaultDrawEnv();
    } else {
        PacketBuff[2].u_u64[0] = SCE_GS_SET_ALPHA_2(0, 1, 2, 1, Alpha1);
        PacketBuff[2].u_u64[1] = SCE_GS_ALPHA_2;

        pDma.Gif->chcr.TTE = 0;
        SCE_GIF_CLEAR_TAG(PacketBuff);
        PacketBuff[0].u_u32[0] = 0x70000000 | 2;
        PacketBuff[1].u_u64[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
        PacketBuff[1].u_u64[1] = SCE_GIF_PACKED_AD;
        FlushCache(0);
        pDma.Gif->chcr.TTE = 0;
        sceDmaSend(pDma.Gif, PacketBuff);
        sceDmaSync(pDma.Gif, 0, 0);

        SETRGBA(RGBA[0], 128, 128, 128, 128);
        SETRGBA(RGBA[1], 128, 128, 128, 128);
        SETRGBA(RGBA[2], 128, 128, 128, 128);
        SETRGBA(RGBA[3], 128, 128, 128, 128);

        SETUV(UV[0], (Num1 << 10) + 0x008, 0x008);
        SETUV(UV[1], ((Num1 + 1) << 10) + 0x008, 0x008);
        SETUV(UV[2], (Num1 << 10) + 0x008, 0x208);
        SETUV(UV[3], ((Num1 + 1) << 10) + 0x008, 0x208);
    
        SETXYZ(XYZ[0], 0x000, 0x000, 0x000);
        SETXYZ(XYZ[1], 0x400, 0x000, 0x000);
        SETXYZ(XYZ[2], 0x000, 0x200, 0x000);
        SETXYZ(XYZ[3], 0x400, 0x200, 0x000);

        SCE_GIF_CLEAR_TAG(PacketBuff);
        PacketBuff[0].u_u32[0] = 0x70000000 | 13;
        PacketBuff[1].u_u64[0] = SCE_GIF_SET_TAG(1, 1, 1, SCE_GS_SET_PRIM(4, 1, 1, 0, 1, 0, 1, 1, 0), 0, 12);
        PacketBuff[1].u_u64[1] = \
              (u64)SCE_GS_RGBAQ       | (u64)SCE_GS_UV << 4  | (u64)SCE_GS_XYZ2 << 8  \
            | (u64)SCE_GS_RGBAQ << 12 | (u64)SCE_GS_UV << 16 | (u64)SCE_GS_XYZ2 << 20 \
            | (u64)SCE_GS_RGBAQ << 24 | (u64)SCE_GS_UV << 28 | (u64)SCE_GS_XYZ2 << 32 \
            | (u64)SCE_GS_RGBAQ << 36 | (u64)SCE_GS_UV << 40 | (u64)SCE_GS_XYZ2 << 44;
        PacketBuff[2].u_u128 = RGBA[0].u_u128;
        PacketBuff[3].u_u128 = UV[0].u_u128;
        PacketBuff[4].u_u128 = XYZ[0].u_u128;
        PacketBuff[5].u_u128 = RGBA[1].u_u128;
        PacketBuff[6].u_u128 = UV[1].u_u128;
        PacketBuff[7].u_u128 = XYZ[1].u_u128;
        PacketBuff[8].u_u128 = RGBA[2].u_u128;
        PacketBuff[9].u_u128 = UV[2].u_u128;
        PacketBuff[10].u_u128 = XYZ[2].u_u128;
        PacketBuff[11].u_u128 = RGBA[3].u_u128;
        PacketBuff[12].u_u128 = UV[3].u_u128;
        PacketBuff[13].u_u128 = XYZ[3].u_u128;
        FlushCache(0);
        pDma.Gif->chcr.TTE = 0;
        sceDmaSend(pDma.Gif, PacketBuff);
        sceDmaSync(pDma.Gif, 0, 0);

        SetDefaultDrawEnv();
    }
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

// Not matching: https://decomp.me/scratch/4yr7l
SFXOBJ* SetSfxObject(u32 *DataAdrs) {
    s32 i;
    u32 *TmpDataAdrs;
    u32 DataSize = 0;
    s32 ModelNum = *DataAdrs;
    s32 PartsNum;
    s32 CoordNum;
    u32 *ActAdrs;
    u32 *GmsAdrs;
    u32 *MimeAdrs;
    u32 *EnvAdrs;
    s32 CoordBuffSize;
    s32 PartsBuffSize;
    s32 MimeBuffSize = 0;
    u32 *BuffAdrs;
    SFXOBJ *pObj;
    SFXOBJ *pObjSub;
    SFXOBJ *pObjLast;
    u32 SimpleFlag;
    
    if (ModelNum > 3) {
        TmpDataAdrs = Fadr(DataAdrs, 3);
        if (TmpDataAdrs[11] == 0x1234567) {
            return SetSvxObject(DataAdrs);
        }
    }

    for (i = 0; i < ModelNum; i++) {
        TmpDataAdrs = Fadr(DataAdrs, i);
        DataSize = (DataSize + 0xF) & ~0xF;
        DataSize += sizeof(SFXOBJ);
        DataSize = (DataSize + 0xF) & ~0xF;

        BuffAdrs = Fadr(TmpDataAdrs, 0);
        PartsNum = ((TYPE_SFX_HEADER *)BuffAdrs)->parts_num;
        PartsBuffSize = PartsNum;
        PartsBuffSize *= sizeof(PARTS);
        DataSize += PartsBuffSize;
        DataSize = (DataSize + 0xF) & ~0xF;

        ActAdrs = Fadr(TmpDataAdrs, 3);
        if (*ActAdrs != 0) {
            BuffAdrs = Fadr(ActAdrs, 0);
            CoordNum = ((ACT_HEADER *)BuffAdrs)->PartsNum;
            DataSize += sizeof(MOTION);
            DataSize = (DataSize + 0xF) & ~0xF;
            CoordBuffSize = sizeof(tagCOORD);
            CoordBuffSize *= CoordNum + 1;
            DataSize += CoordBuffSize;
        }

        if (*ActAdrs == 0 && i == 0) {
            DataSize += sizeof(MOTION);
            DataSize = (DataSize + 0xF) & ~0xF;
            DataSize += sizeof(tagCOORD);
        }
    }

    TmpDataAdrs = Fadr(DataAdrs, 0);
    TmpDataAdrs = Fadr(TmpDataAdrs, 2);
    if (*TmpDataAdrs != 0) {
        DataSize = (DataSize + 0xF) & ~0xF;
        MimeBuffSize = (sizeof(MIME) + 0xF) & ~0xF;
        DataSize += (sizeof(MIME) + 0xF) & ~0xF;
    }
    
    pObj = (SFXOBJ *)GetMem(DataSize);
    func_00213088();
    pObj->pObjTop = pObj;
    pObj->pObjSub = NULL;
    pObjLast = pObj;

    TmpDataAdrs = Fadr(DataAdrs, 0);
    BuffAdrs = Fadr(TmpDataAdrs, 0);
    BuffAdrs = (u32 *)((u32)pObj + SetSfxWork(pObj, BuffAdrs));
    BuffAdrs = ALIGN_BUF(BuffAdrs);
    
    if (*Fadr(TmpDataAdrs, 3) != 0) {
        SimpleFlag = 0;
        BuffAdrs = (u32 *)((u32)BuffAdrs + SetSfxAct(BuffAdrs, pObj, Fadr(TmpDataAdrs, 3)));
        BuffAdrs = ALIGN_BUF(BuffAdrs);
    } else {
        SimpleFlag = 1;
        BuffAdrs = (u32 *)((u32)BuffAdrs + SetSfxActSimple(BuffAdrs, pObj));
        BuffAdrs = ALIGN_BUF(BuffAdrs);
    }
    
    if (Fadr(TmpDataAdrs, 1) == NULL) {
        pObj->GmsAdrs = NULL;
    } else {
        pObj->GmsAdrs = (s32 *)Fadr(TmpDataAdrs, 1);
    }
    
    if (Fadr(TmpDataAdrs, 2) == NULL) {
        pObj->MimeAdrs = NULL;
    } else {
        pObj->MimeAdrs = (s32 *)Fadr(TmpDataAdrs, 2);
    }

    if ((s32)*TmpDataAdrs < 5) {
        pObj->EnvAdrs = NULL;
    } else {
        EnvAdrs = Fadr(TmpDataAdrs, 4);
        if (*EnvAdrs != 0) {
            pObj->EnvAdrs = (SFXENV *)EnvAdrs;
        } else {
            pObj->EnvAdrs = NULL;
        }
    }

    pObj->Condition = 1;
    pObj->Flag = 1;
    pObj->Pause = 0;

    if (!SimpleFlag) {
        for (i = 1; i < ModelNum; i++) {
            TmpDataAdrs = Fadr(DataAdrs, i);
            pObjSub = (SFXOBJ *)BuffAdrs;
            pObjLast->pObjSub = pObjSub;
            pObjSub->pObjSub = NULL;
            pObjSub->pObjTop = pObj;
            BuffAdrs = (u32 *)((u32)BuffAdrs + SetSfxWork(BuffAdrs, Fadr(TmpDataAdrs, 0)));
            BuffAdrs = ALIGN_BUF(BuffAdrs);

            BuffAdrs = (u32 *)((u32)BuffAdrs + SetSfxAct(BuffAdrs, pObjSub, Fadr(TmpDataAdrs, 3)));
            BuffAdrs = ALIGN_BUF(BuffAdrs);
            
            GmsAdrs = Fadr(TmpDataAdrs, 1);
            if (*GmsAdrs != 0) {
                pObjSub->GmsAdrs = (s32 *)GmsAdrs;
            } else {
                pObjSub->GmsAdrs = pObj->GmsAdrs;
            }
    
            MimeAdrs = Fadr(TmpDataAdrs, 2);
            if (*MimeAdrs != 0) {
                pObjSub->MimeAdrs = (s32 *)MimeAdrs;
            } else {
                pObjSub->MimeAdrs = NULL;
            }
    
            if ((s32)*TmpDataAdrs < 5) {
                pObjSub->EnvAdrs = NULL;
            } else {
                EnvAdrs = Fadr(TmpDataAdrs, 4);
                if (*EnvAdrs != 0) {
                    pObjSub->EnvAdrs = (SFXENV *)EnvAdrs;
                } else {
                    pObjSub->EnvAdrs = NULL;
                }
            }
            
            pObjSub->Flag = 1 << i;
            pObjLast = pObjSub;
        }

        if (MimeBuffSize != 0) {
            for (pObjSub = pObj; pObjSub != NULL; pObjSub = pObjSub->pObjSub) {
                pObjSub->pMime = (MIME *)BuffAdrs;
                pObjSub->pMime->IdFlag = 0;
                pObjSub->pMime->pVmime = NULL;
            }
        } else {
            for (pObjSub = pObj; pObjSub != NULL; pObjSub = pObjSub->pObjSub) {
                pObjSub->pMime = NULL;
            }
        }
        
        for (pObjSub = pObj; pObjSub != NULL; pObjSub = pObjSub->pObjSub) {
            pObjSub->ClipOffset = -15.0f;
            pObjSub->ClipZone = 50.0f;
        }
    }

    if (pObj != NULL) {
        for (pObjSub = pObj; pObjSub != NULL; pObjSub = pObjSub->pObjSub) {
            if (pObjSub->EnvAdrs != NULL) {
                for (i = 0; i < pObjSub->PartsNum; i++) {
                    pObjSub->pParts[i].SpecType = pObjSub->EnvAdrs->spectype[i];
                    pObjSub->pParts[i].OutLine = pObjSub->EnvAdrs->outline[i];
                }
                pObjSub->LineEnv.status = (u8 *)pObjSub->EnvAdrs->outline;
            } else {
                for (i = 0; i < pObjSub->PartsNum; i++) {
                    pObjSub->pParts[i].SpecType = 0;
                    pObjSub->pParts[i].OutLine = 0;
                }
                pObjSub->LineEnv.status = (u8 *)OutLineStatusDef;
            }
        }
    }
    
    return pObj;
}

SFXOBJ* SetSvxObject(u32 *DataAdrs) {
	s32 CoordNum;
	s32 CoordBuffSize;
	u32 *BuffAdrs;
	SFXOBJ *pObj;
    u32 var_s2 = ~0xF; // ?
    s16 yeet; // ?

    yeet = *(s16 *)Fadr(Fadr(DataAdrs, 2), 0);
    CoordBuffSize = sizeof(tagCOORD);
    CoordBuffSize *= yeet + 1;
    CoordBuffSize += 0x2C0;
    pObj = (SFXOBJ *)GetMem(CoordBuffSize);
    // BuffAdrs = ALIGN_ALT(pObj, sizeof(SFXOBJ));
    BuffAdrs = (u32 *)(((u32)pObj + sizeof(SFXOBJ) + 0xF) & var_s2);
    pObj->pObjTop = pObj;
    pObj->pObjSub = NULL;
    pObj->SvxAdrs = (s32 *)Fadr(DataAdrs, 0);
    pObj->GmsAdrs = (s32 *)Fadr(DataAdrs, 1);
    Fadr(DataAdrs, 2);
    SetSfxAct(BuffAdrs, pObj, Fadr(DataAdrs, 2));
    pObj->SvxWorkAdrs = (s32 *)Fadr(DataAdrs, 3);
    pObj->ClipOffset = -15.0f;
    pObj->ClipZone = 50.0f;
    pObj->Condition = 1;
    pObj->Flag = 1;
    pObj->Pause = 0;
    pObj->ClipFlag = 1;
    pObj->ScaleVector[0] = 1.0f;
    pObj->ScaleVector[1] = 1.0f;
    pObj->ScaleVector[2] = 1.0f;
    pObj->ScaleVector[3] = 1.0f;
    pObj->pLightColor = &Light3.LightColor;
    pObj->pNormalLight = &Light3.NormalLight;
    pObj->OutLineFlag = 0;
    pObj->GmsTransType = 1;
    pObj->Cbp = 0;
    pObj->ClutNum0 = 0;
    pObj->ClutNum1 = 0;
    pObj->ClutWeight = 0.0f;
    pObj->pMime = NULL;
    pObj->MotionSyncFlag = 0;
    if (pObj->SvxWorkAdrs[10] == 0) {
        hr_decode_tkVU1((HRVU1OBJ *)pObj->SvxWorkAdrs, (u32 *)pObj->SvxAdrs, (u32 *)pObj->GmsAdrs);
        ((u32 *)pObj->SvxWorkAdrs)[10] = 0xFFFFFFFF;
    }

    return pObj;
}

void SetObjClipZone(SFXOBJ *pObj, f32 offset, f32 zone) {
	SFXOBJ *pObjTmp;

    for (pObjTmp = pObj; pObjTmp != NULL; pObjTmp = pObjTmp->pObjSub) {
        pObjTmp->ClipOffset = offset;
        pObjTmp->ClipZone = zone;
    }
}

s32 GetObjShadowData(SFXOBJ *pObj, sceVu0FVECTOR TmpVec) {
	s32 i;
	f32 MaxScale;

    for (i = 0, MaxScale = 0.0f; i < 3; i++) {
        if (pObj->ScaleVector[i] > MaxScale) {
            MaxScale = pObj->ScaleVector[i];
        }
    }

    TmpVec[0] = MaxScale * pObj->ShadowSize;
    TmpVec[1] = MaxScale * pObj->ShadowOffset;
    TmpVec[2] = MaxScale * pObj->ShadowRange;
    return pObj->ShadowType;
}

void SetObjShadow(SFXOBJ *pObj, s32 Type, f32 Size, f32 Offset, f32 Range) {
	SFXOBJ *pObjTmp;

    for (pObjTmp = pObj; pObjTmp != NULL; pObjTmp = pObjTmp->pObjSub) {
        pObjTmp->ShadowType = Type;
        pObjTmp->ShadowSize = Size;
        pObjTmp->ShadowOffset = Offset;
        pObjTmp->ShadowRange = Range;
    }
}

s32 SfxObjBallClipCheck(SFXOBJ *pObj) {
	sceVu0FMATRIX TmpMatrix;
	sceVu0FVECTOR TmpVector[5];
	static sceVu0IVECTOR TmpIVector[5];
	s32 i;
	f32 Radius = pObj->ClipZone;
	f32 Scale = 0.0f;
	sceVu0IVECTOR COL0;
	sceVu0IVECTOR XYZ0;
	sceVu0IVECTOR XYZ1;
	sceVu0IVECTOR XYZ2;
	sceVu0IVECTOR XYZ3;
	qword PacketBuff[64];

    for (i = 0; i < 3; i++) {
        if (Scale < pObj->ScaleVector[i]) {
            Scale = pObj->ScaleVector[i];
        }
    }
    
    sceVu0UnitMatrix(TmpMatrix);
    SETVEC(TmpMatrix[3], 0.0f, pObj->ClipOffset, 0.0f, 1.0f);
    sceVu0MulMatrix(TmpMatrix, pObj->pMot->pBaseCoord->Mtx, TmpMatrix);
    sceVu0MulMatrix(TmpMatrix, Scr.WvMtx, TmpMatrix);    
    
    SETVEC(TmpMatrix[0], Scale, 0.0f, 0.0f, 0.0f);
    SETVEC(TmpMatrix[1], 0.0f, Scale, 0.0f, 0.0f);
    SETVEC(TmpMatrix[2], 0.0f, 0.0f, Scale, 0.0f);
    sceVu0MulMatrix(TmpMatrix, Scr.VsMtx, TmpMatrix);
    
    SETVEC(TmpVector[0], 0.0f + Radius, 0.0f, 0.0f, 1.0f);
    SETVEC(TmpVector[1], 0.0f - Radius, 0.0f, 0.0f, 1.0f);
    SETVEC(TmpVector[2], 0.0f, 0.0f + Radius, 0.0f, 1.0f);
    SETVEC(TmpVector[3], 0.0f, 0.0f - Radius, 0.0f, 1.0f);
    SETVEC(TmpVector[4], 0.0f, 0.0f, 0.0f + Radius, 1.0f);
    
    sceVu0RotTransPers(TmpIVector[0], TmpMatrix, TmpVector[0], 0);
    sceVu0RotTransPers(TmpIVector[1], TmpMatrix, TmpVector[1], 0);
    sceVu0RotTransPers(TmpIVector[2], TmpMatrix, TmpVector[2], 0);
    sceVu0RotTransPers(TmpIVector[3], TmpMatrix, TmpVector[3], 0);
    sceVu0RotTransPers(TmpIVector[4], TmpMatrix, TmpVector[4], 0);
    if (TmpIVector[0][0] < 0x6C00) {
        return 1;
    } else if (TmpIVector[1][0] > 0x9400) {
        return 1;
    } else if (TmpIVector[2][1] < 0x7800) {
        return 1;
    } else if (TmpIVector[3][1] > 0x8800) {
        return 1;
    } else if (TmpIVector[4][2] >= 0x1000000U) {
        return 1;
    } else {
        return 0;
    }
}

void SetObjAllPause(s32 flag) {
    SfxAllPause = flag;
}

