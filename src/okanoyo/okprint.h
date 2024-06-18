#ifndef OKPRINT_H
#define OKPRINT_H

#include "okanoyo.h"

extern void OkSetIDprofile();
extern void OkSetIDprofileRoot();
extern void OkPrintDataLoad();
extern void OkReSendTex(char *buff);
extern void OkPrint(s32 personal_id, s16 x, s16 y, u8 *scr, char *format, ...);
extern void OkPrintSysInit(s32 personal_id, s32 screenw, s32 screenh, s32 fontw, s32 fonth);
extern void OkDefaultSysInit(s32 screenw, s32 screenh, s32 fontw, s32 fonth);
extern void OkFontScreenClr(s64 *scr);
extern void OkFontFlush(s32 personal_id, u8 *scr);
extern void OkPFontFlush(u16 trg);
extern void OkHtimeClrAll();
extern void OkHtimeClr(s32 id);
extern void OkSetHtimeStart(s32 personal_id, s64 col);
extern void OkSetHtimeEnd(s32 personal_id);
extern void OkDebugGame2pCon();
extern void OkDebugGame2pConSet(s32 personal_id, void *prgptr);
extern void OkDebugGame2pConInit(void *mainprgptr);

#endif
