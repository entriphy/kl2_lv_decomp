#include "motsys.h"
#include "object.h"
#include "motip.h"
#include "sfxbios.h"

void GetMotion(SFXOBJ *pObj) {
    if (pObj->pObjTop->Pause == 0 && SfxAllPause == 0 && pObj->MotionSyncFlag != 1)
        GetSfxWorldMatrix(pObj);
    GetNormalLightMatrix(pObj);
    GetScreenMotion(pObj);
}

void GetMotionSimple(SFXOBJ *pObj) {
    GetNormalLightMatrix(pObj);
    GetScreenMotion(pObj);
}

void GetLocalMotion(MOTION *m) {
    // TODO
}

void GetSimpleSfxMatrix(SFXOBJ *pObj) {
    MOTION *m;
    sceVu0FMATRIX ScaleMtx;

    m = pObj->pMot;
    sceVu0MulMatrix(SfxLcLightMtx[0], *pObj->pNormalLight, m->pBaseCoord->Mtx);
    GetScaleMtx(ScaleMtx, pObj->ScaleVector);
    sceVu0MulMatrix(ScaleMtx, m->pBaseCoord->Mtx, ScaleMtx);
    sceVu0MulMatrix(SfxLvMtx[0], Scr.WvMtx, ScaleMtx);
    sceVu0MulMatrix(SfxLsMtx[0], Scr.VsMtx, SfxLvMtx[0]);
    sceVu0MulMatrix(SfxLvSpecMtx[0], Scr.WvMtx, m->pBaseCoord->Mtx);
}

void GetScreenMotion(SFXOBJ *pObj) {
    s32 i;
    f32 *pItrW;
    sceVu0FVECTOR EnvTrans;
    sceVu0FMATRIX ScaleMtx;
    sceVu0FMATRIX TmpMtx;
    MOTION *m = pObj->pMot;
    sceVu0FMATRIX TmpMatrix;
    sceVu0FVECTOR TmpVector;
    sceVu0IVECTOR TmpIVector;

    m = pObj->pMot;
    GetScaleMtx(ScaleMtx, pObj->ScaleVector);
    sceVu0InversMatrix(TmpMtx, m->pBaseCoord->Mtx);
    sceVu0MulMatrix(ScaleMtx, ScaleMtx, TmpMtx);
    sceVu0MulMatrix(ScaleMtx, m->pBaseCoord->Mtx, ScaleMtx);
    pItrW = (f32 *)m->pItrW;
    for (i = 0; i < m->CoordNum; i++) {
        EnvTrans[0] = *pItrW++;
        EnvTrans[1] = *pItrW++;
        EnvTrans[2] = *pItrW++;
        EnvTrans[3] = 0.0f;
        pItrW++;
        sceVu0ApplyMatrix(EnvTrans, m->pCoord[i].Mtx, EnvTrans);
        sceVu0CopyMatrix(SfxSkinMtx[i], m->pCoord[i].Mtx);
        SfxSkinMtx[i][3][0] = m->pCoord[i].Mtx[3][0] - EnvTrans[0];
        SfxSkinMtx[i][3][1] = m->pCoord[i].Mtx[3][1] - EnvTrans[1];
        SfxSkinMtx[i][3][2] = m->pCoord[i].Mtx[3][2] - EnvTrans[2];
        sceVu0MulMatrix(SfxSkinMtx[i], ScaleMtx, SfxSkinMtx[i]);
        sceVu0MulMatrix(SfxLvMtx[i], Scr.WvMtx, SfxSkinMtx[i]);
        sceVu0MulMatrix(SfxLsMtx[i], Scr.VsMtx, SfxLvMtx[i]);
        sceVu0MulMatrix(SfxLvSpecMtx[i], Scr.WvMtx, m->pCoord[i].Mtx);
    }
}

