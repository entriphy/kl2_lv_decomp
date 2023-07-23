#ifndef H_UTIL_H
#define H_UTIL_H

#include "hoshino.h"

extern void dump8(u8 *p);
extern void dump64(u64 *p, char *str);
extern u32  GetFHMNum(u32 *pAddr);
extern u32* GetFHMAddress(u32 *pAddr, u32 nNum);
extern s32  hFileSize(char *filename);
extern s32  hFileLoad(char *filename,  s32 buff);
extern s32  hFileRead(char *filename, s32 buff, s32 *size);

#endif
