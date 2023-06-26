#include "common.h"

hMNGLIST EvMng[64];
hEvOBJECT EvObj[64];
hEvOBJECT *pEvObj[64];
hEvTABLE EvTbl[20][128];
hEvRESTART EvRestart[20][128];
static s32 NoStageData;
static s32 NoEvent;
static u32 EvFlag;
static s32 TblNum;
static s32 ObjNum;
static s32 KloRt;
static s32 KloCnt;
static s32 RtBak;
static s32 CntBak;
static s32 areaMax;
static s32 tblMax[20];

void hEvInitFrame() {
    Vu0ProgTrans();
    OutLineEnvInit(GameGbl.fr, GameGbl.inter);
}

void hEvObjMake(hEvTABLE *pt) {
    hEvOBJECT *pEO;

    pEO = pEvObj[ObjNum] = &EvObj[hWorkGet(1)];
    pEO->Scale[0] = pEO->Scale[1] = pEO->Scale[2] = pt->pHd->Scale / 100.0f;
    pEO->Scale[3] = 0.0f;
    pEO->Loop = 0;
    pEO->Wait = pt->pHd->Wait;
    pEO->Frame = 0;
    pEO->Frame2 = 0;
    pEO->TblIdx = pt->pHd->Index;
    pEO->Mcn = 0;
    pEO->pTbl = pt;
    pEO->RtWrk.rtp = GetEvRtAdrs(pt->pHd->Route);
    pEO->pSfx = GetSfxObjectData(pt->pHd->Object);
    SetObjScale(pEO->pSfx, pEO->Scale);
    if (pt->pHd->OutLine == 0)
        SetObjOutlineOff(pEO->pSfx);

    pEO->seFlag = 0;
    if (pt->pHd->StartSEprog == -1)
        pEO->seObj = -1;
    else
        pEO->seObj = hSeKeyOnEv(pt->pHd->StartSEprog, pt->pHd->StartSEsplt, pEO->pSfx->pMot->pBaseCoord->Trans, 1);

    pt->ActObj++;
    pt->Count++;
    ObjNum++;
}


void hEvObjDel(s32 idx) {
    hEvOBJECT *pEO;

    pEO = pEvObj[idx];
    if (pEO->seObj != -1)
        hSeKeyOff(pEO->seObj);
    ObjNum--;
    pEvObj[idx] = pEvObj[ObjNum];
    pEO->pTbl->ActObj--;
    EraseSfxObject(pEO->pSfx);
    hWorkDel(1, pEO->Idx);
}

void hEvObjAllDel() {
    s32 i;

    for (i = 0; i < ObjNum; i++)
        hEvObjDel(i);
}

u32 hEvGetFlag() {
    return EvFlag;
};

void hAngleClip(f32 *vec, s32 num) {
    s32 i;

    for (i = 0; i < num; i++) {
        while (vec[i] < -M_PIf)
            vec[i] += M_TWOPIf;
        while (vec[i] > M_PIf)
            vec[i] -= M_TWOPIf;
    }
}

void hEvObjMukiChange(hEvOBJECT *pEO) {
    hEvTHEAD *pTH;
    hEvTBODY *pTB;
    sceVu0FVECTOR muki;
    sceVu0FVECTOR rot;
    s32 i;

    pTH = pEO->pTbl->pHd;
    pTB = &pEO->pTbl->pBd[pEO->TblIdx];
    pEO->Plc = pEO->RtWrk.plc;
    pEO->RotCnt = 0;

    GetMichiVec(&pEO->RtWrk, muki);
    GetMichiRotXY(muki, rot);
    if (pEO->Dist < 0)
        rot[1] += 3.14f;
    if (pTH->RotPrm[0] == 0)
        rot[0] = 0.0f;
    if (pTH->RotPrm[1] == 0)
        rot[1] = 0.0f;
    rot[2] = 0.0f;

    for (i = 0; i < 2; i++) {
        rot[i] += (pTH->DefRot[i] + pTB->Rot[i]) * 3.14f / 180.0f;
        if (pEO->Frame == 0)
            pEO->pSfx->pMot->pBaseCoord->Rot[i] = rot[i];
        pEO->dRot[i] = rot[i] - pEO->pSfx->pMot->pBaseCoord->Rot[i];
    }

    hAngleClip(pEO->dRot, 2);
    for (i = 0; i < 2; i++)
        pEO->dRot[i] /= 32.0f;
}