void GetNormalLightMatrix(SFXOBJ *pObj) {
    s32 i;
    sceVu0FMATRIX ScaleMtx;

    for (i = 0; i < pObj->pMot->CoordNum; i++)
        sceVu0MulMatrix(SfxLcLightMtx[i], *pObj->pNormalLight, pObj->pMot->pCoord[i].Mtx);
}

void InitSfxCoord(MOTION *m, u8 *pInf, COORD *pCoord) {
    s32 i;
    s16 *pInfs;

    m->pInf = pInf;
    m->pItr = pInf + *(s32 *)(pInf + 4);
    m->pItrW = pInf + *(s32 *)(pInf + 8);
    m->SubScale = 1.0f;
    pInfs = (s16 *)pInf;
    m->CoordNum = *pInfs;
    pInfs += 6;

    for (i = 0; i < 4; i++)
        m->Mb[i].Type = 0;

    m->pBaseCoord = pCoord;
    m->pCoord = pCoord + 1;

    for (i = 0; i < m->CoordNum; i++) {
        if (*pInfs == -1)
            m->pCoord[i].Super = NULL;
        else
            m->pCoord[i].Super = &m->pCoord[*pInfs];
        pInfs++;
    }
}

void InitCoord( MOTION *m, u8 *pInf, u8 *pItr, u8 *pItrW, COORD *pCoord) {
    s32 i;
    s16 *pInfs;

    m->pInf = pInf;
    m->pItr = pItr;
    m->pItrW = pItrW;
    pInfs = (s16 *)pInf;
    m->CoordNum = *pInfs++;

    for (i = 0; i < 4; i++)
        m->Mb[i].Type = 0;

    m->pBaseCoord = pCoord;
    m->pCoord = pCoord + 1;

    for (i = 0; i < m->CoordNum; i++) {
        if (*pInfs == -1)
            m->pCoord[i].Super = NULL;
        else
            m->pCoord[i].Super = &m->pCoord[*pInfs];
        pInfs++;
    }
}

void SetLocalMatrix(sceVu0FVECTOR Rot, sceVu0FVECTOR Tra, sceVu0FMATRIX Mtx) {
    sceVu0FMATRIX work;

    sceVu0UnitMatrix(work);
    sceVu0RotMatrix(work, work, Rot);
    sceVu0TransMatrix(Mtx, work, Tra);
}

void InterPolateMatrix(sceVu0FMATRIX dm, sceVu0FMATRIX m0, sceVu0FMATRIX m1, f32 Weight) {
    sceVu0FVECTOR axis;

    GetInterPolateAxis(axis, m0, m1);

    // AxisInterPolate must not be extern'd for this to match
    // Guess they forgot to include a header?
    AxisInterPolate(dm, m0, m1, axis, Weight);
}

void MotionMix(sceVu0FMATRIX *dm, sceVu0FMATRIX *m0, sceVu0FMATRIX *m1, s32 CoordNum, f32 Weight, u64 OnFlag) {
    s32 i;
    u64 MtxOut;

    MtxOut = OnFlag;
    if (Weight < 0.01) {
        for (i = 0; i < CoordNum; i++) {
            if (MtxOut & 1)
                sceVu0CopyMatrix(dm[i], m0[i]);
            MtxOut >>= 1;
        }
    } else {
        if (Weight > 0.99) {
            for (i = 0; i < CoordNum; i++) {
                if (MtxOut & 1)
                    sceVu0CopyMatrix(dm[i], m1[i]);
                MtxOut >>= 1;
            }

        } else {
            for (i = 0; i < CoordNum; i++) {
                if (MtxOut & 1)
                    LinerInterPolateMatrix(dm[i], m0[i], m1[i], Weight);
                MtxOut >>= 1;
            }
        }
    }
}

void CopyRotMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1) {
    __asm__ volatile(
        "lq $6, 0x0(%1)\n"
        "lq $7, 0x10(%1)\n"
        "lq $8, 0x20(%1)\n"
        "sd $6, 0x0(%0)\n"
        "pexew $6, $6\n"
        "sw $6, 0x8(%0)\n"
        "sd $7, 0x10(%0)\n"
        "pexew $7, $7\n"
        "sw $7, 0x18(%0)\n"
        "sd $8, 0x20(%0)\n"
        "pexew $8, $8\n"
        "sw $8, 0x28(%0)"
    : : "r" (m0), "r" (m1));
}

