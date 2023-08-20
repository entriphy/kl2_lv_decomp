#include "harada/hr_obcon3.h"
#include "harada/hr_main.h"
#include "harada/hr_obcon.h"
#include "harada/hr_mirr.h"
#include "okanoyo/okio.h"

static void hr_ptes_commVPO(HRPT_EFC *pt) {
    sceVu0FMATRIX m;

    if ((s32)hr_obcvpo == -1 || (s32)hr_obcvpf == -1) {
        return;
    }

    if (*hr_obcvpf > pt->bun) {
        sceVu0UnitMatrix(m);
        sceVu0RotMatrix(m, m, pt->rot);
        sceVu0TransMatrix(m, m, pt->pos);
        hr_comm_drawVPO(m, &hr_obcvpo[pt->bun], pt->reg3);
    }
}

static void hr_ptes_0706swon(HRPT_EFC *pt) {
    pt->reg0++;
    // TODO: this might be wrong?
    switch (pt->reg0) {
        case 10:
        case 25:
        case 40:
            pt->reg3 = 2;
            break;
        case 15:
        case 30:
        case 45:
            pt->reg3 = 0;
            break;
        case 55:
            pt->reg3 = 1;
            break;
    }
}

static void hr_ptes_0806swon(HRPT_EFC *pt) {
    pt->reg0++;
    switch (pt->reg0) {
        case 20:
            pt->reg3 = 2;
            break;
        case 30:
            pt->reg0 = 0;
            pt->reg3 = 0;
            break;
    }
}

static u8 hr_backcol(s32 time) {
    if (time < 0) {
        time = 0;
    } else if (time > 58) {
        time = 58;
    }
    time = time * 255 / 58;
    return time;
}

static void hr_ptes_openback(HRPT_EFC *pt) {
    pt->reg0++;
    mapvw.bgu.b = mapvw.bgu.g = mapvw.bgu.r = hr_backcol(pt->reg0 - 22);
    mapvw.bgt.b = mapvw.bgt.g = mapvw.bgt.r = hr_backcol(pt->reg0 - 38);
}

static void hr_ptes_mirrBig(HRPT_EFC *pt) {
    pt->reg0++;
    if (pt->reg0 < 90) {
        hfmcam.vsmFg = 2;
        hfmcam.vsmSy = pt->reg0 / 30.0f + 1.0f;
    } else {
        hfmcam.vsmFg = 0;
    }
}

static void hr_ptes_mirrOff(HRPT_EFC *pt) {
    hfmcam.flag |= 1;
}

static void hr_ptefc_draw(OBJWORK *objw) {
    HRPT_EFC *pt;
    s32 i;

    for (i = 0, pt = (HRPT_EFC *)objw->work; i < 4; i++, pt++) {
        if (pt->id != PTEFC_NON && pt->draw != NULL) {
            pt->draw(pt);
        }
    }
}

void hr_ptefc_set(OBJWORK *objw) {
    objw->work = getmem(4 * sizeof(HRPT_EFC));
    if ((s32)objw->work == -1) {
        objw->stat0 = 0;
    } else {
        memset(objw->work, 0, 4 * sizeof(HRPT_EFC));
        objw->draw = hr_ptefc_draw;
    }
}

void hr_ptefc_erase(s32 *ptr) {
    freemem(((OBJWORK *)ptr)->work);
    ((OBJWORK *)ptr)->stat0 = 0;
}

void hr_ptefc_work(OBJWORK *objw) {
    HRPT_EFC *pt;
    s32 i;

    for (i = 0, pt = (HRPT_EFC *)objw->work; i < 4; i++, pt++) {
        if (pt->id != PTEFC_NON && pt->work != NULL) {
            pt->work(pt);
        }
    }
}

static HRPT_EFC* hr_ptefc_searchID(OBJWORK *objw, s32 id) {
    HRPT_EFC *efc;
    s32 i;

    for (efc = (HRPT_EFC *)objw->work, i = 0; i < 4; i++, efc++) {
        if (efc->id == id) {
            return efc;
        }
    }

    return NULL;
}

static void hr_ptefc_copy(HRPT_EFC *efc, sceVu0FVECTOR pos, sceVu0FVECTOR rot) {
    sceVu0CopyVectorXYZ(efc->pos, pos);
    sceVu0CopyVector(efc->rot, rot);
    sceVu0ScaleVector(efc->rot, efc->rot, M_PIf);
    sceVu0DivVector(efc->rot, efc->rot, 180.0f);
    efc->pos[3] = efc->rot[3] = 1.0f;
}

s32 hr_ptes_ON(s32 *ptr, s32 id, sceVu0FVECTOR pos, sceVu0FVECTOR rot) {
    HRPT_EFC *efc;

    efc = hr_ptefc_searchID((OBJWORK *)ptr, PTEFC_NON);
    if (efc == NULL) {
        return 0;
    }
    memset(efc, 0, sizeof(HRPT_EFC));
    efc->id = id;

    switch (id) {
        case PT0706_SWON:
            efc->work = hr_ptes_0706swon;
            efc->draw = hr_ptes_commVPO;
            efc->bun = 0;
            break;
        case PT0706_SWOFF:
            efc->reg3 = 1;
            efc->draw = hr_ptes_commVPO;
            efc->bun = 0;
            break;
        case PT0706_SWLON:
            efc->draw = hr_ptes_commVPO;
            efc->bun = 0;
            break;
        case PT0806_SWBOU:
            efc->work = hr_ptes_0806swon;
            efc->draw = hr_ptes_commVPO;
            efc->bun = 0;
            break;
        case PTOPEN_RAKKA:
            efc->work = hr_ptes_openback;
            break;
        case PT1111_MIRR:
            efc->work = hr_ptes_mirrBig;
            break;
        case PT1111_MIRRF:
            efc->work = hr_ptes_mirrOff;
            break;
        case PT1306_HANET:
            efc->draw = hr_ptes_commVPO;
            efc->bun = 1;
            break;
        case PT1306_HANEH:
            efc->draw = hr_ptes_commVPO;
            efc->bun = 0;
            break;
        case PT2601_ATAMA:
            efc->draw = hr_ptes_commVPO;
            efc->bun = 0;
            break;
        case PT0605_YUKA:
            efc->draw = hr_ptes_commVPO;
            efc->bun = 0;
            break;
        case PT0605_YUKAD:
            efc->draw = hr_ptes_commVPO;
            efc->bun = 1;
            break;
    }

    hr_ptefc_copy(efc, pos, rot);
    return 1;
}

s32 hr_ptes_WORK(s32 *ptr, s32 id, sceVu0FVECTOR pos, sceVu0FVECTOR rot) {
    HRPT_EFC *efc;

    efc = hr_ptefc_searchID((OBJWORK *)ptr, id);
    if (efc == NULL) {
        return 0;
    }
    hr_ptefc_copy(efc, pos, rot);
    return 1;
}

s32 hr_ptes_DEL(s32 *ptr, s32 id) {
    HRPT_EFC *efc;

    efc = hr_ptefc_searchID((OBJWORK *)ptr, id);
    if (efc == NULL) {
        return 0;
    }
    if (id == PT1111_MIRR) {
        hfmcam.vsmFg = 0;
    }
    memset(efc, 0, sizeof(HRPT_EFC));
    return 1;
}
