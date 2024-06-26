#ifndef OKMAIN_H
#define OKMAIN_H

#include "okanoyo.h"

typedef struct {
    sceVu0FVECTOR posdiv;
    sceVu0FVECTOR angdiv;
    sceVu0FVECTOR posdivbak;
    sceVu0FVECTOR angdivbak;
    sceVu0FVECTOR posbak;
    sceVu0FVECTOR angbak;
    sceVu0FVECTOR posdivset;
    sceVu0FVECTOR angdivset;
    sceVu0FVECTOR posset;
    sceVu0FVECTOR angset;
    sceVu0FVECTOR posdivD;
    sceVu0FVECTOR angdivD;
    s16 StatTrgFlag;
    s16 pad0;
    OBJWORK* trgptr;
    s32 camSetFlag;
    s32 camBakFlag;
    s32 camRefFlag;
    s32 camActFlag;
    s32 camActBack;
    s32 camMode;
    s32 camDebug;
    s32 camSet;
    s32 pad1;
    s32 camSetFlagBak;
    s32 stat;
    s32 camType;
    s32 DirectCam;
    s32 orgCamBakFlg;
    s32 setCamDatFlg;
    s32 camJobMode;
    sceVu0FVECTOR fpos;
    sceVu0FVECTOR fang;
    sceVu0FVECTOR tpos;
    sceVu0FVECTOR tang;
    sceVu0FVECTOR ofs;
    sceVu0FVECTOR fofs;
    sceVu0FVECTOR tofs;
    sceVu0FVECTOR fdir;
    sceVu0FVECTOR tdir;
    sceVu0FVECTOR pspd;
    sceVu0FVECTOR aspd;
    sceVu0FVECTOR ospd;
    sceVu0FVECTOR dspd;
    s32 time;
    s32 count;
    s32 pad00;
    s32 pad01;
    f32 retdiv;
    f32 acodiv;
    f32 pcodiv;
    f32 test1;
    sceVu0FVECTOR spd;
    s32 preflag;
    f32 ftz;
    s32 test01;
    s32 act_mode;
    sceVu0FVECTOR fpspd;
    sceVu0FVECTOR faspd;
    s32 time0;
    s32 time1;
    s32 time2;
    s32 time3;
    s32 time4;
    s32 time5;
    s32 time6;
    s32 time7;
    sceVu0FVECTOR getpos;
    sceVu0FVECTOR getang;
    sceVu0FVECTOR cpos;
    sceVu0FVECTOR cang;
    sceVu0FVECTOR cofs;
    sceVu0FVECTOR cdir;
    s32 Directflag;
    s32 mode;
    s32 dum1;
    s32 dum2;
} OkCWork;

extern u8 *scr_p[9];
extern TEXINFO texinfo[256];
extern TEXINFODEB texdeb[256];
extern s32 fontTexID;
extern s32 fontDispID;
extern s32 menuID;
extern s32 scr_x;
extern s32 scr_y;
extern s32 fsz_x;
extern s32 fsz_y;
extern PROFILE profile;
extern char loginName[16];
extern s32 OBJWORKSortFlag;
extern s32 DebTestOn;
extern s32 *prgBufPtr[4];
extern s32 *prgBufTop[4];
extern s32 prgBufPtrIdxMax[4];
extern s32 zakodataflag;
extern char *teki_shutugen;
extern char *teki_Rshutugen;
extern char *zoutflgBufp[32];
extern s32 *ioutflgBufp[32];
extern s32 okzcom_mode;
extern s32 OkDebugGameMode;
extern s32 texinfocnt;
extern s32 OkDebugCamCurMode;
extern s32 alive_mode;
extern DEBMEM OkDebMem[10];
extern s32 *MMptr;
extern s32 *buffstartptrORG;
extern s32 *buffstartptr;
extern s32 *buffstagetop;
extern s32 *buffareatop;
extern char *ZAKODataBuffp;
extern char *ZAKODataBuffTop;
extern s32 fontOnOff;
extern void *ItemSetBufp;
extern s32 ItemSetFlag;
extern s32 *siworkp;
extern u8 *ITEMDataBuffp;
extern s32 itemdataflag;
extern s32 OkDebDPcnt;
extern s32 OkDebDPStcnt;
extern s32 OkDebEXId;
extern s32 OkDebEXcnt;
extern s32 OkDebDPTrg;
extern s32 OkDebHTTrg;
extern s32 OkDebKLTrg;
extern s32 OkDebEXTrg;
extern s32 OkDebM_Elf;
extern s32 OkDebZKRTno;
extern s32 *OkDebSIDTop;
extern s32 OkDebSIDflag;
extern s32 *OkSIDBufp;
extern s32 *OkSTGBufp;
extern s32 OkZakoFuncOn;
extern void *OkDPGVpo;
extern void *OkDPBVpo;
extern s32 OkM_ElfCnt;
extern s32 LastBalloonPic;
extern s32 OkFurorLightCount;
extern s32 OkFurorLightSetCount;
extern s32 OkKeyDoorOpen;
extern OBJWORK *OkFurorLightPtr;
extern f32 OkJumpDaiSpd;
extern OKDEBDATA okDebData;
extern PERSONAL_DEB PDbuf[9];
extern u8 scram_0[4480];
extern u8 scram_1[4480];
extern u8 scram_2[4480];
extern u8 scram_3[4480];
extern u8 scram_4[4480];
extern u8 scram_5[4480];
extern u8 scram_6[4480];
extern u8 scram_7[4480];
extern u8 scram_8[4480];
extern s32 clr_f[9];
extern s32 fontID;
extern void * (*OkDebugGame2pTbl[9])();
extern OkCWork okCwork;
extern s16 *shutu[68];

#endif
