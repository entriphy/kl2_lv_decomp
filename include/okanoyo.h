#ifndef OKANOYO_H
#define OKANOYO_H

#include "common.h"

#define COPYVECTOR(v0, v1) (v0[0] = v1[0], v0[1] = v1[1], v0[2] = v1[2], v0[3] = v1[3])
#define SETVECTOR(v, x, y, z, w) (v[0] = x, v[1] = y, v[2] = z, v[3] = w)
#define NORMRAD(x) ({if (x < -M_PIf) { x += M_TWOPIf; } else if (x > M_PIf) { x -= M_TWOPIf; }})
#define FADR(x, i) ((void *)((u8 *)x + ((s32 *)x)[i]))

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
    OKDEBDATAEX *debp;
    s32 pad0;
    s32 pad1;
    s32 pad2;
} OKDEBDATA;

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
    /* 0x000 */ char name[256];
    /* 0x100 */ s16 pw;
    /* 0x102 */ s16 ph;
    /* 0x104 */ s16 px;
    /* 0x106 */ s16 py;
} TEXINFODEB;

typedef union { // 0x10
    /* 0x0 */ u128 ul128;
    /* 0x0 */ u64 ul64[2];
    /* 0x0 */ u32 ui32[4];
} QWdata;

typedef struct { // 0x20
    /* 0x00 */ s32 fontID;
    /* 0x04 */ s32 menuID;
    /* 0x08 */ s32 modeID;
    /* 0x0c */ s32 plan;
    /* 0x10 */ s32 muteki;
    /* 0x14 */ s32 pad1;
    /* 0x18 */ s64 pad0;
} PROFILE;

typedef struct { // 0x10
    /* 0x0 */ s32 stat;
    /* 0x4 */ u16 hstart;
    /* 0x6 */ u16 hend;
    /* 0x8 */ s64 col;
} ZOSAN_DATA;

typedef struct { // 0x120
    /* 0x000 */ s32 scr_x;
    /* 0x004 */ s32 scr_y;
    /* 0x008 */ s32 fsz_x;
    /* 0x00c */ s32 fsz_y;
    /* 0x010 */ ZOSAN_DATA zd[16];
    /* 0x110 */ s32 z_id;
    /* 0x114 */ s32 pad0;
    /* 0x118 */ s32 pad1;
    /* 0x11c */ s32 pad2;
} PERSONAL_DEB;

typedef struct { // 0x10
    /* 0x0 */ u32 top;
    /* 0x4 */ u32 end;
    /* 0x8 */ s64 pad;
} DEBMEM;

typedef struct { // 0x10
    /* 0x0 */ SFXOBJ *pSfx;
    /* 0x4 */ SFXOBJ *pMir;
    /* 0x8 */ s32 pad1;
    /* 0xc */ s32 pad2;
} tOBJECT2;

typedef struct { // 0x10
    /* 0x0 */ u16 bp;
    /* 0x2 */ u32 bw;
    /* 0x3 */ u32 psm;
    /* 0x4 */ u32 pad0;
    /* 0x8 */ u16 x;
    /* 0xa */ u16 y;
    /* 0xc */ u16 w;
    /* 0xe */ u16 h;
} GIM_TAG;

typedef struct { // 0x10
    /* 0x0 */ u32 id;
    /* 0x4 */ u32 flag;
    /* 0x8 */ u32 data[2];
} TIM_DATA;

typedef struct { // 0xc
    /* 0x0 */ u32 bnum;
    /* 0x4 */ u16 dx;
    /* 0x6 */ u16 dy;
    /* 0x8 */ u16 w;
    /* 0xa */ u16 h;
} TIM_RECT;

typedef union { // 0x400
    /* 0x000 */ s32 *next;
    /* 0x000 */ char buf[1024];
} MEMBLOCK;

typedef struct { // 0x10
    /* 0x0 */ s16 dis;
    /* 0x2 */ s16 ydis;
    /* 0x4 */ s8 bake;
    /* 0x5 */ s8 nbake;
    /* 0x6 */ s8 look;
    /* 0x7 */ s8 hoko;
    /* 0x8 */ s16 turnint;
    /* 0xa */ s16 herootno;
    /* 0xc */ s32 heroot;
} ZAKO_BORN;

typedef struct { // 0x10
    /* 0x0 */ s32 walkspd;
    /* 0x4 */ s16 jumphigh;
    /* 0x6 */ s16 jumptime;
    /* 0x8 */ s16 mizo;
    /* 0xa */ s16 target0;
    /* 0xc */ s16 target1;
    /* 0xe */ s16 tachido;
} WALK_COMMON;

typedef struct { // 0x10
    /* 0x0 */ f32 len;
    /* 0x4 */ f32 yoff;
    /* 0x8 */ f32 ysize;
    /* 0xc */ f32 dummy;
} HITTBL2;

