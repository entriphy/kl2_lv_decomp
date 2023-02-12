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

void hSndPkSetMVol(int voll, int volr) {
    sD->PkMax++;
    *sD->PkNum++ = 7;
    *sD->PkNum++ = (u8)voll;
    *sD->PkNum++ = (u8)(((u32)voll >> 8) & 0x7F);
    *sD->PkNum++ = (u8)volr;
    *sD->PkNum++ = (u8)(((u32)volr >> 8) & 0x7F);
}

u32 JamGetHdSize(void *hdaddr) {
    return *((u32 *)hdaddr + 4);
}

u32 JamGetBdSize(void *hdaddr) {
    return *((u32 *)hdaddr + 8);
}

void hSndPkEffect() {
    sD->effChange = 1;
    for (int i = 0; i < 2; i++) {
        sD->PkMax++;
        *sD->PkNum++ = 8;
        *sD->PkNum++ = (u8)i;
        *sD->PkNum++ = (u8)sD->effMode;
        *sD->PkNum++ = (u8)sD->effDepth;
        *sD->PkNum++ = (u8)((u32)sD->effDepth >> 8);
        *sD->PkNum++ = (u8)sD->effDepth;
        *sD->PkNum++ = (u8)((u32)sD->effDepth >> 8);
        *sD->PkNum++ = (u8)sD->effDelay;
        *sD->PkNum++ = (u8)sD->effFeed;
    }
}

void hSndPkSetEVol(int vol) {
    sD->PkMax++;
    *sD->PkNum++ = 9;
    *sD->PkNum++ = (u8)vol;
    *sD->PkNum++ = (u8)((u32)vol >> 8);
}

int hSndPkGetSize() {
    // Pretty sure it's subtracting a pointer here, might need to fix it
    int size = (int)(sD->PkNum - 4289265);
    if (size < 0) {
        size = (int)(sD->PkNum - 4289250);
    }
    return size;
}