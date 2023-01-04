#include <iostream>
#include <ostream>
#include <stdio.h>
#include <math.h>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include "pine.h"
#include "types.h" // Modified to work on 64-bit (removes pointers)

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLSL_VERSION "#version 330"
#define stringify( name ) #name
static const char* EmuStatuses[] = { "Running", "Paused", "Shutdown" };
static const ImVec4 EmuStatusColors[] = { ImVec4(0.0, 1.0, 0.0, 1.0), ImVec4(1.0, 1.0, 0.0, 1.0), ImVec4(1.0, 0.0, 0.0, 1.0) };
static const ImVec4 FpsColor = ImVec4(0.0, 0.75, 0.75, 1.0);

u8* ps2_ram = (u8*)malloc(0x2000000);
bool isValidPointer(uint ptr) {
    return ptr != 0 && ptr < 0x2000000;
}

uint getFHMAddress(PINE::PCSX2* ipc, uint addr, uint index) {
    return addr + ipc->Read<uint32_t>(addr + 4 + index * 4);
}

char* getAnimationName(PINE::PCSX2* ipc, uint packAddr, uint num) {
    uint animationNameAddr = getFHMAddress(ipc, packAddr, num) + 8;
    if (isValidPointer(animationNameAddr)) {
        *(ulong *)(ps2_ram + animationNameAddr) = ipc->Read<uint64_t>(animationNameAddr);
        return (char *)(ps2_ram + animationNameAddr);
    } else {
        return "N/A";
    }
}

int main() {
    // Initialize PINE
    PINE::PCSX2* ipc = new PINE::PCSX2();
    try {
        // Attempt connection
        ipc->Status();
    } catch (PINE::Shared::IPCStatus status) {
        std::cout << "PINE connection to PCSX2 failed (status " << status << ")\n";
        glfwTerminate();
        return -1;
    }

    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Klonoa 2 Thing", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Could not create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (monitor != nullptr) {
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        if (mode != nullptr) {
            int monitorX, monitorY;
            glfwGetMonitorPos(monitor, &monitorX, &monitorY);

            int windowWidth, windowHeight;
            glfwGetWindowSize(window, &windowWidth, &windowHeight);
            glfwSetWindowPos(window, monitorX + (mode->width - windowWidth) / 2, monitorY + (mode->height - windowHeight) / 2);
        }
    }

    // Initialize ImGui
    ImGui::CreateContext();
    ImGui::GetStyle().WindowRounding = 0.0f;

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.IniFilename = nullptr;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);

    // ImGui loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int app_width, app_height;
        glfwGetFramebufferSize(window, &app_width, &app_height);
        glViewport(0, 0, app_width, app_height);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(640, 480));

        uint irqc = ipc->Read<uint32_t, false>(0x0036EAC0);
        int objwork = ipc->Read<uint32_t, false>(0x003667DC); // GameGbl.klonoa
        for (int i = 0; i < sizeof(OBJWORK); i++) {
            ps2_ram[objwork + i] = ipc->Read<u8>(objwork + i);
        }
        OBJWORK* objw = (OBJWORK *)(ps2_ram + objwork);

        u32 pModel = objw->prim;
        for (int i = 0; i < sizeof(klMODEL); i++) {
            ps2_ram[pModel + i] = ipc->Read<u8>(pModel + i);
        }
        klMODEL* model = (klMODEL* )(ps2_ram + pModel);

        uint levelPackAddr = ipc->Read<uint32_t>(0x003FC2E8);
        uint nakanoPack = getFHMAddress(ipc, levelPackAddr, 0);
        uint chrPack = getFHMAddress(ipc, nakanoPack, 1);
        uint klonoaPack = getFHMAddress(ipc, chrPack, 371);
        uint klmPack = getFHMAddress(ipc, klonoaPack, 0);
        uint klonoaModel = getFHMAddress(ipc, klmPack, 0);
        uint klonoaAnimationPack = getFHMAddress(ipc, klmPack, 3);

        uint vision = ipc->Read<uint32_t>(0x00366454); // GameGbl.vision
        uint vtWaveAddr = ipc->Read<uint32_t>(0x003FB940);
        
        ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        {
            ImGui::SetWindowFontScale(2.0f);
            if (ImGui::BeginMenuBar()) {
                // Show PINE status
                ImGui::Text("PINE:");
                PINE::PCSX2::EmuStatus status;
                try {
                    status = ipc->Status();
                } catch (PINE::Shared::IPCStatus ex) {
                    status = PINE::PCSX2::EmuStatus::Shutdown;
                }
                ImGui::PushStyleColor(ImGuiCol_Text, EmuStatusColors[status]);
                ImGui::Text("%s", EmuStatuses[status]);
                ImGui::PopStyleColor();

                // Window FPS counter
                ImGui::Text("FPS:");
                ImGui::PushStyleColor(ImGuiCol_Text, FpsColor);
                ImGui::Text("%.0f", io.Framerate);
                ImGui::PopStyleColor();

                // IRQC (in-game frame) count
                // ImGui::Text("IRQC:");
                // ImGui::PushStyleColor(ImGuiCol_Text, FpsColor);
                // ImGui::Text("%d", irqc);
                // ImGui::PopStyleColor();

                ImGui::EndMenuBar();
            }

            ImGui::Text("vision:        %04x", vision);
            ImGui::Text("");

            ImGui::Text("klonoa->posi:  %.2f %.2f %.2f %.2f", objw->posi.x, objw->posi.y, objw->posi.z, objw->posi.w);
            ImGui::Text("klonoa->spd:   %.4f %.4f %.4f %.4f", objw->spd.x, objw->spd.y, objw->spd.z, objw->spd.w);
            ImGui::Text("klonoa->muki:  %.2f %.2f %.2f %.2f", objw->muki.x, objw->muki.y, objw->muki.z, objw->muki.w);
            ImGui::Text("klonoa->ang:   %.2f %.2f %.2f %.2f", objw->ang.x, objw->ang.y, objw->ang.z, objw->ang.w);
            // ImGui::Text("klonoa->rot:   %.2f %.2f %.2f %.2f", objw->rot.x, objw->rot.y, objw->rot.z, objw->rot.w);
            ImGui::Text("");

            if (isValidPointer(klonoaAnimationPack)) {
                char* animation = getAnimationName(ipc, klonoaAnimationPack, model->klm.ActNum);
                ImGui::Text("klonoa->ActNum  %d (%s)", model->klm.ActNum, animation);
                ImGui::Text("");
            }

            if (isValidPointer(vtWaveAddr)) {
                for (int i = 0; i < sizeof(kitWaveParam); i++) {
                    ps2_ram[vtWaveAddr + 0x10 + i] = ipc->Read<u8>(vtWaveAddr + 0x10 + i);
                }
                kitWaveParam* wave = (kitWaveParam*)(ps2_ram + vtWaveAddr + 0x10);
                if (ImGui::SliderFloat("wave->speedx", &wave->speedx, 1, 200)) {
                    ipc->Write<float>(vtWaveAddr + 0x10 + offsetof(kitWaveParam, speedx), wave->speedx);
                }
                ImGui::Text("");
            }

            // gp_value: 0x3FE070
        }
        
        
        ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        glfwSwapBuffers(window);
    }

    // Shutdown
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    delete ipc;

    return 0;
}
