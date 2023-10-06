#ifndef HR_VPA_H
#define HR_VPA_H

#include "harada.h"

typedef struct { // 0x1c
    /* 0x00 */ VPOINFO vpoi;
    /* 0x10 */ u32 n;
    /* 0x14 */ u32 *anm;
    /* 0x18 */ u32 ver;
} VPAINFO;

typedef struct { // 0x68
    /* 0x00 */ u32 flag;
    /* 0x04 */ s32 draw;
    /* 0x08 */ s32 time;
    /* 0x0c */ qword *dmatag;
    /* 0x10 */ u32 *code;
    /* 0x14 */ u32 npos;
    /* 0x18 */ qword *pos;
    /* 0x1c */ u32 *blk;
    /* 0x20 */ f32 scalex;
    /* 0x24 */ f32 scaley;
    /* 0x28 */ f32 scalez;
    /* 0x2c */ f32 scsx;
    /* 0x30 */ f32 scsy;
    /* 0x34 */ f32 scsz;
    /* 0x38 */ f32 vcolr;
    /* 0x3c */ f32 vcolg;
    /* 0x40 */ f32 vcolb;
    /* 0x44 */ f32 vcsr;
    /* 0x48 */ f32 vcsg;
    /* 0x4c */ f32 vcsb;
    /* 0x50 */ s32 rotx;
    /* 0x54 */ s32 rotsx;
    /* 0x58 */ s32 rotax;
    /* 0x5c */ s32 roty;
    /* 0x60 */ s32 rotsy;
    /* 0x64 */ s32 rotay;
} ANMINFO;

typedef struct { // 0x40
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword vcol;
    /* 0x20 */ qword scale;
    /* 0x30 */ qword mpg;
} ATR_VPAVS;

enum {
    VPA_END = 0,
    VPA_JMP = 1,
    VPA_PAT = 2,
    VPA_TIME = 3,
    VPA_DRAW = 4,
    VPA_VCL = 5,
    VPA_VCLR = 6,
    VPA_VCLG = 7,
    VPA_VCLB = 8,
    VPA_SCL = 9,
    VPA_SCLX = 10,
    VPA_SCLY = 11,
    VPA_SCLZ = 12,
    VPA_SCLS = 13,
    VPA_SCLSX = 14,
    VPA_SCLSY = 15,
    VPA_SCLSZ = 16,
    VPA_ROT = 17,
    VPA_ROTX = 18,
    VPA_ROTY = 19,
    VPA_ROTS = 20,
    VPA_ROTSX = 21,
    VPA_ROTSY = 22,
    VPA_ROTA = 23,
    VPA_ROTAX = 24,
    VPA_ROTAY = 25,
    VPA_CAMY = 26,
    VPA_CMDNUM = 27
};

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 jmp;
} HVPA_JMP;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 patno;
} HVPA_PAT;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 wait;
} HVPA_TIME;

typedef struct { // 0x10
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 red;
    /* 0x8 */ f32 green;
    /* 0xc */ f32 blue;
} HVPA_VCL;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 red;
} HVPA_VCLR;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 green;
} HVPA_VCLG;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 blue;
} HVPA_VCLB;

typedef struct { // 0x10
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 sclx;
    /* 0x8 */ f32 scly;
    /* 0xc */ f32 sclz;
} HVPA_SCL;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 sclx;
} HVPA_SCLX;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 scly;
} HVPA_SCLY;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 sclz;
} HVPA_SCLZ;

typedef struct { // 0x10
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 sclsx;
    /* 0x8 */ f32 sclsy;
    /* 0xc */ f32 sclsz;
} HVPA_SCLS;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 sclsx;
} HVPA_SCLSX;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 sclsy;
} HVPA_SCLSY;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ f32 sclsz;
} HVPA_SCLSZ;

typedef struct { // 0xc
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 rx;
    /* 0x8 */ s32 ry;
} HVPA_ROT;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 rx;
} HVPA_ROTX;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 ry;
} HVPA_ROTY;

typedef struct { // 0xc
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 srx;
    /* 0x8 */ s32 sry;
} HVPA_ROTS;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 srx;
} HVPA_ROTSX;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 sry;
} HVPA_ROTSY;

typedef struct { // 0xc
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 arx;
    /* 0x8 */ s32 ary;
} HVPA_ROTA;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 arx;
} HVPA_ROTAX;

typedef struct { // 0x8
    /* 0x0 */ s32 code;
    /* 0x4 */ s32 ary;
} HVPA_ROTAY;

extern VPAINFO infovpa;
extern ANMINFO *hri_anm;
extern ANMINFO *hri_bg;

extern void DecodeVpo2(u32 *vpo_top, VPOINFO *vpoi);
extern void DecodeVpo2Tag(u32 *vpo_top);
extern void DecodeVpo2S(u32 *vpo_top, VPOINFO *vpoi);
extern void hr_anmVPA_init_M(ANMINFO *anm, u32 *top, u32 *addr, qword *tag);
extern void hr_anmVPA_init_M1(ANMINFO *anm, u32 *top, u32 *addr, qword *tag);
extern void hr_anmVPA_init();
extern void hr_anmVPA_initBG();
extern void hr_anmVPA_draw();
extern void hr_anmVPA_work_M(ANMINFO *anm);
extern void hr_anmVPA_work();
extern void hr_anmVPA_workBG();

#endif
