#include <cmath>
#include <eetypes.h>
#include <libpad.h>
#include "cpad.h"

static CPadSetup PadPortSlot[2] = {};
static u8 align_dualshock[6] = {};
static u8 align_jogcon[6] = {};
static s32 analog_to_degital[9] = {};
static s32 PadPressKeyAssignFromDigital[12] = {};
CPadControl pad;
s32 CPressButton::default_pressure = 255;
static u128 PadDmaBuffer[2][16];

#pragma region CAnalogPad

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

#pragma endregion CAnalogPad

#pragma region CBasicPad

void CBasicPad::Open(void *buff) {
    while (scePadPortOpen(PadPortSlot[this->id].port, PadPortSlot[this->id].slot, (u128 *)buff) != 1);
}

void CBasicPad::Close() {
    while (scePadPortClose(PadPortSlot[this->id].port, PadPortSlot[this->id].slot) != 1);
}

void CBasicPad::Clear(s32 mode) {
    if (mode == 0) {
        this->pad_status = 0;
        this->phase = 0;
        this->kind = this->info_mode = this->info_mode_ex = 0;
        this->fix_pad_status = this->kind;
        this->looks = 0;
        this->exec_status = 0x69;
        for (s32 i = 0; i < 6; i++)
            this->act_data[i] = 0;
        for (s32 j = 0; j < 12; j++)
            this->pb[j].Clear();
        this->error_count = 0;
    }
    this->lvl = 0;
    this->lvl_last = 0;
    this->trg = 0;
    this->rep = 0;
    this->rep_last = 0;
    this->repcnt = 0;
    this->repmask = 0xF0FF;
    this->repf = 21;
    this->reps = 6;
}

// TODO: Not matching
void CBasicPad::Update(bool skip) {
    if (!skip)
        Read();
    if (this->exec_status & 0x100) {
        this->lvl_last = this->lvl;
        this->lvl = (this->raw[3] | this->raw[2] << 8) ^ 0xFFFF;
        s32 get_digit = 0;
        switch (this->info_mode) {
            case 5:
            case 7:
                this->analog[0].Makeup(this->raw[4], this->raw[5]);
                this->analog[1].Makeup(this->raw[6], this->raw[7]);
                if (this->exec_status & 0x10) {
                    for (s32 i = 0; i < 12; i++) {
                        this->pb[i].SetPressure(this->raw[i + 8]);
                    }
                }
                break;
            case 2:
                this->neg.angle = this->raw[4];
                for (s32 i = 0; i < 3; i++) {
                    this->neg.pb[i].SetPressure(this->raw[i + 5]);
                }
                if (this->neg.pb[0].GetPressure() >= 64) {
                    this->lvl |= 0x40;
                }
                if (this->neg.pb[1].GetPressure() >= 64) {
                    this->lvl |= 0x80;
                }
                if (this->neg.pb[2].GetPressure() >= 64) {
                    this->lvl |= 0x4;
                }
                get_digit = 1;
                break;
            default:
                get_digit = 1;
                break;
        }

        if (get_digit != 0) {
            this->analog[0].Makeup(0x80, 0x80);
            this->analog[1].Makeup(0x80, 0x80);
        }
        DTOA();
        if (get_digit == 0) {
            if (this->analog[1].lscalar != 0.0f && this->exec_status & 1 && !(this->lvl & 0xF000)) {
                if (this->analog[1].direction != 0) {
                    this->lvl |= analog_to_degital[this->analog[1].direction];
                }
            }
        }
        if ((*(u64 *)&this->id & 0x1400000000) == 0x400000000) {
            for (s32 i = 0; i < 12; i++) {
                this->pb[i].SetPressureMask(this->lvl & PadPressKeyAssignFromDigital[i]);
            }
        }
    } else {
        this->lvl_last = 0;
        this->lvl = 0;
        this->analog[0].Makeup(0x80, 0x80);
        this->analog[1].Makeup(0x80, 0x80);
        switch (this->info_mode) {
            case 0:
            case 5:
            case 7:
                if (this->exec_status & 0x10) {
                    for (s32 i = 0; i < 12; i++) {
                        this->pb[i].SetPressure(0);
                    }
                }
                break;
            case 2:
                this->neg.angle = 0;
                for (s32 i = 0; i < 3; i++) {
                    this->neg.pb[i].SetPressure(0);
                }
                break;
        }
    }

    this->trg = (this->lvl_last ^ this->lvl) & this->lvl;
    this->rep = this->lvl & this->repmask;
    if (this->rep != 0) {
        this->repcnt++;
        if (this->repcnt != 1) {
            if (this->repcnt < this->repf) {
                this->rep = 0;
            } else {
                this->repcnt -= this->reps;
            }
        }
    } else {
        this->repcnt = 0;
        this->rep_last = 0;
    }
}