void ChangeLocalMatrix(sceVu0FMATRIX lm,sceVu0FMATRIX wm,sceVu0FMATRIX lwm) {
    sceVu0FMATRIX TmpMtx;

    sceVu0InversMatrix(TmpMtx, wm);
    sceVu0MulMatrix(lm, TmpMtx, lwm);
}

void GetLwMtx(COORD *pCoord) {
    sceVu0FMATRIX *pLwMtx;
    COORD *stack[64];
    s32 index;

    index = 0;
    while (1) {
        if (pCoord->Super == NULL) {
            pCoord->Flag = 1;
            pLwMtx = &pCoord->Mtx;
            break;
        }
        if (pCoord->Flag != 0) {
            pLwMtx = &pCoord->Mtx;
            break;
        }
        stack[index++] = pCoord;
        pCoord = pCoord->Super;
    }

    if (--index != -1) {
        do {
            pCoord = stack[index--];
            sceVu0MulMatrix(pCoord->Mtx, *pLwMtx, pCoord->Mtx);
            pCoord->Flag = 1;
            pLwMtx = &pCoord->Mtx;
        } while (index >= 0);
    }
}

void GetRotTransMatrixXYZ(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra) {
    sceVu0UnitMatrix(mtx);
    sceVu0RotMatrix(mtx, mtx, rot);
    sceVu0TransMatrix(mtx, mtx, tra);
}

void GetRotTransMatrixYXZ(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra) {
    sceVu0UnitMatrix(mtx);
    sceVu0RotMatrixZ(mtx, mtx, rot[2]);
    sceVu0RotMatrixZ(mtx, mtx, rot[0]);
    sceVu0RotMatrixZ(mtx, mtx, rot[1]);
    sceVu0TransMatrix(mtx, mtx, tra);
}

void GetRotTransMatrixXZY(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra) {
    sceVu0UnitMatrix(mtx);
    sceVu0RotMatrixZ(mtx, mtx, rot[1]);
    sceVu0RotMatrixZ(mtx, mtx, rot[2]);
    sceVu0RotMatrixZ(mtx, mtx, rot[0]);
    sceVu0TransMatrix(mtx, mtx, tra);
}

void GetRotTransMatrixZXY(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra) {
    sceVu0UnitMatrix(mtx);
    sceVu0RotMatrixZ(mtx, mtx, rot[1]);
    sceVu0RotMatrixZ(mtx, mtx, rot[0]);
    sceVu0RotMatrixZ(mtx, mtx, rot[2]);
    sceVu0TransMatrix(mtx, mtx, tra);
}

void GetRotTransMatrixYZX(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra) {
    sceVu0UnitMatrix(mtx);
    sceVu0RotMatrixZ(mtx, mtx, rot[0]);
    sceVu0RotMatrixZ(mtx, mtx, rot[2]);
    sceVu0RotMatrixZ(mtx, mtx, rot[1]);
    sceVu0TransMatrix(mtx, mtx, tra);
}

void GetRotTransMatrixZYX(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra) {
    sceVu0UnitMatrix(mtx);
    sceVu0RotMatrixZ(mtx, mtx, rot[0]);
    sceVu0RotMatrixZ(mtx, mtx, rot[1]);
    sceVu0RotMatrixZ(mtx, mtx, rot[2]);
    sceVu0TransMatrix(mtx, mtx, tra);
}

