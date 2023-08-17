#ifndef HR_NAK_H
#define HR_NAK_H

#include "harada.h"

extern void hr_nak_init_allobj();
extern void hr_nak_work_allobj();
extern void hr_nak_draw_allobj();
extern void hr_nak_draw_effobj();
extern void hr_nak_dummyVK();
extern void ptcat_camera();
extern void ptrel_camera(f32 tm);
extern RT_WRK* hr_get_klort();
extern void hr_open_klokey(s32 fg);
extern void hr_set_klokey(s32 trg, s32 lvl);
extern s32 hr_pkey_change(s32 trg);
extern void hr_pklo_center(f32 time);
extern void hr_pklo_jump(f32 spd, s32 flag);

#endif
