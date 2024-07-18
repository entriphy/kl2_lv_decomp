#include "hr_tchr.h"
#include "nakano/dma.h"
#include "nakano/main.h"
#include "hr_mapdr.h"

static u32 *hrgms_old;
extern u32 hr_vu1c_mp __attribute__((section(".vudata")));

void hr_decode_tkVU1(HRVU1OBJ *hrobj, u32 *buff, u32 *gms) {
    u32 top;
    u32 *dmatag;
    s32 i;
    HR_SFX_HEADER *head;
    TYPE_PARTS_TBLK *tblk;

    head = (HR_SFX_HEADER *)buff;
    hrobj->fix_num = 0;
    hrobj->skin_num = 0;
    hrobj->scale = head->scale;
    hrobj->gms = gms;
    top = (u32)buff;
    if (head->id != 0x5856) {
        return;
    }

    if (gms != NULL) {
        hrobj->qwc = *(u16 *)gms;
        gms[0] = gms[1] = 0;
    }

    for (i = 0, tblk = (TYPE_PARTS_TBLK *)buff + 1; i < ((u16 *)buff)[1]; i++, tblk++) { // head->parts_num
        if (tblk->type == 'A' || tblk->type == 'C') {
            tblk->dmatag = (u32 *)(top + (u32)tblk->dmatag);
            dmatag = tblk->dmatag;
            if (hrobj->skin_num == 0) {
                hrobj->dma_skin = (qword *)dmatag;
            }
            dmatag++;
            *dmatag += top;
            hrobj->skin_num++;
        } else {
            tblk->jblock_adrs = (HR_JOINT_BLK *)(top + (u32)tblk->jblock_adrs);
            dmatag = (u32 *)tblk->jblock_adrs;
            if (hrobj->fix_num == 0) {
                hrobj->dma_fix = (qword *)dmatag;
            }
            dmatag++;
            *dmatag += top;
            hrobj->fix_num++;
        }
    }
}

static void hr_draw_skin(HRVU1OBJ *hrobj, s32 maxmat, sceVu0FMATRIX *LsMat, sceVu0FMATRIX *LcLight) {
    qword *qw;
    u16 i;
    s32 j;
    ATR_SFXVU1D *pp;

    for (j = 0; j < maxmat; j++) {
        nkMakeUnpackVif1(&p1_packet, LsMat, j * 4, 4);
        nkMakeUnpackVif1(&p1_packet, LcLight, j * 4 + 0x100, 4);
        LsMat++;
        LcLight++;
    }

    pp = (ATR_SFXVU1D *)p1_packet;
    pp->dma[0] = 0x10000000 | (sizeof(ATR_SFXVU1D) / sizeof(qword) - 1);
    pp->dma[1] = pp->dma[2] = pp->dma[3] = 0;
    pp->vif[0] = pp->vif[1] = pp->vif[2] = 0;
    pp->vif[3] = SCE_VIF1_SET_DIRECT(6, 0);
    ((u64 *)pp->gif)[0] = SCE_GIF_SET_TAG(5, 1, 0, 0, 0, 1);
    pp->gif[2] = SCE_GIF_PACKED_AD;
    pp->gif[3] = 0;
    ((u64 *)pp->tex1)[0] = SCE_GS_SET_TEX1_1(0, 0, 1, 1, 0, 0, 0);
    ((u64 *)pp->tex1)[1] = SCE_GS_TEX1_1;
    ((u64 *)pp->clamp)[0] = SCE_GS_SET_CLAMP_1(0, 0, 0, 0, 0, 0);
    ((u64 *)pp->clamp)[1] = SCE_GS_CLAMP_1;
    ((u64 *)pp->test)[0] = SCE_GS_SET_TEST_1(1, 1, 0, 0, 0, 0, 1, 2);
    ((u64 *)pp->test)[1] = SCE_GS_TEST_1;
    ((u64 *)pp->alpha)[0] = SCE_GS_SET_ALPHA_1(0, 1, 2, 1, 127);
    ((u64 *)pp->alpha)[1] = SCE_GS_ALPHA_1;
    ((u64 *)pp->fba)[0] = SCE_GS_SET_FBA_1(1);
    ((u64 *)pp->fba)[1] = SCE_GS_FBA_1;

    FlushCache(0);
    pp++;
    p1_packet = (qword *)pp;
    for (i = 0, qw = hrobj->dma_skin; i < hrobj->skin_num; i++, qw++) {
        nkAddRef(qw);
    }
    FlushCache(0);
}

void hr_draw_tkVU1(HRVU1OBJ *hrobj, s32 maxmat, sceVu0FMATRIX *LsMat, sceVu0FMATRIX *LcLight, sceVu0FMATRIX *LColor) {
    ATR_SFXVU1 *pp;

    nkMakeUnpackVif1(&p1_packet, *LColor, 0x384, 4);
    if (hr_objtype != 1) {
        hr_objtype = 1;
        hrgms_old = NULL;
        nkAddRef(&hr_vu1c_mp);
    }

    if (hrobj->gms != NULL && hrobj->gms != hrgms_old) {
        (*p1_packet)[0] = 0x30000000 | hrobj->qwc;
        (*p1_packet)[1] = (u32)hrobj->gms;
        (*p1_packet)[2] = (*p1_packet)[3] = 0;
        p1_packet++;
    }

    pp = (ATR_SFXVU1 *)p1_packet;
    pp->dma0[0] = 0x10000000 | (sizeof(ATR_SFXVU1) / sizeof(qword) - 1);
    pp->dma0[1] = pp->dma0[2] = pp->dma0[3] = 0;
    pp->vifw[0] = pp->vifw[1] = pp->vifw[2] = 0;
    pp->vifw[3] = SCE_VIF1_SET_FLUSH(0);
    hrSetObjAttrUnpack(pp->vif0, 0x3AD, 2);
    pp->scale[0] = hrobj->scale;
    pp->scale[1] = 1.0f / 255.0f;
    pp->scale[2] = pp->scale[3] = 0.0f;
    pp->tmpcol[0] = pp->tmpcol[1] = pp->tmpcol[2] = 255.0f;
    pp->tmpcol[3] = 2048.0f;
    hrSetObjAttrUnpack(pp->vifr, 0x39B, 2);
    pp->pabe0 = SCE_GS_SET_PABE(0);
    pp->pabe2 = SCE_GS_PABE;
    pp->texa0 = SCE_GS_SET_TEXA(64, 0, 64);
    pp->texa2 = SCE_GS_TEXA;
    hrSetObjAttrUnpack(pp->vifg, 0x397, 1);
    pp->gt0 = SCE_GIF_SET_TAG(3, 0, 0, 0, 0, 1);
    pp->gt2 = SCE_GIF_PACKED_AD;
    pp->vif1[0] = 0x1000404;
    pp->vif1[1] = 0x3000000;
    pp->vif1[2] = 0x20001C0;
    pp->vif1[3] = SCE_VIF1_SET_MSCAL(6, 0); // idk man

    FlushCache(0);
    pp++;
    p1_packet = (qword *)pp;
    if (hrobj->skin_num != 0) {
        hr_draw_skin(hrobj, maxmat, LsMat, LcLight);
    }
}