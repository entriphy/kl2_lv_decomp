#include "common.h"

s32 DAT_00639770;
s32 DAT_00639774;
s32 DAT_00639778;
s32 DAT_0063977c;
s32 DAT_00639780;
s32 DAT_00639784;
s32 DAT_006397c8;
s32 DAT_006397cc;
s32 DAT_006397d0;
s32 DAT_006397d8;
s32 DAT_006397dc;

f32 DAT_006397e0;
f32 DAT_006397e8;
f32 DAT_006397ec;
f32 DAT_006397f0;

s32 (*nkInitAtrTbl[2])() = {
    MenuInit,
    MenuMain
};

s32 MenuInit() {
    SysGbl.smode++;
    DAT_00639770 = 0; // MenuGbl
    DAT_00639774 = 0;
    DAT_00639778 = 0;
    DAT_0063977c = 0;
    DAT_00639780 = 0;
    DAT_00639784 = 0;
    nkInitDma();
    // TODO
    // sceDmaSend(DmaChVIF1, &stuff);
    sceGsSyncV(0);
    nkGsInitFrm();
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 12, 6);
    OkPrintSysInit(2, SCR_WIDTH, SCR_HEIGHT, 12, 6);
    DAT_006397c8 = 0; // theta
    DAT_006397cc = 0;
    DAT_006397d0 = 0;
    DAT_006397d8 = 0; // trans
    DAT_006397dc = 0;
    DAT_006397e0 = 1000.0f;
    DAT_006397e8 = M_PI / 90.0f; // dtheta
    DAT_006397ec = -0.017453292f;
    DAT_006397f0 = 0.017453292f;
    return 0;
}

s32 MenuMain() {
    return 0;
}
