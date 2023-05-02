#include <math.h>
#include <sifrpc.h>
#include "hoshino.h"
#include "data_symbols.h"
#include "function_symbols.h"

EFXSE *EfxSE[50][20] = {};

void hSndPkEffect() {
    int i;
    
    sD->effChange = 1;
    for (i = 0; i < 2; i++) {
        sD->PkMax++;
        *sD->PkNum++ = 8;
        *sD->PkNum++ = i;
        *sD->PkNum++ = sD->effMode;
        *sD->PkNum++ = sD->effDepth;
        *sD->PkNum++ = sD->effDepth >> 8;
        *sD->PkNum++ = sD->effDepth;
        *sD->PkNum++ = sD->effDepth >> 8;
        *sD->PkNum++ = sD->effDelay;
        *sD->PkNum++ = sD->effFeed;
    }
}

void hSndPkSetMVol(int voll, int volr) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_MVOLALL;
    *sD->PkNum++ = voll;
    *sD->PkNum++ = (voll >> 8) & 0x7F;
    *sD->PkNum++ = volr;
    *sD->PkNum++ = (volr >> 8) & 0x7F;
}

void hSndPkSetEVol(int vol) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_EVOL;
    *sD->PkNum++ = vol;
    *sD->PkNum++ = vol >> 8;
}

void hSndPkSetVol(int voice, float pan, float vol) {
    int p = pan * 16384.0f;
    int v = vol * 16384.0f;

    if (p > 0x4000)
        p = 0x4000;
    if (p < -0x4000)
        p = -0x4000;
    if (v > 0x4000)
        v = 0x4000;
    if (v < -0x4000)
        v = -0x4000;
    
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_VOL;
    *sD->PkNum++ = voice;
    *sD->PkNum++ = p;
    *sD->PkNum++ = p >> 8;
    *sD->PkNum++ = v;
    *sD->PkNum++ = v >> 8;
}

void hSndPkSetPitch(int voice, int pitch) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_PITCH;
    *sD->PkNum++ = voice;
    *sD->PkNum++ = pitch;
    *sD->PkNum++ = pitch >> 8;
}

void hSndPkSetPalPitch(int voice) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_PALPITCH;
    *sD->PkNum++ = voice;
}

void hSndPkKeyOn(int voice, int flag, int bank, int prog, int splt, float pan, float vol) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_KEYON;
    *sD->PkNum++ = voice;
    *sD->PkNum++ = flag;
    *sD->PkNum++ = bank;
    *sD->PkNum++ = prog;
    *sD->PkNum++ = splt;
    sD->KeyonV[voice & 1] |= 1 << (voice >> 1);
    hSndPkSetVol(voice, pan, vol);
}

void hSndPkKeyOff(int voice) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_KEYOFF;
    *sD->PkNum++ = voice;
}

void hSndPkKeyOffAll() {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_KEYOFFALL;
}

int hSndPkGetSize() {
    // i have no clue how this matches
    int size = (s32)sD->PkNum - (s32)SndMainBuffer;
    size = size < -1 ? size + 0x1E : size + 0xF;
    return (((s32)sD->PkNum - (s32)SndMainBuffer + 0xF) / 0x10) * 0x10;
}

void hSndReset() {
    hBgmReset();
    hPptReset();
    hSndPkKeyOffAll();
    hSeInitGrp(0);
    hSndSetMVol(1.0f);
    sD->fadeFlag = 0;
    sD->Mute = 0;
}

void hSndFadeOutAll(int frame) {
    hSeKeyOffAll();
    sD->fadeFlag = 1;
    sD->fadeCnt = 0.0f;
    sD->fadeMax = (float)frame;
}

void hSndFadeInAll(int frame) {
    sD->fadeFlag = 3;
    sD->fadeCnt = 0.0f;
    sD->fadeMax = (float)frame;
}

void hSndSetMVol(float vol) {
    sD->MVol = hSndFader(vol);
}

