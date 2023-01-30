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

typedef struct {
	float x;
	float y;
	float z;
	float w;
} FVECTOR __attribute__((aligned(16)));

typedef struct {
	int	x;
	int	y;
	int	z;
	int	w;
} IVECTOR __attribute__((aligned(16)));

typedef struct {
	int	x;
	int	y;
	int	z;
	int	w;
} IVECTOR_UA; // Unaligned

typedef struct {
	short	x;
	short	y;
	short	z;
	short	w;
} SVECTOR;

typedef struct {
	FVECTOR m[4];
} FMATRIX __attribute__((aligned(16)));

#endif
