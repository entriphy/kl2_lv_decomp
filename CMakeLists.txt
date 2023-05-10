cmake_minimum_required(VERSION 3.21)
project(kl2_lv_decomp LANGUAGES CXX C ASM)

add_subdirectory(iop)

# SCE PS2 SDK
set(SCESDK $ENV{SCESDK})
unset(CMAKE_DEPFILE_FLAGS_C)
unset(CMAKE_DEPFILE_FLAGS_CXX)
unset(CMAKE_DEPFILE_FLAGS_ASM)
add_compile_options(-O1 -g3 -U__STRICT_ANSI__ -B ${SCESDK}/gcc/bin/ee-)
add_compile_definitions(SCE)
add_link_options(-T${SCESDK}/ee/lib/app.cmd -Wall -Werror -Wa,-al -fno-common -mno-crt0 -B ${SCESDK}/gcc/bin/ee-)
include_directories(${SCESDK}/common/include ${SCESDK}/ee/include include)
link_directories(${SCESDK}/ee/lib)
link_libraries(m cdvd graph dma vu0 dev sdr)

# Decomp
set(EXECUTABLES)
set(SUBDIRS abe harada hato hoshino kazuya nakano okano taka)
foreach (SUBDIR IN LISTS SUBDIRS)
    file(GLOB files src/${SUBDIR}/*.c)
    foreach (file ${files})
        list(APPEND EXECUTABLES ${file})
    endforeach()

    file(GLOB files src/${SUBDIR}/*.cpp)
    foreach (file ${files})
        list(APPEND EXECUTABLES ${file})
    endforeach()
endforeach()

# crt0.s
add_library(PS2_CRT0 OBJECT ${SCESDK}/ee/lib/crt0.s)
target_compile_options(PS2_CRT0 PRIVATE -x assembler-with-cpp)
set_target_properties(PS2_CRT0 PROPERTIES OUTPUT_NAME "crt0.o")

# Executable
add_executable(${PROJECT_NAME} ${EXECUTABLES})
add_dependencies(${PROJECT_NAME} PS2_CRT0 kl2_irx)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C "${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_EXECUTABLE_SUFFIX_CXX "${CMAKE_EXECUTABLE_SUFFIX}")

# Copy kl2.irx to build directory
add_custom_command(TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:kl2_irx> ${CMAKE_CURRENT_BINARY_DIR}/kl2.irx)

# Copy IOP image to build directory
file(GLOB IOP_IMAGES "${SCESDK}/iop/modules/*.img")
foreach (IOP_IMAGE ${IOP_IMAGES})
    cmake_path(GET IOP_IMAGE FILENAME IOP_IMAGE_FILENAME)
    configure_file(${IOP_IMAGE} ${IOP_IMAGE_FILENAME} COPYONLY)
endforeach()

# Copy IOP modules to build directory
set(IOP_MODULES sio2man mcman mcserv padman libsd sdrdrv)
foreach (MODULE IN LISTS IOP_MODULES)
    configure_file(${SCESDK}/iop/modules/${MODULE}.irx ${MODULE}.irx COPYONLY)
endforeach()

# Link crt0.s
if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.0.0)
    # Link to crt0.s object
    target_link_libraries(${PROJECT_NAME} PS2_CRT0)
else()
    # Copy crt0.s object to build directory as crt0.o
    add_custom_command(TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:PS2_CRT0> ${CMAKE_CURRENT_BINARY_DIR}/crt0.o)
endif()