// TODO: Not matching
void CBasicPad::Read() {
    this->exec_status &= -257;
    s32 port = PadPortSlot[this->id].port;
    s32 slot = PadPortSlot[this->id].slot;
    this->pad_status = scePadGetState(port, slot);
    switch (this->pad_status) {
        case 0:
            Clear(0);
            break;
        case 7:
            this->error_count++;
            this->phase = 0;
            break;
        case 1:
        case 5:
            return;
        case 2:
        case 6:
            if (GetModeID() != this->fix_pad_status) {
                this->phase = 0;
            }
        default:
            switch (this->phase) {
                case 0x100:
                    this->stable_counter = 0;
                    this->phase = 0x101;
                case 0x101:
                    if (this->stable_counter < 8) {
                        this->stable_counter++;
                    } else {
                        if (scePadRead(port, slot, this->raw) != 0) {
                            this->exec_status |= 0x100;
                            if ((*(u64 *)&this->id & 0xC000000000) == 0xC000000000) {
                                if (this->kind == 0x300) {
                                    scePadSetActDirect(port, slot, this->jog.act_data);
                                } else {
                                    scePadSetActDirect(port, slot, this->act_data);
                                }
                            }
                        }
                    }
                    break;
                case 0x0:
                    this->fix_pad_status = this->kind = GetModeID();
                    if (this->pad_status == 2) {
                        switch (this->kind) {
                            case 2:
                                SetLooks(PAD_LOOKS_NEGCON);
                                this->phase = 0x100;
                                this->exec_status &= -9;
                                break;
                            case 5:
                                this->phase = 0x200;
                                break;
                            default:
                                SetLooks(PAD_LOOKS_DEGITAL);
                                this->phase = 0x100;
                                this->exec_status |= 8;
                                break;
                        }
                    } else {
                        this->exec_status &= -9;
                        switch (this->kind) {
                            case 5:
                                SetLooks(PAD_LOOKS_ANAST);
                                this->phase = 0x200;
                                break;
                            case 7:
                                this->phase = 0x20;
                                break;
                            case 768:
                                this->phase = 0x40;
                                break;
                            default:
                                this->exec_status |= 8;
                                s32 max = scePadInfoAct(port, slot, -1, 0);
                                s32 func = scePadInfoAct(port, slot, 0, 1);
                                s32 sub = scePadInfoAct(port, slot, 0, 2);
                                if (max == 0) {
                                    SetLooks(PAD_LOOKS_ANAST);
                                    this->kind = 5;
                                    this->phase = 256;
                                } else if (max == 1 && func == 4 && sub == 3) {
                                    this->kind = 768;
                                    this->phase = 64;
                                } else if (max == 2) {
                                    this->kind = 7;
                                    this->phase = 32;
                                } else {
                                    SetLooks(PAD_LOOKS_UNKNOWN);
                                    this->kind = 0;
                                    this->phase = 512;
                                }
                                break;
                        }
                    }
                    break;
                case 0x10:
                    if (scePadSetMainMode(port, slot, 1, 0) == 1) {
                        this->phase++;
                    }
                    break;
                case 0x11:
                    if (scePadGetReqState(port, slot) == 1) {
                        this->phase--;
                    }
                    if (scePadGetReqState(port, slot) == 0) {
                        this->phase = 0;
                    }
                    break;
                case 0x20:
                    if (scePadInfoPressMode(port, slot) != 1) {
                        this->SetLooks(PAD_LOOKS_DUALSHOCK);
                    } else {
                        this->SetLooks(PAD_LOOKS_DUALSHOCK2);
                    }
                    this->exec_status |= 0x80;
                    if (scePadSetActAlign(port, slot, align_dualshock) == 1) {
                        this->phase++;
                    }
                    break;
                case 0x30:
                    if (scePadInfoPressMode(port, slot) == 1) {
                        this->exec_status |= 0x10;
                        this->phase++;
                        if (scePadEnterPressMode(port, slot) == 1) {
                            this->phase++;
                        }
                    } else {
                        this->phase = 256;
                    }
                    break;

                case 0x38:
                    if (scePadInfoPressMode(port, slot) != 1) {
                        this->phase = 256;
                    } else {
                        this->exec_status &= -17;
                        this->phase++;
                    }
                    break;
                case 0x39:
                    if (scePadExitPressMode(port, slot) == 1) {
                        this->phase++;
                    }
                    break;
                case 0x32:
                case 0x3A:
                    if (scePadGetReqState(port, slot) == 1) {
                        this->phase--;
                    }
                    if (scePadGetReqState(port, slot) == 0) {
                        this->phase = 256;
                    }
                    break;
                case 0x40:
                    this->exec_status |= 0x80;
                    this->SetLooks(PAD_LOOKS_JOCON);
                    if (scePadSetActAlign(port, slot, align_jogcon) == 1) {
                        this->phase++;
                    }
                    break;
                case 0x41:
                    if (scePadGetReqState(port, slot) == 1) {
                        this->phase--;
                    }
                    if (scePadGetReqState(port, slot) == 0) {
                        this->phase = 256;
                    }
                    break;
                default:
                    break;
            }
            break;
    }
}

