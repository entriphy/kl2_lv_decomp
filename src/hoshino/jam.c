#include "common.h"

int JamGetHdSize(JAMHD *hdaddr) {
    return hdaddr->Header.headerSize;
}

int JamGetBdSize(JAMHD *hdaddr) {
    return hdaddr->Header.bodySize;
}
