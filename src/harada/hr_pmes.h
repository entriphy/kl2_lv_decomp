#ifndef HR_PMES_H
#define HR_PMES_H

#include "harada.h"

typedef enum {
    HRPMJ_NOPRINT,
    HRPMJ_WAITSTART,
    HRPMJ_WAITSTP,
    HRPMJ_NUM0 = 3,
    HRPMJ_PRINT = 3,
    HRPMJ_WAITBOT,
    HRPMJ_CHGPAGE,
    HRPMJ_WAITALPHA,
    HRPMJ_NUM1,
} HRPMJ;

typedef enum {
    MES_END = -1,
    MES_PAGE = -2,
    MES_GYOU = -3,
    MES_ITEM = -4,
    MES_YN = -5,
    MES_TBOX = -6,
    MES_CLUT = -7,
    MES_SPAC = -8,
    MES_KEY = -9,
    MES_WAIT = -10,
    MES_TIME = -11,
    MES_SPD = -12,
    MES_VCST = -13,
    MES_DSPD = -14,
    MES_VCEND = -15,
    MES_VCTIME = -16
} HRMES;

typedef struct { // 0xd0
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword gif0;
    /* 0x20 */ qword test;
    /* 0x30 */ qword alpha;
    /* 0x40 */ qword xyofs;
    /* 0x50 */ qword tex0;
    /* 0x60 */ qword gif1;
    /* 0x70 */ qword rgb0;
    /* 0x80 */ qword uv0;
    /* 0x90 */ qword xyz0;
    /* 0xa0 */ qword rgb1;
    /* 0xb0 */ qword uv1;
    /* 0xc0 */ qword xyz1;
} ATR_MWAKU;

typedef struct { // 0x60
    /* 0x00 */ qword uv0;
    /* 0x10 */ qword rgb0;
    /* 0x20 */ qword xyz0;
    /* 0x30 */ qword uv1;
    /* 0x40 */ qword rgb1;
    /* 0x50 */ qword xyz1;
} ATR_MMOJI;

typedef struct { // 0x60
    /* 0x00 */ qword dmatag;
    /* 0x10 */ qword gif0;
    /* 0x20 */ qword tex0;
    /* 0x30 */ qword test;
    /* 0x40 */ qword alpha;
    /* 0x50 */ qword gif1;
} ATR_MMOJIH;

typedef struct { // 0x90
    /* 0x00 */ qword gif0;
    /* 0x10 */ qword tex0;
    /* 0x20 */ qword gif1;
    /* 0x30 */ qword rgb0;
    /* 0x40 */ qword uv0;
    /* 0x50 */ qword xyz0;
    /* 0x60 */ qword rgb1;
    /* 0x70 */ qword uv1;
    /* 0x80 */ qword xyz1;
} ATR_MKAS;

extern void hr_pmes_ns_st(HRPMOJI *pmes);
extern void hr_pmes_ns_ai(HRPMOJI *pmes);
extern void hr_pmes_init(HRPMOJI *pmes);
extern void hr_pmes_exit(HRPMOJI *pmes);
extern void hr_pmes_set(HRPMOJI *pmes, s32 *addr);
extern void hr_pmes_start(HRPMOJI *pmes);
extern void hr_pmes_reta(HRPMOJI *pmes);
extern void hr_pmes_window(HRPMOJI *pmes, s32 onoff);
extern void hr_pmes_waitstp(HRPMOJI *pmes);
extern void hr_pmes_work(HRPMOJI *pmes);
extern void hr_pmes_draw(HRPMOJI *pmes);
extern void hr_mesp_draw(HRMESP *mesp, HRPMOJI *pmes);

#endif
