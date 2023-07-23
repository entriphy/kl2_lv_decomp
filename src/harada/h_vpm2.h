#ifndef H_VPM2_H
#define H_VPM2_H

#include "harada.h"

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

extern VPCLIP cvpm;
extern VPCLIP cvbg;
extern VPCLIP cvpo;

extern void h_vp_init(VPCLIP *vpc, f32 proj, f32 near, f32 far, f32 hamix, f32 hamiy, f32 hamiz, s32 fg);
extern s32  hm_vpclip(sceVu0FMATRIX rtm, sceVu0FVECTOR zure, sceVu0FVECTOR cmax, sceVu0FVECTOR cmin, sceVu0FVECTOR hmax, sceVu0FVECTOR hmin, sceVu0FVECTOR *out);
extern s32  h_vpm_bclip(VPCLIP *vpc, s32 *bhead, sceVu0FMATRIX world_view);
extern s32  h_vpo_vclip(VPCLIP *vpc, sceVu0FVECTOR pos, sceVu0FMATRIX world_view);

#endif
