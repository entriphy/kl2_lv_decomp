#ifndef GMISS_H
#define GMISS_H

#include "nakano.h"

extern void Hero_LifeLost_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void Hero_Dead_Sub(OBJWORK *objw, HERO_WORK *herow);
extern void nkKloNoLife(OBJWORK *objw, HERO_WORK *herow);
extern void nkKloPhCode01(OBJWORK *objw, HERO_WORK *herow);
extern void nkKloPhCode03(OBJWORK *objw, HERO_WORK *herow);
extern void nkKloMissMain();


#endif