#include "nakano/gscreen.h"
#include "nakano/vu0mac.h"
#include "nakano/main.h"
#include "nakano/k_math.h"
#include "take/motsys.h"
#include "take/object.h"
#include "hoshino/h_gamesnd.h"
#include "harada/hr_pall.h"

static PsfxMODEL PsfxHeart[3];
static PsfxMODEL PsfxYumekake;
static PsfxMODEL PsfxZanki;
static PsfxMODEL PsfxKagi;
static PsfxMODEL PsfxExitm; // ?
static nkGameScrInfo nkGSI;
static sceVu0FMATRIX nkgsNLM;
static sceVu0FMATRIX nkgsLCM;
static s32 nkGameDispFlag;

// TODO: fill these in
nkTEXIF kys_ti = {};
nkTEXIF stj_ti = {};
nkTEXIF mm_ti[6] = {};
nkTEXIF stj_mt = {};
nkTEXIF stj_mk = {};

void nkGameDisp(s32 flag) {
    nkGameDispFlag = flag;
}

void SetGScreenLight() {
    s32 no = 0;

    switch (GameGbl.vision) {
        case 0x0100:
            no = 5;
            break;
        case 0x0101:
            no = 5;
            break;
        case 0x0102:
            no = 6;
            break;
        case 0x0103:
            no = 5;
            break;
        case 0x0104:
            no = 5;
            break;
        case 0x0200:
            no = 3;
            break;
        case 0x0201:
            no = 7;
            break;
        case 0x0202:
            no = 3;
            break;
        case 0x0203:
            no = 10;
            break;
        case 0x0204:
            no = 3;
            break;
        case 0x0205:
            no = 10;
            break;
        case 0x0206:
            no = 4;
            break;
        case 0x0207:
            no = 3;
            break;
        case 0x0400:
            no = 14;
            break;
        case 0x0401:
            no = 12;
            break;
        case 0x0402:
            no = 7;
            break;
        case 0x0403:
            no = 7;
            break;
        case 0x0404:
            no = 9;
            break;
        case 0x0405:
            no = 5;
            break;
        case 0x0406:
            no = 6;
            break;
        case 0x0407:
            no = 4;
            break;
        case 0x0408:
            no = 6;
            break;
        case 0x0409:
            no = 5;
            break;
        case 0x040A:
            no = 6;
            break;
        case 0x040B:
            no = 9;
            break;
        case 0x040C:
            no = 15;
            break;
        case 0x0500:
            no = 9;
            break;
        case 0x0501:
            no = 9;
            break;
        case 0x0503:
            no = 5;
            break;
        case 0x0504:
            no = 9;
            break;
        case 0x0505:
            no = 3;
            break;
        case 0x0600:
            no = 7;
            break;
        case 0x0601:
            no = 7;
            break;
        case 0x0602:
            no = 7;
            break;
        case 0x0603:
            no = 8;
            break;
        case 0x0604:
            no = 9;
            break;
        case 0x0605:
            no = 7;
            break;
        case 0x0606:
            no = 9;
            break;
        case 0x0607:
            no = 8;
            break;
        case 0x0608:
            no = 7;
            break;
        case 0x0700:
            no = 4;
            break;
        case 0x0701:
            no = 13;
            break;
        case 0x0702:
            no = 6;
            break;
        case 0x0703:
            no = 4;
            break;
        case 0x0704:
            no = 1;
            break;
        case 0x0705:
            no = 4;
            break;
        case 0x0800:
            no = 5;
            break;
        case 0x0801:
            no = 13;
            break;
        case 0x0802:
            no = 6;
            break;
        case 0x0803:
            no = 4;
            break;
        case 0x0804:
            no = 1;
            break;
        case 0x0805:
            no = 4;
            break;
        case 0x0900:
            no = 4;
            break;
        case 0x0901:
            no = 4;
            break;
        case 0x0902:
            no = 3;
            break;
        case 0x0903:
            no = 3;
            break;
        case 0x0904:
            no = 3;
            break;
        case 0x0905:
            no = 4;
            break;
        case 0x0906:
            no = 4;
            break;
        case 0x0907:
            no = 4;
            break;
        case 0x0908:
            no = 3;
            break;
        case 0x0A00:
            no = 1;
            break;
        case 0x0A01:
            no = 6;
            break;
        case 0x0A02:
            no = 1;
            break;
        case 0x0A03:
            no = 6;
            break;
        case 0x0A04:
            no = 6;
            break;
        case 0x0B00:
            no = 3;
            break;
        case 0x0B01:
            no = 4;
            break;
        case 0x0B02:
            no = 4;
            break;
        case 0x0B03:
            no = 4;
            break;
        case 0x0B04:
            no = 4;
            break;
        case 0x0B05:
            no = 4;
            break;
        case 0x0B06:
            no = 4;
            break;
        case 0x0B07:
            no = 4;
            break;
        case 0x0B08:
            no = 4;
            break;
        case 0x0B09:
            no = 4;
            break;
        case 0x0B0A:
            no = 4;
            break;
        case 0x0B0C:
            no = 4;
            break;
        case 0x0B0D:
            no = 4;
            break;
        case 0x0B0E:
            no = 4;
            break;
        case 0x0B0F:
            no = 4;
            break;
        case 0x0B10:
            no = 3;
            break;
        case 0x0C00:
            no = 3;
            break;
        case 0x0C01:
            no = 8;
            break;
        case 0x0C02:
            no = 3;
            break;
        case 0x0C03:
            no = 11;
            break;
        case 0x0C04:
            no = 3;
            break;
        case 0x0C05:
            no = 11;
            break;
        case 0x0C06:
            no = 4;
            break;
        case 0x0D00:
            no = 4;
            break;
        case 0x0D01:
            no = 5;
            break;
        case 0x0D02:
            no = 5;
            break;
        case 0x0D03:
            no = 5;
            break;
        case 0x0D04:
            no = 5;
            break;
        case 0x0D05:
            no = 5;
            break;
        case 0x0D06:
            no = 5;
            break;
        case 0x0E00:
            no = 3;
            break;
        case 0x0E01:
            no = 3;
            break;
        case 0x0E02:
            no = 3;
            break;
        case 0x0E03:
            no = 4;
            break;
        case 0x0E04:
            no = 4;
            break;
        case 0x0F00:
            no = 3;
            break;
        case 0x0F01:
            no = 4;
            break;
        case 0x0F02:
            no = 3;
            break;
        case 0x0F03:
            no = 3;
            break;
        case 0x0F04:
            no = 3;
            break;
        case 0x0F05:
            no = 4;
            break;
        case 0x0F06:
            no = 4;
            break;
        case 0x0F07:
            no = 4;
            break;
        case 0x0F08:
            no = 3;
            break;
        case 0x1000:
            no = 1;
            break;
        case 0x1001:
            no = 2;
            break;
        case 0x1002:
            no = 2;
            break;
        case 0x1003:
            no = 2;
            break;
        case 0x1004:
            no = 2;
            break;
        case 0x1005:
            no = 2;
            break;
        case 0x1006:
            no = 2;
            break;
        case 0x1007:
            no = 2;
            break;
        case 0x1008:
            no = 2;
            break;
        case 0x1009:
            no = 2;
            break;
        case 0x100A:
            no = 2;
            break;
        case 0x1100:
            no = 5;
            break;
        case 0x1101:
            no = 5;
            break;
        case 0x1102:
            no = 5;
            break;
        case 0x1200:
            no = 1;
            break;
        case 0x1201:
            no = 1;
            break;
        case 0x1202:
            no = 1;
            break;
        case 0x1203:
            no = 2;
            break;
        case 0x1204:
            no = 2;
            break;
        case 0x1205:
            no = 2;
            break;
        case 0x1300:
            no = 1;
            break;
        case 0x1301:
            no = 1;
            break;
        case 0x1302:
            no = 1;
            break;
        case 0x1303:
            no = 2;
            break;
        case 0x1304:
            no = 2;
            break;
        case 0x1305:
            no = 2;
            break;
        case 0x1306:
            no = 2;
            break;
        case 0x1307:
            no = 2;
            break;
        case 0x1308:
            no = 2;
            break;
        case 0x1500:
            no = 3;
            break;
        case 0x1600:
            no = 5;
            break;
        case 0x1700:
            no = 3;
            break;
        case 0x1800:
            no = 1;
            break;
        case 0x1A00:
            no = 1;
            break;
        case 0x1B00:
            no = 1;
            break;
        case 0x1B01:
            no = 5;
            break;
        case 0x1B02:
            no = 5;
            break;
        case 0x1E00:
            no = 1;
            break;
        case 0x2000:
            no = 1;
            break;
        case 0x2200:
            no = 1;
            break;
        case 0x2300:
            no = 1;
            break;
        case 0x2700:
            no = 1;
            break;
        case 0x2800:
            no = 1;
            break;
    }

    nkSetLightNo(nkgsNLM, nkgsLCM, no, NULL);
}

