#include "h_gamehall.h"

CHALL CHall = {};
CHALL *cH = NULL;

void hCHallInit() {
    cH = &CHall;
    cH->stat = CH_IDLE;
    cH->req = 0;
    cH->idx2 = -1;
    cH->max = kzGetBGMListNumber();
    hBgmChangeID(-1, 0);
}

void hCHallQuit() {
    sD->bgmMVol = 0.708661437f;
    hBgmReset();
    hBgmChangeID(0, 0);
}

s32 hCHallRun(s32 req, s32 arg) {
    switch (req) {
        case 1:
            if (arg == -1) {
                cH->req = 2;
            } else {
                cH->req = 1;
                cH->idx = arg;
                cH->no = kzGetBGM_No(cH->idx);
                cH->ch = kzGetBGM_Ch(cH->idx);
            }
            break;
        case 2:
            if (cH->stat != CH_END) {
                cH->stat = CH_END;
                if (hBgmGetStat() == 1) {
                    if (arg < 2)
                        hBgmReset();
                    else
                        hBgmFadeOut(arg - 2);
                }
            }
            break;
    }

    switch ((s32)cH->stat) {
        case CH_IDLE:
            cH->idx2 = -1;

            switch (cH->req) {
                case 1:
                    if (hBgmGetStat() != 0) {
                        hBgmStop();
                    } else {
                        sD->bgmMVol = 0.86614174f;
                        hBgmPlay(cH->no, cH->ch);
                        hBgmSetVol(hBgmGetDefVol(cH->no));
                        cH->stat = CH_PLAY;
                        cH->req = 0;
                        cH->idx2 = cH->idx;
                        if (cH->no == BGM005)
                            cH->size = 0x39a752;
                        else
                            cH->size = hBgmGetPlaySize();
                    }
                    break;
                case 2:
                    cH->req = 0;
                    break;
                default:
                    break;
            }
            break;
        case CH_PLAY:
            switch (cH->req) {
                case 1:
                    hBgmFadeOut(15);
                    cH->stat = CH_STOP;
                    break;
                case 2:
                    hBgmFadeOut(60);
                    cH->stat = CH_STOP;
                    break;
                default:
                    if (cH->size - bD->iopPoint < 0x6270) {
                        hBgmFadeOut(60);
                        cH->stat = CH_FADEOUT;
                    }
                    break;
            }
            break;
        case CH_FADEOUT:
            switch (cH->req) {
                case 1:
                    cH->stat = CH_STOP;
                    break;
                case 2:
                    cH->stat = CH_STOP;
                    break;
                default:
                    if (hBgmGetStat() == 0) {
                        cH->stat = CH_IDLE;
                        cH->idx++;
                        if (cH->idx >= cH->max)
                            cH->idx = 0;
                        cH->req = 1;
                        cH->no = kzGetBGM_No(cH->idx);
                        cH->ch = kzGetBGM_Ch(cH->idx);
                    }
                    break;
            }
            break;
        case CH_STOP:
            if (hBgmGetStat() == 0)
                cH->stat = CH_IDLE;
            break;
    }

    return cH->idx2;
}
