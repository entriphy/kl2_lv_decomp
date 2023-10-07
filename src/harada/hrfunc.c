#include "harada/hrfunc.h"
#include "harada/hr_vpov.h"
#include "harada/hr_mapv.h"
#include "harada/hrmenu.h"

static FUNCTBL MainFuncTbl[5] = {
    { hrMenuFuncTbl, -1 },
    { hrMapVFuncTbl, -1 },
    { NULL,          -1 },
    { HrPtFuncTbl,   -1 },
    { hrVpoVFuncTbl, -1 }
};

s32 (*hrFuncTbl[2])() = {
    hrInit,
    hrMain
};

s32 hrInit() {
    SysGbl.nsmode++;
    printf("hrInit\n");
    // Return intentionally left blank
}

s32 hrMain() {
    s32 (**subfunctbl)() = MainFuncTbl[SysGbl.fmode].func;
    if (subfunctbl != NULL) {
        subfunctbl[SysGbl.smode]();
    } else {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }
    // Return intentionally left blank
}