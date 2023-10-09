#ifndef HARADA_H
#define HARADA_H

#include "common.h"

#pragma region Defines

#pragma endregion Defines

#pragma region Structs

typedef enum {
    unk = -1,
    PTOPENING,
    PT0100_S2,
    PT0100_S0,
    PT0100_S1,
    PT0104_S0,
    PT0104_S1,
    PT4001_S0,
    PT0204_S0,
    PT0207_S0,
    PT2100_S0,
    PT2100_S1,
    PT2100_S2,
    PT4001_S1,
    PT4002_S0,
    PT0400_S1,
    PT0403_S0,
    PT040A_S0,
    PT040A_S1,
    PT040B_S0,
    PT040B_S1,
    PT040B_S2,
    PT0500_S0,
    PT0505_S2,
    PT0505_S0,
    PT0505_S1,
    PT4002_S1,
    PT4002_S2,
    PT2201_S0,
    PT4001_S2,
    PTXXXX_S0,
    PT0605_S0,
    PT0605_S1,
    PT0609_S0,
    PT0706_S0,
    PT20XX_S0,
    PT20XX_S1,
    PT08XX_S0,
    PT0805_S0,
    PT0706_S1,
    PT4001_S3,
    PT09XX_S0,
    PT09XX_S1,
    PT0908_S1,
    PT1000_S0,
    PT1004_S0,
    PT1100_S0,
    PT1111_S0,
    PT20XX_S2,
    PT20XX_S3,
    PT20XX_S4,
    PT4001_S4,
    PT1200_S0,
    PT1207_S0,
    PT1207_S1,
    PT1207_S2,
    PT1304_S0,
    PT1207_S3,
    PT13XX_S0,
    PT13XX_S1,
    PT13XX_S2,
    PT14XX_S0,
    PT14XX_S1,
    PT20XX_S5,
    PT20XX_S6,
    PT20XX_S7,
    PT1502_S0,
    PT1507_S0,
    PT1507_S1,
    PT1500_S0,
    PT1502_S2,
    PT1502_S1,
    PT16XX_S0,
    PT16XX_S1,
    PT17XX_S0,
    PT2XXX_S0,
    PT2701_S0,
    PT2XXX_S1,
    PT2XXX_S2,
    PT2XXX_S3,
    PTENDING,
    PTMOME_S0,
    PTMOME_S1,
    PTMOME_S2,
    PTMOME_S3,
    PTMOME_S4,
    PTMOME_S5,
    PTTITLE,
    PT0102_S0,
    PT1805_S0,
    PT1908_S0,
    PTSCENE_NUM
} PT;

typedef enum {
    HRTMV_WP,
    HRTMV_WI,
    HRTMV_WA,
    HRTMV_LRP,
    HRTMV_LRI,
} HRTMV;

