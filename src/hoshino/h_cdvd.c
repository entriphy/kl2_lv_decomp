#include "hoshino/h_cdvd.h"
#include "hoshino/h_str.h"
#include "hoshino/h_util.h"

KLTABLE *FileData = NULL;
hCDDATA CdData = {};
hCDDATA *cD = NULL;
hCDCUE *cQ = NULL;
u8 cb_stack[8192];

void hCdReadFile(char *filename, u32 *buff) {
    sceCdlFILE file;
    s32 ret;
    s32 sectors;
    
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&file, filename));
    sectors = ((file.size + 0x7FF) / 0x800);
    if (sectors > 0x400) {
        sectors = 0x400;
    }
    while (!sceCdRead(file.lsn, sectors, buff, &cD->mode));
    do {
        s32 i;
        for (i = 0; i < 0x100000; i++);
    } while (sceCdSync(1));
}

u32 * hCdReadFileRet(char *filename) {
    sceCdlFILE file;
    u32 *buf;
    s32 ret;
    s32 sectors;
    
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&file, filename));
    sectors = ((file.size + 0x7FF) / 0x800);
    buf = (u32 *)getBuff(1, sectors * 0x800, NULL, &ret);
    while (!sceCdRead(file.lsn, sectors, buf, &cD->mode));
    do {
        s32 i;
        for (i = 0; i < 0x100000; i++);
    } while (sceCdSync(1));

    return buf;
}

void hCdCuePushLIFO(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 exe;
    
    cQ->Exe--;
    if (cQ->Exe < 0) {
        cQ->Exe = 31;
    }

    exe = cQ->Exe;
    cQ->Arg[exe][0] = arg0;
    cQ->Arg[exe][1] = arg1;
    cQ->Arg[exe][2] = arg2;
    cQ->Arg[exe][3] = arg3;
    cQ->Arg[exe][4] = arg4;
    cQ->Num++;
}

