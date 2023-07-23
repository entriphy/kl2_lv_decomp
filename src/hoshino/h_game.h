#ifndef H_GAME_H
#define H_GAME_H

#include "hoshino.h"
#include "h_str.h"

typedef enum {
    DEAD_DAMAGE,
    DEAD_FALL,
    DEAD_DROWN,
    DEAD_CHOKE,
    DEAD_BURN
} DEAD;

typedef struct { // 0x10
    /* 0x0 */ s32 bgmNo;
    /* 0x4 */ s32 bgmCh;
    /* 0x8 */ f32 fadeIn;
    /* 0xc */ f32 fadeOut;
} BGMAREA;

typedef enum {
    BGMMODE_TOP,
    BGMMODE_START,
    BGMMODE_GAME,
    BGMMODE_END,
} BGMMODE;

typedef struct {
    BGMMODE BGMmode;
    s32 BGMppt;
    s32 BGMpptcnt;
    s32 BGMid;
    s32 BGMchgreq;
    s32 BGMchgid;
    s32 BGMchgmode;
    s32 nextVision;
    s32 areaTime;
    s32 resFlag;
    s32 resBGM;
    s32 resVision;
    f32 resEVol;
    s32 deadFlag;
    s32 deadCnt;
} hGAMEDATA;

extern BGMAREA *BGMvision[50][24];
extern hGAMEDATA GameData;
extern hGAMEDATA *gD;

extern void hInitGame();
extern void hExitGame();
extern void hInitStage0();
extern void hInitStage1();
extern void hExitStage();
extern s32  hGameAreaStart(s32 flag);
extern s32  hGameAreaEnd(s32 next);
extern void hGameMain();
extern void hPushRestart();
extern void hPopRestart();
extern void hKlonoaDead(s32 vision, s32 frame, DEAD pattern);
extern s32  hGameBgmGetID();
extern BGM  hGameBgmGetNo();
extern s32  hGameBgmGetCh();
extern void hGameBgmChange();
extern s32  hGameBgmComp(s32 v0, s32 id0, s32 v1, s32 id1);

#endif
