#include "common.h"

//static char *bgmdebug_str[0] = {};
s32 TestMode = 0;
static s32 templist[26] = {
    24, 0,  1,  2,
    3,  4,  5,  6,
    7,  8,  9,  10,
    11, 12, 13, 14,
    15, 16, 17, 18,
    19, 20, 21, 22,
    23, 0
};
s32 (*hTestFuncTbl[2])() = {
    hTestInit,
    hTestMain
};
s32 TestWork = 0;

// Originally declared as void
s32 hTestInit() {

}

// Originally declared as void
s32 hTestMain() {

}
