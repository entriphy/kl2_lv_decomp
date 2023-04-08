#include "common.h"

int hGameReadOK() {
    return isLoading();
}

u8 *hGetDataAddr(int i) {
    u8 *addr = GetFHMAddress(tblMax, i);
    return *((s32 *)addr) == -1 ? NULL : addr;
}

u8 *FUN_00167c80(int i) {
    u8 *addr = GetFHMAddress(areaBuff, i);
    return *((s32 *)addr) == -1 ? NULL : addr;
}

int FUN_00167bd0(int param_1) {
    return FUN_00166128((GameGbl.vision >> 7 & 0x1FE) + param_1) << 0xB;
}

void FUN_00167c00(int param_1, u8 *param_2) {
    tblMax = param_2;
    hCdPushKlPack((GameGbl.vision >> 7) + param_1, param_2);
}

void hInitStage0() {
    gD = &GameData;
    hSndReset();
    hSeLock(0);
    hSndBankSetStage();
    hSndSetMVol(0.0f);
    gD->BGMmode = BGMMODE_TOP;
    gD->BGMppt = 0;
    gD->resFlag = 0;
}

void FUN_00167c20(u8 *param_1) {
    areaBuff = param_1;
    hCdReadKlPack(199, param_1);
}
