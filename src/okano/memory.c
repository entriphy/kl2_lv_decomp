#include "common.h"

DEBMEM OkDebMem[10];
u8 *texdeb;
u8 *buffstartptrORG;
u8 *buffstartptr;
u8 *buffstagetop;


void memorySysFormat() {
    FUN_001d3710();
    buffstartptrORG = (u8 *)0x00A00000;
    buffstartptr = (u8 *)0x00A00000;
    texdeb = NULL;
    FUN_001d31a0();
}

void memoryStageFormat() {
    buffstartptr = buffstagetop;
    texdeb = NULL;
}

void FUN_001d3638(int i) {
    u32 tops[] = {
        0x0000000, 0x2000000, 0x2100000, 0x2300000,
        0x2400000, 0x2800000, 0x2B00000, 0x2F00000,
        0x5700000
    };

    u32 ends[] = {
        0x0000000, 0x20FFFFF, 0x22FFFFF, 0x23FFFFF,
        0x27FFFFF, 0x2AFFFFF, 0x2EFFFFF, 0x56FFFFF,
        0x57FFFFF
    };

    OkDebMem[i].top = tops[i];
    OkDebMem[i].end = ends[i];
}

void FUN_001d3710() {
    int i;
    for (i = 0; i < 9; i++) {
        FUN_001d3638(i);
    }
}
