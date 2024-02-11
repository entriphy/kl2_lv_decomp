#ifndef NK_GAME_H
#define NK_GAME_H

#include "nakano.h"

extern s32 (*GameFuncTbl[3])();
extern s16 obj_id[129];
extern OBJWORK ObjWorkBuff[128];
extern qword *nkDstAdr;

extern void _nkGetMapBuf();
extern void nkInitPS2();

#endif
