#include "harada/hr_main.h"
#include "harada/hr_pflag.h"
#include "harada/hr_pall.h"
#include "harada/hr_pbgm.h"
#include "harada/hr_mirr.h"
#include "harada/hr_bgwk.h"
#include "harada/hr_vpa.h"
#include "harada/hr_anmdt.h"
#include "harada/hr_anmvp.h"
#include "harada/h_vpm2.h"
#include "harada/hr_mapdr.h"
#include "harada/hr_pefc.h"
#include "harada/hr_obcon.h"
#include "hoshino/h_util.h"
#include "okanoyo/okio.h"
#include "hoshino/h_file.h"
#include "nakano/dma.h"
#include "nakano/main.h"

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
s16 hr_resFg = 0;
s16 hr_resSV = 0;
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

void hr_hpmk_init(u32 *top) {
    // TODO
}

u32* func_001028E0(u32 *top) {
    // TODO
}

u32* hr_hpmk_blk(u16 gx, u16 gy, u16 gz, u32 *top) {
    HR_HPXZ *hxz;
    u32 xzcnt;
    u32 i;
    u32 cnt;
    u32 *dw;
    HR_HPYL *hyl;

    xzcnt = top[7];

    for (i = 0, hxz = (HR_HPXZ *)top + 4; i < xzcnt; i++, hxz++) {
        if (hxz->z > gz) {
            return NULL;
        }
        if (hxz->z >= gz) {
            if (hxz->x > gx) {
                return NULL;
            }
            if (hxz->x == gx) {
                break;
            }
        }
    }

    cnt = 0;
    if (i == xzcnt) {
        return NULL;
    }

    dw = top + (hxz->addr >> 2);
    xzcnt = *dw++;
    for (i = 0, hyl = (HR_HPYL *)dw; i < xzcnt; i++, hyl++) {
        if (hyl->y > gy) {
            return NULL;
        }
        if (hyl->y + hyl->cnt > gy) {
            break;
        }
        cnt += hyl->cnt;
    }

    if (i == xzcnt) {
        return NULL;
    }

    return dw + xzcnt + (cnt + (gy - hyl->y)) * 2;
}

u32* hr_hpmk(f32 x, f32 y, f32 z, u32 *top) {
    HR_HPMK *hp;
    u16 gx;
    u16 gy;
    u16 gz; // ?
    f32 f;

    hp = (HR_HPMK *)top;

    f = (x - hp->xs) / hp->gsize;
    if (f < 0.0f || f > hp->gcntx) {
        return NULL;
    }
    gx = f;

    f = (y - hp->ys) / hp->gsizey;
    if (f < 0.0f || f > hp->gcnty) {
        return NULL;
    }
    gy = f;

    f = (z - hp->zs) / hp->gsize;
    if (f < 0.0f || f > hp->gcntz) {
        return NULL;
    }
    gz = f;

    return hr_hpmk_blk(gx, gy, gz, top);
}

u32* hr_hpmk_deb(f32 x, f32 y, f32 z, u32 *top, HR_HPDEB *hpd) {
    // TODO
}

void hr_stage_no(char *name, s32 fg) {
    char *hex[16] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f"};

    strcat(name, hex[(GameGbl.vision / 0x0A00) % 10]);
    strcat(name, hex[(GameGbl.vision / 0x0100) % 10]);
    if (fg == 0) {
        strcat(name, hex[GameGbl.vision >> 4 & 0xF]);
        strcat(name, hex[GameGbl.vision & 0xF]);
    }
}

s32 hr_check_dmy(qword *addr) {
    if ((*addr)[0] == 0x6D6D7564 && (*addr)[1] == 0x363179) { // "dummy16"
        return 1;
    } else {
        return 0;
    }
}

