#include "take/t_menu.h"
#include "nakano/nkpad.h"
#include "okanoyo/okprint.h"

static s32 tkMenuInit();
static s32 tkMenuMain();

s32 (*tkMenuFuncTbl[])() = {
    tkMenuInit,
    tkMenuMain
};
MENU_WORK MenuGbl;

static s32 tkMenuInit() {
    SysGbl.smode++;
    MenuGbl.mode = 0;
    MenuGbl.vision = 0;
    MenuGbl.flag = 0;
    MenuGbl.vscnt = 0;
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    FlushCache(WRITEBACK_DCACHE);
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 12, 6);
    OkPrintSysInit(7, SCR_WIDTH, SCR_HEIGHT, 12, 6);
    // Return intentionally left blank
}

static s32 tkMenuMain() {
    static struct {
		u8 *str0;
		u8 *str1;
		s32 fmode;
		s32 smode;
	} MenuCont[3] = {
        { "VIEWER", "_VIEWER", 0, 0 },
        { "EFFECT", "_EFFECT", 1, 0 },
        { "TITLE",  "_TITLE",  2, 0 }
    };
	s32 lp1;
	s32 sy;
	kPadDATA *kpd;
	s32 inter;

    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.fr++;
    sceGsSyncPath(0, 0);
    nkGetPad();
    kpd = &GameGbl.kpd[0];
    kpd->lvl = nkGetPlvl(0);
    kpd->trg = nkGetPtrg(0);
    kpd->rep = nkGetPrep(0);
    FlushCache(WRITEBACK_DCACHE);

    for (lp1 = 0; lp1 < 3; lp1++) {
        // Stubbed print
    }

    if (MenuGbl.mode > 0 && pPAD_TRG_UP(kpd)) {
        MenuGbl.mode--;
    } else if (MenuGbl.mode < 2 && pPAD_TRG_DOWN(kpd)) {
        MenuGbl.mode++;
    }

    if (pPAD_TRG_START(kpd) && (MenuGbl.mode < 3)) {
        SysGbl.fmode = MenuGbl.mode + 1;
        SysGbl.smode = 0;
    }
    if (pPAD_TRG_R2(kpd)) {
        SysGbl.nmode = 7;
        SysGbl.nsmode = 0;
    }
    if (pPAD_TRG_L2(kpd)) {
        SysGbl.nmode = 5;
        SysGbl.nsmode = 0;
    }

    sceGsSyncPath(0,0);
    OkPFontFlush(PAD_TRG_SQUARE(GameGbl.kpd[0]) && PAD_LVL_CIRCLE(GameGbl.kpd[0]));
    inter = sceGsSyncV(0) ^ 1;
    if (GameGbl.fr & 1) {
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
