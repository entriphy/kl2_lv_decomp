#ifndef DEFINES_H
#define DEFINES_H

#define SCR_WIDTH  640
#define SCR_HEIGHT 224

#define M_Ef		2.7182818284590452354f
#define M_LOG2Ef		1.4426950408889634074f
#define M_LOG10Ef	0.43429448190325182765f
#define M_LN2f		0.69314718055994530942f
#define M_LN10f		2.30258509299404568402f
#define M_PIf		3.14159265358979323846f
#define M_TWOPIf         (M_PIf * 2.0f)
#define M_PI_2f		1.57079632679489661923f
#define M_PI_4f		0.78539816339744830962f
#define M_3PI_4f		2.3561944901923448370E0f
#define M_SQRTPIf        1.77245385090551602792981f
#define M_1_PIf		0.31830988618379067154f
#define M_2_PIf		0.63661977236758134308f
#define M_2_SQRTPIf	1.12837916709551257390f
#define M_SQRT2f		1.41421356237309504880f
#define M_SQRT1_2f	0.70710678118654752440f
#define M_LN2LOf         1.9082149292705877000E-10f
#define M_LN2HIf         6.9314718036912381649E-1f
#define M_SQRT3f   	1.73205080756887719000f
#define M_IVLN10f        0.43429448190325182765f /* 1 / log(10) */
#define M_LOG2_Ef        0.693147180559945309417f
#define M_INVLN2f        1.4426950408889633870E0f  /* 1 / log(2) */

#define PAD_TRG_UP(kpd) (kpd.trg & 0x1000)
#define PAD_TRG_DOWN(kpd) (kpd.trg & 0x4000)
#define PAD_TRG_LEFT(kpd) (kpd.trg & 0x8000)
#define PAD_TRG_RIGHT(kpd) (kpd.trg & 0x200)
#define PAD_TRG_TRIANG(kpd) (kpd.trg & 0x10)
#define PAD_TRG_CROSS(kpd) (kpd.trg & 0x40)
#define PAD_TRG_SQUARE(kpd) (kpd.trg & 0x80)
#define PAD_TRG_CIRCLE(kpd) (kpd.trg & 0x20)
#define PAD_TRG_L1(kpd) (kpd.trg & 0x4)
#define PAD_TRG_L2(kpd) (kpd.trg & 0x1)
#define PAD_TRG_R1(kpd) (kpd.trg & 0x8)
#define PAD_TRG_R2(kpd) (kpd.trg & 0x2)
#define PAD_TRG_START(kpd) (kpd.trg & 0x800)
#define PAD_TRG_SELECT(kpd) (kpd.trg & 0x100)

#define PAD_REP_UP(kpd) (kpd.rep & 0x1000)
#define PAD_REP_DOWN(kpd) (kpd.rep & 0x4000)
#define PAD_REP_LEFT(kpd) (kpd.rep & 0x8000)
#define PAD_REP_RIGHT(kpd) (kpd.rep & 0x2000)

#define PAD_LVL_UP(kpd) (kpd.lvl & 0x1000)
#define PAD_LVL_DOWN(kpd) (kpd.lvl & 0x4000)
#define PAD_LVL_LEFT(kpd) (kpd.lvl & 0x8000)
#define PAD_LVL_RIGHT(kpd) (kpd.lvl & 0x2000)
#define PAD_LVL_TRIANG(kpd) (kpd.lvl & 0x10)
#define PAD_LVL_CROSS(kpd) (kpd.lvl & 0x40)
#define PAD_LVL_SQUARE(kpd) (kpd.lvl & 0x80)
#define PAD_LVL_CIRCLE(kpd) (kpd.lvl & 0x20)
#define PAD_LVL_L1(kpd) (kpd.lvl & 0x4)
#define PAD_LVL_L2(kpd) (kpd.lvl & 0x1)
#define PAD_LVL_R1(kpd) (kpd.lvl & 0x8)
#define PAD_LVL_R2(kpd) (kpd.lvl & 0x2)

#define pPAD_TRG_UP(kpd) (kpd->trg & 0x1000)
#define pPAD_TRG_DOWN(kpd) (kpd->trg & 0x4000)
#define pPAD_TRG_LEFT(kpd) (kpd->trg & 0x8000)
#define pPAD_TRG_RIGHT(kpd) (kpd->trg & 0x200)
#define pPAD_TRG_TRIANG(kpd) (kpd->trg & 0x10)
#define pPAD_TRG_CROSS(kpd) (kpd->trg & 0x40)
#define pPAD_TRG_SQUARE(kpd) (kpd->trg & 0x80)
#define pPAD_TRG_CIRCLE(kpd) (kpd->trg & 0x20)
#define pPAD_TRG_L1(kpd) (kpd->trg & 0x4)
#define pPAD_TRG_L2(kpd) (kpd->trg & 0x1)
#define pPAD_TRG_R1(kpd) (kpd->trg & 0x8)
#define pPAD_TRG_R2(kpd) (kpd->trg & 0x2)
#define pPAD_TRG_START(kpd) (kpd->trg & 0x800)
#define pPAD_TRG_SELECT(kpd) (kpd->trg & 0x100)

#define pPAD_REP_UP(kpd) (kpd->rep & 0x1000)
#define pPAD_REP_DOWN(kpd) (kpd->rep & 0x4000)
#define pPAD_REP_LEFT(kpd) (kpd->rep & 0x8000)
#define pPAD_REP_RIGHT(kpd) (kpd->rep & 0x2000)

#define pPAD_LVL_UP(kpd) (kpd->lvl & 0x1000)
#define pPAD_LVL_DOWN(kpd) (kpd->lvl & 0x4000)
#define pPAD_LVL_LEFT(kpd) (kpd->lvl & 0x8000)
#define pPAD_LVL_RIGHT(kpd) (kpd->lvl & 0x2000)
#define pPAD_LVL_TRIANG(kpd) (kpd->lvl & 0x10)
#define pPAD_LVL_CROSS(kpd) (kpd->lvl & 0x40)
#define pPAD_LVL_SQUARE(kpd) (kpd->lvl & 0x80)
#define pPAD_LVL_CIRCLE(kpd) (kpd->lvl & 0x20)
#define pPAD_LVL_L1(kpd) (kpd->lvl & 0x4)
#define pPAD_LVL_L2(kpd) (kpd->lvl & 0x1)
#define pPAD_LVL_R1(kpd) (kpd->lvl & 0x8)
#define pPAD_LVL_R2(kpd) (kpd->lvl & 0x2)

#endif
