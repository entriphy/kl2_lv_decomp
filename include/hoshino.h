#ifndef HOSHINO_H
#define HOSHINO_H

#include "ps2.h"
#include "types.h"

#pragma region Structs

typedef enum {
    CDREAD_IDLE,
    CDREAD_BGM,
    CDREAD_BGM2,
    CDREAD_PPT,
    CDREAD_DATA,
} CDREAD;

typedef struct {
    s32 Num;
    s32 Exe;
    s32 Reg;
    s32 Arg[32][5];
} hCDCUE;

typedef enum {
    SND_MODE_MONAURAL,
    SND_MODE_STEREO,
    SND_MODE_5P1CH,
    SND_MODE_NUM,
} SND_MODE;

typedef struct {
    sceCdlFILE file;
    sceCdRMode mode;
    s32 ThID;
    s32 DiscType;
    s32 DiscError;
    s32 DiscStat;
    s32 eeCnt; // 0x34
    s32 Sync; // 0x38
    s32 BGMplay; // 0x3C
    s32 dataStat; // 0x40
    s32 dataLSN; // 0x44
    s32 dataSectors; // 0x48
    u8 *buf; // 0x4C
    CDREAD dataFlag; // 0x50
    hCDCUE Cue;
} hCDDATA;

typedef struct {
    SND_MODE Stereo;
    s32 iopBankAddr;
    s32 stageBank;
    s16 stageTblNum;
    s16 stageTbl[1024];
    u8 *Pk;
    s32 PkNum;
    s32 PkMax;
    s32 pad;
    s32 fadeFlag;
    f32 fadeCnt;
    f32 fadeMax;
    s32 ObjList;
    s32 VoiceStat[2];
    s32 KeyonV[2];
    s32 Mute;
    s32 MVol;
    f32 log10Volume;
    f32 dBfader;
    f32 bgmMVol;
    f32 seMVol;
    s32 seObjID;
    s32 seLock;
    f32 effVol;
    f32 effVolBak;
    s32 effIdx;
    s32 effChange;
    s32 effMode;
    s32 effDepth;
    s32 effDelay;
    s32 effFeed;
    s32 effMix;
    f32 pptMVol;
    f32 pptFadeCnt;
    s32 pptFade;
    s32 envNum;
    s32 envObj[16];
    s32 envFade[16];
    s32 envFadeCnt[16];
    s32 envFadeMax[16];
    f32 envFadeVol0[16];
    f32 envFadeVol1[16];
    s32 TitleDelayCnt;
} hSNDDATA;

typedef enum {
    BGM000,
    BGM001,
    BGM002,
    BGM003,
    BGM004,
    BGM005,
    BGM006,
    BGM007,
    BGM008,
    BGM009,
    BGM010,
    BGM011,
    BGM012,
    BGM071,
    BGM013,
    BGM014,
    BGM015,
    BGM016,
    BGM017,
    BGM018,
    BGM019,
    BGM020,
    BGM021,
    BGM022,
    BGM078,
    BGM023,
    BGM077,
    BGM024,
    BGM025,
    BGM026,
    BGM027,
    BGM028,
    BGM029,
    BGM030,
    BGM031,
    BGM032,
    BGM033,
    BGM034,
    BGM035,
    BGM036,
    BGM037,
    BGM038,
    BGM039,
    BGM040,
    BGM062,
    BGM041,
    BGM042,
    BGM043,
    BGM044,
    BGM045,
    BGM046,
    BGM047,
    BGM048,
    BGM050,
    BGM052,
    BGM053,
    BGM054,
    BGM060,
    BGM055,
    BGM056,
    BGM057,
    BGM058,
    BGM059,
    BGM070,
    BGM061,
    BGM075,
    BGM076,
    BGM063,
    BGM064,
    BGM065,
    BGM066,
    BGM067,
    BGM068,
    BGM069,
    BGM072,
    BGM073,
    BGM074,
    BGM079,
    BGM080,
    BGM081,
    BGM082,
    BGM083,
    BGM049,
    BGM084,
    BGM085,
    BGM086,
    BGM172,
    BGM173,
    BGM174,
    BGM181,
    BGM176,
    BGM182,
    BGM099,
    BGM_MAXNUM,
} BGM;

