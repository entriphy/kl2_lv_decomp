#include "common.h"

KLTABLE *KlTable = NULL; // ?
PPTTABLE *PptTable = NULL;
BGMTABLE *BgmTable = NULL;

hBGMDATA BgmData = {};
hBGMDATA* bD = NULL;
hPPTDATA PptData;
hPPTDATA* pD;
hAC3DATA Ac3Data = {};
hAC3DATA* aD = NULL;

#pragma region BGM

s32 hBgmGetPlaySize() {
    return BgmTable[bD->bgmNo].chsize;
}

f32 hBgmGetDefVol(BGM no) {
    s32 vol;

    switch (no) {
        case BGM000:
            vol = 75;
            break;
        case BGM001:
            vol = 75;
            break;
        case BGM003:
            vol = 97;
            break;
        case BGM004:
            vol = 107;
            break;
        case BGM005:
            vol = 90;
            break;
        case BGM006:
            vol = 77;
            break;
        case BGM007:
            vol = 70;
            break;
        case BGM008:
            vol = 80;
            break;
        case BGM009:
            vol = 95;
            break;
        case BGM010:
            vol = 127;
            break;
        case BGM012:
            vol = 100;
            break;
        case BGM013:
            vol = 70;
            break;
        case BGM014:
            vol = 85;
            break;
        case BGM015:
            vol = 85;
            break;
        case BGM016:
            vol = 80;
            break;
        case BGM017:
            vol = 83;
            break;
        case BGM018:
            vol = 75;
            break;
        case BGM019:
            vol = 80;
            break;
        case BGM020:
            vol = 88;
            break;
        case BGM021:
            vol = 88;
            break;
        case BGM022:
            vol = 95;
            break;
        case BGM023:
            vol = 100;
            break;
        case BGM024:
            vol = 80;
            break;
        case BGM025:
            vol = 80;
            break;
        case BGM026:
            vol = 90;
            break;
        case BGM027:
            vol = 100;
            break;
        case BGM028:
            vol = 70;
            break;
        case BGM029:
            vol = 108;
            break;
        case BGM030:
            vol = 97;
            break;
        case BGM031:
            vol = 82;
            break;
        case BGM032:
            vol = 120;
            break;
        case BGM033:
            vol = 50;
            break;
        case BGM034:
            vol = 90;
            break;
        case BGM035:
            vol = 100;
            break;
        case BGM036:
            vol = 73;
            break;
        case BGM037:
            vol = 55;
            break;
        case BGM038:
            vol = 100;
            break;
        case BGM039:
            vol = 90;
            break;
        case BGM040:
            vol = 125;
            break;
        case BGM041:
            vol = 110;
            break;
        case BGM042:
            vol = 90;
            break;
        case BGM043:
            vol = 60;
            break;
        case BGM044:
            vol = 95;
            break;
        case BGM045:
            vol = 80;
            break;
        case BGM046:
            vol = 100;
            break;
        case BGM047:
            vol = 100;
            break;
        case BGM048:
            vol = 90;
            break;
        case BGM049:
            vol = 90;
            break;
        case BGM050:
            vol = 90;
            break;
        case BGM052:
            vol = 80;
            break;
        case BGM053:
            vol = 115;
            break;
        case BGM054:
            vol = 85;
            break;
        case BGM055:
            vol = 110;
            break;
        case BGM056:
            vol = 75;
            break;
        case BGM057:
            vol = 127;
            break;
        case BGM058:
            vol = 90;
            break;
        case BGM059:
            vol = 110;
            break;
        case BGM060:
            vol = 70;
            break;
        case BGM061:
            vol = 75;
            break;
        case BGM062:
            vol = 125;
            break;
        case BGM063:
            vol = 110;
            break;
        case BGM064:
            vol = 85;
            break;
        case BGM065:
            vol = 50;
            break;
        case BGM066:
            vol = 50;
            break;
        case BGM067:
            vol = 70;
            break;
        case BGM068:
            vol = 40;
            break;
        case BGM069:
            vol = 80;
            break;
        case BGM070:
            vol = 55;
            break;
        case BGM072:
            vol = 115;
            break;
        case BGM073:
            vol = 120;
            break;
        case BGM074:
            vol = 120;
            break;
        case BGM075:
            vol = 80;
            break;
        case BGM076:
            vol = 105;
            break;
        case BGM077:
            vol = 100;
            break;
        case BGM078:
            vol = 100;
            break;
        case BGM079:
            vol = 70;
            break;
        case BGM080:
            vol = 75;
            break;
        case BGM081:
            vol = 120;
            break;
        case BGM082:
            vol = 100;
            break;
        case BGM083:
            vol = 90;
            break;
        case BGM084:
            vol = 45;
            break;
        case BGM085:
            vol = 90;
            break;
        case BGM086:
            vol = 85;
            break;
        default:
            vol = 100;
            break;
    }

    return vol / 127.0f;
}