void hEvObjBankChange(hEvOBJECT *pEO) {
    hEvTHEAD *pTH;
    hEvTBODY *pTB;

    pTH = pEO->pTbl->pHd;
    pTB = &pEO->pTbl->pBd[pEO->TblIdx];

    pEO->pSfx->pMot->pBaseCoord->Rot[2] = (pTH->DefRot[2] + pTB->Bank[0]) * M_PIf / 180.0f;
    pEO->dRot[2] = ((pTB->Bank[1] - pTB->Bank[0]) * M_PIf / 180.0f) / pEO->Time;
    hAngleClip((f32 *)pEO->dRot + 2, 1);
}

void hEvObjMuki(hEvOBJECT *pEO) {
    s32 i;

    if (pEO->Plc != pEO->RtWrk.plc)
        hEvObjMukiChange(pEO);

    if (pEO->RotCnt < 32) {
        pEO->RotCnt++;
        for (i = 0; i < 2; i++)
            pEO->pSfx->pMot->pBaseCoord->Rot[i] += pEO->dRot[i];
    }

    pEO->pSfx->pMot->pBaseCoord->Rot[2] += pEO->dRot[2];
    hAngleClip(pEO->pSfx->pMot->pBaseCoord->Rot, 3);
}

void hEvObjMoveInit(hEvOBJECT *pEO) {
    hEvTBODY *pTB;

    pTB = &pEO->pTbl->pBd[pEO->TblIdx];
    pEO->Dist = pTB->Point[1] - pTB->Point[0];
    hEvObjMukiChange(pEO);
    pEO->Infinity = 0;

    if (pTB->Type == 1) {
        pEO->Time = (s32)GetActEndCnt(pEO->pSfx) * pTB->MotLoop;
        hEvObjBankChange(pEO);
    } else if (pTB->Type == 2) {
        pEO->Time = pTB->Stay;
        if (pEO->Time == 0) {
            pEO->Time = 1;
            pEO->Infinity = 1;
        }
        hEvObjBankChange(pEO);
    } else {
        if (pEO->Dist != 0) {
            pEO->Time = pEO->Dist * 2 / (pTB->Speed[1] + pTB->Speed[0]);
            pEO->Accel = pTB->Speed[1] * pTB->Speed[1] - pTB->Speed[0] * pTB->Speed[0];
            hEvObjBankChange(pEO);
        }
    }
}

void hEvObjMove(hEvOBJECT *pEO) {
    hEvTBODY *pTB;
    s32 x;
    s32 t;
    s32 c; // idk how to match this without using another variable

    pTB = &pEO->pTbl->pBd[pEO->TblIdx];
    hEvObjMuki(pEO);

    if (pEO->Dist == 0) {
        x = pTB->Point[0];
    } else {
        t = pEO->Frame2;
        x = (pTB->Point[0] + pTB->Speed[0] * t) + pEO->Accel * t * t / pEO->Dist / 4;
    }

    c = pEO->Mcn;
    pEO->Mcn = x;
    MichiMove(&pEO->RtWrk, x - c);
    MichiPosi(&pEO->RtWrk, pEO->pSfx->pMot->pBaseCoord->Trans);
    SetBaseMatrix(pEO->pSfx, pEO->pSfx->pMot->pBaseCoord->Rot, pEO->pSfx->pMot->pBaseCoord->Trans, 1);
}

