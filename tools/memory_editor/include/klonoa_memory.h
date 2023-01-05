#ifndef KLONOA_MEM_H
#define KLONOA_MEM_H

#include "pine.h"

class KlonoaMemory {
public:
    static const uint IRQC_ADDRESS = 0x0036EAC0;
    static const uint GAMEGBL_VISION = 0x00366454;
    static const uint GAMEGBL_KLONOA_ADDRESS = 0x003667DC;
    static const uint LEVEL_PACK_PTR_ADDRESS = 0x003FC2E8;
    static const uint VT_WAVE_ADDRESS = 0x003FB940;

    static bool isValidPointer(uint ptr) {
        return ptr != 0 && ptr < 0x2000000;
    }

    PINE::PCSX2* ipc;
    char* ps2_ram;

    KlonoaMemory()  {
        ipc = new PINE::PCSX2();
        ps2_ram = (char*)malloc(0x2000000);
    }

    ~KlonoaMemory() {
        delete ipc;
        free(ps2_ram);
    }

    template <class T>
    T read(uint address) {
        return ipc->Read<T>(address);
    }

    template <class T>
    T* read_obj(uint address) {
        for (int i = 0; i < sizeof(T); i++) {
            ps2_ram[address + i] = ipc->Read<char>(address + i);
        }
        return (T*)(ps2_ram + address);
    }

    template <class T>
    inline void write(uint address, T value) {
        ipc->Write<T>(address, value);
    }

    template <class T>
    inline void write_obj(uint address) {
        for (int i = 0; i < sizeof(T); i++) {
            ipc->Write<char>(address + i, *(ps2_ram + address + i));
        }
    }

    // Class to traverse the game's archive files
    class FHM {
    public:
        inline uint get_address(KlonoaMemory* mem) {
            return ((char*)this - mem->ps2_ram);
        }

        uint get_count(KlonoaMemory* mem) {
            return mem->read<uint>(get_address(mem));
        }

        uint get_file_address(KlonoaMemory* mem, uint index) {
            uint addr = get_address(mem);
            return addr + mem->read<uint>(addr + 4 + index * 4);
        }

        template <class T>
        T* read_obj(KlonoaMemory* mem, uint index) {
            uint address = get_file_address(mem, index);
            return mem->read_obj<T>(address);
        }

        FHM* read_FHM(KlonoaMemory* mem, uint index) {
            uint address = get_file_address(mem, index);
            return mem->read_FHM(address);
        }
    };

    FHM* read_FHM(uint address) {
        return (FHM *)(ps2_ram + address);
    }
};

#endif
