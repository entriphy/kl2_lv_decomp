#include "harada/hr_prm.h"
#include "harada/hr_pcam.h"
#include "harada/hr_pall.h"
#include "harada/hr_obcon.h"
#include "harada/hr_obcon2.h"
#include "harada/hr_nak.h"
#include "harada/hr_pefc.h"
#include "harada/hr_pbgm.h"
#include "harada/hr_pread.h"
#include "harada/hr_preak.h"
#include "harada/hr_pflag.h"
#include "harada/hr_mapv.h"
#include "harada/hr_take.h"
#include "okanoyo/okio.h"
#include "hoshino/h_gamesnd.h"
#include "hoshino/h_sound.h"
#include "kazuya/interfac.h"
#include "hoshino/h_str.h"

static s32 hrpt_vcntid;
static sceVu0FVECTOR hatrot; // TODO: find out where this is supposed to go

s32* pt_al_data(HR_CALL *ca, s16 ptr) {
    s32 *data;

    data = ca->read + ptr;
    return data + *data;
}

s32* pt_al_dataB(s32 *base, s16 ptr) {
    s32 *data;

    data = base + ptr;
    return data + *data;
}

void hr_pcall_rtinit(HR_CALL *ca) {
    memset(&ca->route, 0, sizeof(PCAMSR));
    sceVu0ScaleVectorXYZ(ca->rtpos, ca->rtpos, 0.0f);
    ca->rtpos[3] = 1.0f;
    sceVu0CopyVector(ca->rtrot, ca->rtpos);
    ca->flag &= -0x21;
}

static void hr_pcall_init(HR_CALL *ca) {
    memset(ca, 0x00, sizeof(HR_CALL));
    hr_pcam_cinit(&ca->pos);
    hr_pcam_cinit(&ca->rot);
    ca->flag = 0;
    ca->read = ca->ret = NULL;
    ca->hObj = NULL;
    ca->motlp = ca->motret = ca->motno = ca->motlpi = 0;
    ca->id = ca->chr = ca->fhm = -1;
    ca->time = 0;
    ca->getft = 0;
    ca->efc = PTEFC_NON;
    ca->mem = (s32 *)-1;
    ca->lightno = 0;
    ca->nlight[0][0] = ca->nlight[0][1] = ca->nlight[0][3] = 0.0f;
    ca->nlight[0][2] = 1.0f;
    sceVu0CopyVector(ca->nlight[1], ca->nlight[0]);
    sceVu0CopyVector(ca->nlight[2], ca->nlight[0]);
    hr_pcam_cinit(&ca->nlw[0]);
    hr_pcam_cinit(&ca->nlw[1]);
    hr_pcam_cinit(&ca->nlw[2]);
    hr_pcam_cinit(&ca->lcolor[0]);
    hr_pcam_cinit(&ca->lcolor[1]);
    hr_pcam_cinit(&ca->lcolor[2]);
    ca->lcolor[0].p[0] = ca->lcolor[0].p[1] = ca->lcolor[0].p[2] = ca->lcolor[0].p[3] = 1.0f;
    ca->ambi[0] = ca->ambi[1] = ca->ambi[2] = 0.2f;
    ca->ambi[3] = 1.0f;
    hr_pcall_rtinit(ca);
}

HR_CALL* hr_pcall_search(HR_PSYS *ps, HR_CALL *call) {
    HR_CALL *ca;
    s32 i;

    if (call == &ps->sys) {
        i = 0;
    } else {
        i = ((u32)call - (u32)ps->call) / sizeof(HR_CALL) + 1;
    }
    ca = &ps->call[i];

    for (; i < 16; i++, ca++) {
        if (ca->flag == 0) {
            hr_pcall_init(ca);
            ps->ncall++;
            return ca;
        }
    }
    return NULL;
}

HR_CALL* hr_id_search(HR_PSYS *ps, s16 id) {
    HR_CALL *ca;
    s32 i;

    for (i = 0, ca = ps->call; i < 16; i++, ca++) {
        if (ca->flag != 0 && ca->id == id) {
            return ca;
        }
    }
    return NULL;
}

HR_CALL* hr_pfg_search(HR_PSYS *ps, s32 flag) {
    HR_CALL *ca;
    s32 i;

    for (i = 0, ca = ps->call; i < 16; i++, ca++) {
        if (ca->flag & flag) {
            return ca;
        }
    }
    return NULL;
}

static void hr_call_memOFF(HR_CALL *ca) {
    if (ca->hObj != NULL) {
        if (ca->hObj->pSfx != NULL) {
            EraseSfxObject(ca->hObj->pSfx);
        }
        freemem(ca->hObj);
    }

    if ((s32)ca->mem != -1) {
        freemem(ca->mem);
    }
}

