cmake_minimum_required(VERSION 3.17)
project(kl2_lv_decomp)

set(INCLUDE_DIR "include")

# ps2dev
if(DEFINED ENV{PS2DEV})
    SET(PS2DEV $ENV{PS2DEV})
    SET(PS2SDK $ENV{PS2SDK})
else()
    message(FATAL_ERROR "The environment variable PS2DEV needs to be defined.")
endif()

SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)
SET(CMAKE_SYSTEM_PROCESSOR mips)
SET(CMAKE_C_COMPILER "${PS2DEV}/ee/bin/mips64r5900el-ps2-elf-gcc")
SET(CMAKE_CXX_COMPILER "${PS2DEV}/ee/bin/mips64r5900el-ps2-elf-g++")
SET(PLATFORM_PS2 TRUE)
SET(PS2 TRUE)

add_compile_options(-O1 -G0 -g2 -gdwarf-2 -gstrict-dwarf -Wno-strict-aliasing)
add_compile_definitions(_EE PS2 __PS2__)
add_link_options(-T${PS2SDK}/ee/startup/linkfile -Wl,-zmax-page-size=128 -lvux -ldebug -lc -lcdvd -lxcdvd)
include_directories(${PS2SDK}/common/include ${PS2SDK}/ee/include include)
link_directories(${PS2SDK}/ee/lib)
link_libraries(vux debug cdvd xcdvd)

# Decomp
set(EXECUTABLES src/kl2_lv.cpp src/common.cpp)
set(SUBDIRS abe harada hato hoshino kazuya kazuya nakano okanoyo)
foreach (SUBDIR IN LISTS SUBDIRS)
    file(GLOB files "src/${SUBDIR}/*.cpp")
    foreach (file ${files})
        list(APPEND EXECUTABLES "${file}")
    endforeach()
endforeach()

add_executable(${PROJECT_NAME} ${EXECUTABLES})
set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}.elf)