void SetMotionWorldIp(MOTION *m, f32 *wipcnt) {
    s32 i;
    s32 ii;

    for (i = 0; i < 4; i++) {
        if (m->Mb[i].Type == 5)
            m->Mb[i].Type = 0;
    }

    for (i = 0; i < 4; i++) {
        if (m->Mb[i].Type == 0) {
            m->Mb[i].Type = 5;
            m->Mb[i].OnFlag = 0xFFFFFFFFFFFFFFFF;
            m->Mb[i].Weight = 1.0f;

            for (ii = 0; ii < m->CoordNum; ii++) {
                m->pCoord[ii].WipCnt = wipcnt[ii];
                sceVu0CopyMatrix(m->pCoord[ii].MtxSav, m->pCoord[ii].Mtx);
            }

            break;
        }
    }
}

void SetAct(SFXOBJ *pObj, s32 Actnum) {
    SFXOBJ *pObjTmp;

    pObjTmp = GetActiveSfx(pObj);
    if (pObjTmp != NULL && pObj->pMot->CoordNum != 0) {
        if (Fadr((u32 *)pObjTmp->pMot->ActAdrs, Actnum) != Fadr((u32 *)pObjTmp->pMot->ActAdrs, Actnum + 1) && Actnum != 0)
            SetActSub(pObjTmp->pMot, Actnum);
        else
            return;
    }
}

void SetActIp(SFXOBJ *pObj, s32 Actnum) {
    SFXOBJ *pObjTmp;

    pObjTmp = GetActiveSfx(pObj);
    if (pObjTmp != NULL && pObj->pMot->CoordNum != 0) {
        if (Fadr((u32 *)pObjTmp->pMot->ActAdrs, Actnum) != Fadr((u32 *)pObjTmp->pMot->ActAdrs, Actnum + 1) && Actnum != 0)
            SetActIpSub(pObjTmp->pMot, Actnum);
        else
            return;
    }
}

void SetActMix(SFXOBJ *pObj, s32 Actnum) {
    SFXOBJ *pObjTmp;

    pObjTmp = GetActiveSfx(pObj);
    if (pObjTmp != NULL && pObj->pMot->CoordNum != 0) {
        if (Fadr((u32 *)pObjTmp->pMot->ActAdrs, Actnum) != Fadr((u32 *)pObjTmp->pMot->ActAdrs, Actnum + 1) && Actnum != 0)
            SetActMixSub(pObjTmp->pMot, Actnum);
        else
            return;
    }
}

void SetActSub(MOTION *m, s32 ActNum) {
    s32 i;
    ACT_HEADER *pAct;

    m->ActNum = ActNum;
    pAct = (ACT_HEADER *)Fadr((u32 *)m->ActAdrs, m->ActNum);

    for (i = 0; i < 4; i++)
        m->Mb[i].Type = 0;

    for (i = 0; i < 4; i++) {
        if (m->Mb[i].Type == 0) {
            m->Mb[i].Type = 1;
            m->Mb[i].OnFlag = 0xFFFFFFFFFFFFFFFF;
            m->Mb[i].StopFlag = pAct->stopflag;
            m->Mb[i].CntSpeed = 1.0f;
            m->Mb[i].pAct = (s8 *)pAct;
            m->Mb[i].SyncFlag = m->pActtbl[ActNum].sync;
            m->Mb[i].MotionEndCnt = pAct->FlameNum;
            if (m->Mb[i].StopFlag != 0)
                m->Mb[i].MotionEndCnt -= 1.0f;
            if (m->Mb[i].MotionEndCnt <= 0) {
                m->Mb[i].MotionEndCnt = 0.0f;
                m->Mb[i].SyncFlag = 0;
            }
            m->Mb[i].Weight = 1.0f;
            m->Mb[i].MotionCnt = 0.0f;
            m->Mb[i].InMode = 0;
            m->Mb[i].OutMode = 0;
            m->BaseIndex = i;
            break;
        }
    }
}

