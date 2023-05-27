// Unknown filename
#include "common.h"

hMOVDATA MovData = {};
hMOVDATA* mD = NULL;

void hMovInit() {
    mD = &MovData;
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&mD->file, "\\SR.SMV;1"));
}