typedef enum {
    PTEFC_NON,
    PT0100_LTBALL,
    PT0100_RINGLT,
    PT0104_RINGOUT,
    PT0706_SWON,
    PT0706_SWOFF,
    PT0706_SMOKE,
    PT0706_JOUKI,
    PT0706_LTOFF0,
    PT0706_LTOFF1,
    PT0706_LTOFF2,
    PT0706_AWAN,
    PT0706_AWAB,
    PT0706_AWAN1,
    PT0706_AWAB1,
    PT0706_AWAN2,
    PT0706_AWAB2,
    PT4002_ZANZ,
    PT4002_ENGI,
    PTOPEN_HIKA0,
    PTOPEN_HIKA1,
    PTOPEN_HIKA2,
    PTOPEN_HIKA3,
    PT0104_KANE,
    PT0207_LIGHT,
    PT0207_HANE,
    PT2100_SMOKE,
    PT2100_BOSS0,
    PT2100_BOSS1,
    PT2100_KDAMA,
    PT2100_ELEM,
    PT040A_SMOKE,
    PT4004_SPAK,
    PT4004_FIRE,
    PT4004_SMOKE,
    PT4001_ECOL0,
    PT4001_ECOL1,
    PT4001_ECOL2,
    PT4001_ECOL3,
    PT1306_RSHOT,
    PT1306_KIRA,
    PT1306_HAKK,
    PT1305_HIKW,
    PT1305_SITW,
    PT1400_YURA,
    PT2600_FLUSH0,
    PT2600_FLUSH1,
    PT2600_FLUSH2,
    PT2600_FLUSH3,
    PT2600_BFOG,
    PT1404_MOYA,
    PT1505_BAKUH,
    PT1505_SMOKE,
    PT1500_SMOKE,
    PT2700_POWUP,
    PT2700_BARI,
    PT2700_FUJN,
    PTEND_HAMON,
    PTEND_KIER,
    PTEND_ASAHI,
    PT2300_BOSS0,
    PTOPEN_RAKKA,
    PT1111_MIRR,
    PT1111_MIRRF,
    PT4004_AWA,
    PT2301_BOSS1,
    PT2301_BOSS2,
    PT2301_REOSHOT,
    PT1306_HANET,
    PT1306_HANEH,
    PT1502_BAKUH,
    PT0801_BOSS2,
    PT0801_BOSS4,
    PT0801_BOSS7,
    PT0801_BOSS8,
    PT2700_REOSHOT,
    PT1306_KIRA6,
    PT1306_KIRA7,
    PT1306_KIRA8,
    PT1306_KIRA9,
    PT0805_BOSS3,
    PT0805_BOSS4,
    PT0805_BOSS6,
    PT0805_TSMOKE,
    PT0805_RSMOKE,
    PT2400_SMOKE,
    PT2400_BOSS0,
    PT1208_HASIRA,
    PT2200_ELEM,
    PT2300_ELEM,
    PT2400_ELEM,
    PT3901_HAMON0,
    PT3901_HAMON1,
    PT3901_HAMON2,
    PT3901_HAMON3,
    PT3901_HAMON4,
    PT3901_HAMON5,
    PT3901_HAMON6,
    PT3901_HAMON7,
    PT1207_SEPIA,
    PT1111_MONO,
    PT2601_MOYA,
    PT2100_ELEMS,
    PT2200_ELEMS,
    PT2300_ELEMS,
    PT2400_ELEMS,
    PT0207_VISI,
    PT2601_ATAMA,
    PT2700_ELEM,
    PT2700_ELEMS,
    PT2100_WALL,
    PT2301_AWA0,
    PT2301_AWA1,
    PT2301_AWA2,
    PT2301_GRA0,
    PT2301_GRA1,
    PT2301_GRA2,
    PT2600_FLUSH0S,
    PT2600_FLUSH1S,
    PT2600_FLUSH2S,
    PT2600_FLUSH3S,
    PT0605_YUKA,
    PT0605_YUKAD,
    PT1500_SMOKE1,
    PT1500_SMOKE2,
    PT1500_SMOKE3,
    PT1500_SMOKE4,
    PT1500_SMOKE5,
    PT2701_SCROLL,
    PT1208_HASIRA1,
    PT1208_HASIRA2,
    PT1208_HASIRA3,
    PT1507_BAKUH,
    PTTITLE_KUMO,
    PT0105_HAMON,
    PT2100_BOSS2,
    PT2100_BOSS3,
    PT0100_SIBUKI0,
    PT0100_SIBUKI1,
    PT2301_SMOKE,
    PT2301_MERA0,
    PT2301_MERA1,
    PT2700_KDAMA,
    PT0706_SWLON,
    PT0806_SWBOU,
    PTEFC_NUM,
} PTEFC;

typedef enum {
    PTE_HARADA,
    PTE_ABE,
    PTE_HATO,
    PTE_ALLNUM,
} PTE;

typedef struct { // 0x2050
    /* 0x0000 */ u32 block_list[2049];
    /* 0x2004 */ qword *block_head_ptr;
    /* 0x2008 */ u32 *clip_head_ptr;
    /* 0x200c */ u32 *vpm_data_top;
    /* 0x2010 */ u32 vpm_zone_num;
    /* 0x2014 */ u32 vpm_block_num;
    /* 0x2018 */ s32 course_level;
    /* 0x201c */ s32 fog_near;
    /* 0x2020 */ s32 fog_far;
    /* 0x2030 */ qword fog_col;
    /* 0x2040 */ s32 pixel_intpl;
    /* 0x2044 */ u8 *data_buff;
    /* 0x2048 */ u8 *hm_buff;
} vpmINFO;