s32 CBasicPad::GetModeID() {
    this->info_mode = scePadInfoMode(PadPortSlot[this->id].port, PadPortSlot[this->id].slot, 1, 0);
    if (this->info_mode != 0) {
        this->info_mode_ex = scePadInfoMode(PadPortSlot[this->id].port, PadPortSlot[this->id].slot, 2, 0);
        if (this->info_mode_ex < 1) {
            return this->info_mode;
        }
        // what
    } else {
        return 0;
    }
}

void CBasicPad::SetPressureMode(bool &b) {
    if ((this->exec_status & 16) != b) {
        if (b)
            this->phase = 48;
        else
            this->phase = 56;
    }
}

#pragma endregion CBasicPad

#pragma region CPadControl

// TODO: Not matching
void CPadControl::Init() {
    while (scePadInit(0) != 1);

    for (s32 id = 0; id < 2; id++) {
        this->paddata[id].Setup(id);
    }
}

void CPadControl::Open() {
    for (s32 id = 0; id < 2; id++) {
        this->paddata[id].Open(PadDmaBuffer[id]);
    }
    ClearAll(0);
}

void CPadControl::Close() {
    for (s32 id = 0; id < 2; id++) {
        this->paddata[id].Close();
    }
}

void CPadControl::Update(bool skip) {
    for (s32 id = 0; id < 2; id++) {
        this->paddata[id].Update(skip);
    }
}

void CPadControl::Clear(s32 id, s32 mode) {
    this->paddata[id].Clear(mode);
}

void CPadControl::ClearAll(s32 mode) {
    for (s32 id = 0; id < 2; id++) {
        this->paddata[id].Clear(mode);
    }
}

void CBasicPad::DTOA() {
    if (this->analog[1].lscalar == 0.0f && this->exec_status & 2 && this->lvl & 0xF000) {
        this->analog[1].AttachFrom(this->lvl);
    }
}

#pragma endregion CPadControl