s32 func_00103398(u32 *pack, s32 param_2, s32 param_3) {
    u32 *buff;
    V1100MIR *mir;
    u32 cnt;

    cnt = 0;
    switch (func_0010DB60(param_2 + 0xB00)) {
        case 3:
            if (*pack - 19 > 1) {
                buff = GetFHMAddress(pack, 19);
                if (!hr_check_dmy((qword *)buff)) {
                    mir = hrm1100;
                    if (param_3 == 0) {
                        DecodeVpmTag(buff);
                    } else {
                        mir->addrA = buff;
                        DecodeVpmS(buff, &mir->infoA);
                    }
                    cnt++;
                }

                buff = GetFHMAddress(pack, 20);
                if (!hr_check_dmy((qword *)buff)) {
                    mir = hrm1100;
                    if (param_3 == 0) {
                        DecodeVpmTag(buff);
                    } else {
                        mir->addrB = buff;
                        DecodeVpmS(buff, &mir->infoB);
                    }
                    cnt++;
                }

                if (cnt != 2) {
                    cnt = 0;
                }
            }
            return cnt;
        case 1:
            if (*pack != 19) {
                buff = GetFHMAddress(pack, 19);
                if (!hr_check_dmy((qword *)buff)) {
                    mir = hrm1100;
                    if (param_3 == 0) {
                        DecodeVpmTag(buff);
                    } else {
                        mir->addrA = buff;
                        DecodeVpmS(buff, &mir->infoA);
                    }
                    cnt++;
                }
            }
            return cnt;
        default:
            return cnt;
    }
}

void func_00103508() {
    u32 vision;
    s32 pack_num;
    u32 *pack;
    s32 i;
    s32 j;
    s32 k;
    u32 *buff;
    s32 cnt;

    vision = GameGbl.vision >> 8 & 0x7F;
    pack_num = *hrd_pack;

    for (i = 0; i < pack_num; i++) {
        pack = GetFHMAddress(hrd_pack, i);
        if (*pack > 18) {
            for (j = 0, k = 0; j < 6; j++, k++) {
                buff = GetFHMAddress(pack, k);
                if (hr_check_dmy((qword *)buff)) {
                    break;
                } else {
                    DecodeVpmTag(buff);
                }

            }

            buff = GetFHMAddress(pack, 14);
            if (!hr_check_dmy((qword *)buff)) {
                DecodeVpo2Tag(buff);
            }

            buff = GetFHMAddress(pack, 15);
            if (!hr_check_dmy((qword *)buff)) {
                cnt = *buff;
                for (j = 0; j < cnt; j++) {
                    DecodeVpo2Tag(GetFHMAddress(buff, j));
                }
            }

            buff = GetFHMAddress(pack, 16);
            if (!hr_check_dmy((qword *)buff)) {
                DecodeVpmTag(buff);
            }

            if (*pack != 19 && vision == 11) {
                func_00103398(pack, i, 0);
            }
        }
    }
}

void hrPtSeLoad() {
    char filename[64];
    s32 i;

    strcpy(filename, "host0:../../sdat/ptse");
    hr_stage_no(filename, 1);
    strcat(filename, ".bin");
    hrse_pack = (u32 *)getBuff(0, 0, filename, &i);
}

void hrStageDataLoad(s32 fg) {
    s32 num;
    u32 *buff;

    hrse_pack = (u32 *)-1;
    hrf_pack = (u32 *)-1;
    hrg_pack = (u32 *)-1;
    hrd_pack = (u32 *)-1;
    ppwaku.gms = (u32 *)-1;
    hrpt_gms = (u32 *)-1;
    hr_retry_set();

    hrd_pack = (u32 *)hGetDataAddr(3);
    if (hrd_pack == NULL) {
        hrd_pack = (u32 *)-1;
    }
    if ((s32)hrd_pack == -1) {
        return;
    }

    num = *hrd_pack;
    if (num > 6) {
        num = 6;
    }

    switch (num) {
        case 6:
            hrpt_gms = GetFHMAddress(hrd_pack, 5);
            if (hr_check_dmy((qword *)hrpt_gms)) {
                hrpt_gms = (u32 *)-1;
            }
        case 5:
            hrse_pack = GetFHMAddress(hrd_pack, 4);
            if (hr_check_dmy((qword *)hrse_pack)) {
                hrse_pack = (u32 *)-1;
            }
        case 4:
            hrf_pack = GetFHMAddress(hrd_pack, 3);
            if (hr_check_dmy((qword *)hrf_pack)) {
                hrf_pack = (u32 *)-1;
            }
        case 3:
            hrg_pack = GetFHMAddress(hrd_pack, 2);
            if (hr_check_dmy((qword *)hrg_pack)) {
                hrg_pack = (u32 *)-1;
            }
            num = 2;
        case 2:
            buff = GetFHMAddress(hrd_pack, 1);
            if (!hr_check_dmy((qword *)buff)) {
                ppwaku.gms = buff;
            }
            num = 1;
        case 1:
            hrd_pack = GetFHMAddress(hrd_pack, 0);
            func_00103508();
            break;
        default:
            hrd_pack = (u32 *)-1;
            break;
    }
}

