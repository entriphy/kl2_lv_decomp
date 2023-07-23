#include "ab_init.h"
#include "ab_tex.h"

extern "C" {
    void abGimInit() {
        // TODO
        CTexManager::Initialize();
    }

    void abStageInitA() {
        // TODO
        abGimInit();
    }
}

