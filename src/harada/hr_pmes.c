#include "harada/hr_pmes.h"
#include "harada/hr_pall.h"
#include "harada/hr_main.h"
#include "okanoyo/okio.h"
#include "nakano/main.h"
#include "nakano/dma.h"

static s32 hr_pmes_wsize(u16 code, s32 *dx) {
    switch (code) {
        case 0x08:
        case 0x22:
        case 0x25:
        case 0x44:
            *dx = (*dx * 5) / 0x14;
            return 5;
        case 0x35:
        case 0x3E:
        case 0x40:
        case 0x46:
            *dx = (*dx * 6) / 0x14;
            return 6;
        case 0x09:
        case 0x23:
        case 0x2B:
        case 0x2D:
        case 0x3F:
        case 0x42:
        case 0x45:
        case 0x47:
        case 0x48:
            *dx = (*dx * 7) / 0x14;
            return 7;
        case 0x0B:
        case 0x1F:
        case 0x2C:
        case 0x4D:
            *dx = (*dx << 3) / 0x14;
            return 8;
        case 0x04:
        case 0x05:
        case 0x1A:
        case 0x1C:
        case 0x1E:
        case 0x21:
        case 0x24:
        case 0x27:
        case 0x49:
        case 0x4A:
        case 0x4B:
            *dx = (*dx * 9) / 0x14;
            return 9;
        case 0x0A:
        case 0x11:
        case 0x18:
        case 0x1D:
        case 0x43:
            *dx = (*dx * 11) / 0x14;
            return 11;
        case 0x00:
        case 0x02:
        case 0x14:
        case 0x17:
        case 0x19:
            *dx = (*dx * 12) / 0x14;
            return 12;
        case 0x03:
        case 0x06:
        case 0x07:
        case 0x0D:
        case 0x15:
            *dx = (*dx * 13) / 0x14;
            return 13;
        case 0x0E:
        case 0x10:
        case 0x26:
        case 0x30:
        case 0x4C:
            *dx = (*dx * 14) / 0x14;
            return 14;
        case 0xc:
            *dx = (*dx * 0xf) / 0x14;
            return 15;
        case 0x4e:
            *dx = (*dx * 0x11) / 0x14;
            return 17;
        case 0x16:
            *dx = (*dx * 0x12) / 0x14;
            return 18;
        default:
            *dx = *dx / 2;
            return 10;
    }
}

static void hr_pmes_init_st(HRPMOJI *pmes) {
    pmes->time = 1;
    pmes->dcnt = 0;
    if (pmes->flag & 0x20) {
        pmes->flag ^= 0x20;
    }
    pmes->acnt = 0;
    if ((s32)pmes->mema != -1) {
        memset(pmes->mema, 0, 0x300);
    }
}

void hr_pmes_ns_st(HRPMOJI *pmes) {
    pmes->skipcnt = 60;
}

void hr_pmes_ns_ai(HRPMOJI *pmes) {
    pmes->skipcnt = 23;
}

void hr_pmes_init(HRPMOJI *pmes) {
    pmes->wtime = 5;
    pmes->page = NULL;
    pmes->dtop = NULL;
    pmes->stop = 0;
    pmes->count = 0;
    pmes->flag = 0;
    pmes->mode = HRPMJ_NOPRINT;
    pmes->mema = getmem(0x300);
    hr_pmes_init_st(pmes);
    pmes->flag |= 0x40000;
    pmes->stop++;
    hr_pmes_ns_st(pmes);
}

void hr_pmes_exit(HRPMOJI *pmes) {
    if ((s32)pmes->mema != -1) {
        freemem(pmes->mema);
    }
    pmes->stop = 1;
}

void hr_pmes_set(HRPMOJI *pmes, s32 *addr) {
    hr_pmes_init_st(pmes);
    pmes->next = (u16 *)addr;
    pmes->read = (u16 *)addr;
    pmes->page = (u16 *)addr;
    pmes->dtop = (u16 *)addr;
    pmes->mode = HRPMJ_WAITSTART;
}

void hr_pmes_start(HRPMOJI *pmes) {
    pmes->mode = HRPMJ_NUM0;
    pmes->flag &= -0x81;
}

void hr_pmes_reta(HRPMOJI *pmes) {
    if (pmes->flag & 0x40) {
        pmes->flag |= 0x100;
    }
}

void hr_pmes_window(HRPMOJI *pmes, s32 onoff) {
    pmes->stop = onoff;
}

