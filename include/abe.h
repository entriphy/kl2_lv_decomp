#ifndef ABE_H
#define ABE_H

#ifdef __cplusplus

#include "types.h"

class CTexManager {
    protected:
        s32 m_texID[672];
    
    public:
        static void Initialize();
        static CTexManager* Instance();
};

#endif

#endif
