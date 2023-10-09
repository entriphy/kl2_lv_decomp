#include "harada/hr_pbgm.h"
#include "harada/hr_pall.h"
#include "harada/hr_prm.h"
#include "hoshino/h_str.h"

static PTBGM ptbgm_table[82] = {}; // TODO
static PTBGM *ptb;
s32 hrpt_id = 0;
s16 hrptm_res = 0;
s32 hrpt_sbgm = 0;

s32 hr_ptchk_45(s32 id) {
    s32 var;

    var = id & 2 ? HRPSFG_CHK040B : HRPSFG_CHK0505 ;
    return pt_flagj_P(var);
}

void hr_pt_setBGM(HR_PSYS *ps, s16 bgm, s32 mode) {
    ps->reqbgm = bgm;
    if (mode == 0) {
        ps->flag |= 0x20000000;
    } else {
        ps->flag |= 0x10000000;
    }
}

void hr_pt_reqBGM(HR_PSYS *ps) {
    s32 mode;

    mode = ps->flag & 0x20000000 ? 0 : 2;
    ps->flag &= -0x30000001;
    hBgmChangeID(ps->reqbgm, mode);
}

void hr_ptbgm_init(HR_PSYS *ps) {
    s32 id;

    ps->reqbgm = -1;
    if (hrpt_id != -1 && hrpt_flag != 4) {
        ptb = &ptbgm_table[hrpt_id];
        if (ptb->flag & 1) {
            hr_pt_setBGM(ps, ptb->sbgm, 0);
        }
        return;
    }
}

void hr_ptbgm_end(HR_PSYS *ps, s32 mode) {
    s32 id;

    if (hrpt_id == -1 || hrpt_flag == 4) {
        return;
    }

    ptb = &ptbgm_table[hrpt_id];
    if (ptb->flag & 0xC) {
        id = ptb->flag & 8;
        hBgmChangeID(-1, id ? 0 : 2);
    } else if (ptb->flag & 0x10) {
        if (ps->reqbgm == -1 || ps->reqbgm != ptb->ebgm) {
            hBgmChangeID(ptb->ebgm, mode);
        }
    }
}