static void hr_pmes_key(HRPMOJI *pmes) {
    kPadDATA *kpd;

    kpd = &GameGbl.kpd[0];
    pmes->flag &= -0x2004;
    if (pmes->flag & 0x40) {
        if (pmes->flag & 0x100) {
            pmes->flag ^= 0x100;
            pmes->flag |= 1;
        }
    } else {
        if (kpd->trg & 0x60) {
            pmes->flag |= 1;
        }
        if (pmes->skipcnt == 0 && kpd->trg & 0x20 && !(pmes->flag & 0x20000)) {
            pmes->flag |= 0x2000;
            hr_pmes_ns_ai(pmes);
        }
        if (kpd->lvl & 0x60) {
            pmes->flag |= 2;
        }
    }
}

void hr_pmes_waitstp(HRPMOJI *pmes) {
    pmes->time = 1;
    pmes->mode = HRPMJ_WAITSTP;
}

static s32 hr_pmes_acheck(HRPMOJI *pmes) {
    if ((s32)pmes->mema == -1 || pmes->acnt == 0) {
        return 1;
    } else {
        return pmes->mema[pmes->acnt - 1] >= 128;
    }
}

static void hr_pmes_waitalpha(HRPMOJI *pmes) {
    pmes->time = 1;
    pmes->mode = HRPMJ_WAITALPHA;
}

static void hr_pmes_init_wbot(HRPMOJI *pmes, s32 flag) {
    if (flag == 0x80) {
        if (hr_pmes_acheck(pmes)) {
            pmes->flag |= flag;
            hr_pmes_waitstp(pmes);
        } else {
            hr_pmes_waitalpha(pmes);
        }
    } else {
        pmes->flag |= flag;
        pmes->mode = HRPMJ_WAITBOT;
    }
    pmes->read++;
}

static void hr_pmes_init_chgp(HRPMOJI *pmes) {
    pmes->mode = HRPMJ_CHGPAGE;
    pmes->next = ++pmes->read;
    pmes->time = 1;
}

static s32 hr_pmes_readM(HRPMOJI *pmes) {
    s32 ret;

    pmes->dcnt++;
    ret = 0;

    if (!(*pmes->read & -0x8000)) {
        if ((s32)pmes->mema != -1) {
            pmes->acnt++;
        }
        pmes->read++;
        if (pmes->flag & 0x2000) {
            ret++;
            if ((s32)pmes->mema != -1) {
                pmes->mema[pmes->acnt - 1] = 128;
            }
        }
        return ret;
    } else {
        switch (*pmes->read) {
            case (u16)MES_END:
                hr_pmes_init_wbot(pmes, 16);
                break;
            case (u16)MES_KEY:
                hr_pmes_init_wbot(pmes, 4);
                break;
            case (u16)MES_WAIT:
                hr_pmes_init_wbot(pmes, 128);
                break;
            case (u16)MES_PAGE:
                hr_pmes_init_chgp(pmes);
                break;
            case (u16)MES_CLUT:
                pmes->read += 2;
                ret++;
                break;
            case (u16)MES_GYOU:
                pmes->read++;
                ret++;
                break;
            case (u16)MES_ITEM:
                pmes->read += 2;
                if ((s32)pmes->mema != -1) {
                    pmes->acnt++;
                }
                if (pmes->flag & 0x2000) {
                    ret++;
                    if ((s32)pmes->mema != -1) {
                        pmes->mema[pmes->acnt - 1] = 128;
                    }
                }
                break;
            case (u16)MES_YN:
                pmes->read++;
                ret++;
                break;
            case (u16)MES_TBOX:
                pmes->read++;
                break;
            case (u16)MES_SPAC:
                pmes->read += 2;
                ret++;
                break;
            case (u16)MES_TIME:
                pmes->read++;
                if (pmes->flag & 0x2000) {
                    ret++;
                } else {
                    pmes->time += *pmes->read;
                }
                pmes->read++;
                break;
            case (u16)MES_SPD:
                pmes->read++;
                pmes->wtime = pmes->time = *pmes->read;
                pmes->read++;
                ret++;
                break;
            case (u16)MES_DSPD:
                pmes->wtime = pmes->time = 5;
                pmes->read++;
                ret++;
                break;
            case (u16)MES_VCST:
                pmes->flag &= -0x2001;
                pmes->flag |= 0x4000;
                pmes->read++;
                pmes->flag |= 0x600;
                pmes->wtime = pmes->time = 5;
                pmes->cntv = *pmes->read;
                pmes->read++;
                pmes->hasv = 0;
                if (*pmes->read == (u16)-0x10) {
                    pmes->flag |= 0x800;
                    pmes->read++;
                    pmes->hasv = *pmes->read;
                    pmes->read++;
                }
                ret++;
                break;
            case (u16)MES_VCEND:
                pmes->read++;
                pmes->wtime = pmes->time = 5;
                if (pmes->flag & 0x2000) {
                    pmes->hasv = 1;
                    pmes->flag &= -0x2001;
                }
                if (pmes->hasv != 0) {
                    pmes->time = pmes->hasv;
                    pmes->hasv = 0;
                    pmes->flag |= 0x8000;
                } else {
                    pmes->flag &= -0x4001;
                    ret++;
                }
                break;
        }
        return ret;
    }
}

