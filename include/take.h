#ifndef TAKE_H
#define TAKE_H

#include "common.h"

#pragma region Defines

#define DECADDR0 ((sceVu0FMATRIX *)0x70000000)
#define DECADDR1 ((sceVu0FMATRIX *)0x70001000)
#define DECADDR2 ((sceVu0FMATRIX *)0x70002000)
#define DECADDR3 ((sceVu0FMATRIX *)0x70003000)
#define ALIGN(x) (u32 *)(((u32)x + 0xF) & ~0xF)
#define ALIGN_ALT(x, y) (u32 *)(((u32)x + y + 0xF) & ~0xF)
#define SETRGBA(x, r, g, b, a) { x.u_u32[0] = r; x.u_u32[1] = g; x.u_u32[2] = b; x.u_u32[3] = a; }
#define SETUV(x, u, v) { x.u_u32[0] = u; x.u_u32[1] = v; x.u_u32[2] = 0; x.u_u32[3] = 0; }
#define SETXYZ(a, x, y, z) { a.u_u32[0] = x; a.u_u32[1] = y; a.u_u32[2] = z; a.u_u32[3] = 0; }
#define SETREG(x, y, z) { *((u64 *)&x) = y; *((u64 *)&x + 1) = z; }
#define QWORD_SET128(x, y) { *(u128 *)&x = y; }
#define QWORD_SET64(x, y, i) { *((u64 *)&x + i) = y; }
#define QWORD_SET32(x, y, i) { *((u32 *)&x + i) = y; }

#pragma endregion Defines

#endif
