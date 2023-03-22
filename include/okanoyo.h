#ifndef OKANOYO_H
#define OKANOYO_H

#include "types.h"

typedef struct {
    u32 top;
    u32 end;
    u64 pad;
} DEBMEM;

typedef struct {
    s32 stat;
    u16 hstart;
    u16 hend;
    s64 col;
} ZOSAN_DATA;

typedef struct {
    int scr_x;
    int scr_y;
    int fsz_x;
    int fsz_y;
    ZOSAN_DATA zd[16];
    int z_id;
    int pad0;
    int pad1;
    int pad2;
} PERSONAL_DEB;

typedef struct {
    int tetton2_upspd;
    int tetton2_upmax;
    int tetton2_dwspd;
    int tetton2_dwmax;
    int tetton2_mcspd;
    int tetton2_mcmax;
    int tetton2_count;
    int tetton2_ypmax;
    int gomgumi_cntmax;
    int gomgumi_ydown;
    int gomgumi_mdown;
    int gomgumi_nobih;
    int trmprn_upspd0;
    int trmprn_upspd1;
    int jetimo_upspd;
    int jetimo_acctim;
    int jetimo_livtim;
    int jetimo_dietim;
    int klonoa_dwmax;
    int rocketmuu_pspd;
} ZAK_DEFAULT;

typedef struct {
    int hit_csr_pic;
    int hit_csr_id;
    int hit_csr_on;
    float hit_csr_rot;
    float csr_pos[4];
    int csr_rno;
    int csr_mcn;
    int csr_mode;
    int csr_trg;
    float csr_yofs;
    int csr_dis;
    int csr_xzdis;
    int csr_ydis;
    int csr_keyhis;
    int csr_dbkey;
    int csr_wtkey;
    int csr_dbcnt;
    u8 *ITEMDataBuffp;
    int itemdataflag;
    int OkDebDPcnt;
    int OkDebHTcnt;
    int OkDebKLcnt;
    int OkDebEXcnt;
    int OkDebDPTrg;
    int OkDebHTTrg;
    int OkDebKLTrg;
    int OkDebEXTrg;
    int OkDebM_Elf;
    int pad0;
    ZAK_DEFAULT zak_com;
    int okzcom_mode;
    int okzcom_team;
    int okzcom_csr;
    int okzcom_set;
    float okzcom_valf;
    int okzcom_vali;
    int OkDebugGameMode;
    int texinfocnt;
    int OkDebugCamCurMode;
    int alive_mode;
    int pad1;
    int pad2;
} OKDEBDATAEX;

typedef struct {
    OKDEBDATAEX* debp;
    int pad0;
    int pad1;
    int pad2;
} OKDEBDATA;

typedef struct {
    int fontID;
    int vsonID;
    int modeID;
    int kloDmg;
    int plnMod;
} OKPROFILE;

typedef struct {
    int fontID;
    int menuID;
    int modeID;
    int plan;
    int muteki;
    int pad1;
    long pad0;
} PROFILE;

#endif
