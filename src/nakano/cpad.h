#ifndef CPAD_H
#define CPAD_H

#include "nakano.h"

class CAnalogPad { // 0x28
public:
    /* 0x00 */ s32 ih;
    /* 0x04 */ s32 iv;
    /* 0x08 */ s32 direction;
    /* 0x0c */ f32 angle;
    /* 0x10 */ f32 scalar;
    /* 0x14 */ f32 lscalar;
    /* 0x18 */ f32 fh;
    /* 0x1c */ f32 fv;
    /* 0x20 */ s32 ldirection;
    /* 0x24 */ f32 langle;

    void Makeup(s32 h, s32 v);
    void AttachFrom(s32 lvl);
};

class CPressButton { // 0x8
protected:
    static s32 default_pressure;
    /* 0x0 */ s32 pressure;
    /* 0x4 */ s32 last_pressure;

public:
    void SetDefaultPressure(s32 df) {
        default_pressure = df;
    }

    void Clear() {
        this->pressure = this->last_pressure = 0;
    }

    void SetPressure(s32 press) {
        this->last_pressure = this->pressure;
        this->pressure = press;
    }

    void SetPressureMask(s32 mask) {
        this->last_pressure = this->pressure;
        this->pressure = mask != 0 ? CPressButton::default_pressure : 0;
    }

    s32 GetPressure() {
        return this->pressure;
    }

    s32 GetLastPressure() {
        return this->last_pressure;
    }
};

class CNegPad { // 0x1c
public:
    /* 0x00 */ s32 angle;
    /* 0x04 */ CPressButton pb[3];
};

class CJogPad { // 0x20
public:
    /* 0x00 */ f32 angle;
    /* 0x04 */ s32 rotate;
    /* 0x08 */ s32 direction_flag;
    /* 0x0c */ s32 motor;
    /* 0x10 */ s32 safety_mode;
    /* 0x14 */ s32 safety_cnt;
    /* 0x18 */ u8  act_data[6];
};

typedef enum {
    PAD_LOOKS_EMPTY = 0,
    PAD_LOOKS_DEGITAL = 1,
    PAD_LOOKS_DUALSHOCK = 2,
    PAD_LOOKS_DUALSHOCK2 = 3,
    PAD_LOOKS_ANAST = 4,
    PAD_LOOKS_NEGCON = 5,
    PAD_LOOKS_JOCON = 6,
    PAD_LOOKS_GUN = 7,
    PAD_LOOKS_GUNCON = 8,
    PAD_LOOKS_UNKNOWN = 9
} PadLooks;

class CBasicPad { // 0x1c0
    /* 0x000 */ __attribute__((aligned(64))) u8 raw[64];
protected:
    /* 0x040 */ s32 id;
    /* 0x044 */ s32 exec_status;
    /* 0x048 */ s32 pad_status;
    /* 0x04c */ s32 fix_pad_status;
    /* 0x050 */ s32 phase;
    /* 0x054 */ s32 info_mode;
    /* 0x058 */ s32 info_mode_ex;
    /* 0x05c */ s32 kind;
    /* 0x060 */ s32 stable_counter;
public:
    /* 0x064 */ s32 looks;
    /* 0x068 */ s32 error_count;
    /* 0x06c */ u32 lvl;
    /* 0x070 */ u32 lvl_last;
    /* 0x074 */ u32 trg;
    /* 0x078 */ u32 rep;
    /* 0x07c */ u32 rep_last;
    /* 0x080 */ u32 repcnt;
    /* 0x084 */ u32 repmask;
    /* 0x088 */ u32 repf;
    /* 0x08c */ u32 reps;
    /* 0x090 */ u8  act_data[6];
    /* 0x098 */ CAnalogPad analog[2];
    /* 0x0e8 */ CPressButton pb[12];
    /* 0x148 */ CNegPad neg;
    /* 0x164 */ CJogPad jog;

    void Setup(s32 id) {
        this->id = id;
    }

    void Open(void *buff);
    void Close();
    void Clear(s32 mode);
    void Update(bool skip);
    void SetPressureMode(bool &b);
protected:
    void Read();
    s32 GetModeID();
    void SetLooks(PadLooks l) {
        if (this->looks == PAD_LOOKS_EMPTY) {
            this->looks = l;
        }
    }
    void DTOA();
    void AttachFrom(CAnalogPad *ap);
    void ATOD();
    void DTOP();
};

class CPadControl { // 0x380
    /* 0x000 */ CBasicPad paddata[2];

public:
    void Init();
    void Open();
    void Close();
    void Update(bool skip);
    void Clear(s32 id, s32 mode);
    void ClearAll(s32 mode);
    u32 lvl(s32 id) {
        return (this->paddata + id)->lvl;
    }

    u32 trg(s32 id) {
        return (this->paddata + id)->trg;
    }

    u32 rep(s32 id) {
        return (this->paddata + id)->rep;
    }

    s32 looks(s32 id) {
        return (this->paddata + id)->looks;
    }

    void SetRepeatTime(u32 rep1, u32 rep2, s32 id) {
        (this->paddata + id)->repf = rep1;
        (this->paddata + id)->reps = rep2;
    }

    s32 r3ih(s32 id) {
        return (this->paddata + id)->analog[0].ih;
    }

    s32 r3iv(s32 id) {
        return (this->paddata + id)->analog[0].iv;
    }

    f32 r3fh(s32 id) {
        return (this->paddata + id)->analog[0].fh;
    }

    f32 r3fv(s32 id) {
        return (this->paddata + id)->analog[0].fv;
    }

    s32 r3dir(s32 id) {
        return (this->paddata + id)->analog[0].direction;
    }

    f32 r3scalar(s32 id) {
        return (this->paddata + id)->analog[0].scalar;
    }

    f32 r3lscalar(s32 id) {
        return (this->paddata + id)->analog[0].lscalar;
    }

    f32 r3angle(s32 id) {
        return (this->paddata + id)->analog[0].angle;
    }

    s32 r3h(s32 id) {
        return this->r3ih(id);
    }

    s32 r3v(s32 id) {
        return this->r3iv(id);
    }

    s32 l3ih(s32 id) {
        return (this->paddata + id)->analog[1].ih;
    }

    s32 l3iv(s32 id) {
        return (this->paddata + id)->analog[1].iv;
    }

    f32 l3fh(s32 id) {
        return (this->paddata + id)->analog[1].fh;
    }

    f32 l3fv(s32 id) {
        return (this->paddata + id)->analog[1].fv;
    }

    s32 l3dir(s32 id) {
        return (this->paddata + id)->analog[1].direction;
    }

    f32 l3scalar(s32 id) {
        return (this->paddata + id)->analog[1].scalar;
    }

    f32 l3lscalar(s32 id) {
        return (this->paddata + id)->analog[1].lscalar;
    }

    f32 l3angle(s32 id) {
        return (this->paddata + id)->analog[1].angle;
    }

    s32 l3h(s32 id) {
        return this->l3ih(id);
    }

    s32 l3v(s32 id) {
        return this->l3iv(id);
    }
};

class CPadSetup { // 0x8
public:
    /* 0x0 */ s32 port;
    /* 0x4 */ s32 slot;
};

typedef struct { // 0x20
    /* 0x00 */ u8 is_ok;
    /* 0x01 */ u8 kind;
    /* 0x02 */ u8 data[2];
    /* 0x04 */ u8 etc[28];
} RawPAD;

extern CPadControl pad;

#endif