void InitStageGScreen() {
    char filename[80];

    // Empty function
}

void InitGScreen() {
    SFXOBJ *pObj;
    PsfxMODEL *model;
    s32 i;

    for (i = 0; i < 3; i++) {
        model = &PsfxHeart[i];
        model->Rot[0] = 0.0f;
        model->Rot[1] = 0.0f;
        model->Rot[2] = 0.0f;
        model->Rot[3] = 0.0f;
        model->Trans[0] = 0.0f;
        model->Trans[1] = 0.0f;
        model->Trans[2] = 0.0f;
        model->Trans[3] = 1.0f;
        pObj = model->pObj = GetSfxObjectData(820);
        if (pObj != NULL) {
            pObj->Condition = 1;
            SetBaseMatrix(pObj, model->Rot, model->Trans, 0);
        }
    }

    model = &PsfxYumekake;
    model->Rot[0] = 0.0f;
    model->Rot[1] = 0.0f;
    model->Rot[2] = 0.0f;
    model->Rot[3] = 0.0f;
    model->Trans[0] = 0.0f;
    model->Trans[1] = 0.0f;
    model->Trans[2] = 0.0f;
    model->Trans[3] = 1.0f;
    pObj = model->pObj = GetSfxObjectData(821);
    if (pObj != NULL) {
        pObj->Condition = 1;
        SetBaseMatrix(pObj, model->Rot, model->Trans, 0);
    }

    model = &PsfxExitm;
    model->Rot[0] = 0.0f;
    model->Rot[1] = 0.0f;
    model->Rot[2] = 0.0f;
    model->Rot[3] = 0.0f;
    model->Trans[0] = 0.0f;
    model->Trans[1] = 0.0f;
    model->Trans[2] = 0.0f;
    model->Trans[3] = 1.0f;
    pObj = model->pObj = GetSfxObjectData(822);
    if (pObj != NULL) {
        pObj->Condition = 1;
        SetBaseMatrix(pObj, model->Rot, model->Trans, 0);
    }

    model = &PsfxZanki;
    model->Rot[0] = 0.0f;
    model->Rot[1] = 0.0f;
    model->Rot[2] = 0.0f;
    model->Rot[3] = 0.0f;
    model->Trans[0] = 0.0f;
    model->Trans[1] = 0.0f;
    model->Trans[2] = 0.0f;
    model->Trans[3] = 1.0f;
    pObj = model->pObj = GetSfxObjectData(819);
    if (pObj != NULL) {
        pObj->Condition = 1;
        SetBaseMatrix(pObj, model->Rot, model->Trans, 0);
    }

    model = &PsfxKagi;
    model->Rot[0] = 0.0f;
    model->Rot[1] = 0.0f;
    model->Rot[2] = 0.0f;
    model->Rot[3] = 0.0f;
    model->Trans[0] = 0.0f;
    model->Trans[1] = 0.0f;
    model->Trans[2] = 0.0f;
    model->Trans[3] = 1.0f;
    pObj = model->pObj = GetSfxObjectData(655);
    if (pObj != NULL) {
        pObj->Condition = 1;
        SetBaseMatrix(pObj, model->Rot, model->Trans, 0);
    }

    nkGSI.k0heart = GameGbl.kloLife[0];
    nkGSI.htime = GameGbl.kloLife[0];
    if (!kzCheckGameClear()) {
        nkGSI.ytime = OkDPcnt();
    } else {
        nkGSI.ytime = OkDPSTcnt();
    }
    nkGSI.k0zanki = GameGbl.kloZanki[0];
    nkGSI.k0exitm = OkEXcnt();
    nkGSI.ztime = 0.0f;
    nkGSI.ztime2 = 0.0f;
    nkGSI.R1time = 0.0f;
    nkGSI.mm_rot = 0.0f;
    nkGSI.extime = 0.0f;
    nkGSI.gus_cnt = 0.0f;
    nkGSI.yume150_flag = 0;

    SetGScreenLight();
}

