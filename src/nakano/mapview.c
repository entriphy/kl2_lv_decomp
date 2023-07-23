#include "mapview.h"
#include "main.h"
#include "dma.h"
#include "harada/hr_main.h"
#include "harada/hr_bgwk.h"
#include "nkpad.h"

static s32 MapVMenu;
static s32 MapVMsw;
static char MapName[256];

void nkMapFileRead() {
	vpmINFO *vpmi = &VpmInfo;
	char FileName[256];
	char VpmName[256];
	char GmsName[256];
	char *hex[16] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
    
    strcpy(FileName, "host0:./dat/st");
    strcat(FileName, hex[(GameGbl.vision / 0x0A00) % 10]);
    strcat(FileName, hex[(GameGbl.vision / 0x0100) % 10]);
    strcat(FileName, "/map");
    strcat(FileName, hex[(GameGbl.vision / 0x0A00) % 10]);
    strcat(FileName, hex[(GameGbl.vision / 0x0100) % 10]);
    strcat(FileName, hex[GameGbl.vision >> 4 & 0xF]);
    strcat(FileName, hex[GameGbl.vision & 0xF]);
    
    strcpy(MapName, "map");
    strcat(MapName, hex[(GameGbl.vision / 0x0A00) % 10]);
    strcat(MapName, hex[(GameGbl.vision / 0x0100) % 10]);
    strcat(MapName, hex[GameGbl.vision >> 4 & 0xF]);
    strcat(MapName, hex[GameGbl.vision & 0xF]);

    strcpy(GmsName, FileName);
    strncat(GmsName, ".gms", 4);
    read_file(GmsName, vpmi->data_buff);
    nkLoadGms(vpmi->data_buff);
    sceGsSyncPath(0, 0);

    strcpy(VpmName, FileName);
    strncat(VpmName, ".vpm", 4);
    read_file(VpmName, vpmi->data_buff);
    DecodeVpm(vpmi->data_buff);
    vpmi->vpm_data_top = (u32 *)vpmi->data_buff;
}

void MapConfFileWrite() {
	s32 wfd;
	char FileName[256];
	char *hex[16] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "a", "b", "c", "d", "e", "f" };
    
    strcpy(FileName, "host0:./dat/st");
    strcat(FileName, hex[(GameGbl.vision / 0x0A00) % 10]);
    strcat(FileName, hex[(GameGbl.vision / 0x0100) % 10]);
    strcat(FileName, "/map");
    strcat(FileName, hex[(GameGbl.vision / 0x0A00) % 10]);
    strcat(FileName, hex[(GameGbl.vision / 0x0100) % 10]);
    strcat(FileName, hex[GameGbl.vision >> 4 & 0xF]);
    strcat(FileName, hex[GameGbl.vision & 0xF]);
    strncat(FileName, ".mvc", 4);
    if ((wfd = sceOpen(FileName, SCE_WRONLY | SCE_CREAT | SCE_TRUNC)) > -1) {
        sceWrite(wfd, &mapvw, sizeof(MAPVWORK));
        sceClose(wfd);
    }
}

static s32 MapvInit() {
    vpmINFO *vpmi;
	u32 i;

    vpmi = &VpmInfo;

    _nkGetMapBuf();
    sceGsResetPath();
    sceDevGifReset();
    sceDevVif1Reset();
    sceDevVu1Reset();
    sceDevVif0Reset();
    sceDevVu0Reset();
    sceGsSetDefDBuffDc(&GameGbl.db, SCE_GS_PSMCT32, SCR_WIDTH, SCR_HEIGHT, SCE_GS_ZGREATER, SCE_GS_PSMZ24, SCE_GS_CLEAR);
    FlushCache(0);
    nkInitDma();
    nkMapFileRead();
    OkSetIDprofile();
    OkPrintSysInit(1, 0x280, 0xe0, 10, 6);
    CamInit(&GameGbl.cam);
    draw_id = 0;
    vpmi->block_list[0] = 0;
    for (i = 0; i < 2048; i++) {
        vpmi->block_list[i + 1] = 0;
    }
    nkPathReset();
    nkInitAtrMain();
    sceGsSyncPath(0,0);
    nkDebFlush();
    nkPathFlush();
    MapVMenu = 0;
    MapVMsw = 0;
    MapConfFileRead();
    GameGbl.fr = 0;
    SysGbl.smode = SysGbl.smode + 1;
    return 0;
}