static void hrAreaGmsSet(s32 area) {
    u32 *pack;
    u32 *buff;

    if (area >= *hrg_pack) {
        return;
    }

    pack = GetFHMAddress(hrg_pack, area);
    buff = GetFHMAddress(pack, 0);
    if (!hr_check_dmy((qword *)buff)) {
        nkLoadGms((qword *)buff);
        sceGsSyncPath(0, 0);
    }

    buff = GetFHMAddress(pack, 1);
    if (!hr_check_dmy((qword *)buff)) {
        nkLoadGms((qword *)buff);
        sceGsSyncPath(0, 0);
    }

    buff = GetFHMAddress(pack, 2);
    if (!hr_check_dmy((qword *)buff)) {
        nkLoadGms((qword *)buff);
        sceGsSyncPath(0, 0);
    }
}

static void hrAreaDataInit() {
    hrcntbg = 0;
    hfm_addr = NULL;
    hcm_addr = NULL;
    hrm1100 = (V1100MIR *)-1;
    infovpa.vpoi.data_top = NULL;
    hr_mt_addr = (u32 *)-1;
    hr_obcvpf = (u32 *)-1;
    hr_obcdata = (u32 *)-1;
    hr_obcvpo = (VPOINFO *)-1;
    hrpt_mini_addr = (s32 *)-1;
    hrpt_pack = (u32 *)-1;
}

static void hrAreaDataSet(s32 area) {
    qword *addr;
    u32 *base;
    mINFO *info;
    s32 i;
    s32 j;
    VPOINFO *vinfo;

    if (area >= *hrd_pack) {
        return;
    }

    base = GetFHMAddress(hrd_pack, area);
    if (*base < 19) {
        return;
    }

    for (i = 0, j = 0, info = hrbgi; i < 6; i++, j++, info++) {
        addr = (qword *)GetFHMAddress(base, j);
        if (hr_check_dmy(addr)) {
            break;
        } else {
            DecodeVpmMiniS((u32 *)addr, info);
            hrcntbg++;
        }
    }

    for (i = 0, j = 6; i < 6; i++, j++) {
        if (i >= hrcntbg) {
            hrbgbin[i] = (u32 *)-1;
        } else {
            addr = (qword *)GetFHMAddress(base, j);
            if (hr_check_dmy(addr)) {
                hrbgbin[i] = (u32 *)-1;
            } else {
                hrbgbin[i] = (u32 *)addr;
            }
        }
    }

    addr = (qword *)GetFHMAddress(base, 12);
    if (!hr_check_dmy(addr)) {
        hfm_addr = (u8 *)addr;
        hfm_size = (u32)GetFHMAddress(base, 13) - (u32)addr;
    }

    addr = (qword *)GetFHMAddress(base, 13);
    if (!hr_check_dmy(addr)) {
        hcm_addr = (u8 *)addr;
        hcm_size = (u32)GetFHMAddress(base, 14) - (u32)addr;
    }

    addr = (qword *)GetFHMAddress(base, 14);
    if (!hr_check_dmy(addr)) {
        infovpa.vpoi.data_top = (u32 *)addr;
        DecodeVpo2S(addr, &infovpa.vpoi);
    }

    addr = (qword *)GetFHMAddress(base, 15);
    if (!hr_check_dmy(addr)) {
        hr_obcvpf = (u32 *)addr;
        j = (*addr)[0];
        hr_obcvpo = (VPOINFO *)getBuff(1, j * sizeof(VPOINFO), "obcvpo", &i);
        if ((s32)hr_obcvpo != -1) {
            for (i = 0, vinfo = hr_obcvpo; i < j; i++, vinfo++) {
                addr = (qword *)GetFHMAddress(hr_obcvpf, i);
                DecodeVpo2S((u32 *)addr, vinfo);
            }
        }
    }

    addr = (qword *)GetFHMAddress(base, 16);
    if (!hr_check_dmy(addr)) {
        hr_mt_addr = (u32 *)addr;
        DecodeVpmS((u32 *)addr, &hr_mtexi);
    }

    addr = (qword *)GetFHMAddress(base, 17);
    if (!hr_check_dmy(addr)) {
        hrpt_mini_addr = (s32 *)addr;
    }

    addr = (qword *)GetFHMAddress(base, 18);
    if (!hr_check_dmy(addr)) {
        hrpt_pack = (u32 *)addr;
    }

    if (*base > 19) {
        if (func_0010DB60(GameGbl.vision)) {
            hrm1100 = (V1100MIR *)getBuff(1, sizeof(V1100MIR), "m1100", &i);
            if ((s32)hrm1100 != -1) {
                i = func_00103398(base, GameGbl.vision & 0xFF, 1);
                if (i == 0) {
                    hrm1100 = (V1100MIR *)-1;
                } else if (i == 2) {
                    hrm1100->flag = 3;
                } else {
                    hrm1100->flag = 1;
                }
            }
        }
    }
}

