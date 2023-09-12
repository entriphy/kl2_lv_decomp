#include "h_vpm2.h"

VPCLIP cvpm = {};
VPCLIP cvbg = {};
VPCLIP cvpo = {};

void h_vp_init(VPCLIP *vpc, f32 proj, f32 near, f32 far, f32 hamix, f32 hamiy, f32 hamiz, s32 fg) {
    vpc->cmax[0] = 320.0f / proj;
    vpc->cmax[1] = 240.0f / proj;
    vpc->cmax[2] = 0.0f;
    vpc->cmax[3] = 0.0f;
    vpc->cmin[0] = -vpc->cmax[0];
    vpc->cmin[1] = -vpc->cmax[1];
    vpc->cmin[2] = 0.0f;
    vpc->cmin[3] = 0.0f;

    if (!fg) {
        hamix = hamiy = hamiz = sqrtf(hamix * hamix + hamiy * hamiy + hamiz * hamiz);;
        if (proj < 320.0f) {
            hamix += ((320.0f - proj) * hamix) / proj + 724.0f;
            if (proj < 240.0f) {
                hamiy += ((240.0f - proj) * hamiy) / proj + 724.0f;
            }
        }
        if (GameGbl.vision == 0x1101) {
            hamix *= 2;
            hamiy *= 2;
            hamiz *= 1.5f;
        }
    }

    vpc->hmax[0] = hamix;
    vpc->hmax[1] = hamiy;
    vpc->hmax[2] = far + hamiz;
    vpc->hmax[3] = 0.0f;
    vpc->hmin[0] = -hamix;
    vpc->hmin[1] = -hamiy;
    vpc->hmin[2] = near - hamiz;
    vpc->hmin[3] = 0.0f;
}

s32 hm_vpclip(sceVu0FMATRIX rtm, sceVu0FVECTOR zure, sceVu0FVECTOR cmax, sceVu0FVECTOR cmin, sceVu0FVECTOR hmax, sceVu0FVECTOR hmin, sceVu0FVECTOR *out) {
    asm __volatile__(
        "lqc2        $vf4, 0x0(%0)\n"
        "lqc2        $vf5, 0x10(%0)\n"
        "lqc2        $vf6, 0x20(%0)\n"
        "lqc2        $vf7, 0x30(%0)\n"
        "lqc2        $vf8, 0x0(%1)\n"
        "lqc2        $vf9, 0x0(%2)\n"
        "lqc2        $vf10, 0x0(%3)\n"
        "lqc2        $vf11, 0x0(%4)\n"
        "lqc2        $vf12, 0x0(%5)\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf13, $vf7, $vf8w\n"
        "vnop\n"
        "vnop\n"
        "vadda.xy    ACC, $vf0, $vf11\n"
        "vmaddz.xy   $vf11, $vf9, $vf13z\n"
        "vadda.xy    ACC, $vf0, $vf12\n"
        "vmaddz.xy   $vf12, $vf10, $vf13z\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "ctc2        $0, $vi16\n"
        "vsub.xyz    $vf20, $vf11, $vf13\n"
        "vsub.xyz    $vf21, $vf13, $vf12\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "sqc2        $vf20, 0x0(%6)\n"
        "sqc2        $vf21, 0x10(%6)\n"
        "vnop\n"
        "vnop\n"
        "vnop\n"
        "cfc2        $2, $vi16\n"
        "andi        $2, $2, 0xC0\n"
    : : "r" (rtm), "r" (zure), "r" (cmax), "r" (cmin), "r" (hmax), "r" (hmin), "r" (out));
}

s32 h_vpm_bclip(VPCLIP *vpc, s32 *bhead, sceVu0FMATRIX world_view) {
    sceVu0FVECTOR v;

    bhead += 2;
    v[0] = ((f32 *)bhead)[0];
    v[1] = ((f32 *)bhead)[1];
    v[2] = ((f32 *)bhead)[2];
    v[3] = 1.0f;
    return hm_vpclip(world_view, v, vpc->cmax, vpc->cmin, vpc->hmax, vpc->hmin, &vpc->zmax);
}

s32 h_vpo_vclip(VPCLIP *vpc, sceVu0FVECTOR pos, sceVu0FMATRIX world_view) {
    sceVu0FVECTOR v;

    v[0] = pos[0];
    v[1] = pos[1];
    v[2] = pos[2];
    v[3] = 1.0f;
    return hm_vpclip(world_view, v, vpc->cmax, vpc->cmin, vpc->hmax, vpc->hmin, &vpc->zmax);
}
