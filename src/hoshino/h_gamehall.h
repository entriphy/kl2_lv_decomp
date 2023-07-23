#ifndef H_GAMEHALL_H
#define H_GAMEHALL_H

#include "hoshino.h"
#include "h_str.h"

typedef enum {
    CH_IDLE,
    CH_PLAY,
    CH_FADEOUT,
    CH_STOP,
    CH_END
} CH_STAT;

typedef struct {
    CH_STAT stat;
    s32 req;
    s32 idx;
    s32 idx2;
    s32 max;
    BGM no;
    s32 ch;
    s32 size;
} CHALL;

extern void hCHallInit();
extern void hCHallQuit();
extern s32  hCHallRun(s32 req, s32 arg);

#endif