void SetActIpSub(MOTION *m, s32 ActNum) {
    s32 i;
    ACT_HEADER *pAct;

    m->ActNum = ActNum;
    pAct = (ACT_HEADER *)Fadr((u32 *)m->ActAdrs, m->ActNum);

    for (i = 0; i < 4; i++) {
        if (m->Mb[i].Type != 0) {
            switch (m->Mb[i].Type) {
                case 1:
                    if (pAct->inmode != 0 && pAct->incnt != 0) {
                        m->Mb[i].Type = 2;
                        m->Mb[i].Weight = 1.0f;
                        m->Mb[i].InMode = 0;
                        if (m->Mb[i].OutMode > 2)
                            m->Mb[i].OutMode -= 2;
                        m->IpIndex = i;
                    } else {
                        m->Mb[i].Type = 0;
                    }
                    break;
                case 2:
                    m->Mb[i].Type = 0;
                    break;
            }
        }
    }

    for (i = 0; i < 4; i++) {
        if (m->Mb[i].Type == 0) {
            m->Mb[i].Type = 1;
            m->Mb[i].OnFlag = 0xFFFFFFFFFFFFFFFF;
            m->Mb[i].StopFlag = pAct->stopflag;
            m->Mb[i].CntSpeed = 1.0f;
            m->Mb[i].pAct = (s8 *)pAct;
            m->Mb[i].SyncFlag = pAct->sync;
            m->Mb[i].MotionEndCnt = pAct->FlameNum;
            if (m->Mb[i].StopFlag != 0)
                m->Mb[i].MotionEndCnt -= 1.0f;
            if (m->Mb[i].MotionEndCnt <= 0) {
                m->Mb[i].MotionEndCnt = 0.0f;
                m->Mb[i].SyncFlag = 0;
            }

            m->Mb[i].Weight = 0.0f;
            m->Mb[i].MotionCnt = 0.0f;
            m->Mb[i].InMode = pAct->inmode;

            if (pAct->outmode != 0)
                m->Mb[i].OutMode = pAct->outmode + 2;
            else
                m->Mb[i].OutMode = 0;

            if (pAct->incnt < 0)
                m->Mb[m->IpIndex].OutCnt = m->Mb[i].MotionEndCnt * 0.25f;
            else
                m->Mb[m->IpIndex].OutCnt = pAct->incnt;

            if (m->Mb[i].SyncFlag != 0) {
                if (m->Mb[i].SyncFlag == m->Mb[m->IpIndex].SyncFlag)
                    m->Mb[i].MotionCnt = (m->Mb[m->IpIndex].MotionCnt / m->Mb[m->IpIndex].MotionEndCnt) * m->Mb[i].MotionEndCnt;
            }

            m->BaseIndex = i;
            break;
        }
    }
}

void SetActMixSub(MOTION *m, s32 Actnum) {
    s32 i;

    for (i = 0; i < 4; i++) {
        switch (m->Mb[i].Type) {
            case 0:
                break;
            case 3:
                m->Mb[i].Type = 0;
                break;
        }
    }

    for (i = 0; i < 4; i++) {
        if (m->Mb[i].Type == 0) {
            m->Mb[i].Type = 3;
            m->Mb[i].OnFlag = 0xFFFFFFFFFFFFFFFF;
            m->Mb[i].StopFlag = m->pActtbl[Actnum].stopflag;
            m->Mb[i].CntSpeed = 1.0f;
            m->ActNum = m->pActtbl[Actnum].mot;
            m->Mb[i].pAct = (s8 *)Fadr((u32 *)m->ActAdrs, m->ActNum);
            m->Mb[i].SyncFlag = m->pActtbl[Actnum].sync;
            m->Mb[i].MotionEndCnt = ((ACT_HEADER *)m->Mb[i].pAct)->FlameNum;
            if (m->Mb[i].StopFlag != 0)
                m->Mb[i].MotionEndCnt -= 1.0f;
            if (m->Mb[i].MotionEndCnt <= 0) {
                m->Mb[i].MotionEndCnt = 0.0f;
                m->Mb[i].SyncFlag = 0;
            }

            m->Mb[i].Weight = 0.0f;
            m->Mb[i].MotionCnt = 0.0f;
            m->Mb[i].TargetWeight = 1.0f;
            m->Mb[i].InMode = m->pActtbl[Actnum].inmode;

            if (m->pActtbl[Actnum].outmode != 0)
                m->Mb[i].OutMode = m->pActtbl[Actnum].outmode + 2;
            else
                m->Mb[i].OutMode = 0;

            if (m->pActtbl[Actnum].incnt < 0)
                m->Mb[i].InCnt = m->Mb[i].MotionEndCnt * 0.25f;
            else
                m->Mb[i].InCnt = m->pActtbl[Actnum].incnt;

            if (m->pActtbl[Actnum].outcnt < 0)
                m->Mb[i].OutCnt = m->Mb[i].MotionEndCnt * 0.25f;
            else
                m->Mb[i].OutCnt = m->pActtbl[Actnum].outcnt;

            if (m->Mb[i].SyncFlag != 0) {
                if (m->Mb[i].SyncFlag == m->Mb[m->BaseIndex].SyncFlag)
                    m->Mb[i].MotionCnt = (m->Mb[m->BaseIndex].MotionCnt / m->Mb[m->BaseIndex].MotionEndCnt) * m->Mb[i].MotionEndCnt;
            }

            m->BaseIndex = i;
            break;
        }
    }
}

