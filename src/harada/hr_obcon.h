#ifndef HR_OBCON_H
#define HR_OBCON_H

#include "harada.h"

typedef struct { // 0x10
    /* 0x0 */ s32 no;
    /* 0x4 */ s32 min;
    /* 0x8 */ s32 max;
    /* 0xc */ s32 pad0;
} HR_RTC;

typedef struct { // 0x30
    /* 0x00 */ f32 px;
    /* 0x04 */ f32 py;
    /* 0x08 */ f32 pz;
    /* 0x0c */ f32 rx;
    /* 0x10 */ f32 ry;
    /* 0x14 */ f32 rz;
    /* 0x18 */ s32 fhmno;
    /* 0x1c */ s32 stat1;
    /* 0x20 */ s32 rtc;
    /* 0x24 */ HR_RTC *rtcaddr;
    /* 0x28 */ s32 on;
    /* 0x2c */ s16 extra;
    /* 0x2e */ s16 res;
} HR_OBJ;

typedef struct { // 0x30
    /* 0x00 */ f32 px;
    /* 0x04 */ f32 py;
    /* 0x08 */ f32 pz;
    /* 0x0c */ f32 rx;
    /* 0x10 */ f32 ry;
    /* 0x14 */ f32 rz;
    /* 0x18 */ s16 fhmno;
    /* 0x1a */ s16 motno;
    /* 0x1c */ s16 flag;
    /* 0x1e */ s16 reg;
    /* 0x20 */ u64 vag;
    /* 0x28 */ s32 pad0;
    /* 0x2c */ s32 pad1;
} HR_HOBJ;

typedef struct { // 0xe0
    /* 0x00 */ sceVu0FMATRIX m;
    /* 0x40 */ sceVu0FMATRIX pnl;
    /* 0x80 */ sceVu0FMATRIX plc;
    /* 0xc0 */ VPOINFO *info;
    /* 0xc4 */ HR_OBJ *obj;
    /* 0xc8 */ HR_HOBJ *hh;
    /* 0xcc */ void *hobj;
    /* 0xd0 */ s32 objno;
    /* 0xd4 */ s32 bun0;
    /* 0xd8 */ s32 drawno;
    /* 0xdc */ s32 count;
} HR_OBJW;

typedef enum {
    HR_MAPEXPL,
    HR_SWITCHTRG,
    HR_GPUPPET,
    HR_PTEFFECT,
    HR_GPTMINI,
    HR_DPUPPET,
    HR_OBCEX00,
    HR_DUMMYOBC,
    HR_OBJTYPENUM,
} HR_OBCT;

typedef enum {
    DMAPEXPL_NON,
    DMAPEXPL_VPO,
    DMAPEXPL_VPOMOT,
    DMAPEXPL_VPOS,
    DMAPEXPL_MOT,
    DMAPEXPL_NUM,
} DMAPEXPL;

typedef enum {
    MAPEXPL_FLAG,
    MAPEXPL_MOT,
    MAPEXPL_AFTER,
    MAPEXPL_ERASE,
    MAPEXPL_AFTM,
} MAPEXPL;

extern s32 hr_route_clipM(HR_RTC *hrtc, s32 count, s32 no, s32 mc);
extern OBJWORK* hr_get_obcon(s16 stat1);
extern void hr_comm_drawVPO(sceVu0FMATRIX m, VPOINFO *info, s32 objno);
extern s32* hr_init_obconM(s32 id, s32 *tbl);
extern void hr_init_obcon();
extern void hr_work_obcon(OBJWORK *objw);
extern void hrLoadObcData(char *stname);

#endif