typedef struct {
    sceCdlFILE file;
    s32 Command; // 0x20
    BGM bgmNo;
    s32 bgmCh;
    s32 bgmChMax;
    s32 bgmLength;
    s32 bgmSkipSize;
    s32 bgmNo2;
    f32 bgmVol; // 0x3C
    s32 bgmMute; // 0x40
    s32 fadeFlag;
    s32 fadeCnt;
    s32 fadeMax;
    BGM nextNo;
    s32 nextCh;
    s32 nextFrame;
    f32 nextVol;
    s32 spWk0;
    s32 iopPoint;
    s32 iopNext;
    s32 iopID; // 0x6C
    s32 *iopAddr[2]; // 0x70
    s32 iopOK[2]; // 0x78
    s32 iopFineTop[2]; // 0x80
    s32 iopFine[2]; // 0x88
    s32 cdReq; // 0x90
    s32 cdTop; // 0x94
    s32 cdCur; // 0x98
    s32 cdSectors; // 0x9C
    s32 cdRemain; // 0xA0
} hBGMDATA;

typedef struct {
    sceCdlFILE file;
    s32 reqNum;
    u32 reqID;
    f32 reqVol[4];
    s32 listLoad;
    s32 listPlay;
    s32 listPlayIdx;
    s32 listNum[8];
    u8 *listTbl[8];
    s32 pptMute;
    f32 pptVol[4];
    s32 pptPlay[4];
    s32 pptSize[4];
    s32 pptStop[4];
    s32 pptID[4];
    s32 iopID;
    s32 iopBfID[4];
    s32 iopNext[4];
    s32 *iopAddr[4][2];
    s32 eeID;
    u8 *eeAddr[4];
    s32 eeOffset[4];
    s32 eeStat[4];
} hPPTDATA;

typedef struct {
    sceCdlFILE files[4];
    s32 field_0x80;
    s32 field_0x84;
    s32 field_0x88;
    s32 field_0x8C;
    s32 field_0x90;
    s32 field_0x94;
    s32 field_0x98;
    s32 field_0x9C;
    s32 field_0xA0;
    s32 field_0xA4;
    s32 field_0xA8;
    s32 AC3stat;
    s32 field_0xB0;
    s32 *field_0xB4;
    s32 *field_0xB8;
    s32 field_0xBC;
    s32 field_0xC0;
    s32 field_0xC4;
    s32 field_0xC8;
    s32 field_0xCC;
    u8* field_0xD0;
    u8* field_0xD4;
    u8* field_0xD8;
    u8* field_0xDC;
    s32 field_0xE0;
    s32 field_0xE4;
    s32 field_0xE8;
    s32 field_0xEC;
    // char* bgmStrings[86];
} hAC3DATA; // ?

typedef struct {
    sceCdlFILE file;
} hMOVDATA; // ?

typedef struct {
    s32 eeCnt;
    s32 Command;
    SND_MODE BGMstereo;
    s32 BGMlength;
    s32 BGMskipsize;
    s32 BGMvol;
    s8 BGMch;
    s8 BGMchmax;
    s8 BGMok[2];
    s32 BGMfinetop[2];
    s32 BGMfine[2];
    s32 PPTvol[4];
    s32 PPTsize[4];
    s8 PPTstop[4];
    s32 work0;
    s32 idk;
} STRINFO;

typedef struct {
    s32 start;
    s32 chsize;
    s32 chnum;
} BGMTABLE;

typedef struct {
    s32 top;
    s32 nsector;
} PPTTABLE;

typedef enum {
    BGMMODE_TOP,
    BGMMODE_START,
    BGMMODE_GAME,
    BGMMODE_END,
} BGMMODE;

typedef enum {
    DEAD_DAMAGE,
    DEAD_FALL,
    DEAD_DROWN,
    DEAD_CHOKE,
    DEAD_BURN
} DEAD;