s32 GetActStopFlag(SFXOBJ *pObj) {
    SFXOBJ *pObjTmp;

    pObjTmp = GetActiveSfx(pObj);
    if (pObjTmp != NULL)
        return pObjTmp->pMot->EndFlag;
    else
        return 0;
}

f32 GetActEndCnt(SFXOBJ *pObj) {
    SFXOBJ *pObjTmp;
    s32 cnt;

    pObjTmp = GetActiveSfx(pObj);
    if (pObjTmp != NULL) {
        // converts it to an int then back to a float
        // ...ok????
        cnt = pObjTmp->pMot->Mb[pObjTmp->pMot->BaseIndex].MotionEndCnt;
        return cnt;
    } else {
        return 0.0f;
    }
}

f32 GetActCnt(SFXOBJ *pObj) {
    SFXOBJ *pObjTmp;
    s32 cnt;

    pObjTmp = GetActiveSfx(pObj);
    if (pObjTmp != NULL) {
        cnt = pObjTmp->pMot->Mb[pObjTmp->pMot->BaseIndex].MotionCnt;
        return cnt;
    } else {
        return 0.0f;
    }
}

void SetBaseMatrix(SFXOBJ *pObj, sceVu0FVECTOR Rot, sceVu0FVECTOR Trans, s32 RotOrder) {
    MOTION *m;

    m = pObj->pMot;
    m->pBaseCoord->Rot[0] = Rot[0];
    m->pBaseCoord->Rot[1] = Rot[1];
    m->pBaseCoord->Rot[2] = Rot[2];
    m->pBaseCoord->Rot[3] = 0.0f;
    m->pBaseCoord->Trans[0] = Trans[0];
    m->pBaseCoord->Trans[1] = Trans[1];
    m->pBaseCoord->Trans[2] = Trans[2];
    m->pBaseCoord->Trans[3] = 1.0f;
    m->pBaseCoord->Flag = -1;

    switch (RotOrder) {
        case 0:
            GetRotTransMatrixXYZ(m->pBaseCoord->Mtx, m->pBaseCoord->Rot, m->pBaseCoord->Trans);
            break;
        case 1:
            GetRotTransMatrixYXZ(m->pBaseCoord->Mtx, m->pBaseCoord->Rot, m->pBaseCoord->Trans);
            break;
        case 2:
            GetRotTransMatrixXZY(m->pBaseCoord->Mtx, m->pBaseCoord->Rot, m->pBaseCoord->Trans);
            break;
        case 3:
            GetRotTransMatrixZXY(m->pBaseCoord->Mtx, m->pBaseCoord->Rot, m->pBaseCoord->Trans);
            break;
        case 4:
            GetRotTransMatrixYZX(m->pBaseCoord->Mtx, m->pBaseCoord->Rot, m->pBaseCoord->Trans);
            break;
        case 5:
            GetRotTransMatrixZYX(m->pBaseCoord->Mtx, m->pBaseCoord->Rot, m->pBaseCoord->Trans);
            break;
    }
}