static void hr_pmes_a80(HRPMOJI *pmes) {
    s32 i;
    u32 *mema;

    for (mema = pmes->mema, i = 0; i < pmes->acnt; mema++, i++) {
        *mema = 128;
    }
}

static void hr_pmes_read_data(HRPMOJI *pmes) {
    hr_pmes_key(pmes);
    if (pmes->time != 0) {
        pmes->time--;
    }
    if (pmes->flag & 0x2000) {
        pmes->time = 0;
        if (pmes->flag & 0xC000) {
            pmes->flag &= -0xC001;
            pmes->flag |= 0x10000;
        }
        if ((s32)pmes->mema != -1) {
            hr_pmes_a80(pmes);
        }
    }
    if (pmes->time == 0) {
        pmes->time = pmes->wtime;
        if (pmes->flag & 0x8000) {
            pmes->flag &= -0xC001;
        }
        while (hr_pmes_readM(pmes)) {

        }
    }
}

static void hr_pmes_wait_bot(HRPMOJI *pmes) {
    hr_pmes_key(pmes);
    if (!(pmes->flag & 1)) {
        return;
    }

    pmes->flag ^= 1;
    if (hr_pmes_acheck(pmes)) {
        pmes->flag |= 0x80;
        if (pmes->flag & 0x10) {
            pmes->flag ^= 0x10;
            pmes->flag |= 0x20;
            pmes->mode = HRPMJ_NOPRINT;
        } else if (pmes->flag & 0x4) {
            pmes->flag ^= 4;
            hr_pmes_waitstp(pmes);
        } else {
            pmes->flag &= -0x9;
            pmes->mode = HRPMJ_CHGPAGE;
        }
    }
}

static void hr_pmes_change_page(HRPMOJI *pmes) {
    hr_pmes_init_st(pmes);
    pmes->read = pmes->next;
    pmes->page = pmes->next;
    pmes->mode = HRPMJ_NUM0;
}

void hr_pmes_wait_alpha(HRPMOJI *pmes) {
    if (hr_pmes_acheck(pmes)) {
        pmes->flag |= 0x80;
        hr_pmes_waitstp(pmes);
    }
}

void hr_pmes_countA(HRPMOJI *pmes) {
    s32 i;
    u32 *mema;

    for (mema = pmes->mema, i = 0; i < pmes->acnt; i++, mema++) {
        if (*mema < 128) {
            *mema += 4;
        }
    }
}

static void (*hrMestbl[4])(HRPMOJI *) = {
    hr_pmes_read_data,
    hr_pmes_wait_bot,
    hr_pmes_change_page,
    hr_pmes_wait_alpha
};

void hr_pmes_work(HRPMOJI *pmes) {
    if (pmes->skipcnt != 0) {
        pmes->skipcnt--;
    }
    if (pmes->stop) {
        return;
    }
    pmes->count++;
    if (pmes->mode >= 3) {
        hrMestbl[pmes->mode - 3](pmes);
        if (pmes->acnt != 0 && (s32)pmes->mema != -1) {
            hr_pmes_countA(pmes);
        }
    }
}

