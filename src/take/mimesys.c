#include "mimesys.h"
#include "sfxbios.h"

void MimeWork(SFXOBJ *pObj) {
    VoiceMime(pObj);
    SetSyncMime(pObj);
    if (pObj->pMime->IdFlag == 0)
        MimeOff(pObj);
    pObj->pMime->IdFlag = 0;
}

void MimeSetWork(SFXOBJ *pObj) {
    // Empty function
}

void MimeTblSet(SFXOBJ *pObj, s16 *TblAdrs) {
    // Empty function
}

u64 MimeIdCheck(SFXOBJ *pObj, s32 StartNum, s32 EndNum) {
    s32 i;
    TYPE_SFZ_TBL *pSfzParts0;
    TYPE_SFZ_HEADER *pSfz0;
    TYPE_SFZ_HEADER *pSfz1;
    u64 flag;
    s32 id;

    flag = 0;
    if (pObj->MimeAdrs == NULL)
        return 0;

    pSfz0 = (TYPE_SFZ_HEADER *)Fadr(pObj->MimeAdrs, StartNum);
    pSfz1 = (TYPE_SFZ_HEADER *)Fadr(pObj->MimeAdrs, EndNum);

    if (*(u64 *)pSfz0 == 0 && *(u64*)pSfz1 == 0) {
        return flag;
    }
    if (*(u64 *)pSfz0 != 0 && *(u64*)pSfz1 == 0) {
        pSfz1 = pSfz0;
    }
    if (*(u64*)pSfz1 != 0 && *(u64 *)pSfz0 == 0) {
        pSfz0 = pSfz1;
    }

    pSfzParts0 = (TYPE_SFZ_TBL *)(pSfz0 + 1);
    i = 0;
    while (i < pSfz0->parts_num) {
        id = pSfzParts0->parts_id;
        flag |= 1 << id;
        i++;
        pSfzParts0++;
    }
    return flag;
}

void MimeOff(SFXOBJ *pObj) {
    s32 i;

    for (i = 0; i < pObj->PartsNum; i++) {
        if (pObj->pParts[i].type == 3) {
            pObj->pParts[i].type = 1;
        }
    }
}

void SetVoiceMime(SFXOBJ *pObj, s16 *pVmime) {
    if (pObj->pMime != NULL) {
        pObj->pMime->pVmime = pVmime;
        pObj->pMime->VmimeInd = 0;
        pObj->pMime->VmimeCnt = 0.0f;
        pObj->pMime->VmimeCntSpeed = 1.0f; 
    }
}

void EraseVoiceMime(SFXOBJ *pObj) {
    if (pObj->pMime != NULL) {
        pObj->pMime->pVmime = NULL;
        pObj->pMime->VmimeInd = 0;
        pObj->pMime->VmimeCnt = 0.0f;
        pObj->pMime->VmimeCntSpeed = 1.0f; 
    }
}

void VoiceMime(SFXOBJ *pObj) {
    s16 LastInd;
    s16 *pVmime;
    f32 MaxCnt;
    s32 a;
    s32 b;

    if (pObj->pMime->pVmime == NULL || pObj->pMime == NULL)
        return;

    LastInd = pObj->pMime->pVmime[0];
    pVmime = pObj->pMime->pVmime + pObj->pMime->VmimeInd * 2 + 2;
    MaxCnt = pVmime[0];
    a = pVmime[1];
    if (pObj->pMime->VmimeInd + 1 == LastInd)
        b = a;
    else
        b = pVmime[3];
    MimeSet(pObj, a, b, 1.0f - (MaxCnt - pObj->pMime->VmimeCnt) / MaxCnt);
    pObj->pMime->VmimeCnt += pObj->pMime->VmimeCntSpeed;
    if (MaxCnt <= pObj->pMime->VmimeCnt) {
        pObj->pMime->VmimeInd++;
        pObj->pMime->VmimeCnt = MaxCnt - pObj->pMime->VmimeCnt;
        if (LastInd <= pObj->pMime->VmimeInd)
            pObj->pMime->pVmime = NULL;
    }
}

