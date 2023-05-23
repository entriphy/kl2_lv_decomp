#include "common.h"

hBGMDATA BgmData = {};
hBGMDATA* bD = NULL;
hPPTDATA PptData;
hPPTDATA* pD;

/* BGM Functions */

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

int hBgmGetStat() {
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

void hBgmWorkClear() {
    bD->nextNo = -1;
    bD->bgmCh = 0;
    bD->bgmChMax = 2;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
    bD->iopID = 0;
    bD->cdReq = 0;
    bD->fadeFlag = 0;
    bD->bgmVol = 0.0;
    bD->bgmMute = 0;
    cD->BGMplay = 0;
}

/* PPT Functions */

void hPptWorkClear() {
    int i;

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

void hPptReset() {
    if (cD->dataFlag == CDREAD_PPT && cD->dataStat != 2) {
        while (!sceCdBreak());
        cD->dataStat = 0;
    }
    hCdCueFlushPPT();
    hPptWorkClear();
}

/* STR Functions */

void hStrInfo() {
    float f;
    int n;
    STRINFO *str;
    int i;

    switch (bD->fadeFlag) {
        case 1:
            f = (float)(bD->fadeMax - bD->fadeCnt) / (float)bD->fadeMax;
            break;
        case 2:
            f = (float)bD->fadeCnt / (float)bD->fadeMax;
            break;
        case 3:
            f = 0.0f;
            break;
        default:
            f = 1.0f;
            break;
    }

    switch (sD->pptFade) {
        case 1:
            f *= 1.0f - 15.0f * sD->pptFadeCnt / 60.0f;
            if (++sD->pptFadeCnt >= 60.0f)
                sD->pptFade = 2;
            break;
        case 2:
            f *= 0.85f;
            break;
        case 3:
            f *= 0.85f + 0.15f * sD->pptFadeCnt / 60.0;
            if (++sD->pptFadeCnt >= 60.0)
                sD->pptFade = 0;
            break;
    }

    if (sD->Stereo == SND_MODE_MONAURAL) {
        f *= 1.0f + 6.0f / sD->dBfader;
    }
    
    n = (int)(sD->bgmMVol * bD->bgmVol * hSndFader2(f) * 16367.0f);
    if (n < 0x4000) {
        if (n < 0) {
            n = 0;
        }
    } else {
        n = 0x3FFF;
    }
    if (bD->bgmMute) {
        n = 0;
    }
    if (!hBgmGetStat()) {
        n = 0;
    }

    str = (STRINFO *)RpcArg;
    str->eeCnt = cD->eeCnt;
    str->Command = bD->Command;
    str->BGMok[0] = (s8)bD->iopOK[0];
    str->BGMok[1] = (s8)bD->iopOK[1];
    str->BGMfine[0] = (s8)bD->iopFine[0];
    str->BGMfine[1] = (s8)bD->iopFine[1];
    str->BGMfinetop[0] = (s8)bD->iopFineTop[0];
    str->BGMfinetop[1] = (s8)bD->iopFineTop[1];
    str->BGMlength = bD->bgmLength;
    str->BGMskipsize = bD->bgmSkipSize;
    str->BGMstereo = sD->Stereo;
    str->BGMvol = n;
    str->BGMch = (s8)bD->bgmCh;
    str->BGMchmax = (s8)bD->bgmChMax;
    if (str->Command & 0x20 != 0) {
        str->work0 = (bD->cdSectors / BgmTable[bD->bgmNo].chnum - 0x40) * 0x800;
    }

    f = 1.0f;
    if (sD->Stereo == SND_MODE_MONAURAL) {
        f *= 1.0f - 6.0f * sD->dBfader;
    }

    for (i = 0; i < 4; i++) {
        n = (int)(sD->pptMVol * hSndFader2(f * pD->pptVol[i]) * 16367.0f);
        if (n < 0x4000) {
            if (n < 0) {
                n = 0;
            }
        } else {
            n = 0x3FFF;
        }
        if (pD->pptMute) {
            n = 0;
        }

        str->PPTvol[i] = n;
        str->PPTsize[i] = pD->pptPlay[i];
        str->PPTsize[i] = (s8)pD->pptStop[i];
        pD->pptPlay[i] = 0;
        pD->pptStop[i] = 0;
    }

    str->idk = aD->field_0x94;
    hRpc(IOP_StrInfo);
    bD->Command = 0;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
}

void hStrInit() {
    int i, j;
    int *iopAddr;
    int *n;

    bD = &BgmData;
    pD = &PptData;
    aD = &Ac3Data;
    hBgmWorkClear();
    bD->Command = 0;
    bD->bgmVol = 0.78740156f;
    iopAddr = (s32 *)hRpc(IOP_StrInit);
    bD->iopAddr[0] = iopAddr;
    bD->iopAddr[1] = iopAddr + 0x20000;
    n = bD->iopAddr[1] + 0x20000;
    for (i = 0; i < 4; i++) {
        pD->pptPlay[i] = 0;
        pD->eeStat[i] = 0;
        pD->eeAddr[i] = pptEeAddrs[i];
        for (j = 0; j < 2; j++) {
            pD->iopAddr[i][j] = n;
            n += 0x1000;
        }
    }
    aD->field_0xB4 = n;
    aD->field_0xB8 = n + 0x3000;
    hStrInfo();
    while (hRpcSync());
    hStr_0016c6e8();

    bD->iopID = bD->iopNext;
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&bD->file, "\\BGMPACK.BIN;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&pD->file, "\\PPTPACK.BIN;1"));

    aD->field_0x90 = -1;
    aD->field_0x80 = 0;
    aD->field_0x84 = 0;
    aD->field_0x88 = 0;
    aD->field_0xA8 = 0;
    aD->field_0x9C = 0;
    aD->field_0xD0 = pptEeAddrs[0];
    aD->field_0xD4 = pptEeAddrs[2];

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[0], "\\BGM000.AC3;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[1], "\\BGM001.AC3;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[2], "\\BGM002.AC3;1"));
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&aD->files[3], "\\BGM003.AC3;1"));
}

void hStr_0016c6e8() {
    s32 i;
    s32 *ret = (s32 *)hRpc(IOP_RpcInfo);

    sD->VoiceStat[0] = ret[0];
    sD->VoiceStat[1] = ret[1];
    bD->iopPoint = ret[3];
    bD->iopNext = ret[2];
    for (i = 0; i < 4; i++) {
        pD->iopNext[i] = ret[4 + i];
    }
    aD->AC3stat = ret[8];
}

void hStr_0016f6e8() {
    bD->Command |= 0x200;
    aD->field_0x8C = 0;
    aD->field_0x9C = 0;
    aD->field_0x90 = -1;
    aD->field_0x80 = 0;
}
