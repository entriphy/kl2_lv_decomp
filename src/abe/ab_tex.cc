#include "types.h"
#include "abe.h"

static CTexManager m_instance;

void CTexManager::Initialize() {
    for (s32 index = 0; index < 672; ++index) {
        CTexManager::Instance()->m_texID[index] = 0;
    }
}

CTexManager* CTexManager::Instance() {
    return &m_instance;
}
