#include "common.h"

void TkGetLpMAtrix(sceVu0FMATRIX Mtx, sceVu0FVECTOR Pos, sceVu0FVECTOR Norm, sceVu0FVECTOR Rot) {
    sceVu0FVECTOR v;

    sceVu0OuterProduct(v, Norm, Rot);
    sceVu0ScaleVectorXYZ(v, v, -1.0f);
    sceVu0ScaleVectorXYZ(Rot, Rot, -1.0f);
    sceVu0UnitMatrix(Mtx);
    sceVu0Normalize(Mtx[0], v);
    sceVu0Normalize(Mtx[1], Norm);
    sceVu0Normalize(Mtx[2], Rot);
    sceVu0TransMatrix(Mtx, Mtx, Pos);
}

void GetJetMatrix(sceVu0FMATRIX Mtx, sceVu0FVECTOR VecNow) {
    sceVu0FVECTOR v0, v1;

    sceVu0CopyVector(v0, VecNow);
    v0[1] = 0.0;
    v1[0] = 0.0;
    v1[1] = 1.0;
    v1[2] = 0.0;
    v1[3] = 0.0;
    sceVu0OuterProduct(v1, v0, v1);
    sceVu0OuterProduct(v0, v1, VecNow);
    sceVu0UnitMatrix(Mtx);
    sceVu0Normalize(Mtx[0], VecNow);
    sceVu0Normalize(Mtx[1], v0);
    sceVu0Normalize(Mtx[2], v1);
}

void LinerInterPolateMatrixBuff(sceVu0FMATRIX dm, sceVu0FMATRIX m0, sceVu0FMATRIX m1, s32 num, u64 flag, f32 weight) {
    __asm__ volatile(
        "mfc1    $9,%5\n"
        "qmtc2   $9,vf1\n"
        "vsubx.w vf2w,vf0w,vf1x\n"
    "_Libp_Loop:\n"
        "andi    $10,%4,0xffff\n"
        "dsrl    %4,%4,0x1\n"
        "beq     $0,$10,_Libp_WorkEnd\n"
        
        "lqc2    vf3,0x00(%1)\n"
        "lqc2    vf4,0x10(%1)\n"
        "lqc2    vf5,0x20(%1)\n"
        "lqc2    vf6,0x30(%1)\n"
        
        "lqc2    vf7,0x00(%2)\n"
        "lqc2    vf8,0x10(%2)\n"
        "lqc2    vf9,0x20(%2)\n"
        "lqc2    vf10,0x30(%2)\n"
        
        "vmulaw.xyzw ACC,vf3,vf2\n"
        "vmaddx.xyzw vf11,vf7,vf1\n"
        "vmulaw.xyzw ACC,vf4,vf2\n"
        "vmaddx.xyzw vf12,vf8,vf1\n"
        "vmulaw.xyzw ACC,vf5,vf2\n"
        "vmaddx.xyzw vf13,vf9,vf1\n"
        "vmulaw.xyzw ACC,vf6,vf2\n"
        "vmaddx.xyzw vf14,vf10,vf1\n"

        "vmul.xyz  vf3,vf11,vf11\n"
        "vaddy.x   vf3,vf3,vf3\n"
        "vaddz.x   vf3,vf3,vf3\n"
        "vrsqrt    Q,vf0w,vf3x\n"
        "vsub.xyzw vf15,vf0,vf0\n"
        "vaddw.xyzw vf15,vf15,vf11\n"
        "vwaitq\n"
        "vmulq.xyz vf15,vf11,Q\n"
        
        "vmul.xyz vf19,vf12,vf15\n"
        "vaddy.x vf19,vf19,vf19\n"
        "vaddz.x   vf19,vf19,vf19\n"
        "vmulx.xyzw vf20,vf15,vf19\n"
        "vsub.xyzw vf20,vf12,vf20\n"
        "vmul.xyz vf3,vf20,vf20\n"
        "vaddy.x vf3,vf3,vf3\n"
        "vaddz.x   vf3,vf3,vf3\n"
        "vrsqrt    Q,vf0w,vf3x\n"
        "vsub.xyzw vf16,vf0,vf0\n"
        "vaddw.xyzw vf16,vf16,vf20\n"
        "vwaitq\n"
        "vmulq.xyz vf16,vf20,Q\n"
        
        "vopmula.xyz ACC,vf15,vf16\n"
        "vopmsub.xyz vf17,vf16,vf15\n"
        "vsub.w  vf17,vf17,vf17\n"
        "vmove.xyzw vf18,vf14\n"
        
        "sqc2    vf15,0x00(%0)\n"
        "sqc2    vf16,0x10(%0)\n"
        "sqc2    vf17,0x20(%0)\n"
        "sqc2    vf18,0x30(%0)\n"
    "_Libp_WorkEnd:\n"
        "addi    %0,%0,0x40\n"
        "addi    %1,%1,0x40\n"
        "addi    %2,%2,0x40\n"
        "addi    %3,%3,-1\n"
        "bne     $0,%3,_Libp_Loop\n"
    : : "r" (dm), "r" (m0), "r" (m1), "r" (num), "r" (flag), "f" (weight));
}

