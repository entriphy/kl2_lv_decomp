#ifndef H_FILE_H
#define H_FILE_H

#include "hoshino.h"

extern s32  hGameDataSize(s32 mode);
extern void hGameRead(s32 mode, s32 buff);
extern void hSysDataRead(s32 buff);
extern s32  hGameReadOK();
extern s32  hGetDataAddr(s32 i);
extern s32  hGetSysDataAddr(s32 i);

#endif
