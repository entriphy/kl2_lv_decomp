#ifndef NAKANO_H
#define NAKANO_H

#include "types.h"
#include "ps2.h"
#include "objwork.h"

// Structs

typedef struct {
    s32 map_draw_flag;
    s32 cam_debug_mode;
    s32 hitmap_flag;
    s32 meter_cnt;
    s32 meter_num;
    s32 flag;
    s32 opflag;
    s32 capture_num;
    s32 work_path[16];
} NKDEBGBL;

typedef struct {
    s32 outtime;
    s32 intime;
    s32 next_vision;
    s32 rtn;
    s32 mcn;
    s32 imuki;
    s32 omuki;
    s32 flag;
    s32 fdata;
} nkWIPE_WRK;

typedef union {
    u128 ul128;
    u64 ul64[2];
    u32 ul32[4];
    sceVu0IVECTOR vect;
    sceVu0FVECTOR fvct;
} nkQWdata;

typedef struct {
    nkQWdata *buf;
    u32 size;
} nkGifPacket;

typedef struct {
	u16 bit_buf;
	u16 bit_buf2;
	s16 bit8;
	u16 blocksize;
	u8 *in_data_adrs;
	s32 end_flag;
	s64 block_num;
	u8  p_len[19];
	u8  c_len[510];
	u16 p_table[256];
	u16 c_table[4096];
	u16 left[1019];
	u16 right[1019];
	u16 count[17];
	u16 weight[17];
	u16 start[18];
} HLZWork;

typedef struct {
    u32 cnt;
    u32 status;
    u32 lvl;
    u8 r3h;
    u8 r3v;
    u8 l3h;
    u8 l3v;
    u32 lvl2;
    u16 trg;
    u16 trgbuf;
    u16 rep;
    u16 reptimer;
    u32 looks;
} kPadDATA;

typedef struct {
    u32 irqc;
    u32 drawhc;
    u32 proc_hcnt;
    s32 cp;
    s32 nmode;
    s32 nsmode;
    s32 fmode;
    s32 smode;
    u32 modorg;
    u32 modorgend;
    OBJWORK *objwork;
    s32 n_objw;
    s32 Language;
    s32 TVSystem;
} SYSGBL;

typedef struct {
    s32 mode;
    s32 vision;
    s32 flag;
    u32 fr;
    sceGsDBuffDc db;
    s32 inter;
    s32 pause_flag;
    s32 non_pause_flag;
    s32 pause_flag2;
    s32 route_max;
    s32 init_vision;
    s32 st_rtn;
    s32 st_mcn;
    s32 st_muki;
    f32 st_height;
    s32 st_flag;
    s32 st_klomode;
    s32 Re_st_rtn;
    s32 Re_st_mcn;
    s32 Re_st_vision;
    s32 Re_st_klomode;
    s32 Re_st_muki;
    f32 Re_st_height;
    s32 Re_st_zanki_flag;
    s32 Re_st_zanki_count;
    OBJWORK *klonoa;
    OBJWORK *kazd;
    s32 playdemo_time;
    s32 playdemo_flag;
    kPadDATA kpd[2];
    nkWIPE_WRK wipe;
    s32 kloLife[2];
    s32 kloZanki[2];
    f32 reverse_cnt;
    s32 stage_flag;
    s32 filter_type;
    f32 filter_cnt;
    s32 filter_prty;
    s32 area_max;
    f32 gus_timer;
    f32 gus_cnt;
    s32 popuka_flag;
    s32 time_atack_cnt;
    s32 vs_clear_cnt;
    s32 time_atack_best;
    f32 fcnt0;
    f32 fcnt1;
    f32 fcnt2;
    s32 area_cnt;
    f32 retry_posy;
    s32 retry_bun1;
    s32 retry_muki;
    s32 tokurom_cnt;
    sceVu0FMATRIX wsm;
    sceVu0FMATRIX wvm;
    sceVu0FMATRIX vsm;
    sceVu0FMATRIX d_wsm;
    sceVu0FMATRIX d_wvm;
    sceVu0FMATRIX d_vsm;
    CAM_WORK cam;
    CAM_WORK d_cam;
} GAME_WORK;

