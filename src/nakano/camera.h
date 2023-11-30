#ifndef CAMERA_H
#define CAMERA_H

#include "nakano.h"

typedef struct { // 0x20
    /* 0x00 */ sceVu0FVECTOR posi;
    /* 0x10 */ sceVu0FVECTOR ang;
} VIEWPOSI;

typedef struct { // 0x1c
    /* 0x00 */ u32 Ctbl;
    /* 0x04 */ s16 Prio;
    /* 0x06 */ s16 Zoom;
    /* 0x08 */ s16 AngX;
    /* 0x0a */ s16 AngY;
    /* 0x0c */ s16 AngZ;
    /* 0x0e */ s16 PosX;
    /* 0x10 */ s16 PosY;
    /* 0x12 */ s16 Time;
    /* 0x14 */ s16 Mode;
    /* 0x18 */ VIEWPOSI *ViewPosi;
} VCONTBL;

typedef struct { // 0x10
    /* 0x0 */ s32 rtcnt;
    /* 0x4 */ s32 height;
    /* 0x8 */ s32 dummy;
    /* 0xc */ u8 flg;
    /* 0xd */ u8 type;
    /* 0xe */ s16 idx;
} nkCamMNG;

typedef struct { // 0x20
    /* 0x00 */ s32 zoom;
    /* 0x04 */ s32 angx;
    /* 0x08 */ s32 angy;
    /* 0x0c */ s32 angz;
    /* 0x10 */ s32 trnsx;
    /* 0x14 */ s32 trnsy;
    /* 0x18 */ s32 proj;
    /* 0x1c */ s32 flg;
} nkCamNOR;

typedef struct { // 0x24
    /* 0x00 */ s32 posix;
    /* 0x04 */ s32 posiy;
    /* 0x08 */ s32 posiz;
    /* 0x0c */ s32 angx;
    /* 0x10 */ s32 angy;
    /* 0x14 */ s32 angz;
    /* 0x18 */ s32 proj;
    /* 0x1c */ s32 time;
    /* 0x20 */ s32 flg;
} nkCamFIX;

typedef struct { // 0x90
    /* 0x00 */ f32 zoom;
    /* 0x04 */ f32 angx;
    /* 0x08 */ f32 angy;
    /* 0x0c */ f32 angz;
    /* 0x10 */ f32 wangx;
    /* 0x14 */ f32 wangy;
    /* 0x18 */ f32 posix;
    /* 0x1c */ f32 posiy;
    /* 0x20 */ sceVu0FVECTOR adiv;
    /* 0x30 */ f32 dtime;
    /* 0x34 */ f32 dcnt;
    /* 0x38 */ s32 mode;
    /* 0x3c */ s32 release;
    /* 0x40 */ f32 frtime;
    /* 0x44 */ f32 fwtime;
    /* 0x48 */ f32 offx;
    /* 0x4c */ f32 offy;
    /* 0x50 */ sceVu0FVECTOR tposi;
    /* 0x60 */ u32 tblcnt;
    /* 0x64 */ u32 tblc_b;
    /* 0x68 */ s32 old_type;
    /* 0x6c */ s32 old_idx;
    /* 0x70 */ s32 old_rtn;
    /* 0x74 */ VCONTBL *tbl;
    /* 0x78 */ nkCamNOR *nml;
    /* 0x7c */ u32 flg;
    /* 0x80 */ u32 flg2;
    /* 0x84 */ s32 rtlen;
} VWork;

extern void CamDataNone();
extern void MngDataInit(s32 *adrs, nkCamMNG *mng_p[], nkCamNOR *nmr_p[], nkCamFIX *fix_p[]);
extern void CamDataInit(s32 *adrs);
extern void nkCeVtAdrInit(VCONTBL *vtp, s32 rtn);
extern void CamInit(CAM_WORK *cam);
extern void CamCalMatrix(CAM_WORK *cam);
extern void CamDebug(CAM_WORK *cam);
extern nkCamMNG* CamGetMNG(OBJWORK *objw);
extern void CamInitNml(nkCamNOR *nml, OBJWORK *objw, CAM_WORK *cam);
extern void CamReverseInit();
extern void CamVWorkInit(OBJWORK *objw, CAM_WORK *cam);
extern void CamPipe(OBJWORK *objw, CAM_WORK *cam);
extern void CamFollow(OBJWORK *objw, CAM_WORK *cam);
extern void CamFix(CAM_WORK *cam, CAM_WORK *tcam);
extern void CamDivTime(CAM_WORK *cam, CAM_WORK *tcam, f32 *divtime);
extern void CamMonOut(OBJWORK *objw, CAM_WORK *cam);
extern void CamTblCopy(VCONTBL *vt);
extern void CamNmlDiv(VWork *vwrk, nkCamNOR *nml);
extern void CamCtrlTbl(OBJWORK *objw);
extern void SetBossCamTbl(s32 tbln);
extern void CamBossTbl(OBJWORK *objw);
extern void CamGetPrm(sceVu0FVECTOR posi, sceVu0FVECTOR ang, sceVu0FVECTOR div);
extern void CamExtCtrl(sceVu0FVECTOR posi, sceVu0FVECTOR ang, sceVu0FVECTOR div);
extern void CamSetFlwFlag(s32 flg);
extern void CamRelease(f32 div);
extern void CamInitThr(OBJWORK *objw);
extern void CamPadCtrl(OBJWORK *objw, CAM_WORK *cam);
extern void _CamChangeOku();
extern void _CamChangeTemae();
extern void CamControl();



#endif
