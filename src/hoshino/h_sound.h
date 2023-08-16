#ifndef H_SOUND_H
#define H_SOUND_H

#include "hoshino.h"

typedef enum {
    SNDCMD_KEYON = 0,
    SNDCMD_KEYOFF = 1,
    SNDCMD_KEYOFFALL = 2,
    SNDCMD_PITCH = 3,
    SNDCMD_PALPITCH = 4,
    SNDCMD_VOL = 5,
    SNDCMD_MVOL = 6,
    SNDCMD_MVOLALL = 7,
    SNDCMD_EFFECT = 8,
    SNDCMD_EVOL = 9
} SNDCMD;

typedef enum {
    SND_MODE_MONAURAL,
    SND_MODE_STEREO,
    SND_MODE_5P1CH,
    SND_MODE_NUM,
} SND_MODE;

typedef struct {
    SND_MODE Stereo;
    s32 iopBankAddr;
    s32 stageBank;
    s16 stageTblNum;
    s16 stageTbl[1024];
    u8 *Pk;
    s32 PkNum;
    s32 PkMax;
    s32 fadeFlag;
    f32 fadeCnt;
    f32 fadeMax;
    s32 ObjList;
    s32 VoiceStat[2];
    s32 KeyonV[2];
    s32 Mute;
    s32 MVol;
    f32 log10Volume;
    f32 dBfader;
    f32 bgmMVol;
    f32 seMVol;
    s32 seObjID;
    s32 seLock;
    f32 effVol;
    f32 effVolBak;
    s32 effIdx;
    s32 effChange;
    s32 effMode;
    s32 effDepth;
    s32 effDelay;
    s32 effFeed;
    s32 effMix;
    f32 pptMVol;
    f32 pptFadeCnt;
    s32 pptFade;
    s32 envNum;
    s32 envObj[16];
    s32 envFade[16];
    s32 envFadeCnt[16];
    s32 envFadeMax[16];
    f32 envFadeVol0[16];
    f32 envFadeVol1[16];
    s32 TitleDelayCnt;
} hSNDDATA;

typedef struct {
    s32 efx;
    f32 vol;
    s32 delay;
    s32 feed;
    s32 dry;
    f32 vol_ppt;
    f32 workf0;
    s32 worki0;
    s32 worki1;
} EFXSE;

extern s32  JamGetHdSize(s32 hdaddr);
extern s32  JamGetBdSize(s32 hdaddr);
extern void hSndPkEffect();
extern void hSndPkSetMVol(s32 voll, s32 volr);
extern void hSndPkSetEVol(s32 vol);
extern void hSndPkSetVol(s32 voice, f32 vollf, f32 volrf);
extern void hSndPkSetPitch(s32 voice, s32 pitch);
extern void hSndPkSetPalPitch(s32 voice);
extern void hSndPkKeyOn(s32 voice, s32 flag, s32 bank, s32 prog, s32 splt, f32 vollf, f32 volrf);
extern void hSndPkKeyOff(s32 voice);
extern void hSndPkKeyOffAll();
extern s32  hSndPkGetSize();
extern void hSndReset();
extern void hSndFadeOutAll(s32 frame);
extern void hSndFadeInAll(s32 frame);
extern void hSndSetMVol(f32 vol);
extern s32  hSndFader(f32 vol);
extern f32  hSndFader2(f32 vol);
extern void hSndMain();
extern void hSndInit();
extern void hSndBankSet(s32 packaddr, s32 id);
extern void hSndBankSetMain();
extern void hSndBankSetStage();
extern void hSndEffSetArea();
extern void hSndEffSetVolIdx(s32 idx);
extern void hSndEffSetVol_PPTstart();
extern void hSndEffSetVol_PPTend();
extern void hSndEffSetVol(f32 vol);
extern void hSndSetStereo(SND_MODE i);

extern u8 SndPacket[1024];
extern u8 SndTempBuff[1048576];
extern hSNDDATA *sD;

#endif
