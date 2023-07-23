#ifndef HR_ANMVP_H
#define HR_ANMVP_H

#include "harada.h"

extern u32  hr_float2int(f32 f);
extern void hr_anmVPM_allinit();
extern void hr_anmVPM_set(HRANMVS *avs);
extern void hr_set_anmVPMtbl(HRANMVS *avs);
extern void hr_anmVPM_work();
extern void hr_set_vlight(HRAVL *vlight, f32 x, f32 y, f32 z, f32 r, f32 g, f32 b, f32 n, f32 f);
extern void hr_set_vlightMini(HRAVL *vlight, f32 r, f32 g, f32 b);

#endif