typedef struct { // 0x90
    /* 0x00 */ RT_WRK rtw;
    /* 0x10 */ sceVu0FVECTOR pos;
    /* 0x20 */ sceVu0FVECTOR rot;
    /* 0x30 */ sceVu0FVECTOR vec;
    /* 0x40 */ sceVu0FVECTOR bak;
    /* 0x50 */ sceVu0FVECTOR p0;
    /* 0x60 */ sceVu0FVECTOR p1;
    /* 0x70 */ tOBJECT2 *hOp;
    /* 0x74 */ s32 tekipic;
    /* 0x78 */ s32 pad1;
    /* 0x7c */ s32 pad2;
    /* 0x80 */ f32 pad3;
    /* 0x84 */ f32 pad4;
    /* 0x88 */ s32 pad5;
    /* 0x8c */ s32 pad6;
} ZM_WORK;

typedef struct { // 0x30
    /* 0x00 */ u8 l0;
    /* 0x01 */ u8 l1;
    /* 0x02 */ s16 rno;
    /* 0x04 */ f32 lip;
    /* 0x08 */ f32 ypos;
    /* 0x0c */ s32 mcn;
    /* 0x10 */ s32 type;
    /* 0x20 */ sceVu0FVECTOR pos;
} LT_WORK;

typedef struct { // 0xb0
    /* 0x00 */ sceVu0FVECTOR lpos;
    /* 0x10 */ sceVu0FMATRIX nlx;
    /* 0x50 */ sceVu0FMATRIX lcx;
    /* 0x90 */ s8 l0;
    /* 0x91 */ s8 l1;
    /* 0x92 */ s16 rno;
    /* 0x94 */ f32 lip;
    /* 0x98 */ f32 ypos;
    /* 0x9c */ s32 mcn;
    /* 0xa0 */ s32 type;
    /* 0xa4 */ s32 flag;
    /* 0xa8 */ s32 pad1;
    /* 0xac */ s32 pad2;
} ZLT_WORK;

typedef struct { // 0xb0
    /* 0x00 */ ZLT_WORK zltw;
} ZSUB_WORK;

typedef struct { // 0x20
    /* 0x00 */ sceVu0FVECTOR scale;
    /* 0x10 */ void *sfx;
    /* 0x14 */ f32 s_y;
    /* 0x18 */ s32 motno;
    /* 0x1c */ s32 pad0;
} DR_WORK;

typedef struct { // 0x20
    /* 0x00 */ s32 seno[4];
    /* 0x10 */ s32 setmp[4];
} SND_WORK;

typedef struct { // 0x90
    /* 0x00 */ s32 cnt;
    /* 0x04 */ s32 pad0;
    /* 0x08 */ s32 pad1;
    /* 0x0c */ s32 pad2;
    /* 0x10 */ DR_WORK drw[4];
} DR_OBJ;

typedef struct { // 0xc0
    /* 0x00 */ s16 xzdis;
    /* 0x02 */ s16 ydis;
    /* 0x04 */ u16 timer;
    /* 0x06 */ u16 count;
    /* 0x08 */ s16 flag;
    /* 0x0a */ s16 flag2;
    /* 0x0c */ s16 flag3;
    /* 0x0e */ s16 tekipic;
    /* 0x10 */ void *movetbl;
    /* 0x14 */ s16 look;
    /* 0x16 */ s16 bomtimer;
    /* 0x18 */ s32 motno;
    /* 0x1c */ OBJWORK *prt;
    /* 0x20 */ s32 seno[4];
    /* 0x30 */ s32 sedat[4];
    /* 0x40 */ OBJWORK *movebox;
    /* 0x44 */ f32 gspd;
    /* 0x48 */ OBJWORK *gmp;
    /* 0x4c */ s32 gimspd;
    /* 0x50 */ LT_WORK ltw;
    /* 0x80 */ f32 rt_ofs;
    /* 0x84 */ f32 rt_spd;
    /* 0x88 */ s64 jumpspd;
    /* 0x90 */ s16 res0;
    /* 0x92 */ s16 res1;
    /* 0x94 */ s32 test0;
    /* 0x98 */ s32 test1;
    /* 0xa0 */ s64 test2;
    /* 0xa8 */ s64 juryoku;
    /* 0xb0 */ s64 otoM;
} ZAKO_WORK;