s32 hr_efc_why(s32 id) {
    s32 ret;

    switch (id) {
        case PT0706_SWON:
        case PT0706_SWOFF:
        case PTOPEN_RAKKA:
        case PT1111_MIRR:
        case PT1111_MIRRF:
        case PT1306_HANET:
        case PT1306_HANEH:
        case PT2601_ATAMA:
        case PT0605_YUKA:
        case PT0605_YUKAD:
        case PT0706_SWLON:
        case PT0806_SWBOU:
            ret = PTE_HARADA;
            break;
        case PT2100_BOSS0:
        case PT2100_BOSS1:
        case PT2300_BOSS0:
        case PT2301_BOSS1:
        case PT2301_BOSS2:
        case PT0801_BOSS2:
        case PT0801_BOSS4:
        case PT0801_BOSS7:
        case PT0801_BOSS8:
        case PT0805_BOSS3:
        case PT0805_BOSS4:
        case PT0805_BOSS6:
        case PT2400_BOSS0:
        case PT2701_SCROLL:
        case PT2100_BOSS2:
        case PT2100_BOSS3:
            ret = PTE_HATO;
            break;
        default:
            ret = PTE_ABE;
            break;
    }

    return ret;
}

void hr_call_efcdel(HR_CALL *ca, HR_PSYS *ps) {
    s32 pte;

    if (ca->efc == PTEFC_NON) {
        return;
    }

    pte = hr_efc_why(ca->efc);
    if (pte == PTE_ABE) {
        hr_abe_efcDel(ca->efc);
    } else if (pte == PTE_HATO) {
        hr_hato_efcDel(ca->efc);
    } else {
        if (ps->efc != NULL) {
            hr_ptes_DEL(ps->efc, ca->efc);
        }
    }

    ca->efc = PTEFC_NON;
}

static HR_ECONT* hr_search_ec(HR_PSYS *ps, s32 id) {
    HR_ECONT *ec;
    s32 i;

    for (i = 0, ec = ps->econ; i < 8; i++, ec++) {
        if (ec->id == id) {
            return ec;
        }
    }
    return NULL;
}

static void hr_del_econt(HR_PSYS *ps, HR_ECONT *ec) {
    s32 pte;

    pte = hr_efc_why(ec->id);
    if (pte == PTE_ABE) {
        hr_abe_efcDel(ec->id);
    } else if (pte == PTE_HATO) {
        hr_hato_efcDel(ec->id);
    } else {
        if (ps->efc != NULL) {
            hr_ptes_DEL(ps->efc, ec->id);
        }
    }
    ec->id = PTEFC_NON;
}

void hr_call_efcdelE(HR_PSYS *ps, s32 id) {
    HR_ECONT *ec;

    if (id != 0) {
        ec = hr_search_ec(ps, id);
        if (ec != NULL) {
            hr_del_econt(ps, ec);
        }
    }
}

static void hr_del_econtALL(HR_PSYS *ps) {
    HR_ECONT *ec;
    s32 i;

    for (i = 0, ec = ps->econ; i < 8; i++, ec++) {
        if (ec->id != PTEFC_NON) {
            hr_del_econt(ps, ec);
        }
    }
}

void hr_call_efcon(HR_CALL *ca, HR_PSYS *ps, s32 id, s32 cmd) {
    HR_ECONT *ec;
    s32 pte;

    if (id == PTEFC_NON) {
        return;
    }

    sceVu0ScaleVector(hatrot, ca->rot.p, 3.141592f);
    sceVu0DivVector(hatrot, hatrot, 180.0f);
    hatrot[3] = 1.0f;

    hr_efc_ofs(ca, id);
    pte = hr_efc_why(id);
    if (pte == PTE_ABE) {
        hr_abe_efcOn(id, hrvo);
    } else if (pte == PTE_HATO) {
        hr_hato_efcOn(id, hrvo, hatrot);
    } else {
        if (ps->efc == NULL) {
            ps->efc = (OBJWORK *)hr_init_obconM(HR_PTEFFECT, NULL);
            if (ps->efc == NULL) {
                return;
            }
        }
        if (hr_ptes_ON(ps->efc, id, hrvo, ca->rot.p) == 0) {
            return;
        }
    }

    if (cmd == 0x0030) {
        ca->efc = id;
        return;
    }

    ec = hr_search_ec(ps, PTEFC_NON);
    if (ec != NULL) {
        ec->id = id;
        ec->x = hrvo[0];
        ec->y = hrvo[1];
        ec->z = hrvo[2];
        ec->rx = hatrot[0];
        ec->ry = hatrot[1];
        ec->rz = hatrot[2];
    }
}

void hr_callbuff_init(HR_PSYS *ps, s32 fg) {
    s32 i;
    HR_CALL *ca;

    ps->ncall = 0;
    if ((s32)ps->call != -1) {
        if (fg) {
            for (i = 0, ca = ps->call; i < 16; i++, ca++) {
                if (ca->flag != 0) {
                    hr_call_memOFF(ca);
                    hr_call_efcdel(ca, ps);
                }
            }
        }

        for (ca = ps->call, i = 0; i < 16; i++) {
            hr_pcall_init(ca++);
        }
    }

    ps->mircnt = 0;
}

void hr_rt_posrot(HR_CALL *ca) {
    sceVu0FVECTOR v;

    MichiPosi(&ca->rtw, ca->rtpos);
    GetMichiVec(&ca->rtw, v);
    GetMichiRotXY(v, ca->rtrot);
    ca->rtrot[0] = ca->rtrot[2] = 0.0f;
}