void LinerInterPolateMatrix(sceVu0FMATRIX dm, sceVu0FMATRIX m0, sceVu0FMATRIX m1, f32 Weight) {
    __asm__ volatile(
        "mfc1        $7, %3\n"
        "qmtc2       $7, $vf1\n"
        "vsubx.w     $vf2, $vf0, $vf1x\n"
        "lqc2        $vf3, 0x0(%1)\n"
        "lqc2        $vf4, 0x10(%1)\n"
        "lqc2        $vf5, 0x20(%1)\n"
        "lqc2        $vf6, 0x30(%1)\n"
        "lqc2        $vf7, 0x0(%2)\n"
        "lqc2        $vf8, 0x10(%2)\n"
        "lqc2        $vf9, 0x20(%2)\n"
        "lqc2        $vf10, 0x30(%2)\n"
        "vmulaw.xyzw ACC, $vf3, $vf2w\n"
        "vmaddx.xyzw $vf11, $vf7, $vf1x\n"
        "vmulaw.xyzw ACC, $vf4, $vf2w\n"
        "vmaddx.xyzw $vf12, $vf8, $vf1x\n"
        "vmulaw.xyzw ACC, $vf5, $vf2w\n"
        "vmaddx.xyzw $vf13, $vf9, $vf1x\n"
        "vmulaw.xyzw ACC, $vf6, $vf2w\n"
        "vmaddx.xyzw $vf14, $vf10, $vf1x\n"
        "vmul.xyz    $vf3, $vf11, $vf11\n"
        "vaddy.x     $vf3, $vf3, $vf3y\n"
        "vaddz.x     $vf3, $vf3, $vf3z\n"
        "vrsqrt      Q, $vf0w, $vf3x\n"
        "vsub.xyzw   $vf15, $vf0, $vf0\n"
        "vaddw.xyzw  $vf15, $vf15, $vf11w\n"
        "vwaitq\n"
        "vmulq.xyz   $vf15, $vf11, Q\n"
        "vmul.xyz    $vf1, $vf12, $vf15\n"
        "vaddy.x     $vf1, $vf1, $vf1y\n"
        "vaddz.x     $vf1, $vf1, $vf1z\n"
        "vmulx.xyzw  $vf2, $vf15, $vf1x\n"
        "vsub.xyzw   $vf2, $vf12, $vf2\n"
        "vmul.xyz    $vf3, $vf2, $vf2\n"
        "vaddy.x     $vf3, $vf3, $vf3y\n"
        "vaddz.x     $vf3, $vf3, $vf3z\n"
        "vrsqrt      Q, $vf0w, $vf3x\n"
        "vsub.xyzw   $vf16, $vf0, $vf0\n"
        "vaddw.xyzw  $vf16, $vf16, $vf2w\n"
        "vwaitq\n"
        "vmulq.xyz   $vf16, $vf2, Q\n"
        "vopmula.xyz ACC, $vf15, $vf16\n"
        "vopmsub.xyz $vf17, $vf16, $vf15\n"
        "vsub.w      $vf17, $vf17, $vf17\n"
        "vmove.xyzw  $vf18, $vf14\n"
        "sqc2        $vf15, 0x0(%0)\n"
        "sqc2        $vf16, 0x10(%0)\n"
        "sqc2        $vf17, 0x20(%0)\n"
        "sqc2        $vf18, 0x30(%0)\n"
        "nop\n"
    : : "r" (dm), "r" (m0), "r" (m1), "f" (Weight));
}