s32 hBgmGetLoopSize(BGM no) {
    s32 size;

    if (SysGbl.nmode == 2)
        return hTestBGMskip;

    switch (no) {
        case BGM000:
            size = 3311616;
            break;
        case BGM001:
            size = 2179072;
            break;
        case BGM004:
            size = 330624;
            break;
        case BGM005:
            size = 134848;
            break;
        case BGM006:
            size = 112000;
            break;
        case BGM007:
            size = 190400;
            break;
        case BGM008:
            size = 428736;
            break;
        case BGM009:
            size = 132608;
            break;
        case BGM010:
            size = 1657600;
            break;
        case BGM013:
            size = 3508736;
            break;
        case BGM014:
            size = 3437056;
            break;
        case BGM015:
            size = 2982784;
            break;
        case BGM016:
            size = 440832;
            break;
        case BGM017:
            size = 2538368;
            break;
        case BGM018:
            size = 3809344;
            break;
        case BGM020:
            size = 2694272;
            break;
        case BGM021:
            size = 2694272;
            break;
        case BGM022:
            size = 3293248;
            break;
        case BGM023:
            size = 161280;
            break;
        case BGM026:
            size = 1613696;
            break;
        case BGM027:
            size = 1589056;
            break;
        case BGM028:
            size = 3810240;
            break;
        case BGM031:
            size = 297472;
            break;
        case BGM032:
            size = 66304;
            break;
        case BGM033:
            size = 1436736;
            break;
        case BGM034:
            size = 428736;
            break;
        case BGM036:
            size = 1177792;
            break;
        case BGM037:
            size = 1437184;
            break;
        case BGM039:
            size = 1000384;
            break;
        case BGM040:
            size = 460096;
            break;
        case BGM041:
            size = 9226112;
            break;
        case BGM043:
            size = 2674560;
            break;
        case BGM045:
            size = 1060864;
            break;
        case BGM046:
            size = 771904;
            break;
        case BGM047:
            size = 955136;
            break;
        case BGM048:
            size = 969920;
            break;
        case BGM049:
            size = 756224;
            break;
        case BGM055:
            size = 847168;
            break;
        case BGM056:
            size = 163520;
            break;
        case BGM057:
            size = 2694272;
            break;
        case BGM058:
            size = 879872;
            break;
        case BGM059:
            size = 1000384;
            break;
        case BGM060:
            size = 1444352;
            break;
        case BGM061:
            size = 799232;
            break;
        case BGM064:
            size = 6038144;
            break;
        case BGM066:
            size = 632128;
            break;
        case BGM069:
            size = 662144;
            break;
        case BGM070:
            size = 342720;
            break;
        case BGM076:
            size = 4563328;
            break;
        case BGM077:
            size = 2116800;
            break;
        case BGM078:
            size = 4553472;
            break;
        case BGM079:
            size = 726208;
            break;
        case BGM080:
            size = 1154496;
            break;
        case BGM082:
            size = 22447936;
            break;
        case BGM086:
            size = 6038144;
            break;
        default:
            size = 0;
            break;
    }

    return size / 448 * 256;
}

s32 hBgmGetLoopLSN() {
    s32 size;
    s32 sector;
    s32 fine;

    size = hBgmGetLoopSize(bD->bgmNo);
    sector = size / 2048;
    fine = (size % 2048 + 128) / 256 * 256;

    if (fine == 2048) {
        sector++;
        fine = 0;
    }

    if (bD->cdRemain != 0) {
        bD->iopFineTop[bD->iopID] = (0x80000 - bD->cdRemain * 0x800) / bD->bgmChMax / 2;
        bD->iopFine[bD->iopID] = fine;
    } else {
        bD->iopFineTop[bD->iopID] = 0;
        bD->iopFine[bD->iopID] = fine;
    }

    return bD->cdTop + sector * bD->bgmChMax * 2;
}

