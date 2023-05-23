#include "common.h"

int DAT_00639770;
int DAT_00639774;
int DAT_00639778;
int DAT_0063977c;
int DAT_00639780;
int DAT_00639784;
int DAT_006397c8;
int DAT_006397cc;
int DAT_006397d0;
int DAT_006397d8;
int DAT_006397dc;

float DAT_006397e0;
float DAT_006397e8;
float DAT_006397ec;
float DAT_006397f0;

int (*nkInitAtrTbl[2])() = {
    nkInitAtrInit,
    nkInitAtrMain
};

int nkInitAtrInit() {
    SysGbl.smode++;
    DAT_00639770 = 0;
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
    DAT_006397c8 = 0;
    DAT_006397cc = 0;
    DAT_006397d0 = 0;
    DAT_006397d8 = 0;
    DAT_006397dc = 0;
    DAT_006397e0 = 1000.0f;
    DAT_006397e8 = M_PI / 90.0f;
    DAT_006397ec = -0.017453292f;
    DAT_006397f0 = 0.017453292f;
    return 0;
}

int nkInitAtrMain() {
    return 0;
}
