#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "cam.h"
#include "compression.h"
#include "gbl.h"
#include "gim.h"
#include "hoshino.h"
#include "harada.h"
#include "kazuya.h"
#include "nakano.h"
#include "objwork.h"
#include "okanoyo.h"
#include "ps2.h"
#include "tobj.h"
#include "types.h"

#include "structs.h"
#include "data_symbols.h"
#include "function_symbols.h"

void* GetFHMAddress(void* pAddr, int nNum);

#ifdef SCE
inline int roundSizeToSector(int size);
#else
inline int roundSizeToSector(int size) {
	return size + 0x7FF & 0xFFFFF800;
}
#endif

#endif