static void hr_pmes_waku_draw(HRPMWAKU *waku) {
    ATR_MWAKU *pp;
    s32 flag;

    flag = (s32)waku->gms != -1;
    pp = (ATR_MWAKU *)0x70000000;
    if (!(waku->flag & 1) && flag) {
        nkDmaSendVif1((qword *)waku->gms);
        waku->flag |= 1;
    }

    pp->dmatag[0] = 0x70000000 | (sizeof(ATR_MWAKU) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    ((u64 *)pp->gif0)[0] = SCE_GIF_SET_TAG(4, 0, 0, 0, 0, 1);
    pp->gif0[2] = SCE_GIF_PACKED_AD;
    pp->gif0[3] = 0;
    ((u64 *)pp->test)[0] = SCE_GS_SET_TEST_2(1, 0, 0, 3, 0, 0, 1, 2);
    ((u64 *)pp->test)[1] = SCE_GS_TEST_2;
    ((u64 *)pp->alpha)[0] = SCE_GS_SET_ALPHA_2(0, 1, 0, 1, 0);
    ((u64 *)pp->alpha)[1] = SCE_GS_ALPHA_2;
    ((u64 *)pp->xyofs)[0] = *(u64 *)&(((sceGsDrawEnv2 *)hr_get_draw2())->xyoffset2);
    ((u64 *)pp->xyofs)[1] = SCE_GS_XYOFFSET_2;
    ((u64 *)pp->tex0)[0] = SCE_GS_SET_TEX0_2(0x2300, 10, 19, 10, 7, 1, 0, 0x2440, 0, 0, 0, 1);
    ((u64 *)pp->tex0)[1] = SCE_GS_TEX0_2;

    if (flag) {
        ((u64 *)pp->gif1)[0] = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    } else {
        ((u64 *)pp->gif1)[0] = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 0, 0, 1, 0, 1, 1, 0), 0, 3);
    }
    pp->gif1[2] = SCE_GS_RGBAQ << 0 | SCE_GS_UV << 4 | SCE_GS_XYZ2 << 8;
    pp->gif1[3] = 0;
    if (!flag) {
        pp->rgb0[0] = pp->rgb0[1] = 0;
        pp->rgb0[2] = 255;
        pp->rgb0[3] = 64;
    } else {
        pp->rgb0[0] = pp->rgb0[1] = pp->rgb0[2] = pp->rgb0[3] = 128;
    }
    pp->uv0[0] = pp->uv0[1] = 8;
    pp->xyz0[0] = 0x6D00;
    pp->xyz0[1] = 0x82C0;
    pp->xyz0[2] = -1;
    pp->xyz0[3] = 0;
    if (!flag) {
        pp->rgb1[0] = pp->rgb1[1] = 0;
        pp->rgb1[2] = 255;
        pp->rgb1[3] = 64;
    } else {
        pp->rgb1[0] = pp->rgb1[1] = pp->rgb1[2] = pp->rgb1[3] = 128;
    }
    pp->uv1[0] = 0x2680;
    pp->uv1[1] = 0x800;
    pp->xyz1[0] = pp->xyz0[0] + 0x2600;
    pp->xyz1[1] = pp->xyz0[1] + 0x400;
    pp->xyz1[2]= -1;
    pp->xyz1[3] = 0;

    sceGsSyncPath(0, 0);
    *((u16 *)&DmaChGIF->chcr) = 4;
    sceDmaSend(DmaChGIF, (u32)pp | 0x80000000);
}

static void hr_pmes_sprite(ATR_MMOJI *moji, s32 x, s32 y, s32 w, s32 v0, s32 no, s32 fw, s32 alpha, s32 dx, s32 dy) {
    s32 ws;

    // TODO
}

static s32 hr_pmes_mojiAlpha(HRPMOJI *pmes, s32 alpha, s32 count) {
    if ((s32)pmes->mema == -1 || pmes->acnt == 0) {
        return alpha;
    } else {
        if (count >= pmes->acnt) {
            return alpha;
        } else {
            return pmes->mema[count];
        }
    }
}

