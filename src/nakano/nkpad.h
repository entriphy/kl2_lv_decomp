#ifndef NKPAD_H
#define NKPAD_H

#include "nakano.h"

#ifdef __cplusplus
extern "C" {
#endif

extern void nkInitPad();
extern s32 nkGetPr3ih(s32 id);
extern s32 nkGetPr3iv(s32 id);
extern f32 nkGetPr3fh(s32 id);
extern f32 nkGetPr3fv(s32 id);
extern s32 nkGetPr3dir(s32 id);
extern f32 nkGetPr3scalar(s32 id);
extern f32 nkGetPr3lscalar(s32 id);
extern f32 nkGetPr3angle(s32 id);
extern s32 nkGetPl3ih(s32 id);
extern s32 nkGetPl3iv(s32 id);
extern f32 nkGetPl3fh(s32 id);
extern f32 nkGetPl3fv(s32 id);
extern s32 nkGetPl3dir(s32 id);
extern f32 nkGetPl3scalar(s32 id);
extern f32 nkGetPl3lscalar(s32 id);
extern f32 nkGetPl3angle(s32 id);
extern s32 nkGetPlvl(s32 id);
extern s32 nkGetPtrg(s32 id);
extern s32 nkGetPrep(s32 id);
extern s32 nkGetPlooks(s32 id);
extern void nkGetPad();

#ifdef __cplusplus
}
#endif

#endif
