#ifndef TK_VIEWSUB_H
#define TK_VIEWSUB_H

#include "take.h"

extern void BaseMove(SFXOBJ *pObj, sceVu0FVECTOR Rot, sceVu0FVECTOR Trans);
extern void BgDraw();
extern s32 GetChrNum(char *str);
extern void DrawClipZone(SFXOBJ *pObj);

#endif