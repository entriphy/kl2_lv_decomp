#include "harada/hr_pall.h"
#include "harada/hr_pbgm.h"
#include "harada/hr_pflag.h"
#include "harada/hr_main.h"
#include "harada/hr_obcon2.h"
#include "harada/hr_prm.h"
#include "harada/hr_pcam.h"
#include "harada/hr_pmes.h"
#include "harada/hr_pefc.h"
#include "hoshino/h_util.h"
#include "hoshino/h_str.h"
#include "okanoyo/okio.h"
#include "hoshino/h_game.h"

PCAMS ppcam = {};
PCAMS *hrpcam = NULL;
HRPMOJI ppmes = {};
HRPMWAKU ppwaku = {};
s32 ptflag_buff[4] = {};
s32 ptflag_st[4] = {};
s32 ptflag_ar[4] = {};
s32 ptflag_th[4] = {};
sceVu0FMATRIX hrpum = {};
sceVu0FMATRIX hrpm = {};
sceVu0FVECTOR hrvi = {};
sceVu0FVECTOR hrvo = {};
s32 *hrpt_addr = NULL;
s32 *hrptm_addr = NULL;
s16 hrpt_flag = 0;
s16 hrpt_scene = 0;
s16 hrpt_view = 0;
s16 hrpt_th = 0;
s16 hrpt_vt = 0;
s16 hrpt_deb = 0;
PCAMSC *hrpcc = NULL;
u32 *hrpt_pack = NULL;
u32 *hrpt_gms = NULL; // ?
s16 hrpt_patch = 0;
s16 hrpt_patcnt = 0;
s16 hrpt_stnk = 0;
HR_PSYS ppsys = {};
HR_CALL *hrpt_call = NULL;
s32 hrpt_mini = 0;
s32 *hrpt_mini_addr = NULL;

f32 hr_pall_atan2(f32 y, f32 x) {
    if (x == 0.0f) {
        if (y >= 0.0f) {
            return M_PI_2f;
        } else {
            return -M_PI_2f;
        }
    } else {
        return atan2f(y, x);
    }
}

f32 hr_pall_rotpai(f32 r) {
    if (r > M_PIf) {
        while (r > M_PIf) {
            r -= M_TWOPIf;
        }
    } else if (r < -M_PIf) {
        while (r < -M_PIf) {
            r += M_TWOPIf;
        }
    }
    return r;
}

void hr_pall_rotpaiV(sceVu0FVECTOR v) {
    s32 i;

    for (i = 0; i < 3; i++) {
        v[i] = hr_pall_rotpai(v[i]);
    }
}

f32 hr_pall_rot180(f32 r) {
    if (r > 180.0f) {
        while (r > 180.0f) {
            r -= 360.0f;
        }
    } else if (r < -180.0f) {
        while (r < -180.0f) {
            r += 360.0f;
        }
    }
    return r;
}

void hr_pall_rot180V(sceVu0FVECTOR v) {
    s32 i;

    for (i = 0; i < 3; i++) {
        v[i] = hr_pall_rot180(v[i]);
    }
}

void hr_pall_limitV(sceVu0FVECTOR v, f32 max, f32 min) {
    f32 *f;
    s32 i;

    for (i = 0, f = v; i < 3; i++, f++) {
        if (*f > max) {
            *f = max;
        } else if (*f < min) {
            *f = min;
        }
    }
}

void hr_pt_set(s16 flag, s16 scene, s16 view, s16 th) {
    hrpt_flag = flag;
    hrpt_scene = scene;
    hrpt_view = view;
    hrpt_th = th;

    if (flag != 0) {
        if (hrpt_flag == 4)
            hrpt_id = -1;
        else
            hrpt_id = hr_pflag_get_id();
    }
}

void hr_pt_fclear() {
    hr_pt_set(0, 0, 0, 0);
    hrpt_vt = 0;
    hrpt_addr = (s32 *)-1;
}

s32 hr_pt_check() {
    return (hrpt_flag & 0x1B) != 0;
}

