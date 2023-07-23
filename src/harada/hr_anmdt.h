#ifndef HR_ANMDT_H
#define HR_ANMDT_H

#include "harada.h"

extern s32 (*hrAnmVpmTbl[5])(HRANMV *, s32);
extern HRANMV hravbuf[1];
extern s32 hravcnt;
extern s32 hrmapoff;
extern HRAVL hrvlight[2];

extern void hr_vision_anmVPM_set();
extern void hr_change_anmVPM(s32 id);

#endif
