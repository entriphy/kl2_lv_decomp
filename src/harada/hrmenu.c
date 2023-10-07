#include "harada/hrmenu.h"
#include "harada/hr_pall.h"
#include "harada/hr_pflag.h"
#include "okanoyo/okprint.h"
#include "nakano/nkpad.h"

static s32 hrMenuInit();
static s32 hrMenuMain();

s32 (*hrMenuFuncTbl[2])() = {
    hrMenuInit,
    hrMenuMain
};
static MENU_WORK MenuGbl;
static s8 D_3FA730;

static s32 hrMenuInit() {
    SysGbl.smode++;
    MenuGbl.mode = 0;
    MenuGbl.vision = 0;
    MenuGbl.flag = 0;
    MenuGbl.vscnt = 0;
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    FlushCache(WRITEBACK_DCACHE);
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 0x10, 8);
    OkPrintSysInit(4, SCR_WIDTH, SCR_HEIGHT, 0x20, 12);
    hr_pt_fclear();
    hrpt_pack = (u32 *)-1;
    D_3FA730 = 1;
    // Return intentionally left blank
}

static s32 hrMenuMain() {
    static struct {
        u8 *str0;
        u8 *str1;
        s32 fmode;
        s32 smode;
    } MenuCont[5] = {
        { "MAP",  "_MAPVIEWER", 1, 0 },
        { "VU1V", "_TEST1",     1, 0 },
        { "PT",   "_TEST2",     2, 0 },
        { "VPO",  "_TEST3",     3, 0 },
        { "LANG", "_TEST4",     4, 0 }
    };

    s32 lp1;
    s32 sy;
    kPadDATA *kpd;
    s32 inter;
    s32 vs1;
    s32 vs2;
    s32 pmode;

    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.fr++;
    sceGsSyncPath(0, 0);

    kpd = &GameGbl.kpd[0];
    nkGetPad();
    kpd->lvl = nkGetPlvl(0);
    kpd->trg = nkGetPtrg(0);
    kpd->rep = nkGetPrep(0);
    FlushCache(0);

    for (lp1 = 0; lp1 < 5; lp1++) {
        // Stubbed print
    }

    if (MenuGbl.mode == 0) {
        vs1 = GameGbl.vision / 256;
        vs2 = GameGbl.vision & 0xFF;

        if (pPAD_TRG_R1(kpd)) {
            MenuGbl.vscnt++;
            MenuGbl.vscnt %= 2;
        }
        if (pPAD_TRG_L1(kpd)) {
            MenuGbl.vscnt++;
            MenuGbl.vscnt %= 2;
        }

        if (pPAD_REP_RIGHT(kpd)) {
            switch (MenuGbl.vscnt) {
                case 0:
                    vs1++;
                    if (vs1 > 99) {
                        vs1 = 0;
                    }
                    break;
                case 1:
                    vs2++;
                    vs2 &= 0xFF;
                    break;
            }
        } else if (pPAD_REP_LEFT(kpd)) {
            switch (MenuGbl.vscnt) {
                case 0:
                    vs1--;
                    if (vs1 < 0) {
                        vs1 = 99;
                    }
                    break;
                case 1:
                    vs2 += 0xFF;
                    vs2 &= 0xFF;
                    break;
            }
        }
        GameGbl.vision = (vs1 << 8) + vs2;
    }

    // i don't know man
    if (((volatile SYSGBL *)&SysGbl)->irqc & 0x20) {
        vs1 = GameGbl.vision / 256;
        vs2 = GameGbl.vision & 0xFF;
        ((volatile SYSGBL *)&SysGbl)->irqc;
    }

    if (MenuGbl.mode == 1) {
        char *vu1v_name[3] = { "harada", "user_1", "user_2" };

        if (pPAD_REP_RIGHT(kpd)) {
            MenuGbl.vscnt++;
            if (MenuGbl.vscnt > 2) {
                MenuGbl.vscnt = 0;
            }
        } else if (pPAD_REP_LEFT(kpd)) {
            MenuGbl.vscnt--;
            if (MenuGbl.vscnt < 0) {
                MenuGbl.vscnt = 2;
            }
        }
    }

    if (MenuGbl.mode == 2) {
        if (pPAD_REP_RIGHT(kpd)) {
            MenuGbl.vscnt++;
            if (MenuGbl.vscnt > 9) {
                MenuGbl.vscnt = 0;
            }
        } else if (pPAD_REP_LEFT(kpd)) {
            MenuGbl.vscnt--;
            if (MenuGbl.vscnt < 0) {
                MenuGbl.vscnt = 9;
            }
        }

        if (kpd->trg & 0xC) { // L1/R1
            D_3FA730 ^= 1;
        }
    }

    if (MenuGbl.mode == 4) {
        if (pPAD_REP_RIGHT(kpd)) {
            SysGbl.Language++;
            if (SysGbl.Language > 5) {
                SysGbl.Language = 1;
            }
        } else if (pPAD_REP_LEFT(kpd)) {
            SysGbl.Language--;
            if (SysGbl.Language < 1) {
                SysGbl.Language = 5;
            }
        }
    }

    {
        char name[4];
        switch (SysGbl.Language) {
            case 1:
                strcpy(name, "ENG");
                break;
            case 2:
                strcpy(name, "FRN");
                break;
            case 3:
                strcpy(name, "SPA");
                break;
            case 4:
                strcpy(name, "GET");
                break;
            case 5:
                strcpy(name, "ITA");
                break;
        }
    }

    if (MenuGbl.mode > 0 && pPAD_TRG_UP(kpd)) {
        MenuGbl.mode--;
        MenuGbl.vscnt = 0;
    } else if (MenuGbl.mode < 4 && pPAD_TRG_DOWN(kpd)) {
        MenuGbl.mode++;
        MenuGbl.vscnt = 0;
    }

    if (pPAD_TRG_START(kpd) && MenuGbl.mode < 5) {
        SysGbl.fmode = MenuGbl.mode + 1;
        SysGbl.smode = 0;
        if (MenuGbl.mode == 2) {
            hr_pt_set(1, MenuGbl.vscnt, D_3FA730 + 1, 1);
            if (pPAD_LVL_CIRCLE(kpd)) {
                pmode = 1;
            } else if (pPAD_LVL_CROSS(kpd)) {
                pmode = 2;
            } else if (pPAD_LVL_SQUARE(kpd)) {
                pmode = 3;
            } else {
                pmode = kpd->lvl >> 0x2;
                pmode &= 4;
            }
            hr_pflag_theatar(hr_pflag_get_id(), pmode);
        }
    }

    if (pPAD_TRG_R2(kpd)) {
        SysGbl.nmode = 4;
        SysGbl.nsmode = 0;
    }
    if (pPAD_TRG_L2(kpd)) {
        SysGbl.nmode = 2;
        SysGbl.nsmode = 0;
    }

    sceGsSyncPath(0,0);
    OkPFontFlush(PAD_TRG_SELECT(GameGbl.kpd[1]));
    inter = sceGsSyncV(0) ^ 1;
    if ((GameGbl.fr & 1) != 0) {
        sceGsSetHalfOffset(&GameGbl.db.draw11, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw12, 0x800, 0x800, inter);
    } else {
        sceGsSetHalfOffset(&GameGbl.db.draw01, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw02, 0x800, 0x800, inter);
    }

    FlushCache(0);
    sceGsSyncPath(0, 0);
    // Return intentionally left blank
}