void pt_set_rtpos(HR_CALL *ca, s32 si0, u16 us0) {
    s32 *data;
    s32 ofs;

    ca->flag |= 0x20;
    ca->route.acc = 0;
    ca->route.spd = 0;
    ca->route.p = si0;
    if (ca->route.addr == NULL) {
        SetRouteNo(&ca->rtw, us0);
        SetMichinori(&ca->rtw, si0);
    } else {
        data = (s32 *)ca->route.addr;
        ofs = us0;
        if (*data <= us0) {
            ofs = 0;
        }
        data += data[ofs + 1] / 4;
        (ca->rtw).rtp = (ROUTE *)data;
        (ca->rtw).rtn = ofs;
    }

    hr_rt_posrot(ca);
}

void pt_get_routeK(HR_CALL *ca) {
    RT_WRK *rt;
    s32 no;

    if (hrpt_flag & 2) {
        rt = hr_get_klort();
        no = GetMichiNo(rt->rtp);
        if (no != -1) {
            pt_set_rtpos(ca, rt->mcn, no);
        } else {
            pt_set_rtpos(ca, 0, 0);
        }
    }
}

void hr_psys_init(HR_PSYS *ps, s32 *addr, PCAMS *cam, s32 *addrm, HRPMOJI *pmes, HRPMWAKU *waku) {
    memset(ps, 0, sizeof(HR_PSYS));
    ps->cam = cam;
    ps->pmes = pmes;
    ps->waku = waku;
    ps->model = addrm;
    hr_pfade_init(&ps->fade);
    ps->call = hrpt_call;
    hr_callbuff_init(ps, 0);
    hr_pcall_init(&ps->sys);
    ps->sys.read = addr;
    pt_get_routeK(&ps->sys);
    ps->idvc = -1;
    hr_ptbgm_init(ps);
    addr++; // idk
}

void hr_seloop_on(HR_PTSE *se, u32 splt, f32 *pos) {
    s32 i;

    for (i = 0; i < 4; i++, se++) {
        if (se->flag == 0) {
            memset(se, 0, sizeof(HR_PTSE));
            se->flag++;
            se->id = hSeKeyOn(splt, pos, 0);
            se->splt = splt;
            se->pit.p = 1200;
            se->vol.p = 1.0f;
            break;
        }
    }
}

void hr_seloop_off(HR_PTSE *se, u32 splt) {
    s32 i;

    for (i = 0; i < 4; i++, se++) {
        if (se->flag != 0 && se->splt == splt) {
            hSeKeyOff(se->id);
            se->flag = 0;
            break;
        }
    }
}

static void hr_seloop_alloff(HR_PTSE *se) {
    s32 i;

    for (i = 0; i < 4; i++, se++) {
        if (se->flag != 0) {
            hSeKeyOff(se->id);
        }
    }
}

HR_PTSE* hr_seloop_search(HR_PSYS *ps, u32 splt) {
    s32 i;
    HR_PTSE *se;

    for (i = 0, se = ps->se; i < 4; i++, se++) {
        if (se->flag != 0 && se->splt == splt) {
            return se;
        }
    }
    return NULL;
}

static void pt_getm_mesp(HR_PSYS *ps) {
    if (ps->mesp == NULL) {
        ps->mesp = (HRMESP *)getmem(sizeof(HRMESP) * 8);
        if ((s32)ps->mesp == -1) {
            ps->mesp = NULL;
        } else {
            memset(ps->mesp, 0, sizeof(HRMESP) * 8);
        }
    }
}

static void pt_free_mesp(HR_PSYS *ps) {
    if (ps->mesp != NULL) {
        freemem(ps->mesp);
        ps->mesp = NULL;
    }
}

static HRMESP* pt_search_Amesp(HRMESP *mesp) {
    s32 i;

    for (i = 0; i < 8; i++, mesp++) {
        if (mesp->flag == 0) {
            memset(mesp, 0, sizeof(HRMESP));
            return mesp;
        }
    }
    return NULL;
}

static HRMESP* pt_search_Imesp(HRMESP *mesp, u16 *addr) {
    s32 i;

    for (i = 0; i < 8; i++, mesp++) {
        if (mesp->flag & 1 && mesp->ptr == addr) {
            return mesp;
        }
    }
    return NULL;
}

void pt_set_mesp(HR_CALL *ca, HR_PSYS *ps, u16 *addr, s16 time) {
    HRMESP *mesp;

    if (ps->mesp == NULL) {
        pt_getm_mesp(ps);
        if (ps->mesp == NULL) {
            return;
        }
    }

    mesp = pt_search_Amesp(ps->mesp);
    if (mesp != NULL) {
        mesp->flag |= 1;
        mesp->time = time;
        mesp->ca = ca;
        mesp->ptr = addr;
        if (time != 0) {
            mesp->flag |= 2;
        } else {
            mesp->alpha = 0x80;
        }
    }
}