void nkGScreenMatrix() {
    sceVu0FVECTOR vec;
    sceVu0FMATRIX mat;
    sceVu0FMATRIX vsm;
    sceVu0FMATRIX wvm;
    sceVu0FMATRIX wsm;

    sceVu0UnitMatrix(mat);
    vec[0] = 0.0f;
    vec[1] = 0.0f;
    vec[2] = 500.0f;
    vec[3] = 1.0f;
    sceVu0TransMatrix(wvm, mat, vec);
    sceVu0ViewScreenMatrix(vsm, 1000.0f, 1.0f, 0.47f, 1748.0f, 1998.0f, 1.0f, 16777215.0f, 0.1f, 65535.0f);
    sceVu0MulMatrix(wsm, vsm, wvm);
    sceVu0CopyMatrix(Scr.WvMtx, wvm);
    sceVu0CopyMatrix(Scr.VsMtx, vsm);
    sceVu0CopyMatrix(Scr.WsMtx, wsm);
}

void nkGScreenMatrixXY(f32 x, f32 y) {
    sceVu0FVECTOR vec;
    sceVu0FMATRIX mat;
    sceVu0FMATRIX vsm;
    sceVu0FMATRIX wvm;
    sceVu0FMATRIX wsm;

    sceVu0UnitMatrix(mat);
    vec[0] = 0.0f;
    vec[1] = 0.0f;
    vec[2] = 500.0f;
    vec[3] = 1.0f;
    sceVu0TransMatrix(wvm, mat, vec);
    sceVu0ViewScreenMatrix(vsm, 1000.0f, 1.0f, 0.47f, x + 2048.0f, y + 2048.0f, 1.0f, 16777215.0f, 0.1f, 65535.0f);
    sceVu0MulMatrix(wsm, vsm, wvm);
    sceVu0CopyMatrix(Scr.WvMtx, wvm);
    sceVu0CopyMatrix(Scr.VsMtx, vsm);
    sceVu0CopyMatrix(Scr.WsMtx, wsm);
}

static void DrawPrimGT4_2(nkGifPacket *pp, s32 x, s32 y, s32 px, s32 py, s32 pw, s32 ph) {
    px += 8;
    py += 8;

    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    pp->buf[pp->size++].ul64[1] = SCE_GS_UV | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;

    pp->buf[pp->size].ul32[0] = px;
    pp->buf[pp->size].ul32[1] = py;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x80;
    pp->buf[pp->size].ul32[1] = 0x80;
    pp->buf[pp->size].ul32[2] = 0x80;
    pp->buf[pp->size].ul32[3] = 0x80;
    pp->size++;
    pp->buf[pp->size].ul32[0] = x + 0x6C00;
    pp->buf[pp->size].ul32[1] = y + 0x7900;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = px + pw;
    pp->buf[pp->size].ul32[1] = py;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x80;
    pp->buf[pp->size].ul32[1] = 0x80;
    pp->buf[pp->size].ul32[2] = 0x80;
    pp->buf[pp->size].ul32[3] = 0x80;
    pp->size++;
    pp->buf[pp->size].ul32[0] = x + 0x6C00 + pw;
    pp->buf[pp->size].ul32[1] = y + 0x7900;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = px;
    pp->buf[pp->size].ul32[1] = py + ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x80;
    pp->buf[pp->size].ul32[1] = 0x80;
    pp->buf[pp->size].ul32[2] = 0x80;
    pp->buf[pp->size].ul32[3] = 0x80;
    pp->size++;
    pp->buf[pp->size].ul32[0] = x + 0x6C00;
    pp->buf[pp->size].ul32[1] = y + 0x78F8 + ((ph >> 4) << 3);
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = px + pw;
    pp->buf[pp->size].ul32[1] = py + ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x80;
    pp->buf[pp->size].ul32[1] = 0x80;
    pp->buf[pp->size].ul32[2] = 0x80;
    pp->buf[pp->size].ul32[3] = 0x80;
    pp->size++;
    pp->buf[pp->size].ul32[0] = x + 0x6C00 + pw;
    pp->buf[pp->size].ul32[1] = y + 0x78F8 + ((ph >> 4) << 3);
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
}

void DrawGameScrHeart() {
    SFXOBJ *pObj;
    PsfxMODEL *model;
    sceVu0FMATRIX nlm;
    sceVu0FMATRIX lcm;
    s32 i;
    sceVu0FMATRIX mat;
    f32 Scale;
    f32 work;
    f32 roty;
    f32 rad;

    nkGScreenMatrix();
    if (GameGbl.pause_flag == 0) {
        work = GameGbl.kloLife[0] - nkGSI.htime;
        if (work > 1.0f / 30.0f) {
            nkGSI.htime += 1.0f / 30.0f;
        } else if (work < -1.0f / 30.0f) {
            nkGSI.htime -= 1.0f / 30.0f;
        } else {
            nkGSI.htime = GameGbl.kloLife[0];
        }
    }

    model = PsfxHeart;
    rad = model->Rot[3] += M_5PI_3f / 10.0f;
    if (rad <= -M_PIf) {
        rad += M_TWOPIf;
    } else if (rad > M_PIf) {
        rad-= M_TWOPIf;
    }
    model->Rot[3] = rad;

    roty = (_sin(rad) * 45.0f * M_PIf) / 180.0f;
    model->Rot[1] = roty;

    if (nkGSI.htime > 3.0f) {
        nkGSI.htime = 3.0f;
    }

    for (i = 0; i < nkGSI.htime; i++) {
        model = &PsfxHeart[i];
        pObj = model->pObj;

        SetObjPause(pObj, GameGbl.pause_flag);
        SetObjNormalLight(pObj, &nkgsNLM);
        SetObjLightColor(pObj, &nkgsLCM);
        pObj->Condition = 1;
        model->Rot[1] = roty;
        sceVu0UnitMatrix(mat);
        sceVu0RotMatrixZ(mat, mat, model->Rot[2]);
        sceVu0RotMatrixX(mat, mat, model->Rot[0]);
        sceVu0RotMatrixY(mat, mat, model->Rot[1]);
        sceVu0TransMatrix(mat, mat, model->Trans);

        Scale = 1.0f;
        work = nkGSI.htime - i;
        if (work > 0.0f && work < 1.0f) {
            Scale = work;
        }

        sceVu0ScaleVectorXYZ(mat[0], mat[0], Scale);
        sceVu0ScaleVectorXYZ(mat[1], mat[1], Scale);
        sceVu0ScaleVectorXYZ(mat[2], mat[2], Scale);
        SetBaseMatrix2(pObj, mat);
        nkGScreenMatrixXY(i * 48.0f + (-261.0f), -94.0f);
        ModelDraw(pObj);
    }
}

