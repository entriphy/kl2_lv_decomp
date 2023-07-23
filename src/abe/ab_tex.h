#ifndef AB_TEX_H
#define AB_TEX_H

#include "abe.h"

#ifdef __cplusplus

class CTexManager {
    protected:
        s32 m_texID[672];

    public:
        static void Initialize();
        static CTexManager* Instance();
};

#endif // __cplusplus

#endif
