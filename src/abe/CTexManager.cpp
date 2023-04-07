#include "types.h"
#include "abe.h"

void CTexManager::Initialize() {
    for (int index = 0; index < 672; ++index) {
        CTexManager::Instance()->m_texID[index] = 0;
    }
}

CTexManager* CTexManager::Instance() {
    return &m_instance;
}
