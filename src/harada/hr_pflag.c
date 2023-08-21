#include "harada/hr_pflag.h"
#include "harada/hr_pall.h"
#include "harada/hr_pbgm.h"
#include "harada/hr_main.h"

static PTDATA scene_table[90] = {}; // TODO

void hr_pflag_initAr() {
    memset(ptflag_ar, 0, 0x10);
}

void hr_pflag_initSt() {
    memset(ptflag_st, 0, 0x10);
    hr_pflag_initAr();
}

void hr_pflag_init() {
    memset(ptflag_buff, 0, 0x10);
    memset(ptflag_th, 0, 0x10);
    hr_pflag_initSt();
}

void hr_pflag_EndCopy(s32 *moto, s32 *saki) {
    s32 i;

    for (i = 0; i < 4; i++, moto++, saki++) {
        *saki |= *moto;
    }
}

s32 hr_pflag_sam(HRMEMC *buff) {
    s32 i;
    s32 *var_a0 = (s32 *)buff + 1;
    s32 var_a1;
    s32 var_a2 = sizeof(HRMEMC) / 4 - 1;

    for (var_a1 = 0, i = 0; i < var_a2; i++, var_a0++) {
        var_a1 += *var_a0;
    }

    return var_a1 + 0x20010119;
}

void hr_pflag_memcLoad(HRMEMC *buff) {
    hr_pflag_init();
    if (buff->id == hr_pflag_sam(buff)) {
        hr_pflag_EndCopy(buff->ptkeep, ptflag_buff);
    }
}

void hr_pflag_memcSave(HRMEMC *buff) {
    memset(buff, 0, sizeof(HRMEMC));
    hr_pflag_EndCopy(ptflag_buff, buff->ptkeep);
    buff->ver = 0;
    buff->id = hr_pflag_sam(buff);
}

void hr_pflag_memcClear(HRMEMC *buff) {
    memset(buff, 0, sizeof(HRMEMC));
    buff->ver = 0;
    buff->id = hr_pflag_sam(buff);
}

void hr_pflag_on(s32 id, s32 onoff) {
    s32 size = 32;
    s32 i = id / size;

    if (i < 4) {
        s32 mask = 1 << id % size;
        if (onoff != 0) {
            if (hrpt_th != 0) {
                ptflag_th[i] |= mask;
            } else {
                ptflag_ar[i] |= mask;
            }
        } else if (hrpt_th != 0) {
            ptflag_th[i] &= ~mask;
        } else {
            ptflag_ar[i] &= ~mask;
        }
    }
}

void hr_pflag_theatar(s32 id, s32 mode) {
    memset(ptflag_th, 0, 4 * sizeof(s32));
    if (mode != 0) {
        switch (id) {
            case PT0706_S0:
                hr_pflag_on(PT0609_S0, 1);
                break;
            case PT0609_S0:
                hr_pflag_on(PT0706_S0, 1);
                break;
            default:
                return;
        }
    }
}

s32 hr_pflag_checkSV(s32 id) {
    s32 i;
    s32 size;

    size = 32;
    i = id / size;
    if (i > 3) {
        return 1;
    }
    id = 1 << (id % size);
    if (hrpt_th != 0) {
        return ptflag_th[i] & id;
    } else {
        return ptflag_buff[i] & id;
    }
}

s32 hr_pflag_check(s32 id) {
    s32 i;
    s32 size;

    size = 32;
    i = id / size;
    if (i > 3) {
        return 1;
    }
    id = 1 << (id % size);
    size = ptflag_st[i] & id;
    if (size == 0) {
        size = ptflag_ar[i] & id;
    }
    return size;
}

s32 hr_pflag_get_id() {
    PTDATA *table = scene_table;
    s32 i;

    for (i = 0; i < 90; i++) {
        if (table->area == GameGbl.vision && table->scene == hrpt_scene) {
            return i;
        }
        table++;
    }

    return -1;
}

s16 hr_pflag_get_vs(s32 id) {
    PTDATA *p;

    p = scene_table;
    if (id >= 90) {
        hrpt_scene = 0;
        return 0;
    }
    p += id;
    hrpt_scene = p->scene;
    return p->area;
}

void hr_pflag_onS() {
    s32 id = hrpt_id;
    if (id != -1) {
        hr_pflag_on(id, 1);
    }
}

s32 hr_ASFGcheck(s32 id) {
    s32 var_s0 = 0;
    if (hr_resFg != 0) {
        return var_s0;
    }
    switch (id) {
        case PT1500_S0:
            if (OkGetSwitchFlag2(4, 1) == 1) {
                var_s0++;
            }
            break;
        case PT1502_S2:
            if (OkGetSwitchFlag2(4, 2) == 1) {
                var_s0++;
            }
            break;
        case PT1502_S1:
            if (OkGetSwitchFlag2(4, 3) == 1) {
                var_s0++;
            }
            break;
    }
    return var_s0;
}