static void CamMonitor(CAM_WORK *cam) {
    sceVu0FVECTOR ang;

    ang[0] = cam->ang[0] * 360.0f / M_TWOPIf;
    ang[1] = cam->ang[1] * 360.0f / M_TWOPIf;
    ang[2] = cam->ang[2] * 360.0f / M_TWOPIf;
}

void MapVDrawBG() {
	static qword_uni packetb[10];
	static sceDmaTag tag[3];
	qword_uni *pb;
	sceDmaTag *tp;
	s32 qwc;

    pb = packetb;
    tp = tag;

    if (hrcntbg < 1) {
        pb->u_u64[0] = SCE_GIF_SET_TAG(4, 1, 1, SCE_GS_SET_PRIM(SCE_GS_PRIM_TRISTRIP, 1, 0, 0, 0, 0, 0, 1, 0), SCE_GIF_PACKED, 2);
        pb->u_u64[1] = SCE_GS_RGBAQ | SCE_GS_XYZ2 << 0x4;
        pb++;

        pb->u_u32[0] = mapvw.bgt.r;
        pb->u_u32[1] = mapvw.bgt.g;
        pb->u_u32[2] = mapvw.bgt.b;
        pb->u_u32[3] = 0;
        pb++;

        pb->u_u32[0] = 0x6C00;
        pb->u_u32[1] = 0x7900;
        pb->u_u32[2] = 1;
        pb->u_u32[3] = 0;
        pb++;

        pb->u_u32[0] = mapvw.bgt.r;
        pb->u_u32[1] = mapvw.bgt.g;
        pb->u_u32[2] = mapvw.bgt.b;
        pb->u_u32[3] = 0;
        pb++;

        pb->u_u32[0] = 0x9400;
        pb->u_u32[1] = 0x7900;
        pb->u_u32[2] = 1;
        pb->u_u32[3] = 0;
        pb++;

        pb->u_u32[0] = mapvw.bgu.r;
        pb->u_u32[1] = mapvw.bgu.g;
        pb->u_u32[2] = mapvw.bgu.b;
        pb->u_u32[3] = 0;
        pb++;

        pb->u_u32[0] = 0x6C00;
        pb->u_u32[1] = 0x8700;
        pb->u_u32[2] = 1;
        pb->u_u32[3] = 0;
        pb++;

        pb->u_u32[0] = mapvw.bgu.r;
        pb->u_u32[1] = mapvw.bgu.g;
        pb->u_u32[2] = mapvw.bgu.b;
        pb->u_u32[3] = 0;
        pb++;

        pb->u_u32[0] = 0x9400;
        pb->u_u32[1] = 0x8700;
        pb->u_u32[2] = 1;
        pb->u_u32[3] = 0;
        pb++;
        
        sceDmaAddRef(&tp, 9, packetb);
        sceDmaAddEnd(&tp, 0, NULL);
        FlushCache(0);
        sceDmaSend(DmaChGIF, tag);
        sceDmaSync(DmaChGIF, 0, 0);
    }
}

