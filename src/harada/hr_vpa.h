#ifndef HR_VPA_H
#define HR_VPA_H

#include "harada.h"

typedef struct { // 0x1c
    /* 0x00 */ VPOINFO vpoi;
    /* 0x10 */ u32 n;
    /* 0x14 */ u32 *anm;
    /* 0x18 */ u32 ver;
} VPAINFO;

extern VPAINFO infovpa;
//extern ANMINFO *hri_anm;
//extern ANMINFO *hri_bg;

#endif
