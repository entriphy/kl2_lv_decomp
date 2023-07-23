#include "menu.h"
#include "dma.h"
#include "gssub.h"
#include "okanoyo/okprint.h"

typedef struct { // 0x18
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 vision;
    /* 0x08 */ s32 flag;
    /* 0x0c */ s32 vscnt;
    /* 0x10 */ s32 option;
    /* 0x14 */ s32 opmode;
} nkMENU_WORK; // Technically this should have the same name as MENU_WORK, but whateverrrrr duuuude

static nkMENU_WORK MenuGbl;
static FMATRIX VLmtx;
static FVECTOR theta;
static FVECTOR trans;
static FVECTOR dtheta;

s32 (*MenuFuncTbl[2])() = {
    MenuInit,
    MenuMain
};

s32 MenuInit() {
    SysGbl.smode++;
    MenuGbl.mode = 0; // MenuGbl
    MenuGbl.vision = 0;
    MenuGbl.flag = 0;
    MenuGbl.vscnt = 0;
    MenuGbl.option = 0;
    MenuGbl.opmode = 0;
    nkInitDma();
    // TODO
    // sceDmaSend(DmaChVIF1, &stuff);
    sceGsSyncV(0);
    nkGsInitFrm();
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 12, 6);
    OkPrintSysInit(2, SCR_WIDTH, SCR_HEIGHT, 12, 6);
    theta.x = 0.0f; // theta
    theta.y = 0.0f;
    theta.z = 0.0f;
    trans.x = 0.0f; // trans
    trans.y = 0.0f;
    trans.z = 1000.0f;
    dtheta.x = M_PI / 90.0f; // dtheta
    dtheta.y = -0.017453292f;
    dtheta.z = 0.017453292f;
    return 0;
}

s32 MenuMain() {
    // Stubbed in retail build
    return 0;
}
