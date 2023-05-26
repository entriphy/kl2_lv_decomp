#ifndef OKANOYO_H
#define OKANOYO_H

#include "types.h"

// Structs

typedef struct {
    u32 top;
    u32 end;
    u64 pad;
} DEBMEM;

typedef struct {
    s32 stat;
    u16 hstart;
    u16 hend;
    s64 col;
} ZOSAN_DATA;

typedef struct {
    s32 scr_x;
    s32 scr_y;
    s32 fsz_x;
    s32 fsz_y;
    ZOSAN_DATA zd[16];
    s32 z_id;
    s32 pad0;
    s32 pad1;
    s32 pad2;
} PERSONAL_DEB;

typedef struct {
    s32 tetton2_upspd;
    s32 tetton2_upmax;
    s32 tetton2_dwspd;
    s32 tetton2_dwmax;
    s32 tetton2_mcspd;
    s32 tetton2_mcmax;
    s32 tetton2_count;
    s32 tetton2_ypmax;
    s32 gomgumi_cntmax;
    s32 gomgumi_ydown;
    s32 gomgumi_mdown;
    s32 gomgumi_nobih;
    s32 trmprn_upspd0;
    s32 trmprn_upspd1;
    s32 jetimo_upspd;
    s32 jetimo_acctim;
    s32 jetimo_livtim;
    s32 jetimo_dietim;
    s32 klonoa_dwmax;
    s32 rocketmuu_pspd;
} ZAK_DEFAULT;

typedef struct {
    s32 hit_csr_pic;
    s32 hit_csr_id;
    s32 hit_csr_on;
    f32 hit_csr_rot;
    f32 csr_pos[4];
    s32 csr_rno;
    s32 csr_mcn;
    s32 csr_mode;
    s32 csr_trg;
    f32 csr_yofs;
    s32 csr_dis;
    s32 csr_xzdis;
    s32 csr_ydis;
    s32 csr_keyhis;
    s32 csr_dbkey;
    s32 csr_wtkey;
    s32 csr_dbcnt;
    u8 *ITEMDataBuffp;
    s32 itemdataflag;
    s32 OkDebDPcnt;
    s32 OkDebHTcnt;
    s32 OkDebKLcnt;
    s32 OkDebEXcnt;
    s32 OkDebDPTrg;
    s32 OkDebHTTrg;
    s32 OkDebKLTrg;
    s32 OkDebEXTrg;
    s32 OkDebM_Elf;
    s32 pad0;
    ZAK_DEFAULT zak_com;
    s32 okzcom_mode;
    s32 okzcom_team;
    s32 okzcom_csr;
    s32 okzcom_set;
    f32 okzcom_valf;
    s32 okzcom_vali;
    s32 OkDebugGameMode;
    s32 texinfocnt;
    s32 OkDebugCamCurMode;
    s32 alive_mode;
    s32 pad1;
    s32 pad2;
} OKDEBDATAEX;

typedef struct {
    OKDEBDATAEX* debp;
    s32 pad0;
    s32 pad1;
    s32 pad2;
} OKDEBDATA;

typedef struct {
    s32 fontID;
    s32 vsonID;
    s32 modeID;
    s32 kloDmg;
    s32 plnMod;
} OKPROFILE;

typedef struct {
    s32 fontID;
    s32 menuID;
    s32 modeID;
    s32 plan;
    s32 muteki;
    s32 pad1;
    s64 pad0;
} PROFILE;

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

typedef struct {
    u64 tex0;
    s32 flag;
    u16 pbp;
    u16 cbp;
    u32 px;
    u32 py;
    u32 cx;
    u32 cy;
    u16 pw;
    u16 ph;
    u32 pad0;
    u32 pad1;
    u32 pad2;
} TEXINFO;

typedef struct { // 0x108
	/* 0x000 */ u8  name[256];
	/* 0x100 */ s16 pw;
	/* 0x102 */ s16 ph;
	/* 0x104 */ s16 px;
	/* 0x106 */ s16 py;
} TEXINFODEB;

// Functions

// okinit.c
extern void OkMainInit(s32 argc, char *argv[]);

// okio.c
extern s32 OkCheck_file(char *name);
extern s32 OkRead_file(char *name, char *buff);
extern void memorySysFormat();
extern void memoryStageptrSet();
extern void memoryStageFormat();
extern void memoryAreaptrSet();
extern void memoryAreaFormat();
extern void OkMemDisp();
extern s32 * getmemblksize();
extern void * getmem(s32 byte);
extern void DebmemFormat(s32 ID);
extern void debmemorySysFormat();
extern void * getBuff(s32 type, s32 byte, char *name, s32 *ret);
extern void freeBuff(s32 type, s32 byte, char *name);

// okmain.c


// okprint.c
extern void OkPrintSysInit(s32 personal_id, s32 screenw, s32 screenh, s32 fontw, s32 fonth);
extern void OkDefaultSysInit(s32 screenw, s32 screenh, s32 fontw, s32 fonth);
extern void OkPFontFlush(s16 trg);

// okprofile.c



// Data

// okinit.c
extern s32 *pfdatap[6];
extern OKDEBDATAEX testdb;

// okio.c
extern s32 testcbp;
extern s32 memtest;
extern s32 memblocksize[16];

// okmain.c
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
extern u8 loginName[16];
extern s32 OBJWORKSortFlag;
extern s32 DebTestOn;
extern s32 *prgBufPtr[4];
extern s32 *prgBufTop[4];
extern s32 prgBufPtrIdxMax[4];
extern s32 zakodataflag;
extern u8 *teki_shutugen;
extern u8 *teki_Rshutugen;
extern u8 *zoutflgBufp[32];
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
extern u8 *ZAKODataBuffp;
extern u8 *ZAKODataBuffTop;
extern s32 fontOnOff;
extern void *ItemSetBufp;
extern s32 ItemSetFlag;
extern s32 *siworkp;
extern u8 *ITEMDataBuffp;
extern s32 itemdataflag;
extern s32 OkDebDPcnt;
extern s32 OkDebHTcnt;
extern s32 OkDebKLcnt;
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

// okprint.c



// okprofile.c
extern OKPROFILE okProfile;
extern s64 profileFlag;

#endif
