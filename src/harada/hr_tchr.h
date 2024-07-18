#ifndef HR_TCHR_H
#define HR_TCHR_H

#include "harada.h"

typedef struct { // 0x20
    /* 0x00 */ u16 joint_id[4];
    /* 0x08 */ u16 vert_n;
    /* 0x0a */ u16 norm_n;
    /* 0x0c */ u16 vert_vif;
    /* 0x0e */ u16 norm_vif;
    /* 0x10 */ u32 *vert_ofs;
    /* 0x14 */ u32 *v_wt_adrs;
    /* 0x18 */ u32 *norm_ofs;
    /* 0x1c */ u32 *n_wt_adrs;
} HR_JOINT_BLK;

typedef struct { // 0x10
    /* 0x0 */ u16 id;
    /* 0x2 */ u16 parts_num;
    /* 0x4 */ u16 vif_pnt_fix;
    /* 0x6 */ u16 vif_pnt_skin;
    /* 0x8 */ f32 scale;
    /* 0xc */ u32 pad;
} HR_SFX_HEADER;

typedef struct { // 0x10
    /* 0x0 */ u16 type;
    /* 0x2 */ u16 texture_id;
    /* 0x4 */ u16 jblock_num;
    /* 0x6 */ u16 pad0;
    /* 0x8 */ HR_JOINT_BLK *jblock_adrs;
    /* 0xc */ u32 *dmatag;
} TYPE_PARTS_TBLK;

typedef struct { // 0x20
    /* 0x00 */ u16 skin_num;
    /* 0x02 */ u16 fix_num;
    /* 0x04 */ qword *dma_skin;
    /* 0x08 */ qword *dma_fix;
    /* 0x0c */ f32 scale;
    /* 0x10 */ u32 *gms;
    /* 0x14 */ u32 qwc;
    /* 0x18 */ s32 pad0;
    /* 0x1c */ s32 pad1;
} HRVU1OBJ;

typedef struct { // 0xb0
    /* 0x00 */ qword dma0;
    /* 0x10 */ qword vifw;
    /* 0x20 */ qword vif0;
    /* 0x30 */ f32 scale[4];
    /* 0x40 */ f32 tmpcol[4];
    /* 0x50 */ qword vifr;
    /* 0x60 */ u64 pabe0;
    /* 0x68 */ u64 pabe2;
    /* 0x70 */ u64 texa0;
    /* 0x78 */ u64 texa2;
    /* 0x80 */ qword vifg;
    /* 0x90 */ u64 gt0;
    /* 0x98 */ u64 gt2;
    /* 0xa0 */ qword vif1;
} ATR_SFXVU1;

typedef struct { // 0x80
    /* 0x00 */ qword dma;
    /* 0x10 */ qword vif;
    /* 0x20 */ qword gif;
    /* 0x30 */ qword tex1;
    /* 0x40 */ qword clamp;
    /* 0x50 */ qword test;
    /* 0x60 */ qword alpha;
    /* 0x70 */ qword fba;
} ATR_SFXVU1D;

extern void hr_decode_tkVU1(HRVU1OBJ *hrobj, u32 *buff, u32 *gms);
extern void hr_draw_tkVU1(HRVU1OBJ *hrobj, s32 maxmat, sceVu0FMATRIX *LsMat, sceVu0FMATRIX *LcLight, sceVu0FMATRIX *LColor);

#endif
