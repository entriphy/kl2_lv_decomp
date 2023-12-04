#ifndef HEROSUB_H
#define HEROSUB_H

#include "nakano.h"

extern s32 HeroRootChangeSub(OBJWORK *objw, sceVu0FVECTOR hitposi, s32 yflag);
extern void Hero_Taiki_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Shot_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Shot2_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Run_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Jump_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Jump_Sub2(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_SJump_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Fuwafuwa_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Chaku_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_TShot2_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_TShot_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_TShot3_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Yarare_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Hajikare_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Todomari_Sub(/* a0 4 */ OBJWORK *objw, /* a1 5 */ HERO_WORK *herow);
extern void Hero_DoorJump_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_DoorStop_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_GateInWalk_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_GateOutWalk_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_FireSi_Sub(OBJWORK *objw, HERO_WORK *herow);

#endif