typedef struct { // 0x10
    /* 0x0 */ s32 bgmNo;
    /* 0x4 */ s32 bgmCh;
    /* 0x8 */ f32 fadeIn;
    /* 0xc */ f32 fadeOut;
} BGMAREA;

typedef struct {
    BGMMODE BGMmode;
    s32 BGMppt;
    s32 BGMpptcnt;
    s32 BGMid;
    s32 BGMchgreq;
    s32 BGMchgid;
    s32 BGMchgmode;
    s32 nextVision;
    s32 areaTime;
    s32 resFlag;
    s32 resBGM;
    s32 resVision;
    f32 resEVol;
    s32 deadFlag;
    s32 deadCnt;
} hGAMEDATA;

typedef struct { // 0x10
    /* 0x0 */ u32 Creator;
    /* 0x4 */ u32 Type;
    /* 0x8 */ u32 chunkSize;
    /* 0xc */ u16 reserved;
    /* 0xe */ u8 versionMajor;
    /* 0xf */ u8 versionMinor;
} CHK_VER;

typedef struct { // 0x40
    /* 0x00 */ u32 Creator;
    /* 0x04 */ u32 Type;
    /* 0x08 */ u32 chunkSize;
    /* 0x0c */ u32 headerSize;
    /* 0x10 */ u32 bodySize;
    /* 0x14 */ u32 programChunkAddr;
    /* 0x18 */ u32 samplesetChunkAddr;
    /* 0x1c */ u32 sampleChunkAddr;
    /* 0x20 */ u32 vagInfoChunkAddr;
    /* 0x24 */ u8 reserved[28];
} CHK_HEAD;

typedef struct { // 0x50
    /* 0x00 */ CHK_VER Version;
    /* 0x10 */ CHK_HEAD Header;
} JAMHD;

enum {
    SNDCMD_KEYON = 0,
    SNDCMD_KEYOFF = 1,
    SNDCMD_KEYOFFALL = 2,
    SNDCMD_PITCH = 3,
    SNDCMD_PALPITCH = 4,
    SNDCMD_VOL = 5,
    SNDCMD_MVOL = 6,
    SNDCMD_MVOLALL = 7,
    SNDCMD_EFFECT = 8,
    SNDCMD_EVOL = 9
};

typedef struct {
    s32 efx;
    f32 vol;
    s32 delay;
    s32 feed;
    s32 dry;
    f32 vol_ppt;
    f32 workf0;
    s32 worki0;
    s32 worki1;
} EFXSE;

typedef struct {
    /* 0x00 */ s32 BGMnext;
    /* 0x04 */ s32 BGMpoint;
    /* 0x08 */ s32 PPTnext[4];
    /* 0x18 */ s32 AC3stat;
} hSTRINFO2;

typedef struct {
    u32 vStatKeyon[2];
    hSTRINFO2 STRinfo;
} hRPCINFO;

typedef struct {
    s32 lsn;
    s32 sectors;
} KLTABLE;

typedef struct { // 0xc
    /* 0x0 */ s32 Idx;
    /* 0x4 */ s32 Busy;
    /* 0x8 */ s32 Next;
} hMNGLIST;

typedef struct { // 0xc
    /* 0x0 */ s32 Idx;
    /* 0x4 */ s32 Max;
    /* 0x8 */ hMNGLIST *pList;
} hMNG;

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

typedef struct { // 0x88
    /* 0x00 */ sceDmaChan *dmaGif;
    /* 0x04 */ u64 *pTop;
    /* 0x08 */ u64 *p;
    /* 0x0c */ u64 *pGiftag;
    /* 0x10 */ u64 *pDmatag;
    /* 0x14 */ u64 *pDmatagSV;
    /* 0x18 */ s32 size;
    /* 0x1c */ s32 eopchk;
    /* 0x20 */ u64 prim_def;
    /* 0x28 */ u64 prim;
    /* 0x30 */ s32 bp;
    /* 0x34 */ s32 tbp;
    /* 0x38 */ s32 cbp;
    /* 0x40 */ u64 tex0tmp;
    /* 0x48 */ u64 ctxt;
    /* 0x50 */ u64 tpcl;
    /* 0x58 */ u64 tex0[2];
    /* 0x68 */ u64 tex1[2];
    /* 0x78 */ u64 tex2[2];
} hPACKET_DATA;