static qword* hr_pmes_moji_drawM(HRPMOJI *pmes, ATR_MMOJIH *pp, ATR_MMOJI *pm, s32 x0, s32 y0, s32 alpha, s32 dx, s32 dy) {
    s32 i;
    u16 *ptr;
    s32 x;
    s32 y;
    s32 count;
    s32 flag;
    s32 fw;
    s32 a;
    s32 dx0;

    dx0 = dx;
    x = x0;
    y = y0;
    flag = 0;
    ptr = pmes->page;
    for (i = 0, count = 0; i < pmes->dcnt; i++) {
        if (!(*ptr & 0x8000)) {
            a = hr_pmes_mojiAlpha(pmes, alpha, count);
            dx = dx0;
            fw = hr_pmes_wsize(*ptr, &dx);
            hr_pmes_sprite(pm, x, y, 20, 96, *ptr, fw, a, dx, dy);
            x += fw + dx + 1;
            if (*ptr == 0x44) {
                x += 4;
            }
            count++;
            pm++;
            ptr++;
        } else {
            switch (*ptr) {
                case (u16)MES_SPD:
                case (u16)MES_TIME:
                case (u16)MES_CLUT:
                case (u16)MES_TBOX:
                    ptr += 2;
                    break;
                case (u16)MES_VCST:
                    ptr += 2;
                    if (*ptr == (u16)MES_VCTIME) {
                        ptr += 2;
                    }
                    break;
                case (u16)MES_GYOU:
                    x = x0;
                    y += 14;
                    ptr++;
                    break;
                case (u16)MES_SPAC:
                    ptr++;
                    if (*ptr != 0) {
                        x += *ptr;
                    } else {
                        x += 95;
                    }
                    ptr++;
                    break;
                case (u16)MES_ITEM:
                    ptr++;
                    a = hr_pmes_mojiAlpha(pmes, alpha, count);
                    hr_pmes_sprite(pm, x, y, 80, 0, *ptr, 80, a, 0, 0);
                    x += 85;
                    count++;
                    pm++;
                    ptr++;
                    break;
                case (u16)MES_END:
                    flag++;
                    break;
                default:
                    ptr++;
                    break;
            }

            if (flag) {
                break;
            }
        }
    }

    if (count != 0) {
        pp->gif1[0] += count * 2;
        pp->dmatag[0] += count * 6;
    }

    return &pm->uv0;
}

