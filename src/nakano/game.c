#include "common.h"

int (*GameFuncTbl[3])() = {
    GameInit,
    GameLoad,
    GameMain
};

int GameInit() {
    SysGbl.smode++;
    return 0;
}

int GameLoad() {
    return 0;
}

int GameMain() {
    return 0;
}