void hBgmPlay(BGM no, s32 ch) {
    s32 ac3;

    if (no < BGM000) {
        no = BGM002;
        ch = 0;
    }

    if (sD->Stereo == SND_MODE_5P1CH) {
        switch (no) {
            case BGM072:
                ac3 = 0;
                break;
            case BGM073:
                ac3 = 2;
                break;
            case BGM074:
                ac3 = 3;
                break;
            case BGM081:
                ac3 = 1;
                break;
            default:
                ac3 = -1;
                break;
        }

        if (ac3 != -1) {
            hStr_0016f6a8(ac3);
            return;
        }
    }

    bD->Command |= 1;
    cD->BGMplay = 1;
    hCdCueFlushBGM();
    if (cD->dataFlag == CDREAD_BGM && cD->dataStat != 2) {
        while (!sceCdBreak());
        cD->dataStat = 0;
    }

    bD->iopID = 0;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
    bD->iopFineTop[0] = 0;
    bD->iopFineTop[1] = 0;
    bD->iopFine[0] = 0;
    bD->iopFine[1] = 0;
    hBgmSetCh(ch);
    bD->bgmNo = no;
    bD->bgmChMax = BgmTable[no].chnum / 2;
    bD->bgmLength = BgmTable[no].chsize;
    bD->bgmSkipSize = hBgmGetLoopSize(no);
    bD->cdCur = bD->cdTop = BgmTable[no].start + bD->file.lsn;
    bD->cdSectors = ((BgmTable[no].chsize + 0x7FF) / 0x800) * BgmTable[no].chnum;
    bD->fadeFlag = 0;
    bD->bgmVol = 0.78740156f;
    bD->nextNo = -1;
    hCdCuePush(bD->cdCur, 0x100, bD->iopAddr[bD->iopID], 1, cD->eeCnt);
    bD->cdReq = 0;
    bD->cdCur += 0x100;
    bD->cdRemain = bD->cdCur - (bD->cdTop + bD->cdSectors);
}

void hBgmPlayLast10sec(BGM no, s32 ch) {
    bD->Command |= 0x20;
    cD->BGMplay = 1;
    hCdCueFlushBGM();
    if (cD->dataFlag == CDREAD_BGM && cD->dataStat != 2) {
        while (!sceCdBreak());
        cD->dataStat = 0;
    }

    bD->iopID = 0;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
    bD->iopFineTop[0] = 0;
    bD->iopFineTop[1] = 0;
    bD->iopFine[0] = 0;
    bD->iopFine[1] = 0;
    hBgmSetCh(ch);
    bD->bgmNo = no;
    bD->bgmChMax = BgmTable[no].chnum / 2;
    bD->bgmLength = BgmTable[no].chsize;
    bD->bgmSkipSize = hBgmGetLoopSize(no);
    bD->cdTop = BgmTable[no].start + bD->file.lsn;
    bD->cdSectors = ((BgmTable[no].chsize + 0x7FF) / 0x800) * BgmTable[no].chnum;
    bD->cdCur = bD->cdTop + bD->cdSectors - BgmTable[no].chnum * 64;
    bD->fadeFlag = 0;
    bD->bgmVol = 0.78740156f;
    bD->nextNo = -1;
    hCdCuePush(bD->cdCur, 0x100, bD->iopAddr[bD->iopID], 1, cD->eeCnt);
    bD->cdReq = 0;
    bD->cdCur += 0x100;
    bD->cdRemain = bD->cdCur - (bD->cdTop + bD->cdSectors);
}

void hBgmFadeIn(BGM no, s32 ch, s32 frame) {
    hBgmPlay(no, ch);
    if (frame > 0) {
        bD->fadeFlag = 2;
        bD->fadeCnt = 0;
        bD->fadeMax = frame;
    }
}

void hBgmFadeOut(s32 frame) {
    if (sD->Stereo != SND_MODE_5P1CH || aD->field_0x80 == 0) {
        bD->fadeFlag = 1;
        bD->fadeCnt = 0;
        bD->fadeMax = frame;
    }
}

