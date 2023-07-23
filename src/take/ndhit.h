#ifndef NDHIT_H
#define NDHIT_H

#include "sfx.h"
#include "objwork.h"

typedef struct {
    /* 0x000 */ u64 tex0;
    /* 0x010 */ sceVu0FVECTOR BaseVec;
    /* 0x020 */ sceVu0FVECTOR pad[3];
    /* 0x050 */ qword_uni GsEnv[16];
    /* 0x150 */ s32 GsEnvInd;
    /* 0x154 */ s16 px;
    /* 0x156 */ s16 py;
    /* 0x158 */ s16 ph;
    /* 0x15a */ s16 pw;
    /* 0x15c */ f32 Radius[12];
    /* 0x18c */ s32 Cnt;
    /* 0x190 */ u8 Alpha[12];
    /* 0x1a0 */ sceVu0FVECTOR Vec[12];
    /* 0x260 */ f32 SpinCnt;
} NDHIT;

extern OBJWORK* SetNdhit(sceVu0FVECTOR Vec);
extern void NdhitEnvInit(NDHIT *pNh);
extern void DrawNdhit(OBJWORK *pObjw);
extern void DrawNdhitSub(sceVu0IVECTOR *Xyz, s32 px, s32 py, s32 pw, s32 ph, u64 tex0, u8 alpha);
extern void DrawNdhitSub2(sceVu0IVECTOR Xyz, sceVu0IVECTOR Xyz2, u8 alpha);

#endif
