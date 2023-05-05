if(DEFINED ENV{SCESDK})
    SET(SCESDK $ENV{SCESDK})
    SET(SCE ON)
    message(NOTICE "Using SCE PS2 SDK!")
else()
    message(FATAL_ERROR "SCESDK needs to be defined.")
endif()

SET(CMAKE_C_COMPILER ${SCESDK}/ee/gcc/bin/ee-gcc)
SET(CMAKE_CXX_COMPILER ${SCESDK}/ee/gcc/bin/ee-g++)
SET(CMAKE_ASM_COMPILER ${SCESDK}/ee/gcc/bin/ee-gcc)

# Clear these flags since they don't work with GCC <3.x
unset(CMAKE_DEPFILE_FLAGS_C)
unset(CMAKE_DEPFILE_FLAGS_CXX)
unset(CMAKE_DEPFILE_FLAGS_ASM)

# Don't check compilers
SET(CMAKE_C_COMPILER_FORCED TRUE)
SET(CMAKE_CXX_COMPILER_FORCED TRUE)
SET(CMAKE_ASM_COMPILER_FORCED TRUE)
