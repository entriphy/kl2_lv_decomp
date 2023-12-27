#ifndef KLOHIT0_H
#define KLOHIT0_H

#include "nakano.h"

extern void HeroKabeCheck(OBJWORK *objw, HERO_WORK *herow);
extern void HeroKabeCheck2(OBJWORK *objw, HERO_WORK *herow);
extern void HeroKabeCheck3(OBJWORK *objw, HERO_WORK *herow);
extern f32 Ashi_HitSub(OBJWORK *objw, sceVu0FVECTOR hitposi, s32 *yflag, OBJWORK **yukaobj, s16 *kage_flag, u32 *phcode);
extern f32 Ashi_HitCheck(OBJWORK *objw, s32 *retflag, f32 saka, s32 *kage_flag);
extern void Hero_JumpDown_Sub(OBJWORK *objw, HERO_WORK *herow);
extern f32 Ashi_HitSubUp(OBJWORK *objw, sceVu0FVECTOR hitposi, s32 *yflag, OBJWORK **yukaobj, s16 *kage_flag, u32 *phcode);
extern f32 Ashi_HitCheckUp(OBJWORK *objw, s32 *retflag, f64 saka, s32 *kage_flag);
extern void Hero_JumpUp_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void TenjoHit(OBJWORK *objw, HERO_WORK *herow);

#endif