static void hr_pmes_kas_drawS(HRPMOJI *pmes, ATR_MKAS *pk) {
    s32 y;

    ((u64 *)pk->gif0)[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
    pk->gif0[2] = SCE_GIF_PACKED_AD;
    pk->gif0[3] = 0;
    ((u64 *)pk->tex0)[0] = SCE_GS_SET_TEX0_2(0x2460, 8, 0x14, 5, 9, 1, 0, 0x25e4, 0, 0, 0, 1);
    ((u64 *)pk->tex0)[1] = SCE_GS_TEX0_2;

    ((u64 *)pk->gif1)[0] = SCE_GIF_SET_TAG(2, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    pk->gif1[2] = SCE_GS_RGBAQ << 0 | SCE_GS_UV << 4 | SCE_GS_XYZ2 << 8;
    pk->gif1[3] = 0;
    y = pmes->count >> 2 & 0xF;
    if (y >= 8) {
        y = 16 - y;
    }
    pk->rgb0[0] = pk->rgb0[1] = pk->rgb0[2] = pk->rgb0[3] = 128;
    pk->uv0[0] = 0x208;
    pk->uv0[1] = 0x1748;
    pk->xyz0[0] = 0x91A0;
    pk->xyz0[1] = ((y + 0xCC) << 4) + 0x7900;
    pk->xyz0[2] = -1;
    pk->xyz0[3] = 0;
    pk->rgb1[0] = pk->rgb1[1] = pk->rgb1[2] = pk->rgb1[3] = 128;
    pk->uv1[0] = 8;
    pk->uv1[1] = 0x1548;
    pk->xyz1[0] = 0x8FA0;
    pk->xyz1[1] = ((y + 0xCC) << 4) + 0x7800;
    pk->xyz1[2] = -1;
    pk->xyz1[3] = 0;
}

static void hr_pmes_moji_h(ATR_MMOJIH *pp, s32 id) {
    pp->dmatag[0] = 0x70000000 | (sizeof(ATR_MMOJIH) / sizeof(qword) - 1);
    pp->dmatag[1] = pp->dmatag[2] = pp->dmatag[3] = 0;
    ((u64 *)pp->gif0)[0] = SCE_GIF_SET_TAG(3, 0, 0, 0, 0, 1);
    pp->gif0[2] = SCE_GIF_PACKED_AD;
    pp->gif0[3] = 0;
    if (id == 0) {
        ((u64 *)pp->tex0)[0] = SCE_GS_SET_TEX0_2(0x2460, 8, 0x14, 9, 9, 1, 0, 0x25E1, 0, 0, 0, 1);
    } else {
        ((u64 *)pp->tex0)[0] = SCE_GS_SET_TEX0_2(0x2460, 8, 0x14, 9, 9, 1, 0, 0x25E5, 0, 0, 0, 1);
    }
    ((u64 *)pp->tex0)[1] = SCE_GS_TEX0_2;
    ((u64 *)pp->test)[0] = SCE_GS_SET_TEST_2(1, 0, 0, 3, 0, 0, 1, 2);
    ((u64 *)pp->test)[1] = SCE_GS_TEST_2;
    ((u64 *)pp->alpha)[0] = SCE_GS_SET_ALPHA_2(0, 1, 0, 1, 0);
    ((u64 *)pp->alpha)[1] = SCE_GS_ALPHA_2;

    ((u64 *)pp->gif1)[0] = SCE_GIF_SET_TAG(0, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_SPRITE, 0, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    pp->gif1[2] = SCE_GS_UV << 0 | SCE_GS_RGBAQ << 4 | SCE_GS_XYZ2 << 8;
    pp->gif1[3] = 0;
}

static void hr_pmes_moji_draw(HRPMOJI *pmes, s32 id) {
    ATR_MMOJIH *pp;
    ATR_MKAS *pk;
    s32 flag;

    flag = 0;
    if (!(pmes->flag & 0x1000) && (s32)pmes->font != -1) {
        flag = 1;
        nkDmaSendVif1((qword *)pmes->font);
        pmes->flag |= 0x1000;
    }
    pp = (ATR_MMOJIH *)0x700000D0;
    hr_pmes_moji_h(pp, 0);
    pk = (ATR_MKAS *)hr_pmes_moji_drawM(pmes, pp, (ATR_MMOJI *)(pp + 1), 40, 167, 128, 0, 0);
    if (id == 2) {
        pp->dmatag[0] += 9;
        pp->gif1[0] ^= 0x8000;
        hr_pmes_kas_drawS(pmes, pk);
    }
    FlushCache(0);
    sceDmaSync(DmaChGIF, 0, 0);
    *((vu16 *)&DmaChGIF->chcr) = 4;
    if (flag) {
        sceDmaSync(DmaChVIF1, 0, 0);
    }
    sceDmaSend(DmaChGIF, (u32)pp | 0x80000000);
}

void hr_pmes_draw(HRPMOJI *pmes) {
    s32 chkm;

    if (pmes->stop) {
        return;
    }

    hr_pmes_waku_draw(&ppwaku);
    if (pmes->dcnt == 0) {
        chkm = 0;
    } else {
        switch (pmes->mode) {
            case HRPMJ_WAITSTP:
            case HRPMJ_NUM0:
            case HRPMJ_WAITALPHA:
                chkm = 1;
                break;
            case HRPMJ_WAITBOT:
                chkm = 2;
                break;
            default:
                chkm = 0;
                break;
        }
    }

    if (chkm) {
        hr_pmes_moji_draw(pmes, chkm);
    }
    sceGsSyncPath(0, 0);
}

static s32 hr_mesp_calcXY(HRMESP *mesp, s32 *px, s32 *py, s32 *scale) {
    HR_CALL *ca;

    ca = mesp->ca;
    *px = ca->pos.p[0];
    *py = ca->pos.p[1];
    if (ca->pos.p[2] == 1.0f) {
        *scale = 0;
    } else {
        *scale = (s32)((ca->pos.p[2] - 1.0f) * 20.0f);
    }
}

void hr_mesp_draw(HRMESP *mesp, HRPMOJI *pmes) {
    HRPMOJI mj;
    ATR_MMOJIH *pp;
    ATR_MMOJI *pm;
    s32 i;
    s32 flag;
    s32 x;
    s32 y;
    s32 s;

    pp = (ATR_MMOJIH *)0x70000000;
    if ((s32)pmes->font != -1) {
        nkDmaSendVif1((qword *)pmes->font);
    }
    hr_pmes_moji_h((ATR_MMOJIH *)0x70000000, 1);
    memset(&mj, 0, sizeof(HRPMOJI));
    mj.dcnt = 0x1000;
    pm = (ATR_MMOJI *)(0x70000000 + sizeof(ATR_MMOJIH));

    flag = 0;
    for (i = 0; i < 8; i++, mesp++) {
        if (mesp->flag & 1) {
            flag++;
            hr_mesp_calcXY(mesp, &x, &y, &s);
            mj.page = mesp->ptr;
            pm = (ATR_MMOJI *)hr_pmes_moji_drawM(&mj, pp, pm, x, y, mesp->alpha, s, s / 2);
        }
    }

    if (flag) {
        FlushCache(0);
        sceDmaSync(DmaChGIF, 0, 0);
        *((vu16 *)&DmaChGIF->chcr) = 4;
        sceDmaSync(DmaChVIF1, 0, 0);
        sceDmaSend(DmaChGIF, (u32)pp | 0x80000000);
        sceGsSyncPath(0, 0);
    }
}