typedef struct { // 0xe4
    /* 0x00 */ s32 fd;
    /* 0x04 */ u32 old;
    /* 0x08 */ u32 l;
    /* 0x0c */ u32 on;
    /* 0x10 */ u32 off;
    /* 0x14 */ u32 r;
    /* 0x18 */ u32 r2;
    /* 0x1c */ s32 rep_on[16];
    /* 0x5c */ s32 rcount[16];
    /* 0x9c */ s32 rcount2[16];
    /* 0xdc */ s32 rwait0;
    /* 0xe0 */ s32 rwait1;
} hPAD_DATA;

typedef struct { // 0x5d0
    /* 0x000 */ hPACKET_DATA PkData;
    /* 0x088 */ hPAD_DATA pad[2];
    /* 0x250 */ sceGsDBuffDc db;
    /* 0x580 */ sceDmaChan *dmaGif;
    /* 0x584 */ s32 fr;
    /* 0x588 */ s32 inter;
    /* 0x58c */ s32 light_dir[4][3];
    /* 0x5bc */ u8 light_col[4][4];
} hGLOBAL;

typedef enum {
    CH_IDLE,
    CH_PLAY,
    CH_FADEOUT,
    CH_STOP,
    CH_END
} CH_STAT;

typedef struct {
    CH_STAT stat;
    s32 req;
    s32 idx;
    s32 idx2;
    s32 max;
    BGM no;
    s32 ch;
    s32 size;
} CHALL;

typedef struct { // 0x14
    /* 0x00 */ f32 GVol;
    /* 0x04 */ f32 GVolOld;
    /* 0x08 */ s32 UseList[3];
} SEGRP;

typedef struct { // 0x80
    /* 0x00 */ s32 ID;
    /* 0x04 */ s32 Time;
    /* 0x08 */ s32 Flag;
    /* 0x0c */ s32 Pri;
    /* 0x10 */ s32 GrpNo;
    /* 0x14 */ s32 Next;
    /* 0x18 */ s32 UseVoiceNum;
    /* 0x1c */ s32 Vnum[2];
    /* 0x24 */ f32 fadeCnt;
    /* 0x28 */ f32 OVol;
    /* 0x2c */ f32 DistVol;
    /* 0x30 */ f32 PanL;
    /* 0x34 */ f32 RearL;
    /* 0x38 */ f32 VolL;
    /* 0x3c */ f32 VolLbak;
    /* 0x40 */ f32 PanR;
    /* 0x44 */ f32 RearR;
    /* 0x48 */ f32 VolR;
    /* 0x4c */ f32 VolRbak;
    /* 0x50 */ u32 Pitch;
    /* 0x54 */ f32 *Pos;
    /* 0x58 */ f32 PosCopy[4];
    /* 0x68 */ s32 rearFade;
    /* 0x6c */ f32 rearCnt;
    /* 0x70 */ f32 camDist;
    /* 0x74 */ f32 camSpeed;
    /* 0x78 */ f32 camAng;
    /* 0x7c */ s32 camRear;
} SEOBJ;

typedef struct { // 0x10
    /* 0x0 */ s32 Stat;
    /* 0x4 */ s32 Next;
    /* 0x8 */ s32 GrpNo;
    /* 0xc */ s32 ObjNo;
} SEVOICE;

typedef struct { // 0x30
    /* 0x00 */ s64 splt;
    /* 0x08 */ f32 posx;
    /* 0x0c */ f32 posy;
    /* 0x10 */ f32 posz;
    /* 0x14 */ f32 vol;
    /* 0x18 */ s32 pitch;
    /* 0x1c */ f32 workf0;
    /* 0x20 */ f32 workf1;
    /* 0x24 */ s32 worki0;
    /* 0x28 */ s32 worki1;
} ENVSE;

