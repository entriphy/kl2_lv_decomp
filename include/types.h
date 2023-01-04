#ifndef TYPES_H
#define TYPES_H

typedef struct
{
	int	x;
	int	y;
	int	z;
	int	w;
}VU_IVECTOR __attribute__((aligned(16)));

typedef struct
{
	short	x;
	short	y;
	short	z;
	short	w;
}VU_SVECTOR __attribute__((aligned(16)));

#endif