void pt_del_mesp(HR_CALL *ca, HR_PSYS *ps, u16 *addr, s16 time) {
    HRMESP *mesp;

    if (ps->mesp != NULL) {
        mesp = pt_search_Imesp(ps->mesp, addr);
        if (mesp != NULL) {
            if (time == 0) {
                mesp->flag = 0;
            } else {
                mesp->flag |= 4;
                mesp->count = mesp->time = time;
            }
        }
    }
}

static void pt_work_mesp(HRMESP *mesp) {
    s32 i;

    for (i = 0; i < 8; i++, mesp++) {
        if (!(mesp->flag & 1)) {
            continue;
        }

        if (mesp->flag & 2) {
            mesp->count++;
            if (mesp->count >= mesp->time) {
                mesp->flag &= -3;
                mesp->alpha = 0x80;
            }
        } else if (mesp->flag & 4) {
            mesp->count--;
            if (mesp->count <= 0) {
                mesp->flag = 0;
            }
        }
        if (mesp->flag & 6) {
            mesp->alpha = 128 * mesp->count / mesp->time;
        }
    }
}

s32 hr_pt_checkExit(HR_PSYS *ps) {
    if (hrpt_flag & 4) {
        return 4;
    }
    if (ps->flag & 0x40) {
        return 3;
    }
    if (ps->flag & 0x10) {
        return 2;
    }
    return 1;
}

s32 hr_psys_exit(HR_PSYS *ps) {
    s32 ret;

    if (hrpt_flag != 4) {
        hSndEffSetVol_PPTend();
        if (ps->flag & 0x40) {
            ret = 3;
            if (hrpt_id != PTTITLE) {
                ret |= 0x100;
            }
        } else if (ps->flag & 0x10) {
            ret = 2;
            if (hrpt_view == 0) {
                hr_ptback_klo(ps->klono, ps->klomc >> 12, ps->klov, ps->kloy, ps->flag);
            }
        } else {
            ret = 1;
        }

        if (ps->flag & 0x20) {
            ret |= 0x100;
            if (hrpt_id == PTTITLE) {
                kzSetTPCFlag();
            }
        }
    } else {
        ret = 4;
    }

    hr_seloop_alloff(ps->se);
    hr_callbuff_init(ps, 1);
    hr_call_memOFF(&ps->sys);
    if (ps->efc != NULL) {
        hr_ptefc_erase(ps->efc);
    }
    hr_del_econtALL(ps);
    hr_pt_fclear();
    pt_free_mesp(ps);
    hr_pmes_exit(ps->pmes);
    hr_abe_efcClean();
    if (ret == 2) {
        hr_ptbgm_end(ps,0);
    }

    return ret;
}

void hr_pt_getpos(HR_CALL *ca, sceVu0FVECTOR *v) {
    sceVu0CopyVectorXYZ(*v, ca->pos.p);
    if (ca->flag & 0x20U) {
        sceVu0AddVector(*v, *v, ca->rtpos);
    }
    (*v)[3] = 1.0f;
}

void hr_pt_getrot(HR_CALL *ca, sceVu0FVECTOR *v) {
    sceVu0ScaleVectorXYZ(*v, (ca->rot).p, M_PIf);
    sceVu0DivVectorXYZ(*v, *v, 180.0f);
    if (ca->flag & 0x20) {
        sceVu0AddVector(*v, *v, ca->rtrot);
    }
    (*v)[3] = 1.0f;
    hr_pall_rotpaiV(*v);
}

void hr_pt_getrotD(HR_CALL *ca, sceVu0FVECTOR *v) {
    if (ca->flag & 0x20U) {
        sceVu0ScaleVectorXYZ(*v, ca->rtrot, 180.0f);
        sceVu0DivVectorXYZ(*v, *v, M_PIf);
        sceVu0AddVector(*v, *v, (ca->rot).p);
        (*v)[3] = 1.0f;
    } else {
        sceVu0CopyVector(*v, (ca->rot).p);
    }

    hr_pall_rot180V(*v);
}

s32 hr_pcall_read(HR_CALL *ca, HR_PSYS *ps) {
    u16 *sptr;
    s32 (**functbl)(HR_CALL *, HR_PSYS *);

    sptr = (u16 *)ca->read;
    functbl = (*sptr >> 8)[hrPtFuncTbl]; // bro???
    return functbl[*sptr & 0xFF](ca, ps);
}

void hr_check_time_move(PCAMSC *pcc) {
    sceVu0FVECTOR v;

    if (pcc->time == 0) {
        return;
    }
    if (--pcc->time == 0) {
        sceVu0CopyVector(v, pcc->spd);
        hr_pcam_spdclr(pcc);
        if (pcc->flag & 2) {
            pcc->p[0] = pcc->moku[0];
            pcc->flag ^= 2;
        }
        if (pcc->flag & 4) {
            pcc->p[1] = pcc->moku[1];
            pcc->flag ^= 4;
        }
        if (pcc->flag & 8) {
            pcc->p[2] = pcc->moku[2];
            pcc->flag ^= 8;
        }
    }
}

