#include <iostream>
#include <GLFW/glfw3.h>
#include <imgui.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "window.h"

#define GLSL_VERSION "#version 330"

Window::Window() {
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(720, 720, "Klonoa 2 Thing", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw "Could not create GLFW window";
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

    io = &ImGui::GetIO();
    io->IniFilename = nullptr;
    io->ConfigWindowsMoveFromTitleBarOnly = true;

    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(GLSL_VERSION);
}

void Window::loop() {
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
        ImGui::SetNextWindowSize(ImVec2(720, 720));

        draw();

        ImGui::End();
        ImGui::Render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
}

Window::~Window() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}
