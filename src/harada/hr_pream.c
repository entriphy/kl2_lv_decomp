#include "harada/hr_pream.h"
#include "harada/hr_main.h"
#include "harada/hr_pbgm.h"
#include "harada/hr_pmes.h"
#include "harada/hr_pall.h"
#include "harada/hr_pread.h"
#include "hoshino/h_gamesnd.h"
#include "hoshino/h_str.h"

// TODO: fill these in
///* data 2e80e8 */ s32 (*HrPtMotTbl[8])(/* parameters unknown */);
///* data 2e8108 */ s32 (*HrPtSndTbl[14])(/* parameters unknown */);
/* sbss 375870 */ static PT32A *p32a;
/* sbss 375874 */ static PT32B *p32b;
/* sbss 375878 */ static PT64A *p64a;
/* sbss 37587c */ static PT64B *p64b;
/* sbss 375880 */ static PT96D *p96d;
/* sbss 375884 */ static PT96E *p96e;
/* sbss 375888 */ static PT96F *p96f;

void hr_set_motion(HR_CALL *ca, s32 noth) {
    if (noth == 0) {
        hr_take_motionSet(ca->hObj, ca->motno, 0);
    } else {
        hr_take_motionSetN(ca->hObj, ca->motno, 0);
    }

    ca->flag |= 4;
}

f32* pt_se_panpos(HR_CALL *ca, u16 on) {
    if (on == 0) {
        if (ca->hObj != NULL) {
            hr_pt_getpos(ca, ca->hObj->Base.Trans);
            return ca->hObj->Base.Trans;
        } else {
            return ca->pos.p;
        }
    } else {
        return NULL;
    }
}

s32 pt_se_changer(s32 id) {
    if ((s32)hrse_pack == -1) {
        return -1;
    }
    if (id < 0 || *hrse_pack <= id) {
        return -1;
    }
    return hrse_pack[id + 1];
}

static s32 pt_motlp_secall(HR_CALL *ca) {
    hSeKeyOn(ca->sevag & 0xFFFFFFFF, pt_se_panpos(ca, 0), 0);
}

static s32 pt_mot_play(HR_CALL *ca, HR_PSYS *ps) {
    s32 noth;

    p64a = (PT64A *)ca->read;
    ca->motlp = p64a->ss0;
    ca->motlpi = 0;
    ca->flag &= -0x2001;
    ca->motno = p64a->ss1;

    if (p64a->code == 0x0104 || p64a->code == 0x0109) {
        noth = 1;
    } else {
        noth = 0;
    }

    if (ca->hObj != NULL) {
        hr_set_motion(ca, noth);
        if (p64a->code == 0x0108 || p64a->code == 0x0109) {
            ca->sevag = pt_se_changer(p64a->ss2);
            if (ca->sevag != -1) {
                ca->flag |= 0x2000;
                pt_motlp_secall(ca);
            }
        }
    }

    ca->read += 2;
    return 1;
}

static s32 pt_mot_wait(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    if (p32b->us0 == 0) {
        if (ca->flag & 4) {
            return 0;
        }
    } else if (ca->motlpi < p32b->us0) {
        return 0;
    }

    ca->read++;
    return 1;
}

static s32 pt_mime_set(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    ca->read++;
    return 1;
}

static s32 pt_lips(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    if (hrpt_id == PT13XX_S0) {
        if (ca->hObj != NULL) {
            hr_take_lips(ca->hObj, pt_al_data(ca, p32a->ss0));
        }
    }

    ca->read++;
    return 1;
}

static s32 pt_mot_clip(HR_CALL *ca, HR_PSYS *ps) {
    s32 fg;

    p32b = (PT32B *)ca->read;
    if (p32b->us0 == 0) {
        fg = 1;
    } else {
        fg = 0;
    }

    if (ca->hObj != NULL) {
        hr_take_modelclip(ca->hObj, fg);
    }

    ca->read++;
    return 1;
}

static s32 pt_mot_line(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    if (p32b->us0 == 1) {
        hr_take_rinkaku(ca->hObj);
    }
    ca->read++;
    return 1;
}

static s32 pt_mot_lpdata(HR_CALL *ca, HR_PSYS *ps) {
    s32 *ptr;

    p32a = (PT32A *)ca->read;
    ptr = ca->read + p32a->ss0;
    ps->cntlip = *ptr++;
    ps->addrlip = (u16 *)ptr;
    ca->read++;
    return 1;
}

static s32 pt_snd_voice(HR_CALL *ca, HR_PSYS *ps) {
    p32b = (PT32B *)ca->read;
    hr_ptvoice_call(ps, ps->pmes);
    ca->read++;
    return 1;
}

