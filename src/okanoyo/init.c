#include "common.h"

void* DAT_0038a138;
OKDEBDATAEX testdb;
PROFILE profile;
OKDEBDATA okDebData;
PERSONAL_DEB PDbuf[9];
OKPROFILE okProfile;
int OkZakoFuncOn;
long profileFlag;
int fontDispID;

void OkPrintSysInit(int personal_id, int screenw, int screenh, int fontw, int fonth) {
    PDbuf[personal_id].fsz_x = fontw;
    PDbuf[personal_id].fsz_y = fonth;
    PDbuf[personal_id].scr_x = screenw / fontw;
    PDbuf[personal_id].scr_y = screenh / fonth;
}

void OkDefaultSysInit(int screenw, int screenh, int fontw, int fonth) {
    for (int i = 0; i < 9; i++) {
        PDbuf[i].fsz_x = fontw;
        PDbuf[i].fsz_y = fonth;
        PDbuf[i].scr_x = screenw;
        PDbuf[i].scr_y = screenh;
        for (int j = 0; j < 280; j++) {
            // what
        }
    }
}

void OkMainInit(int argc, char **argv) {
    // TODO
    OkZakoFuncOn = 1;
    DAT_0038a138 = NULL;
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

    // OkPFontFlush?(0x3e00);
    // FUN_001da520();
    OkDefaultSysInit(640, 224, 16, 8);
    // FUN_001dacb0();
    fontDispID = 0;
    GameGbl.vision = 0x100;
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
