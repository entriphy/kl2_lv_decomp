#include <iostream>
#include <ostream>
#include <stdio.h>
#include <math.h>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include "pine.h"

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define GLSL_VERSION "#version 330"
#define stringify( name ) #name
static const char* EmuStatuses[] = { "Running", "Paused", "Shutdown" };
static const ImVec4 EmuStatusColors[] = { ImVec4(0.0, 1.0, 0.0, 1.0), ImVec4(1.0, 1.0, 0.0, 1.0), ImVec4(1.0, 0.0, 0.0, 1.0) };

int main() {
    // Initialize PINE
    PINE::PCSX2 *ipc = new PINE::PCSX2();
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

    float color[] = { 1.0, 1.0, 1.0, 1.0 };

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

        
        ImGui::Begin("Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        {
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

                ImGui::EndMenuBar();
            }
            ImGui::Text("Hello, Dear ImGUI!");
            ImGui::ColorEdit4("Color", color);
            float samples[100];
            for (int n = 0; n < 100; n++)
                samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 10.0f);
            ImGui::PlotLines("Samples", samples, 100);
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