typedef enum {
    hKEI_L,
    hKEI_M,
    hKEI_S,
    hKEI_F,
    hKEI_MAX
} hKEI;

typedef enum {
    HRPSIZE_L,
    HRPSIZE_M,
    HRPSIZE_S,
    HRPSIZE_F
} HRPSIZE;

typedef enum {
    PGETFT_ML,
    PGETFT_MP,
    PGETFT_LP,
    PGETFT_TM,
    PGETFT_ZN,
    PGETFT_NUM
} PGETFT;

typedef struct {
    s16 area;
    s16 scene;
} PTDATA;

typedef struct tCOORD {
    sceVu0FVECTOR Rot;
    sceVu0FVECTOR Trans;
    sceVu0FMATRIX LcMtx;
    sceVu0FMATRIX LwMtx;
    sceVu0FMATRIX LsMtx;
    sceVu0FMATRIX LvMtx;
    sceVu0FMATRIX LcLightMtx;
    struct tCOORD *Super;
    s32 Flag;
} tCOORD;

typedef struct {
    tCOORD Base;
    sceVu0FMATRIX *pNormalLight;
    sceVu0FMATRIX *pLightColor;
    f32 Scale;
    s32 Size;
    s32 Fuku;
    s32 OutLine;
    SFXOBJ *pSfx;
    MOTION *pMot;
    s32 ret;
} tOBJECT;

typedef tOBJECT HOT;
typedef tCOORD COLD;

typedef struct { // 0x4
	/* 0x0 */ u8 r;
	/* 0x1 */ u8 b;
	/* 0x2 */ u8 g;
	/* 0x3 */ u8 a;
} CVECTOR;

typedef struct { // 0x30
    /* 0x00 */ s32 ids;
    /* 0x04 */ s32 idv;
    /* 0x08 */ s16 map_spds;
    /* 0x0a */ s16 map_spdt;
    /* 0x0c */ s16 bg_spds;
    /* 0x0e */ s16 bg_spdt;
	/* 0x10 */ sceVu0IVECTOR fog_col;
	/* 0x20 */ s32 fog_near;
	/* 0x24 */ s32 fog_far;
	/* 0x28 */ CVECTOR bgt;
	/* 0x2c */ CVECTOR bgu;
} MAPVWORK;

typedef struct { // 0x240
	/* 0x000 */ u32 block_list[128];
	/* 0x200 */ u32 drawb_num;
	/* 0x204 */ qword *block_head_ptr;
	/* 0x208 */ u32 *clip_head_ptr;
	/* 0x20c */ u32 *vpm_data_top;
	/* 0x210 */ u32 vpm_zone_num;
	/* 0x214 */ u32 vpm_block_num;
	/* 0x218 */ s32 fog_near;
	/* 0x21c */ s32 fog_far;
	/* 0x220 */ qword fog_col;
	/* 0x230 */ s32 pixel_intpl;
	/* 0x234 */ u8 *data_buff;
} mINFO;

typedef struct HRANMV { // 0x40
    /* 0x00 */ sceVu0FVECTOR pos;
    /* 0x10 */ vpmINFO *info;
    /* 0x14 */ void (*prog)(struct HRANMV *);
    /* 0x18 */ u32 flag;
    /* 0x1c */ s32 drawno;
    /* 0x20 */ s32 work0;
    /* 0x24 */ s32 work1;
    /* 0x28 */ s32 work2;
    /* 0x2c */ s32 work3;
    /* 0x30 */ f32 fwk0;
    /* 0x34 */ f32 fwk1;
    /* 0x38 */ f32 fwk2;
    /* 0x3c */ f32 fwk3;
} HRANMV;

typedef struct { // 0x10
    /* 0x0 */ void (*prog)(HRANMV *);
    /* 0x4 */ void (*init)(HRANMV *);
    /* 0x8 */ s32 drawno;
    /* 0xc */ vpmINFO *info;
} HRANMVS;

