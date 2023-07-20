#include "common.h"

void hr_abe_efcOn(PTEFC id, sceVu0FVECTOR v) {
    abSetPtEffect(id, v);
}

void hr_abe_efcWork(PTEFC id, sceVu0FVECTOR v) {
    abLocatePtEffect(id, v);
}

void hr_abe_efcDel(PTEFC id) {
    abDeletePtEffect(id);
}

void hr_abe_initCE() {
    abInitCameraEffect();
}

void hr_abe_efcClean() {
    abCleanUpPtEffect();
}

void hr_hato_efcOn(PTEFC id, sceVu0FVECTOR v, sceVu0FVECTOR r) {
    htSetPtBoss();
}

void hr_hato_efcWork(PTEFC id, sceVu0FVECTOR v, sceVu0FVECTOR r) {
    htLocatePtBoss(id, v, r);
}

void hr_hato_efcDel(PTEFC id) {
    htDeletePtBoss(id);
}