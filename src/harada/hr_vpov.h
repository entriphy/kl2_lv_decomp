#ifndef HR_VPOV_H
#define HR_VPOV_H

#include "harada.h"

typedef struct { // 0x90
    /* 0x00 */ sceVu0FVECTOR pos;
    /* 0x10 */ sceVu0FVECTOR rot;
    /* 0x20 */ sceVu0FVECTOR rotp;
    /* 0x30 */ sceVu0FMATRIX m;
    /* 0x70 */ VPOINFO *info;
    /* 0x74 */ s32 mon;
    /* 0x78 */ s32 mno;
    /* 0x7c */ s32 infono;
    /* 0x80 */ s32 infomax;
    /* 0x84 */ s32 objno;
    /* 0x88 */ s32 objmax;
} HRVPOV;

extern s32 (*hrVpoVFuncTbl[2])();

#endif
