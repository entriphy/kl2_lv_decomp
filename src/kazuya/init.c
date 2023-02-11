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
