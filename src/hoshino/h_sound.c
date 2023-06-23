#include "common.h"

EFXSE *EfxSE[50][20] = {};
u8 SndPacket[1024] __attribute__((aligned(16)));
u8 SndTempBuff[1048576] __attribute__((aligned(16)));
hSNDDATA SndData = {};
hSNDDATA *sD = NULL;

s32 JamGetHdSize(s32 hdaddr) {
    return ((s32 *)hdaddr)[7];
}

s32 JamGetBdSize(s32 hdaddr) {
    return ((s32 *)hdaddr)[8];
}

void hSndPkEffect() {
    s32 core;
    
    sD->effChange = 1;
    for (core = 0; core < 2; core++) {
        sD->PkNum++;
        *sD->Pk++ = 8;
        *sD->Pk++ = core;
        *sD->Pk++ = sD->effMode;
        *sD->Pk++ = sD->effDepth;
        *sD->Pk++ = sD->effDepth >> 8;
        *sD->Pk++ = sD->effDepth;
        *sD->Pk++ = sD->effDepth >> 8;
        *sD->Pk++ = sD->effDelay;
        *sD->Pk++ = sD->effFeed;
    }
}

void hSndPkSetMVol(s32 voll, s32 volr) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_MVOLALL;
    *sD->Pk++ = voll;
    *sD->Pk++ = (voll >> 8) & 0x7F;
    *sD->Pk++ = volr;
    *sD->Pk++ = (volr >> 8) & 0x7F;
}

void hSndPkSetEVol(s32 vol) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_EVOL;
    *sD->Pk++ = vol;
    *sD->Pk++ = vol >> 8;
}

void hSndPkSetVol(s32 voice, f32 vollf, f32 volrf) {
    s32 voll;
    s32 volr;

    voll = vollf * 16384.0f;
    volr = volrf * 16384.0f;
    if (voll > 0x4000)
        voll = 0x4000;
    if (voll < -0x4000)
        voll = -0x4000;
    if (volr > 0x4000)
        volr = 0x4000;
    if (volr < -0x4000)
        volr = -0x4000;
    
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_VOL;
    *sD->Pk++ = voice;
    *sD->Pk++ = voll;
    *sD->Pk++ = voll >> 8;
    *sD->Pk++ = volr;
    *sD->Pk++ = volr >> 8;
}

void hSndPkSetPitch(s32 voice, s32 pitch) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_PITCH;
    *sD->Pk++ = voice;
    *sD->Pk++ = pitch;
    *sD->Pk++ = pitch >> 8;
}

void hSndPkSetPalPitch(s32 voice) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_PALPITCH;
    *sD->Pk++ = voice;
}

void hSndPkKeyOn(s32 voice, s32 flag, s32 bank, s32 prog, s32 splt, f32 vollf, f32 volrf) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_KEYON;
    *sD->Pk++ = voice;
    *sD->Pk++ = flag;
    *sD->Pk++ = bank;
    *sD->Pk++ = prog;
    *sD->Pk++ = splt;
    sD->KeyonV[voice & 1] |= 1 << (voice >> 1);
    hSndPkSetVol(voice, vollf, volrf);
}

void hSndPkKeyOff(s32 voice) {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_KEYOFF;
    *sD->Pk++ = voice;
}

void hSndPkKeyOffAll() {
    sD->PkNum++;
    *sD->Pk++ = SNDCMD_KEYOFFALL;
}

