#include "common.h"

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
    if (MainFuncTbl[SysGbl.fmode].func == NULL) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    } else {
        MainFuncTbl[SysGbl.fmode].func[SysGbl.smode]();
    }
}
