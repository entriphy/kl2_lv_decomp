#include "nakano/objklo2.h"
#include "nakano/vu0mac.h"
#include "nakano/klmot.h"
#include "nakano/main.h"
#include "harada/hr_pall.h"
#include "hoshino/h_gamesnd.h"

void DrawKloBhd(OBJWORK *objw) {
    if (hr_pt_check() != 1) {
        nkSetKlonoaShadow(objw);
        DrawSfxKlonoa_BH(objw);
    }
}

static void sbbHero_Todomari_Sub(OBJWORK *objw, HERO_WORK *herow) {
    sceVu0FVECTOR hitposi;
    sceVu0FVECTOR hitmuki;
    s32 pata0 = 0;
    s32 pata1 = 0;
    f32 pus = 0.0f;
    f32 pds = 0.0f;

    if (objw->bun1 == 3 || objw->bun1 == 4) {
        pata0 = 20;
        pata1 = 35;
        pus = 0.8360887f;
        pds = 0.48764083f;
    }

    if (objw->reg0 & 1) {
        objw->bun0 = 15;
    } else {
        objw->bun0 = 20;
    }

    if (herow->lvl & GameGbl.k_jump_button && herow->count < pata1) {
        if (herow->count >= pata0) {
            objw->spd[1] -= pus;
        } else {
            objw->spd[1] -= pds;
        }
        hitmuki[0] = hitmuki[2] = 0.0f;
        hitmuki[1] = -0.9992676f;
        hitposi[0] = objw->posi[0] + objw->muki[0] * 8.75f;
        hitposi[1] = objw->posi[1] - 28.0f;
        hitposi[2] = objw->posi[2] + objw->muki[2] * 8.75f;
        hitposi[0] = objw->posi[0] - objw->muki[0] * 8.75f;
        hitposi[1] = objw->posi[1] - objw->muki[1] * 8.75f;
    } else if (objw->bun0 != 15) {
        objw->bun0 = 2;
    }
}

void sbbHero_Jump_Sub(OBJWORK *objw, HERO_WORK *herow) {
    objw->spd[1] += 0.5110887f;
    if (objw->spd[1] > herow->jumpspd_limit) {
        objw->spd[1] = herow->jumpspd_limit;
    }

    if (objw->bun0 == 20 || objw->bun0 == 21) {
        sbbHero_Todomari_Sub(objw, herow);
    } else if (!(objw->spd[1] < -1.75f)) {
        herow->spflag &= -0x11;
    }
}

void nkKloKsslwt(OBJWORK *objw) {
    s32 obj = -1;
    if (SysGbl.irqc % 3 == 0) {
        switch (GameGbl.vision >> 8) {
            case 5:
                obj = hSeKeyOn(0x1900640ea0a202, objw->posi, 0);
                break;
            case 10:
                obj = hSeKeyOn(0x1900640ea0a201, objw->posi, 0);
                break;
            case 14:
                obj = hSeKeyOn(0x1900640ea0a200, objw->posi, 0);
                break;
            default:
                obj = hSeKeyOn(0x1900640ea0a201, objw->posi, 0);
                break;
            case 1:
                break;
        }
    }

    if (obj != -1) {
        sceVu0FVECTOR vf;
        f32 var_f0, vol;

        vu0_Square(vf, objw->spd);
        var_f0 = vu0_SquareRoot(vf[0] + vf[2]) * 50.0f;
        vol = 0.6f + (var_f0 - 360.0f) / 220.0f * 0.4f;
        hSeSetObjVol(obj, vol);
    }
}

void _nkSbDrawTTriangle(sceVu0FVECTOR vf0, sceVu0FVECTOR vf1, sceVu0FVECTOR vf2, sceVu0FVECTOR posi, sceVu0FVECTOR rot, sceVu0IVECTOR rgba) {
    sceVu0FMATRIX lmat;
    sceVu0IVECTOR vi0;
    sceVu0IVECTOR vi1;
    sceVu0IVECTOR vi2;
    qword packet_buff[8];
    nkGifPacket pack;
    nkGifPacket *pp = &pack;

    sceVu0UnitMatrix(lmat);
    sceVu0RotMatrixX(lmat, lmat, rot[0]);
    sceVu0RotMatrixY(lmat, lmat, rot[1]);
    sceVu0RotMatrixZ(lmat, lmat, rot[2]);
    sceVu0TransMatrix(lmat, lmat, posi);
    sceVu0MulMatrix(lmat, GameGbl.wsm, lmat);
    sceVu0RotTransPers(vi0 ,lmat, vf0, 1);
    sceVu0RotTransPers(vi1, lmat, vf1, 1);
    sceVu0RotTransPers(vi2, lmat, vf2, 1);
    sceVu0ITOF4Vector(vf0, vi0);
    sceVu0ITOF4Vector(vf1, vi1);
    sceVu0ITOF4Vector(vf2, vi2);

    if (!sceVu0ClipScreen3(vf0, vf1, vf2)) {
        pp->size = 0;
        pp->buf = (nkQWdata *)packet_buff;

        pp->buf[pp->size].ul128 = 0;
        pp->buf[pp->size++].ul32[0] = 0x70000000 | 7;
        pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(3, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRI, 1, 0, 0, 1, 0, 0, 1, 0), 0, 2);
        pp->buf[pp->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 4;

        vi0[2] = 0xFFFF;
        vi1[2] = 0xFFFF;
        vi2[2] = 0xFFFF;

        pp->buf[2].ul32[0] = rgba[0];
        pp->buf[2].ul32[1] = rgba[1];
        pp->buf[2].ul32[2] = rgba[2];
        pp->buf[2].ul32[3] = rgba[3];
        pp->buf[3].ul32[0] = vi0[0];
        pp->buf[3].ul32[1] = vi0[1];
        pp->buf[3].ul32[2] = vi0[2];
        pp->buf[3].ul32[3] = vi0[3];

        pp->buf[4].ul32[0] = rgba[0];
        pp->buf[4].ul32[1] = rgba[1];
        pp->buf[4].ul32[2] = rgba[2];
        pp->buf[4].ul32[3] = rgba[3];
        pp->buf[5].ul32[0] = vi1[0];
        pp->buf[5].ul32[1] = vi1[1];
        pp->buf[5].ul32[2] = vi1[2];
        pp->buf[5].ul32[3] = vi1[3];

        pp->buf[6].ul32[0] = rgba[0];
        pp->buf[6].ul32[1] = rgba[1];
        pp->buf[6].ul32[2] = rgba[2];
        pp->buf[6].ul32[3] = rgba[3];
        pp->buf[7].ul32[0] = vi2[0];
        pp->buf[7].ul32[1] = vi2[1];
        pp->buf[7].ul32[2] = vi2[2];
        pp->buf[7].ul32[3] = vi2[3];

        FlushCache(0);
        sceDmaSend(DmaChGIF, pp->buf);
        sceDmaSync(DmaChGIF, 0, 0);
    }
}