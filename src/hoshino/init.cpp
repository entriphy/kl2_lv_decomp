#include <sifrpc.h>
#include <sifdma.h>
#include <sifcmd.h>
#include <libcdvd.h>
#include <loadfile.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include "hoshino.h"
#include "common.h"
#include "gbl.h"

const char* modules[8] = {
    "cdrom:\\M\\SIFCMD.IRX;1",
    "cdrom:\\M\\SIO2MAN.IRX;1",
    "cdrom:\\M\\MCMAN.IRX;1",
    "cdrom:\\M\\MCSERV.IRX;1",
    "cdrom:\\M\\PADMAN.IRX;1",
    "cdrom:\\M\\LIBSD.IRX;1",
    "cdrom:\\M\\SDRDRV.IRX;1",
    "cdrom:\\M\\KL2.IRX;1"
};

void hInitBoot() {
    SifInitRpc(0);
    while (!sceCdInit(SCECdINIT));
    while (!SifIopReboot("cdrom:\\M\\IOPRP213.IMG;1"));
    while (!SifIopSync());
    SifInitRpc(0);
    SifInitIopHeap();
    SifLoadFileInit();
    while (!sceCdInit(SCECdINIT));
    while (!sceCdMmode(SCECdMmodeDvd));
    for (int i = 0; i < 8; i++) {
        if (SifLoadModule(modules[i], 0, nullptr) < -1) {
            while (!sceCdInit(SCECdINIT));
            sceCdDiskReady(0);
            while (!sceCdMmode(SCECdMmodeDvd));
            // sceFsReset?
        }
        
    }
    GameGbl.vision = 0x6300;
}