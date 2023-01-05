cmake_minimum_required(VERSION 3.17)
project(memory_editor)

set(CMAKE_CXX_STANDARD 20)

find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(OpenGL REQUIRED)
link_libraries(glfw imgui OpenGL::GL)
include_directories(include)

add_executable(${PROJECT_NAME}
        src/imgui_impl_glfw.cpp
        src/imgui_impl_opengl3.cpp
        src/window.cpp
        src/main.cpp)
