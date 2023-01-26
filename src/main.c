#include <stdio.h>
#include <unistd.h>
#include "common.h"

SYSGBL SysGbl;
GAME_WORK GameGbl;

int main(int argc, char *argv[]) {
    hInitBoot();
    SysGbl.irqc = 0;
    hrpt_deb = 1;
    SysGbl.fmode = 1;
    GameGbl.vision = 0x1e00;
    nkDG.opflag |= 6;
    sleep(5);
    return 0;
}