// Not matching
int hSndFader(float vol) {
    if (vol == 0.0f) {
        return 0;
    } else {
        hSNDDATA *snd;
        int n;
        
        if (vol > 1.0f) {
            vol = 1.0f;
        }
        snd = sD;
        n = powf(10.0f, ((1.0f - vol) * snd->dBfader + snd->log10Volume) / 20.0f);
        if (n > 0x3FFF) {
            n = 0x3FFF;
        }
        if (n < -0x4000) {
            n = -0x4000;
        }

        return n;
    }
}

// Not matching
float hSndFader2(float vol) {
    float f = 0.0f;
    hSNDDATA *snd;

    if (vol != 0.0f) {
        if (vol > 1.0f)
            vol = 1.0f;
        snd = sD;
        f = powf(10.0f, ((1.0f - vol) * snd->dBfader) / 20.0f);
    }
    return f;
}

// Not matching
void hSndMain() {
    if (sD->TitleDelayCnt != 0 && --sD->TitleDelayCnt == 0) {
        hSeKeyOn(0xc80c80ca0e900, NULL, 0);
        hSeKeyOn(0xc80c80ca0e901, NULL, 0);
    }

    switch (sD->fadeFlag) {
        case 1:
            hSndSetMVol((sD->fadeMax - sD->fadeCnt) / sD->fadeMax);
            sD->fadeCnt += 1.0f;
            if (sD->fadeCnt >= sD->fadeMax) {
                sD->fadeFlag = 2;
                sD->fadeCnt = 0.0f;
                hBgmReset();
                hPptReset();
                hSndPkKeyOffAll();
                hSeInitGrp(0);
                hSndSetMVol(0.0f);
            }
            break;
        case 2:
            sD->fadeCnt += 1.0f;
            if (sD->fadeCnt >= 2.0f) {
                sD->fadeFlag = 0;
                hSndSetMVol(1.0f);
            }
            break;
        case 3:
            hSndSetMVol(sD->fadeCnt / sD->fadeMax);
            sD->fadeCnt += 1.0f;
            if (sD->fadeCnt >= sD->fadeMax) {
                hSndSetMVol(1.0f);
                sD->fadeFlag = 0;
                sD->fadeCnt = 0.0f;
            }
            break;
        case 4:
            sD->seMVol = sD->fadeCnt * 1.43f / sD->fadeMax;
            sD->fadeCnt += 1.0f;
            if (sD->fadeCnt >= sD->fadeMax) {
                sD->seMVol = 1.43f;
                sD->fadeFlag = 0;
                sD->fadeCnt = 0.0f;
            }
            break;
    }

    if (sD->effVol != sD->effVolBak && sD->effChange == 0) {
        if (sD->effVol - sD->effVolBak > 0.01f)
            sD->effVolBak += 0.01f;
        else if (sD->effVol - sD->effVolBak < -0.01f)
            sD->effVolBak -= 0.01f;
        else
            sD->effVolBak = sD->effVol;
        hSndPkSetEVol(sD->effVolBak * 32767.0f);
    }
    
    if (sD->Mute != 0) {
        hSndPkSetMVol(0,0);
        sD->Mute = 0;
    } else {
        hSndPkSetMVol(sD->MVol, sD->MVol);
    }
    
    hSndPkGetSize(); // why does this get called lol
    ((short *)SndMainBuffer)[0] = sD->PkMax;
    hRpc(0x2a000001);
    sD->PkNum = SndMainBuffer + 2;
    sD->PkMax = 0;
    sD->VoiceStat[0] |= sD->KeyonV[0];
    sD->VoiceStat[1] |= sD->KeyonV[1];
    sD->KeyonV[0] = 0;
    sD->KeyonV[1] = 0;
    heSeObjMain();
    sD->effChange = 0;
}