void hr_set_defMVC() {
    mapvw.fog_near = 2000;
    mapvw.fog_far = 20000;
    mapvw.fog_col[0] = 0;
    mapvw.fog_col[1] = 0;
    mapvw.fog_col[2] = 0;
    mapvw.bgt.r = 0;
    mapvw.bgt.g = 0;
    mapvw.bgt.b = 0;
    mapvw.bgu.r = 0;
    mapvw.bgu.g = 0;
    mapvw.bgu.b = 0;

    mapvw.ids = 0x5453424D;
    mapvw.idv = 0x31524556;
    mapvw.bg_spdt = 0;
    mapvw.bg_spds = 0;
    mapvw.map_spdt = 0;
    mapvw.map_spds = 0;
}

void func_00103E00(u32 *param_1) {
    void *dst;
    s32 size;

    if (param_1[0] != 0x5453424D || param_1[1] != 0x31524556) {
        dst = &mapvw.fog_col;
        size = sizeof(MAPVWORK) - 0x10;
        mapvw.ids = 0x5453424D;
        mapvw.idv = 0x31524556;
        mapvw.bg_spdt = 0;
        mapvw.bg_spds = 0;
        mapvw.map_spdt = 0;
        mapvw.map_spds = 0;
    } else {
        dst = &mapvw;
        size = sizeof(MAPVWORK);
    }

    memcpy(dst, param_1, size);
}

void MapConfFileRead() {
    u32 vision;
    u32 flag;

    vision = GameGbl.vision & 0xFF;
    flag = 0;
    if ((s32)hrf_pack != -1 && vision < *hrf_pack) {
        flag = !hr_check_dmy((qword *)GetFHMAddress(hrf_pack, vision));
    }

    if (!flag) {
        hr_set_defMVC();
    } else {
        func_00103E00(GetFHMAddress(hrf_pack, vision));
    }
}

