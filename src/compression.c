#include "common.h"

void FUN_00198a28(DecompressionStructure *scratchpad) {
    scratchpad->finished = 0;
    scratchpad->s2 = 0;
    scratchpad->s0 = 0;
    scratchpad->s1 = 0;
    FUN_00199100(scratchpad, 0x10);
    scratchpad->s3 = 0;
}

u16 FUN_001990c0(DecompressionStructure *scratchpad, s16 param_2) {
    u16 uVar1 = scratchpad->s1 >> (0x10 - param_2) & 0xFFFF;
    FUN_00199100(scratchpad, param_2);
    return uVar1;
}

void FUN_00199100(DecompressionStructure *scratchpad, u16 param_2) {
    // something
}
