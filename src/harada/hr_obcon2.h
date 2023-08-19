#ifndef HR_OBCON2_H
#define HR_OBCON2_H

#include "harada.h"

extern void hr_gpt_getset(OBJWORK *objw, s32 *tbl);
extern void hr_gpt_resReg1(OBJWORK *objw, s16 *idt, s32 num);
extern s32 hr_ptmini_getset(OBJWORK *objw);
extern s32 hr_pnt_getset(OBJWORK *objw);
extern void hr_ptback_klo(s32 no, s32 mcn, s32 muki, f32 y, s32 flag);
extern void hr_ptnext_klo(s32 vision, s32 no, s32 mcn, s32 muki, f32 y);
extern void hr_game_puppet(OBJWORK *objw);

#endif