s32 hr_check_time_moveRT(PCAMSR *rt) {
    if (rt->time == 0) {
        return 0;
    }
    if (--rt->time != 0) {
        return 0;
    }
    rt->p = rt->moku;
    hr_pcamsr_spdclr(rt);
    return 1;
}

s32 hr_check_time_moveL(PCAMSL *len) {
    if (len->time == 0) {
        return 0;
    }
    if (--len->time != 0) {
        return 0;
    }
    len->p = len->moku;
    hr_pcamsl_spdclr(len);
    return 1;
}

void hr_efc_ofs(HR_CALL *ca, s32 id) {
    sceVu0CopyVector(hrvo, ca->pos.p);
}

static void hr_call_efcwork(HR_CALL *ca, HR_PSYS *ps) {
    s32 pte;

    hr_efc_ofs(ca, ca->efc);
    pte = hr_efc_why(ca->efc);
    if (pte == PTE_ABE) {
        hr_abe_efcWork(ca->efc, hrvo);
    } else if (pte == PTE_HATO) {
        sceVu0ScaleVector(hatrot, ca->rot.p, M_PIf);
        sceVu0DivVector(hatrot, hatrot, 180.0f);
        hatrot[3] = 1.0f;
        hr_hato_efcWork(ca->efc, hrvo, hatrot);
    } else if (ps->efc != NULL) {
        hr_ptes_WORK(ps->efc, ca->efc, hrvo, ca->rot.p);
    }
}

void hr_call_efcworkDeb(HR_CALL *ca, HR_PSYS *ps) {
    if (ca->efc != 0 && hr_efc_why(ca->efc) == PTE_HARADA && ps->efc != NULL) {
        hr_efc_ofs(ca, ca->efc);
        hr_ptes_WORK(ps->efc, ca->efc, hrvo, ca->rot.p);
    }
}

static void hr_econt_work(HR_PSYS *ps) {
    HR_ECONT *ec;
    s32 i;
    s32 pte;

    hrvi[0] = hrvi[1] = hrvi[2] = 0.0f;
    hrvi[3] = hrvo[3] = 1.0f;

    for (i = 0, ec = ps->econ; i < 8; i++, ec++) {
        if (ec->id != 0) {
            hrvo[0] = ec->x;
            hrvo[1] = ec->y;
            hrvo[2] = ec->z;

            pte = hr_efc_why(ec->id);
            if (pte == PTE_ABE) {
                hr_abe_efcWork(ec->id, hrvo);
            } else if (pte == PTE_HATO) {
                hatrot[0] = ec->rx;
                hatrot[1] = ec->ry;
                hatrot[2] = ec->rz;
                hatrot[3] = 1.0f;
                hr_hato_efcWork(ec->id, hrvo, hatrot);
            } else if (ps->efc != NULL) {
                hr_ptes_WORK(ps->efc, ec->id, hrvo, hrvi);
            }
        }
    }
}

static void hr_pcall_work(HR_CALL *ca, HR_PSYS *ps) {
    if (ca->time != 0) {
        ca->time--;
    }

    hr_check_time_move(&ca->pos);
    hr_check_time_move(&ca->rot);
    hr_check_time_move(&ca->lcolor[0]);
    hr_check_time_move(&ca->lcolor[1]);
    hr_check_time_move(&ca->lcolor[2]);

    if (ca->flag & 0x40) {
        hr_check_time_move(&ca->nlw[0]);
    }
    if (ca->flag & 0x80) {
        hr_check_time_move(&ca->nlw[1]);
    }
    if (ca->flag & 0x100) {
        hr_check_time_move(&ca->nlw[2]);
    }

    if (hr_check_time_moveRT(&ca->route)) {
        if (ca->route.moku != ca->rtw.mcn) {
            SetMichinori(&ca->rtw, ca->route.moku);
        }
    }

    if (ca->time == 0) {
        while (hr_pcall_read(ca, ps));
    }
}

void hr_pcall_mkm(sceVu0FVECTOR pos, sceVu0FVECTOR rot, sceVu0FMATRIX m) {
    sceVu0UnitMatrix(m);
    sceVu0RotMatrixZ(m, m, rot[2]);
    sceVu0RotMatrixX(m, m, rot[0]);
    sceVu0RotMatrixY(m, m, rot[1]);
    sceVu0TransMatrix(m, m, pos);
}

static void hr_pcall_workMV(HR_CALL *ca, HR_PSYS *ps) {
    s32 i;

    hr_pcam_spdacc(&ca->pos);
    hr_pcam_spdacc(&ca->rot);
    hr_pall_rot180V(ca->rot.p);

    for (i = 0; i < 3; i++) {
        hr_pcam_spdacc(&ca->lcolor[i]);
        hr_pall_limitV(ca->lcolor[i].p, 1.0f, 0.0f);
    }

    hrpcc = &ca->nlw[0];
    if (ca->flag & 0x40) {
        hr_pcam_spdacc(hrpcc);
        hr_pall_rot180V(hrpcc->p);
    }
    hrpcc++;
    if (ca->flag & 0x80) {
        hr_pcam_spdacc(hrpcc);
        hr_pall_rot180V(hrpcc->p);
    }
    hrpcc++;
    if (ca->flag & 0x100) {
        hr_pcam_spdacc(hrpcc);
        hr_pall_rot180V(hrpcc->p);
    }

    if (ca->flag & 0x20) {
        hr_rt_movework(ca);
    }
    if (ca->efc != 0) {
        hr_call_efcwork(ca, ps);
    }
    if (ca->flag & 0x200) {
        hr_pcall_mkm(ca->pos.p, ca->rot.p, ps->wmat);
    }
}

