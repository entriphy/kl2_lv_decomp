#ifndef H_MENU_H
#define H_MENU_H

#include "hoshino.h"
#include "h_func.h"
#include "h_pad.h"

typedef struct { // 0x10
    /* 0x0 */ s32 mode;
    /* 0x4 */ s32 vision;
    /* 0x8 */ s32 flag;
    /* 0xc */ s32 vscnt;
} MENU_WORK;

typedef struct { // 0x5d0
    /* 0x000 */ hPACKET_DATA PkData;
    /* 0x088 */ hPAD_DATA pad[2];
    /* 0x250 */ sceGsDBuffDc db;
    /* 0x580 */ sceDmaChan *dmaGif;
    /* 0x584 */ s32 fr;
    /* 0x588 */ s32 inter;
    /* 0x58c */ s32 light_dir[4][3];
    /* 0x5bc */ u8 light_col[4][4];
} hGLOBAL;

extern s32 (*hMenuFuncTbl[2])();
extern hGLOBAL hG;
extern s32 RpcArg[16];
extern s128 hPacketArea[2048];

#endif
