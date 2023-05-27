#include "common.h"

FUNCTBL nkFuncs[5] = {
    {nkInitAtrTbl, -1},
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
    if (nkFuncs[SysGbl.fmode].func == NULL) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    } else {
        nkFuncs[SysGbl.fmode].func[SysGbl.smode]();
    }
}