void InterPolateMatrixLightTmp2(sceVu0FMATRIX dm, sceVu0FMATRIX m0, sceVu0FMATRIX m1, f32 Weight) {
    __asm__ volatile(
        "mfc1        $7, $f12\n"
        "qmtc2       $7, $vf1\n"
        "vsubx.w     $vf2, $vf0, $vf1x\n"
        "lqc2        $vf3, 0x0(%1)\n"
        "lqc2        $vf4, 0x10(%1)\n"
        "lqc2        $vf5, 0x20(%1)\n"
        "lqc2        $vf6, 0x30(%1)\n"
        "lqc2        $vf7, 0x0(%2)\n"
        "lqc2        $vf8, 0x10(%2)\n"
        "lqc2        $vf9, 0x20(%2)\n"
        "lqc2        $vf10, 0x30(%2)\n"
        "vmulaw.xyzw ACC, $vf3, $vf2w\n"
        "vmaddx.xyzw $vf11, $vf7, $vf1x\n"
        "vmulaw.xyzw ACC, $vf4, $vf2w\n"
        "vmaddx.xyzw $vf12, $vf8, $vf1x\n"
        "vmulaw.xyzw ACC, $vf5, $vf2w\n"
        "vmaddx.xyzw $vf13, $vf9, $vf1x\n"
        "vmulaw.xyzw ACC, $vf6, $vf2w\n"
        "vmaddx.xyzw $vf14, $vf10, $vf1x\n"
        "sqc2        $vf11, 0x0(%0)\n"
        "sqc2        $vf12, 0x10(%0)\n"
        "sqc2        $vf13, 0x20(%0)\n"
        "sqc2        $vf14, 0x30(%0)\n"
        "nop\n"
    : : "r" (dm), "r" (m0), "r" (m1), "f" (Weight));
}

static void axis2mat(sceVu0FMATRIX m, sceVu0FVECTOR axis);
void AxisInterPolate(sceVu0FMATRIX DistMtx, sceVu0FMATRIX StartMtx, sceVu0FMATRIX EndMtx, sceVu0FVECTOR Axis, f32 Weight) {
    sceVu0FMATRIX AxisMtx;
    sceVu0FMATRIX TmpMtx;
    sceVu0FVECTOR TmpVector;

    Axis[3] *= Weight;
    axis2mat(AxisMtx, Axis);
    sceVu0CopyMatrix(TmpMtx, StartMtx);
    TmpVector[0] = StartMtx[3][0];
    TmpVector[1] = StartMtx[3][1];
    TmpVector[2] = StartMtx[3][2];
    TmpVector[3] = StartMtx[3][3];
    TmpMtx[3][0] = 0.0f;
    TmpMtx[3][1] = 0.0f;
    TmpMtx[3][2] = 0.0f;
    TmpMtx[3][3] = 0.0f;
    sceVu0MulMatrix(DistMtx, AxisMtx, TmpMtx);
    DistMtx[3][0] = (EndMtx[3][0] - TmpVector[0]) * Weight + TmpVector[0];
    DistMtx[3][1] = (EndMtx[3][1] - TmpVector[1]) * Weight + TmpVector[1];
    DistMtx[3][2] = (EndMtx[3][2] - TmpVector[2]) * Weight + TmpVector[2];
    DistMtx[3][3] = (EndMtx[3][3] - TmpVector[3]) * Weight + TmpVector[3];
}

