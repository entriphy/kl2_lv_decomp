#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "cam.h"
#include "gbl.h"
#include "gim.h"
#include "hoshino.h"
#include "harada.h"
#include "kazuya.h"
#include "nakano.h"
#include "objwork.h"
#include "okanoyo.h"
#include "ps2.h"
#include "psfx.h"
#include "tobj.h"
#include "types.h"

#include "structs.h"
#include "data_symbols.h"
#include "function_symbols.h"

u8 *GetFHMAddress(u8 *pAddr, int nNum);

#endif
