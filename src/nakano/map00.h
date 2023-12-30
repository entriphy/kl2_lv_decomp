#ifndef MAP00_H
#define MAP00_H

#include "nakano.h"

extern void SetVu1MemImage();
extern void nkSetObjAttrUnpack(qword tag, s32 vu_addr, s32 qwc);
extern void nkSetObjAttrMscal(qword tag, s32 vu_addr);
extern void nkSetObjAttrDBlbuf(qword tag, u32 base, u32 offset);
extern void nkInitAtrMain();
extern void nkDrawMap();

#endif
