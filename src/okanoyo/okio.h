#ifndef OKIO_H
#define OKIO_H

#include "okanoyo.h"

extern s32 testcbp;
extern s32 memtest;
extern s32 memblocksize[16];

extern s32 OkCheck_file(char *name);
extern s32 OkRead_file(char *name, char *buff);
extern void memorySysFormat();
extern void memoryStageptrSet();
extern void memoryStageFormat();
extern void memoryAreaptrSet();
extern void memoryAreaFormat();
extern void OkMemDisp();
extern s32* getmemblksize();
extern void* getmem(s32 byte);
extern s32 freemem(void *ptr);
extern void DebmemFormat(s32 ID);
extern void debmemorySysFormat();
extern void* getBuff(s32 type, s32 byte, char *name, s32 *ret);
extern void freeBuff(s32 type, s32 byte, char *name);

#endif
