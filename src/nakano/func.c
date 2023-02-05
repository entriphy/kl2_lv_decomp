#include "common.h"

FUNCTBL nkFuncs[5] = {
    {nkInitAtrTbl, -1},
    // {GameFuncTbl, -1},
    {NULL, 0},
    {NULL, 0},
    {NULL, 0},
    {NULL, 0}
};

int (*nkFuncTbl[2])() = {
    nkInit,
    nkMain
};

int nkInit() {
    SysGbl.nsmode++;
    return 0;
}

int nkMain() {
    if (nkFuncs[SysGbl.fmode].func == NULL) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    } else {
        nkFuncs[SysGbl.fmode].func[SysGbl.smode]();
    }
}
