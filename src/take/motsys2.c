#include "motsys2.h"
#include "motsys.h"
#include "motip.h"

void GetSfxWorldMatrix(SFXOBJ *pObj) {
    s32 i;
    s32 ii;
    s32 Ind;
    f32 BaseFrameWeight;
    s32 MbStackIndex[6];
    s32 MbStack[6][4];
    MOTION *m;
    s32 BaseIndex;
    s32 IpIndex;

    m = pObj->pMot;
    IpIndex = 0;
    for (i = 0; i < 6; i++) {
        MbStackIndex[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        MbStack[m->Mb[i].Type][MbStackIndex[m->Mb[i].Type]] = i;
        MbStackIndex[m->Mb[i].Type]++;
    }
    BaseIndex = MbStack[1][0];
    m->BaseIndex = BaseIndex;
    BaseFrameWeight = m->Mb[BaseIndex].MotionCnt / m->Mb[BaseIndex].MotionEndCnt;
    if (MbStackIndex[2] != 0) {
        f32 yeet; // idk how to match this without adding a variable but it works
        m->IpIndex = IpIndex = MbStack[2][0];
        yeet = m->Mb[IpIndex].MotionEndCnt;
        if (m->Mb[BaseIndex].SyncFlag != 0 && m->Mb[BaseIndex].SyncFlag == m->Mb[IpIndex].SyncFlag) {
            m->Mb[IpIndex].MotionCnt = BaseFrameWeight * yeet;
        }
    }

    DecodeMotion(DECADDR0, m, BaseIndex);
    if (MbStackIndex[2] != 0) {
        DecodeMotion(DECADDR1, m, IpIndex);
        if (MbStackIndex[2] != 0) {
            for (i = 0; i < m->CoordNum; i++) {
                LinerInterPolateMatrix(DECADDR0[i], DECADDR0[i], DECADDR1[i], m->Mb[IpIndex].Weight);
            }
        }
    }

    m->Mb[BaseIndex].MotionCnt += m->Mb[BaseIndex].CntSpeed;
    if (MbStackIndex[2] != 0) {
        if (m->Mb[BaseIndex].CntSpeed > 0.0f) {
            m->Mb[IpIndex].MotionCnt += m->Mb[IpIndex].CntSpeed;
            m->Mb[IpIndex].Weight -= m->Mb[IpIndex].Weight / m->Mb[IpIndex].OutCnt;
            m->Mb[IpIndex].OutCnt -= m->Mb[BaseIndex].CntSpeed;
        } else if (m->Mb[BaseIndex].CntSpeed < 0.0f) {
            m->Mb[IpIndex].MotionCnt += m->Mb[IpIndex].CntSpeed;
            m->Mb[IpIndex].Weight -= m->Mb[IpIndex].Weight / m->Mb[IpIndex].OutCnt;
            m->Mb[IpIndex].OutCnt += m->Mb[BaseIndex].CntSpeed;
        }

        if (m->Mb[IpIndex].OutCnt <= 0.0f) {
            m->Mb[IpIndex].Type = 0;
            for (i = 0; i < MbStackIndex[4]; i++) {
                m->Mb[MbStack[4][i]].Type = 0;
            }
        }
    }

    for (i = 1; i < 5; i++) {
        for (ii = 0; ii < MbStackIndex[i]; ii++) {
            Ind = MbStack[i][ii];
            m->EndFlag = 0;
            if (m->Mb[Ind].StopFlag != 0) {
                if (m->Mb[Ind].MotionCnt >= m->Mb[Ind].MotionEndCnt) {
                    m->Mb[Ind].MotionCnt = m->Mb[Ind].MotionEndCnt;
                    if (m->Mb[Ind].Type == 1)
                        m->EndFlag = 1;
                }
                if (m->Mb[Ind].MotionCnt < 0.0f)
                    m->Mb[Ind].MotionCnt = 0.0f;
            } else {
                if (m->Mb[Ind].MotionCnt >= m->Mb[Ind].MotionEndCnt) {
                    if (m->Mb[Ind].Type == 1)
                        m->EndFlag = 2;
                    do
                        m->Mb[Ind].MotionCnt -= m->Mb[Ind].MotionEndCnt;
                    while (!(m->Mb[Ind].MotionEndCnt > m->Mb[Ind].MotionCnt));
                }

                if (m->Mb[Ind].MotionCnt < 0.0f) {
                    if (m->Mb[Ind].Type == 1)
                        m->EndFlag = 2;
                    do
                        m->Mb[Ind].MotionCnt = m->Mb[Ind].MotionEndCnt + m->Mb[Ind].MotionCnt;
                    while (!(0 <= m->Mb[Ind].MotionCnt));
                }
            }
        }
    }

    if (MbStackIndex[5] != 0) {
        WorldIpMotion(pObj);
    }

    for (i = 0; i < m->CoordNum; i++) {
        sceVu0CopyMatrix(m->pCoord[i].Mtx, DECADDR0[i]);
        m->pCoord[i].Flag = 0;
    }

    for (i = 0; i < m->CoordNum; i++) {
        GetLwMtx(&m->pCoord[i]);
    }

    if (m->pBaseCoord->Flag > -1) {
        GetRotTransMatrixXYZ(m->pBaseCoord->Mtx, m->pBaseCoord->Rot, m->pBaseCoord->Trans);
    }

    for (i = 0; i < m->CoordNum; i++) {
        sceVu0MulMatrix(m->pCoord[i].Mtx, m->pBaseCoord->Mtx, m->pCoord[i].Mtx);
    }
}

void SetActCnt(SFXOBJ *pObj, f32 Cnt) {
    SFXOBJ *pObjTmp;
    s32 Ind;
    MOTION *m;

    pObjTmp = GetActiveSfx(pObj);
    if (pObjTmp == NULL)
        return;

    Ind = pObjTmp->pMot->BaseIndex;
    m = pObjTmp->pMot;
    m->Mb[Ind].MotionCnt = Cnt;
    if (m->Mb[Ind].StopFlag != 0) {
        if (m->Mb[Ind].MotionEndCnt <= Cnt) {
            m->Mb[Ind].MotionCnt = m->Mb[Ind].MotionEndCnt;
            if (m->Mb[Ind].Type == 1) {
                m->EndFlag = 1;
            }
        }
        if (m->Mb[Ind].MotionCnt < 0.0f) {
            m->Mb[Ind].MotionCnt = 0.0f;
        }
    } else {
        if (m->Mb[Ind].MotionCnt >= m->Mb[Ind].MotionEndCnt) {
            if (m->Mb[Ind].Type == 1)
                m->EndFlag = 2;
            do
                m->Mb[Ind].MotionCnt -= m->Mb[Ind].MotionEndCnt;
            while (!(m->Mb[Ind].MotionEndCnt > m->Mb[Ind].MotionCnt));
        }

        if (m->Mb[Ind].MotionCnt < 0.0f) {
            if (m->Mb[Ind].Type == 1)
                m->EndFlag = 2;
            do
                m->Mb[Ind].MotionCnt = m->Mb[Ind].MotionEndCnt + m->Mb[Ind].MotionCnt;
            while (!(0 <= m->Mb[Ind].MotionCnt));
        }
    }
}

void SetActSpeed(SFXOBJ *pObj, f32 Speed) {
    pObj->pMot->Mb[pObj->pMot->BaseIndex].CntSpeed = Speed;
}

void WorldIpMotion(SFXOBJ *pObj) {
    s32 i;
    s32 Ind;
    s32 total;
    MOTION *m;
    sceVu0FMATRIX TmpMtx;

    m = pObj->pMot;
    for (i = 0; i < 4; i++) {
        if (m->Mb[i].Type == 5) {
            Ind = i;
            break;
        }
    }

    for (i = 0; i < m->CoordNum; i++) {
        sceVu0CopyMatrix(m->pCoord[i].Mtx, DECADDR0[i]);
        m->pCoord[i].Flag = 0;
    }

    for (i = 0; i < m->CoordNum; i++) {
        GetLwMtx(&m->pCoord[i]);
    }

    if (m->pBaseCoord->Flag >= 0) {
        GetRotTransMatrixXYZ(m->pBaseCoord->Mtx, m->pBaseCoord->Rot, m->pBaseCoord->Trans);
    }

    for (i = 0; i < m->CoordNum; i++) {
        sceVu0MulMatrix(m->pCoord[i].Mtx, m->pBaseCoord->Mtx, m->pCoord[i].Mtx);
    }

    total = 0;
    for (i = 0; i < m->CoordNum; i++) {
        if ((m->Mb[Ind].OnFlag >> i) & 1 && m->pCoord[i].WipCnt > 0.0f) {
            InterPolateMatrix(m->pCoord[i].Mtx, m->pCoord[i].MtxSav, m->pCoord[i].Mtx, 1.0f / m->pCoord[i].WipCnt);
            sceVu0CopyMatrix(m->pCoord[i].MtxSav, m->pCoord[i].Mtx);
            m->pCoord[i].WipCnt -= m->Mb[m->BaseIndex].CntSpeed;
            if (m->pCoord[i].WipCnt <= 0.0f) {
                m->pCoord[i].WipCnt = 0.0f;
            }
            total++;
        }
    }

    if (total == 0) {
        m->Mb[Ind].Type = 0;
    } else {
        for (i = 0; i < m->CoordNum; i++) {
            if (m->pCoord[i].Super != NULL) {
                ChangeLocalMatrix(TmpMtx, m->pCoord[i].Super->Mtx, m->pCoord[i].Mtx);
                CopyRotMatrix(DECADDR0[i], TmpMtx);
            } else {
                ChangeLocalMatrix(TmpMtx, m->pBaseCoord->Mtx, m->pCoord[i].Mtx);
                CopyRotMatrix(DECADDR0[i], TmpMtx);
            }
        }
    }
}