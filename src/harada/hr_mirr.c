#include "common.h"

s32 hfmircnt = 0;
s32 hcmircnt = 0;
s32 hrmirflush = 0;
s32 hroldflush = 0;
// HFMIR hfmirbuf[4] = {};
// HCMIR hcmirbuf[1] = {};
// HFMIRC hfmcam = {};
u8 *hfm_addr = NULL;
u8 *hcm_addr = NULL;
u32 hfm_size = 0;
u32 hcm_size = 0;
f32 *hfm_scale = NULL;
f32 *hcm_scale = NULL;
V1100MIR *hrm1100 = NULL;