void hCdCuePush(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 reg = cQ->Reg;

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

void hCdCuePop(s32 *p) {
    s32 exe = cQ->Exe;
    
    p[0] = cQ->Arg[exe][0];
    p[1] = cQ->Arg[exe][1];
    p[2] = cQ->Arg[exe][2];
    p[3] = cQ->Arg[exe][3];
    p[4] = cQ->Arg[exe][4];

    cQ->Exe++;
    if (cQ->Exe == 32) {
        cQ->Exe = 0;
    }
    cQ->Num--;
}

void hCdCuePopTest(s32 *p) {
    s32 i = cQ->Exe;
    p[0] = cQ->Arg[i][0];
    p[1] = cQ->Arg[i][1];
    p[2] = cQ->Arg[i][2];
    p[3] = cQ->Arg[i][3];
    p[4] = cQ->Arg[i][4];
}

s32 hCdCueNum() {
    return cQ->Num;
}

s32 hCdCueTime() {
    return cQ->Arg[cQ->Exe][4];
}

void hCdCueFlushBGM() {
    s32 i;
    s32 iVar2;
    s32 cue;
    u32 arr[32][5];

    i = 0;
    while ((cue = hCdCueNum()) != 0) {
        hCdCuePop(arr[i++]);
    }
    
    for (iVar2 = 0; iVar2 < i; iVar2++) {
        if (arr[iVar2][3] - 1 > 1) {
            hCdCuePush(arr[iVar2][0], arr[iVar2][1], arr[iVar2][2], arr[iVar2][3], arr[iVar2][4]);
        }
    }
}

void hCdCueFlushPPT() {
    s32 i;
    s32 iVar2;
    s32 cue;
    u32 arr[32][5];

    i = 0;
    while ((cue = hCdCueNum()) != 0) {
        hCdCuePop(arr[i++]);
    }
    
    for (iVar2 = 0; iVar2 < i; iVar2++) {
        if (arr[iVar2][3] != 3) {
            hCdCuePush(arr[iVar2][0], arr[iVar2][1], arr[iVar2][2], arr[iVar2][3], arr[iVar2][4]);
        }
    }
}

void hCdCueFlushBGM2() {
    s32 i;
    s32 iVar2;
    s32 cue;
    u32 arr[32][5];

    i = 0;
    while ((cue = hCdCueNum()) != 0) {
        hCdCuePop(arr[i++]);
    }
    
    for (iVar2 = 0; iVar2 < i; iVar2++) {
        if (arr[iVar2][3] - 1 > 1) {
            hCdCuePush(arr[iVar2][0], arr[iVar2][1], arr[iVar2][2], arr[iVar2][3], arr[iVar2][4]);
        }
    }
}

s32 hCdRead(u32 lsn, u32 sectors, void *buff, sceCdRMode *mode) {
    sceCdDiskReady(0);
    if (sceCdRead(lsn, sectors, buff, mode) == 0) {
        return 0;
    } else {
        cD->dataLSN = lsn;
        cD->dataSectors = sectors;
        cD->buf = buff;
        return 1;
    }
}

s32 hCdReadIOPm(u32 lsn, u32 sectors, void *buff, sceCdRMode *mode) {
    sceCdDiskReady(0);
    if (sceCdReadIOPm(lsn, sectors, buff, mode) == 0) {
        return 0;
    } else {
        cD->dataLSN = lsn;
        cD->dataSectors = sectors;
        cD->buf = buff;
        return 1;
    }
}

s32 hCdGetSize(s32 no) {
    return FileData[no].sectors;
}

void hCdReadDataBlock(s32 no, s32 buff) {
    s32 i;

    sceCdDiskReady(0);
    while (!sceCdRead( cD->file.lsn + FileData[no].lsn, FileData[no].sectors, (void *)buff, &cD->mode));
    do {
        for (i = 0; i < 0x100000; i++) {
            // Do nothing
        }
    } while (sceCdSync(1));
}

void hCdReadData(s32 no, s32 buff) {
    hCdCuePush((cD->file).lsn + FileData[no].lsn, FileData[no].sectors, buff, 4, cD->eeCnt);
}

s32 hCdReadSync() {
    if (cD->dataFlag == CDREAD_DATA && cD->dataStat == 2)
        return 1;
    else
        return 0;
}

void hCdInit() {
    u32 *buff;

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
    
    sceCdInitEeCB(0, cb_stack, 0x2000);
    ChangeThreadPriority(cD->ThID = GetThreadId(), 1);

    sceCdDiskReady(0);
    cD->DiscType = sceCdGetDiskType();
    buff = hCdReadFileRet("\\HEADPACK.BIN;1");
    FileData = (KLTABLE *)(GetFHMAddress(buff, 0) + 1);
    PptTable = (PPTTABLE *)(GetFHMAddress(buff, 1) + 1);
    BgmTable = (BGMTABLE *)(GetFHMAddress(buff, 2) + 1);

    sceCdDiskReady(0);
    while (!sceCdSearchFile(&cD->file, "\\KLDATA.BIN;1"));
}

void hCdMain(void) {
    s32 com_ok;
    s32 stat;
    s32 arg[5];

    while ((cD->DiscError = sceCdGetError()) == -1);
    cD->Sync = sceCdSync(1);

    switch (cD->DiscError) {
        case SCECdErNO:
        case SCECdErABRT:
            com_ok = 1;
            break;
        case SCECdErNORDY:
            com_ok = 0;
            break;
        default:
            com_ok = 0;
            while (!sceCdBreak());
            sceCdDiskReady(1);
            if (cD->dataStat != 0) {
                hCdCuePushLIFO(cD->dataLSN, cD->dataSectors, (s32)cD->buf, cD->dataFlag, cD->eeCnt);
                cD->dataFlag = CDREAD_IDLE;
                cD->dataStat = 0;
            }
            break;
    }

    if (cD->dataStat == 2) {
        cD->dataFlag = CDREAD_IDLE;
        cD->dataStat = 0;
    }

    if (hCdCueNum() > 0 && hCdCueTime() <= cD->eeCnt  && com_ok && cD->dataStat == 0) {
        hCdCuePopTest(arg);
        cD->dataFlag = arg[3];
        switch (cD->dataFlag) {
            case CDREAD_BGM:
            case CDREAD_BGM2:
                stat = hCdReadIOPm(arg[0], arg[1], (void *)arg[2], &cD->mode);
                break;
            case CDREAD_PPT:
            case CDREAD_DATA:
                stat = hCdRead(arg[0], arg[1], (void *)arg[2], &cD->mode);
                break;
            case 5:
                stat = hCdRead(arg[0], arg[1], (void *)arg[2], &cD->mode);
                break;
        }
        if (stat != 0) {
            hCdCuePop(arg);
            cD->dataStat = 1;
            cD->Sync = 1;
        }
    }

    if (cD->dataStat != 0 && cD->Sync == 0)
        cD->dataStat = 2;

    hBgmMain();
    hPptMain();
    hStrMain();
    hStrInfo();

    cD->eeCnt++;
}

s32 hCdDiscStatOK() {
    switch (cD->DiscError) {
        case SCECdErNO:
        case SCECdErABRT:
            return 0;
        default:
            return 1;
    }
}