typedef struct { // 0x30
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
    /* 0x08 */ f32 z;
    /* 0x0c */ f32 r;
    /* 0x10 */ f32 g;
    /* 0x14 */ f32 b;
    /* 0x18 */ f32 n;
    /* 0x1c */ f32 f;
    /* 0x20 */ f32 ri;
    /* 0x24 */ f32 gi;
    /* 0x28 */ f32 bi;
    /* 0x2c */ f32 pad0;
} HRAVL;

typedef enum {
    HRAV_WAVE,
    HRAV_SINT,
    HRAV_SINT2,
    HRAV_SINTD,
    HRAV_SINT2D,
} HRAV;

typedef struct { // 0x50
    /* 0x00 */ qword foga;
    /* 0x10 */ qword fogb;
    /* 0x20 */ qword max;
    /* 0x30 */ qword min;
    /* 0x40 */ qword spot;
} ATR_LIGHTP;

typedef struct { // 0x90
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword vif0;
    /* 0x20 */ qword tag;
    /* 0x30 */ ATR_LIGHTP light0;
    /* 0x80 */ qword mscal;
} ATR_AVPSINT0;

typedef struct { // 0xe0
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword vif0;
    /* 0x20 */ qword tag;
    /* 0x30 */ ATR_LIGHTP light0;
    /* 0x80 */ ATR_LIGHTP light1;
    /* 0xd0 */ qword mscal;
} ATR_AVPSINT2;

typedef struct { // 0x20
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword mscal;
} ATR_AVPSINT2Z;

typedef struct { // 0x1c
    /* 0x00 */ f32 xs;
    /* 0x04 */ f32 ys;
    /* 0x08 */ f32 zs;
    /* 0x0c */ u16 gcntx;
    /* 0x0e */ u16 gcnty;
    /* 0x10 */ u16 gcntz;
    /* 0x12 */ u16 pad0;
    /* 0x14 */ u32 gsize;
    /* 0x18 */ u32 gsizey;
} HR_HPMK;

typedef struct { // 0x8
    /* 0x0 */ u16 z;
    /* 0x2 */ u16 x;
    /* 0x4 */ u32 addr;
} HR_HPXZ;

typedef struct { // 0x4
    /* 0x0 */ u16 y;
    /* 0x2 */ u16 cnt;
} HR_HPYL;

typedef struct { // 0x24
    /* 0x00 */ u16 gx;
    /* 0x02 */ u16 gy;
    /* 0x04 */ u16 gz;
    /* 0x06 */ u16 pad0;
    /* 0x08 */ u32 xzcnt;
    /* 0x0c */ u32 addr;
    /* 0x10 */ u32 ylen;
    /* 0x14 */ u16 y;
    /* 0x16 */ u16 cnt;
    /* 0x18 */ u32 gouk;
    /* 0x1c */ u32 qwc;
    /* 0x20 */ u32 daddr;
} HR_HPDEB;

typedef struct { // 0x10
    /* 0x00 */ u32 ntag;
    /* 0x04 */ qword *dmatag;
    /* 0x08 */ u32 *data_top;
    /* 0x0c */ u32 pad0;
} VPOINFO;

typedef struct { // 0x50
    /* 0x00 */ sceVu0FVECTOR p;
    /* 0x10 */ sceVu0FVECTOR spd;
    /* 0x20 */ sceVu0FVECTOR acc;
    /* 0x30 */ sceVu0FVECTOR moku;
    /* 0x40 */ s32 flag;
    /* 0x44 */ s32 time;
    /* 0x48 */ s32 pad0;
    /* 0x4c */ s32 pad1;
} PCAMSC;

typedef struct { // 0x20
    /* 0x00 */ f32 p;
    /* 0x04 */ f32 spd;
    /* 0x08 */ f32 acc;
    /* 0x0c */ f32 moku;
    /* 0x10 */ s32 flag;
    /* 0x14 */ s32 time;
    /* 0x18 */ s32 pad0;
    /* 0x1c */ s32 pad1;
} PCAMSL;