void hBgmFadeNext(s32 frame, BGM nextno, s32 nextch, s32 nextframe, f32 nextvol) {
    bD->fadeFlag = 1;
    bD->fadeCnt = 0;
    bD->fadeMax = frame;
    bD->nextNo = nextno;
    bD->nextCh = nextch;
    bD->nextFrame = nextframe;
    bD->nextVol = nextvol;
}

void hBgmPause() {
    bD->Command |= 2;
}

void hBgmSetVol(f32 vol) {
    bD->bgmVol = vol;
}

void hBgmMute() {
    bD->bgmMute = 1 - bD->bgmMute;
}

void hBgmSetCh(s32 ch) {
    bD->Command |= 8;
    bD->bgmCh = ch;
}

void hBgmChangeChX(s32 ch) {
    bD->Command |= 0x10;
    bD->bgmCh = ch;
}

void hBgmWorkClear() {
    bD->nextNo = -1;
    bD->bgmCh = 0;
    bD->bgmChMax = 2;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
    bD->iopID = 0;
    bD->cdReq = 0;
    bD->fadeFlag = 0;
    bD->bgmVol = 0.0f;
    bD->bgmMute = 0;
    cD->BGMplay = 0;
}

void hBgmReset() {
    if (cD->dataFlag == CDREAD_BGM && cD->dataStat != 2) {
        while (!sceCdBreak());
        cD->dataStat = 0;
    }

    hBgmWorkClear();
    bD->Command |= 0x4;
    hCdCueFlushBGM();
    if (sD->Stereo == SND_MODE_5P1CH && aD->field_0x80 != 0) {
        hStr_0016f6e8();
    }
}

void hBgmStop() {
    if (cD->dataFlag == CDREAD_BGM && cD->dataStat != 2) {
        while (!sceCdBreak());
        cD->dataStat = 0;
    }

    bD->bgmCh = 0;
    bD->bgmChMax = 2;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
    bD->iopID = 0;
    bD->cdReq = 0;
    bD->fadeFlag = 0;
    bD->bgmVol = 0.0f;
    bD->bgmMute = 0;
    cD->BGMplay = 0;
    bD->Command |= 4;
    hCdCueFlushBGM();
}

void hBgmFadeNextClear() {
    if (cD->dataFlag == CDREAD_BGM && cD->dataStat != 2) {
        while (!sceCdBreak());
        cD->dataStat = 0;
    }

    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
    bD->iopID = 0;
    bD->cdReq = 0;
    bD->bgmVol = 0.0f;
    bD->bgmMute = 0;
    bD->Command |= 4;
    hCdCueFlushBGM();
}

void hBgmRead() {
    s32 nsec;

    nsec = 0x100;
    if (bD->cdRemain == 0) {
        bD->cdCur = hBgmGetLoopLSN();
    } else {
        bD->iopFineTop[bD->iopID] = 0;
        bD->iopFine[bD->iopID] = 0;
    }

    bD->cdRemain = (bD->cdCur + nsec) - (bD->cdTop + bD->cdSectors);
    if (bD->cdRemain > 0)
        nsec -= bD->cdRemain;
    hCdCuePush(bD->cdCur, nsec, bD->iopAddr[bD->iopID], 1, cD->eeCnt);
    bD->cdReq = 0;
    bD->cdCur += nsec;
}

void hBgmRemain() {
    bD->cdCur = hBgmGetLoopLSN();
    hCdCuePush(bD->cdCur, bD->cdRemain, bD->iopAddr[bD->iopID] + 0x80000 - bD->cdRemain * 0x800, 1, cD->eeCnt);
    bD->cdCur += bD->cdRemain;
    bD->cdRemain = bD->cdCur - (bD->cdTop + bD->cdSectors);
    bD->cdReq = 0;
}

