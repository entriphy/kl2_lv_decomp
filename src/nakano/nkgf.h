#ifndef NKGF_H
#define NKGF_H

#include "nakano.h"

extern u32 nkGfGetAll();
extern void nkGfInit();
extern void nkGfReStartInit();
extern void nkGfReStartSave();
extern void nkGfSetON(s32 id);
extern void nkGfSetOFF(s32 id);
extern s32 nkGfGet(s32 id);

#endif