void hrDataLoad() {
    char StName[128];
    char FileName[128];
    char Name[128];
    char BgName[128];
    char AnmName[128];
    qword *addrn;
    char *c;
    s32 i;
    s32 ret;
    s32 len;
    mINFO *info;
    char mName[256];

    strcpy(StName, "host0:./dat/st");
    hr_stage_no(StName, 1);

    strcpy(BgName, StName);
    strcat(BgName, "/bg0");
    len = strlen(BgName) - 1;
    hr_stage_no(BgName, 0);

    strcpy(AnmName, StName);
    strcat(AnmName, "/anm");
    hr_stage_no(AnmName, 0);

    strcpy(FileName, StName);
    strcat(FileName, "/map");
    hr_stage_no(FileName, 0);

    strcpy(Name, BgName);
    strcat(Name, ".gms");
    addrn = (qword *)getBuff(0, 0, Name, &i);
    if ((s32)addrn != -1) {
        nkLoadGms(addrn);
        sceGsSyncPath(0, 0);
        freeBuff(0, 0, Name);
    }

    strcpy(Name, BgName);
    strcat(Name, ".vpm");
    c = Name + len;
    hrcntbg = 0;
    for (i = 0, info = hrbgi; i < 6; i++, info++) {
        *c = i + '0';
        addrn = (qword *)getBuff(0, 0, Name, &ret);
        if ((s32)addrn == -1) {
            break;
        } else {
            DecodeVpmMini((u32 *)addrn, info);
        }
        hrcntbg++;
    }

    strcpy(Name, BgName);
    strcat(Name, ".bin");
    for (i = 0; i < 6; i++) {
        if (i >= hrcntbg) {
            hrbgbin[i] = (u32 *)-1;
        } else {
            *c = i + '0';
            hrbgbin[i] = (u32 *)getBuff(0, 0, Name, &ret);
        }
    }

    strcpy(Name, FileName);
    strcat(Name, ".mrf");
    hfm_addr = NULL;
    addrn = (qword *)getBuff(0, 0, Name, &ret);
    if ((s32)addrn != -1) {
        hfm_addr = (u8 *)addrn;
        hfm_size = ret;
    }

    strcpy(Name, FileName);
    strcat(Name, ".mrc");
    hcm_addr = NULL;
    addrn = (qword *)getBuff(0, 0, Name, &ret);
    if ((s32)addrn != -1) {
        hcm_addr = (u8 *)addrn;
        hcm_size = ret;
    }

    hrm1100 = (V1100MIR *)-1;
    ret = func_0010DB60(GameGbl.vision);
    if (ret) {
        hrm1100 = (V1100MIR *)getBuff(1, sizeof(V1100MIR), "m1100", &i);
        if ((s32)hrm1100 != -1) {
            strcpy(mName, StName);
            strcat(mName, "/m");
            hr_stage_no(mName, 0);
            len = strlen(mName);
            strcat(mName, "a.vpm");
            hrm1100->addrA = (u32 *)getBuff(0, 0, mName, &i);
            if (ret & 2) {
                mName[len] = 'b';
                hrm1100->addrB = (u32 *)getBuff(0, 0, mName, &i);
            } else {
                hrm1100->addrB = NULL;
            }
            if ((s32)hrm1100->addrA == -1 || (s32)hrm1100->addrB == -1) {
                hrm1100 = (V1100MIR *)-1;
            } else {
                DecodeVpm2(hrm1100->addrA, &hrm1100->infoA);
                if (ret & 2) {
                    DecodeVpm2(hrm1100->addrB, &hrm1100->infoB);
                }
                hrm1100->flag = ret;
            }
        }
    }

    strcpy(Name, AnmName);
    strcat(Name, ".gms");
    addrn = (qword *)getBuff(0, 0, Name, &i);
    if ((s32)addrn != -1) {
        nkLoadGms(addrn);
        sceGsSyncPath(0, 0);
        freeBuff(0, 0, Name);
    }

    strcpy(Name, AnmName);
    strcat(Name, ".vpa");
    infovpa.vpoi.data_top = (u32 *)getBuff(0, 0, Name, &i);
    if ((s32)infovpa.vpoi.data_top == -1) {
        infovpa.vpoi.data_top = NULL;
    } else {
        DecodeVpo2(infovpa.vpoi.data_top, &infovpa.vpoi);
    }

    strcpy(Name, FileName);
    strcat(Name, ".mt");
    hr_mt_addr = (u32 *)getBuff(0, 0, Name, &i);
    if ((s32)hr_mt_addr != -1) {
        DecodeVpm2(hr_mt_addr, &hr_mtexi);
    }

    hrLoadObcData(StName);
    hrPtMiniDataLoad(StName);
    strcpy(Name, StName);
    strcat(Name, "hrp");
    hr_stage_no(Name, 0);
    strcat(Name, "u.fhm");
    hrpt_pack = (u32 *)getBuff(0, 0, Name, &i);
}

void func_00104508() {
    s32 vision;

    vision = GameGbl.vision & 0xFF;
    if ((s32)hrg_pack != -1) {
        hrAreaGmsSet(vision);
    }
    hrAreaDataInit();
    if ((s32)hrg_pack != -1) {
        hrAreaDataSet(vision);
    }
}

static void func_00104568() {
    // Empty function
}

static void func_00104570() {
    // Empty function
}

void hrAreaEnd() {
    hr_pflag_EndCopy(ptflag_ar, ptflag_st);
    hr_pflag_initAr();
}

void hrStageEnd() {
    hrAreaEnd();
    hr_pflag_EndCopy(ptflag_st, ptflag_buff);
    hr_pflag_initAr();
}

void hr_set_vpmfog(vpmINFO *info) {
    info->fog_near = mapvw.fog_near;
    info->fog_far = mapvw.fog_far;
    info->fog_col[0] = mapvw.fog_col[0];
    info->fog_col[1] = mapvw.fog_col[1];
    info->fog_col[2] = mapvw.fog_col[2];
}

s32* hr_get_draw2() {
    return (s32 *)(GameGbl.fr & 1 ? &GameGbl.db.draw02 : &GameGbl.db.draw12);
}

