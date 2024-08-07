#include "take/t_func.h"
#include "take/t_menu.h"
#include "take/viewer.h"

// TODO: uncomment these
FUNCTBL MainFuncTbl[] = {
    { tkMenuFuncTbl, -1 },
    { tkViewerFuncTbl, -1 },
    // { tkTornadoFuncTbl, -1 },
    // { tkTitleFuncTbl, -1 }
};

s32 (*tkFuncTbl[])() = {
    tkInit,
    tkMain
};

s32 tkInit() {
    SysGbl.nsmode++;
    // No return
}

s32 tkMain() {
	s32 (**subfunctbl)() = MainFuncTbl[SysGbl.fmode].func;
    if (subfunctbl != NULL) {
        subfunctbl[SysGbl.smode]();
    } else {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }
    // No return
}