s32 SetSyncMime(SFXOBJ *pObj) {
    s32 ii;
    u8 *pMime;
    ACT_HEADER *pActHeader;
    s16 MimeNum;
    s32 MotionCnt;
    MOTION *m;

    m = pObj->pMot;
    MotionCnt = (u32)m->Mb[m->BaseIndex].MotionCnt;
    pActHeader = (ACT_HEADER *)m->Mb[m->BaseIndex].pAct;
    if (pActHeader->MimeAddrs == 0)
        return;
    pMime = (u8 *)pActHeader + pActHeader->MimeAddrs;
    
    MimeNum = *pMime;
    pMime += MotionCnt * MimeNum * 3 + 1;
    for (ii = 0; ii < MimeNum; ii++) {
        MimeSet(pObj, *pMime++, *pMime++, *pMime++ / 255.0f);
    }
}

void MimeSet(SFXOBJ *pObj, s32 StartNum, s32 EndNum, f32 Weight) {
    s32 i;
    SFXOBJ *pObjTmp;
    TYPE_SFZ_TBL *pSfzParts0;
    TYPE_SFZ_TBL *pSfzParts1;
    TYPE_SFZ_HEADER *pSfz0;
    TYPE_SFZ_HEADER *pSfz1;
    s32 id;
    u64 IdFlag;

    pSfz0 = (TYPE_SFZ_HEADER *)Fadr(pObj->MimeAdrs, StartNum);
    pSfz1 = (TYPE_SFZ_HEADER *)Fadr(pObj->MimeAdrs, EndNum);
    
    if (*(u64 *)pSfz0 == 0) {
        if (*(u64*)pSfz1 == 0) {
            return;
        }
    } else {
        if (*(u64*)pSfz1 == 0) {
            pSfz1 = pSfz0;
        }
    }
    if (*(u64*)pSfz1 != 0 && *(u64 *)pSfz0 == 0) {
        pSfz0 = pSfz1;
    }

    pSfzParts0 = (TYPE_SFZ_TBL *)(pSfz0 + 1);
    pSfzParts1 = (TYPE_SFZ_TBL *)(pSfz1 + 1);

    IdFlag = 0;
    i = 0;
    while (i < pSfz0->parts_num) {
        id = pSfzParts0->parts_id;
        IdFlag |= 1 << id;
        i++;
        pSfzParts0++;
    }

    if ((IdFlag & pObj->pMime->IdFlag) == 0) {
        pObj->pMime->IdFlag |= IdFlag;
        pObjTmp = pObj;
        while (pObjTmp != NULL) {
            if ((pObj->Condition & pObjTmp->Flag) != 0) {
                if (pObjTmp->MimeAdrs == NULL)
                    return;
                pSfz0 = (TYPE_SFZ_HEADER *)Fadr(pObjTmp->MimeAdrs, StartNum);
                pSfz1 = (TYPE_SFZ_HEADER *)Fadr(pObjTmp->MimeAdrs, EndNum);
                
                if (*(u64 *)pSfz0 == 0) {
                    if (*(u64*)pSfz1 == 0) {
                        return;
                    }
                } else {
                    if (*(u64*)pSfz1 == 0) {
                        pSfz1 = pSfz0;
                    }
                }
                if (*(u64*)pSfz1 != 0 && *(u64 *)pSfz0 == 0) {
                    pSfz0 = pSfz1;
                }

                pSfzParts0 = (TYPE_SFZ_TBL *)(pSfz0 + 1);
                pSfzParts1 = (TYPE_SFZ_TBL *)(pSfz1 + 1);
                for (i = 0; i < pSfz0->parts_num; i++, pSfzParts0++, pSfzParts1++) {
                    id = pSfzParts0->parts_id;
                    pObjTmp->pParts[id].type = 3;
                    pObjTmp->pParts[id].vert_adrs_mime0 = (int *)((int)pSfz0 + (int)pSfzParts0->vert_adrs);
                    pObjTmp->pParts[id].norm_adrs_mime0 = (int *)((int)pSfz0 + (int)pSfzParts0->norm_adrs);
                    pObjTmp->pParts[id].MimeWeight = Weight;
                    id = pSfzParts1->parts_id;
                    pObjTmp->pParts[id].vert_adrs_mime1 = (int *)((int)pSfz1 + (int)pSfzParts1->vert_adrs);
                    pObjTmp->pParts[id].norm_adrs_mime1 = (int *)((int)pSfz1 + (int)pSfzParts1->norm_adrs);
                }
            }
            pObjTmp = pObjTmp->pObjSub;
        }
        
    }
}
