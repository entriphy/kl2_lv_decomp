#include "common.h"

EFXSE *EfxSE[50][20] = {};
u8 SndPacket[1024] __attribute__((aligned(16)));
u8 SndTempBuff[1048576] __attribute__((aligned(16)));
hSNDDATA SndData = {};
hSNDDATA *sD = NULL;

s32 JamGetHdSize(JAMHD *hdaddr) {
    return hdaddr->Header.headerSize;
}

s32 JamGetBdSize(JAMHD *hdaddr) {
    return hdaddr->Header.bodySize;
}

void hSndPkEffect() {
    s32 i;
    
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

void hSndPkSetMVol(s32 voll, s32 volr) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_MVOLALL;
    *sD->PkNum++ = voll;
    *sD->PkNum++ = (voll >> 8) & 0x7F;
    *sD->PkNum++ = volr;
    *sD->PkNum++ = (volr >> 8) & 0x7F;
}

void hSndPkSetEVol(s32 vol) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_EVOL;
    *sD->PkNum++ = vol;
    *sD->PkNum++ = vol >> 8;
}

void hSndPkSetVol(s32 voice, f32 pan, f32 vol) {
    s32 p = pan * 16384.0f;
    s32 v = vol * 16384.0f;

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

void hSndPkSetPitch(s32 voice, s32 pitch) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_PITCH;
    *sD->PkNum++ = voice;
    *sD->PkNum++ = pitch;
    *sD->PkNum++ = pitch >> 8;
}

void hSndPkSetPalPitch(s32 voice) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_PALPITCH;
    *sD->PkNum++ = voice;
}

void hSndPkKeyOn(s32 voice, s32 flag, s32 bank, s32 prog, s32 splt, f32 pan, f32 vol) {
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

void hSndPkKeyOff(s32 voice) {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_KEYOFF;
    *sD->PkNum++ = voice;
}

void hSndPkKeyOffAll() {
    sD->PkMax++;
    *sD->PkNum++ = SNDCMD_KEYOFFALL;
}

s32 hSndPkGetSize() {
    // i have no clue how this matches
    s32 size = (s32)sD->PkNum - (s32)SndPacket;
    size = size < -1 ? size + 0x1E : size + 0xF;
    return (((s32)sD->PkNum - (s32)SndPacket + 0xF) / 0x10) * 0x10;
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

void hSndFadeOutAll(s32 frame) {
    hSeKeyOffAll();
    sD->fadeFlag = 1;
    sD->fadeCnt = 0.0f;
    sD->fadeMax = (f32)frame;
}

void hSndFadeInAll(s32 frame) {
    sD->fadeFlag = 3;
    sD->fadeCnt = 0.0f;
    sD->fadeMax = (f32)frame;
}

void hSndSetMVol(f32 vol) {
    sD->MVol = hSndFader(vol);
}

// Not matching
s32 hSndFader(f32 vol) {
    if (vol == 0.0f) {
        return 0;
    } else {
        hSNDDATA *snd;
        s32 n;
        
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
f32 hSndFader2(f32 vol) {
    f32 f = 0.0f;
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
    ((short *)SndPacket)[0] = sD->PkMax;
    hRpc(0x2a000001);
    sD->PkNum = SndPacket + 2;
    sD->PkMax = 0;
    sD->VoiceStat[0] |= sD->KeyonV[0];
    sD->VoiceStat[1] |= sD->KeyonV[1];
    sD->KeyonV[0] = 0;
    sD->KeyonV[1] = 0;
    hSeObjMain();
    sD->effChange = 0;
}

void hSndInit() {
    sceSdRemoteInit();
    
    sD = &SndData;
    sD->PkNum = &SndPacket[2];
    sD->PkMax = 0;
    sD->iopBankAddr = (s32 *)hRpc(IOP_SndInit);

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

s32* hSndBankSet(s32 packaddr, s32 bank) {
    JAMHD *hdaddr;
    u8 *bdaddr;
    s32 hdsize;
    s32 bdsize;
    s32 *rpc;
    u8 stackShiz[16];
    
    if (bank != 0) {
        s16 *fhm = (s16 *)GetFHMAddress((u32 *)packaddr, 2);
        s16 tblNum = *fhm;
        sD->stageTblNum = tblNum;
        memcpy(sD->stageTbl, fhm + 1, tblNum * 2);
    }

    hdaddr = (JAMHD *)GetFHMAddress((u32 *)packaddr, 0);
    bdaddr = (u8 *)GetFHMAddress((u32 *)packaddr, 1);
    hdsize = JamGetHdSize(hdaddr);
    bdsize = JamGetBdSize(hdaddr);
    hTrans2IOP((s32)(sD->iopBankAddr + bank * 0x2000), (s32)hdaddr, hdsize); // addr, data, size
    
    RpcArg[0] = bank;
    rpc = (s32 *)hRpc(IOP_JamBankSet);
    
    while (bdsize > 0) {
        s32 transsize;
        if (bdsize < 0x10000) {
            transsize = bdsize;
        } else {
            transsize = 0x10000;
        }
        
        hTrans2IOP((s32)(sD->iopBankAddr + 0x4000), (s32)bdaddr, transsize);
        bdsize -= 0x10000;
        bdaddr += 0x10000;
        rpc = (s32 *)hRpc(IOP_JamBdTrans);
    }
    
    return rpc;
}

void hSndBankSetMain() {
    s32 ret;

    u32 *buf = (u32 *)getBuff(1, 0x200000, NULL, &ret);
    hCdReadDataBlock(198, (s32)buf);
    buf = GetFHMAddress(buf, 2);
    hSndBankSet((s32)buf, 0);
    freeBuff(1, 0x200000, NULL);
}

void hSndBankSetStage() {
    s32 addr = hGetDataAddr(2);
    if (addr != 0) {
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

void hSndEffSetVolIdx(s32 idx) {
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

void hSndEffSetVol(f32 vol) {
    sD->effVol = vol;
}

void hSndSetStereo(SND_MODE i) {
    sD->Stereo = i;
}
