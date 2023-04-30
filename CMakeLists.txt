cmake_minimum_required(VERSION 3.17)
project(kl2_lv_decomp LANGUAGES CXX C ASM)

set(INCLUDE_DIR "include")

# SCE PS2 SDK
if(DEFINED ENV{SCESDK})
    SET(SCESDK $ENV{SCESDK})
    SET(SCE ON)
    message(NOTICE "Using SCE PS2 SDK!")
else()
    message(FATAL_ERROR "SCESDK needs to be defined.")
endif()

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_SYSTEM_PROCESSOR mips)

SET(PLATFORM_PS2 TRUE)
SET(PS2 TRUE)

SET(CMAKE_C_COMPILER ${SCESDK}/ee/gcc/bin/ee-gcc)
SET(CMAKE_CXX_COMPILER ${SCESDK}/ee/gcc/bin/ee-g++)
SET(CMAKE_ASM_COMPILER ${SCESDK}/ee/gcc/bin/ee-gcc)
add_compile_options(-O1 -G0 -g2 -gdwarf-2 -U__STRICT_ANSI__)
add_compile_definitions(SCE)
add_link_options(-T${SCESDK}/ee/lib/app.cmd -Wall -Werror -Wa,-al -fno-common -mno-crt0)
include_directories(${SCESDK}/common/include ${SCESDK}/ee/include include)
link_directories(${SCESDK}/ee/lib)
link_libraries(m cdvd graph dma vu0 dev sdr)

set(CMAKE_C_STANDARD 99)

# Decomp
set(EXECUTABLES ${SCESDK}/ee/lib/crt0.s)
set_source_files_properties(${SCESDK}/ee/lib/crt0.s PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp")

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

# Copy IOP image to build directory
file(GLOB IOP_IMAGE "${SCESDK}/iop/modules/*.img")
cmake_path(GET IOP_IMAGE FILENAME IOP_IMAGE_FILENAME)
configure_file(${IOP_IMAGE} ${IOP_IMAGE_FILENAME} COPYONLY)

# Copy IOP modules to build directory
set(IOP_MODULES sio2man mcman mcserv padman libsd sdrdrv)
foreach (MODULE IN LISTS IOP_MODULES)
    configure_file(${SCESDK}/iop/modules/${MODULE}.irx ${MODULE}.irx COPYONLY)
endforeach()

add_executable(${PROJECT_NAME} ${EXECUTABLES})
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
set(CMAKE_EXECUTABLE_SUFFIX_C "${CMAKE_EXECUTABLE_SUFFIX}")
set(CMAKE_EXECUTABLE_SUFFIX_CXX "${CMAKE_EXECUTABLE_SUFFIX}")
