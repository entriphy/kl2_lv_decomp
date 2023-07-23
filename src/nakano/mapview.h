#ifndef MAPVIEW_H
#define MAPVIEW_H

#include "nakano.h"

extern s32 (*MapVFuncTbl[2])();

extern void nkMapFileRead();
extern void MapConfFileWrite();
extern void MapVDrawBG();

#endif
