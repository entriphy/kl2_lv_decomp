#include <stdio.h>
#include <tamtypes.h>
#include <sifrpc.h>
#include <debug.h>
#include <unistd.h>
#include "gbl.h"
#include "harada.h"
#include "hoshino.h"
#include "nakano.h"

int main(int argc, char *argv[]) {
    hInitBoot();
    SysGbl.irqc = 0;
    hrpt_deb = 1;
    SysGbl.fmode = 1;
    GameGbl.vision = 0x1e00;
    nkDG.opflag |= 6;
    return 0;
}
