#include "nakano/nkgf.h"

static u32 nkGameFlagD;
static u32 nkGameReStartFlagD;

u32 nkGfGetAll() {
    return nkGameFlagD;
}

void nkGfInit() {
    nkGameFlagD = nkGameReStartFlagD = 0;
}

void nkGfReStartInit() {
    nkGameFlagD = nkGameReStartFlagD;
}

void nkGfReStartSave() {
    nkGameReStartFlagD = nkGameFlagD;
};

void nkGfSetON(s32 id) {
    nkGameFlagD |= 1 << id;
}

void nkGfSetOFF(s32 id) {
    nkGameFlagD &= ~(1 << id);
}

s32 nkGfGet(s32 id) {
    if (nkGameFlagD & (1 << id)) {
        return 1;
    } else {
        return 0;
    }
}
