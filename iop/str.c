#include "iop.h"

STRDATA StrData;
STRDATA *pSTR;
BGMDATA BGMdata[2];
BGMDATA *pBGM;
BGMDATA *pBGM2;
PPTDATA PPTdata[4];

void MarkTop(int a, int s) {
    *(u8 *)(a + 0x1) = 6;
    *(u8 *)(a + 0x11) = 2;
    *(u8 *)(a + s - 0xF) = 2;
    FlushDcache();
}

void MarkBottom(int a, int s) {
    *(u8 *)(a + 0x1) = 2;
    *(u8 *)(a + 0x11) = 2;
    *(u8 *)(a + s - 0xF) = 3;
    FlushDcache();
}

static void * SpuInt() {
    iSignalSema(pSTR->Sem);
    return (void *)0x1; // ?
}

void StrDebug() {
	static u_char buff[512];

    sceSdVoiceTrans(1, 1, buff, BGMdata[0].spuAddr[0][0], 0x200);
    WaitSema(pSTR->Sem);
}

u_int StrKick() {
    int i;
	int st;

    sceSdSetSwitch(0x1601, 0xFF0000);
    sceSdSetCoreAttr(5, 0);
    sceSdSetIRQCallback(&SpuInt);
    
    for (st = 0; st < 2; st++) {
        for (i = 0; i < 2; i++) {
            MarkTop(pSTR->ZeroBuff, 0x100);
            sceSdVoiceTrans(1, 8, pSTR->ZeroBuff, BGMdata[st].spuAddr[i][0], 0x100);
            MarkBottom(pSTR->ZeroBuff, 0x100);
            sceSdVoiceTrans(1, 8, pSTR->ZeroBuff, BGMdata[st].spuAddr[i][1], 0x100);
        }
    }
    
    for (st = 0; st < 4; st++) {
        MarkTop(pSTR->ZeroBuff2, 0x80);
        sceSdVoiceTrans(1, 8, pSTR->ZeroBuff2, PPTdata[st].spuAddr[0], 0x80);
        MarkBottom(pSTR->ZeroBuff2, 0x80);
        sceSdVoiceTrans(1, 8, pSTR->ZeroBuff2, PPTdata[st].spuAddr[1], 0x80);
    }

    pSTR->spuID = 0;
    sceSdSetAddr(0x1F01, BGMdata[0].spuAddr[0][1]);
    sceSdSetCoreAttr(5,1);
    sceSdSetSwitch(0x1501, 0xFF0000);
}

