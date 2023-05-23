#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "common.h"

hGAMEDATA GameData;
hAC3DATA Ac3Data;
hMOVDATA MovData;
hGAMEDATA *gD;
KLTABLE* KlTable;
PPTTABLE* PptTable;
BGMTABLE* BgmTable;
hAC3DATA* aD;
u8 pptEeAddrs[4][0x40000] __attribute__((aligned(16)));
u8 *GameDataTop;
u8 *areaBuff;
hMOVDATA* mD;

void hMovInit() {
    mD = &MovData;
    sceCdDiskReady(0);
    while (!sceCdSearchFile(&mD->file, "\\SR.SMV;1"));
}

