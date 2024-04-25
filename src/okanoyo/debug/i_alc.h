#ifndef OK_I_ALC_H
#define OK_I_ALC_H

#include "okanoyo.h"

// TODO: this might be in a different okano header
typedef struct { // 0x10
    /* 0x0 */ s32 itemcnt;
    /* 0x4 */ s32 sisize;
    /* 0x8 */ s32 flagsize;
    /* 0xc */ s32 dispsize;
} ITEM_DATA_HEAD;

typedef struct { // 0x20
    /* 0x00 */ sceVu0FVECTOR pos;
    /* 0x10 */ sceVu0FVECTOR rot;
} SI_WORK;

extern void CopySI_WORK(SI_WORK *dist, SI_WORK *sorc);
extern void OkItemDataInport(s32 *buf, SI_WORK *sibuf);

#endif