s32 hr_ptmir_check() {
    return ppsys.mircnt;
}

void hr_ptmir_draw() {
    hr_ptmir_drawMain(&ppsys);
}

void hrPtDataLoad() {
    char FileName[128];
    char FontName[128];
    char Name[128];
    char *hex[16] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
    s32 i;

    if (hrpt_flag == 0) {
        return;
    }

    strcpy(FileName, "host0:./dat/st");
    hr_stage_no(FileName, 1);
    strcpy(FontName, FileName);
    strcat(FileName, "/v");
    hr_stage_no(FileName, 0);
    strcat(FileName, "s");
    strcat(FileName, hex[hrpt_scene & 0xF]);
    strcpy(Name, FileName);
    strcat(FileName, ".bin");
    strcat(Name, ".fhm");
    strcat(FontName, "/f");
    hr_stage_no(FontName, 0);
    strcat(FontName, "s");
    strcat(FontName, hex[hrpt_scene & 0xF]);
    strcat(FontName, ".gms");
    hrpt_addr = (s32 *)getBuff(0, 0, FileName, &i);
    if ((s32)hrpt_addr == -1) {
        hrpt_flag = 0;
    }
    hrptm_addr = (s32 *)getBuff(0, 0, Name, &i);
    strcpy(Name, "host0:./dat/common/pfntus.gms");
    ppmes.font = (u32 *)getBuff(0, 0, Name, &i);
    strcpy(Name, "host0:./dat/common/waku11.gms");
    ppwaku.gms = (u32 *)getBuff(0, 0, Name, &i);
}

void hrGameInPtDataLoad(s32 sw) {
    // Empty function
}

void hrPtMiniDataLoad(char *stname) {
    char Name[128];
    s32 i;

    strcpy(Name, stname);
    strcat(Name, "/pts");
    hr_stage_no(Name, 0);
    strcat(Name, ".bin");
    hrpt_mini_addr = (s32 *)getBuff(0, 0, Name, &i);
}

void hrPtMini_init() {
    hrpt_mini = NULL;
}

static void hr_pt_LoadPack() {
    s32 i;

    ppmes.font = (u32 *)-1;
    hrpt_addr = (s32 *)-1;
    if ((s32)hrpt_pack == -1) {
        return;
    }
    if (hrpt_scene << 1 < *hrpt_pack) {
        hrpt_addr = (s32 *)GetFHMAddress(hrpt_pack, hrpt_scene << 1);
        ppmes.font = hrpt_gms;
    }
}

void hr_ptcall_getbuff() {
    s32 i;

    hrpt_call = (HR_CALL *)getBuff(1, 16 * sizeof(HR_CALL), "ps_call", &i);
}

void hr_pall_init() {
    if (hrpt_flag == 0) {
        return;
    }
    hrpt_sbgm = 0;
    sceVu0UnitMatrix(hrpum);
    hr_psys_init(&ppsys, hrpt_addr, &ppcam, hrptm_addr, &ppmes, &ppwaku);
    hr_pcam_init(GameGbl.cam.posi, GameGbl.cam.ang, GameGbl.cam.scr_z);
    hr_pmes_init(&ppmes);
    ppwaku.flag = 0;
    hrptm_res = 0;
    hrpt_patch = 0;
    if (hrpt_flag != 4) {
        hPptReset();
        hrpt_patch |= 1;
    }
    if (hrpt_flag != 4) {
        hrpt_stnk = 64;
    } else {
        hrpt_stnk = 0;
    }
}

static void hr_pt_skipcheck(HR_PSYS *ps) {
    if (ps->flag & 0x200) {
        return;
    }

    if (((0x3A < hrpt_id || (hrpt_id < 0x39 && hrpt_id != PTOPENING && hrpt_id != PT1304_S0) || (hrpt_view != 0 || sD->Stereo != SND_MODE_5P1CH)) && GameGbl.kpd[0].trg & 0x800)) {
        pt_set_skip(ps);
    }
}

