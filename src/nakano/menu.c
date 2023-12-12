#include "nakano/menu.h"
#include "nakano/dma.h"
#include "nakano/gssub.h"
#include "nakano/main.h"
#include "okanoyo/okprint.h"

extern f32 My_param[] __attribute__((section(".vutext")));
extern u32 My_dma_start0 __attribute__((section(".vutext")));

typedef struct { // 0x18
    /* 0x00 */ s32 mode;
    /* 0x04 */ s32 vision;
    /* 0x08 */ s32 flag;
    /* 0x0c */ s32 vscnt;
    /* 0x10 */ s32 option;
    /* 0x14 */ s32 opmode;
} nkMENU_WORK; // Technically this should have the same name as MENU_WORK, but whateverrrrr duuuude

static nkMENU_WORK MenuGbl;
static FMATRIX VLmtx;
static FVECTOR theta;
static FVECTOR trans;
static FVECTOR dtheta;

s32 (*MenuFuncTbl[2])() = {
    MenuInit,
    MenuMain
};

static void nkSetMatrix() {
    ((u32 *)&My_param)[0] = *(u32 *)&(VLmtx.m[0].x);
    ((u32 *)&My_param)[1] = *(u32 *)&(VLmtx.m[1].x);
    ((u32 *)&My_param)[2] = *(u32 *)&(VLmtx.m[2].x);
    ((u32 *)&My_param)[3] = *(u32 *)&(VLmtx.m[3].x);

    ((u32 *)&My_param)[4] = *(u32 *)&(VLmtx.m[0].y);
    ((u32 *)&My_param)[5] = *(u32 *)&(VLmtx.m[1].y);
    ((u32 *)&My_param)[6] = *(u32 *)&(VLmtx.m[2].y);
    ((u32 *)&My_param)[7] = *(u32 *)&(VLmtx.m[3].y);

    ((u32 *)&My_param)[8] = *(u32 *)&(VLmtx.m[0].z);
    ((u32 *)&My_param)[9] = *(u32 *)&(VLmtx.m[1].z);
    ((u32 *)&My_param)[10] = *(u32 *)&(VLmtx.m[2].z);
    ((u32 *)&My_param)[11] = *(u32 *)&(VLmtx.m[3].z);

    ((u32 *)&My_param)[12] = *(u32 *)&(VLmtx.m[0].w);
    ((u32 *)&My_param)[13] = *(u32 *)&(VLmtx.m[1].w);
    ((u32 *)&My_param)[14] = *(u32 *)&(VLmtx.m[2].w);
    ((u32 *)&My_param)[15] = *(u32 *)&(VLmtx.m[3].w);
}

static void nkRotMatrix(FVECTOR *rot, FVECTOR *trs, FMATRIX *vlm) {
    f32 c0;
    f32 c1;
    f32 c2;
    f32 s0;
    f32 s1;
    f32 s2;

    c0 = cosf(rot->x);
    c1 = cosf(rot->y);
    c2 = cosf(rot->z);
    s0 = sinf(rot->x);
    s1 = sinf(rot->y);
    s2 = sinf(rot->z);

    vlm->m[0].x = c1 * c2;
    vlm->m[0].y = s0 * s1 * c2 - s2 * c0;
    vlm->m[0].z = s1 * c0 * c2 + s0 * s2;
    vlm->m[0].w = trs->x;
    vlm->m[1].x = s2 * c1;
    vlm->m[1].y = s0 * s1 * s2 + c0 * c2;
    vlm->m[1].z = s1 * c0 * s2 - s0 * c2;
    vlm->m[1].w = trs->y;
    vlm->m[2].x = -s1;
    vlm->m[2].y = s0 * c1;
    vlm->m[2].z = c0 * c1;
    vlm->m[2].w = trs->z;
    vlm->m[3].x = 0.0f;
    vlm->m[3].y = 0.0f;
    vlm->m[3].z = 0.0f;
    vlm->m[3].w = 1.0f;
}

s32 MenuInit() {
    SysGbl.smode++;
    MenuGbl.mode = 0;
    MenuGbl.vision = 0;
    MenuGbl.flag = 0;
    MenuGbl.vscnt = 0;
    MenuGbl.option = 0;
    MenuGbl.opmode = 0;
    nkInitDma();
    sceDmaSend(DmaChVIF1, &My_dma_start0);
    sceGsSyncV(0);
    nkGsInitFrm();
    OkDefaultSysInit(SCR_WIDTH, SCR_HEIGHT, 12, 6);
    OkPrintSysInit(2, SCR_WIDTH, SCR_HEIGHT, 12, 6);
    theta.x = 0.0f;
    theta.y = 0.0f;
    theta.z = 0.0f;
    trans.x = 0.0f;
    trans.y = 0.0f;
    trans.z = 1000.0f;
    dtheta.x = M_PI / 90.0f;
    dtheta.y = -M_PIf / 180.0f;
    dtheta.z = M_PIf / 180.0f;
    return 0;
}

s32 MenuMain() {
    // Stubbed in retail build
    return 0;
}