void hEvObjAppear() {
    hEvTABLE *pt;
    hEvTHEAD *ph;
    s32 i;
    s32 app;

    if (RtBak == -1)
        return;

    for (i = 0; i < TblNum; i++) {
        pt = &EvTbl[GameGbl.vision & 0xFF][i];
        ph = pt->pHd;

        for (app = 0; app < ph->AppNum; app++) {
            if (pt->ActObj <= 0) {
                if (ph->AppRt[app] == -1) {
                    if (ph->AppCnt[app] == -1 && pt->StartApp == 0 && (ph->Boot > pt->Count || ph->Boot == 0)) {
                        pt->StartApp = 1;
                        if (ObjNum < 64)
                            hEvObjMake(pt);
                    }
                } else if (KloRt == ph->AppRt[app] && KloRt == RtBak) {
                    if (KloCnt >= ph->AppCnt[app] && ph->AppCnt[app] > CntBak || KloCnt < ph->AppCnt[app] && CntBak >= ph->AppCnt[app]) {
                        if (ph->Boot > pt->Count || ph->Boot == 0) {
                            if (ObjNum < 64)
                                hEvObjMake(pt);
                        }
                    }
                }
            }
        }
    }
}

void hEvObjCtrl() {
    hEvOBJECT *pEO;
    hEvTHEAD *pTH;
    hEvTBODY *pTB;
    s32 i;
    s32 del;
    s32 idx;
    s32 step;

    if (GameGbl.pause_flag != 0)
        return;

    EvFlag = 0;
    for (i = 0; i < ObjNum; i++) {
        pEO = pEvObj[i];
        idx = pEO->TblIdx;
        pTH = pEO->pTbl->pHd;
        pTB = &pEO->pTbl->pBd[idx];

        if (pEO->Wait < 1) {
            if (pEO->Frame2 == 0) {
                EvFlag |= pTB->Flag;
                pEO->pSfx->Condition = 1 << pTB->ModelSize;

                if (idx == 0) {
                    SetAct(pEO->pSfx, pTB->Motion);
                    SetMichinori(&pEO->RtWrk, 0);
                    hEvObjMoveInit(pEO);
                    pEO->MotLoop = 0;
                    pEO->seFlag = 0;
                    pEO->Mcn = 0;
                    pEO->Light = pTB->Light;
                    pEO->LightBak = -1;
                } else {
                    if (pTB->Motion != (pTB - 1)->Motion)
                        SetAct(pEO->pSfx, pTB->Motion);
                    hEvObjMoveInit(pEO);
                    pEO->MotLoop = 0;
                    pEO->seFlag = 0;
                    pEO->LightBak = pEO->Light;
                    pEO->Light = pTB->Light;
                    pEO->LightCnt = 0.0f;
                }
            }

            hEvObjMove(pEO);
            if (pEO->Frame2 == 0 && pTB->Type == 0 && pTB->SEprog != -1 && GetSfxOutFlag(pEO->pSfx) == 0)
                hSeKeyOnEv(pTB->SEprog, pTB->SEsplt, pEO->pSfx->pMot->pBaseCoord->Trans, 0);
            if (GetActStopFlag(pEO->pSfx) != 0) {
                pEO->MotLoop++;
                pEO->seFlag = 0;
            }
            pEO->Frame++;
            pEO->Frame2++;

            step = 0;
            switch (pTB->Type) {
                case 0:
                    if (pEO->Frame2 == pEO->Time)
                        step = 1;
                    break;
                case 1:
                    if (pEO->MotLoop >= pTB->MotLoop) {
                        step = 1;
                        break;
                    }
                    if (pEO->seFlag == 0 && GetActCnt(pEO->pSfx) < 2.0f && pTB->SEprog != -1 && GetSfxOutFlag(pEO->pSfx) == 0) {
                        pEO->seFlag = 1;
                        hSeKeyOnEv(pTB->SEprog, pTB->SEsplt, pEO->pSfx->pMot->pBaseCoord->Trans, 0);
                    }
                    break;
                case 2:
                    if (pEO->Frame2 == pEO->Time && pEO->Infinity == 0) {
                        step = 1;
                        break;
                    }
                    if (pEO->seFlag == 0 && GetActCnt(pEO->pSfx) < 2.0f && pTB->SEprog != -1 && GetSfxOutFlag(pEO->pSfx) == 0) {
                        pEO->seFlag = 1;
                        hSeKeyOnEv(pTB->SEprog, pTB->SEsplt, pEO->pSfx->pMot->pBaseCoord->Trans, 0);
                    }
                    break;
            }

            if (step != 0) {
                pEO->TblIdx++;
                if (pTH->TblNum == pEO->TblIdx) {
                    pEO->Loop++;
                    if (pTH->Loop == 0 || pTH->Loop > pEO->Loop) {
                        pEO->TblIdx = 0;
                        pEO->Wait = -1;
                        pEO->Frame = 0;
                        pEO->Frame2 = 0;
                    } else {
                        hEvObjDel(i);
                        i--;
                        continue;
                    }
                } else {
                    pEO->Frame2 = 0;
                }
            }

            for (del = 0; del < pTH->DelNum; del++) {
                if (pTH->DelRt[del] != -1 && KloRt == pTH->DelRt[del] && KloRt == RtBak) {
                    if (KloCnt >= pTH->DelCnt[del] && CntBak < pTH->DelCnt[del] || KloCnt < pTH->DelCnt[del] && CntBak >= pTH->DelCnt[del]) {
                        hEvObjDel(i);
                        i--;
                        break;
                    }
                }
            }
        }
    }
}

