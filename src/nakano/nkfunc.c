#include "nkfunc.h"
#include "menu.h"
#include "game.h"

static FUNCTBL MainFuncTbl[5] = {
    {MenuFuncTbl, -1},
    {GameFuncTbl, -1},
    {NULL, 0},
    {NULL, 0},
    {NULL, 0}
};

s32 (*nkFuncTbl[2])() = {
    nkInit,
    nkMain
};

s32 nkInit() {
    SysGbl.nsmode++;
    return 0;
}

s32 nkMain() {
    s32 (**subfunctbl)() = MainFuncTbl[SysGbl.fmode].func;
    if (subfunctbl != NULL) {
        subfunctbl[SysGbl.smode]();
    } else {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }
    return 0;
}