typedef struct { // 0x10
    /* 0x0 */ u16 TBP : 14;
    /* 0x1 */ u32 : 2;
    /* 0x2 */ u32 TBW : 6;
    /* 0x2 */ u32 : 2;
    /* 0x3 */ u32 PSM : 6;
    /* 0x3 */ u32 : 2;
    /* 0x4 */ u32 : 32;
    /* 0x8 */ u32 PX : 11;
    /* 0x9 */ u32 : 5;
    /* 0xa */ u32 PY : 11;
    /* 0xb */ u32 : 5;
    /* 0xc */ u32 PW : 12;
    /* 0xd */ u32 : 4;
    /* 0xe */ u32 PH : 12;
    /* 0xf */ u32 : 4;
} hGIM_PHEAD;

typedef struct { // 0x10
    /* 0x0 */ u16 CBP : 14;
    /* 0x1 */ u32 : 2;
    /* 0x2 */ u32 CBW : 6;
    /* 0x2 */ u32 : 2;
    /* 0x3 */ u32 CPSM : 6;
    /* 0x3 */ u32 : 2;
    /* 0x4 */ u32 : 32;
    /* 0x8 */ u32 CX : 11;
    /* 0x9 */ u32 : 5;
    /* 0xa */ u32 CY : 11;
    /* 0xb */ u32 : 5;
    /* 0xc */ u32 CW : 12;
    /* 0xd */ u32 : 4;
    /* 0xe */ u32 CH : 12;
    /* 0xf */ u32 : 4;
} hGIM_CHEAD;

#pragma endregion Structs

#pragma region Functions

