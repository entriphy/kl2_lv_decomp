#ifndef TYPES_H
#define TYPES_H

typedef unsigned char u8;
typedef volatile u8 vu8;
typedef signed char s8;
typedef volatile s8 vs8;

typedef unsigned short u16;
typedef volatile u16 vu16;
typedef signed short s16;
typedef volatile s16 vs16;

typedef unsigned int u32;
typedef volatile u32 vu32;
typedef signed int s32;
typedef volatile s32 vs32;

typedef unsigned long long u64;
typedef volatile u64 vu64;
typedef signed long long s64;
typedef volatile s64 vs64;

typedef unsigned int u128 __attribute__((mode(TI)));
typedef volatile u128 vu128 __attribute__((mode(TI)));
typedef signed int s128 __attribute__((mode(TI)));
typedef volatile s128 vs128 __attribute__((mode(TI)));

typedef float f32;
typedef double f64;

typedef struct {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} FVECTOR;

typedef struct {
    FVECTOR m[4];
} FMATRIX;

typedef struct {
    s32	x;
    s32	y;
    s32	z;
    s32	w;
} IVECTOR;

typedef struct {
    s16	x;
    s16	y;
    s16	z;
    s16	w;
} SVECTOR;

typedef union { // 0x10
    u128 u_u128;
    u64  u_u64[2];
    s64  u_s64[2];
    u32  u_u32[4];
    s32  u_s32[4];
    u16  u_u16[8];
    s16  u_s16[8];
    u8   u_u8[16];
    s8   u_s8[16];
    f32  u_f32[4];
} qword_uni;

typedef union { // 0x8
    u64 u_u64;
    u32 u_u32[2];
    u16 u_u16[4];
    s64 u_s64;
    s32 u_s32[2];
    s16 u_s16[4];
    f32 u_f32[2];
} long_uni;

#endif
