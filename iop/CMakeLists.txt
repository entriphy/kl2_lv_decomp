cmake_minimum_required(VERSION 3.17)
project(kl2_irx LANGUAGES C)

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

SET(CMAKE_C_COMPILER ${SCESDK}/iop/gcc/bin/iop-gcc)
unset(CMAKE_DEPFILE_FLAGS_C) # Clear these flags since they don't work with GCC <3.x
add_compile_options(-Wall -Werror -G 0 -O0 -fno-delayed-branch)
add_link_options(-L${SCESDK}/iop/lib -ilb=${SCESDK}/iop/lib/libsd.ilb)
include_directories(${SCESDK}/common/include ${SCESDK}/iop/include)

add_executable(${PROJECT_NAME} entry.c str.c jam.c sound.c main.c)
set(CMAKE_EXECUTABLE_SUFFIX ".irx")
set(CMAKE_EXECUTABLE_SUFFIX_C ${CMAKE_EXECUTABLE_SUFFIX})
