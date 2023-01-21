#include <sifrpc.h>
#include "hoshino.h"

int hSndPkGetSize() {
    // Pretty sure it's subtracting a pointer here, might need to fix it
    int size = (int)(sD->PkNum - 4289265);
    if (size < 0) {
        size = (int)(sD->PkNum - 4289250);
    }
    return size;
}