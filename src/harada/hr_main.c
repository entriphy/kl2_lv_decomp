#include "common.h"

MAPVWORK mapvw = {};
HRSCRST hrmapst = {};
HRSCRST hrbgst = {};
u32 *hrd_pack = NULL;
u32 *hrg_pack = NULL;
u32 *hrf_pack = NULL;
u32 *hrse_pack = NULL;
s32 hr_abeoff = 0;
s32 hr_objtype = 0;
u32 *hr_mt_addr = NULL;
vpmINFO hr_mtexi = {};
s32 hr_resKeep = 0;
s32 hr_resTmp = 0;
u32 *hr_obcdata = NULL;
u32 *hr_obcvpf = NULL;
VPOINFO *hr_obcvpo = NULL;

void hr_cold_start() {
    hr_pflag_init();
    hr_pt_fclear();
    hrpt_deb = 0;
    hr_abeoff = 0;
    hrpt_sbgm = 0;
}
