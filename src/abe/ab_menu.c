#include "common.h"

static MENU_WORK MenuGbl;
s32 (*abMenuFuncTbl[2])() = {
    abMenuInit,
    abMenuMain
};

s32 abMenuInit() {
    SysGbl.smode = SysGbl.smode + 1;
    MenuGbl.mode = 0;
    MenuGbl.vision = 0;
    MenuGbl.flag = 0;
    MenuGbl.vscnt = 0;
    sceGsResetGraph(0, SCE_GS_INTERLACE, SCE_GS_NTSC, SCE_GS_FRAME);
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ16S, SCE_GS_CLEAR);
    FlushCache(WRITEBACK_DCACHE);
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 16, 8);
    OkPrintSysInit(5, SCR_WIDTH, SCR_HEIGHT, 32, 12);
    return 0;
}

s32 abMenuMain() {
    s32 lp1;
    s32 sy;
    kPadDATA *kpd;
    s32 inter;
    s32 vs1;
	s32 vs2;
    s32 val;
    
    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.fr++;
    sceGsSyncPath(0, 0);
    nkGetPad();
    kpd = &GameGbl.kpd[0];
    kpd->lvl = nkGetPlvl(0);
    kpd->trg = nkGetPtrg(0);
    kpd->rep = nkGetPrep(0);
    FlushCache(WRITEBACK_DCACHE);

    for (lp1 = 0; lp1 < 4; lp1++) {
        // Stubbed print
    }

    if (MenuGbl.mode == 0) {
        s32 vs1 = GameGbl.vision / 256;
        s32 vs2 = GameGbl.vision & 0xFF;
        
        if ((kpd->trg & 8) != 0)
            MenuGbl.vscnt = (MenuGbl.vscnt + 1) % 2;
        if ((kpd->trg & 4) != 0)
            MenuGbl.vscnt = (MenuGbl.vscnt + 1) % 2;

        if (kpd->rep & 0x2000) {
            switch (MenuGbl.vscnt) {
                case 0:
                    vs1++;
                    if (vs1 > 99)
                        vs1 = 0;
                    break;
                case 1:
                    vs2++;
                    vs2 &= 0xFF;
                    break;
            }
        } else if (kpd->rep & 0x8000) {
            switch (MenuGbl.vscnt) {
                case 0:
                    vs1--;
                    if (vs1 < 0)
                        vs1 = 99;
                    break;
                case 1:
                    vs2 += 0xFF;
                    vs2 &= 0xFF;
                    break;
            }
        }
        
        GameGbl.vision = (vs1 << 0x8) + vs2;
    }

    if ((SysGbl.irqc & 0x20) != 0 || MenuGbl.vscnt != 0 || (GameGbl.kpd[0].lvl & 0xA000) != 0) {
        // Stubbed print
    }

    if ((SysGbl.irqc & 0x20) != 0 || MenuGbl.vscnt != 1 || (GameGbl.kpd[0].lvl & 0xA000) != 0) {
        // Stubbed print
    }
    
    if (MenuGbl.mode > 0 && (kpd->trg & 0x1000) != 0) {
        MenuGbl.mode--;
    } else if (MenuGbl.mode < 3 && (kpd->trg & 0x4000) != 0) {
        MenuGbl.mode++;
    }
    
    if (((kpd->trg & 0x800) != 0) && (MenuGbl.mode < 4)) {
        SysGbl.fmode = MenuGbl.mode + 1;
        SysGbl.smode = 0;
    }
    if ((kpd->trg & 2U) != 0) {
        SysGbl.nmode = 5;
        SysGbl.nsmode = 0;
    }
    if ((kpd->trg & 1U) != 0) {
        SysGbl.nmode = 3;
        SysGbl.nsmode = 0;
    }
    
    sceGsSyncPath(0, 0);
    OkPFontFlush((GameGbl.kpd[0].trg & 0x80) && (GameGbl.kpd[0].lvl & 0x20));
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
    
    return 0;
}