static void axis2mat(sceVu0FMATRIX m, sceVu0FVECTOR axis) {
    sceVu0FVECTOR xyz[3];
    sceVu0FVECTOR s;
    f32 c;

    sceVu0UnitMatrix(m);
    c = __sin(axis[3]);
    sceVu0ScaleVector(s, axis, c);
    c = __cos(axis[3]);
    sceVu0ScaleVector(xyz[0], axis, axis[0]);
    sceVu0ScaleVector(xyz[1], axis, axis[1]);
    sceVu0ScaleVector(xyz[2], axis, axis[2]);
    m[0][0] = c;
    m[0][1] = s[2];
    m[0][2] = -s[1];
    m[1][0] = -s[2];
    m[1][1] = c;
    m[1][2] = s[0];
    m[2][0] = s[1];
    m[2][1] = -s[0];
    m[2][2] = c;
    c = 1.0f - c;
    sceVu0ScaleVector(xyz[0], xyz[0], c);
    sceVu0ScaleVector(xyz[1], xyz[1], c);
    sceVu0ScaleVector(xyz[2], xyz[2], c);
    m[0][0] += xyz[0][0];
    m[0][1] += xyz[0][1];
    m[0][2] += xyz[0][2];
    m[1][0] += xyz[1][0];
    m[1][1] += xyz[1][1];
    m[1][2] += xyz[1][2];
    m[2][0] += xyz[2][0];
    m[2][1] += xyz[2][1];
    m[2][2] += xyz[2][2];
}

