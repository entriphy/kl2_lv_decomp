#ifndef H_WKMNG_H
#define H_WKMNG_H

#include "hoshino.h"

typedef struct { // 0xc
    /* 0x0 */ s32 Idx;
    /* 0x4 */ s32 Busy;
    /* 0x8 */ s32 Next;
} hMNGLIST;

typedef struct { // 0xc
    /* 0x0 */ s32 Idx;
    /* 0x4 */ s32 Max;
    /* 0x8 */ hMNGLIST *pList;
} hMNG;

extern void hMngInit(hMNGLIST *pMng, s32 max, s32 id);
extern s32  hWorkGet(s32 id);
extern void hWorkDel(s32 id, s32 idx);

#endif
