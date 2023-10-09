#ifndef H_GAMESND_H
#define H_GAMESND_H

#include "hoshino.h"

typedef struct { // 0x14
    /* 0x00 */ f32 GVol;
    /* 0x04 */ f32 GVolOld;
    /* 0x08 */ s32 UseList[3];
} SEGRP;

typedef struct { // 0x80
    /* 0x00 */ s32 ID;
    /* 0x04 */ s32 Time;
    /* 0x08 */ s32 Flag;
    /* 0x0c */ s32 Pri;
    /* 0x10 */ s32 GrpNo;
    /* 0x14 */ s32 Next;
    /* 0x18 */ s32 UseVoiceNum;
    /* 0x1c */ s32 Vnum[2];
    /* 0x24 */ f32 fadeCnt;
    /* 0x28 */ f32 OVol;
    /* 0x2c */ f32 DistVol;
    /* 0x30 */ f32 PanL;
    /* 0x34 */ f32 RearL;
    /* 0x38 */ f32 VolL;
    /* 0x3c */ f32 VolLbak;
    /* 0x40 */ f32 PanR;
    /* 0x44 */ f32 RearR;
    /* 0x48 */ f32 VolR;
    /* 0x4c */ f32 VolRbak;
    /* 0x50 */ u32 Pitch;
    /* 0x54 */ f32 *Pos;
    /* 0x58 */ f32 PosCopy[4];
    /* 0x68 */ s32 rearFade;
    /* 0x6c */ f32 rearCnt;
    /* 0x70 */ f32 camDist;
    /* 0x74 */ f32 camSpeed;
    /* 0x78 */ f32 camAng;
    /* 0x7c */ s32 camRear;
} SEOBJ;

typedef struct { // 0x10
    /* 0x0 */ s32 Stat;
    /* 0x4 */ s32 Next;
    /* 0x8 */ s32 GrpNo;
    /* 0xc */ s32 ObjNo;
} SEVOICE;

typedef struct { // 0x30
    /* 0x00 */ s64 splt;
    /* 0x08 */ f32 posx;
    /* 0x0c */ f32 posy;
    /* 0x10 */ f32 posz;
    /* 0x14 */ f32 vol;
    /* 0x18 */ s32 pitch;
    /* 0x1c */ f32 workf0;
    /* 0x20 */ f32 workf1;
    /* 0x24 */ s32 worki0;
    /* 0x28 */ s32 worki1;
} ENVSE;

extern void hSeLock(s32 i);
extern void hSeInitObj();
extern s32  hSeObjErrChk(s32 obj);
extern s32  hSeGetObj();
extern void hSeReleaseObj(s32 obj);
extern void hSeInitGrp(s32 stage);
extern s32  hSeGetVoice(s32 grp, s32 uselv);
extern void hSeReleaseVoice(s32 vnum);
extern void hSeObjMain();
extern void hSePitch(s32 obj, s32 pitch);
extern f32  hSePanFunc(f32 t);
extern void hSePan(s32 obj);
extern void hSeDist(int obj);
extern s32  hSeKeyOn(s64 splt, f32 *parent, s32 reserve);
extern s32  hSeKeyOnEv(s32 prog, s32 splt, f32 *pos, s32 flag);
extern void hSeKeyOffAll();
extern void hSeKeyOffAll2();
extern void hSeKeyOff(s32 obj);
extern void hSeKeyOffMain(s32 obj);
extern void hSeSetMVol(f32 vol);
extern void hSeSetGrpVol(s32 grp, f32 vol);
extern void hSeSetObjVol(s32 obj, f32 vol);
extern void hSeSetObjPan(s32 obj, f32 panl, f32 panr);
extern void hSeSetObjdB(s32 obj, f32 db);
extern s32  hSeEnvKeyOn(s32 i);
extern void hSeEnvStart();
extern void hSeEnvChangeVol(s32 obj, f32 vol, s32 frame);
extern void hSeEnvSetVol(s32 obj, f32 vol);

#endif
