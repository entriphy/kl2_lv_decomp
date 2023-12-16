#ifndef KLMOT_H
#define KLMOT_H

#include "nakano.h"

extern SFXOBJ* nkGetKloPSFXOBJ();
extern void nkSetKlonoaShadow(OBJWORK *objw);
extern void InitKloModel(OBJWORK *objw);
extern void nkBaseMove(SFXOBJ *pObj, sceVu0FVECTOR Rot, sceVu0FVECTOR Trans);
extern s32 nkCallMotion(SFXOBJ *pObj, s32 *now, s32 after);
extern void DrawMirSfxKlonoa(OBJWORK *objw);
extern void nk_ptget_klonoa(sceVu0FVECTOR pos, sceVu0FVECTOR rot, RT_WRK *rt, s16 *rtno, s32 *mc, s32 *yuka);
extern void DrawSfxKlonoa(OBJWORK *objw);
extern void DrawSfxKlonoa_SS(OBJWORK *objw);
extern void nkCallBoardFlear(OBJWORK *objw);
extern void DrawSfxKlonoa_BH(OBJWORK *objw);

#endif