void hEvObjDraw() {
    hEvOBJECT *pEO;
    hEvTHEAD *pTH;
    LgtWorkF lgtw;
    LgtWorkF lgtw0;
    LgtWorkF lgtw1;
    sceVu0FMATRIX NormalLight;
    sceVu0FMATRIX LightColor;
    s32 i;

    for (i = 0; i < ObjNum; i++) {
        pEO = pEvObj[i];
        pTH = pEO->pTbl->pHd;

        if (pEO->LightBak != -1) {
            nkGetLgtWorkF(&lgtw0, pEO->LightBak);
            nkGetLgtWorkF(&lgtw1, pEO->Light);
            nkLightIp(&lgtw, &lgtw0, &lgtw1, pEO->LightCnt / 120.0f);
            nkCalcMatLgtWork(NormalLight, LightColor, &lgtw, NULL);
            SetObjNormalLight(pEO->pSfx, NormalLight);
            SetObjLightColor(pEO->pSfx, LightColor);
        } else {
            nkGetLgtWorkF(&lgtw, pEO->Light);
            nkCalcMatLgtWork(NormalLight, LightColor, &lgtw, NULL);
            SetObjNormalLight(pEO->pSfx, NormalLight);
            SetObjLightColor(pEO->pSfx, LightColor);
        }

        if (GameGbl.pause_flag != 0) {
            if (pEO->Wait < 1) {
                ModelDraw(pEO->pSfx);
                if (pTH->Shadow != 0)
                    abSetShadow(pEO->pSfx->pMot->pBaseCoord->Trans, pEO->pSfx);
            }
        } else {
            if (pEO->LightBak != -1) {
                pEO->LightCnt++;
                if (pEO->LightCnt >= 120.0f)
                    pEO->LightBak = -1;
            }

            if (pEO->Wait > 0) {
                pEO->Wait--;
            } else {
                if (pEO->Wait < 0)
                    pEO->Wait = pTH->Wait;
                ModelDraw(pEO->pSfx);
                if (pTH->Shadow != 0)
                    abSetShadow(pEO->pSfx->pMot->pBaseCoord->Trans, pEO->pSfx);
            }
        }
    }
}

