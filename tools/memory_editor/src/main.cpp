#include <iostream>
#include <ostream>
#include <stdio.h>
#include <math.h>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include "pine.h"
#include "types.h" // Modified to work on 64-bit (removes pointers)
#include "klonoa_memory.h"
#include "window.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLSL_VERSION "#version 330"

static const char* EmuStatuses[] = { "Running", "Paused", "Shutdown" };
static const ImVec4 EmuStatusColors[] = { ImVec4(0.0, 1.0, 0.0, 1.0), ImVec4(1.0, 1.0, 0.0, 1.0), ImVec4(1.0, 0.0, 0.0, 1.0) };
static const ImVec4 FpsColor = ImVec4(0.0, 0.75, 0.75, 1.0);

KlonoaMemory memory;
Window window;

void draw() {
    uint irqc = memory.read<uint>(KlonoaMemory::IRQC_ADDRESS);
    uint vision = memory.read<uint>(KlonoaMemory::GAMEGBL_VISION); // GameGbl.vision
    uint vtWaveAddr = memory.read<uint>(KlonoaMemory::VT_WAVE_ADDRESS);

    uint pKlonoa = memory.read<uint>(KlonoaMemory::GAMEGBL_KLONOA_ADDRESS); // GameGbl.klonoa
    OBJWORK* klonoa = memory.read_obj<OBJWORK>(pKlonoa);

    uint pModel = klonoa->prim;
    klMODEL* model = memory.read_obj<klMODEL>(pModel);

    uint pLevelPack = memory.read<uint>(KlonoaMemory::LEVEL_PACK_PTR_ADDRESS);
    KlonoaMemory::FHM* levelPack = memory.read_FHM(pLevelPack);
    KlonoaMemory::FHM* nakanoPack = levelPack->read_FHM(&memory, 0);
    KlonoaMemory::FHM* chrPack = nakanoPack->read_FHM(&memory, 1);
    KlonoaMemory::FHM* klonoaPack = chrPack->read_FHM(&memory, 371);
    KlonoaMemory::FHM* klmPack = klonoaPack->read_FHM(&memory, 0);
    KlonoaMemory::FHM* klonoaAnimationPack = klmPack->read_FHM(&memory, 3);
    
    ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
    {
        if (ImGui::BeginMenuBar()) {
            // Show PINE status
            ImGui::Text("PINE:");
            PINE::PCSX2::EmuStatus status;
            try {
                status = memory.ipc->Status();
            } catch (PINE::Shared::IPCStatus ex) {
                status = PINE::PCSX2::EmuStatus::Shutdown;
            }
            ImGui::PushStyleColor(ImGuiCol_Text, EmuStatusColors[status]);
            ImGui::Text("%s", EmuStatuses[status]);
            ImGui::PopStyleColor();

            // Window FPS counter
            ImGui::Text("FPS:");
            ImGui::PushStyleColor(ImGuiCol_Text, FpsColor);
            ImGui::Text("%.0f", window.io->Framerate);
            ImGui::PopStyleColor();

            // IRQC (in-game frame) count
            ImGui::Text("IRQC:");
            ImGui::PushStyleColor(ImGuiCol_Text, FpsColor);
            ImGui::Text("%d", irqc);
            ImGui::PopStyleColor();

            ImGui::EndMenuBar();
        }

        ImGui::Text("vision:        %04x", vision);
        ImGui::Text("");

        ImGui::Text("klonoa->posi:  %.2f %.2f %.2f %.2f", klonoa->posi.x, klonoa->posi.y, klonoa->posi.z, klonoa->posi.w);
        ImGui::Text("klonoa->spd:   %.4f %.4f %.4f %.4f", klonoa->spd.x, klonoa->spd.y, klonoa->spd.z, klonoa->spd.w);
        ImGui::Text("klonoa->muki:  %.2f %.2f %.2f %.2f", klonoa->muki.x, klonoa->muki.y, klonoa->muki.z, klonoa->muki.w);
        ImGui::Text("klonoa->ang:   %.2f %.2f %.2f %.2f", klonoa->ang.x, klonoa->ang.y, klonoa->ang.z, klonoa->ang.w);
        // ImGui::Text("klonoa->rot:   %.2f %.2f %.2f %.2f", objw->rot.x, objw->rot.y, objw->rot.z, objw->rot.w);
        ImGui::Text("");

        if (KlonoaMemory::isValidPointer(klonoaAnimationPack->get_address(&memory))) {
            uint nameAddress = klonoaAnimationPack->get_file_address(&memory, model->klm.ActNum) + 8;
            char* animation = (char *)(memory.read_obj<ulong>(nameAddress));
            ImGui::Text("klonoa->ActNum  %d (%s)", model->klm.ActNum, animation);
            ImGui::Text("");
        }

        if (KlonoaMemory::isValidPointer(vtWaveAddr)) {
            kitWaveParam* wave = memory.read_obj<kitWaveParam>(vtWaveAddr + 0x10);
            if (ImGui::SliderFloat("wave->speedx", &wave->speedx, 1, 200)) {
                memory.ipc->Write<float>(vtWaveAddr + 0x10 + offsetof(kitWaveParam, speedx), wave->speedx);
            }
            ImGui::Text("");
        }

        // gp_value: 0x3FE070
    }
}

int main() {
    window.draw = draw;
    window.loop();

    return 0;
}
