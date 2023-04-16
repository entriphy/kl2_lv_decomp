#include <math.h>
#include <sifrpc.h>
#include "hoshino.h"
#include "data_symbols.h"
#include "function_symbols.h"

void hSndSetMVol(float vol) {
    sD->MVol = hSndFader(vol);
}

int hSndFader(float vol) {
    if (vol == 0.0f) {
        return 0;
    }
    if (vol > 1.0f) {
        return 1.0;
    }

    int n = (int)(powf(10.0f, ((1.0f - vol) * sD->dBfader + sD->log10Volume) / 20.0f));
    if (n > 0x3FFF) {
        n = 0x3FFF;
    }
    if (n < -0x4000) {
        n = -0x4000;
    }

    return n;
}

float hSndFader2(float vol) {
    if (vol != 0.0f) {
        if (vol > 1.0f) {
            vol = 1.0f;
        }
        float f = powf(10.0, ((1.0f - vol) * sD->dBfader) * 20.0f);
        return f;
    }
    return 0.0f;
}

void hSndPkSetMVol(int voll, int volr) {
    sD->PkMax++;
    *sD->PkNum++ = 7;
    *sD->PkNum++ = voll;
    *sD->PkNum++ = (voll >> 8) & 0x7F;
    *sD->PkNum++ = volr;
    *sD->PkNum++ = (volr >> 8) & 0x7F;
}

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

void hSndPkSetEVol(int vol) {
    sD->PkMax++;
    *sD->PkNum++ = 9;
    *sD->PkNum++ = vol;
    *sD->PkNum++ = vol >> 8;
}

void hSndPkKeyOffAll() {
    sD->PkMax++;
    *sD->PkNum++ = 2;
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

int hSndPkGetSize() {
    // i have no clue how this matches
    int size = (s32)sD->PkNum - (s32)SndMainBuffer;
    size = size < -1 ? size + 0x1E : size + 0xF;
    return (((s32)sD->PkNum - (s32)SndMainBuffer + 0xF) / 0x10) * 0x10;
}

void hSndBankSetStage(void) {
    u8 *addr = hGetDataAddr(2);
    if (addr != NULL) {
        sD->stageBank = 1;
        hSndBankSet(addr, 1);
    } else {
        sD->stageBank = 0;
    }
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
    u8 *hdaddr;
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

    hdaddr = GetFHMAddress(buf, 0);
    bdaddr = GetFHMAddress(buf, 1);
    hdsize = JamGetHdSize(hdaddr);
    bdsize = JamGetBdSize(hdaddr);
    hRpcSetDma((u8 *)(sD->iopBankAddr + bank * 0x2000), hdaddr, hdsize); // addr, data, size
    
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
