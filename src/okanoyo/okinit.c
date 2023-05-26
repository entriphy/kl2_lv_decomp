#include "common.h"

s32 *pfdatap[6] = {
    &fontDispID,
    NULL,
    &nkDG.opflag,
    &nkDG.map_draw_flag,
    NULL
};
OKDEBDATAEX testdb = {};

void OkMainInit(s32 argc, char *argv[]) {
    // TODO
    OkZakoFuncOn = 1;
    prgBufPtr[0] = NULL;
    okDebData.debp = &testdb;
    okDebData.debp->okzcom_mode = 0;

    if (profileFlag & 1 != 0) {
        fontDispID = profile.fontID;
    }
    if (profileFlag & 2 != 0) {
        GameGbl.vision = (profile.menuID & 0xFF00U) + (profile.menuID & 0xFF00U) >> 0xC * -0x6000 + (profile.menuID & 0xFFU);
    }
    if (profileFlag & 8U != 0) {
        nkDG.opflag = okProfile.kloDmg;
    }
    if (profileFlag & 16U != 0) {
        nkDG.map_draw_flag = okProfile.plnMod;
    }

    // OkTexAllocInit(0x3e00);
    // OkPrintDataLoad();
    OkDefaultSysInit(640, 224, 16, 8);
    // OkHtimeClrAll();
    fontDispID = 0;
    GameGbl.vision = 0x0100;
    okDebData.debp->zak_com.tetton2_upspd = 0x1600;
    okDebData.debp->zak_com.tetton2_upmax = 0x5000;
    okDebData.debp->zak_com.tetton2_dwspd = 0x200;
    okDebData.debp->zak_com.tetton2_dwmax = 0x5800;
    okDebData.debp->zak_com.tetton2_mcspd = 0x300;
    okDebData.debp->zak_com.tetton2_mcmax = 0x3800;
    okDebData.debp->zak_com.tetton2_count = 0x1a0;
    okDebData.debp->zak_com.tetton2_ypmax = 0x4f0;
    okDebData.debp->zak_com.gomgumi_cntmax = 0x40;
    okDebData.debp->zak_com.gomgumi_ydown = 0x10000;
    okDebData.debp->zak_com.gomgumi_mdown = 0x10000;
    okDebData.debp->zak_com.gomgumi_nobih = 0x80000;
    okDebData.debp->zak_com.trmprn_upspd0 = 0;
    okDebData.debp->zak_com.trmprn_upspd1 = 0;
    okDebData.debp->zak_com.jetimo_upspd = 0;
    okDebData.debp->zak_com.jetimo_acctim = 0x32;
    okDebData.debp->zak_com.jetimo_livtim = 0;
    okDebData.debp->zak_com.jetimo_dietim = 0;
    okDebData.debp->zak_com.klonoa_dwmax = 0;
    okDebData.debp->zak_com.rocketmuu_pspd = 0;
}
