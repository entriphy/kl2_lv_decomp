#ifndef NKMIRR_H
#define NKMIRR_H

#include "nakano.h"

extern s32 nkMirClip(sceVu0FVECTOR pos);
extern void nkMirPushMatrix();
extern void nkMirPopMatrix();
extern s32 nkGetMirrFlag();
extern void nkMirDraw();

#endif