// Defines

#define nkSPR (nkQWdata *)0x70000000

// Functions

// dma.c
extern void nkInitDma();
extern void nkDmaGifSend(void *tag);
extern void nkLoadGms(void *tag);

// game.c
extern s32 GameInit();
extern s32 GameLoad();
extern s32 GameMain();
extern void nkInitPS2();

// gssub.c
extern void nkGsInitFrm();
extern void nkGsSetNormalFZ2_0();
extern void nkGsSetNormalFZ2_1();
extern void nkGsSetNormalFZ2();

// hlzdeco.c
extern u8 * HLZDecode(u8 *src, u8 *dst, HLZWork *wk_adrs);

// init.c
extern void init_config_system();
extern void nkInitSys();

// keyfsv.c
extern void nkKeyFrameStart();

// main.c
// extern s32 main(s32 argc, char *argv[])

// menu.c
extern s32 MenuInit();
extern s32 MenuMain();

// mfunc.c
extern void MainFunc();

// nkfunc.c
extern s32 nkInit();
extern s32 nkMain();

// nkpad.cc
#ifdef __cplusplus
extern "C" {
#endif
extern void nkInitPad();
#ifdef __cplusplus
}
#endif

// objfunc.c
extern void InitObjWork(OBJWORK *objw);
extern void InitAllObjWork(OBJWORK *objwork, s32 n_objw);
extern OBJWORK * SearchOBJWORK(OBJWORK *objwork, s32 n_objw);
extern void SortObjWork(OBJWORK *objwork, s32 n_objw);
extern s32 ObjFunc(OBJWORK *objwork);

// sinit.c
extern void nkStageInit0();
extern void nkStageInit1();

// wipe.c
extern void nkWipeInit();
extern void nkWipeSetMatrix();
extern void nkWipeEffBClear();

// Data

// dma.c


// game.c
extern s32 (*GameFuncTbl[3])();
extern GAME_WORK GameGbl;
extern s16 obj_id[129];
extern OBJWORK ObjWorkBuff[128];
extern qword *nkDstAdr;

// init.c
extern sceDmaChan *DmaChVIF0;
extern sceDmaChan *DmaChVIF1;
extern sceDmaChan *DmaChGIF;
extern sceDmaChan *DmaChfromSPR;
extern sceDmaChan *DmaChtoSPR;
extern OBJWORK ObjWorkBuff[128];

// keyfsv.c


// main.c
extern NKDEBGBL nkDG;
extern SYSGBL SysGbl;
extern sceDmaChan *DmaChVIF0;
extern sceDmaChan *DmaChVIF1;
extern sceDmaChan *DmaChGIF;
extern sceDmaChan *DmaChfromIPU;
extern sceDmaChan *DmaChtoIPU;
extern sceDmaChan *DmaChfromSPR;
extern sceDmaChan *DmaChtoSPR;
extern qword PsfxWorkBuff[131072];
// extern vpmINFO VpmInfo;
extern qword *p1_packet_top;
extern qword *p1_packet;
extern qword *p1_packet_cnt;
extern qword p1_packet_buff[2][4096];
extern s32 draw_id;
extern qword p1_ot_buff[2][5];
extern qword *p1_ot;
extern s32 p1_packet_max;
extern qword vu0mem_image[256];
extern qword vu1mem_image[1024];
extern u32 *NakanoPackAdr;
extern u32 *nkGimPackAdr;
extern u32 *vtWaveAdr;
extern u32 *vtSprayAdr;
extern u32 *vtMovieAdr[8];

// menu.c
extern s32 (*MenuFuncTbl[2])();

// mfunc.c


// nkfunc.c
extern s32 (*nkFuncTbl[2])();


// objfunc.c
extern void (*ObjJTbl[10])(OBJWORK *objw);


// wipe.c

#endif