void hEvObjMain() {
    OBJWORK *objw;

    if (NoEvent == 0) {
        objw = GameGbl.klonoa;
        KloRt = GetMichiNo(objw->rtw.rtp);
        KloCnt = objw->rtw.mcn >> 12;
        hEvObjAppear();
        hEvObjCtrl();
        hEvObjDraw();
        RtBak = KloRt;
        CntBak = KloCnt;
    }
}

void hEvTblSet(s32 *pEv, s32 pEv_index, s32 no) {
    EvTbl[no][pEv_index].ActObj = 0;
    EvTbl[no][pEv_index].Count = 0;
    EvTbl[no][pEv_index].StartApp = 0;
    EvTbl[no][pEv_index].pHd = (hEvTHEAD *)pEv;
    EvTbl[no][pEv_index].pBd = (hEvTBODY *)((u8 *)pEv + sizeof(hEvTHEAD));
}

void hEvTblInit(s32 addr, s32 ar) {
    s32 i;
    s32 *pEv; // idk how to match this without using another variable

    pEv = (s32 *)GetFHMAddress((u32 *)addr, 1);
    tblMax[ar] = GetFHMNum((u32 *)pEv);
    for (i = 0; i < tblMax[ar]; i++) {
        hEvTblSet(GetFHMAddress(pEv, i), i, ar);
    }
}

s32 hEvGetAreaAddr(s32 ar) {
    return (s32)GetFHMAddress((u32 *)hGetDataAddr(2), ar);
}

void hEvAreaInit() {
    s32 i;
    s32 packaddr;

    NoEvent = 0;
    if (NoStageData != 0) {
        NoEvent = 1;
    } else {
        packaddr = hEvGetAreaAddr(GameGbl.vision & 0xFF);
        if (packaddr == -1) {
            NoEvent = 1;
        } else {
            if (GetFHMNum((u32 *)packaddr) == 1) {
                NoEvent = 1;
            } else {
                nkInitEvRt(GetFHMAddress((u32 *)packaddr, 0));
                TblNum = GetFHMNum(GetFHMAddress((u32 *)packaddr, 1));
                for (i = 0; i < TblNum; i++) {
                    EvTbl[GameGbl.vision & 0xFF][i].ActObj = 0;
                    EvTbl[GameGbl.vision & 0xFF][i].StartApp = 0;
                }

                ObjNum = 0;
                CntBak = -1;
                RtBak = -1;
                hMngInit(EvMng, 64, 1);
                for (i = 0; i < 64; i++) {
                    EvObj[i].Idx = i;
                }
            }
        }
    }
}

void hEvStageInit() {
    char filename[256];
    s32 ar;

    NoStageData = 0;
    EvFlag = 0;

    if (hGetDataAddr(2) == NULL) {
        NoStageData = 1;
        areaMax = 0;
    } else {
        areaMax = GetFHMNum((u32 *)hGetDataAddr(2));
        for (ar = 0; ar < areaMax; ar++) {
            if (GetFHMNum((u32 *)hEvGetAreaAddr(ar)) == 2)
                hEvTblInit(hEvGetAreaAddr(ar), ar);
        }
    }
}

void hEvPushRestart() {
    s32 ar;
    s32 i;

    if (NoStageData == 0) {
        for (ar = 0; ar < areaMax; ar++) {
            for (i = 0; i < tblMax[ar]; i++) {
                EvRestart[ar][i].Count = EvTbl[ar][i].Count;
            }
        }
    }
}

void hEvPopRestart() {
    s32 ar;
    s32 i;

    if (NoStageData == 0) {
        for (ar = 0; ar < areaMax; ar++) {
            for (i = 0; i < tblMax[ar]; i++) {
                EvTbl[ar][i].Count = EvRestart[ar][i].Count;
            }
        }
    }
}
