#include <stdio.h>
#include <unistd.h>
#include "common.h"

SYSGBL SysGbl;
GAME_WORK GameGbl;
FUNCTBL functbl[8] = {
    {nkFuncTbl, -1}
};

int main(int argc, char *argv[]) {
    memorySysFormat();
    hInitBoot();
    SysGbl.irqc = 0;
    init_config_system();
    *T0_MODE = T_MODE_CLKS_M | T_MODE_CUE_M;
    nkInitPad();
    OkMainInit(argc, argv);
    nkInitSys();
    hr_cold_start();
    hrpt_deb = 1;
    SysGbl.fmode = 1;
    GameGbl.vision = 0x1e00;
    nkDG.opflag |= 6;
    while (!sceGsSyncV(0));
    
    while (true) {
        hLoopTop();
        MainFunc();
        SysGbl.irqc++;
    }

    return 0;
}

void MainFunc() {
    if (functbl[SysGbl.nmode].func == NULL) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    } else {
        functbl[SysGbl.nmode].func[SysGbl.nsmode]();
    }
}