static void hr_work_nlight(sceVu0FVECTOR n, PCAMSC *pcc) {
    sceVu0FVECTOR v;

    sceVu0ScaleVectorXYZ(v, pcc->p, M_PIf);
    sceVu0DivVectorXYZ(v, v, 180.0f);
    v[3] = 1.0f;

    n[0] = sinf(v[0]);
    n[0] *= cosf(v[1]);
    n[2] = cosf(v[0]);
    n[2] *= cosf(v[1]);
    n[1] = sinf(v[1]);
    n[3] = 0.0f;
}

void hr_pt_light(HR_CALL *ca) {
    if (ca->flag & 0x40u) {
        hr_work_nlight(ca->nlight[0], &ca->nlw[0]);
    }
    if (ca->flag & 0x80) {
        hr_work_nlight(ca->nlight[1], &ca->nlw[1]);
    }
    if (ca->flag & 0x100) {
        hr_work_nlight(ca->nlight[2], &ca->nlw[2]);
    }

    sceVu0NormalLightMatrix(ca->NormalLight, ca->nlight[0], ca->nlight[1], ca->nlight[2]);
    sceVu0LightColorMatrix(ca->LightColor, ca->lcolor[0].p, ca->lcolor[1].p, ca->lcolor[2].p, ca->ambi);
}

void hr_pt_motion(HR_CALL *ca) {
    if (!(ca->flag & 4)) {
        hr_pt_light(ca);
    } else {
        hr_pt_getpos(ca, &ca->hObj->Base.Trans);
        hr_pt_getrot(ca, &ca->hObj->Base.Rot);
        hr_pt_light(ca);
        hr_take_motion(ca->hObj, 0);
    }
}

void hr_pt_motionS(HOT *hObj) {
    // Empty function
}

void hr_pt_motionL(HOT *hObj, sceVu0FMATRIX m, sceVu0FVECTOR nlight, sceVu0FVECTOR lcolor) {
    // Empty function
}

static void hr_psys_motdrawM(HR_CALL *ca, s32 pause, s32 sk) {
    if (pause || !(ca->flag & 4)) {
        if (!(ca->flag & 0x1002)) {
            ca->motret = hr_take_modeldraw(ca->hObj, sk);
            if (ca->flag & 0x400) {
                hr_take_shadow(ca->hObj, ca->hObj->Base.Trans);
            }
        }
    } else {
        if (!(ca->flag & 8)) {
            hr_pt_motion(ca);
        } else {
            ca->flag ^= 8;
        }

        if (!(ca->flag & 0x1002)) {
            ca->motret = hr_take_modeldraw(ca->hObj, 0);
            if (ca->flag & 0x400) {
                hr_take_shadow(ca->hObj, ca->hObj->Base.Trans);
            }
        }

        if (ca->motret != 0) {
            ca->motret = 0;
            ca->motlpi++;
            if (ca->motlp == 0 || --ca->motlp != 0) {
                hr_set_motion(ca, 0);
                if (ca->flag & 0x2000) {
                    pt_motlp_secall(ca);
                }
                ca++; // idk
            } else {
                ca->flag &= -5;
            }
        }
    }
}

void hr_psys_motdraw(HR_PSYS *ps, s32 pause) {
    HR_CALL *ca;
    s32 i;
    s32 n;
    s32 sk;

    if ((s32)ps->call == -1 || hrpt_flag == 0) {
        return;
    }

    hr_take_setM();
    if (ps->flag & 0x20) {
        pause = 1;
        sk = 1;
    } else {
        sk = 0;
    }

    n = 0;
    for (i = 0, ca = ps->call; i < 16; i++, ca++) {
        if (ca->flag != 0) {
            if (ca->hObj != NULL) {
                hr_psys_motdrawM(ca, pause, sk);
            }
            n++;
            if (n >= ps->ncall) {
                break;
            }
        }
    }

    if (hrpt_patch & 0x20) {
        hrpt_patch &= -0x21;
        kzLoadCommonGms();
    }
}

void hr_ptmir_drawMain(HR_PSYS *ps) {
    HR_CALL *ca;
    s32 i;
    s32 j;

    if ((s32) ps->call == -1) {
        return;
    }

    j = 0;
    for (i = 0, ca = ps->call; i < 16; i++, ca++) {
        if (ca->flag == 0) {
            continue;
        }

        if (ca->flag & 0x1000) {
            if (ca->hObj != NULL) {
                ca->motret = hr_take_modeldraw(ca->hObj, 0);
            }
        } else {
            if (!(ca->flag & 0x800)) {
                continue;
            }
            if (ca->hObj != NULL) {
                hr_take_modeldrawMir(ca->hObj, ca->chr);
            }
        }

        j++;
        if (j >= ps->mircnt) {
            break;
        }
    }
}

