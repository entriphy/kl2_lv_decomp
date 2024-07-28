#ifndef TAKE_VIEW_H
#define TAKE_VIEW_H

#include "take.h"

typedef struct { // 0x40
	/* 0x00 */ SFXOBJ *pObj;
	/* 0x10 */ sceVu0FVECTOR Rot;
	/* 0x20 */ sceVu0FVECTOR Trans;
	/* 0x30 */ s32 ActNum;
	/* 0x34 */ s32 DirNum;
	/* 0x38 */ s32 ChrNum;
	/* 0x3c */ s32 AllFlag;
} VIEWOBJ;

typedef struct { // 0x4482
	/* 0x0000 */ s16 DirNum;
	/* 0x0002 */ char DirName[64][16];
	/* 0x0402 */ s16 FileNum[64];
	/* 0x0482 */ char FileName[64][64][4];
} BINH;

enum {
	VIEWREAD = 0,
	VIEWACTION = 1,
	VIEMOTION = 2,
	MODEMAX = 3
};

extern void ViewInit();
extern void View();

extern f32 klwipsp[48];
extern void (*pnfPrg[3])();
extern char ModeName[3][16];
extern qword_uni XYZ[4];
extern qword_uni RGB[4];
extern qword_uni UV[4];
extern u8 *SfxDataBuff[4];
extern sceVu0FVECTOR Trans;
extern sceVu0FVECTOR Rot;
extern char fname[32];
extern s32 TypeNumMax;
extern s32 ModelIndex;
extern s32 TypeNum;
extern VIEWOBJ vObj[4];
extern char ReadFileName[64];
extern char ChrNameTmp[64];
extern BINH *pBinh;
extern s32 ViewMode;
extern s32 ViewSubMode;
extern s32 DirNum;
extern s32 ChrNum;
extern s16 *pSpecGim;
extern s32 ChrNum2;
extern f32 *pClip;

#endif