void hr_scrst_init(HRSCRST *st) {
    memset(st, 0, sizeof(HRSCRST));
}

void hr_scrst_set(HRSCRST *st, s16 s, s16 t) {
    if (s != 0)
        st->spds = 1.0f / s;
    if (t != 0)
        st->spdt = 1.0f / t;
}

void hr_scrst_work(HRSCRST *st) {
    st->s += st->spds;
    while (st->s >= 1.0f) {
        st->s -= 1.0f;
    }
    while (st->s <= -1.0f) {
        st->s += 1.0f;
    }

    st->t += st->spdt;
    while (st->t >= 1.0f) {
        st->t -= 1.0f;
    }
    while (st->t <= -1.0f) {
        st->t += 1.0f;
    }
}

void hrInitWork() {
    hrpt_patch = 0;
    hr_bginit();
    hr_anmVPM_allinit();
    hr_vision_anmVPM_set();
    hr_mir_set();
    hr_anmVPA_init();
    hr_ptcall_getbuff();
    hr_init_obcon();
    hr_set_vpmfog(&VpmInfo);
    hrPtMini_init();
    if (hrpt_flag == 0) {
        hr_pt_fclear();
    }
    hr_scrst_init(&hrmapst);
    hr_scrst_init(&hrbgst);
    hr_scrst_set(&hrmapst,mapvw.map_spds,mapvw.map_spdt);
    hr_scrst_set(&hrbgst,mapvw.bg_spds,mapvw.bg_spdt);
    hr_pfade_sinit();
    hr_resFg = 0;
    hrpt_sbgm = 0;
}

void hrInitWorkDeb() {
    hr_bginit();
    hr_anmVPM_allinit();
    hr_vision_anmVPM_set();
    hr_mir_set();
    hr_anmVPA_init();
    hr_ptcall_getbuff();
    hr_set_vpmfog(&VpmInfo);
    if ((s32)hr_mt_addr != -1) {
        hr_set_vpmfog(&hr_mtexi);
    }
    hr_scrst_init(&hrmapst);
    hr_scrst_init(&hrbgst);
    hr_scrst_set(&hrmapst,mapvw.map_spds,mapvw.map_spdt);
    hr_scrst_set(&hrbgst,mapvw.bg_spds,mapvw.bg_spdt);
    hr_pfade_sinit();
    hr_resFg = 0;
    hrpt_sbgm = 0;
}

void hrMainWork() {
    h_vp_init(&cvpm,GameGbl.cam.scr_z,0.0f,65536.0f,1024.0f,1024.0f,1024.0f,0);
    hr_bg_workclip();
    if (GameGbl.pause_flag == 0) {
        if (!hr_pt_check()) {
            hrpt_sbgm = 0;
        }
        hr_anmVPM_work();
        hr_mir_work();
        hr_anmVPA_work();
        hr_scrst_work(&hrmapst);
        hr_scrst_work(&hrbgst);
    }
}

void hrMainDraw() {
    s32 flag;

    h_vp_init(&cvpm,GameGbl.cam.scr_z,0.0f,65536.0f,1024.0f,1024.0f,1024.0f,0);
    hrSetMapREG();
    hrDrawBG();
    hrDrawAnmMap();
    hrDrawMap();
    if (!hr_pt_check() || hrpt_vt != 0) {
        flag = 1;
        nkVT_Exec();
    } else {
        flag = 0;
    }
    hrSetMapREG();
    if (flag && (s32)hr_mt_addr != -1) {
        hrDrawMTex();
    }
    hr_anmVPA_draw();
    nkP1Flush(p1_ot);
    hr_objtype = 0;
}

void hr_retry_set() {
    hr_pflag_initSt();
    hr_restart_clear();
    hr_resSV = 0;
}

void hr_restart_clear() {
    hr_resTmp = 0;
    hr_resKeep = 0;
    hr_resFg = 0;
}

void hr_restart_Tmpclear() {
    hr_pflag_initAr();
    hr_resTmp = 0;
    hr_resFg++;
}

void hr_restart_set(s32 id) {
    hr_resTmp |= 1 << id;
}

s32 hr_restart_check(s32 id) {
    id = 1 << id;

    if ((hr_resKeep & id) || (hr_resTmp & id)) {
        return 1;
    } else {
        return 0;
    }
}
