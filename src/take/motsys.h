#ifndef MOTSYS_H
#define MOTSYS_H

#include "take.h"

extern void GetMotion(SFXOBJ *pObj);
extern void GetMotionSimple(SFXOBJ *pObj);
extern void GetLocalMotion(MOTION *m);
extern void GetSimpleSfxMatrix(SFXOBJ *pObj);
extern void GetScreenMotion(SFXOBJ *pObj);
extern void GetNormalLightMatrix(SFXOBJ *pObj);
extern void InitSfxCoord(MOTION *m, u8 *pInf, tagCOORD *pCoord);
extern void InitCoord(MOTION *m, u8 *pInf, u8 *pItr, u8 *pItrW, tagCOORD *pCoord);
extern void SetLocalMatrix(sceVu0FVECTOR Rot, sceVu0FVECTOR Tra, sceVu0FMATRIX Mtx);
extern void InterPolateMatrix(sceVu0FMATRIX dm, sceVu0FMATRIX m0, sceVu0FMATRIX m1, f32 Weight);
extern void MotionMix(sceVu0FMATRIX *dm, sceVu0FMATRIX *m0, sceVu0FMATRIX *m1, s32 CoordNum, f32 Weight, u64 OnFlag);
extern void CopyRotMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1);
extern void ChangeLocalMatrix(sceVu0FMATRIX lm,sceVu0FMATRIX wm,sceVu0FMATRIX lwm);
extern void GetLwMtx(tagCOORD *pCoord);
extern void GetRotTransMatrixXYZ(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra);
extern void GetRotTransMatrixYXZ(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra);
extern void GetRotTransMatrixXZY(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra);
extern void GetRotTransMatrixZXY(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra);
extern void GetRotTransMatrixYZX(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra);
extern void GetRotTransMatrixZYX(sceVu0FMATRIX mtx, sceVu0FVECTOR rot, sceVu0FVECTOR tra);
extern void SetMotionWorldIp(MOTION *m, f32 *wipcnt);
extern void SetAct(SFXOBJ *pObj, s32 Actnum);
extern void SetActIp(SFXOBJ *pObj, s32 Actnum);
extern void SetActMix(SFXOBJ *pObj, s32 Actnum);
extern void SetActSub(MOTION *m, s32 ActNum);
extern void SetActIpSub(MOTION *m, s32 ActNum);
extern void SetActMixSub(MOTION *m, s32 Actnum);
extern s32 GetActStopFlag(SFXOBJ *pObj);
extern f32 GetActEndCnt(SFXOBJ *pObj);
extern f32 GetActCnt(SFXOBJ *pObj);
extern void SetBaseMatrix(SFXOBJ *pObj, sceVu0FVECTOR Rot, sceVu0FVECTOR Trans, s32 RotOrder);
extern void SetBaseMatrix2(SFXOBJ *pObj, sceVu0FMATRIX SrcMatrix);
extern void GetScaleMtx(sceVu0FMATRIX mtx, sceVu0FVECTOR vec);
extern void NormalMatrix(sceVu0FMATRIX m0, sceVu0FMATRIX m1);
extern void DecodeMotion(sceVu0FMATRIX *DecodeBuff, MOTION *m, s32 Ind);
extern void ClearQwordMem(u32 Addrs, u32 Num);
extern void AcxDecodeMotion(sceVu0FMATRIX *DecodeBuff, MOTION *m, s32 Ind);

#endif
