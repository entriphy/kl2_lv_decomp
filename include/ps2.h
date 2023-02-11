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
#include <libvu0.h>
#include <sif.h>
#include <sifcmd.h>
#include <sifdev.h>
#include <sifrpc.h>

typedef sceSifClientData SifClientData;
typedef sceSifDmaData SifDmaData;
#define sce_print scePrintf
#define DevVif0Reset sceDevVif0Reset
#define DevVif1Reset sceDevVif1Reset
#define DevVu0Reset sceDevVu0Reset
#define DevVu1Reset sceDevVu1Reset
#define dma_reset sceDmaReset
#define GsResetGraph sceGsResetGraph
#define GsResetPath sceGsResetPath
#define GsVSync sceGsSyncV
#define SifInitRpc sceSifInitRpc
#define SifIopReboot sceSifRebootIop
#define SifIopSync sceSifSyncIop
#define SifInitIopHeap sceSifInitIopHeap
#define SifLoadFileInit sceSifLoadFileReset
#define SifLoadModule sceSifLoadModule
#define SifCheckStatRpc sceSifCheckStatRpc
#define SifBindRpc sceSifBindRpc
#define SifCallRpc sceSifCallRpc
#define SifSetDma sceSifSetDma
#define SifDmaStat sceSifDmaStat
#define SCECdMmodeDvd SCECdDVD
#define SIF_RPC_M_NOWAIT SIF_RPCM_NOWAIT
#define GS_INIT_RESET 0
#define GS_INTERLACED SCE_GS_INTERLACE
#define GS_MODE_NTSC SCE_GS_NTSC
#define GS_FFMD_FRAME SCE_GS_FRAME
#define R_EE_T0_MODE T0_MODE

#else
#include <dma.h>
#include <ee_regs.h>
#include <iopcontrol.h>
#include <iopheap.h>
#include <kernel.h>
#include <libcdvd.h>
#include <loadfile.h>
#include <sifcmd.h>
#include <sifdma.h>
#include <sifrpc.h>
#include <vif_registers.h>

typedef SifRpcClientData_t SifClientData;
typedef SifDmaTransfer_t SifDmaData;
void sce_print(const char* fmt, ...);
void DevVif0Reset();
void DevVif1Reset();
void DevVu0Reset();
void DevVu1Reset();

#endif

#endif
