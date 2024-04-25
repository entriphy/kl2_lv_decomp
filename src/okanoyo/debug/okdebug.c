#include "okanoyo/debug/okdebug.h"
#include "okanoyo/okio.h"

void OkDebugGame() {
    OkMemDisp();
}

void OkStrDisp() {
    // Empty function
}

void objwview() {
    s32 i;
    s32 cnt0 = 0;
    s32 cnt1 = 0;
    s32 cnt2 = 0;
    s32 cnt3 = 0;
    s32 cnt4 = 0;
    s32 cnt5 = 0;
    s32 cnt6 = 0;
    s32 cnt7 = 0;
    s32 stat[16];
    OBJWORK *objw = SysGbl.objwork;
    s32 j = 0;

    for (i = 0; i < 128; i++, objw++) {
        if (objw->stat0 == 0) {
            cnt0++;
        } else if (objw->stat0 == 1) {
            cnt1++;
        } else if (objw->stat0 == 2) {
            cnt2++;
        } else if (objw->stat0 == 3) {
            cnt3++;
        } else if (objw->stat0 == 4) {
            cnt4++;
        } else if (objw->stat0 == 5) {
            cnt5++;
        } else if (objw->stat0 == 6) {
            cnt6++;
            stat[j++] = objw->stat1;
        } else if (objw->stat0 == 7) {
            cnt7++;
        }
    }

    for (i = 0; i < j; i++) {
        // Debug stuff
    }
}

char* okvagnameN(s64 spt) {
    switch (spt) {
        case 0x4b04b00da0b101:
            return "VAG_ELCHG";
        case 0x3203a0b100:
            return "VAG_ELSPK";
        case 0x4b04b00ca0b102:
            return "VAG_ELPIKA";
        case 0x4b04b00da09801:
            return "VAG_JMCHG";
        case 0x4b04b00da09802:
            return "VAG_JMUP";
        case 0x4b04b00da00801:
            return "VAG_KAZE1";
        case 0xc80c80da09800:
            return "VAG_SACHANGE";
        case 0xc80c80ca00604:
            return "VAG_SGTURN";
        case 0x4b04b00ca00605:
            return "VAG_SGBANE01";
        case 0x4b04b00ca00606:
            return "VAG_TIMPANI";
        case 0x4b04b00ca00609:
            return "VAG_TLFIRE11";
        case  0x4b04b00ca00607:
            return "VAG_TSFIRE";
        case 0x4b04b00ca00608:
            return "VAG_TLSUIKOM";
        case 0xc80c80ca0060a:
            return "VAG_TLLOAD";
        case 0x4b04b00ca00603:
            return "VAG_FUKURAMI";
        case 0x3203200a0b400:
            return "VAG_GPFIRE11";
        case 0xc80c802a0b401:
            return "VAG_GPBOMB";
        case 0xc80c802a00501:
            return "VAG_BAKUHAS1";
        case 0xffffffff8da08600:
            return "VAG_PROPER";
        case 0xc80c80ca09700:
            return "VAG_KITINIDO";
        case 0x4b04b002a08b00:
            return "VAG_MYVOX";
        case 0x6400002a08700:
            return "VAG_MNBRK";
        case 0x6400002a08702:
            return "VAG_MNJBRK";
        case 0xffffffff80a08800:
            return "VAG_H1TRBUP";
        case 0x1401406a00601:
            return "VAG_MNREF";
        case 0xc80c823a0a205:
            return "VAG_MVWATER";
        case 0xc80c823a0a204:
            return "VAG_MVSNOW";
        case 0xc80c823a0a203:
            return "VAG_MVSAND";
        case 0x4b04b000a0b200:
            return "VAG_SPHIT01";
        case 0x1d01d06a00600:
            return "VAG_SSTIMP";
        case 0x6406402a08900:
            return "VAG_ZAP01";
        case 0x6406402a08901:
            return "VAG_ZAP03";
        case 0x4b04b000a0a801:
            return "VAG_MDCHAK";
        case 0x4b04b000a0a800:
            return "VAG_MDSWON";
        case 0xc806402a0a802:
            return "VAG_MDBOMB";
        case 0xc80c800a08f00:
            return "VAG_MXBACUM";
        case 0xc80c800a08f01:
            return "VAG_MXCLS";
        case 0xffffffff81a08f02:
            return "VAG_MXYUM";
        case 0xc80c800a08f03:
            return "VAG_MXEJC";
        case 0x12c12c22a09f00:
            return "VAG_05SABOTEN";
        case 0x3203202a08701:
            return "VAG_MPJUMP";
        case 0x4b04b00ca10006:
            return "VAG_PACHI";
        case 0x4b04b000a10007:
            return "VAG_KLWARE11";
        case 0x4b04b001a00806:
            return "VAG_NAGE";
        case 0xc80c80ca00a00:
            return "VAG_KLBON";
        case 0x4b04b01ca00301:
            return "VAG_SCSMALL";
        case 0x4b04b01ca00300:
            return "VAG_SCBIG";
        case 0x4b04b01ca00302:
            return "VAG_S3SMALL";
        case 0x4b04b01ca00303:
            return "VAG_S3KAGAMI";
        case 0x4b04b01ca00304:
            return "VAG_S4BIG";
        case 0x4b04b01ca00305:
            return "VAG_S4KAGAMI";
        case 0x4b04b00ca10005:
            return "VAG_KLTSUKAMI";
        case 0xffffffff84a09602:
            return "VAG_04TOBIRA3";
        case 0xffffffff8ca09604:
            return "VAG_04ENTOTU";
        case 0xc80c800a0ce02:
            return "VAG_15TOWER";
        case 0xc80c809a0ce01:
            return "VAG_15JET";
        case 0xc80c801a0b702:
            return "VAG_08LASER";
        case 0xffffffffffffffff:
            return "ERRROR";
        default:
            return "NODATA";
    }
}
