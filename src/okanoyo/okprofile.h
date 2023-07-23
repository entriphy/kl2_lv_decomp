#ifndef OKPROFILE_H
#define OKPROFILE_H

#include "okanoyo.h"

typedef struct {
    s32 fontID;
    s32 vsonID;
    s32 modeID;
    s32 kloDmg;
    s32 plnMod;
} OKPROFILE;

extern OKPROFILE okProfile;
extern s64 profileFlag;

#endif
