#include "iop.h"

static int transpitch[0];
JAMDATA JamData;
JAMDATA *jM;

u32 * GetFHMAddress(u32 *pAddr, s32 nNum) {
    return (pAddr[nNum + 1] >> 2) + pAddr;
}

int JamGetBdSize(int id) {
    JAMHD *pHD;
    CHK_HEAD *pHead;

    pHD = (JAMHD *)jM->hdAddr[id];
    pHead = &pHD->Header;
    return pHead->bodySize;
}

void JamGetPrm(int id, int prog, int splt, SNDKEYPRM *prm) {
    JAMHD *pHD;
	CHK_HEAD *pHead;
	CHK_PROG *pProg;
	CHK_SSET *pSSet;
	CHK_SMPL *pSmpl;
	CHK_VINFO *pVInfo;
	PROG_PRMBLK *pProgPrm;
	SPLT_PRMBLK *pSpltPrm;
	SSET_PRMBLK *pSSetPrm;
	SMPL_PRMBLK *pSmplPrm;
	VINFO_PRMBLK *pVInfoPrm;
	u32 addr;
	u32 *ptr;
	u16 *ptr2;
	s32 vol;

    // what
    pHD = (JAMHD *)jM->hdAddr[id];
    pHead = &pHD->Header;
    pProg = (CHK_PROG *)(jM->hdAddr[id] + pHead->programChunkAddr);
    ptr = &pProg->pointer;
    pProgPrm = (PROG_PRMBLK *)((u32)pProg + ptr[prog]);
    pSpltPrm = (SPLT_PRMBLK *)&pProgPrm->pointer;
    pSpltPrm = &pSpltPrm[splt];
    pSSet = (CHK_SSET *)(jM->hdAddr[id] + pHead->samplesetChunkAddr);
    ptr = &pSSet->pointer;
    pSSetPrm = (SSET_PRMBLK *)((u32)pSSet + ptr[pSpltPrm->sampleSetIndex]);
    ptr2 = &pSSetPrm->pointer;
    pSmpl = (CHK_SMPL *)(jM->hdAddr[id] + pHead->sampleChunkAddr);
    ptr = &pSmpl->pointer;
    pSmplPrm = (SMPL_PRMBLK *)((u32)pSmpl + ptr[pSSetPrm->pointer]);
    pVInfo = (CHK_VINFO *)(jM->hdAddr[id] + pHead->vagInfoChunkAddr);
    ptr = &pVInfo->pointer;
    pVInfoPrm = (VINFO_PRMBLK *)((u32)pVInfo + ptr[pSmplPrm->VagIndex]);
    
    prm->VmixL = pSmplPrm->sampleSpuAttr & 1;
    prm->VmixR = pSmplPrm->sampleSpuAttr >> 1 & 1;
    prm->VmixEL = pSmplPrm->sampleSpuAttr >> 2 & 1;
    prm->VmixER = pSmplPrm->sampleSpuAttr >> 3 & 1;
    prm->Core = pSmplPrm->sampleSpuAttr >> 5 & 1;
    addr = jM->bdAddr[id] + pVInfoPrm->vagOffsetAddr;
    prm->SSA = addr & 0x3FFFF8;
    prm->ADSR1 = pSmplPrm->sampleAdsr1;
    prm->ADSR2 = pSmplPrm->sampleAdsr2;
    
    if (pProgPrm->progPanpot * pSpltPrm->splitPanpot * pSmplPrm->samplePanpot < 0) {
        vol = (pProgPrm->progVolume * -0x4000 * pSpltPrm->splitVolume) / 0x4000;
    } else {
        vol = (pProgPrm->progVolume * 0x3FFF * pSpltPrm->splitVolume) / 0x4000;
    }
    vol = (vol * pSmplPrm->sampleVolume) / 0x80;

    prm->VOLL = (u16)vol & 0x7FFF;
    prm->VOLR = (u16)vol & 0x7FFF;
    prm->PITCH = ((transpitch[pSpltPrm->splitTranspose + 0xC] * pVInfoPrm->vagSampleRate) / 48000) & 0x3FFF;
    prm->BendRangeL = pSpltPrm->splitBendRangeLow;
    prm->BendRangeH = pSpltPrm->splitBendRangeHigh;
    prm->Loop = pVInfoPrm->vagAttribute;
}

void JamBankSet(int id) {
    if (id == 0) {
        jM->bdAddr[id] = Mem.sBankbd;
    } else {
        jM->bdAddr[id] = jM->bdAddr[id - 1] + JamGetBdSize(id - 1);
    }

    jM->hdAddr[id] = Mem.iBankhd + id * 0x8000;
    jM->transAddr = jM->bdAddr[id];
    jM->transSize = JamGetBdSize(id);
}

void JamBdTrans() {
    int size;

    if (jM->transSize < 0x10000) {
        size = jM->transSize;
    } else {
        size = 0x10000;
    }
    
    sD->DmaWait = 1;
    sceSdVoiceTrans(0, 0, Mem.iBankhd + 0x10000, jM->transAddr, size);
    jM->transSize -= size;
    jM->transAddr += size;
    while (sD->DmaWait != 0);
}

void JamInit() {
    jM = &JamData;
    jM->BankNum = 0;
}
