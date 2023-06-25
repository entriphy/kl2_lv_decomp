#include "common.h"

static FUNCTBL NameFuncTbl[8] = {
    {nkFuncTbl, -1},
    {}, // OkFuncTbl
    {}, // HFuncTbl
    {}, // hrFuncTbl
    {abFuncTbl, -1},
    {}, // htFuncTbl
    {}, // tkFuncTbl
    {}  // kzFuncTbl
};

s32 MainFunc() {
    s32 (**subfunctbl)();

    subfunctbl = NameFuncTbl[SysGbl.nmode].func;
    if (subfunctbl != NULL) {
        subfunctbl[SysGbl.nsmode]();
    } else {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }
    return 0;
}
