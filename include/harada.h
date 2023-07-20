#ifndef HARADA_H
#define HARADA_H

#include "types.h"
#include "take.h"

#define HR_DUMB_STUPID_NULL_CHECK(x) ((s32)x != -1)

#pragma region Structs

typedef enum {
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
    hKEI_L,
    hKEI_M,
    hKEI_S,
    hKEI_F,
    hKEI_MAX
} hKEI;

typedef struct {
    short area;
    short scene;
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
    hKEI Size;
    s32 Fuku;
    s32 OutLine;
    SFXOBJ *pSfx;
    MOTION *pMot;
    s32 ret;
} tOBJECT;

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

typedef struct { // 0x90
	/* 0x00 */ sceVu0FMATRIX m;
	/* 0x40 */ sceVu0FMATRIX m0;
	/* 0x80 */ mINFO *info;
	/* 0x84 */ s32 rx;
	/* 0x88 */ s32 ry;
	/* 0x8c */ s32 fg;
} BGWK;

typedef struct { // 0x20
    /* 0x00 */ s32 tex0[4];
    /* 0x10 */ s32 flag;
    /* 0x14 */ u32 *gms;
    /* 0x18 */ s32 pad0;
    /* 0x1c */ s32 pad1;
} HRPMWAKU;

typedef struct { // 0x10
    /* 0x0 */ f32 s;
    /* 0x4 */ f32 spds;
    /* 0x8 */ f32 t;
    /* 0xc */ f32 spdt;
} HRSCRST;

typedef struct {
    sceVu0FVECTOR cmin;
    sceVu0FVECTOR cmax;
    sceVu0FVECTOR hmin;
    sceVu0FVECTOR hmax;
    sceVu0FVECTOR hami;
    sceVu0FVECTOR zmax;
    sceVu0FVECTOR zmin;
    sceVu0FVECTOR xmax;
    sceVu0FVECTOR xmin;
    sceVu0FVECTOR ymax;
    sceVu0FVECTOR ymin;
} VPCLIP;

#pragma endregion Structs

#pragma region Functions

// h_vpm2.c
extern void h_vp_init(VPCLIP *vpc, f32 proj, f32 near, f32 far, f32 hamix, f32 hamiy, f32 hamiz, s32 fg);
extern s32  hm_vpclip(sceVu0FMATRIX rtm, sceVu0FVECTOR zure, sceVu0FVECTOR cmax, sceVu0FVECTOR cmin, sceVu0FVECTOR hmax, sceVu0FVECTOR hmin, sceVu0FVECTOR *out);
extern s32  h_vpm_bclip(VPCLIP *vpc, s32 *bhead, sceVu0FMATRIX world_view);
extern s32  h_vpo_vclip(VPCLIP *vpc, sceVu0FVECTOR pos, sceVu0FMATRIX world_view);


// hr_main.c
extern void hr_cold_start();

// hr_pall.c
extern void hr_pt_set(s16 flag, s16 scene, s16 view, s16 th);
extern void hr_pt_fclear();

// hr_pbgm.c


// hr_pflag.c
extern void hr_pflag_initAr();
extern void hr_pflag_initSt();
extern void hr_pflag_init();
extern PT   hr_pflag_get_id();

#pragma endregion Functions

#pragma region Data

// h_vpm2.c
extern VPCLIP cvpm;
extern VPCLIP cvbg;
extern VPCLIP cvpo;

// hr_bgwk.c
extern BGWK *hrbgbuff;
extern s32 hrcntbg;
extern s32 hrcntdrawbg;
extern mINFO hrbgi[6];
extern u32 *hrbgbin[6];

// hr_main.c
extern MAPVWORK mapvw;
extern HRSCRST hrmapst;
extern HRSCRST hrbgst;
extern u32 *hrd_pack;
extern u32 *hrg_pack;
extern u32 *hrf_pack;
extern u32 *hrse_pack;
extern s32 hr_abeoff;
extern s32 hr_objtype;
extern u32 *hr_mt_addr;
// extern vpmINFO hr_mtexi;
extern s32 hr_resKeep;
extern s32 hr_resTmp;
extern u32 *hr_obcdata;
extern u32 *hr_obcvpf;
extern VPOINFO *hr_obcvpo;

// hr_mirr.c
extern s32 hfmircnt;
extern s32 hcmircnt;
extern s32 hrmirflush;
extern s32 hroldflush;
// extern HFMIR hfmirbuf[4];
// extern HCMIR hcmirbuf[1];
// extern HFMIRC hfmcam;
extern u8 *hfm_addr;
extern u8 *hcm_addr;
extern u32 hfm_size;
extern u32 hcm_size;
extern f32 *hfm_scale;
extern f32 *hcm_scale;
// extern V1100MIR *hrm1100;

// hr_pall.c
extern HRPMWAKU ppwaku;
extern s32 ptflag_buff[4];
extern s32 ptflag_st[4];
extern s32 ptflag_ar[4];
extern s32 ptflag_th[4];
extern sceVu0FMATRIX hrpum;
extern sceVu0FMATRIX hrpm;
extern sceVu0FVECTOR hrvi;
extern sceVu0FVECTOR hrvo;
extern s32 *hrpt_addr;
extern s32 *hrptm_addr;
extern s16 hrpt_flag;
extern s16 hrpt_scene;
extern s16 hrpt_view;
extern s16 hrpt_th;
extern s16 hrpt_vt;
extern s16 hrpt_deb;
// extern PCAMSC *hrpcc;
extern u32 *hrpt_pack;
extern u32 *hrpt_gms;
extern s16 hrpt_stnk;
// extern HR_PSYS ppsys;
// extern HR_CALL *hrpt_call;
extern s32 hrpt_mini;
extern s32 *hrpt_mini_addr;

// hr_pbgm.c
extern PT hrpt_id;
extern s32 hrptm_res;
extern s32 hrpt_sbgm;

#pragma endregion Data

#endif
