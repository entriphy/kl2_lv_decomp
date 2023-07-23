#ifndef H_EVENT_H
#define H_EVENT_H

#include "hoshino.h"

typedef struct { // 0xe4
    /* 0x00 */ char Version[16];
    /* 0x10 */ char Code[4];
    /* 0x14 */ s32 Object;
    /* 0x18 */ s32 OutLine;
    /* 0x1c */ s32 DefModel;
    /* 0x20 */ s32 Boot;
    /* 0x24 */ s32 Loop;
    /* 0x28 */ s32 Route;
    /* 0x2c */ s32 TblNum;
    /* 0x30 */ s32 AppNum;
    /* 0x34 */ s32 DelNum;
    /* 0x38 */ s32 AppRt[8];
    /* 0x58 */ s32 AppCnt[8];
    /* 0x78 */ s32 DelRt[8];
    /* 0x98 */ s32 DelCnt[8];
    /* 0xb8 */ s32 DefRot[3];
    /* 0xc4 */ s32 Index;
    /* 0xc8 */ s32 RotPrm[2];
    /* 0xd0 */ s32 Scale;
    /* 0xd4 */ s32 Wait;
    /* 0xd8 */ s32 Shadow;
    /* 0xdc */ s32 StartSEprog;
    /* 0xe0 */ s32 StartSEsplt;
} hEvTHEAD;

typedef struct { // 0x48
    /* 0x00 */ s32 Type;
    /* 0x04 */ s32 Point[2];
    /* 0x0c */ s32 Speed[2];
    /* 0x14 */ s32 Rot[3];
    /* 0x20 */ s32 Bank[2];
    /* 0x28 */ s32 Motion;
    /* 0x2c */ s32 MotLoop;
    /* 0x30 */ s32 Stay;
    /* 0x34 */ s32 ModelSize;
    /* 0x38 */ s32 Light;
    /* 0x3c */ s32 Flag;
    /* 0x40 */ s32 SEprog;
    /* 0x44 */ s32 SEsplt;
} hEvTBODY;

typedef struct { // 0x14
    /* 0x00 */ s32 ActObj;
    /* 0x04 */ s32 Count;
    /* 0x08 */ s32 StartApp;
    /* 0x0c */ hEvTHEAD *pHd;
    /* 0x10 */ hEvTBODY *pBd;
} hEvTABLE;

typedef struct { // 0x4
    /* 0x0 */ s32 Count;
} hEvRESTART;

typedef struct { // 0x90
    /* 0x00 */ sceVu0FVECTOR Scale;
    /* 0x10 */ RT_WRK RtWrk;
    /* 0x20 */ SFXOBJ *pSfx;
    /* 0x24 */ hEvTABLE *pTbl;
    /* 0x28 */ s32 Idx;
    /* 0x2c */ s32 Wait;
    /* 0x30 */ s32 Loop;
    /* 0x34 */ s32 MotLoop;
    /* 0x38 */ s32 Frame;
    /* 0x3c */ s32 Frame2;
    /* 0x40 */ s32 TblIdx;
    /* 0x44 */ s32 Infinity;
    /* 0x48 */ s32 Time;
    /* 0x4c */ s32 Dist;
    /* 0x50 */ s32 Mcn;
    /* 0x58 */ s64 Accel;
    /* 0x60 */ s32 Plc;
    /* 0x64 */ s32 RotCnt;
    /* 0x70 */ sceVu0FVECTOR dRot;
    /* 0x80 */ s32 Light;
    /* 0x84 */ s32 LightBak;
    /* 0x88 */ f32 LightCnt;
    /* 0x8c */ s32 seObj;
    /* 0x90 */ s32 seFlag;
} hEvOBJECT;

extern void hEvInitFrame();
extern void hEvObjMake(hEvTABLE *pt);
extern void hEvObjDel(s32 idx);
extern void hEvObjAllDel();
extern u32  hEvGetFlag();
extern void hAngleClip(f32 *vec, s32 num);
extern void hEvObjMukiChange(hEvOBJECT *pEO);
extern void hEvObjBankChange(hEvOBJECT *pEO);
extern void hEvObjMuki(hEvOBJECT *pEO);
extern void hEvObjMoveInit(hEvOBJECT *pEO);
extern void hEvObjMove(hEvOBJECT *pEO);
extern void hEvObjAppear();
extern void hEvObjCtrl();
extern void hEvObjDraw();
extern void hEvObjMain();
extern void hEvTblSet(s32 *pEv, s32 pEv_index, s32 no);
extern void hEvTblInit(s32 addr, s32 ar);
extern s32  hEvGetAreaAddr(s32 ar);
extern void hEvAreaInit();
extern void hEvStageInit();
extern void hEvPushRestart();
extern void hEvPopRestart();

#endif
