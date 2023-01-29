#include <ee_regs.h>
#include <stdio.h>
#include <unistd.h>
#include "common.h"

SYSGBL SysGbl;
GAME_WORK GameGbl;

int main(int argc, char *argv[]) {
    FUN_00318a80();
    memorySysFormat();
    hInitBoot();
    SysGbl.irqc = 0;
    init_config_system();
    *R_EE_T0_MODE = 0x83;
    nkInitPad();
    OkMainInit();
    nkInitSys();
    hr_cold_start();
    hrpt_deb = 1;
    SysGbl.fmode = 1;
    GameGbl.vision = 0x1e00;
    nkDG.opflag |= 6;
    GsVSync(0);

    sleep(5);
    return 0;
}
