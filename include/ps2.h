#ifndef PS2_H
#define PS2_H

#include "types.h"

#define SCR_WIDTH  640
#define SCR_HEIGHT 224

#ifdef SCE
#include <eekernel.h>
#include <eestruct.h>
#include <devgif.h>
#include <devvif0.h>
#include <devvif1.h>
#include <devvu0.h>
#include <devvu1.h>
#include <libdma.h>
#include <eeregs.h>
#include <libcdvd.h>
#include <libgraph.h>
#include <libpad.h>
#include <libsdr.h>
#include <libvu0.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifdev.h>
#include <sifrpc.h>
#endif // SCE

#endif // PS2_H