void SetBaseMatrix2(SFXOBJ *pObj, sceVu0FMATRIX SrcMatrix) {
    pObj->pMot->pBaseCoord->Flag = -1;
    sceVu0CopyMatrix(pObj->pMot->pBaseCoord->Mtx,SrcMatrix);
}

void GetScaleMtx(sceVu0FMATRIX mtx, sceVu0FVECTOR vec) {
    mtx[0][0] = vec[0];
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = vec[1];
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = vec[2];
    mtx[2][3] = 0.0f;
    mtx[3][0] = 0.0f;
    mtx[3][1] = 0.0f;
    mtx[3][2] = 0.0f;
    mtx[3][3] = 1.0f;
}

void NormalMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1) {
    sceVu0FVECTOR tmpvect;

    sceVu0Normalize(m0[0], m1[0]);
    sceVu0ScaleVector(tmpvect, m0[0], sceVu0InnerProduct(m1[1], m0[0]));
    sceVu0SubVector(tmpvect, m1[1], tmpvect);
    sceVu0Normalize(m0[1], tmpvect);
    sceVu0OuterProduct(m0[2], m0[0], m0[1]);
    sceVu0CopyVector(m0[3], m1[3]);
}

void DecodeMotion(sceVu0FMATRIX *DecodeBuff, MOTION *m, s32 Ind) {
    s32 i;
    s16 *pInf;
    s16 *pRot;
    s16 *pTra;
    f32 *pItr;
    s16 *pRot2;
    s16 *pTra2;
    f32 *pItr2;
    ACT_HEADER *pActHeader;
    s16 RotNum;
    s16 TraNum;
    u64 TraOut;
    sceVu0FMATRIX TmpMtx0;
    sceVu0FMATRIX TmpMtx1;
    sceVu0IVECTOR RotI;
    sceVu0FVECTOR RotF;
    sceVu0FVECTOR RotF2;
    sceVu0IVECTOR TraI;
    sceVu0FVECTOR TraF;
    sceVu0FVECTOR TraF2;
    f32 scale;
    sceVu0FVECTOR Offset;
    f32 MotionCntFloatOnly;
    f32 MotionCntFloat;
    s32 MotionCnt;
    s32 SubMotionCnt;

    pActHeader = (ACT_HEADER *)m->Mb[Ind].pAct;
    if (pActHeader->CompressFlag != 0) {
        AcxDecodeMotion(DecodeBuff, m, Ind);
        return;
    }

    MotionCnt = (u32)m->Mb[Ind].MotionCnt;
    MotionCntFloat = m->Mb[Ind].MotionCnt;
    MotionCntFloatOnly = MotionCntFloat - MotionCnt;
    RotNum = ((ACT_HEADER *)m->Mb[Ind].pAct)->RotOutNum;
    TraNum = ((ACT_HEADER *)m->Mb[Ind].pAct)->TraOutNum;
    TraOut = *(u64 *)&pActHeader->TraOutFlag;
    Offset[0] = pActHeader->Xoffset;
    Offset[1] = pActHeader->Yoffset;
    Offset[2] = pActHeader->Zoffset;
    scale = pActHeader->Scale;
    RotI[3] = 0;
    TraI[3] = 1;
    pRot = (s16 *)((u32)m->Mb[Ind].pAct + pActHeader->RotAddrs + MotionCnt * RotNum * 6);
    pTra = (s16 *)((u32)m->Mb[Ind].pAct + pActHeader->TraAddrs + MotionCnt * TraNum * 6);
    pInf = (s16 *)(m->pInf + 0xC);
    pItr = (f32 *)m->pItr;

    if (m->Mb[Ind].StopFlag == 0 && m->Mb[Ind].MotionCnt + 1.0 >= m->Mb[Ind].MotionEndCnt) {
        SubMotionCnt = 0;
    } else {
        SubMotionCnt = MotionCnt + 1;
        pRot2 = (s16 *)((u32)m->Mb[Ind].pAct + pActHeader->RotAddrs + SubMotionCnt * RotNum * 6);
        pTra2 = (s16 *)((u32)m->Mb[Ind].pAct + pActHeader->TraAddrs + SubMotionCnt * TraNum * 6);
        pItr2 = (f32 *)m->pItr;
    }

    for (i = 0; i < m->CoordNum; TraOut >>= 1, i++, pInf++) {
        RotI[0] = *pRot++;
        RotI[1] = *pRot++;
        RotI[2] = *pRot++;
        sceVu0ITOF0Vector(RotF, RotI);
        sceVu0ScaleVector(RotF, RotF, 0.0001f);
        if (RotF[0] > 3.141592653589793)
            RotF[0] -= 6.283185;
        if (RotF[1] > 3.141592653589793)
            RotF[1] -= 6.283185;
        if (RotF[2] > 3.141592653589793)
            RotF[2] -= 6.283185;

        if (TraOut & 1) {
            TraI[0] = *pTra++;
            TraI[1] = *pTra++;
            TraI[2] = *pTra++;
            pItr += 4;
            sceVu0ITOF0Vector(TraF, TraI);
            TraF[0] *= scale;
            TraF[1] *= scale;
            TraF[2] *= scale;

            if (*pInf < 0) {
                TraF[0] += Offset[0];
                TraF[1] += Offset[1];
                TraF[2] += Offset[2];
            }
        } else {
            TraF[0] = *pItr++;
            TraF[1] = *pItr++;
            TraF[2] = *pItr++;
            TraF[3] = *pItr++;
        }

        if (SubMotionCnt != 0) {
            RotI[0] = *pRot2++;
            RotI[1] = *pRot2++;
            RotI[2] = *pRot2++;
            sceVu0ITOF0Vector(RotF2, RotI);
            sceVu0ScaleVector(RotF2, RotF2, 0.0001f);
            if (RotF2[0] > 3.141592653589793)
                RotF2[0] -= 6.283185;
            if (RotF2[1] > 3.141592653589793)
                RotF2[1] -= 6.283185;
            if (RotF2[2] > 3.141592653589793)
                RotF2[2] -= 6.283185;

            if (TraOut & 1) {
                TraI[0] = *pTra2++;
                TraI[1] = *pTra2++;
                TraI[2] = *pTra2++;
                pItr2 += 4;
                sceVu0ITOF0Vector(TraF2, TraI);
                TraF2[0] *= scale;
                TraF2[1] *= scale;
                TraF2[2] *= scale;

                if (*pInf < 0) {
                    TraF2[0] += Offset[0];
                    TraF2[1] += Offset[1];
                    TraF2[2] += Offset[2];
                }
            } else {
                TraF2[0] = *pItr2++;
                TraF2[1] = *pItr2++;
                TraF2[2] = *pItr2++;
                TraF2[3] = *pItr2++;
            }

            GetRotTransMatrixXYZ(TmpMtx0, RotF, TraF);
            GetRotTransMatrixXYZ(TmpMtx1, RotF2, TraF2);
            LinerInterPolateMatrix(DecodeBuff[i], TmpMtx0, TmpMtx1, MotionCntFloatOnly);
        } else {
            GetRotTransMatrixXYZ(DecodeBuff[i], RotF, TraF);
        }
    }
}

void ClearQwordMem(u32 Addrs, u32 Num) {
    __asm__ volatile(
        "sq $0, 0x0(%0)\n"
        "addi %0, %0, 0x10\n"
        "addi %1, %1, -1\n"
        "nop\n"
        "nop\n"
        "bne $0, %1, ClearQwordMem\n"
    : : "r" (Addrs), "r" (Num));
}

void AcxDecodeMotion(sceVu0FMATRIX *DecodeBuff, MOTION *m, s32 Ind) {
    // TODO
}