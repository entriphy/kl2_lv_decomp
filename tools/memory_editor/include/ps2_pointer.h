#ifndef PS2_PTR_H
#define PS2_PTR_H

#include "klonoa_memory.h"

template <typename T>
class PS2Pointer {
public:
    uint ptr;

    T* Get(KlonoaMemory* mem) {
        return mem->read_obj<T>(ptr);
    }
};

#endif