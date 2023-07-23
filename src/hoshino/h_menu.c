#include "h_menu.h"
#include "nakano/nkpad.h"

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
    SysGbl.smode++;
    MenuGbl.mode = 0;
    MenuGbl.vision = 0;
    MenuGbl.flag = 0;
    MenuGbl.vscnt = 0;

    // Return intentionally left blank
}

static s32 hMenuMain() {
    static struct { // 0x10
        /* 0x000 */ u8 *str0;
        /* 0x004 */ u8 *str1;
        /* 0x008 */ s32 fmode;
        /* 0x00c */ s32 smode;
    } MenuCont[1];
    s32 lp1;
    s32 sy;
    kPadDATA *kpd;

    kpd = &GameGbl.kpd[0];
    nkGetPad();
    kpd->lvl = nkGetPlvl(0);
    kpd->trg = nkGetPtrg(0);
    kpd->rep = nkGetPrep(0);

    for (lp1 = 0; lp1 < 1; lp1++) {

    }

    if (MenuGbl.mode > 0 && pPAD_TRG_UP(kpd) != 0) {
        MenuGbl.mode--;
    } else if (MenuGbl.mode < 0 && pPAD_TRG_DOWN(kpd) != 0) {
        MenuGbl.mode++;
    }

    if (pPAD_TRG_START(kpd) && MenuGbl.mode < 1) {
        SysGbl.fmode = MenuGbl.mode + 1;
        SysGbl.smode = 0;
    }
    if (pPAD_TRG_R2(kpd)) {
        SysGbl.nmode = 3;
        SysGbl.nsmode = 0;
    }
    if (pPAD_TRG_L2(kpd)) {
        SysGbl.nmode = 1;
        SysGbl.nsmode = 0;
    }

    // Return intentionally left blank
}