void DrawGameScrZanki(s32 boss_flag) {
    SFXOBJ *pObj;
    PsfxMODEL *model;
    sceVu0FMATRIX nlm;
    sceVu0FMATRIX lcm;
    sceVu0FMATRIX mat;
    f32 Scale;
    f32 rotz;
    OBJWORK *klo;

    klo = GameGbl.klonoa;
    nkGScreenMatrix();
    model = &PsfxZanki;
    pObj = model->pObj;
    SetObjPause(pObj, GameGbl.pause_flag);
    SetObjNormalLight(pObj, nkgsNLM);
    SetObjLightColor(pObj, nkgsLCM);

    model->Rot[3] += M_PIf / 16.0f;
    model->Rot[3] = nkRadMask(model->Rot[3]);

    nkGSI.ztime2 += M_PIf / 12.0f;
    nkGSI.ztime2 = nkRadMask(nkGSI.ztime2);

    rotz = (_sin(nkGSI.ztime2) * 45.0f * M_PIf) / 180.0f;
    model->Rot[2] = rotz;
    model->Rot[1] = _sin(model->Rot[3]) * M_PIf / 8.0f;

    pObj->Condition = 1;
    if (nkGSI.R1time > 0.0f) {
        rotz = _sin(nkGSI.ztime2) * 10.0f * M_PIf / 180.0f;
        model->Rot[1] = _sin(nkGSI.ztime2) * M_PIf / 16.0f;
        model->Rot[2] = rotz;
        Scale = 1.0f;
    } else if (klo->bun0 != 12 && klo->bun0 != 34) {
        Scale = _sin(nkGSI.ztime2) * 0.2f + 1.2f;
    } else {
        Scale = nkGSI.ztime / 60.0f;
        model->Rot[1] = nkGSI.ztime2;
    }

    sceVu0UnitMatrix(mat);
    sceVu0RotMatrixZ(mat, mat, model->Rot[2]);
    sceVu0RotMatrixX(mat, mat, model->Rot[0]);
    sceVu0RotMatrixY(mat, mat, model->Rot[1]);
    sceVu0TransMatrix(mat, mat, model->Trans);
    sceVu0ScaleVectorXYZ(mat[0], mat[0], Scale);
    sceVu0ScaleVectorXYZ(mat[1], mat[1], Scale);
    sceVu0ScaleVectorXYZ(mat[2], mat[2], Scale);
    SetBaseMatrix2(pObj, mat);
    nkGScreenMatrixXY(212.0f, boss_flag * 12.0f + (-80.0f));
    ModelDraw(pObj);
}

void DrawMomePanel(nkGifPacket *pp, s32 x, s32 y, nkTEXIF *ti, s32 n) {
    f32 var_f12;
    f32 var_f20;
    f32 work2;
    s32 var_s5;
    s32 var_s6;
    nkTEXIF *var_s1;
    s32 var_s3;
    s32 var_s4;
    s32 var_s7;
    s32 var_s8;

    var_s5 = 0;
    var_s6 = 128;
    var_s1 = ti;
    var_s3 = ti->pw + 8 >> 1;
    var_s4 = ti->ph + 8 >> 2;
    var_s7 = x + var_s3;
    var_s8 = y + var_s4;

    if (nkGSI.k0exitm == 6) {
        if (n > 0 && nkGSI.extime < 120.0f && nkGSI.extime >= 60.0f) {
            var_f12 = nkGSI.mm_rot;
            if (var_f12 > M_PI_2f) {
                var_f12 -= M_PIf;
                var_s1 += 6;
            } else if (var_f12 < -M_PI_2f) {
                var_f12 += M_PIf;
                var_s1 += 6;
            }
            var_s3 *= _cos(var_f12);
            var_s5 = 1;
        } else {
            if (nkGSI.extime < 60.0f && nkGSI.R1time <= 0.0f) {
                var_s6 = 127.0f * _sin(M_PIf * (nkGSI.extime - 10.0f) / 50.0f) + 127.0f;
            }
            var_f20 = nkGSI.extime - 60.0f;
        }
    } else {
        var_f20 = nkGSI.extime;
    }

    if (n > 0 && !var_s5) {
        if (var_f20 > 60.0f && nkGSI.k0exitm == n) {
            var_f12 = nkGSI.mm_rot;
        } else {
            if (n > nkGSI.k0exitm) {
                var_f12 = 0.0f;
            } else {
                var_f12 = M_PIf;
            }
        }

        if (var_f12 > M_PI_2f) {
            var_f12 -= M_PIf;
            var_s1 += 6;
        } else if (var_f12 < -M_PI_2f) {
            var_f12 += M_PIf;
            var_s1 += 6;
        }
        var_s3 *= _cos(var_f12);
    }

    if (nkGSI.extime < 10.0f) {
        var_s6 = 127.0f * nkGSI.extime / 10.0f;
    }

    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
    pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
    pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
    pp->buf[pp->size++].ul64[0] = var_s1->tex0;
    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    pp->buf[pp->size++].ul64[1] = SCE_GS_UV | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;

    pp->buf[pp->size].ul32[0] = var_s1->px;
    pp->buf[pp->size].ul32[1] = var_s1->py;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 128;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_s6;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 - var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_s8 - var_s4;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_s1->px + var_s1->pw;
    pp->buf[pp->size].ul32[1] = var_s1->py;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 128;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_s6;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 + var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_s8 - var_s4;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_s1->px;
    pp->buf[pp->size].ul32[1] = var_s1->py + var_s1->ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 128;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_s6;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 - var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_s8 + var_s4;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_s1->px + var_s1->pw;
    pp->buf[pp->size].ul32[1] = var_s1->py + var_s1->ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 128;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_s6;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 + var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_s8 + var_s4;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
}

