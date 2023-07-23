#ifndef NK_MAIN_H
#define NK_MAIN_H

#include "nakano.h"
#include "harada.h"

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

extern NKDEBGBL nkDG;
extern sceDmaChan *DmaChVIF0;
extern sceDmaChan *DmaChVIF1;
extern sceDmaChan *DmaChGIF;
extern sceDmaChan *DmaChfromIPU;
extern sceDmaChan *DmaChtoIPU;
extern sceDmaChan *DmaChfromSPR;
extern sceDmaChan *DmaChtoSPR;
extern qword PsfxWorkBuff[131072];
extern vpmINFO VpmInfo;
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

#endif
