#include "common.h"

void hStrInfo() {
    float f;
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
    
    int n = (int)(sD->bgmMVol * bD->bgmVol * hSndFader2(f) * 16367.0f);
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

    STRINFO* str = (STRINFO*)RpcArg;
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

    for (int i = 0; i < 4; i++) {
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
    hIopDispatch(0x1b000001);
    bD->Command = 0;
    bD->iopOK[0] = 0;
    bD->iopOK[1] = 0;
}

void FUN_0016c6e8() {
    int *ret = hIopDispatch(0x8000001);
    sD->VoiceStat[0] = ret[0];
    sD->VoiceStat[1] = ret[1];
    bD->iopPoint = ret[3];
    bD->iopNext = ret[2];
    for (int i = 0; i < 4; i++) {
        pD->iopNext[i] = ret[4 + i];
    }
    aD->field_0xAC = ret[8];
}