int hSndPkGetSize() {
    s32 size;

    size = (s32)sD->Pk - (s32)SndPacket;
    return (size + 0xF) / 0x10 * 0x10;
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

s32 hSndFader(f32 vol) {
    s32 retvol;

    if (vol == 0.0f)
        return 0;
    if (vol > 1.0f)
        vol = 1.0f;
    vol = sD->dBfader * (1.0f - vol);
    vol = powf(10.0f, (vol + sD->log10Volume) / 20.0f);
    retvol = (s32)vol;
    if (retvol > 0x3FFF)
        retvol = 0x3FFF;
    if (retvol < -0x4000)
        retvol = -0x4000;

    return retvol;
}

f32 hSndFader2(f32 vol) {
    if (vol == 0.0f)
        return 0.0f;
    if (vol > 1.0f)
        vol = 1.0f;
    vol = sD->dBfader * (1.0f - vol);
    vol = powf(10.0f, vol / 20.0f);
    return vol;
}

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
    ((s16 *)SndPacket)[0] = sD->PkNum;
    hRpc(IOP_SndMain);
    sD->Pk = SndPacket + 2;
    sD->PkNum = 0;
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
    sD->Pk = &SndPacket[2];
    sD->PkNum = 0;
    sD->iopBankAddr = hRpc(IOP_SndInit);

    RpcArg[0] = 0xFFFFFF;
    RpcArg[1] = 0xFFFF;
    hRpc(IOP_SndMask);
    
    sD->PkMax = 0;
    sD->Stereo = SND_MODE_STEREO;
    sD->Mute = 0;
    sD->stageBank = 0;
    sD->dBfader = -30.0f;
    sD->log10Volume = (f32)log10(16367.0) * 20.0f;
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

void hSndBankSet(s32 packaddr, s32 id) {
    s32 hdaddr;
    s32 bdaddr;
    s32 hdsize;
    s32 bdsize;
    s32 transsize;
    s32 tbladdr;
    s32 tblsize;
    char stackShiz[0x10];
    
    if (id != 0) {
        tbladdr = (s32)GetFHMAddress((u32 *)packaddr, 2);
        tblsize = *(s16 *)tbladdr;
        sD->stageTblNum = tblsize;
        memcpy(sD->stageTbl, (void *)(tbladdr + 2), tblsize * 2);
    }

    hdaddr = (s32)GetFHMAddress((u32 *)packaddr, 0);
    bdaddr = (s32)GetFHMAddress((u32 *)packaddr, 1);
    hdsize = JamGetHdSize(hdaddr);
    bdsize = JamGetBdSize(hdaddr);
    hTrans2IOP(sD->iopBankAddr + id * 0x8000, hdaddr, hdsize);
    
    RpcArg[0] = id;
    hRpc(IOP_JamBankSet);
    
    while (bdsize > 0) {
        if (bdsize < 0x10000)
            transsize = bdsize;
        else
            transsize = 0x10000;

        hTrans2IOP(sD->iopBankAddr + 0x10000, bdaddr, transsize);
        bdsize -= 0x10000;
        bdaddr += 0x10000;
        hRpc(IOP_JamBdTrans);
    }
}

void hSndBankSetMain() {
    s32 buff;
    s32 i;

    buff = (s32)getBuff(1, 0x200000, NULL, &i);
    hCdReadDataBlock(198, buff);
    buff = (s32)GetFHMAddress((u32 *)buff, 2);
    hSndBankSet(buff, 0);
    freeBuff(1, 0x200000, NULL);
}

void hSndBankSetStage() {
    s32 addr;

    addr = hGetDataAddr(2);
    if (addr != 0) {
        sD->stageBank = 1;
        hSndBankSet(addr, 1);
    } else {
        sD->stageBank = 0;
    }
}

void hSndEffSetArea() {
    EFXSE *eff;

    eff = &EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF][0];
    sD->effIdx = 0;
    sD->effVol = eff->vol;
    sD->effVolBak = 0.0f;
    sD->effMode = eff->efx;
    sD->effDepth = 0x7fff;
    sD->effDelay = eff->delay;
    sD->effFeed = eff->feed;
    sD->effMix = eff->dry | 2;
    hSndPkEffect();
    hSndPkSetEVol(0);
}

void hSndEffSetVolIdx(s32 idx) {
    EFXSE *eff;

    eff = EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF];
    sD->effIdx = idx;
    eff = &eff[idx];
    sD->effVol = eff->vol;
}

void hSndEffSetVol_PPTstart() {
    EFXSE *eff;

    eff = EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF];
    eff = &eff[sD->effIdx];
    sD->effVol = eff->vol_ppt;
}

void hSndEffSetVol_PPTend() {
    EFXSE *eff;

    eff = EfxSE[GameGbl.vision >> 8 & 0xFF][GameGbl.vision & 0xFF];
    eff = &eff[sD->effIdx];
    sD->effVol = eff->vol;
}

void hSndEffSetVol(f32 vol) {
    sD->effVol = vol;
}

void hSndSetStereo(SND_MODE i) {
    sD->Stereo = i;
}
