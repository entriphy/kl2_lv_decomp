#ifndef OKPRINT_H
#define OKPRINT_H

#include "okanoyo.h"

extern void OkPrintSysInit(s32 personal_id, s32 screenw, s32 screenh, s32 fontw, s32 fonth);
extern void OkDefaultSysInit(s32 screenw, s32 screenh, s32 fontw, s32 fonth);
extern void OkPFontFlush(u16 trg);

#endif