void StrInfo(int *data) {
	STRINFO *info;
	STRINFO2 *info2;
	int i;

    info = (STRINFO *)data;

    if (pSTR->intCnt == pSTR->BGMchkcnt) {
        pSTR->BGMerror++;
    } else {
        pSTR->BGMerror = 0;
    }

    if (pSTR->BGMerror > 4) {
        StrKick();
        pSTR->BGMerror = 0;
    }

    pSTR->BGMchkcnt = pSTR->intCnt;
    pSTR->eeCnt = info->eeCnt;
    for (i = 0; i < 2; i++) {
        if (info->BGMok[i] != 0) {
            pSTR->BGMok[i] = 1;
            pSTR->BGMfinetop[i] = info->BGMfinetop[i];
            pSTR->BGMfine[i] = info->BGMfine[i];
        }
    }
    pSTR->BGMstereo = info->BGMstereo;
    pSTR->BGMbasevol = info->BGMvol;

    if ((info->Command & 2) != 0) {
        pSTR->BGMstat = 1;
    } else {
        pSTR->BGMstat = 0;
        if ((info->Command & 1) != 0) {
            pSTR->BGMpoint = 0;
            pSTR->BGMlength = info->BGMlength;
            pSTR->BGMskipsize = info->BGMskipsize;
            pBGM = &BGMdata[0];
            pBGM2 = &BGMdata[1];
            for (i = 0; i < 2; i++) {
                BGMdata[i].iopID = -1;
                BGMdata[i].iopIDnext = 0;
                BGMdata[i].iopOffset = 0;
            }
        } else {
            if ((info->Command & 0x20) != 0) {
                pSTR->BGMpoint = info->work0;
                pSTR->BGMlength = info->BGMlength;
                pSTR->BGMskipsize = info->BGMskipsize;
                pBGM = &BGMdata[0];
                pBGM2 = &BGMdata[1];
                for (i = 0; i < 2; i++) {
                    BGMdata[i].iopID = -1;
                    BGMdata[i].iopIDnext = 0;
                    BGMdata[i].iopOffset = 0;
                }
            }
        }
    }

    if ((info->Command & 4) != 0) {
        pSTR->BGMstat = 2;
    }
    if ((info->Command & 0x40) != 0) {
        pSTR->PPTstat = 1;
    } else {
        pSTR->PPTstat = 0;
    }
    if ((info->Command & 0x80) != 0) {
        pSTR->PPTstat = 2;
    }

    for (i = 0; i < 4; i++) {
        if (info->PPTstop[i] != 0) {
            PptStop(i);
        }
        PPTdata[i].Vol = info->PPTvol[i];
        if (info->PPTsize[i] != 0 && PPTdata[i].iopNext == -1) {
            PPTdata[i].iopID = -1;
            PPTdata[i].iopNext = 0;
            PPTdata[i].iopOffset = 0;
            PPTdata[i].iopSize = info->PPTsize[i];
        }
    }

    if ((info->Command & 8) != 0) {
        pBGM->Ch = info->BGMch;
        pSTR->BGMchmax = info->BGMchmax;
    }
    if ((info->Command & 0x10) != 0) {
        pSTR->BGMsp = 1;
        pSTR->BGMcnt = info->eeCnt;
        pSTR->BGMch2 = info->BGMch;
    }
    if ((info->Command & 0x100) != 0) {
        Ac3Play();
        pSTR->ac3Size = info->AC3size;
    }
    if ((info->Command & 0x200) != 0) {
        Ac3Stop();
        pSTR->ac3Size = 0;
    }
    if ((info->Command & 0x400) != 0) {
        Ac3ZeroPcmPlay();
    }
    if ((info->Command & 0x800) != 0) {
        Ac3ZeroPcmStop();
    }
    if ((info->Command & 0x1000) != 0) {
        Ac3SetDigitalOut();
    }
    if ((info->Command & 0x2000) != 0) {
        Ac3Play2();
        pSTR->ac3Size = info->AC3size;
    }

    info2 = (STRINFO2 *)&RpcInfo[2];
    info2->BGMpoint = pSTR->BGMpoint;
    info2->BGMnext = pBGM->iopIDnext;
    for (i = 0; i < 4; i++) {
        info2->PPTnext[i] = PPTdata[i].iopNext;
    }
    info2->AC3stat = sceSdBlockTransStatus(0, 0);
}

void StrCross() {
	BGMDATA *p;
    u32 cnt;

    cnt = pSTR->eeCnt - pSTR->BGMcnt;
    pBGM2->Ch = pSTR->BGMch2;
    pBGM2->iopOffset = pBGM->iopOffset;

    if (cnt >= 46) {
        pBGM2->Vol = (pSTR->BGMbasevol * (cnt - 45)) / 45;
    } else {
        pBGM2->Vol = 0;
    }
    pBGM->Vol = (pSTR->BGMbasevol * (90 - cnt)) / 90;

    if (pSTR->BGMstat == 1) {
        pSTR->BGMcnt++;
    }

    if (cnt > 89) {
        pSTR->BGMsp = 0;
        pBGM2->iopID = pBGM->iopID;
        pBGM2->iopIDnext = pBGM->iopIDnext;
        pBGM->Vol = 0;
        pBGM->iopID = -1;
        p = pBGM;
        pBGM = pBGM2;
        pBGM2 = p;
    }
}

