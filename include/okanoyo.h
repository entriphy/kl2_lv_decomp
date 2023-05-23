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
    int scr_x;
    int scr_y;
    int fsz_x;
    int fsz_y;
    ZOSAN_DATA zd[16];
    int z_id;
    int pad0;
    int pad1;
    int pad2;
} PERSONAL_DEB;

typedef struct {
    int tetton2_upspd;
    int tetton2_upmax;
    int tetton2_dwspd;
    int tetton2_dwmax;
    int tetton2_mcspd;
    int tetton2_mcmax;
    int tetton2_count;
    int tetton2_ypmax;
    int gomgumi_cntmax;
    int gomgumi_ydown;
    int gomgumi_mdown;
    int gomgumi_nobih;
    int trmprn_upspd0;
    int trmprn_upspd1;
    int jetimo_upspd;
    int jetimo_acctim;
    int jetimo_livtim;
    int jetimo_dietim;
    int klonoa_dwmax;
    int rocketmuu_pspd;
} ZAK_DEFAULT;

typedef struct {
    int hit_csr_pic;
    int hit_csr_id;
    int hit_csr_on;
    float hit_csr_rot;
    float csr_pos[4];
    int csr_rno;
    int csr_mcn;
    int csr_mode;
    int csr_trg;
    float csr_yofs;
    int csr_dis;
    int csr_xzdis;
    int csr_ydis;
    int csr_keyhis;
    int csr_dbkey;
    int csr_wtkey;
    int csr_dbcnt;
    u8 *ITEMDataBuffp;
    int itemdataflag;
    int OkDebDPcnt;
    int OkDebHTcnt;
    int OkDebKLcnt;
    int OkDebEXcnt;
    int OkDebDPTrg;
    int OkDebHTTrg;
    int OkDebKLTrg;
    int OkDebEXTrg;
    int OkDebM_Elf;
    int pad0;
    ZAK_DEFAULT zak_com;
    int okzcom_mode;
    int okzcom_team;
    int okzcom_csr;
    int okzcom_set;
    float okzcom_valf;
    int okzcom_vali;
    int OkDebugGameMode;
    int texinfocnt;
    int OkDebugCamCurMode;
    int alive_mode;
    int pad1;
    int pad2;
} OKDEBDATAEX;

typedef struct {
    OKDEBDATAEX* debp;
    int pad0;
    int pad1;
    int pad2;
} OKDEBDATA;

typedef struct {
    int fontID;
    int vsonID;
    int modeID;
    int kloDmg;
    int plnMod;
} OKPROFILE;

typedef struct {
    int fontID;
    int menuID;
    int modeID;
    int plan;
    int muteki;
    int pad1;
    long pad0;
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
extern void * getmem(s32 byte);
extern void DebmemFormat(s32 ID);
extern void debmemorySysFormat();
extern void * getBuff(s32 type, s32 byte, char *name, s32 *ret);
extern void freeBuff(s32 type, s32 byte, char *name);

// Data
extern s32 *prgBufPtr[4];
extern s32 *prgBufTop[4];
extern s32 prgBufPtrIdxMax[4];
extern DEBMEM OkDebMem[10];
extern s32 *buffstartptrORG;
extern s32 *buffstartptr;
extern s32 *buffstagetop;
extern s32 *buffareatop;
extern u8 scram_0[4480];
extern u8 scram_1[4480];
extern u8 scram_2[4480];
extern u8 scram_3[4480];
extern u8 scram_4[4480];
extern u8 scram_5[4480];
extern u8 scram_6[4480];
extern u8 scram_7[4480];
extern u8 scram_8[4480];

#endif