void hBgmMain() {
    switch (bD->fadeFlag) {
        case 2:
            if (!(bD->Command & 2) && ++bD->fadeCnt >= bD->fadeMax)
                bD->fadeFlag = 0;
            break;
        case 1:
            if (!(bD->Command & 2) && ++bD->fadeCnt >= bD->fadeMax) {
                if (bD->nextNo != -1) {
                    hBgmFadeNextClear();
                    bD->fadeFlag = 3;
                } else {
                    hBgmReset();
                    bD->fadeFlag = 0;
                }
            }
            break;
        case 3:
            if (!(bD->Command & 2)) {
                bD->fadeFlag = 0;
                hBgmFadeIn(bD->nextNo, bD->nextCh, bD->nextFrame);
                hBgmSetVol(bD->nextVol);
                bD->nextNo = -1;
            }
            break;
    }

    if (cD->BGMplay != 0) {
        if (cD->dataFlag == CDREAD_BGM) {
            if (cD->dataStat == 2) {
                if (bD->cdRemain > 0) {
                    hBgmRemain();
                } else {
                    bD->iopOK[bD->iopID] = 1;
                    bD->iopID = 1 - bD->iopID;
                    bD->cdReq = 1;
                }
            }
        } else if (bD->iopID == bD->iopNext && bD->cdReq != 0) {
            hBgmRead();
        } else {
            return;
        }
    }
}

s32 hBgmGetMaxCh(BGM no) {
    return BgmTable[no].chnum / 2;
}

s32 hBgmGetStat() {
    if (cD->BGMplay != 0) {
        switch (bD->fadeFlag) {
            case 1:
                return 3;
            case 2:
                return 2;
            case 3:
                return 3;
            default:
                return 1;
        }
    } else {
        return 0;
    }    
}

BGM hBgmGetPlayNo() {
    if (hBgmGetStat() != 0)
        return bD->bgmNo;
    else
        return -1;
}

s32 hBgmGetPlayCh() {
    if (hBgmGetStat() != 0)
        return bD->bgmCh;
    else
        return -1;
}

void hBgmChangeID(s32 id, s32 mode) {
    gD->BGMchgreq = 1;
    gD->BGMchgid = id;
    gD->BGMchgmode = mode;
}

#pragma endregion BGM

#pragma region PPT

void hPptWorkClear() {
    s32 i;

    bD->Command |= 0x80;
    pD->pptMute = 0;
    pD->eeID = 0;
    pD->iopID = 0;
    pD->reqNum = 0;
    pD->reqID = 0;
    pD->listLoad = 0;
    pD->listPlay = 0;
    pD->listPlayIdx = 0;
    for (i = 0; i < 8; i++) {
        pD->listNum[i] = 0;
        pD->listTbl[i] = 0;
    }
    for (i = 0; i < 4; i++) {
        pD->pptPlay[i] = 0;
        pD->eeStat[i] = 0;
    }
}

void hPptStop(s32 id) {
    s32 num;

    num = (id >> 8) & 0xFFFFFF;
    id &= 0xFF;
    if (pD->pptID[id] == num) {
        pD->pptStop[id] = 1;
        if (pD->eeStat[id] == 3)
            pD->eeStat[id] = 0;
    }
}

void hPptReset() {
    if (cD->dataFlag == CDREAD_PPT && cD->dataStat != 2) {
        while (!sceCdBreak());
        cD->dataStat = 0;
    }

    hCdCueFlushPPT();
    hPptWorkClear();
}

void hPptPause() {
    bD->Command |= 0x40;
}

s32 hPptReq() {
    s32 var1;
    s32 var2;

    var1 = (pD->iopID + pD->reqNum) % 4;
    var2 = pD->reqID + pD->reqNum;
    pD->reqNum++;
    return var2 << 8 | var1;
}

void hPptKick() {
    s32 id;

    id = pD->iopID;
    if (pD->reqNum != 0) {
        if (pD->iopNext[id] == -1) {
            if (pD->eeStat[id] == 2) {
                pD->pptPlay[id] = pD->pptSize[id];
                pD->pptVol[id] = pD->reqVol[id];
                pD->pptID[id] = pD->reqID;
                pD->iopNext[id] = 0;
                pD->iopBfID[id] = 0;
                pD->eeStat[id] = 3;
                pD->eeOffset[id] = 0;
                pD->reqNum--;
                pD->reqID++;
                pD->reqID &= 0xFFFFFF;
                pD->iopID++;
                if (pD->iopID == 4)
                    pD->iopID = 0;
            }
        }
    }
}

s32 hPptGetFrame(s32 no) {
    return (PptTable[no & 0xFFFF].nsector * 2048 + 209) / 210;
}

void hPptSetList(s32 *p) {
    s32 ld;

    ld = pD->listLoad;
    pD->listNum[ld] = *p++;
    pD->listTbl[ld] = p;
    pD->listLoad++;
    if (pD->listLoad > 7)
        pD->listLoad = 0;
}

