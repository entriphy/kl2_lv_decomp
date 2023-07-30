#ifndef HR_MIRR_H
#define HR_MIRR_H

#include "harada.h"
#include "h_vpm2.h"

typedef struct {
    vpmINFO infoA;
    vpmINFO infoB;
    u32 *addrA;
    u32 *addrB;
    s32 flag;
    s32 pad0;
} V1100MIR;

typedef struct { // 0x14
    /* 0x00 */ u32 addr;
    /* 0x04 */ u32 count;
    /* 0x08 */ f32 rx;
    /* 0x0c */ f32 ry;
    /* 0x10 */ f32 len;
} K2M_HEADF;

typedef struct { // 0x10
    /* 0x0 */ u32 addr;
    /* 0x4 */ u32 count;
    /* 0x8 */ u32 poly;
    /* 0xc */ u32 addr2;
} K2M_HEADC;

// symbol: K2M_HC2:t381=382=s12rx:12,0,32;ry:12,32,32;len:12,64,32;;
typedef struct { // 0xc
    /* 0x0 */ f32 rx;
    /* 0x4 */ f32 ry;
    /* 0x8 */ f32 len;
} K2M_HC2;

typedef struct { // 0x24
    /* 0x00 */ sceVu0FVECTOR *norm;
    /* 0x04 */ sceVu0FVECTOR *center;
    /* 0x08 */ sceVu0FVECTOR *vert;
    /* 0x0c */ f32 rx;
    /* 0x10 */ f32 ry;
    /* 0x14 */ f32 cx;
    /* 0x18 */ f32 cy;
    /* 0x1c */ f32 clen;
    /* 0x20 */ u32 count;
} HFMIR;

typedef struct { // 0x2d0
    /* 0x000 */ sceVu0FVECTOR mpos;
    /* 0x010 */ sceVu0FVECTOR mang;
    /* 0x020 */ sceVu0FMATRIX wsm;
    /* 0x060 */ sceVu0FMATRIX wvm;
    /* 0x0a0 */ sceVu0FMATRIX vsm;
    /* 0x0e0 */ sceVu0FVECTOR smax;
    /* 0x0f0 */ sceVu0FVECTOR smin;
    /* 0x100 */ sceVu0FVECTOR vn;
    /* 0x110 */ sceVu0FVECTOR cent;
    /* 0x120 */ sceVu0FVECTOR dlt;
    /* 0x130 */ VPCLIP vc;
    /* 0x1e0 */ VPCLIP vco;
    /* 0x290 */ u64 xyofs;
    /* 0x298 */ f32 dx;
    /* 0x29c */ f32 dy;
    /* 0x2a0 */ u32 posmode;
    /* 0x2a4 */ void *mir;
    /* 0x2a8 */ u32 type;
    /* 0x2ac */ f32 rx;
    /* 0x2b0 */ f32 ry;
    /* 0x2b4 */ f32 scalex;
    /* 0x2b8 */ f32 scaley;
    /* 0x2bc */ s32 count;
    /* 0x2c0 */ f32 vsmSx;
    /* 0x2c4 */ f32 vsmSy;
    /* 0x2c8 */ s32 vsmFg;
    /* 0x2cc */ s32 flag;
} HFMIRC;

typedef struct { // 0x30
    /* 0x00 */ sceVu0FVECTOR *norm;
    /* 0x04 */ sceVu0FVECTOR *center;
    /* 0x08 */ sceVu0FVECTOR *vert;
    /* 0x0c */ s16 *dst;
    /* 0x10 */ K2M_HC2 *hc2;
    /* 0x14 */ f32 rx;
    /* 0x18 */ f32 ry;
    /* 0x1c */ f32 cx;
    /* 0x20 */ f32 cy;
    /* 0x24 */ u32 poly;
    /* 0x28 */ u32 count;
    /* 0x2c */ u32 men;
} HCMIR;

extern s32 hfmircnt;
extern s32 hcmircnt;
extern s32 hrmirflush;
extern s32 hroldflush;
// extern HFMIR hfmirbuf[4];
// extern HCMIR hcmirbuf[1];
extern HFMIRC hfmcam;
extern u8 *hfm_addr;
extern u8 *hcm_addr;
extern u32 hfm_size;
extern u32 hcm_size;
extern f32 *hfm_scale;
extern f32 *hcm_scale;
extern V1100MIR *hrm1100;

#endif