int StrMain(int status) {
	BGMDATA *bgm;
	PPTDATA *ppt;
	int st;
	int ch;
	int addr;

    while (1) {
        WaitSema(pSTR->Sem);
        sceSdSetCoreAttr(5, 0);
        
        pSTR->intCnt++;
        switch (pSTR->BGMsp) {
            case 1:
                StrCross();
                break;
            default:
                pBGM->Vol = pSTR->BGMbasevol;
                break;
        }

        switch (pSTR->BGMstat) {
            case BGM_PLAY:
                break;
            case BGM_PAUSE:
                break;
            case BGM_RESET:
                pSTR->BGMstat = 0;
                pSTR->BGMsp = 0;
                pSTR->BGMok[0] = 0;
                pSTR->BGMok[1] = 0;
                pSTR->BGMpoint = 0;
                pBGM = &BGMdata[0];
                pBGM2 = &BGMdata[1];
                for (st = 0; st < 2; st++) {
                    BGMdata[st].iopID = -1;
                    BGMdata[st].iopIDnext = 0;
                    BGMdata[st].iopOffset = 0;
                }
                pBGM->Ch = 0;
                break;
        }

        bgm = pBGM;
        for (st = 0; st < 2; st++) {
            if (st == 0) {
                if (bgm->iopID == -1 || bgm->iopOffset * pSTR->BGMchmax * 2 > 0x7FFFF) {
                    if (pSTR->BGMok[bgm->iopIDnext] == 0) {
                        bgm->iopID = -1;
                    } else {
                        bgm->iopID = bgm->iopIDnext;
                        bgm->iopIDnext = 1 - bgm->iopIDnext;
                        bgm->iopOffset = 0;
                    }
                }
                if (pSTR->BGMsp == 1) {
                  pBGM2->iopID = pBGM->iopID;
                  pBGM2->iopIDnext = pBGM->iopIDnext;
                  pBGM2->iopOffset = pBGM->iopOffset;
                }
            }
            
            for (ch = 0; ch < 2; ch++) {
                if (bgm->iopID == -1 || pSTR->BGMstat == 1) {
                    addr = pSTR->ZeroBuff;
                } else {
                    switch (pSTR->BGMsp) {
                        case 0:
                            addr = bgm->iopOffset / 0x800 * 0x1000 * pSTR->BGMchmax;
                            addr += (bgm->Ch * 2 + ch) * 0x800;
                            addr += bgm->iopOffset % 0x800;
                            addr += pSTR->BGMaddr[bgm->iopID];
                            break;
                        case 1:
                            addr = bgm->iopOffset / 0x800 * 0x1000 * pSTR->BGMchmax;
                            addr += (bgm->Ch * 2 + ch) * 0x800;
                            addr += bgm->iopOffset % 0x800;
                            addr += pSTR->BGMaddr[pBGM->iopID];
                            break;
                        default:
                            break;
                    }
                }

                if (pSTR->spuID == 0) {
                    MarkTop(addr, 0x100);
                } else {
                    MarkBottom(addr, 0x100);
                }

                if (addr != pSTR->ZeroBuff || bgm->spuZero[ch][pSTR->spuID] != 1) {
                    if (addr == pSTR->ZeroBuff) {
                        bgm->spuZero[ch][pSTR->spuID] = 1;
                    } else {
                        bgm->spuZero[ch][pSTR->spuID] = 0;
                    }
                    sceSdVoiceTrans(1, 8, addr, bgm->spuAddr[ch][pSTR->spuID], 0x100);
                }
            }
            
            if (bgm->iopID != -1) {
                pSTR->BGMok[bgm->iopID] = 0;
            }
            
            if (bgm->iopID != -1 && pSTR->BGMstat == 0) {
                if (st == 0) {
                    pSTR->BGMpoint += 0x100;
                }
                bgm->iopOffset += 0x100;
                if (pSTR->BGMpoint >= pSTR->BGMlength) {
                    pSTR->BGMpoint = pSTR->BGMskipsize;
                    bgm->iopOffset = (bgm->iopOffset + 0x7FF) / 0x800 * 0x800;
                }
                if (bgm->iopOffset == pSTR->BGMfinetop[bgm->iopID]) {
                    bgm->iopOffset += pSTR->BGMfine[bgm->iopID];
                }
            }

            if (pSTR->BGMstat == 1) {
                sceSdSetParam(bgm->Vnum[0] << 1 | 1, 0);
                sceSdSetParam(bgm->Vnum[0] << 1 | 0x101, 0);
                sceSdSetParam(bgm->Vnum[1] << 1 | 1, 0);
                sceSdSetParam(bgm->Vnum[1] << 1 | 0x101, 0);
                bgm->VolBak = 0;
            } else if (bgm->Vol != bgm->VolBak) {
                if (pSTR->BGMstereo != 0) {
                    sceSdSetParam(bgm->Vnum[0] << 1 | 1, bgm->Vol);
                    sceSdSetParam(bgm->Vnum[0] << 1 | 0x101, 0);
                    sceSdSetParam(bgm->Vnum[1] << 1 | 1, 0);
                    sceSdSetParam(bgm->Vnum[1] << 1 | 0x101, bgm->Vol);
                    bgm->VolBak = bgm->Vol;
                } else {
                    sceSdSetParam(bgm->Vnum[0] << 1 | 1, bgm->Vol);
                    sceSdSetParam(bgm->Vnum[0] << 1 | 0x101, bgm->Vol);
                    sceSdSetParam(bgm->Vnum[1] << 1 | 1, bgm->Vol);
                    sceSdSetParam(bgm->Vnum[1] << 1 | 0x101, bgm->Vol);
                    bgm->VolBak = bgm->Vol;
                }
            }
            bgm = pBGM2;
        }

        switch (pSTR->PPTstat) {
            case PPT_PLAY:
                break;
            case PPT_PAUSE:
                break;
            case PPT_RESET:
                pSTR->PPTstat = 0;
                for (st = 0; st < 4; st++) {
                    PPTdata[st].iopID = -1;
                    PPTdata[st].iopNext = -1;
                    PPTdata[st].iopOffset = 0;
                }
                break;
        }

        for (st = 0; st < 4; st++) {
            ppt = &PPTdata[st];
            
            if (ppt->iopNext == -1) {
                ppt->iopID = -1;
            } else if (ppt->iopID == -1 || (ppt->iopOffset & 0x3FFF) == 0) {
                ppt->iopID = ppt->iopNext;
                ppt->iopNext = 1 - ppt->iopNext;
            }
            
            if (ppt->iopID == -1 || pSTR->PPTstat == 1) {
                addr = pSTR->ZeroBuff2;
            } else {
                addr = ppt->iopOffset % 0x4000 + ppt->iopAddr[ppt->iopID];
            }

            if (pSTR->spuID == 0) {
                MarkTop(addr, 0x80);
            } else {
                MarkBottom(addr, 0x80);
            }

            if (addr != pSTR->ZeroBuff2 || ppt->spuZero[pSTR->spuID] != 1) {
                if (addr == pSTR->ZeroBuff2) {
                    ppt->spuZero[pSTR->spuID] = 1;
                } else {
                    ppt->spuZero[pSTR->spuID] = 0;
                }
                sceSdVoiceTrans(1, 8, addr, ppt->spuAddr[pSTR->spuID], 0x80);
            }

            if (pSTR->PPTstat != 1 && ppt->iopNext != -1) {
                ppt->iopOffset += 0x80;
                if (ppt->iopOffset >= ppt->iopSize ) {
                    ppt->iopNext = -1;
                }
            }

            if (pSTR->PPTstat == 1) {
                sceSdSetParam(ppt->Vnum << 1 | 1, 0);
                sceSdSetParam(ppt->Vnum << 1 | 0x101, 0);
                ppt->VolBak = 0;
            } else if (ppt->Vol != ppt->VolBak) {
                sceSdSetParam(ppt->Vnum << 1 | 1, ppt->Vol);
                sceSdSetParam(ppt->Vnum << 1 | 0x101, ppt->Vol);
                ppt->VolBak = ppt->Vol;
            }
        }

        sceSdSetAddr(0x1F01, BGMdata[0].spuAddr[0][pSTR->spuID]);
        pSTR->spuID = 1 - pSTR->spuID;
        sceSdSetCoreAttr(5, 1);
    }

    return 0;
}

