#include "nakano/vtmrg.h"
#include "nakano/main.h"

void nkVT_LoadVisionData(vtDataPtr *data, u32 vision) {
    s32 i;

    data->wave_data_buf = vtWaveAdr;
    data->spray_data_buf = vtSprayAdr;
    for (i = 0; i < 8; i++) {
        data->movie_data_buf[i] = vtMovieAdr[i];
    }
}

void nkVT_Exec() {
    vtExecWaveBase();
    vtExecWaveEff();
}

void nkVT_ExecMovie() {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (i != 7) {
            vtExecMovieDecode(i);
        }
    }
    vtExecFire();
}

// TODO: Remove this once implemented
extern sceGsTex0 OkGetTex0(s32 index);

void nkVT_InitWave() {
    vtDataPtr data_ptr;
    vtUnkStruct unk_struct;
    s32 index;

    nkVT_LoadVisionData(&data_ptr,GameGbl.vision);
    if (GameGbl.vision >> 8 == 1) {
        index = abGetTexIndex(55);
        if (index > 0) {
            data_ptr.tex0[0] = OkGetTex0(index);
        }
        index = abGetTexIndex(56);
        if (index > 0) {
            data_ptr.tex0[1] = OkGetTex0(index);
        }
        index = abGetTexIndex(57);
        if (index > 0) {
            data_ptr.tex0[2] = OkGetTex0(index);
        }
        index = abGetTexIndex(54);
        if (index > 0) {
            data_ptr.tex0[3] = OkGetTex0(index);
        }
    }
    vtInitWave(&data_ptr);

    unk_struct.field_0x04 = 100;
    unk_struct.field_0x08 = 10;
    unk_struct.field_0x0c = 0;
    unk_struct.field_0x10 = 0;
    unk_struct.field_0x14 = 0;
    unk_struct.field_0x18 = 200.0f;
    unk_struct.field_0x1c = 3000.0f;
    unk_struct.field_0x20 = 200.0f;
    unk_struct.field_0x24 = 2000.0f;
    unk_struct.field_0x28 = 2500.0f;
    unk_struct.field_0x2c = 3000.0f;
    if (OkVTFireInitAreaChk()) {
        unk_struct.field_0x00 = 1;
    } else {
        unk_struct.field_0x00 = 0;
    }
    vtInitFire(&unk_struct);
}