void GetInterPolateAxis(sceVu0FVECTOR Axis, sceVu0FMATRIX StartMtx, sceVu0FMATRIX EndMtx) {
    __asm__ volatile(
    ".data\n"
        "tbl: .float 0.33333334, 0.2, 0.14285715, 0.11111111\n"
    ".text\n"
        "lq      $8,0x00(%1)\n"
        "lq      $9,0x10(%1)\n"
        "lq      $10,0x20(%1)\n"
        "qmfc2   $11,vf0\n"
        "pextlw  $12,$9,$8\n"
        "pextuw  $13,$9,$8\n"
        "pextlw  $14,$11,$10\n"
        "pextuw  $15,$11,$10\n"
        "pcpyld  $8,$14,$12\n"
        "pcpyud  $9,$12,$14\n"
        "pcpyld  $10,$15,$13\n"
        "qmtc2   $8,vf5\n"
        "qmtc2   $9,vf6\n"
        "qmtc2   $10,vf7\n"
        "vsub.xyzw vf8,vf0,vf0\n"
        "lqc2    vf9,0x00(%2)\n"
        "lqc2    vf10,0x10(%2)\n"
        "lqc2    vf11,0x20(%2)\n"
        "lqc2    vf12,0x30(%2)\n"
        
        "vmulax.xyzw ACC,vf9,vf5\n"
        "vmadday.xyzw ACC,vf10,vf5\n"
        "vmaddaz.xyzw ACC,vf11,vf5\n"
        "vmaddw.xyzw vf28,vf12,vf5\n"
        
        "vmulax.xyzw ACC,vf9,vf6\n"
        "vmadday.xyzw ACC,vf10,vf6\n"
        "vmaddaz.xyzw ACC,vf11,vf6\n"
        "vmaddw.xyzw vf29,vf12,vf6\n"
        
        "vmulax.xyzw ACC,vf9,vf7\n"
        "vmadday.xyzw ACC,vf10,vf7\n"
        "vmaddaz.xyzw ACC,vf11,vf7\n"
        "vmaddw.xyzw vf30,vf12,vf7\n"

        "vmulax.xyzw ACC,vf9,vf8\n"
        "vmadday.xyzw ACC,vf10,vf8\n"
        "vmaddaz.xyzw ACC,vf11,vf8\n"
        "vmaddw.xyzw vf31,vf12,vf8\n"

        "vmove.xyzw vf1,vf28\n"
        "vmove.xyzw vf2,vf29\n"
        "vmove.xyzw vf3,vf30\n"
        "vmove.xyzw vf4,vf31\n"

        "vsubw.x vf1,vf1,vf0\n"
        "vsubw.y vf2,vf2,vf0\n"
        "vsubw.z vf3,vf3,vf0\n"
        "vsubw.w vf4,vf4,vf0\n"

        "vsub.xyzw vf5,vf0,vf0\n"
        "vsub.xyzw vf6,vf0,vf0\n"
        "vsub.xyzw vf7,vf0,vf0\n"

        "vmulaz.y ACC,vf2,vf3\n"
        "vmsubz.y vf6,vf3,vf2\n"
        "vmulax.z ACC,vf2,vf3\n"
        "vmsubx.z vf7,vf3,vf2\n"
        "vmulay.x ACC,vf2,vf3\n"
        "vmsuby.x vf5,vf3,vf2\n"

        "vmulaz.y ACC,vf3,vf1\n"
        "vmsubz.y vf5,vf1,vf3\n"
        "vmulax.z ACC,vf3,vf1\n"
        "vmsubx.z vf6,vf1,vf3\n"
        "vmulay.x ACC,vf3,vf1\n"
        "vmsuby.x vf7,vf1,vf3\n"

        "vmulaz.y ACC,vf1,vf2\n"
        "vmsubz.y vf7,vf2,vf1\n"
        "vmulax.z ACC,vf1,vf2\n"
        "vmsubx.z vf5,vf2,vf1\n"
        "vmulay.x ACC,vf1,vf2\n"
        "vmsuby.x vf6,vf2,vf1\n"

        "vmr32.xyzw vf6,vf6\n"
        "vmr32.z $vf6z,$vf6z\n"
        "vmr32.xyzw $vf7xyzw,$vf7xyzw\n"
        "vmr32.xyzw $vf7xyzw,$vf7xyzw\n"
        "vmr32.yz $vf7yz,$vf7yz\n"
        "vabs.xyzw $vf5xyzw,$vf5xyzw\n"
        "vabs.xyzw $vf6xyzw,$vf6xyzw\n"
        "vabs.xyzw $vf7xyzw,$vf7xyzw\n"
        "vmax.xyz $vf5xyz,$vf5xyz,$vf6xyz\n"
        "vmax.xyz $vf5xyz,$vf5xyz,$vf7xyz\n"

        "qmfc2   $8,$vf5\n"
        "mtc1    $8,$f1\n"
        "prot3w  $8,$8\n"
        "mtc1    $8,$f2\n"
        "prot3w  $8,$8\n"
        "mtc1    $8,$f3\n"
        "vmove.w $vf8w,$vf0w\n"
        "vmove.w $vf9w,$vf0w\n"
    "_ei_cp12:\n"
        "c.lt.s  $f1,$f2\n"
        "bc1t    _ei_cp23\n"
    "_ei_cp13:\n"
        "c.lt.s  $f1,$f3\n"
        "bc1t    _ei_set3\n"
    "_ei_set1:\n"
        "vmove.xyz vf8,vf2\n"
        "vmove.xyz vf9,vf3\n"
        "nop\n"
        "j       _ei_cp_end\n"
        "nop\n"
    "_ei_cp23:\n"
        "mov.s   $f1,$f2\n"
        "c.lt.s  $f1,$f3\n"
        "bc1t    _ei_set3\n"
    "_ei_set2:\n"
        "vmove.xyz $vf8xyz,$vf3xyz\n"
        "vmove.xyz $vf9xyz,$vf1xyz\n"
        "nop\n"
        "j       _ei_cp_end\n"
        "nop\n"
    "_ei_set3:\n"
        "mov.s   $f1,$f3\n"
        "vmove.xyz $vf8xyz,$vf1xyz\n"
        "vmove.xyz $vf9xyz,$vf2xyz\n"
    "_ei_cp_end:\n"
        "lwc1    $f4,%3\n" // how 2 do dis
        "c.lt.s  $f1,$f4\n"
        "bc1f    _ei_calc\n"
        "vsub.xyzw $vf15xyzw,$vf0xyzw,$vf0xyzw\n"
        "nop\n"
        "j       _ei_end\n"
        "nop\n"
    "_ei_calc:\n"
        "vopmula.xyz ACC, vf8, vf9\n"
        "vopmsub.xyz $vf4, $vf9, $vf8\n"
        "vsub.w      $vf4, $vf4, $vf4\n"
        "vmul.xyz    $vf5, $vf4, $vf4\n"
        "vaddy.x     $vf5, $vf5, $vf5y\n"
        "vaddz.x     $vf5, $vf5, $vf5z\n"
        "vrsqrt      Q, $vf0w, $vf5x\n"
        "vsub.xyzw   $vf7, $vf0, $vf0\n"
        "vaddw.xyzw  $vf7, $vf7, $vf4w\n"
        "vwaitq\n"
        "vmulq.xyz   $vf15, $vf4, Q\n"
        "vmove.xyzw  $vf16, $vf8\n"
        "vmul.xyz    $vf5, $vf15, $vf15\n"
        "vaddy.x     $vf5, $vf5, $vf5y\n"
        "vaddz.x     $vf5, $vf5, $vf5z\n"
        "vrsqrt      Q, $vf0w, $vf5x\n"
        "vsub.xyzw   $vf7, $vf0, $vf0\n"
        "vaddw.xyzw  $vf7, $vf7, $vf15w\n"
        "vwaitq\n"
        "vmulq.xyz   $vf24, $vf15, Q\n"
        "vmul.xyz    $vf5, $vf16, $vf16\n"
        "vaddy.x     $vf5, $vf5, $vf5y\n"
        "vaddz.x     $vf5, $vf5, $vf5z\n"
        "vrsqrt      Q, $vf0w, $vf5x\n"
        "vsub.xyzw   $vf7, $vf0, $vf0\n"
        "vaddw.xyzw  $vf7, $vf7, $vf16w\n"
        "vwaitq\n"
        "vmulq.xyz   $vf25, $vf16, Q\n"
        "vopmula.xyz ACC, $vf24, $vf25\n"
        "vopmsub.xyz $vf6, $vf25, $vf24\n"
        "vsub.w      $vf6, $vf6, $vf6\n"
        "vmul.xyz    $vf5, $vf6, $vf6\n"
        "vaddy.x     $vf5, $vf5, $vf5y\n"
        "vaddz.x     $vf5, $vf5, $vf5\n"
        "vrsqrt      Q, $vf0w, $vf5x\n"
        "vsub.xyzw   $vf7, $vf0, $vf0\n"
        "vaddw.xyzw  $vf7, $vf7, $vf6w\n"
        "vwaitq\n"
        "vmulq.xyz   $vf26, $vf6, Q\n"
        "vsub.w      $vf24, $vf0, $vf0\n"
        "vsub.w      $vf25, $vf0, $vf0\n"
        "vsub.w      $vf26, $vf0, $vf0\n"
        "vmove.xyzw  $vf27, $vf0\n"
        "qmfc2       $8, $vf24\n"
        "qmfc2       $9, $vf25\n"
        "qmfc2       $10, $vf26\n"
        "qmfc2       $11, $vf27\n"
        "pextlw      $12, $9, $8\n"
        "pextuw      $13, $9, $8\n"
        "pextlw      $14, $11, $10\n"
        "pextuw      $15, $11, $10\n"
        "pcpyld      $8, $14, $12\n"
        "pcpyud      $9, $12, $14\n"
        "pcpyld      $10, $15, $13\n"
        "pcpyud      $11, $13, $15\n"
        "qmtc2       $8, $vf20\n"
        "qmtc2       $9, $vf21\n"
        "qmtc2       $10, $vf22\n"
        "qmtc2       $11, $vf23\n"

        "vmulax.xyzw ACC, $vf28, $vf24x\n"
        "vmadday.xyzw ACC, $vf29, $vf24y\n"
        "vmaddaz.xyzw ACC, $vf30, $vf24z\n"
        "vmaddw.xyzw $vf1, $vf31, $vf24w\n"
        "vmulax.xyzw ACC, $vf28, $vf25x\n"
        "vmadday.xyzw ACC, $vf29, $vf25y\n"
        "vmaddaz.xyzw ACC, $vf30, $vf25z\n"
        "vmaddw.xyzw $vf2, $vf31, $vf25w\n"
        "vmulax.xyzw ACC, $vf28, $vf26x\n"
        "vmadday.xyzw ACC, $vf29, $vf26y\n"
        "vmaddaz.xyzw ACC, $vf30, $vf26z\n"
        "vmaddw.xyzw $vf3, $vf31, $vf26w\n"
        "vmulax.xyzw ACC, $vf28, $vf27x\n"
        "vmadday.xyzw ACC, $vf29, $vf27y\n"
        "vmaddaz.xyzw ACC, $vf30, $vf27z\n"
        "vmaddw.xyzw $vf4, $vf31, $vf27w\n"
        "vmulax.xyzw ACC, $vf20, $vf1x\n"
        "vmadday.xyzw ACC, $vf21, $vf1y\n"
        "vmaddaz.xyzw ACC, $vf22, $vf1z\n"
        "vmaddw.xyzw $vf28, $vf23, $vf1w\n"
        "vmulax.xyzw ACC, $vf20, $vf2x\n"
        "vmadday.xyzw ACC, $vf21, $vf2y\n"
        "vmaddaz.xyzw ACC, $vf22, $vf2z\n"
        "vmaddw.xyzw $vf29, $vf23, $vf2w\n"
        "vmulax.xyzw ACC, $vf20, $vf3x\n"
        "vmadday.xyzw ACC, $vf21, $vf3y\n"
        "vmaddaz.xyzw ACC, $vf22, $vf3z\n"
        "vmaddw.xyzw $vf30, $vf23, $vf3w\n"
        "vmulax.xyzw ACC, $vf20, $vf4x\n"
        "vmadday.xyzw ACC, $vf21, $vf4y\n"
        "vmaddaz.xyzw ACC, $vf22, $vf4z\n"
        "vmaddw.xyzw $vf31, $vf23, $vf4w\n"
        "vmr32.xyzw  $vf2, $vf29\n"
        "vsub.yzw    $vf1, $vf0, $vf0\n"
        "vmr32.xyzw  $vf1, $vf2\n"
        "vsub.yzw    $vf2, $vf0, $vf0\n"
        "qmfc2       $8, $vf1\n"
        "mtc1        $8, $f1\n"
        "mfc1        $8, $f1\n"
        "beq     $0,$8,_zeroy\n"
        "qmtc2       $8, $vf8\n"
        "qmfc2       $8, $vf2\n"
        "mtc1        $8, $f1\n"
        "mfc1        $8, $f1\n"
        "nop\n"
        "beq         $0, $8, _zerox\n"
        "qmtc2       $8, $vf9\n"
        "vdiv        Q, $vf8x, $vf9x\n"
        "la          $8,tbl\n"
        "lqc2        $vf12, 0x0($8)\n"
        "vwaitq\n"
        "vaddq.xyzw  $vf8, $vf0, Q\n"
        "vabs.x      $vf8, $vf8\n"
        "vsubw.x     $vf9, $vf8, $vf0w\n"
        "vmax.x      $vf9, $vf9, $vf0\n"
        "qmfc2       $8, $vf9\n"
        "addiu       $9, $0, 0x0\n"
        "beq         $0, $8, _skip\n"
        "vaddw.x     $vf9, $vf8, $vf0w\n"
        "vsub.x      $vf10, $vf0, $vf8\n"
        "vaddw.x     $vf10, $vf10, $vf0w\n"
        "vdiv        Q, $vf10x, $vf9x\n"
        "lui         $8, 0x3F49\n"
        "ori         $8, $8, 0xFD8\n"
        "qmtc2       $8, $vf7\n"
        "addiu       $9, $0, 0x1\n"
        "vwaitq\n"
        "vaddq.x     $vf8, $vf0, Q\n"
        "nop\n"
        "b           _skip\n"
    "_skip:\n"
        "vmove.x     $vf9, $vf8\n"
        "vmove.x     $vf10, $vf8\n"
        "vmul.x      $vf8, $vf8, $vf8\n"
        "vsub.x      $vf8, $vf0, $vf8\n"
        "vmul.x      $vf10, $vf10, $vf8\n"
        "vmulx.x     $vf11, $vf10, $vf12x\n"
        "vadd.x      $vf9, $vf9, $vf11\n"
        "vmul.x      $vf10, $vf10, $vf8\n"
        "vmuly.x     $vf11, $vf10, $vf12y\n"
        "vadd.x      $vf9, $vf9, $vf11\n"
        "vmul.x      $vf10, $vf10, $vf8\n"
        "vmulz.x     $vf11, $vf10, $vf12z\n"
        "vadd.x      $vf9, $vf9, $vf11\n"
        "vmul.x      $vf10, $vf10, $vf8\n"
        "vmulw.x     $vf11, $vf10, $vf12w\n"
        "vadd.x      $vf9, $vf9, $vf11\n"
        "nop\n"
        "beq         $0, $9, _ans\n"
        "vsub.x      $vf9, $vf7, $vf9\n"
        "nop\n"
        "b           _ans\n"
    "_zerox:\n"
        "lui         $8, 0x3fc9\n"
        "ori         $8, $8, 0xfdb\n"
        "qmtc2       $8, vf9\n"
        "nop\n"
        "b           _ans\n"
    "_zeroy:\n"
        "li          $8,0\n"
        "qmtc2       $8, $vf9\n"
    "_ans:\n"
        "qmfc2       $8, $vf2\n"
        "mtc1        $8, $f1\n"
        "mfc1        $8, $f1\n"
        "bgez        $8, _next\n"
        "lui         $8, 0x4049\n"
        "ori         $8, $8, 0xfd8\n"
        "qmtc2       $8, $vf10\n"
        "vsub.x      $vf9, $vf10, $vf9\n"
    "_next:\n"
        "qmfc2       $8, $vf1\n"
        "mtc1        $8, $f1\n"
        "mfc1        $8, $f1\n"
        "bgez        $8, _next2\n"
        "vsub.x      $vf9, $vf0, $vf9\n"
    "_next2:\n"
        "vmulx.w     $vf15, $vf0, $vf9x\n"
    "_ei_end:\n"
        "sqc2    vf15,0(%0)\n"
        "nop\n"
    : : "r" (Axis), "r" (StartMtx), "r" (EndMtx), "m" (0.00000000001f));
}

