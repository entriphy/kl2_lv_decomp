#include "common.h"

int (*nkFuncTbl[2])() = {
    nkInit,
    nkMain
};

int nkInit() {
    SysGbl.nsmode++;
    return 0;
}

int nkMain() {
    // TODO
    return 0;
}
