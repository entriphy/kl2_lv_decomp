#ifndef TK_EFFECT_H
#define TK_EFFECT_H

#include "taka.h"

typedef struct {
    sceVu0FVECTOR smokePos;
    sceVu0FVECTOR smokeDirec;
    sceVu0FVECTOR warpStartPos;
    sceVu0FVECTOR warpEndPos;
    sceVu0FVECTOR mirrorPos[4];
    sceVu0FVECTOR waterPos;
    sceVu0FVECTOR waterDirec;
    s32 noseSmokeLife;
    s32 noseSmoke_colR;
    s32 noseSmoke_colG;
    s32 noseSmoke_colB;
    s32 noseSmoke_colA;
    s32 noseSmokeType;
    s32 isDestroy;
    f32 eggWarpDeg;
    f32 eggWarpSpeed;
    sceVu0FVECTOR pos[4];
    f32 floatParam[4];
} SEffectParam;

#ifdef __cplusplus
extern "C" {
#endif

extern void TkInit(u32 *dat_addr);
extern void TkMainLoop(s32 tick);
extern void TkAddEffect(s32 id);

#ifdef __cplusplus
}
#endif

extern SEffectParam EffectParamObj;

#endif