typedef struct { // 0xd0
    /* 0x00 */ sceVu0FVECTOR v;
    /* 0x10 */ sceVu0FMATRIX hm;
    /* 0x50 */ sceVu0FVECTOR hv;
    /* 0x60 */ PCAMSC r;
    /* 0xb0 */ sceVu0FVECTOR rr;
    /* 0xc0 */ sceVu0FMATRIX *hdmk;
    /* 0xc4 */ sceVu0FVECTOR *hdv;
    /* 0xc8 */ s32 flag;
    /* 0xcc */ s32 pad0;
} PCAMSM;

typedef struct { // 0x310
    /* 0x000 */ PCAMSC wp;
    /* 0x050 */ PCAMSC wi;
    /* 0x0a0 */ PCAMSC wa;
    /* 0x0f0 */ sceVu0FVECTOR pos;
    /* 0x100 */ sceVu0FVECTOR ang;
    /* 0x110 */ sceVu0FVECTOR inte;
    /* 0x120 */ PCAMSM mp;
    /* 0x1f0 */ PCAMSM mi;
    /* 0x2c0 */ PCAMSL proj;
    /* 0x2e0 */ PCAMSL len;
    /* 0x300 */ s32 flag;
    /* 0x304 */ s32 bmax;
    /* 0x308 */ s32 bnow;
    /* 0x30c */ f32 *bptr;
} PCAMS;

typedef struct { // 0x20
    /* 0x00 */ s32 p;
    /* 0x04 */ s32 spd;
    /* 0x08 */ s32 acc;
    /* 0x0c */ s32 moku;
    /* 0x10 */ s32 flag;
    /* 0x14 */ s32 time;
    /* 0x18 */ u32 *addr;
    /* 0x1c */ s32 pad0;
} PCAMSR;

typedef struct { // 0x3d0
    /* 0x000 */ PCAMSC pos;
    /* 0x050 */ PCAMSC rot;
    /* 0x0a0 */ sceVu0FVECTOR nlight[3];
    /* 0x0d0 */ PCAMSC nlw[3];
    /* 0x1c0 */ PCAMSC lcolor[3];
    /* 0x2b0 */ sceVu0FVECTOR ambi;
    /* 0x2c0 */ sceVu0FMATRIX NormalLight;
    /* 0x300 */ sceVu0FMATRIX LightColor;
    /* 0x340 */ sceVu0FVECTOR rtpos;
    /* 0x350 */ sceVu0FVECTOR rtrot;
    /* 0x360 */ s32 flag;
    /* 0x364 */ s32 *read;
    /* 0x368 */ s32 *ret;
    /* 0x36c */ tOBJECT *hObj;
    /* 0x370 */ u16 motno;
    /* 0x372 */ u16 motlp;
    /* 0x374 */ u16 motret;
    /* 0x376 */ u16 motlpi;
    /* 0x378 */ s16 id;
    /* 0x37a */ s16 chr;
    /* 0x37c */ s16 fhm;
    /* 0x37e */ s16 time;
    /* 0x380 */ s32 getft;
    /* 0x384 */ s32 lightno;
    /* 0x388 */ s32 efc;
    /* 0x38c */ s32 *mem;
    /* 0x390    f32 shady; */
    /* 0x390 */ s32 sevag; // ?
    /* 0x394 */ s32 pad0;
    /* 0x398 */ s16 loop[2];
    /* 0x39c */ s16 pad2;
    /* 0x39e */ s16 pad3;
    /* 0x3a0 */ PCAMSR route;
    /* 0x3c0 */ RT_WRK rtw;
} HR_CALL;

typedef struct { // 0x10
    /* 0x0 */ u8 r;
    /* 0x1 */ u8 g;
    /* 0x2 */ u8 b;
    /* 0x3 */ u8 a;
    /* 0x4 */ u8 ma;
    /* 0x5 */ u8 pad0;
    /* 0x6 */ u8 pad1;
    /* 0x7 */ u8 pad2;
    /* 0x8 */ s32 flag;
    /* 0xc */ u16 time;
    /* 0xe */ u16 count;
} HR_FADE;

