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

void hSndBankSetStage() {
    u8 *soundbank = hGetDataAddr(2);
    if (soundbank != NULL) {
        sD->stageBank = 1;
        hSnd_0016d5f0(soundbank, 1);
    } else {
        sD->stageBank = 0;
    }
}