void DrawKagiPanel(nkGifPacket *pp, s32 x, s32 y, nkTEXIF *ti, s32 n) {
    s32 var_t1;
    s32 var_t3;
    s32 var_t4;
    s32 var_t2;
    s32 var_t5;
    s32 var_t7;
    s32 var_t6;

    var_t1 = 128;
    var_t3 = ti->pw >> 1;
    var_t4 = ti->ph >> 2;
    var_t7 = x + var_t3;
    var_t6 = y + var_t4;
    var_t2 = ti->px + 8;
    var_t5 = ti->py;

    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
    pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
    pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
    pp->buf[pp->size++].ul64[0] = ti->tex0;
    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    pp->buf[pp->size++].ul64[1] = SCE_GS_UV | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;

    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t5;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 128;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_t7 - var_t3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_t6 - var_t4;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_t2 + ti->pw;
    pp->buf[pp->size].ul32[1] = var_t5;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 128;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_t7 + var_t3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_t6 - var_t4;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t5 + ti->ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 128;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_t7 - var_t3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_t6 + var_t4;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_t2 + ti->pw;
    pp->buf[pp->size].ul32[1] = var_t5 + ti->ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 128;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_t7 + var_t3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_t6 + var_t4;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
}

void DrawGameScrKagi() {
    SFXOBJ *pObj;
    PsfxMODEL *model;
    f32 Scale;
    sceVu0FMATRIX nlm;
    sceVu0FMATRIX lcm;
    sceVu0FMATRIX mat;
    GAME_WORK *gbl; // wtf

    gbl = &GameGbl;
    nkGScreenMatrix();
    model = &PsfxKagi;
    pObj = model->pObj;

    SetObjPause(pObj, gbl->pause_flag);
    SetObjNormalLight(pObj, &nkgsNLM);
    SetObjLightColor(pObj, &nkgsLCM);
    model->Rot[3] += M_PIf / 32.0f;
    model->Rot[3] = nkRadMask(model->Rot[3]);
    model->Rot[1] = _sin(model->Rot[3]) * M_PIf / 8.0f;
    pObj->Condition = 1;
    Scale = 1.0f;
    model->Rot[2] = M_PI_4f;

    sceVu0UnitMatrix(mat);
    sceVu0RotMatrixZ(mat, mat, model->Rot[2]);
    sceVu0RotMatrixX(mat, mat, model->Rot[0]);
    sceVu0RotMatrixY(mat, mat, model->Rot[1]);
    sceVu0TransMatrix(mat, mat, model->Trans);
    sceVu0ScaleVectorXYZ(mat[0], mat[0], Scale);
    sceVu0ScaleVectorXYZ(mat[1], mat[1], Scale);
    sceVu0ScaleVectorXYZ(mat[2], mat[2], Scale);
    SetBaseMatrix2(pObj, mat);
    nkGScreenMatrixXY(-248.0f, -62.0f);
    ModelDraw(pObj);
}

void DrawGusMeterPanel(nkGifPacket *pp, s32 x, s32 y, nkTEXIF *ti, s32 n) {
    s32 var_s8;
    nkTEXIF *var_s1;
    s32 var_s3;
    s32 var_s6;
    s32 var_s7;
    s32 var_sp0;
    s32 var_s4;
    s32 var_s5;
    f32 var_f1;
    s32 var_t1;
    s32 var_t0;
    s32 var_t2;

    var_s8 = 128;
    var_s1 = ti;
    var_s3 = var_s1->pw >> 1;
    var_s6 = var_s1->ph >> 2;
    var_s7 = x + var_s3;
    var_sp0 = y + var_s6;
    var_s4 = var_s1->px + 8;
    var_s5 = var_s1->py + 8;

    var_f1 = _sin(nkGSI.gus_cnt);
    if (var_f1 < 0.0f) {
        var_f1 = -var_f1;
    }

    var_t2 = 128;
    if (n == 1) {
        var_t1 = 127.0f * (1.0f - var_f1);
        var_t0 = 127.0f * GameGbl.gus_timer;
        if (var_t0 < var_t1) {
            var_t0 = var_t1;
        }
    } else {
        var_t1 = var_t0 = 128;
    }

    nkGSI.gus_cnt = GameGbl.gus_cnt;
    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
    pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
    pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
    pp->buf[pp->size++].ul64[0] = var_s1->tex0;
    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    pp->buf[pp->size++].ul64[1] = SCE_GS_UV | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;

    pp->buf[pp->size].ul32[0] = var_s4;
    pp->buf[pp->size].ul32[1] = var_s5;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t1;
    pp->buf[pp->size].ul32[2] = var_t0;
    pp->buf[pp->size].ul32[3] = var_s8;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 - var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_sp0 - var_s6;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_s4 + var_s1->pw;
    pp->buf[pp->size].ul32[1] = var_s5;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t1;
    pp->buf[pp->size].ul32[2] = var_t0;
    pp->buf[pp->size].ul32[3] = var_s8;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 + var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_sp0 - var_s6;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_s4;
    pp->buf[pp->size].ul32[1] = var_s5 + var_s1->ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t1;
    pp->buf[pp->size].ul32[2] = var_t0;
    pp->buf[pp->size].ul32[3] = var_s8;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 - var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_sp0 + var_s6;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_s4 + var_s1->pw;
    pp->buf[pp->size].ul32[1] = var_s5 + var_s1->ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t1;
    pp->buf[pp->size].ul32[2] = var_t0;
    pp->buf[pp->size].ul32[3] = var_s8;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 + var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_sp0 + var_s6;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
}

void DrawGusMeterBar(nkGifPacket *pp) {
    s32 var_t1;
    f32 var_f1;
    s32 var_a2;
    s32 var_t3;
    s32 num_0x1700 = 0x1700;

    var_t1 = 64;
    var_f1 = GameGbl.gus_timer;
    var_a2 = 1872.0f * var_f1;
    var_t3 = 127.0f * (1.0f - var_f1);

    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 0, 0, 1, 1, 0), 0, 2);
    pp->buf[pp->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZF2 << 4;

    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + num_0x1700;
    pp->buf[pp->size].ul32[1] = 0x7A10;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = var_t3;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + num_0x1700 + var_a2;
    pp->buf[pp->size].ul32[1] = 0x7A10;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + num_0x1700;
    pp->buf[pp->size].ul32[1] = 0x7AD8;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = var_t3;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + num_0x1700 + var_a2;
    pp->buf[pp->size].ul32[1] = 0x7AD8;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
}

