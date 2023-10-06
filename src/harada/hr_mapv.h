#ifndef HR_MAPV_H
#define HR_MAPV_H

#include "harada.h"
#include "harada/hr_mirr.h"

typedef struct { // 0xf0
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword viftag;
    /* 0x20 */ qword gif0;
    /* 0x30 */ qword stq0;
    /* 0x40 */ qword rgb0;
    /* 0x50 */ qword xyz0;
    /* 0x60 */ qword stq1;
    /* 0x70 */ qword rgb1;
    /* 0x80 */ qword xyz1;
    /* 0x90 */ qword stq2;
    /* 0xa0 */ qword rgb2;
    /* 0xb0 */ qword xyz2;
    /* 0xc0 */ qword stq3;
    /* 0xd0 */ qword rgb3;
    /* 0xe0 */ qword xyz3;
} ATR_HRCL;

typedef struct { // 0x8
    /* 0x0 */ HR_CALL *ca;
    /* 0x4 */ int no;
} HPVSEL;

typedef struct { // 0x28
    /* 0x00 */ char name[16];
    /* 0x10 */ u64 tag0;
    /* 0x18 */ u64 tag2;
    /* 0x20 */ qword *mpg;
} MAPDM;

typedef struct { // 0x18
    /* 0x00 */ qword *gif;
    /* 0x04 */ u16 dpsm;
    /* 0x06 */ u16 rrw;
    /* 0x08 */ u16 rrh;
    /* 0x0a */ u16 dbp;
    /* 0x0c */ u16 dsax;
    /* 0x0e */ u16 dsay;
    /* 0x10 */ u16 dbw;
    /* 0x12 */ u16 dum0;
    /* 0x14 */ u32 *clut;
} HRGMSH;

typedef struct { // 0x3c24
    /* 0x0000 */ s32 mv_clutc;
    /* 0x0004 */ HRGMSH *mv_clutd;
    /* 0x0008 */ s32 mv_clutn;
    /* 0x000c */ s32 mvc_rrw;
    /* 0x0010 */ s32 mvc_rrh;
    /* 0x0014 */ s32 mvc_rgba;
    /* 0x0018 */ s32 mv_pixc;
    /* 0x001c */ HRGMSH *mv_pixd;
    /* 0x0020 */ s32 mv_pixn;
    /* 0x0024 */ HRGMSH clut[320];
    /* 0x1e24 */ HRGMSH pix[320];
} HR_CLUTS;

typedef struct { // 0x7a00
    /* 0x0000 */ s32 ob_no;
    /* 0x0004 */ u32 ob_nblock;
    /* 0x0008 */ qword *ob_blockhead;
    /* 0x0010 */ MAPDM mdmbuf[4];
    /* 0x00b0 */ MAPDM *mdmptr;
    /* 0x00b4 */ s32 mdm;
    /* 0x00c0 */ sceVu0FVECTOR fogcpos;
    /* 0x00d0 */ s32 lookmode;
    /* 0x00d4 */ f32 cposy;
    /* 0x00d8 */ f32 crotx;
    /* 0x00dc */ s32 mvm_bg;
    /* 0x00e0 */ s32 mvm_bgfg;
    /* 0x00e4 */ s32 mvm_bgno;
    /* 0x00e8 */ s32 mvm_mtex;
    /* 0x00ec */ s32 mir_fg;
    /* 0x00f0 */ HCMIR *keepcmir;
    /* 0x00f4 */ s32 avtcnt;
    /* 0x00f8 */ s32 avtV;
    /* 0x00fc */ s32 avtmenu;
    /* 0x0100 */ HRANMV avts;
    /* 0x0140 */ HRANMVS *avt_tbl;
    /* 0x0150 */ HRANMV pushavbuf[1];
    /* 0x0190 */ s32 pushavcnt;
    /* 0x0194 */ HR_CLUTS clsb;
    /* 0x3db8 */ s32 cltb_fg;
    /* 0x3dbc */ HR_CLUTS cls;
    /* 0x79e0 */ s32 clt_fg;
    /* 0x79e4 */ s32 maps[4];
    /* 0x79f4 */ s32 stflag;
} HR_MAPVS;

typedef struct { // 0x90
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword t00;
    /* 0x20 */ sceVu0FVECTOR fixednum;
    /* 0x30 */ qword t01;
    /* 0x40 */ sceVu0FVECTOR clipnum;
    /* 0x50 */ qword t02;
    /* 0x60 */ qword giftag_tmp;
    /* 0x70 */ qword dblbuf;
    /* 0x80 */ qword mscal;
} ATR_VU1M_IV;

typedef struct { // 0xb0
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword viftag;
    /* 0x20 */ qword gif0;
    /* 0x30 */ qword tex0;
    /* 0x40 */ qword gif1;
    /* 0x50 */ qword st0;
    /* 0x60 */ qword rgb0;
    /* 0x70 */ qword xyz0;
    /* 0x80 */ qword st1;
    /* 0x90 */ qword rgb1;
    /* 0xa0 */ qword xyz1;
} HRVV;

extern void hr_avw_sint(HRANMV *av);
extern void hr_avw_sint2(HRANMV *av);
extern void hr_crossline(sceVu0FVECTOR v, u32 wh, u32 r, u32 g, u32 b, u32 fg);
extern void hrMapFileRead();
extern void hr_okmem_reset();
extern void hrGetMapBuf();
extern void hrCamMonitor(CAM_WORK *cam);
extern void hrSetVu1MemImage();
extern void hrDrawMap_VO();
extern s32 hrPtInit();
extern s32 hrPtMain();


#endif
