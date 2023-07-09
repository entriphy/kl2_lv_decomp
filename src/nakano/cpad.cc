#include "common.h"

static CPadSetup PadPortSlot[2] = {};
static u8 align_dualshock[6] = {};
static u8 align_jogcon[6] = {};
static s32 analog_to_degital[9] = {};
s32 _12CPressButton$default_pressure;
static s32 PadPressKeyAssignFromDigital[12] = {};
CPadControl pad;
static u128 PadDmaBuffer[2][16];

inline void revise_paif(f32 &pi) {
    while (pi < -M_PIf)
        pi += M_TWOPIf;
    while (pi > M_PIf)
        pi -= M_TWOPIf;
}

inline s32 rad2dirf(f32 radian) {
    revise_paif(radian);
    return ((s32)((radian + M_PIf) * 16.0f / M_TWOPIf) + 9 + 16 & 0xF) >> 1;
}

void CAnalogPad::Makeup(s32 h, s32 v) {
    this->ih = h - 0x80;
    this->iv = v - 0x80;
    if (this->ih == 0 && this->iv == 0) {
        this->angle = 0.0f;
        this->scalar = 0.0f;
        this->lscalar = 0.0f;
        this->direction = 0;
        this->fh = 0.0f;
        this->fv = 0.0f;
    } else {
        if (this->ih > 0)
            this->ih++;
        if (this->iv > 0)
            this->iv++;
        this->angle = atan2f(this->iv, this->ih);
        this->scalar = sqrtf(this->ih * this->ih + this->iv * this->iv);
        this->lscalar = this->scalar < 80.0f ? 0.0f : (this->scalar < 128.0f ? this->scalar : 128.0f);
        this->fh = cosf(this->angle) * this->lscalar;
        this->fv = sinf(this->angle) * this->lscalar;
        if (this->scalar > 64.0f) {
            this->direction = rad2dirf(this->angle) + 1;
            this->ldirection = this->direction;
            this->langle = this->angle;
        } else {
            this->direction = 0;
        }
    }
}

void CAnalogPad::AttachFrom(s32 lvl) {
    s32 h = 0;
    h = (lvl & 0x8000) == 0 ? ((lvl & 0x2000) != 0 ? 0xFF : 0x80) : 0;

    s32 v = 0;
    v = (lvl & 0x1000) == 0 ? ((lvl & 0x4000) != 0 ? 0xFF : 0x80) : 0;
    
    Makeup(h, v);
}

void CPadControl::Init() {
    // TODO
}

void CPadControl::Open() {
    // TODO
}

void CPadControl::Close() {
    // TODO
}

void CPadControl::Update(bool skip) {
    // TODO
}

void CPadControl::Clear() {
    // TODO
}

void CPadControl::ClearAll() {
    // TODO
}