void DrawEngSysMeterPanel(nkGifPacket *pp, s32 x, s32 y, nkTEXIF *ti, s32 n) {
    s32 var_s8;
    nkTEXIF *var_s1;
    s32 var_s3;
    s32 var_s6;
    s32 var_s7;
    s32 var_sp0;
    s32 var_s4;
    s32 var_s5;
    f32 var_f1;
    s32 var_t1;
    s32 var_t0;
    s32 var_t2;

    var_s8 = 128;
    var_s1 = ti;
    var_s3 = var_s1->pw >> 1;
    var_s6 = var_s1->ph >> 2;
    var_s7 = x + var_s3;
    var_sp0 = y + var_s6;
    var_s4 = var_s1->px + 8;
    var_s5 = var_s1->py + 8;

    var_f1 = _sin(nkGSI.gus_cnt);
    if (var_f1 < 0.0f) {
        var_f1 = -var_f1;
    }

    var_t2 = 128;
    if (n == 1) {
        var_t1 = 127.0f * (1.0f - var_f1);
        var_t0 = 127.0f * GameGbl.gus_timer;
        if (var_t0 < var_t1) {
            var_t0 = var_t1;
        }
    } else {
        var_t1 = var_t0 = 128;
    }

    nkGSI.gus_cnt = GameGbl.gus_cnt;
    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
    pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
    pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
    pp->buf[pp->size++].ul64[0] = var_s1->tex0;
    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 1, 0, 1, 0, 1, 1, 0), 0, 3);
    pp->buf[pp->size++].ul64[1] = SCE_GS_UV | SCE_GS_RGBAQ << 4 | SCE_GS_XYZF2 << 8;

    pp->buf[pp->size].ul32[0] = var_s4;
    pp->buf[pp->size].ul32[1] = var_s5;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t1;
    pp->buf[pp->size].ul32[2] = var_t0;
    pp->buf[pp->size].ul32[3] = var_s8;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 - var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_sp0 - var_s6;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_s4 + var_s1->pw;
    pp->buf[pp->size].ul32[1] = var_s5;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t1;
    pp->buf[pp->size].ul32[2] = var_t0;
    pp->buf[pp->size].ul32[3] = var_s8;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 + var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_sp0 - var_s6;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_s4;
    pp->buf[pp->size].ul32[1] = var_s5 + var_s1->ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t1;
    pp->buf[pp->size].ul32[2] = var_t0;
    pp->buf[pp->size].ul32[3] = var_s8;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 - var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_sp0 + var_s6;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = var_s4 + var_s1->pw;
    pp->buf[pp->size].ul32[1] = var_s5 + var_s1->ph;
    pp->buf[pp->size].ul32[2] = 0;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
    pp->buf[pp->size].ul32[0] = var_t2;
    pp->buf[pp->size].ul32[1] = var_t1;
    pp->buf[pp->size].ul32[2] = var_t0;
    pp->buf[pp->size].ul32[3] = var_s8;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + var_s7 + var_s3;
    pp->buf[pp->size].ul32[1] = 0x7900 + var_sp0 + var_s6;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
}

void DrawEngSysMeterBar(nkGifPacket *pp) {
    s32 var_t1;
    f32 var_f1;
    s32 var_a2;
    s32 var_t3;
    s32 num_0x1700 = 0x1700;

    var_t1 = 64;
    var_f1 = GameGbl.gus_timer;
    var_a2 = 1872.0f * var_f1;
    var_t3 = 127.0f * (1.0f - var_f1);

    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 0, 0, 1, 1, 0), 0, 2);
    pp->buf[pp->size++].ul64[1] = SCE_GS_RGBAQ | SCE_GS_XYZF2 << 4;

    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + num_0x1700;
    pp->buf[pp->size].ul32[1] = 0x7A10;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = var_t3;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + num_0x1700 + var_a2;
    pp->buf[pp->size].ul32[1] = 0x7A10;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = 128;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + num_0x1700;
    pp->buf[pp->size].ul32[1] = 0x7AD8;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;

    pp->buf[pp->size].ul32[0] = 128;
    pp->buf[pp->size].ul32[1] = 0;
    pp->buf[pp->size].ul32[2] = var_t3;
    pp->buf[pp->size].ul32[3] = var_t1;
    pp->size++;
    pp->buf[pp->size].ul32[0] = 0x6C00 + num_0x1700 + var_a2;
    pp->buf[pp->size].ul32[1] = 0x7AD8;
    pp->buf[pp->size].ul32[2] = 0xFFFFF;
    pp->buf[pp->size].ul32[3] = 0;
    pp->size++;
}

