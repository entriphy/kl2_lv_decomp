#ifndef RAYCRS_H
#define RAYCRS_H

#include "nakano.h"

typedef struct { // 0x30
    /* 0x00 */ f32 stx;
    /* 0x04 */ f32 stz;
    /* 0x08 */ s32 gdx;
    /* 0x0c */ s32 gdz;
    /* 0x10 */ f32 sty;
    /* 0x14 */ s32 gdy;
    /* 0x18 */ s32 gdw;
    /* 0x1c */ s32 gdh;
    /* 0x20 */ s32 p0;
    /* 0x24 */ s32 p1;
    /* 0x28 */ s32 p2;
    /* 0x2c */ u32 *prim_top;
} HITMAPINFO;

extern s32 GdCheck(f32 rd, f32 v0, f32 bmin, f32 bmax);
extern s32 GetHitBlocks(sceVu0FVECTOR vf0, sceVu0FVECTOR vf1);
extern f32 CalcNClip(sceVu0FVECTOR v0, sceVu0FVECTOR v1, sceVu0FVECTOR v2);
extern void NorMat(sceVu0FMATRIX mat, sceVu0FVECTOR vf);
extern void vu0_RotTransPers2(sceVu0IVECTOR v0, sceVu0FMATRIX m0, sceVu0FVECTOR v1);
extern void nkMonKloHit(sceVu0FVECTOR posi);
extern void DrawBlockGrid();
extern void DrawHitBlock(sceVu0IVECTOR posi);
extern void DrawHitPol(qword *prim);
extern f32 ChkHitPol(sceVu0FVECTOR vec, sceVu0FVECTOR muki, u32 *prim);
extern f32 ChkHitPol2(sceVu0FVECTOR vec, sceVu0FMATRIX mat, u32 *prim, u32 *hitcolor, sceVu0FVECTOR rnor);
extern f32 RayCrsJudgz(sceVu0FVECTOR vec, sceVu0FVECTOR muki);
extern f32 RayCrsJudgzN(sceVu0FVECTOR vec, sceVu0FVECTOR muki);
extern f32 RayCrsJudgzNL(sceVu0FVECTOR vec0, sceVu0FVECTOR vec1);
extern f32 RayCrsJudgzNLcol(sceVu0FVECTOR vec0, sceVu0FVECTOR vec1, u32 *hitcolor, sceVu0FVECTOR rnor);
extern f32 sbChkHitPol(sceVu0FVECTOR rnor, sceVu0FVECTOR vec, sceVu0FVECTOR muki, u32 *prim);
extern f32 sbRayCrsJudgzN(sceVu0FVECTOR rnor, sceVu0FVECTOR vec, sceVu0FVECTOR muki, f32 len);
extern void InitHitPoly(u32 *adr);
extern void InitHitPolyIndex();
extern void SetVu0MemImage();
extern void StTriFloat(sceVu0IVECTOR v0, sceVu0FMATRIX m0, s32 n);
extern void TestHitPoly();

#endif
