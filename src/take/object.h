#ifndef TAKE_OBJECT_H
#define TAKE_OBJECT_H

#include "take.h"

typedef struct { // 0x38
    /* 0x00 */ sceDmaEnv Env;
    /* 0x14 */ sceDmaChan *Vif0;
    /* 0x18 */ sceDmaChan *Vif1;
    /* 0x1c */ sceDmaChan *Gif;
    /* 0x20 */ sceDmaChan *FromIpu;
    /* 0x24 */ sceDmaChan *ToIpu;
    /* 0x28 */ sceDmaChan *Sif0;
    /* 0x2c */ sceDmaChan *Sif1;
    /* 0x30 */ sceDmaChan *FromSpr;
    /* 0x34 */ sceDmaChan *ToSpr;
} DMAPTR;

typedef struct { // 0x120
    /* 0x000 */ f32 SrcZ;
    /* 0x004 */ f32 AspectX;
    /* 0x008 */ f32 AspectY;
    /* 0x00c */ f32 CenterX;
    /* 0x010 */ f32 CenterY;
    /* 0x014 */ f32 MinZ;
    /* 0x018 */ f32 MaxZ;
    /* 0x01c */ f32 NearZ;
    /* 0x020 */ f32 FarZ;
    /* 0x030 */ sceVu0FVECTOR camera_p;
    /* 0x040 */ sceVu0FVECTOR camera_zd;
    /* 0x050 */ sceVu0FVECTOR camera_yd;
    /* 0x060 */ sceVu0FMATRIX WvMtx;
    /* 0x0a0 */ sceVu0FMATRIX VsMtx;
    /* 0x0e0 */ sceVu0FMATRIX WsMtx;
} SCRENV;

typedef struct { // 0xf0
    /* 0x00 */ sceVu0FVECTOR light0;
    /* 0x10 */ sceVu0FVECTOR light1;
    /* 0x20 */ sceVu0FVECTOR light2;
    /* 0x30 */ sceVu0FVECTOR color0;
    /* 0x40 */ sceVu0FVECTOR color1;
    /* 0x50 */ sceVu0FVECTOR color2;
    /* 0x60 */ sceVu0FVECTOR ambient;
    /* 0x70 */ sceVu0FMATRIX NormalLight;
    /* 0xb0 */ sceVu0FMATRIX LightColor;
} LIGHT3;

extern DMAPTR pDma;
extern SCRENV Scr;
extern LIGHT3 Light3;
extern char MemFlag[512];
extern char *SfxWorkBuff;
extern sceVu0FMATRIX SfxLsMtx[64];
extern sceVu0FMATRIX SfxLvMtx[64];
extern sceVu0FMATRIX SfxLvSpecMtx[64];
extern sceVu0FMATRIX SfxLcLightMtx[64];
extern sceVu0FMATRIX SfxSkinMtx[64];
extern sceVu0IVECTOR SfxVertexI[2048];
extern sceVu0IVECTOR SfxColorI[2048];
extern sceVu0IVECTOR SfxSpecUVI[2048];
extern u32 SfxTexInfoIndex;
extern qword_uni SpecGsEnv[64];
extern s32 SpecGsEnvInd;
extern qword_uni *SpecGsEnvAlpha;
extern sceVu0IVECTOR bboxTmp[2];
extern s32 SfxAllPause;

extern void ModelDraw(SFXOBJ *pObj);
extern s32 GetSfxOutFlag(SFXOBJ *pObj);
extern SFXOBJ* GetSfxObjectData(s32 ChrNum);
extern void SetSfxData(char *pSfxData);
extern s32 SetSfxWork(u32 *DataBuff, u32 *SfxAdrs);
extern s32 SetSfxAct(u32 *DataBuff, SFXOBJ *pObj, u32 *ActAdrs);
extern void PartsEnvInit(SFXOBJ *pObj);
extern void SetObjSubScale(SFXOBJ *pObj, f32 Scale);
extern void SetObjScale(SFXOBJ *pObj, sceVu0FVECTOR ScaleVector);
extern void SetObjGmsTransType(SFXOBJ *pObj, s32 type);
extern void SetObjClipFlag(SFXOBJ *pObj, s32 flag);
extern void GetObjMatrixTrans(SFXOBJ *pObj, s32 num, sceVu0FVECTOR DistVector);
extern void SetObjNormalLight(SFXOBJ *pObj, sceVu0FMATRIX *NormalLight);
extern void SetObjOutlineOff(SFXOBJ *pObj);
extern void SetObjEffDraw(SFXOBJ *pObj);
extern void SetObjLightColor(SFXOBJ *pObj, sceVu0FMATRIX *LightColor);
extern void OutLineEnvInit(u32 frame, u32 inter);
extern void LinkActTbl(MOTION *m, ACTTBL *pActtbl);
extern void SetObjCondition(SFXOBJ *pObj, s16 Condition);
extern void SetObjPause(SFXOBJ *pObj, u32 flag);
extern SFXOBJ* GetActiveSfx(SFXOBJ *pObj);
extern SFXOBJ* GetFlagObjPtr(SFXOBJ *pObj, u32 Condition);
extern void EraseSfxObject(SFXOBJ *pObj);
extern void MixClut(u16 Cbp, s32 Num0, s32 Num1, f32 Weight);
extern void SetSfxVariationClut(SFXOBJ *pObj, u32 ClutNum0, u32 ClutNum1, f32 Weight);
extern void SetSfxVariationGms(SFXOBJ *pObj, u32 GmsNum);
extern s32 SetSyncTex(SFXOBJ *pObj);
extern void SetSfxMotionSync(SFXOBJ *pObj0, SFXOBJ *pObj1);
extern s32 SetSfxActSimple(u32 *DataBuff, SFXOBJ *pObj);
extern void SetObjClipZone(SFXOBJ *pObj, f32 offset, f32 zone);
extern void SetObjShadow(SFXOBJ *pObj, s32 Type, f32 Size, f32 Offset, f32 Range);

#endif
