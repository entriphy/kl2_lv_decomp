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

void hSndPkSetMVol(int voll, int volr) {
    sD->PkMax++;
    *sD->PkNum++ = 7;
    *sD->PkNum++ = (u8)voll;
    *sD->PkNum++ = (u8)(((u32)voll >> 8) & 0x7F);
    *sD->PkNum++ = (u8)volr;
    *sD->PkNum++ = (u8)(((u32)volr >> 8) & 0x7F);
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
