#include <iostream>
#include <ostream>
#include <string>
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
    OBJWORK<klMODEL>* klonoa = memory.read_obj<OBJWORK<klMODEL>>(pKlonoa);
    klMODEL* model = klonoa->prim.Get(&memory);
    
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

        if (ImGui::TreeNode("Klonoa")) {
            klonoa->drawObj(&memory);

            uint pLevelPack = memory.read<uint>(KlonoaMemory::LEVEL_PACK_PTR_ADDRESS);
            KlonoaMemory::FHM* levelPack = memory.read_FHM(pLevelPack);
            KlonoaMemory::FHM* nakanoPack = levelPack->read_FHM(&memory, 0);
            KlonoaMemory::FHM* chrPack = nakanoPack->read_FHM(&memory, 1);
            KlonoaMemory::FHM* klonoaPack = chrPack->read_FHM(&memory, 371);
            KlonoaMemory::FHM* klmPack = klonoaPack->read_FHM(&memory, 0);
            KlonoaMemory::FHM* klonoaAnimationPack = klmPack->read_FHM(&memory, 3);

            if (KlonoaMemory::isValidPointer(klonoaAnimationPack->get_address(&memory))) {
                uint nameAddress = klonoaAnimationPack->get_file_address(&memory, model->klm.ActNum) + 8;
                if (!KlonoaMemory::isValidPointer(nameAddress)) return;
                char* animation = (char *)(memory.read_obj<ulong>(nameAddress));
                ImGui::Text("ActNum  %d (%s)", model->klm.ActNum, animation);
            }

            ImGui::TreePop();
        }

        if (KlonoaMemory::isValidPointer(vtWaveAddr)) {
            uint waveCount = memory.read<uint>(vtWaveAddr);
            for (int i = 0; i < waveCount; i++) {
                std::string name = "Wave_";
                name.push_back('0' + i);
                if (ImGui::TreeNode(name.c_str())) {
                    kitWaveParam* wave = memory.read_obj<kitWaveParam>(vtWaveAddr + 0x10 + i * sizeof(kitWaveParam));
                    wave->draw(&memory);
                    ImGui::TreePop();
                }
            }
        }

        // gp_value: 0x3FE070
    }
}

int main() {
    window.draw = draw;
    window.loop();

    return 0;
}