void DrawGameScreen() {
    nkGifPacket packet;
    nkGifPacket *pp;
    s32 heart_draw;
    s32 yume_draw;
    s32 zanki_draw;
    s32 kagi_draw;
    s32 mome_draw;
    s32 yumekake;
    s32 zanki;
    s32 var_s3;
    s32 var_s1;
    s32 var_a3;
    s32 boss_flag;

    heart_draw = 0;
    yume_draw = 0;
    zanki_draw = 0;
    kagi_draw = 0;
    boss_flag = 0;
    mome_draw = 0;

    var_s3 = GameGbl.vision >> 8;
    if (GameGbl.kloLife[0] > 3) {
        GameGbl.kloLife[0] = 3;
    }

    if (hr_pt_check() == 1) {
        hr_pall_aftdraw();
        pp = &packet;
        pp->size = 0;
        pp->buf = (nkQWdata *)0x70000000;
        pp->buf[pp->size].ul128 = 0;
        pp->size++;
        pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 0, 0, 0, 0, 1);
        pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
        pp->buf[pp->size].ul64[1] = SCE_GS_TEST_2;
        pp->buf[pp->size++].ul64[0] = SCE_GS_SET_TEST(0, 6, 1, 0, 0, 0, 1, 1);
        pp->buf[pp->size].ul64[1] = SCE_GS_CLAMP_2;
        pp->buf[pp->size++].ul64[0] = SCE_GS_SET_CLAMP(1, 1, 0, 0, 0, 0);
        pp->buf[pp->size].ul64[1] = SCE_GS_ALPHA_2;
        pp->buf[pp->size++].ul64[0] = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
        pp->buf[pp->size].ul64[1] = SCE_GS_ZBUF_2;
        pp->buf[pp->size++].ul64[0] = SCE_GS_SET_ZBUF(0x8C, 58, 1);
        nkDrawTimer(pp);

        pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
        pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
        pp->buf[pp->size].ul64[1] = SCE_GS_ZBUF_2;
        pp->buf[pp->size++].ul64[0] = SCE_GS_SET_ZBUF(0x8C, 58, 0);
        pp->buf[0].ul32[0] = 0x70000000 | (pp->size - 1);
        sceDmaSend(DmaChGIF, (void *)((u32)pp->buf | 0x80000000));
        sceDmaSync(DmaChGIF, 0, 0);
        return;
    }

    if (GameGbl.vision >= 0x2202 && GameGbl.vision <= 0x2206) {
        if (nkGameDispFlag) {
            yume_draw = 1;
            zanki_draw = 1;
            nkGSI.R1time = 8.0f;
        } else {
            yume_draw = 0;
            zanki_draw = 0;
            nkGSI.R1time = 0.0f;
            boss_flag = 1;
        }
    } else if (GameGbl.vision >> 8 >= 20 && GameGbl.vision >> 8 <= 29) {
        heart_draw = 1;
        boss_flag = 1;
    } else {
        if (GameGbl.vision >> 8 >= 30 && GameGbl.vision >> 8 <= 40) {
            return;
        }
        heart_draw = 1;
        yume_draw = 1;
    }

    nkGsSetEffectFZ2();
    nkWipeEffBClear();
    nkGsSetNormalFZ2();

    pp = &packet;
    pp->size = 0;
    pp->buf = (nkQWdata *)0x70000000;
    pp->buf[pp->size].ul128 = 0;
    pp->size++;
    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(4, 0, 0, 0, 0, 1);
    pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
    pp->buf[pp->size].ul64[1] = SCE_GS_TEST_2;
    pp->buf[pp->size++].ul64[0] = SCE_GS_SET_TEST(0, 6, 1, 0, 0, 0, 1, 1);
    pp->buf[pp->size].ul64[1] = SCE_GS_CLAMP_2;
    pp->buf[pp->size++].ul64[0] = SCE_GS_SET_CLAMP(1, 1, 0, 0, 0, 0);
    pp->buf[pp->size].ul64[1] = SCE_GS_ALPHA_2;
    pp->buf[pp->size++].ul64[0] = SCE_GS_SET_ALPHA(0, 1, 0, 1, 0);
    pp->buf[pp->size].ul64[1] = SCE_GS_ZBUF_2;
    pp->buf[pp->size++].ul64[0] = SCE_GS_SET_ZBUF(0x8C, 58, 1);

    if (heart_draw) {
        pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
        pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
        pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
        pp->buf[pp->size++].ul64[0] = SCE_GS_SET_TEX0(0x2640, 6, 20, 8, 8, 1, 0, 0x27D5, 0, 0, 0, 1);
        DrawPrimGT4_2(pp, 0x100, 0x80, 0, 0xC80, 0xA40, 0x300);
    }

    if (kzCheckGameClear() == 1 && var_s3 > 29) {
        yume_draw = 0;
    }
    if (yume_draw) {
        pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
        pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
        pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
        pp->buf[pp->size++].ul64[0] = SCE_GS_SET_TEX0(0x2300, 8, 20, 9, 10, 1, 0, 0x2626, 0, 0, 0, 1);
        DrawPrimGT4_2(pp, 0x2040, 0x80, 0x16c0, 0x2cc0, 0x640, 0x200);

        if (!kzCheckGameClear()) {
            if (OkDPcnt() > nkGSI.ytime) {
                nkGSI.ytime += 0.125f;
            }
            yumekake = nkGSI.ytime;
            pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
            pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
            pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
            pp->buf[pp->size++].ul64[0] = SCE_GS_SET_TEX0(0x2300, 8, 20, 9, 9, 1, 0, 0x2632, 0, 0, 0, 1);
            DrawPrimGT4_2(pp, 0x2300, 0xc0, ((yumekake / 10) % 10) * 0x100 + 0x1600, 0x16c0, 0x100, 0x140);
            DrawPrimGT4_2(pp, 0x2400, 0xc0, (yumekake % 10) * 0x100 + 0x1600, 0x16c0, 0x100, 0x140);
        } else {
            var_s1 = 0;
            yumekake = OkDPSTcnt();
            if (yumekake > 149 && nkGfGet(31) == 0 && !(GameGbl.flag & 0x18)) {
                hSeKeyOn(0xc80c81ca0030d, NULL, 0);
                nkGfSetON(31);
            }
            if (yumekake > nkGSI.ytime) {
                nkGSI.ytime += 0.125f;
            }
            yumekake = nkGSI.ytime;
            pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
            pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
            pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
            pp->buf[pp->size++].ul64[0] = SCE_GS_SET_TEX0(0x2300, 8, 20, 9, 9, 1, 0, 0x2632, 0, 0, 0, 1);
            if (yumekake > 99) {
                var_s1 = 128;
                DrawPrimGT4_2(pp, 0x2280, 0xc0, ((yumekake / 100) % 10) * 0x100 + 0x1600, 0x16c0, 0x100, 0x140);
            }
            DrawPrimGT4_2(pp, var_s1 + 0x2300, 0xc0, ((yumekake / 10) % 10) * 0x100 + 0x1600, 0x16c0, 0x100, 0x140);
            DrawPrimGT4_2(pp, var_s1 + 0x2400, 0xc0, (yumekake % 10) * 0x100 + 0x1600, 0x16c0, 0x100, 0x140);
        }
    }

    if (nkGSI.k0zanki != GameGbl.kloZanki[0]) {
        if (GameGbl.kloZanki[0] > 99) {
            GameGbl.kloZanki[0] = 99;
        }
        nkGSI.ztime = 60.0f;
        nkGSI.k0zanki = GameGbl.kloZanki[0];
    }

    if ((PAD_LVL_R1(GameGbl.kpd[0]) && !GameGbl.playdemo_flag) || GameGbl.pause_flag) {
        nkGSI.R1time = 60.0f;
    }

    if (nkGSI.R1time > 0.0f) {
        nkGSI.R1time--;
        if (nkGSI.R1time > 0.0f) {
            zanki_draw = 1;
        }
    }
    if (GameGbl.vision >= 0x2202 && GameGbl.vision <= 0x2206 && !nkGameDispFlag) {
        zanki_draw = 0;
        nkGSI.R1time = 0.0f;
    }
    if (nkGSI.ztime > 0.0f) {
        if (!GameGbl.pause_flag) {
            nkGSI.ztime--;
        }
        zanki_draw = 1;
    }
    if (var_s3 == 18 || var_s3 == 19 || kzCheckGenju() == 1) {
        zanki_draw = 0;
    }
    yumekake = 0xC0;
    if (zanki_draw) {
        if (!boss_flag) {
            yumekake = 0;
        }
        pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
        pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
        pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
        pp->buf[pp->size++].ul64[0] = SCE_GS_SET_TEX0(0x2300, 8, 20, 9, 10, 1, 0, 0x2626, 0, 0, 0, 1);
        DrawPrimGT4_2(pp, 0x2040, yumekake + 0x180, 0x16c0, 0x2cc0, 0x640, 0x200);

        var_a3 = GameGbl.kloZanki[0] > 99 ? 99 : GameGbl.kloZanki[0];
        if (var_a3 >= 0) {
            s32 div = var_a3 / 10;
            s32 yeet; // LMAOOOOOO
            if (div > 10) {
                div = 9;
            }
            pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 0, 0, 0, 0, 1);
            pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
            pp->buf[pp->size].ul64[1] = SCE_GS_TEX0_2;
            pp->buf[pp->size++].ul64[0] = SCE_GS_SET_TEX0(0x2300, 8, 20, 9, 9, 1, 0, 0x2632, 0, 0, 0, 1);
            yeet = var_a3 / 10;
            DrawPrimGT4_2(pp, 0x2300, yumekake + 0x1c0, (div) * 0x100 + 0x1600, 0x16c0, 0x100, 0x140);
            DrawPrimGT4_2(pp, 0x2400, yumekake + 0x1c0, (var_a3 % 10) * 0x100 + 0x1600, 0x16c0, 0x100, 0x140);
        }
    }

    if (nkGSI.k0exitm != OkEXcnt()) {
        nkGSI.extime = 120.0f;
        nkGSI.k0exitm = OkEXcnt();
        nkGSI.mm_rot = 0.0f;
        if (nkGSI.k0exitm > 5) {
            nkGSI.extime += 60.0f;
        }
    }
    if (nkGSI.extime > 0.0f) {
        nkGSI.extime--;
        mome_draw = 1;
    }
    if (nkGSI.R1time > 0.0f && nkGSI.R1time > nkGSI.extime) {
        mome_draw = 1;
        nkGSI.extime = nkGSI.R1time;
    }
    if (GameGbl.vision >> 8 >= 18) {
        mome_draw = 0;
    }
    if (mome_draw) {
        DrawMomePanel(pp, 0x2080, 0x2c0, &stj_ti, 0);
        nkGSI.mm_rot += M_PIf / 12.0f;
        nkGSI.mm_rot = nkRadMask(nkGSI.mm_rot);
        DrawMomePanel(pp, 0x2120, 0x300, &mm_ti[0], 1);
        DrawMomePanel(pp, 0x22a0, 0x300, &mm_ti[1], 2);
        DrawMomePanel(pp, 0x2420, 0x300, &mm_ti[2], 3);
        DrawMomePanel(pp, 0x2120, 0x3c0, &mm_ti[3], 4);
        DrawMomePanel(pp, 0x22a0, 0x3c0, &mm_ti[4], 5);
        DrawMomePanel(pp, 0x2420, 0x3c0, &mm_ti[5], 6);
        if (nkGSI.k0exitm == 6 && nkGSI.extime == 120.0f) {
            hSeKeyOn(0xc80c81ca0030a, NULL, 0);
        }
    }

    if (OkGetKeyID() != -1) {
        kagi_draw = 1;
    }
    if (GameGbl.vision >> 8 >= 20) {
        kagi_draw = 0;
    }
    if (kagi_draw) {
        DrawKagiPanel(pp, 0x190, 0x238, &kys_ti, 0);
    }

    if (GameGbl.vision >> 8 == 12) {
        s32 var_v1 = GameGbl.vision & 0xFF;;
        if (var_v1 == 1 || var_v1 == 3 || var_v1 == 5) {
            DrawGusMeterBar(pp);
            DrawGusMeterPanel(pp, 0x1490, 0x80, &stj_mt, 0);
            DrawGusMeterPanel(pp, 0x14d0, 0xa0, &stj_mk, 1);
        }
    }
    if (GameGbl.vision >> 8 == 15 && nkGetEngSysMeterDisp()) {
        DrawEngSysMeterBar(pp);
        DrawEngSysMeterPanel(pp, 0x1490, 0x80, &stj_mt, 0);
        DrawEngSysMeterPanel(pp, 0x14d0, 0xa0, &stj_mk, 1);
    }
    nkDrawTimer(pp);

    pp->buf[pp->size].ul64[0] = SCE_GIF_SET_TAG(1, 1, 0, 0, 0, 1);
    pp->buf[pp->size++].ul64[1] = SCE_GIF_PACKED_AD;
    pp->buf[pp->size].ul64[1] = SCE_GS_ZBUF_2;
    pp->buf[pp->size++].ul64[0] = SCE_GS_SET_ZBUF(0x8C, 58, 0);
    pp->buf[0].ul32[0] = 0x70000000 | (pp->size - 1);
    sceDmaSend(DmaChGIF, (void *)((u32)pp->buf | 0x80000000));
    sceDmaSync(DmaChGIF, 0, 0);

    Vu0ProgTrans();
    OutLineEnvInit(GameGbl.fr, GameGbl.inter);

    if (heart_draw) {
        DrawGameScrHeart();
    }
    if (yume_draw) {
        DrawGameScrYumekake();
    }
    if (zanki_draw) {
        DrawGameScrZanki(boss_flag);
    }
    if (kagi_draw) {
        DrawGameScrKagi();
    }
}