f32 __sin(f32 rad) {
    __asm__ volatile(
    ".data\n"
        "STBL: .word 0x3E2AAAAB, 0x3D4CCCCD, 0x3CC30C31, 0x3C638E39\n"
    ".text\n"
        "mfc1        $6, $f12\n"
        "qmtc2       $6, $vf8\n"
        "la          $6, STBL\n"
        "lqc2        $vf11, 0x0($6)\n"
        "vmove.x     $vf9, $vf8\n"
        "vmul.x      $vf10, $vf8, $vf8\n"
        "vsub.x      $vf10, $vf0, $vf10\n"
        "vmul.x      $vf9, $vf9, $vf10\n"
        "vmulx.x     $vf9, $vf9, $vf11x\n"
        "vadd.x      $vf8, $vf8, $vf9\n"
        "vmul.x      $vf9, $vf9, $vf10\n"
        "vmuly.x     $vf9, $vf9, $vf11y\n"
        "vadd.x      $vf8, $vf8, $vf9\n"
        "vmul.x      $vf9, $vf9, $vf10\n"
        "vmulz.x     $vf9, $vf9, $vf11z\n"
        "vadd.x      $vf8, $vf8, $vf9\n"
        "vmul.x      $vf9, $vf9, $vf10\n"
        "vmulw.x     $vf9, $vf9, $vf11w\n"
        "vadd.x      $vf8, $vf8, $vf9\n"
        "qmfc2       $6, $vf8\n"
        "mtc1        $6, $f0\n"
        "nop\n"
    : : "f" (rad));
}

