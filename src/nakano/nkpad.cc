#include "nkpad.h"
#include "cpad.h"

extern "C" {

void nkInitPad() {
    pad.Init(); pad.Open();
}

s32 nkGetPr3ih(s32 id) {
    return pad.r3ih(id);
}

s32 nkGetPr3iv(s32 id) {
    return pad.r3iv(id);
}

f32 nkGetPr3fh(s32 id) {
    return pad.r3fh(id);
}

f32 nkGetPr3fv(s32 id) {
    return pad.r3fv(id);
}

s32 nkGetPr3dir(s32 id) {
    return pad.r3dir(id);
}

f32 nkGetPr3scalar(s32 id) {
    return pad.r3scalar(id);
}

f32 nkGetPr3lscalar(s32 id) {
    return pad.r3lscalar(id);
}

f32 nkGetPr3angle(s32 id) {
    return pad.r3angle(id);
}

s32 nkGetPl3ih(s32 id) {
    return pad.l3ih(id);
}

s32 nkGetPl3iv(s32 id) {
    return pad.l3iv(id);
}

f32 nkGetPl3fh(s32 id) {
    return pad.l3fh(id);
}

f32 nkGetPl3fv(s32 id) {
    return pad.l3fv(id);
}

s32 nkGetPl3dir(s32 id) {
    return pad.l3dir(id);
}

f32 nkGetPl3scalar(s32 id) {
    return pad.l3scalar(id);
}

f32 nkGetPl3lscalar(s32 id) {
    return pad.l3lscalar(id);
}

f32 nkGetPl3angle(s32 id) {
    return pad.l3angle(id);
}

s32 nkGetPlvl(s32 id) {
    return (s32)pad.lvl(id);
}

s32 nkGetPtrg(s32 id) {
    return (s32)pad.trg(id);
}

s32 nkGetPrep(s32 id) {
    return (s32)pad.rep(id);
}

s32 nkGetPlooks(s32 id) {
    return pad.looks(id);
}

void nkGetPad() {
    pad.Update(false);
}

}
