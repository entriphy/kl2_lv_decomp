#ifndef HOSHINO_H
#define HOSHINO_H

#include <sifcmd.h>
#include <sifrpc.h>
#include <libcdvd.h>

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
    int ThID;
    int DiscType;
    int DiscError;
    int DiscStat;
    int eeCnt; // 0x34
    int Sync; // 0x38
    int BGMplay; // 0x3C
    int dataStat; // 0x40
    int dataLSN; // 0x44
    int dataSectors; // 0x48
    int pad;
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

typedef struct{
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
} hSTRDATA; // ?

static const char* modules[8] = {
    "cdrom:\\M\\SIFCMD.IRX;1",
    "cdrom:\\M\\SIO2MAN.IRX;1",
    "cdrom:\\M\\MCMAN.IRX;1",
    "cdrom:\\M\\MCSERV.IRX;1",
    "cdrom:\\M\\PADMAN.IRX;1",
    "cdrom:\\M\\LIBSD.IRX;1",
    "cdrom:\\M\\SDRDRV.IRX;1",
    "cdrom:\\M\\KL2.IRX;1"
}; // DAT_00348970

inline hCDDATA CdData; // DAT_00338390
inline u8 EeCb[0x2000]; // DAT_00338670
inline hSNDDATA SndData; // DAT_0034b140
inline hBGMDATA BgmData; // DAT_0034bb58
inline hPPTDATA PptData; // DAT_0034bc00
inline hAC3DATA Ac3Data; // DAT_0034bd50
inline hSTRDATA StrData; // DAT_003bfed0

inline SifRpcClientData_t rpc__003d9718; // DAT_003d9718

inline hCDDATA* cD; // DAT_003fa808
inline hCDCUE* cQ; // DAT_003fa80c

inline int* kldataHead; // DAT_003fa830
inline int* pptpackHead; // DAT_003fa834
inline int* bgmpackHead; // DAT_003fa838

inline hSNDDATA* sD; // DAT_003fa83c
inline hBGMDATA* bD; // DAT_003fa840
inline hPPTDATA* pD; // DAT_003fa844
inline hAC3DATA* aD; // DAT_003fa844

inline u8 pptEeAddrs[4][0x40000]; // DAT_00417700
static void* buffstartptr = (void*)0x00A00000; // DAT_003fbb3c

inline hSTRDATA* strD; // DAT_003bfed0

inline int RpcArg[16]; // DAT_0040efa0
inline int RpcRecvBuf[2][16]; // DAT_00417200
inline int SndMainBuffer[16]; // DAT_00417300
inline SifRpcClientData_t sndRpc; // DAT_00417280

inline int boot_flag; // ?, DAT_0061ad0

void* hReadFile(const char* name); // FUN_00165ae0
void hCdInit(); // FUN_00166248
void hSeLock(int i); // FUN_00168e48
void hSeInitGrp(int stage); // FUN_00168fd8
void hInitBoot(); // FUN_0016ac00
int FUN_0016c778();
void FUN_0016c798();
int* hIopDispatch(u32 param); // FUN_0016c830
void hSndPkEffect(); // FUN_0016ca30
void hSndPkSetMVol(int voll, int volr); // FUN_0016cb40
void hSndPkSetEVol(int vol); // FUN_0016cbc8
void hSndSetMVol(float vol); // FUN_0016cfe0
int hSndFader(float vol); // FUN_0016d008
void FUN_00196c00();
void hBgmWorkClear(); // FUN_0016e350
int hSndPkGetSize(); // FUN_0016cec8
void hSndInit(); // FUN_003fc0c8
void init_config_system(); // FUN_001994e0
int FUN_001d1c08(const char* name);
int FUN_001d1c78(const char* name, void* buf);
void* getBuff(int type, int byte_, const char* name, int* ret); // FUN_001d3748
void hStrInit(); // FUN_002da268
int FUN_0030dad0();
void FUN_00318a80();
void FUN_003189d0();

#endif