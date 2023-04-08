#include "common.h"

int klworks_flag;
DL_STATUS dl_status;
int ret_gameread;
int DAT_003fadcc;

void kzLoadCommonGms() {
    u8 *gms = FUN_00167c80(7);
    if (gms != NULL) {
        FUN_0018dcb0(gms + ((u32 *)gms)[1]);
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

int kzDrawNowload() {
    // TODO
    int iVar3;
    
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