s32 hPptCheckList() {
    return pD->listNum[pD->listPlay] > pD->listPlayIdx ? 1 : 0;
}

s32 hPptGetListNo() {
    s32 no;

    no = pD->listTbl[pD->listPlay][pD->listPlayIdx];
    pD->listPlayIdx++;
    if (pD->listPlayIdx >= pD->listNum[pD->listPlay]) {
        pD->listNum[pD->listPlay] = 0;
        pD->listPlayIdx = 0;
        pD->listPlay++;
        if (pD->listPlay > 7)
            pD->listPlay = 0;
    }

    return no;
}

void hPptMain() {
    s32 i;
    s32 no;

    hPptKick();
    for (i = 0; i < 4; i++) {
        if (pD->eeStat[i] == 3 && pD->iopNext[i] == pD->iopBfID[i]) {
            hTrans2IOP(pD->iopAddr[i][pD->iopNext[i]], pD->eeAddr[i] + pD->eeOffset[i], 0x4000);
            pD->eeOffset[i] += 0x4000;
            if (pD->eeOffset[i] >= pD->pptSize[i])
                pD->eeStat[i] = 0;
            else
                pD->iopBfID[i] = 1 - pD->iopBfID[i];
        }
    }

    if (hPptCheckList() && pD->eeStat[pD->eeID] == 0) {
        pD->eeStat[pD->eeID] = 1;
        no = hPptGetListNo();
        if (no >> 0x10 != 0)
            pD->reqVol[pD->eeID] = 0.7f;
        else
            pD->reqVol[pD->eeID] = 1.0f;
        no &= 0xFFFF;
        hCdCuePush(PptTable[no].top + pD->file.lsn, PptTable[no].nsector, pD->eeAddr[pD->eeID], 3, cD->eeCnt);
        pD->pptSize[pD->eeID] = PptTable[no].nsector * 2048;
    }

    if (cD->dataFlag == CDREAD_PPT && cD->dataStat == 2) {
        pD->eeStat[pD->eeID] = 2;
        pD->eeID++;
        if (pD->eeID == 4)
            pD->eeID = 0;
    }
}

#pragma endregion PPT

#pragma region STR

void hStrInfo() {
    STRINFO *info;
    s32 i;
    s32 vol;
    f32 volf;

    info = (STRINFO *)RpcArg;

    switch (bD->fadeFlag) {
        case 2:
            volf = (f32)bD->fadeCnt / (f32)bD->fadeMax;
            break;
        case 1:
            volf = (f32)(bD->fadeMax - bD->fadeCnt) / (f32)bD->fadeMax;
            break;
        case 3:
            volf = 0.0f;
            break;
        default:
            volf = 1.0f;
            break;
    }

    switch (sD->pptFade) {
        case 0:
            break;
        case 1:
            volf *= 1.0f - sD->pptFadeCnt * 0.15f / 60.0f;
            sD->pptFadeCnt++;
            if (sD->pptFadeCnt >= 60.0f)
                sD->pptFade = 2;
            break;
        case 2:
            volf *= 0.85f;
            break;
        case 3:
            volf *= 0.85f + sD->pptFadeCnt * 0.15f / 60.0f;
            sD->pptFadeCnt++;
            if (sD->pptFadeCnt >= 60.0f)
                sD->pptFade = 0;
            break;
    }

    if (sD->Stereo == SND_MODE_MONAURAL)
        volf *= 1.0f + 6.0f / sD->dBfader;

    vol = sD->bgmMVol * bD->bgmVol * hSndFader2(volf) * 16367.0f;
    if (vol > 0x3FFF)
        vol = 0x3FFF;
    else if (vol < 0)
        vol = 0;
    if (bD->bgmMute != 0)
        vol = 0;
    if (hBgmGetStat() == 0)
        vol = 0;

    info->eeCnt = cD->eeCnt;
    info->Command = bD->Command;
    info->BGMok[0] = bD->iopOK[0];
    info->BGMok[1] = bD->iopOK[1];
    info->BGMfine[0] = bD->iopFine[0];
    info->BGMfine[1] = bD->iopFine[1];
    info->BGMfinetop[0] = bD->iopFineTop[0];
    info->BGMfinetop[1] = bD->iopFineTop[1];
    info->BGMlength = bD->bgmLength;
    info->BGMskipsize = bD->bgmSkipSize;
    info->BGMstereo = sD->Stereo;
    info->BGMvol = vol;
    info->BGMch = bD->bgmCh;
    info->BGMchmax = bD->bgmChMax;
    if (info->Command & 32)
        info->work0 = (bD->cdSectors / BgmTable[bD->bgmNo].chnum - 64) * 2048;

    volf = 1.0f;
    if (sD->Stereo == SND_MODE_MONAURAL)
        volf *= 1.0f - 6.0f / sD->dBfader;

    for (i = 0; i < 4; i++) {
        vol = sD->pptMVol * hSndFader2(volf * pD->pptVol[i]) * 16367.0f;
        if (vol > 0x3FFF)
            vol = 0x3FFF;
        else if (vol < 0)
            vol = 0;
        if (pD->pptMute != 0)
            vol = 0;

        info->PPTvol[i] = vol;
        info->PPTsize[i] = pD->pptPlay[i];
        info->PPTstop[i] = pD->pptStop[i];
        pD->pptPlay[i] = 0;
        pD->pptStop[i] = 0;
    }

    info->idk = aD->field_0x94;
    hRpc(IOP_StrInfo);
    bD->Command = 0;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
}