s32 hr_check_skw() {
    if (ppsys.flag & 0x20) {
        if (hrpt_patch & 4) {
            hrpt_patch &= -5;
            return 0;
        }
        return ppsys.skipcnt;
    }
    return 0;
}

s32 hr_pall_work() {
    if (hrpt_flag == 0) {
        return 0;
    }

    if (hrpt_patch & 1) {
        hrpt_patch &= -2;
        hSndEffSetVol_PPTstart();
    }
    if (!(ppsys.flag & 0x1000) && hrpt_flag != 4) {
        ppsys.flag |= 0x1000;
        hr_pflag_onS();
    }
    if (ppsys.flag & 0x20) {
        if (hrpt_patch & 2) {
            hrpt_patch &= -3;
            pt_set_skip2(&ppsys);
        }
        if (ppsys.skipcnt == 0) {
            pt_set_exit(&ppsys, 10);
        }
    } else {
        if (ppmes.flag & 0x40000) {
            ppmes.flag &= -0x40001;
            ppmes.stop = 0;
        }
        hr_pcam_work_mae();
        hr_psys_work(&ppsys);
        hr_pcam_work();
        if (hrpt_flag != 4) {
            hr_pmes_work(&ppmes);
            if (ppmes.flag & 0x10000) {
                ppmes.flag &= -0x10001;
                hr_ptvoice_stop(&ppsys, &ppmes);
            }
            if (!(ppmes.flag & 0x4000) && ppsys.flag & 0x400) {
                ppsys.flag &= -0x401;
            }
            if (ppmes.flag & 0x200) {
                ppmes.flag &= -0x201;
                hr_ptvoice_call(&ppsys, &ppmes);
            }
        }
        if (hrpt_patch & 0x10) {
            hrpt_patcnt--;
            if (hrpt_patcnt == 0) {
                hrpt_patch &= -0x11;
                hr_hse_areaend();
                hr_pt_setBGM(&ppsys, -1, 0);
            }
        }
    }
    if (hrpt_stnk != 0) {
        hrpt_stnk--;
    }
    if (hrpt_stnk == 0 && !(ppsys.flag & 0x2A0) && hrpt_flag != 4) {
        hr_pt_skipcheck(&ppsys);
    }
    if (ppsys.flag & 0x80) {
        if (hrpt_id == PT17XX_S0) {
            ppsys.klov = 3;
        }
        return hr_psys_exit(&ppsys);
    }
    return 0;
}

void hr_pall_ncnt() {
    hr_ptnext_klo(ppsys.ncVi, ppsys.klono, ppsys.klomc >> 12, ppsys.klov, ppsys.kloy);
}

s32 hr_pall_ncntSN() {
    return ppsys.ncSc;
}

s32 hr_pall_exitCam() {
    return ppsys.ecount0;
}

void hr_hse_areaend() {
    s32 var;

    if (hrpt_view != 0) {
        return;
    }

    if (ppsys.flag & 0x40) {
        if (ppsys.ncSc != -1) {
            var = -1;
        } else {
            var = ppsys.ncVi;
        }
    } else {
        if (hr_pt_checkExit(&ppsys) != 1) {
            return;
        }
        var = -1;
    }

    if (hrpt_id < 59 && (56 < hrpt_id || (hrpt_id == PTOPENING) || hrpt_id == PT1304_S0) && (hrpt_view == 0 && sD->Stereo == SND_MODE_5P1CH)) {
        hStr_0016f718(0x2d);
    }

    hGameAreaEnd(var);
}

void hr_pall_draw() {
    hr_psys_motdraw(&ppsys, GameGbl.pause_flag);
}

void hr_pall_aftdraw() {
    sceGsSyncPath(0, 0);
    if (!(hrpt_flag & 0x18)) {
        if (hrpt_flag != 4 && ppsys.mesp != NULL) {
            hr_mesp_draw(ppsys.mesp, &ppmes);
        }
        hr_pfade_draw(&ppsys.fade);
        if (hrpt_flag != 4) {
            hr_pmes_draw(&ppmes);
        }
    }
    hr_pt_skipdraw(&ppsys);
}