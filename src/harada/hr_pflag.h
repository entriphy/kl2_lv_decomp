#ifndef HR_PFLAG_H
#define HR_PFLAG_H

#include "harada.h"

typedef struct {
    s32 id;
    s32 ver;
    s32 pad0;
    s32 pad1;
    s32 ptkeep[4];
} HRMEMC;

extern void hr_pflag_initAr();
extern void hr_pflag_initSt();
extern void hr_pflag_init();
extern void hr_pflag_EndCopy(s32 *moto, s32 *saki);
extern s32 hr_pflag_sam(HRMEMC *buff);
extern void hr_pflag_memcLoad(HRMEMC *buff);
extern void hr_pflag_memcSave(HRMEMC *buff);
extern void hr_pflag_memcClear(HRMEMC *buff);
extern void hr_pflag_on(s32 id, s32 onoff);
extern void hr_pflag_theatar(s32 id, s32 mode);
extern s32 hr_pflag_checkSV(s32 id);
extern s32 hr_pflag_check(s32 id);
extern s32 hr_pflag_get_id();
extern s16 hr_pflag_get_vs(s32 id);
extern void hr_pflag_onS();
extern s32 hr_ASFGcheck(s32 id);
extern void hr_pflag_AreaStart(s32 mode);

#endif
