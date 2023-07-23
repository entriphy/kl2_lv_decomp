#include "h_file.h"
#include "h_cdvd.h"

static s32 GameDataTop;
static s32 SysDataTop;

s32 hGameDataSize(s32 mode) {
    return hCdGetSize((GameGbl.vision >> 7 & 0x1FE) + mode) << 0xB;
}

void hGameRead(s32 mode, s32 buff) {
    GameDataTop = buff;
    hCdReadData((GameGbl.vision >> 7 & 0x1FE) + mode, buff);
}

void hSysDataRead(s32 buff) {
    SysDataTop = buff;
    hCdReadDataBlock(199, buff);
}

s32 hGameReadOK() {
    return hCdReadSync();
}

s32 hGetDataAddr(s32 i) {
    s32 *addr = GetFHMAddress((u32 *)GameDataTop, i);
    return *addr == -1 ? 0 : (s32)addr;
}

s32 hGetSysDataAddr(s32 i) {
    s32 *addr = GetFHMAddress((u32 *)SysDataTop, i);
    return *addr == -1 ? 0 : (s32)addr;
}
