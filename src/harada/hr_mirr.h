#ifndef HR_MIRR_H
#define HR_MIRR_H

#include "harada.h"

typedef struct {
    vpmINFO infoA;
    vpmINFO infoB;
    u32 *addrA;
    u32 *addrB;
    s32 flag;
    s32 pad0;
} V1100MIR;

extern s32 hfmircnt;
extern s32 hcmircnt;
extern s32 hrmirflush;
extern s32 hroldflush;
// extern HFMIR hfmirbuf[4];
// extern HCMIR hcmirbuf[1];
// extern HFMIRC hfmcam;
extern u8 *hfm_addr;
extern u8 *hcm_addr;
extern u32 hfm_size;
extern u32 hcm_size;
extern f32 *hfm_scale;
extern f32 *hcm_scale;
extern V1100MIR *hrm1100;

#endif
