#ifndef EVRT_H
#define EVRT_H

#include "nakano.h"

extern s32 nkInitEvRt(u32 *adr);
extern ROUTE* GetEvRtAdrs(s32 rtn);
extern s32 GetEvRtNo(ROUTE *rtp);
extern void DrawEvRt();

#endif