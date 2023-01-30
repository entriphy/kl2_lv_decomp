#include <libcdvd.h>
#include "common.h"

void FUN_00166140(int file, void *buf) {
    sce_print("@@@ reading file %d to 0x%08x", file, buf);

    sceCdDiskReady(0);
    while (!sceCdRead(KlTable[file].offset + cD->file.lsn, KlTable[file].count, buf, &cD->mode));
    do {
        for (int i = 0; i < 0x10000; i++) {
            // Do nothing
        }
    } while (sceCdSync(1));
}
