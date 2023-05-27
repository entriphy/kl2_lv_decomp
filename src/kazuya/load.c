#include "common.h"

s32 klworks_flag;
DL_STATUS dl_status;
s32 ret_gameread;
s32 DAT_003fadcc;

void kzLoadCommonGms() {
    u8 *gms = (u8 *)hGetSysDataAddr(7);
    if (gms != NULL) {
        nkLoadGms(gms + ((u32 *)gms)[1]);
        sceGsSyncPath(0, 0);
    }
}

void kzInitNowload() {
    klworks_flag = GameGbl.vision == 0x0105;
    dl_status = DL_STATUS_NML_LOAD;
    ret_gameread = 0;
    if (GameGbl.vision == 0x0105) {
        DAT_003fadcc = 0;
    }
}

s32 kzDrawNowload() {
    // TODO
    s32 iVar3;
    
    if (ret_gameread == 0) {
        ret_gameread = hGameReadOK();
    }

    switch (dl_status) {
        case DL_STATUS_NML_LOAD:
        case DL_STATUS_CMN_LOAD:
            if (ret_gameread != 0) {
                if (dl_status == DL_STATUS_NML_LOAD) {
                    ret_gameread = 0;
                }
                dl_status++;
            }
            break;
        case DL_STATUS_NML_TRANS:
        case DL_STATUS_NML_END:
            iVar3 = 0;
            dl_status++;
            break;
        default:
            iVar3 = 1;
            break;
    }

    if (klworks_flag != 0) {
        
    }
    
    return ret_gameread;
}