void hStrInit() {
    s32 i;
    s32 j;
    s32 addr;

    bD = &BgmData;
    pD = &PptData;
    aD = &Ac3Data;

    hBgmWorkClear();
    bD->Command = 0;
    bD->bgmVol = 0.78740156f;
    addr = hRpc(IOP_StrInit);
    bD->iopAddr[0] = addr;
    bD->iopAddr[1] = addr + 0x80000;

    hPptWorkClear();
    addr = bD->iopAddr[1] + 0x80000;
    for (i = 0; i < 4; i++) {
        pD->pptPlay[i] = 0;
        pD->eeStat[i] = 0;
        pD->eeAddr[i] = (s32)(SndTempBuff + i * 0x40000);
        for (j = 0; j < 2; j++) {
            pD->iopAddr[i][j] = addr;
            addr += 0x4000;
        }
    }

    aD->field_0xB4[0] = addr;
    aD->field_0xB4[1] = addr + 0x8000;
    hStrInfo();
    while (hRpcSync());
    hSndRpcRet();
    bD->iopID = bD->iopNext;

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&bD->file, "\\BGMPACK.BIN;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&pD->file, "\\PPTPACK.BIN;1"));
    hStr_0016f4d8();
}

void hStr_0016f4d8() {
    aD->field_0x90 = -1;
    aD->field_0x80 = 0;
    aD->field_0x84 = 0;
    aD->field_0x88 = 0;
    aD->field_0xA8 = 0;
    aD->field_0x9C = 0;
    aD->field_0xD0[0] = (s32)SndTempBuff;
    aD->field_0xD0[1] = (s32)SndTempBuff + 0x80000;

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[0], "\\BGM000.AC3;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[1], "\\BGM001.AC3;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[2], "\\BGM002.AC3;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[3], "\\BGM003.AC3;1"));
}

void hStr_0016f5c8(s32 ac3) {
    aD->field_0x9C = 0;
    aD->field_0xBC = 0;
    aD->field_0xB0 = 0;
    aD->field_0xC0 = 0;
    aD->field_0xC4 = 0;
    aD->field_0xC8[0] = 0;
    aD->field_0xC8[1] = 0;
    aD->field_0x8C = 0;
    aD->field_0x90 = ac3;
    aD->field_0x94 = aD->files[ac3].size;
    aD->field_0x98 = 0;
    aD->field_0xE0 = aD->field_0xDC = aD->files[ac3].lsn;
    aD->field_0xE4 = (aD->files[ac3].size + 0x7FF) >> 0xB;
    hCdCuePush(aD->field_0xE0, 0x100, aD->field_0xD0[0], 5, cD->eeCnt);
    aD->field_0xD8 = 0;
    aD->field_0xE0 += 0x100;
    aD->field_0xE8 = aD->field_0xE0 - (aD->field_0xDC + aD->field_0xE4);
}

s32 hStr_0016f680() {
    return aD->field_0x80 != 0;
}

s32 hStr_0016f690() {
    return aD->field_0xC8[aD->field_0xC0];
}

