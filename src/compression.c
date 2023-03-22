#include "common.h"

u8 *FUN_00198950(u8 *input, u8 *output, DecompressionStructure *scratchpad) {
    scratchpad->stream_pos = input;
    scratchpad->cycles = *scratchpad->stream_pos++;
    FUN_00198a28(scratchpad);
    
    while (1) {
        u16 uVar1 = FUN_00198a68(scratchpad);
        if (scratchpad->finished != 0) {
            return scratchpad->finished == 1 ? output : 0;
        }
        
        if (uVar1 < 0x100) {
            *output++ = uVar1;
        } else {
            u16 uVar2 = FUN_00198b80(scratchpad);
            u8 *uVar3 = output - (uVar2 + 1);
            s32 i;
            for (i = 0xFD; i < uVar1; i++) {
                *output++ = *uVar3++;
            }
        }
    }
    
    return output;
}

void FUN_00198a28(DecompressionStructure *scratchpad) {
    scratchpad->finished = 0;
    scratchpad->s2 = 0;
    scratchpad->s0 = 0;
    scratchpad->s1 = 0;
    FUN_00199100(scratchpad, 0x10);
    scratchpad->s3 = 0;
}

u32 FUN_00198a68(DecompressionStructure *scratchpad) {
    u32 uVar1;
    u32 mask;
    
    if (!scratchpad->s3) {
        scratchpad->cycles--;
        if (scratchpad->cycles < 0) {
            scratchpad->finished = 1;
            return 0;
        }
        scratchpad->s3 = FUN_001990c0(scratchpad, 0x10);
        FUN_00198c40(scratchpad, 0x13, 5, 3);
        FUN_00198e90(scratchpad);
        FUN_00198c40(scratchpad, 0x10, 4, -1);
        if (scratchpad->finished != 0) {
            return 0;
        }
    }
    scratchpad->s3--;
    uVar1 = scratchpad->buffer3[scratchpad->s1 >> 4];
    mask = 8;
    
    while (uVar1 >= 0x1fe) {
        uVar1 = scratchpad->s1 & mask ? scratchpad->buffer5[uVar1] : scratchpad->buffer4[uVar1];
        mask >>= 1;
    }
    FUN_00199100(scratchpad, scratchpad->buffer1[uVar1 - 1]);
    return uVar1;
}

void FUN_00198c40(DecompressionStructure *scratchpad, s16 param_2, s16 param_3, s16 param_4) {

}

u16 FUN_001990c0(DecompressionStructure *scratchpad, s16 param_2) {
    u16 uVar1 = scratchpad->s1 >> (0x10 - param_2) & 0xFFFF;
    FUN_00199100(scratchpad, param_2);
    return uVar1;
}

void FUN_00199100(DecompressionStructure *scratchpad, u16 param_2) {
    // something
}