f32 __cos(f32 rad) {
    __asm__ volatile(
    ".data\n"
        "CTBL: .word 0x3F000000, 0x3DAAAAAB, 0x3D088889, 0x3C924925, 0x0, 0x0, 0x0, 0x0\n"
    ".text\n"
        "mfc1        $6, $f12\n"
        "qmtc2       $6, $vf10\n"
        "la          $6, CTBL\n"
        "lqc2        $vf11, 0x0($6)\n"
        "vaddw.x     $vf8, $vf0, $vf0w\n"
        "vaddw.x     $vf9, $vf0, $vf0w\n"
        "vmul.x      $vf10, $vf10, $vf10\n"
        "vsub.x      $vf10, $vf0, $vf10\n"
        "vmul.x      $vf9, $vf9, $vf10\n"
        "vmulx.x     $vf9, $vf9, $vf11x\n"
        "vadd.x      $vf8, $vf8, $vf9\n"
        "vmul.x      $vf9, $vf9, $vf10\n"
        "vmuly.x     $vf9, $vf9, $vf11y\n"
        "vadd.x      $vf8, $vf8, $vf9\n"
        "vmul.x      $vf9, $vf9, $vf10\n"
        "vmulz.x     $vf9, $vf9, $vf11z\n"
        "vadd.x      $vf8, $vf8, $vf9\n"
        "vmul.x      $vf9, $vf9, $vf10\n"
        "vmulw.x     $vf9, $vf9, $vf11w\n"
        "vadd.x      $vf8, $vf8, $vf9\n"
        "qmfc2       $6, $vf8\n"
        "mtc1        $6, $f0\n"
        "qmfc2       $2, $vf8\n"
        "nop\n"
    : : "f" (rad));
}
