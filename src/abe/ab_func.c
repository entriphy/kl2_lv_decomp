#include "ab_func.h"
#include "ab_menu.h"
#include "firetest.h"
#include "sample.h"

static FUNCTBL MainFuncTbl[5] = {
    {abMenuFuncTbl,     -1},
    {abFiretestFuncTbl, -1},
    {abFiretestFuncTbl, -1},
    {abFiretestFuncTbl, -1},
    {abSampleFuncTbl,   -1}
};

s32 (*abFuncTbl[2])() = {
    abInit,
    abMain
};

s32 abInit() {
    SysGbl.nsmode++;
    return 0;
}

s32 abMain() {
	s32 (**subfunctbl)() = MainFuncTbl[SysGbl.fmode].func;
    if (subfunctbl != NULL) {
        subfunctbl[SysGbl.smode]();
    } else {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }
    return 0;
}