static void MapVConf(CAM_WORK *cam) {
    kPadDATA *kpd;
	f32 flr;
	s32 ilr;

    kpd = &GameGbl.kpd[0];
    flr = 0.0f;
    ilr = 0;
    
    if (kpd->rep & 0x1000)
        MapVMenu--;
    if (kpd->rep & 0x4000)
        MapVMenu++;
    if (MapVMenu < 0) 
        MapVMenu = 12;
    if (MapVMenu > 12)
        MapVMenu = 0;

    if (kpd->lvl & 0x2000) {
        flr = 1.0f;
        ilr = 1;
    }
    if (kpd->lvl & 0x8000) {
        flr = -1.0f;
        ilr = -1;
    }

    switch (MapVMenu) {
        case 0:
            if (cam->scr_z > 10.0f)
                cam->scr_z += flr * 10.0f;
            break;
        case 1:
            if (ilr > 0 && mapvw.bgt.r < 0xFF)
                mapvw.bgt.r += ilr;
            if (ilr < 0 && mapvw.bgt.r > 0)
                mapvw.bgt.r += ilr;
            break;
        case 2:
            if (ilr > 0 && mapvw.bgt.g < 0xFF)
                mapvw.bgt.g += ilr;
            if (ilr < 0 && mapvw.bgt.g > 0)
                mapvw.bgt.g += ilr;
            break;
        case 3:
            if (ilr > 0 && mapvw.bgt.b < 0xFF)
                mapvw.bgt.b += ilr;
            if (ilr < 0 && mapvw.bgt.b > 0)
                mapvw.bgt.b += ilr;
            break;
        case 4:
            if (ilr > 0 && mapvw.bgu.r < 0xFF)
                mapvw.bgu.r += ilr;
            if (ilr < 0 && mapvw.bgu.r > 0)
                mapvw.bgu.r += ilr;
            break;
        case 5:
            if (ilr > 0 && mapvw.bgu.g < 0xFF)
                mapvw.bgu.g += ilr;
            if (ilr < 0 && mapvw.bgu.g > 0)
                mapvw.bgu.g += ilr;
            break;
        case 6:
            if (ilr > 0 && mapvw.bgu.b < 0xFF)
                mapvw.bgu.b += ilr;
            if (ilr < 0 && mapvw.bgu.b > 0)
                mapvw.bgu.b += ilr;
            break;
        case 7:
            if (ilr > 0 && mapvw.fog_col[0] < 0xFF)
                mapvw.fog_col[0] += ilr;
            if (ilr < 0 && mapvw.fog_col[0] > 0)
                mapvw.fog_col[0] += ilr;
            break;
        case 8:
            if (ilr > 0 && mapvw.fog_col[1] < 0xFF)
                mapvw.fog_col[1] += ilr;
            if (ilr < 0 && mapvw.fog_col[1] > 0)
                mapvw.fog_col[1] += ilr;
            break;
        case 9:
            if (ilr > 0 && mapvw.fog_col[2] < 0xFF)
                mapvw.fog_col[2] += ilr;
            if (ilr < 0 && mapvw.fog_col[2] > 0)
                mapvw.fog_col[2] += ilr;
            break;
        case 10:
            if (mapvw.fog_near > 0)
                mapvw.fog_near += ilr * 10;
            break;
        case 11:
            if (mapvw.fog_far > 0)
                mapvw.fog_far += ilr * 10;
            break;
        case 12:
            if (kpd->trg & 0x20)
                MapConfFileWrite();
            break;
    }
}

static s32 MapvMain() {
	s32 work;
	s32 inter;
	kPadDATA *kpd0;
	kPadDATA *kpd1;

    kpd0 = &GameGbl.kpd[0];
    nkGetPad();
    kpd0->lvl = nkGetPlvl(0);
    kpd0->trg = nkGetPtrg(0);
    kpd0->rep = nkGetPrep(0);
    kpd1 = &GameGbl.kpd[1];
    kpd1->lvl = nkGetPlvl(1);
    kpd1->trg = nkGetPtrg(1);
    kpd1->rep = nkGetPrep(1);

    nkPathReset();
    FlushCache(0);
    CamMonitor(&GameGbl.cam);
    if (MapVMsw != 0) {
        MapVConf(&GameGbl.cam);
    }
    if (PAD_TRG_START(GameGbl.kpd[0])) {
        MapVMsw = ~MapVMsw;
    }
    MapVDrawBG();
    CamDebug(&GameGbl.cam);
    CamCalMatrix(&GameGbl.cam);
    nkDrawMap();
    sceGsSyncPath(0, 0);
    work = *T0_COUNT;
    nkSetMeter();
    nkDrawWorkMeter();
    OkPFontFlush(PAD_TRG_SELECT(GameGbl.kpd[1]));
    inter = sceGsSyncV(0) ^ 1;
    if ((GameGbl.fr & 1) != 0) {
        sceGsSetHalfOffset(&GameGbl.db.draw11, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw12, 0x800, 0x800, inter);
    } else {
        sceGsSetHalfOffset(&GameGbl.db.draw01, 0x800, 0x800, inter);
        sceGsSetHalfOffset2(&GameGbl.db.draw02, 0x800, 0x800, inter);
    }
    sceGsSwapDBuffDc(&GameGbl.db, GameGbl.fr);
    GameGbl.fr++;
    nkResetMeter();
    FlushCache(0);
    nkPathFlush();
    if (PAD_TRG_SELECT(GameGbl.kpd[0])) {
        SysGbl.fmode = 0;
        SysGbl.smode = 0;
    }
    return 0;
}

s32 (*MapVFuncTbl[2])() = {
    MapvInit,
    MapvMain
};