typedef struct { // 0x20
    /* 0x00 */ s32 id;
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0c */ f32 z;
    /* 0x10 */ f32 rx;
    /* 0x14 */ f32 ry;
    /* 0x18 */ f32 rz;
    /* 0x1c */ f32 pad0;
} HR_ECONT;

typedef struct { // 0x20
    /* 0x00 */ s32 trg;
    /* 0x04 */ s32 oldtrg;
    /* 0x08 */ s32 lvl;
    /* 0x0c */ s32 oldlvl;
    /* 0x10 */ s32 open;
    /* 0x14 */ s32 pad0;
    /* 0x18 */ s32 pad1;
    /* 0x1c */ s32 pad2;
} HR_PKEY;

typedef struct { // 0x50
    /* 0x00 */ PCAMSR pit;
    /* 0x20 */ PCAMSL vol;
    /* 0x40 */ s32 flag;
    /* 0x44 */ s32 id;
    /* 0x48 */ s32 pad0;
    /* 0x4c */ u32 splt;
} HR_PTSE;

typedef struct { // 0x20
    /* 0x00 */ s32 tex0[4];
    /* 0x10 */ s32 flag;
    /* 0x14 */ u32 *gms;
    /* 0x18 */ s32 pad0;
    /* 0x1c */ s32 pad1;
} HRPMWAKU;

typedef struct { // 0x40
    /* 0x00 */ s16 time;
    /* 0x02 */ s16 wtime;
    /* 0x04 */ s32 dcnt;
    /* 0x08 */ u16 *dtop;
    /* 0x0c */ u16 *page;
    /* 0x10 */ u16 *read;
    /* 0x14 */ u16 *next;
    /* 0x18 */ s32 flag;
    /* 0x1c */ s32 mode;
    /* 0x20 */ s32 count;
    /* 0x24 */ u32 *font;
    /* 0x28 */ s32 stop;
    /* 0x2c */ s16 cntv;
    /* 0x2e */ s16 hasv;
    /* 0x30 */ u32 *mema;
    /* 0x34 */ s32 acnt;
    /* 0x38 */ s32 skipcnt;
    /* 0x3c */ s32 pad0;
} HRPMOJI;

typedef struct { // 0x10
    /* 0x0 */ HR_CALL *ca;
    /* 0x4 */ u16 *ptr;
    /* 0x8 */ s16 flag;
    /* 0xa */ s16 time;
    /* 0xc */ s16 alpha;
    /* 0xe */ s16 count;
} HRMESP;

typedef struct { // 0x4
    /* 0x0 */ u16 code;
    /* 0x2 */ s16 ss0;
} PT32A;

typedef struct { // 0x4
    /* 0x0 */ u16 code;
    /* 0x2 */ u16 us0;
} PT32B;

typedef struct { // 0x8
    /* 0x0 */ u16 code;
    /* 0x2 */ s16 ss0;
    /* 0x4 */ s16 ss1;
    /* 0x6 */ s16 ss2;
} PT64A;

typedef struct { // 0x8
    /* 0x0 */ u16 code;
    /* 0x2 */ u16 us0;
    /* 0x4 */ s32 si0;
} PT64B;

typedef struct { // 0x8
    /* 0x0 */ u16 code;
    /* 0x2 */ u16 us0;
    /* 0x4 */ f32 f0;
} PT64C;

typedef struct { // 0x8
    /* 0x0 */ u16 code;
    /* 0x2 */ u8 uc0;
    /* 0x3 */ u8 uc1;
    /* 0x4 */ s32 si0;
} PT64D;

typedef struct { // 0x8
    /* 0x0 */ u16 code;
    /* 0x2 */ u16 us0;
    /* 0x4 */ u8 uc0;
    /* 0x5 */ u8 uc1;
    /* 0x6 */ u8 uc2;
    /* 0x7 */ u8 uc3;
} PT64E;

typedef struct { // 0x8
    /* 0x0 */ u16 code;
    /* 0x2 */ u8 uc0;
    /* 0x3 */ u8 uc1;
    /* 0x4 */ f32 f0;
} PT64F;