static void hr_pcam_motion(HR_PSYS *ps) {
    HR_CALL *ca;

    hr_take_setM();
    if (ps->flag & 2) {
        ca = ps->posc;
        if (ca != NULL && ca->hObj != NULL) {
            hr_pt_motion(ca);
            ca->flag |= 8;
        }
    }
    if (ps->flag & 4) {
        ca = ps->intec;
        if (ca != NULL && ca != ps->posc && ca->hObj != NULL) {
            hr_pt_motion(ca);
            ca->flag |= 8;
        }
    }

    if (ps->flag & 2 && !(ps->cam->mp.flag & 4)) {
        ps->posc = NULL;
        ps->flag ^= 2;
    }
    if (ps->flag & 4 && !(ps->cam->mi.flag & 4)) {
        ps->intec = NULL;
        ps->flag ^= 4;
    }
}

void hr_pkey_init(HR_PKEY *key) {
    memset(key, 0, sizeof(HR_PKEY));
}

static void hr_pkey_frmSt(HR_PKEY *key) {
    key->oldtrg = key->trg;
    key->oldlvl = key->lvl;
    key->trg = 0;
    key->lvl = 0;
}

void hr_pkey_trg(HR_PKEY *key, s32 trg) {
    key->trg |= hr_pkey_change(trg);
}

void hr_pkey_lvl(HR_PKEY *key, s32 lvl) {
    s32 i;

    i = hr_pkey_change(lvl);
    key->lvl |= i;
    key->trg |= (i ^ key->oldlvl) & i;
}

void hr_loopse_work(HR_PSYS *ps) {
    HR_PTSE *se;
    s32 i;

    for (i = 0, se = ps->se; i < 4; i++, se++) {
        if (se->flag == 0 || !(se->vol.flag & 1)) {
            continue;
        }

        hr_check_time_moveL(&se->vol);
        if (se->vol.flag & 1) {
            se->vol.spd += se->vol.acc;
            se->vol.p += se->vol.spd;
            if (se->vol.p < 0.0f) {
                se->vol.p = 0.0f;
            } else if (se->vol.p > 4.0f) {
                se->vol.p = 4.0f;
            }
        }

        hSeSetObjVol(se->id, se->vol.p);
    }
}

void hr_psys_work(HR_PSYS *ps) {
    HR_CALL *ca;
    s32 i;
    s32 j;

    if (ps->pmes->flag & 0x80) {
        ps->flag |= 0x40000;
    } else {
        ps->flag &= -0x40001;
    }

    if (ps->key.open) {
        hr_pkey_frmSt(&ps->key);
    }

    hr_loopse_work(ps);
    ca = &ps->sys;
    if (ps->flag & 0x80000000) {
        nk_ptget_klonoa(ca->pos.p, ca->rot.p, &ca->rtw, &ps->rtno, &ps->rtmc, &ps->yuka);;
    }
    ps->flag |= 1;
    hr_pcall_work(ca, ps);
    hr_pcall_workMV(ca, ps);
    ps->flag ^= 1;

    if ((s32)ps->call != -1) {
        j = 0;
        for (i = 0, ca = ps->call; i < 16; i++, ca++) {
            if (ca->flag != 0) {
                hr_pcall_work(ca, ps);
                j++;
                if (j >= ps->ncall) {
                    break;
                }
            }
        }

        j = 0;
        for (i = 0, ca = ps->call; i < 16; i++, ca++) {
            if (ca->flag != 0) {
                hr_pcall_workMV(ca, ps);
                j++;
                if (j >= ps->ncall) {
                    break;
                }
            }
        }
    }

    hr_pfade_work(&ps->fade);
    if (ps->flag & 6) {
        hr_pcam_motion(ps);
    }
    if (ps->flag & 8) {
        hr_pmes_start(ps->pmes);
        ps->flag ^= 8;
    }
    hr_econt_work(ps);
    while (ps->nflag != 0) {
        ps->nflag--;
        pt_flag_work(ps, ps->flagptr[ps->nflag]);
    }
    if (ps->key.open) {
        hr_set_klokey(ps->key.trg, ps->key.lvl);
    }
    if (ps->flag & 0x30000000) {
        hr_pt_reqBGM(ps);
    }
    if (ps->mesp != NULL) {
        pt_work_mesp(ps->mesp);
    }
}

void hr_ptlips_stop(HR_PSYS *ps) {
    HR_CALL *cap;

    if (ps->flag & 0x400) {
        cap = hr_id_search(ps, ps->addrlip[-2]);
        if (cap != NULL && cap->hObj != NULL) {
            hr_take_lipsOFF(cap->hObj);
        }
        ps->flag &= -0x401;
    }
}

void pt_set_skip(HR_PSYS *ps) {
    GameGbl.pause_flag2 = 1;
    hrpt_patch |= 4;
    hrpt_sbgm = 1;
    ps->flag |= 0x20;
    ps->skipcnt = 80;
    hrpt_patch |= 2;
    hPptReset();
    hr_ptbgm_end(ps, 0);
    hr_hse_areaend();
}