int StrThread(int status) {
    StrMain(status);
    return 0;
}

void StrInit() {
	struct SemaParam sem;
	struct ThreadParam param;
	int i;
	int st;
	char *c;

    pSTR = &StrData;
    pBGM = &BGMdata[0];
    pBGM2 = &BGMdata[1];
    pSTR->intCnt = 0;
    pSTR->spuID = 0;
    pSTR->ZeroBuff = Mem.iStream;
    pSTR->ZeroBuff2 = pSTR->ZeroBuff + 0x100;
    pSTR->BGMaddr[0] = pSTR->ZeroBuff2 + 0x80;
    pSTR->BGMaddr[1] = pSTR->BGMaddr[0] + 0x80000;
    pSTR->BGMstat = 0;
    pSTR->BGMsp = 0;
    pSTR->BGMchmax = 2;
    pSTR->BGMbasevol = 0;
    pSTR->BGMerror = 0;
    
    memset((void *)pSTR->ZeroBuff, 0, 0x100);
    c = (char *)pSTR->ZeroBuff;
    for (i = 0; i < 0x10; i++) {
        c[0] = 12;
        c[1] = 2;
        c += 0x10;
    }
    
    memset((void *)pSTR->ZeroBuff2, 0, 0x80);
    c = (char *)pSTR->ZeroBuff2;
    for (i = 0; i < 8; i++) {
        c[0] = 12;
        c[1] = 2;
        c += 0x10;
    }

    sem.initCount = 0;
    sem.maxCount = 1;
    sem.attr = 0;
    pSTR->Sem = CreateSema(&sem);

    param.attr = 0x2000000;
    param.entry = &StrMain;
    param.initPriority = 29;
    param.stackSize = 0x800;
    param.option = 0;
    pSTR->ThID = CreateThread(&param);
    
    sceSdSetIRQCallback(&SpuInt);
    SignalSema(pSTR->Sem);

    for (st = 0; st < 2; st++) {
        BGMdata[st].iopOffset = 0;
        BGMdata[st].Vol = 0;
        BGMdata[st].VolBak = 0;
        BGMdata[st].Ch = 0;
        BGMdata[st].iopID = -1;
        BGMdata[st].iopIDnext = 0;
        for (i = 0; i < 2; i++) {
            BGMdata[st].Vnum[i] = 0x14 + st * 2 + i;
            BGMdata[st].spuAddr[i][0] = 0x5010 + (st * 4 + i * 2) * 0x100;
            BGMdata[st].spuAddr[i][1] =  0x5010 + (st * 4 + i * 2 + 1) * 0x100;
            BGMdata[st].spuZero[i][0] = 1;
            BGMdata[st].spuZero[i][1] = 1;
            sceSdSetParam(BGMdata[st].Vnum[i] << 1 | 0x301, 0xff);
            sceSdSetParam(BGMdata[st].Vnum[i] << 1 | 0x401, 0x1fc0);
            sceSdSetParam(BGMdata[st].Vnum[i] << 1 | 0x201, 0xeb4);
            sceSdSetAddr(BGMdata[st].Vnum[i] << 1 | 0x2041, BGMdata[st].spuAddr[i][0]);
        }
        sceSdSetParam(BGMdata[st].Vnum[0] << 1 | 1, BGMdata[st].Vol);
        sceSdSetParam(BGMdata[st].Vnum[0] << 1 | 0x101, 0);
        sceSdSetParam(BGMdata[st].Vnum[1] << 1 | 1, 0);
        sceSdSetParam(BGMdata[st].Vnum[1] << 1 | 0x101, BGMdata[st].Vol);
    }

    for (st = 0; st < 4; st++) {
        PPTdata[st].Vol = 0;
        PPTdata[st].VolBak = 0;
        PPTdata[st].iopAddr[0] = 0x100180 + Mem.iStream + st * 0x8000;
        PPTdata[st].iopAddr[1] = 0x100180 + Mem.iStream + (st * 2 + 1) * 0x4000;
        PPTdata[st].iopID = -1;
        PPTdata[st].iopNext = -1;
        PPTdata[st].iopOffset = 0;
        PPTdata[st].Vnum = st + 0x10;
        PPTdata[st].spuAddr[0] = st * 0x100 + 0x5810;
        PPTdata[st].spuAddr[1] = (st * 2 + 1) * 0x80 + 0x5810;
        PPTdata[st].spuZero[0] = 1;
        PPTdata[st].spuZero[1] = 1;
        sceSdSetParam(PPTdata[st].Vnum << 1 | 0x301, 0xff);
        sceSdSetParam(PPTdata[st].Vnum << 1 | 0x401, 0x1fc0);
        sceSdSetParam(PPTdata[st].Vnum << 1 | 0x201, 0x75a);
        sceSdSetAddr(PPTdata[st].Vnum << 1 | 0x2041, PPTdata[st].spuAddr[0]);
        sceSdSetParam(PPTdata[st].Vnum << 1 | 1,  PPTdata[st].Vol);
        sceSdSetParam(PPTdata[st].Vnum << 1 | 0x101, PPTdata[st].Vol);
    }

    pSTR->ac3Buff = Mem.iStream + 0x120180;
    pSTR->ac3Size = 0;
    Ac3Clear();
    sceSdSetCoreAttr(5,0);

    for (st = 0; st < 2; st++) {
        for (i = 0; i < 2; i++) {
            MarkTop(pSTR->ZeroBuff, 0x100);
            pSTR->DmaWait = 1;
            sceSdVoiceTrans(1, 8, (void *)pSTR->ZeroBuff, BGMdata[st].spuAddr[i][0], 0x100);
            MarkBottom(pSTR->ZeroBuff, 0x100);
            pSTR->DmaWait = 1;
            sceSdVoiceTrans(1, 8, (void *)pSTR->ZeroBuff, BGMdata[st].spuAddr[i][1], 0x100);
        }
    }

    for (st = 0; st < 4; st++) {
        MarkTop(pSTR->ZeroBuff2, 0x80);
        pSTR->DmaWait = 1;
        sceSdVoiceTrans(1, 8, (void *)pSTR->ZeroBuff2, PPTdata[st].spuAddr[0], 0x80);
        MarkBottom(pSTR->ZeroBuff2,0 x80);
        pSTR->DmaWait = 1;
        sceSdVoiceTrans(1, 8, (void *)pSTR->ZeroBuff2, PPTdata[st].spuAddr[1], 0x80);
    }

    sceSdSetAddr(0x1f01,BGMdata[0].spuAddr[0][1]);
    sceSdSetCoreAttr(5,1);
    sceSdSetSwitch(0x1501, 0xFF0000);
    StartThread(pSTR->ThID,0);
    RpcRet[0] = pSTR->BGMaddr[0];
}

void PptStop(int st) {
    PPTdata[st].iopID = -1;
    PPTdata[st].iopNext = -1;
    PPTdata[st].iopOffset = 0;
}

void Ac3Clear() {
    memset(pSTR->ac3Buff, 0, 0x10000);
    sceSdBlockTrans(0, 0, pSTR->ac3Buff, 0x10000);
}

void Ac3Play() {
    // Empty function
}

void Ac3ZeroPcmPlay() {
    memset(pSTR->ac3Buff, 0, 0x10000);
    sceSdBlockTrans(0, 0x10, pSTR->ac3Buff, 0x10000);
}

void Ac3ZeroPcmStop() {
    sceSdBlockTrans(0, 2, NULL, 0);
}

void Ac3SetDigitalOut() {
    sceSdSetParam(0x800, 0xFC0);
    sceSdSetCoreAttr(10, 0x801);
}

void Ac3Play2() {
    sceSdBlockTrans(0, 0x10, pSTR->ac3Buff, 0x10000);
}

void Ac3Stop() {
    sceSdBlockTrans(0, 2, NULL, 0);
    Ac3Clear();
    sceSdSetCoreAttr(10,0x800);
}
