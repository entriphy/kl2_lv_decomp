#include <sifdev.h>
#include <sifrpc.h>
#include "h_init.h"
#include "nakano/init.h"
#include "h_cdvd.h"
#include "h_file.h"
#include "hato/ht_bcommon.h"
#include "h_rpc.h"
#include "h_movie.h"
#include "h_sound.h"
#include "okanoyo/okio.h"

#ifdef RETAIL
char *LoadModuleList[] = {
    "cdrom:\\M\\SIFCMD.IRX;1",
    "cdrom:\\M\\SIO2MAN.IRX;1",
    "cdrom:\\M\\MCMAN.IRX;1",
    "cdrom:\\M\\MCSERV.IRX;1",
    "cdrom:\\M\\PADMAN.IRX;1",
    "cdrom:\\M\\LIBSD.IRX;1",
    "cdrom:\\M\\SDRDRV.IRX;1",
    "cdrom:\\M\\KL2.IRX;1"
};
#else
char *LoadModuleList[] = {
    "host0:sio2man.irx",
    "host0:mcman.irx",
    "host0:mcserv.irx",
    "host0:padman.irx",
    "host0:libsd.irx",
    "host0:sdrdrv.irx",
    "host0:kl2.irx"
};
#endif

void hInitBoot() {
    u32 i;
    s32 ret;

    sceSifInitRpc(0);
    while (!sceCdInit(SCECdINIT));
#ifdef RELEASE
    while (!sceSifRebootIop("cdrom:\\M\\"IOP_IMAGE_FILE";1"));
#else
    while (!sceSifRebootIop("host0:"IOP_IMAGE_file));
#endif
    while (!sceSifSyncIop());
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sceSifLoadFileReset();
    while (!sceCdInit(SCECdINIT));
    while (!sceCdMmode(SCECdDVD));
    sceFsReset();
    for (i = 0; i < sizeof(LoadModuleList) / sizeof(LoadModuleList[0]); i++) {
        while ((ret = sceSifLoadModule(LoadModuleList[i], 0, NULL)) < 0) {
            while (!sceCdInit(0));
            sceCdDiskReady(0);
            while (!sceCdMmode(SCECdDVD));
            sceFsReset();
        }
    }

    hRpcInit();
    hRpc(IOP_IopInit);
    init_config_system();
    hCdInit();
    hMovInit();
    hSndInit();
    
    GameGbl.vision = 0x6300;
    hSndBankSetMain();
    hSysDataRead((s32)getBuff(1, hGameDataSize(1), NULL, &ret));
    memoryStageptrSet();
    htInitRand(0x399);
}

void hLoopTop() {
    while (hRpcSync() != 0);
    hSndRpcRet();
}

void hLoopBottom() {
    hSndMain();
    hCdMain();
}

s32 SyncV() {
    hLoopBottom();
    return sceGsSyncV(0) ^ 1;
}
