#ifndef OBJKLO0_H
#define OBJKLO0_H

#include "nakano.h"

extern void Init_ReTry();
extern void Init_Restart();
extern void InitKlonoa(OBJWORK *objw, s32 imuki);
extern void SetKlonoaPadData(s32 lvl, s32 trg, s32 con);
extern void PtKlonoaJump(f32 spd, s32 flag);
extern void SayWahoo(OBJWORK *objw, HERO_WORK *herow);
extern void ObjKlo(OBJWORK *objw);
extern void SetRtwKlonoa(s32 rtn, s32 rtcnt, s32 muki);
extern void SetRtwKlonoaY(s32 rtn, s32 rtcnt, s32 muki, f32 height);

#endif
