#ifndef H_CDVD_H
#define H_CDVD_H

#include "hoshino.h"
#include <libcdvd.h>

typedef enum {
    CDREAD_IDLE,
    CDREAD_BGM,
    CDREAD_BGM2,
    CDREAD_PPT,
    CDREAD_DATA,
} CDREAD;

typedef struct {
    s32 Num;
    s32 Exe;
    s32 Reg;
    s32 Arg[32][5];
} hCDCUE;

typedef struct {
    sceCdlFILE file;
    sceCdRMode mode;
    s32 ThID;
    s32 DiscType;
    s32 DiscError;
    s32 DiscStat;
    s32 eeCnt; // 0x34
    s32 Sync; // 0x38
    s32 BGMplay; // 0x3C
    s32 dataStat; // 0x40
    s32 dataLSN; // 0x44
    s32 dataSectors; // 0x48
    u8 *buf; // 0x4C
    CDREAD dataFlag; // 0x50
    hCDCUE Cue;
} hCDDATA;

extern hCDDATA *cD;

extern void hCdReadFile(char *filename, u32 *buff);
extern u32* hCdReadFileRet(char *filename);
extern void hCdCuePushLIFO(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void hCdCuePush(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void hCdCuePop(s32 *p);
extern void hCdCuePopTest(s32 *p);
extern s32  hCdCueNum();
extern s32  hCdCueTime();
extern void hCdCueFlushBGM();
extern void hCdCueFlushPPT();
extern void hCdCueFlushBGM2();
extern s32  hCdRead(u32 lsn, u32 sectors, void *buff, sceCdRMode *mode);
extern s32  hCdReadIOPm(u32 lsn, u32 sectors, void *buff, sceCdRMode *mode);
extern s32  hCdGetSize(s32 no);
extern void hCdReadDataBlock(s32 no, s32 buff);
extern void hCdReadData(s32 no, s32 buff);
extern s32  hCdReadSync();
extern s32  hCdDataStat();
extern void hCdInit();
extern void hCdMain();
extern s32  hCdDiscStatOK();

#endif
