#include <vif_registers.h>
#include <debug.h>
#include "ps2.h"
#include "types.h"

void DevVif0Reset() {
    VIF0_FBRST = 1;
    VIF0_ERR = 6;
}

void DevVu0Reset() {
    asm __volatile__ (
        "cfc2 $v0, $vi28\n"
        "ori $v0, $v0, 0x2\n"
        "ctc2 $v0, $vi28\n"
    );
}