#ifndef VU0MAC
#define VU0MAC

#include "nakano.h"

static inline void vu0_ITOF12Vector(sceVu0FVECTOR dst, sceVu0IVECTOR src) {
    __asm__ volatile(
        "lqc2 $vf8, 0x0(%1)\n"
        "vitof12.xyzw $vf5, $vf8\n"
        "sqc2 $vf5, 0x0(%0)\n"
        : : "r" (dst), "r" (src));
}

static inline void vu0_ITOF0Vector(sceVu0FVECTOR dst, sceVu0IVECTOR src) {
    __asm__ volatile(
        "lqc2 $vf8, 0x0(%1)\n"
        "vitof0.xyzw $vf5, $vf8\n"
        "sqc2 $vf5, 0x0(%0)\n"
        : : "r" (dst), "r" (src));
}

static inline void vu0_FTOI4Vector(sceVu0IVECTOR dst, sceVu0FVECTOR src) {
    __asm__ volatile(
        "lqc2 $vf8, 0x0(%1)\n"
        "vftoi4.xyzw $vf5, $vf8\n"
        "sqc2 $vf5, 0x0(%0)\n"
        : : "r" (dst), "r" (src));
}

// ?
static inline void vu0_LoadMtx(sceVu0FMATRIX mtx) {
    __asm__ volatile(
        "lqc2 $vf4, 0x0(%0)\n"
        "lqc2 $vf5, 0x10(%0)\n"
        "lqc2 $vf6, 0x20(%0)\n"
        "lqc2 $vf7, 0x30(%0)\n"
        : : "r" (mtx));
}

// ?
static inline void vu0_Thing(sceVu0FVECTOR dst, sceVu0FVECTOR src) {
    __asm__ volatile(
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