typedef struct { // 0xc
    /* 0x0 */ u16 code;
    /* 0x2 */ u16 us0;
    /* 0x4 */ u16 us1;
    /* 0x6 */ s16 ss0;
    /* 0x8 */ u32 ui0;
} PT96A;

typedef struct { // 0xc
    /* 0x0 */ u16 code;
    /* 0x2 */ u16 us0;
    /* 0x4 */ u16 us1;
    /* 0x6 */ s16 ss0;
    /* 0x8 */ f32 f0;
} PT96B;

typedef struct { // 0xc
    /* 0x0 */ u16 code;
    /* 0x2 */ u16 us0;
    /* 0x4 */ u32 ui0;
    /* 0x8 */ u32 ui1;
} PT96C;

typedef struct { // 0xc
    /* 0x0 */ u16 code;
    /* 0x2 */ u16 us0;
    /* 0x4 */ s32 si0;
    /* 0x8 */ f32 f0;
} PT96D;

typedef struct { // 0xc
    /* 0x0 */ u16 code;
    /* 0x2 */ u8 uc0;
    /* 0x3 */ u8 uc1;
    /* 0x4 */ s32 si0;
    /* 0x8 */ u32 ui0;
} PT96E;

typedef struct { // 0xc
    /* 0x0 */ u16 code;
    /* 0x2 */ u8 uc0;
    /* 0x3 */ u8 uc1;
    /* 0x4 */ f32 f0;
    /* 0x8 */ u32 ui0;
} PT96F;

typedef struct { // 0x10
    /* 0x0 */ u16 code;
    /* 0x2 */ s16 ss0;
    /* 0x4 */ f32 f0;
    /* 0x8 */ f32 f1;
    /* 0xc */ f32 f2;
} PT128A;

typedef struct { // 0x10
    /* 0x0 */ u16 code;
    /* 0x2 */ u8 uc0;
    /* 0x3 */ u8 uc1;
    /* 0x4 */ f32 f0;
    /* 0x8 */ f32 f1;
    /* 0xc */ f32 f2;
} PT128B;

typedef struct { // 0x10
    /* 0x0 */ u16 code;
    /* 0x2 */ u16 us0;
    /* 0x4 */ u16 us1;
    /* 0x6 */ s16 ss0;
    /* 0x8 */ u32 ui0;
    /* 0xc */ s16 ss1;
    /* 0xe */ s16 ss2;
} PT128C;

typedef struct { // 0x14
    /* 0x00 */ u16 code;
    /* 0x02 */ u8 uc0;
    /* 0x03 */ u8 uc1;
    /* 0x04 */ f32 f0;
    /* 0x08 */ f32 f1;
    /* 0x0c */ f32 f2;
    /* 0x10 */ s16 ss0;
    /* 0x12 */ s16 ss1;
} PT160A;

typedef struct {
    sceVu0FMATRIX wmat;
    HR_CALL sys;
    HR_FADE fade;
    HR_ECONT econ[8];
    HR_PKEY key;
    HR_PTSE se[4];
    u8 r;
    u8 g;
    u8 b;
    u8 a;
    s16 klov;
    s16 kloy;
    s32 klomc;
    s16 klono;
    s16 ecount0; // 0x68E
    s32 flag; // 0x690
    s32 nflag;
    PT96A *flagptr[4];
    s32 ncall;
    HR_CALL *call; // 0x6AC
    PCAMS *cam; // 0x6B0
    HR_CALL *posc; // 0x6B4
    HR_CALL *intec; // 0x6B8
    s32 *model; // 0x6BC
    HRPMOJI *pmes; // 0x6C0
    HRPMWAKU *waku; // 0x6C4
    OBJWORK *efc; // 0x6C8
    s16 rtno;
    s16 oksw;
    int rtmc;
    s32 yuka;
    s32 cntvc;
    s32 *addrvc;
    s32 idvc;
    s32 cntlip;
    u16 *addrlip;
    s16 dckcnt;
    s16 skipcnt;
    s16 ncVi;
    s16 ncSc;
    s16 reqbgm; // 0x6F4
    s16 mircnt;
    HRMESP *mesp;
} HR_PSYS;

#pragma endregion Structs


#endif
