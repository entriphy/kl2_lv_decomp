#include "common.h"

int klworks_flag;
DL_STATUS dl_status;
int ret_gameread;
int DAT_003fadcc;

void kzInitNowload() {
    klworks_flag = GameGbl.vision == 0x0105;
    dl_status = DL_STATUS_NML_LOAD;
    ret_gameread = 0;
    if (GameGbl.vision == 0x0105) {
        DAT_003fadcc = 0;
    }
}

void kzDrawNowload() {
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
}