// h_cdvd.c
extern void hCdReadFile(char *filename, u32 *buff);
extern u32* hCdReadFileRet(char *filename);
extern void hCdCuePushLIFO(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void hCdCuePush(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void hCdCuePop(s32 *p);
extern void hCdCuePopTest(s32 *p);
extern s32  hCdCueNum();
extern s32  hCdCueTime();
extern void hCdCueFlushBGM();
extern void hCdCueFlushPPT();
extern void hCdCueFlushBGM2();
extern s32  hCdRead(u32 lsn, u32 sectors, void *buff, sceCdRMode *mode);
extern s32  hCdReadIOPm(u32 lsn, u32 sectors, void *buff, sceCdRMode *mode);
extern s32  hCdGetSize(s32 no);
extern void hCdReadDataBlock(s32 no, s32 buff);
extern void hCdReadData(s32 no, s32 buff);
extern s32  hCdReadSync();
extern void hCdInit();
extern void hCdMain();
extern s32  hCdDiscStatOK();

// h_debug.c
extern void HsDeb2PtblInit();
extern void HsDeb2Ptbl();

// h_event.c
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

// h_file.c
extern s32  hGameDataSize(s32 mode);
extern void hGameRead(s32 mode, s32 buff);
extern void hSysDataRead(s32 buff);
extern s32  hGameReadOK();
extern s32  hGetDataAddr(s32 i);
extern s32  hGetSysDataAddr(s32 i);

// h_func.c
extern s32  hInit();
extern s32  hMain();

// h_game.c
extern void hInitGame();
extern void hExitGame();
extern void hInitStage0();
extern void hInitStage1();
extern void hExitStage();
extern s32  hGameAreaStart(s32 flag);
extern s32  hGameAreaEnd(s32 next);
extern void hGameMain();
extern void hPushRestart();
extern void hPopRestart();
extern void hKlonoaDead(s32 vision, s32 frame, DEAD pattern);
extern s32  hGameBgmGetID();
extern BGM  hGameBgmGetNo();
extern s32  hGameBgmGetCh();
extern void hGameBgmChange();
extern s32  hGameBgmComp(s32 v0, s32 id0, s32 v1, s32 id1);

// h_gamehall.cc
extern void hCHallInit();
extern void hCHallQuit();
extern s32  hCHallRun(s32 req, s32 arg);

// h_gamesnd.c
extern void hSeLock(s32 i);
extern void hSeInitObj();
extern s32  hSeObjErrChk(s32 obj);
extern s32  hSeGetObj();
extern void hSeReleaseObj(s32 obj);
extern void hSeInitGrp(s32 stage);
extern s32  hSeGetVoice(s32 grp, s32 uselv);
extern void hSeReleaseVoice(s32 vnum);
extern void hSeObjMain();
extern void hSePitch(s32 obj, s32 pitch);
extern f32  hSePanFunc(f32 t);
extern void hSePan(s32 obj);
extern void hSeDist(int obj);
extern s32  hSeKeyOn(s64 splt, sceVu0FVECTOR *parent, s32 reserve);
extern s32  hSeKeyOnEv(s32 prog, s32 splt, sceVu0FVECTOR *pos, s32 flag);
extern void hSeKeyOffAll();
extern void hSeKeyOffAll2();
extern void hSeKeyOff(s32 obj);
extern void hSeKeyOffMain(s32 obj);
extern void hSeSetMVol(f32 vol);
extern void hSeSetGrpVol(s32 grp, f32 vol);
extern void hSeSetObjVol(s32 obj, f32 vol);
extern void hSeSetObjPan(s32 obj, f32 panl, f32 panr);
extern void hSeSetObjdB(s32 obj, f32 db);
extern s32  hSeEnvKeyOn(s32 i);
extern void hSeEnvStart();
extern void hSeEnvChangeVol(s32 obj, f32 vol, s32 frame);
extern void hSeEnvSetVol(f32 vol, s32 obj);


// h_init.c
extern void hInitBoot();
extern void hLoopTop();
extern void hLoopBottom();
extern s32  SyncV();

// h_menu.c


// h_movie.c
extern void hMovInit();

// h_packet.c
extern void hDmaSync(sceDmaChan *DmaChan);
extern void hPkInit(hPACKET_DATA *pk_data);
extern void hPkReset(u128 *pk_area);
extern s32  hPkSetBp(s32 bp);
extern s32  hPkGetSize();
extern void hPkSend();
extern void hSetPrimDefault();
extern void hSetFST(s32 i);
extern void hSetCTXT(s32 i);
extern void hSetFIX(s32 i);
extern void hSetFog(s32 i);
extern void hSetAlpha(s32 i);
extern void hSetAnti(s32 i);
extern void hSetPoint();
extern void hSetLineF2();
extern void hSetLineG2();
extern void hSetLineF2S();
extern void hSetLineG2S();
extern void hSetPolyF3();
extern void hSetPolyG3();
extern void hSetPolyF3S();
extern void hSetPolyG3S();
extern void hSetPolyF3F();
extern void hSetPolyG3F();
extern void hSetPolyFT3();
extern void hSetPolyGT3();
extern void hSetPolyFT3S();
extern void hSetPolyGT3S();
extern void hSetPolyFT3F();
extern void hSetPolyGT3F();
extern void hSetTile();
extern void hSetSprite();
extern void hSetTEXnDefault();
extern void hSetRGB(s32 r, s32 g, s32 b);
extern void hSetRGBA(s32 r, s32 g, s32 b, s32 a);
extern void hSetRGBv(u8 *c);
extern void hSetRGBAv(u8 *c);
extern void hSetAlphaMode(s32 mode);
extern void hSetXYZ(s32 x, s32 y, s32 z);
extern void hSetXYZv(s32 *v);
extern void hSetXYZs(s32 x, s32 y, s32 z);
extern void hSetTpCl(s64 tbp0, s64 psm, s64 tw, s64 th, s64 cbp, s64 cpsm);
extern void hSetTex0(u64 tex0);
extern void hSetUV(s32 u, s32 v);
extern void hSetUVs(s32 u, s32 v);
extern void hSetUVp(s64 uv);
extern void hAddPrim();
extern void hAddDmaTag();
extern void hAddGim(s128 *buff, s32 mode);
extern u64  hGetTex0Tmp();
extern s32  hGet2n(s32 i);

// h_pad.c
extern void hPadInit();
extern void hPadRead();
extern void hPadMake(hPAD_DATA *pp);
extern void hPadGet(hPAD_DATA *pad, s32 i);

// h_rpc.c
extern void hSndRpcRet();
extern s32  hRpcSync();
extern void hRpcInit();
extern s32  hRpc(s32 cmd);
extern s32  hTrans2IOP(s32 iopAddr, s32 eeAddr, s32 size);

// h_sound.c
extern s32  JamGetHdSize(s32 hdaddr);
extern s32  JamGetBdSize(s32 hdaddr);
extern void hSndPkEffect();
extern void hSndPkSetMVol(s32 voll, s32 volr);
extern void hSndPkSetEVol(s32 vol);
extern void hSndPkSetVol(s32 voice, f32 vollf, f32 volrf);
extern void hSndPkSetPitch(s32 voice, s32 pitch);
extern void hSndPkSetPalPitch(s32 voice);
extern void hSndPkKeyOn(s32 voice, s32 flag, s32 bank, s32 prog, s32 splt, f32 vollf, f32 volrf);
extern void hSndPkKeyOff(s32 voice);
extern void hSndPkKeyOffAll();
extern s32  hSndPkGetSize();
extern void hSndReset();
extern void hSndFadeOutAll(s32 frame);
extern void hSndFadeInAll(s32 frame);
extern void hSndSetMVol(f32 vol);
extern s32  hSndFader(f32 vol);
extern f32  hSndFader2(f32 vol);
extern void hSndMain();
extern void hSndInit();
extern void hSndBankSet(s32 packaddr, s32 id);
extern void hSndBankSetMain();
extern void hSndBankSetStage();
extern void hSndEffSetArea();
extern void hSndEffSetVolIdx(s32 idx);
extern void hSndEffSetVol_PPTstart();
extern void hSndEffSetVol_PPTend();
extern void hSndEffSetVol(f32 vol);
extern void hSndSetStereo(SND_MODE i);

// h_str.c
extern void hBgmReset();
extern s32  hBgmGetStat();
extern void hBgmWorkClear();
extern void hPptWorkClear();
extern void hPptReset();
extern void hStrInfo();
extern void hStrInit();
extern void hStr_0016c6e8();
extern void hStr_0016f6e8();

// h_test.c
extern s32  hTestInit();
extern s32  hTestMain();

// h_util.c
extern u32  GetFHMNum(u32 *pAddr);
extern u32* GetFHMAddress(u32 *pAddr, u32 nNum);

#pragma endregion Functions

#pragma region Data

// h_cdvd.c
extern hCDDATA *cD;

// h_func.c
extern s32 (*HFuncTbl[2])();

// h_init.c
extern KLTABLE *KlTable;
extern PPTTABLE *PptTable;
extern BGMTABLE *BgmTable;

// h_menu.c
extern s32 (*hMenuFuncTbl[2])();
extern hGLOBAL hG;
extern s32 RpcArg[16];
extern s128 hPacketArea[2048];

// h_sound.c
extern u8 SndPacket[1024];
extern u8 SndTempBuff[1048576];
extern hSNDDATA *sD;

// h_str.c
extern hBGMDATA *bD;
extern hPPTDATA *pD;
extern hAC3DATA* aD;

// h_test.c
extern s32 TestMode;
extern s32 (*hTestFuncTbl[2])();
extern s32 TestWork;

#pragma endregion Data

#pragma region Defines

#define IOP_IopInit 0x08000000
#define IOP_RpcInfo 0x08000001
#define IOP_StrKick 0x10000003
#define IOP_StrInit 0x14000000
#define IOP_StrInfo 0x1b000001
#define IOP_JamBdTrans  0x20000004
#define IOP_JamBankSet  0x21000003
#define IOP_SndMask 0x22000002
#define IOP_SndInit 0x24000000
#define IOP_SndMain 0x2a000001

#pragma endregion Defines

#endif