void pt_set_skip2(HR_PSYS *ps) {
    HR_CALL *ca;
    s32 i;
    s32 j;

    j = 0;
    for (i = 0, ca = ps->call; i < 16; i++, ca++) {
        if (ca->flag != 0) {
            if (ca->hObj != NULL) {
                hr_take_motionPause(ca->hObj, 1);
            }
            j++;
            if (j >= ps->ncall) {
                break;
            }
        }
    }
}

void pt_set_exit(HR_PSYS *ps, s16 count) {
    ps->ecount0 = count;
    ps->flag |= 0x80;
}

s32 pt_flagj_P(u32 id) {
    s32 mita;

    mita = 0;
    switch (id) {
        case HRPSFG_0706IKA:
            mita = PT0609_S0;
            break;
        case HRPSFG_0609IKA:
            mita = PT0706_S0;
            break;
        case HRPSFG_CHK040B:
            mita = PT0505_S0;
            break;
        case HRPSFG_CHK0505:
            mita = PT040B_S1;
            break;
        case HRPSFG_CHKTREC:
            if (GameGbl.stage_flag & 4) {
                mita++;
            }
            return mita;
        case HRPSFG_CHKTRG:
            if (GameGbl.kpd[0].trg & 0xF0) {
                mita++;
            }
            return mita;
        default:
            return 0;
    }

    if (hr_pflag_checkSV(mita)) {
        return 1;
    } else {
        return 0;
    }
}

s32 hr_strt_check(PT96A *p, u16 no, u32 mc, s32 flag) {
    if (p->us0 == 0 && !flag) {
        return 0;
    }
    if (no != p->us1) {
        return 0;
    }
    if (p->ss0 > 0) {
        if (mc >= p->ui0) {
            return 1;
        }
    } else if (p->ss0 < 0) {
        if (mc <= p->ui0) {
            return 1;
        }
    } else if (mc == p->ui0) {
        return 1;
    }
    return 0;
}

void hr_ptvoice_call(HR_PSYS *ps, HRPMOJI *pmes) {
    s16 *ptr;
    HR_CALL *cap;
    s32 frm;
    s32 cnt;

    if (ps->cntvc != 0) {
        if (pmes->flag & 0x400) {
            pmes->flag &= -0x401;
            cnt = pmes->cntv - 1;
            frm = hPptGetFrame(*ps->addrvc);
            if (pmes->flag & 0x800) {
                pmes->flag &= -0x801;
                frm -= pmes->hasv;
            }
            frm /= 2;
            if (frm < cnt) {
                pmes->wtime = pmes->time = 1;
                pmes->hasv = 0;
            } else {
                pmes->wtime = pmes->time = frm / cnt;
                pmes->hasv = frm * 2 - cnt * pmes->time;
            }
        }

        ps->idvc = hPptReq();
        ps->addrvc++;
        ps->cntvc--;
    }

    ps->flag &= -0x401;
    if (ps->cntlip != 0) {
        ptr = (s16 *)ps->addrlip;
        if (ptr[1] != 0) {
            cap = hr_id_search(ps, ptr[0]);
            if (cap != NULL && cap->hObj != NULL) {
                hr_take_lips(cap->hObj, (s16 *)pt_al_dataB((s32 *)ptr, ptr[1]));
                ps->flag |= 0x400;
            }
        }
        ps->cntlip--;
        ps->addrlip += 2;
    }
}

void hr_ptvoice_stop(HR_PSYS *ps, HRPMOJI *pmes) {
    s32 vid;

    vid = ps->idvc;
    if (vid != -1) {
        hPptStop(vid);
    }
    hr_ptlips_stop(ps);
}

void hr_psys_debugMVP(HPVSEL *pcsel, s32 pause) {
    if (pcsel->ca != NULL) {
        hr_take_motionPause(pcsel->ca->hObj, pause);
    }
}

void hr_psys_debug(HR_PSYS *ps, s32 menu, HPVSEL *pcsel, PCAMS *pcam) {
    HR_CALL *ca;
    s32 rgb;

    switch (menu) {
        case 1:
            rgb = (SysGbl.irqc << 2) & 0xFF;
            hr_crossline(pcam->inte, 32, rgb, rgb, rgb, 1);
            break;
        case 2:
            if (pcsel->ca != NULL) {
                ca = pcsel->ca;
                hr_pt_motionL(ca->hObj, ca->NormalLight, ca->NormalLight[0], ca->LightColor[0]);
            }
            break;
        case 3:
            if (pcsel->ca != NULL) {
                ca = pcsel->ca;
                hr_pt_motion(ca);
            }
            break;
    }
}

void hr_psys_debugP(HR_PSYS *ps, s32 pause) {
    HR_CALL *ca;
    s32 i;

    if ((s32)ps->call == -1) {
        return;
    }

    for (i = 0, ca = ps->call; i < 16; i++, ca++) {
        if (ca->flag != 0 && ca->hObj != NULL) {
            hr_take_motionPause(ca->hObj, pause);
        }
    }
}