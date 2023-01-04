#ifndef TYPES_H
#define TYPES_H

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
	short	x;
	short	y;
	short	z;
	short	w;
} SVECTOR __attribute__((aligned(16)));

typedef struct {
	FVECTOR m[4];
} FMATRIX __attribute__((aligned(16)));

#endif