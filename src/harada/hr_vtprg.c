#include "harada/hr_vtprg.h"

void hrVT_allwaveErase() {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (kitGetWaveDrawFlag(i) != -1) {
            kitWaveDrawDisable(i);
        }
    }
}

void hrVT_waveSet(s32 id) {
    if (kitGetWaveDrawFlag(id) != -1) {
        kitWaveDrawEnable(id);
    }
}
