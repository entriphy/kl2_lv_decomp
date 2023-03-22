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

int FUN_00166128(int file) {
    return KlTable[file].count;
}

void FUN_00166140(int file, u8 *buf) {
    sce_print("@@@ reading file %d to 0x%08x\n", file, buf);

    sceCdDiskReady(0);
    while (!sceCdRead(KlTable[file].offset + cD->file.lsn, KlTable[file].count, buf, &cD->mode));
    do {
        for (int i = 0; i < 0x10000; i++) {
            // Do nothing
        }
    } while (sceCdSync(1));
}

void FUN_001661e0(int param_1, u8 *param_2) {
    hCdCuePush((cD->file).lsn + KlTable[param_1].offset, KlTable[param_1].count, (int)param_2, 4, cD->eeCnt);
}

int isLoading() {
    int ret = 0;
    if (cD->dataFlag == CDREAD_DATA) {
        ret = cD->dataStat == 2;
    }
    return ret;
}

void hCdInit() {
    cD = &CdData;
    cQ = &cD->Cue;

    cQ->Num = 0;
    cQ->Exe = 0;
    cQ->Reg = 0;

    cD->eeCnt = 0;
    cD->dataFlag = CDREAD_IDLE;
    cD->BGMplay = 0;
    cD->mode.trycount = 0;
    cD->mode.spindlctrl = 0;
    cD->mode.datapattern = 0;
    cD->mode.pad = 0;
    
    sceCdInitEeCB(0, EeCb, 0x2000);
    int threadId = GetThreadId();
    cD->ThID = threadId;
    ChangeThreadPriority(threadId, 1);

    sceCdDiskReady(0);
    cD->DiscType = sceCdGetDiskType();
    u8 *buf = hReadFile("\\HEADPACK.BIN;1");
    u8 *addr = GetFHMAddress(buf, 0);
    KlTable = (KLTABLE *)(addr + 4);
    addr = GetFHMAddress(buf, 1);
    PptTable = (PPTTABLE *)(addr + 4);
    addr = GetFHMAddress(buf, 2);
    BgmTable = (BGMTABLE *)(addr + 4);

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&cD->file, "\\KLDATA.BIN;1"));
}