static s32 pt_snd_se(HR_CALL *ca, HR_PSYS *ps) {
    f32 *f;
    s32 splt; // ?

    p64b = (PT64B *)ca->read;
    f = pt_se_panpos(ca, p64b->us0);
    splt = pt_se_changer(p64b->si0);
    if (splt != -1) {
        if (p64b->code == 0x0401) {
            s32 obj = hSeKeyOn((s64)splt & 0xFFFFFFFF, f, 0);
            if (hrpt_id == PTTITLE) {
                switch (p64b->si0) {
                    case 6:
                        hSeSetObjPan(obj, 1.0f, 0.0f);
                        break;
                    case 7:
                        hSeSetObjPan(obj, 0.0f, 1.0f);
                        break;
                }
            }
        } else if (p64b->code == 0x0403) {
            hr_seloop_on(ps->se, splt, f);
        } else {
            hr_seloop_off(ps->se, splt);
        }
    }

    ca->read += 2;
    return 1;
}

static s32 pt_snd_vcdata(HR_CALL *ca, HR_PSYS *ps) {
    s32 *ptr;

    p32a = (PT32A *)ca->read;
    ptr = ca->read + p32a->ss0;
    hPptSetList(ptr);
    ps->cntvc = *ptr++;
    ps->addrvc = ptr;
    hr_pmes_ns_st(ps->pmes);

    ca->read++;
    return 1;
}

static s32 pt_snd_bgm(HR_CALL *ca, HR_PSYS *ps) {
    s32 mode;

    p32b = (PT32B *)ca->read;
    if (p32b->code == 0x0405) {
        mode = 0;
    } else {
        mode = 2;
    }

    if (hrpt_view == 0) {
        hr_pt_setBGM(ps, p32b->us0, mode);
    }

    ca->read++;
    return 1;
}

static s32 pt_se_pitset(HR_CALL *ca, HR_PSYS *ps) {
    p96e = (PT96E *)ca->read;
    ca->read += 3;
    return 1;
}

static s32 pt_se_pitmv(HR_CALL *ca, HR_PSYS *ps) {
    p96e = (PT96E *)ca->read;
    ca->read += 3;
    return 1;
}

static s32 pt_se_pitmvp(HR_CALL *ca, HR_PSYS *ps) {
    p96e = (PT96E *)ca->read;
    ca->read += 3;
    return 1;
}

static HR_PTSE* pt_lse_search(HR_PSYS *ps, s32 id) {
    s32 splt;

    splt = pt_se_changer(id);
    if (splt == -1) {
        return NULL;
    } else {
        return hr_seloop_search(ps, splt);
    }
}

static s32 pt_se_volset(HR_CALL *ca, HR_PSYS *ps) {
    HR_PTSE *se;

    p96f = (PT96F *)ca->read;
    se = pt_lse_search(ps, p96f->ui0);
    if (se != NULL) {
        se->vol.p = p96f->f0;
        hSeSetObjVol(se->id, se->vol.p);
    }

    ca->read += 3;
    return 1;
}

static s32 pt_se_volmv(HR_CALL *ca, HR_PSYS *ps) {
    HR_PTSE *se;

    p96f = (PT96F *)ca->read;
    se = pt_lse_search(ps, p96f->ui0);
    if (se != NULL) {
        comm_getft_mvF(ca, &se->vol);
    }

    ca->read += 3;
    return 1;
}

static s32 pt_se_volmvp(HR_CALL *ca, HR_PSYS *ps) {
    HR_PTSE *se;

    p96f = (PT96F *)ca->read;
    se = pt_lse_search(ps, p96f->ui0);
    if (se != NULL) {
        comm_getft_mvpF(ca, &se->vol);
    }

    ca->read += 3;
    return 1;
}

static s32 pt_snd_sev(HR_CALL *ca, HR_PSYS *ps) {
    f32 *f;
    s32 splt;

    p96d = (PT96D *)ca->read;
    f = pt_se_panpos(ca, p96d->us0);
    splt = pt_se_changer(p96d->si0);
    if (splt != -1) {
        hSeSetObjVol(hSeKeyOn((s64)splt & 0xFFFFFFFF, f, 0), p96d->f0);
    }

    ca->read += 3;
    return 1;
}

static s32 pt_snd_bgmsync(HR_CALL *ca, HR_PSYS *ps) {
    ca->read++;
    return 1;
}

static s32 pt_snd_ac3j(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    if (sD->Stereo == SND_MODE_5P1CH) {
        ca->read += p32a->ss0;
    } else {
        ca->read++;
    }
    return 1;
}

static s32 pt_snd_ac3s(HR_CALL *ca, HR_PSYS *ps) {
    p32a = (PT32A *)ca->read;
    if (sD->Stereo != SND_MODE_5P1CH) {
        ca->read += p32a->ss0;
    } else {
        ca->read++;
    }
    return 1;
}
