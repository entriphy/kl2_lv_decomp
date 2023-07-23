// Unknown filename
#include "hoshino/h_movie.h"

hMOVDATA MovData = {};
hMOVDATA *mD = NULL;

extern "C" {

void hMovInit() {
    mD = &MovData;
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&mD->file, "\\SR.SMV;1"));
}

s32 hMov_002da2b0() {
    // TODO
}

}