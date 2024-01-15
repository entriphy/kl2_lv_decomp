#ifndef VU0MAC
#define VU0MAC

#include "nakano.h"

static inline s32 nkScrClip(sceVu0IVECTOR vi) {
    return !(vi[0] <= 0x6C00 || vi[0] >= 0x9400 || vi[1] <= 0x7900 + (GameGbl.inter << 3) || vi[1] >= 0x7900 + (GameGbl.inter << 3) + 0xE00);
}

static inline void qword_Copy(void *dst, void *src) {
    __asm__ volatile(
        "lq    $6,0x0(%1)\n"
        "sq    $6,0x0(%0)\n"
    : : "r" (dst), "r" (src));
}

static inline void vu0_ITOF12Vector(sceVu0FVECTOR dst, sceVu0IVECTOR src) {
    __asm__ volatile(
        ".set noreoder\n"
        "lqc2 $vf8, 0x0(%1)\n"
        "vitof12.xyzw $vf5, $vf8\n"
        "sqc2 $vf5, 0x0(%0)\n"
    : : "r" (dst), "r" (src));
}

static inline void vu0_ITOF0Vector(sceVu0FVECTOR dst, sceVu0IVECTOR src) {
    __asm__ volatile(
        ".set noreoder\n"
        "lqc2 $vf8, 0x0(%1)\n"
        "vitof0.xyzw $vf5, $vf8\n"
        "sqc2 $vf5, 0x0(%0)\n"
    : : "r" (dst), "r" (src));
}

static inline void vu0_ITOF4Vector(sceVu0FVECTOR dst, sceVu0IVECTOR src) {
    __asm__ volatile(
        ".set noreoder\n"
        "lqc2 $vf8, 0x0(%1)\n"
        "vitof4.xyzw $vf5, $vf8\n"
        "sqc2 $vf5, 0x0(%0)\n"
        : : "r" (dst), "r" (src));
}

static inline void vu0_FTOI4Vector(sceVu0IVECTOR dst, sceVu0FVECTOR src) {
    __asm__ volatile(
        ".set noreoder\n"
        "lqc2 $vf8, 0x0(%1)\n"
        "vftoi4.xyzw $vf5, $vf8\n"
        "sqc2 $vf5, 0x0(%0)\n"
    : : "r" (dst), "r" (src));
}

static inline void vu0_FTOI12Vector(sceVu0IVECTOR dst, sceVu0FVECTOR src) {
    __asm__ volatile(
        ".set noreoder\n"
        "lqc2 $vf8, 0x0(%1)\n"
        "vftoi12.xyzw $vf5, $vf8\n"
        "sqc2 $vf5, 0x0(%0)\n"
    : : "r" (dst), "r" (src));
}

static inline void vu0_Square(sceVu0FVECTOR dst, sceVu0FVECTOR src) {
    __asm__ volatile(
        ".set noreoder\n"
        "lqc2 $vf8, 0x0(%1)\n"
        "vmul.xyzw $vf5, $vf8, $vf8\n"
        "sqc2 $vf5, 0x0(%0)\n"
    : : "r" (dst), "r" (src));
}

static inline f32 vu0_SquareRoot(f32 x) {
    f32 ret;
    __asm__ volatile(
        ".set noreoder\n"
        "mfc1 $6, %1\n"
        "qmtc2 $6, $vf4\n"
        "vsqrt Q, $vf4x\n"
        "vwaitq\n"
        "cfc2 $6, $vi22\n"
        "mtc1 $6, %0\n"
    : "=f" (ret) : "f" (x) : "$6");
    return ret;
}

// ?
static inline void vu0_LoadMtx(sceVu0FMATRIX mtx) {
    __asm__ volatile(
        ".set noreoder\n"
        "lqc2 $vf4, 0x0(%0)\n"
        "lqc2 $vf5, 0x10(%0)\n"
        "lqc2 $vf6, 0x20(%0)\n"
        "lqc2 $vf7, 0x30(%0)\n"
    : : "r" (mtx));
}

// ?
static inline void vu0_Thing(sceVu0FVECTOR dst, sceVu0FVECTOR src) {
    __asm__ volatile(
        ".set noreoder\n"
        "lqc2        $vf8, 0x0(%1)\n"
        "vmove.w     $vf8, $vf0\n"
        "vmulax.xyzw ACC, $vf4, $vf8x\n"
        "vmadday.xyzw ACC, $vf5, $vf8y\n"
        "vmaddaz.xyzw ACC, $vf6, $vf8z\n"
        "vmaddw.xyzw $vf12, $vf7, $vf8w\n"
        "vdiv        Q, $vf0w, $vf12w\n"
        "vwaitq\n"
        "vmulq.xyz   $vf12, $vf12, Q\n"
        "sqc2        $vf12, 0x0(%0)\n"
    : : "r" (dst), "r" (src));
}

static inline f32 nkRadMask(f32 rad) {
    if (rad <= -M_PIf)
        rad += M_TWOPIf;
    else if (rad > M_PIf)
        rad -= M_TWOPIf;
    return rad;
}

#endif