void hr_pflag_AreaStart(s32 mode) {
    if (kzCheckTheater()) {
        return;
    }
    hr_pt_fclear();

    switch (GameGbl.vision) {
        case 0x100:
            if (!hr_pflag_check(PT0100_S2)) {
                hr_pt_set(1, 2, mode, 0);
            } else if (!hr_pflag_check(PT0100_S0)) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x105:
            if (!hr_pflag_check(PTOPENING)) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x204:
            if (hr_pflag_check(PT0204_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x207:
            if (hr_pflag_check(PT0207_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x403:
            if (hr_pflag_check(PT0403_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x500:
            if (hr_pflag_check(PT0500_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x505:
            if (hr_pflag_check(PT0505_S2) == 0) {
                hr_pt_set(1, 2, mode, 0);
            }
            break;
        case 0x605:
            if (hr_pflag_check(PT0605_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x609:
            if (hr_pflag_check(PT0609_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x706:
            if (hr_pflag_check(PT0706_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x806:
            if (hr_pflag_check(PT0706_S1) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x908:
            if (hr_pflag_check(PT09XX_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            } else if (hr_pflag_check(PT0908_S1) == 0) {
                hr_pt_set(1, 1, mode, 0);
            }
            break;
        case 0xA00:
            if (hr_pflag_check(PT1000_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0xB00:
            if (hr_pflag_check(PT1100_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0xB11:
            if (hr_pflag_check(PT1111_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0xC01:
            if (hr_pflag_check(PT1200_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0xC07:
            if (hr_pflag_check(PT1207_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            } else if (hr_pflag_check(PT1207_S1) == 0) {
                hr_pt_set(1, 1, mode, 0);
            } else if (hr_pflag_check(PT1207_S2) == 0) {
                hr_pt_set(1, 2, mode, 0);
            } else if (hr_pflag_check(PT1207_S3) == 0) {
                hr_pt_set(1, 3, mode, 0);
            }
            break;
        case 0xC08:
            if (hr_pflag_check(PT1304_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            goto yeet;
            break;
        case 0xD05:
        yeet:
            if (hr_pflag_check(PT13XX_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            } else if (hr_pflag_check(PT13XX_S2) == 0) {
                hr_pt_set(1, 1, mode, 0);
            }
            break;
        case 0xD06:
            if (hr_pflag_check(PT13XX_S1) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0xE00:
            if (hr_pflag_check(PT14XX_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0xF02:
            if (hr_pflag_check(PT1502_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            } else if (hr_pflag_check(PT1500_S0) == 0 && hr_ASFGcheck(PT1500_S0) != 0) {
                hr_pt_set(1, 3, mode, 0);
            } else if (hr_pflag_check(PT1502_S2) == 0 && hr_ASFGcheck(PT1502_S2) != 0) {
                hr_pt_set(1, 2, mode, 0);
            } else if (hr_pflag_check(PT1502_S1) == 0 && hr_ASFGcheck(PT1502_S1) != 0) {
                hr_pt_set(1, 1, mode, 0);
            }
            break;
        case 0x1000:
            if (hr_pflag_check(PT16XX_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x1100:
            if (hr_pflag_check(PT17XX_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x1205:
            hr_pt_set(1, 0, mode, 0);
            break;
        case 0x1308:
            hr_pt_set(1, 0, mode, 0);
            break;
        case 0x1500:
            if (hr_pflag_check(PT2100_S0) == 0){
                hr_pt_set(1, 0, mode, 0);
            } else if (hr_pflag_check(PT2100_S2) == 0 && hr_resFg == 0) {
                hr_pt_set(1, 2, mode, 0);
            }
            break;
        case 0x1601:
            if (hr_pflag_check(PT4002_S1) == 0) {
                hr_pt_set(1, 1, mode, 0);
            } else if (hr_pflag_check(PT2201_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x1700:
            if (hr_pflag_check(PT20XX_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x1701:
            if (hr_pflag_checkSV(PTXXXX_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            } else if (hr_pflag_check(PT20XX_S1) == 0) {
                hr_pt_set(1, 1, mode, 0);
            }
            break;
        case 0x1800:
            if (hr_pflag_check(PT20XX_S2) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x1A00:
            if (hr_pflag_check(PT20XX_S5) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x1A01:
            if (hr_pflag_check(PT20XX_S7) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x1B00:
            if (hr_pflag_check(PT2XXX_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            } else if (hr_pflag_check(PT2XXX_S1) == 0) {
                if (hr_resFg == 0) {
                    hr_pt_set(1, 1, mode, 0);
                }
            } else if (hr_pflag_check(PT2XXX_S3) == 0 && hr_resFg == 0) {
                hr_pt_set(1, 3, mode, 0);
            }
            break;
        case 0x1B01:
            if (hr_pflag_check(PT2701_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x2701:
            if (hr_pflag_check(PTENDING) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x2801:
            if (hr_pflag_checkSV(PT4001_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            } else if (hr_pflag_checkSV(PT4001_S1) == 0) {
                hr_pt_set(1, 1, mode, 0);
            } else if (hr_pflag_checkSV(PT4001_S2) == 0) {
                hr_pt_set(1, 2, mode, 0);
            } else if (hr_pflag_checkSV(PT4001_S3) == 0) {
                hr_pt_set(1, 3, mode, 0);
            } else if (hr_pflag_checkSV(PT4001_S4) == 0) {
                hr_pt_set(1, 4, mode, 0);
            }
            break;
        case 0x2802:
            if (hr_pflag_check(PT4002_S0) == 0) {
                hr_pt_set(1, 0, mode, 0);
            }
            break;
        case 0x1E01:
            hr_pt_set(1, 0, mode, 0);
            break;
        case 0x2300:
            hrpt_scene = kzGetScene();
            if (hrpt_scene != -1) {
                hr_pt_set(1, hrpt_scene, mode, 0);
            } else {
                hrpt_scene = 0;
            }
            break;

    }
}
