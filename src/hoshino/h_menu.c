#include "common.h"

static s32 hMenuInit();
static s32 hMenuMain();

s32 (*hMenuFuncTbl[2])() = {
    hMenuInit,
    hMenuMain
};
hGLOBAL hG;
s32 RpcArg[16] __attribute__((aligned(16)));
s128 hPacketArea[2048] __attribute__((aligned(16)));
static MENU_WORK MenuGbl;

static s32 hMenuInit() {
    // TODO
}

static s32 hMenuMain() {
    // TODO
}