typedef struct { // 0x100
	/* 0x00 */ s16 xzdis;
	/* 0x02 */ s16 ydis;
	/* 0x04 */ u16 timer;
	/* 0x06 */ u16 count;
	/* 0x08 */ s16 flag;
	/* 0x0a */ s16 flag2;
	/* 0x0c */ s16 flag3;
	/* 0x0e */ s16 tekipic;
	/* 0x10 */ void *movetbl;
	/* 0x14 */ s16 look;
	/* 0x16 */ s16 bomtimer;
	/* 0x18 */ s32 motno;
	/* 0x1c */ OBJWORK *prt;
	/* 0x20 */ s32 seno[4];
	/* 0x30 */ s32 sedat[4];
	/* 0x40 */ OBJWORK *movebox;
	/* 0x44 */ f32 gspd;
	/* 0x48 */ OBJWORK *gmp;
	/* 0x4c */ s32 gimspd;
	/* 0x50 */ LT_WORK ltw;
	/* 0x80 */ void *mp0;
	/* 0x84 */ void *mp1;
	/* 0x88 */ f32 tenoff;
	/* 0x8c */ f32 ang;
	/* 0x90 */ OBJWORK *bnd[4];
	/* 0xa0 */ sceVu0FVECTOR hitpos;
	/* 0xb0 */ sceVu0FVECTOR bakpos;
	/* 0xc0 */ sceVu0FVECTOR scale;
	/* 0xd0 */ sceVu0FVECTOR norm;
	/* 0xe0 */ f32 yold;
	/* 0xe4 */ s32 nageint;
	/* 0xe8 */ f32 nagespd;
	/* 0xec */ f32 g;
	/* 0xf0 */ s32 pad0;
	/* 0xf4 */ s32 pad1;
	/* 0xf8 */ s16 idx[4];
} ZAKO_WORK4;

typedef struct { // 0xe0
    /* 0x00 */ s16 xzdis;
    /* 0x02 */ s16 ydis;
    /* 0x04 */ u16 timer;
    /* 0x06 */ u16 count;
    /* 0x08 */ s16 flag;
    /* 0x0a */ s16 flag2;
    /* 0x0c */ s16 flag3;
    /* 0x0e */ s16 tekipic;
    /* 0x10 */ void *movetbl;
    /* 0x14 */ s16 look;
    /* 0x16 */ s16 bomtimer;
    /* 0x18 */ s32 motno;
    /* 0x1c */ OBJWORK *prt;
    /* 0x20 */ s32 seno[4];
    /* 0x30 */ s32 sedat[4];
    /* 0x40 */ OBJWORK *movebox;
    /* 0x44 */ f32 gspd;
    /* 0x48 */ OBJWORK *gmp;
    /* 0x4c */ s32 gimspd;
    /* 0x50 */ LT_WORK ltw;
    /* 0x80 */ void *mp0;
    /* 0x84 */ void *mp1;
    /* 0x88 */ s16 res0;
    /* 0x8a */ s16 res1;
    /* 0x8c */ f32 jumpspd;
    /* 0x90 */ s32 pad0;
    /* 0x94 */ f32 juryoku;
    /* 0x98 */ s64 otoM;
    /* 0xa0 */ s32 pad1;
    /* 0xa4 */ s16 plc;
    /* 0xa6 */ s16 rtn;
    /* 0xa8 */ s32 mcn;
    /* 0xac */ s32 wspd;
    /* 0xb0 */ s32 michinori;
    /* 0xb4 */ s32 furimuki;
    /* 0xb8 */ WALK_COMMON *walkpa;
    /* 0xbc */ s32 wsbuf;
    /* 0xc0 */ s32 test0;
    /* 0xc4 */ s32 test1;
    /* 0xc8 */ HITTBL2 hittbl2;
} ZAKO_COMMON;

typedef struct { // 0xd0
	/* 0x00 */ s16 xzdis;
	/* 0x02 */ s16 ydis;
	/* 0x04 */ u16 timer;
	/* 0x06 */ u16 count;
	/* 0x08 */ s16 flag;
	/* 0x0a */ s16 flag2;
	/* 0x0c */ s16 flag3;
	/* 0x0e */ s16 tekipic;
	/* 0x10 */ void *movetbl;
	/* 0x14 */ s16 look;
	/* 0x16 */ s16 bomtimer;
	/* 0x18 */ s32 motno;
	/* 0x1c */ OBJWORK *prt;
	/* 0x20 */ s32 seno[4];
	/* 0x30 */ s32 sedat[4];
	/* 0x40 */ OBJWORK *movebox;
	/* 0x44 */ f32 gspd;
	/* 0x48 */ OBJWORK *gmp;
	/* 0x4c */ s32 gimspd;
	/* 0x50 */ LT_WORK ltw;
	/* 0x80 */ s32 id;
	/* 0x84 */ s16 res0;
	/* 0x86 */ s16 res1;
	/* 0x88 */ f32 jumpspd;
	/* 0x8c */ s32 pad0;
	/* 0x90 */ f32 juryoku;
	/* 0x94 */ s32 pad1;
	/* 0x98 */ s32 test0;
	/* 0xa0 */ sceVu0FVECTOR tmp;
	/* 0xb0 */ sceVu0FVECTOR tmp1;
	/* 0xc0 */ void *gmw;
	/* 0xc4 */ s32 wspd;
	/* 0xc8 */ s32 michinori;
	/* 0xcc */ s32 pad;
} GIMM_COMMON;

typedef struct { // 0x10
    /* 0x0 */ s32 itemcnt;
    /* 0x4 */ s32 sisize;
    /* 0x8 */ s32 flagsize;
    /* 0xc */ s32 dispsize;
} ITEM_DATA_HEAD;

typedef struct { // 0x20
    /* 0x00 */ sceVu0FVECTOR pos;
    /* 0x10 */ sceVu0FVECTOR rot;
} SI_WORK;

#endif
