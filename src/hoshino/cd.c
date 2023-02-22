#include <libcdvd.h>
#include "common.h"

void hCdCuePush(int arg0, int arg1, int arg2, int arg3, int arg4) {
    int reg = cQ->Reg;
    cQ->Arg[reg][0] = arg0;
    cQ->Arg[reg][1] = arg1;
    cQ->Arg[reg][2] = arg2;
    cQ->Arg[reg][3] = arg3;
    cQ->Arg[reg][4] = arg4;
    if (++cQ->Reg == 32) {
        cQ->Reg = 0;
    }
    cQ->Num++;
}

void FUN_00166140(int file, void *buf) {
    sce_print("@@@ reading file %d to 0x%08x\n", file, buf);

    sceCdDiskReady(0);
    while (!sceCdRead(KlTable[file].offset + cD->file.lsn, KlTable[file].count, buf, &cD->mode));
    do {
        for (int i = 0; i < 0x10000; i++) {
            // Do nothing
        }
    } while (sceCdSync(1));
}