void hSndInit() {
    sceSdRemoteInit();
    
    sD = &SndData;
    sD->PkNum = (u8*)&SndMainBuffer[2];
    sD->PkMax = 0;
    sD->iopBankAddr = hRpc(IOP_SndInit);

    RpcArg[0] = 0xFFFFFF;
    RpcArg[1] = 0xFFFF;
    hRpc(IOP_SndMask);
    
    sD->pad = 0;
    sD->Stereo = SND_MODE_STEREO;
    sD->Mute = 0;
    sD->stageBank = 0;
    sD->dBfader = -30.0f;
    sD->log10Volume = (float)log10(16367.0) * 20.0f;
    hSndSetMVol(0.0f);
    hSndPkSetMVol(0, 0);
    sD->effMode = 0;
    sD->effDepth = 0;
    sD->effDelay = 0;
    sD->effFeed = 0;
    sD->effMix = 3;
    hSndPkEffect();
    sD->effVolBak = 0.0f;
    sD->effVol = 0.0f;
    hSndPkSetEVol(0);
    sD->envNum = 0;
    hSeLock(0);
    hSeInitGrp(0);
    sD->seMVol = 1.43f;
    sD->seObjID = 0;
    sD->bgmMVol = 0.708661437f;
    sD->pptMVol = 0.314960629f;
    sD->TitleDelayCnt = 0;

    hStrInit();
}

s32 *hSndBankSet(u8 *buf, int bank) {
    JAMHD *hdaddr;
    u8 *bdaddr;
    s32 hdsize;
    s32 bdsize;
    s32 *rpc;
    u8 stackShiz[16];
    
    if (bank != 0) {
        s16 *fhm = (s16 *)GetFHMAddress(buf, 2);
        s16 tblNum = *fhm;
        sD->stageTblNum = tblNum;
        memcpy(sD->stageTbl, fhm + 1, tblNum * 2);
    }

    hdaddr = (JAMHD *)GetFHMAddress(buf, 0);
    bdaddr = GetFHMAddress(buf, 1);
    hdsize = JamGetHdSize(hdaddr);
    bdsize = JamGetBdSize(hdaddr);
    hRpcSetDma((u8 *)(sD->iopBankAddr + bank * 0x2000), (u8 *)hdaddr, hdsize); // addr, data, size
    
    RpcArg[0] = bank;
    rpc = hRpc(IOP_JamBankSet);
    
    while (bdsize > 0) {
        s32 transsize;
        if (bdsize < 0x10000) {
            transsize = bdsize;
        } else {
            transsize = 0x10000;
        }
        
        hRpcSetDma((u8 *)(sD->iopBankAddr + 0x4000), bdaddr, transsize);
        bdsize -= 0x10000;
        bdaddr += 0x10000;
        rpc = hRpc(IOP_JamBdTrans);
    }
    
    return rpc;
}

void hSndBankSetCommon() {
    int ret;

    u8 *buf = getBuff(1, 0x200000, NULL, &ret);
    hCdReadKlPack(198, buf);
    buf = GetFHMAddress(buf, 2);
    FUN_001d37f8(1, 0x200000, NULL);
}

void hSndBankSetStage() {
    u8 *addr = hGetDataAddr(2);
    if (addr != NULL) {
        sD->stageBank = 1;
        hSndBankSet(addr, 1);
    } else {
        sD->stageBank = 0;
    }
}

void hSndEffSetArea() {
    EFXSE *se = &EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF][0];
    sD->effIdx = 0;
    sD->effVol = se->vol;
    sD->effVolBak = 0.0;
    sD->effMode = se->efx;
    sD->effDepth = 0x7fff;
    sD->effDelay = se->delay;
    sD->effFeed = se->feed;
    sD->effMix = se->dry | 2;
    hSndPkEffect();
    hSndPkSetEVol(0);
}

void hSndEffSetVolIdx(int idx) {
    EFXSE *se = EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF];
    sD->effIdx = idx;
    se = &se[idx];
    sD->effVol = se->vol;
}

void hSndEffSetVol_PPTstart() {
    EFXSE *se = EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF];
    se = &se[sD->effIdx];
    sD->effVol = se->vol_ppt;
}

void hSndEffSetVol_PPTend() {
    EFXSE *se = EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF];
    se = &se[sD->effIdx];
    sD->effVol = se->vol;
}

void hSndEffSetVol(float vol) {
    sD->effVol = vol;
}

void hSndSetStereo(SND_MODE i) {
    sD->Stereo = i;
}
