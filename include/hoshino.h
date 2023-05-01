#ifndef HOSHINO_H
#define HOSHINO_H

#include "ps2.h"
#include "types.h"

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

typedef enum {
    hKEI_L,
    hKEI_M,
    hKEI_S,
    hKEI_F,
    hKEI_MAX
} hKEI;

typedef enum {
    CDREAD_IDLE,
    CDREAD_BGM,
    CDREAD_BGM2,
    CDREAD_PPT,
    CDREAD_DATA,
} CDREAD;

typedef struct {
    int Num;
    int Exe;
    int Reg;
    int Arg[32][5];
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
    int* iopBankAddr;
    int stageBank;
    short stageTblNum;
    short stageTbl[1024];
    u8* PkNum; // Value or pointer?
    int PkMax;
    int pad;
    int fadeFlag;
    float fadeCnt;
    float fadeMax;
    int ObjList;
    int VoiceStat[2];
    int KeyonV[2];
    int Mute;
    int MVol;
    float log10Volume;
    float dBfader;
    float bgmMVol;
    float seMVol;
    int seObjID;
    int seLock;
    float effVol;
    float effVolBak;
    int effIdx;
    int effChange;
    int effMode;
    int effDepth;
    int effDelay;
    int effFeed;
    int effMix;
    float pptMVol;
    float pptFadeCnt;
    int pptFade;
    int envNum;
    int envObj[16];
    int envFade[16];
    int envFadeCnt[16];
    int envFadeMax[16];
    float envFadeVol0[16];
    float envFadeVol1[16];
    int TitleDelayCnt;
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
    int Command; // 0x20
    BGM bgmNo;
    int bgmCh;
    int bgmChMax;
    int bgmLength;
    int bgmSkipSize;
    int bgmNo2;
    float bgmVol; // 0x3C
    int bgmMute; // 0x40
    int fadeFlag;
    int fadeCnt;
    int fadeMax;
    BGM nextNo;
    int nextCh;
    int nextFrame;
    float nextVol;
    int spWk0;
    int iopPoint;
    int iopNext;
    int iopID; // 0x6C
    int* iopAddr[2]; // 0x70
    int iopOK[2]; // 0x78
    int iopFineTop[2]; // 0x80
    int iopFine[2]; // 0x88
    int cdReq; // 0x90
    int cdTop; // 0x94
    int cdCur; // 0x98
    int cdSectors; // 0x9C
    int cdRemain; // 0xA0
} hBGMDATA;

typedef struct {
    sceCdlFILE file;
    int reqNum;
    u32 reqID;
    float reqVol[4];
    int listLoad;
    int listPlay;
    int listPlayIdx;
    int listNum[8];
    u8* listTbl[8];
    int pptMute;
    float pptVol[4];
    int pptPlay[4];
    int pptSize[4];
    int pptStop[4];
    int pptID[4];
    int iopID;
    int iopBfID[4];
    int iopNext[4];
    int* iopAddr[4][2];
    int eeID;
    u8* eeAddr[4];
    int eeOffset[4];
    int eeStat[4];
} hPPTDATA;

typedef struct {
    sceCdlFILE files[4];
    int field_0x80;
    int field_0x84;
    int field_0x88;
    int field_0x8C;
    int field_0x90;
    int field_0x94;
    int field_0x98;
    int field_0x9C;
    int field_0xA0;
    int field_0xA4;
    int field_0xA8;
    int field_0xAC;
    int field_0xB0;
    int* field_0xB4;
    int* field_0xB8;
    int field_0xBC;
    int field_0xC0;
    int field_0xC4;
    int field_0xC8;
    int field_0xCC;
    u8* field_0xD0;
    u8* field_0xD4;
    u8* field_0xD8;
    u8* field_0xDC;
    int field_0xE0;
    int field_0xE4;
    int field_0xE8;
    int field_0xEC;
    // char* bgmStrings[86];
} hAC3DATA; // ?

typedef struct {
    sceCdlFILE file;
} hMOVDATA; // ?

typedef struct {
    int eeCnt;
    int Command;
    SND_MODE BGMstereo;
    int BGMlength;
    int BGMskipsize;
    int BGMvol;
    s8 BGMch;
    s8 BGMchmax;
    s8 BGMok[2];
    int BGMfinetop[2];
    int BGMfine[2];
    int PPTvol[4];
    int PPTsize[4];
    s8 PPTstop[4];
    int work0;
    int idk;
} STRINFO;

typedef struct {
    int start;
    int chsize;
    int chnum;
} BGMTABLE;

typedef struct {
    int top;
    int nsector;
} PPTTABLE;

typedef enum {
    BGMMODE_TOP,
    BGMMODE_START,
    BGMMODE_GAME,
    BGMMODE_END,
} BGMMODE;

typedef struct {
    BGMMODE BGMmode;
    int BGMppt;
    int BGMpptcnt;
    int BGMid;
    int BGMchgreq;
    int BGMchgid;
    int BGMchgmode;
    int nextVision;
    int areaTime;
    int resFlag;
    int resBGM;
    int resVision;
    float resEVol;
    int deadFlag;
    int deadCnt;
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

#endif