void hStr_0016f6a8(s32 ac3) {
    hStr_0016f5c8(ac3);
    aD->field_0x88 = 1;
    bD->Command |= 0x400;
    aD->field_0x80 = 1;
    aD->field_0x84 = 0;
}

void hStr_0016f6e8() {
    bD->Command |= 0x200;
    aD->field_0x8C = 0;
    aD->field_0x9C = 0;
    aD->field_0x90 = -1;
    aD->field_0x80 = 0;
}

void hStr_0016f718(s32 param_1) {
    aD->field_0x9C = 1;
    aD->field_0xA0 = 0;
    aD->field_0xA4 = param_1;
}

void hStr_0016f730() {
    aD->field_0x84++;
    if (aD->field_0x84 > 9) {
        bD->Command |= 0x1000;
        aD->field_0x80 = 2;
        aD->field_0x84 = 0;
    }
}

void hStr_0016f770() {
    aD->field_0x84++;
    if (aD->field_0x84 > 19) {
        bD->Command |= 0x800;
        aD->field_0x80 = 3;
        aD->field_0x84 = 0;
        hStr_0016f7b0();
    }
}

void hStr_0016f7b0() {
    if (hStr_0016f690()) {
        aD->field_0x80 = 4;
        aD->field_0x8C = 1;
        aD->field_0x88 = 0;
    }
}

void hStr_0016f7e8() {
    if (aD->field_0x8C != 0 && aD->field_0xC8[aD->field_0xC0] != 0) {
        if (aD->field_0x8C == 1) {
            sceSdTransToIOP((void *)(aD->field_0xD0[aD->field_0xC0] + aD->field_0xC4), aD->field_0xB4[aD->field_0xB0], 0x10000, 1);
            aD->field_0xC4 += 0x10000;
            aD->field_0x98 += 0x10000;
            if (aD->field_0xC4 > 0x7FFFF) {
                aD->field_0xC0 = 1 - aD->field_0xC0;
                aD->field_0xC4 = 0;
                aD->field_0xC8[aD->field_0xBC] = 0;
            }
            bD->Command |= 0x2000;
            aD->field_0x8C = 2;
        } else if ((aD->AC3stat >> 24 & 1) != aD->field_0xB0) {
            sceSdTransToIOP((void *)(aD->field_0xD0[aD->field_0xC0] + aD->field_0xC4), aD->field_0xB4[aD->field_0xB0], 0x8000, 1);
            aD->field_0xC4 += 0x8000;
            aD->field_0x98 += 0x8000;
            if (aD->field_0xC4 > 0x7FFFF) {
                aD->field_0xC0 = 1 - aD->field_0xC0;
                aD->field_0xC4 = 0;
                aD->field_0xC8[aD->field_0xBC] = 0;
            }
            aD->field_0xB0 = 1 - aD->field_0xB0;
        }

        if (aD->field_0x98 >= aD->field_0x94) {
            hStr_0016f6e8();
            aD->field_0x80 = 0;
        }
    }
}

void hStrMain() {
    if (aD->field_0x90 != -1) {
        if (aD->field_0x9C == 1) {
            aD->field_0xA0++;
            if (aD->field_0xA0 > aD->field_0xA4)
                hStr_0016f6e8();
        }

        if (cD->dataFlag == 5) {
            if (cD->dataStat == 2) {
                aD->field_0xC8[aD->field_0xBC] = 1;
                aD->field_0xBC = 1 - aD->field_0xBC;
                if (aD->field_0xE8 < 0)
                    aD->field_0xD8 = 1;
            }
        } else if (aD->field_0xC8[aD->field_0xBC] == 0 && aD->field_0xD8 != 0) {
            hCdCuePush(aD->field_0xE0, 0x100, aD->field_0xD0[aD->field_0xBC], 5, cD->eeCnt);
            aD->field_0xD8 = 0;
            aD->field_0xE0 += 0x100;
            aD->field_0xE8 = aD->field_0xE0 - (aD->field_0xDC + aD->field_0xE4);
        }

        switch (aD->field_0x80) {
            case 0:
                return;
            case 1:
                hStr_0016f730();
                break;
            case 2:
                hStr_0016f770();
                break;
            case 3:
                hStr_0016f7b0();
                break;
            case 4:
                hStr_0016f7e8();
                break;
        }
    }
}

#pragma endregion STR
