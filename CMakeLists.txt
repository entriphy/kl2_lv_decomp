cmake_minimum_required(VERSION 3.21)
project(kl2_lv_decomp LANGUAGES C CXX ASM)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
enable_language(DSM)

add_subdirectory(iop)
if(CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 2.9)
    # Set SCE_OBSOLETE if using older libraries
    target_compile_definitions(kl2_irx PUBLIC SCE_OBSOLETE)
endif()

# SCE PS2 SDK
set(SCESDK $ENV{SCESDK})
unset(CMAKE_DEPFILE_FLAGS_C)
unset(CMAKE_DEPFILE_FLAGS_CXX)
unset(CMAKE_DEPFILE_FLAGS_ASM)

# crt0.s
add_library(PS2_CRT0 OBJECT ${SCESDK}/ee/lib/crt0.s)
target_compile_options(PS2_CRT0 PRIVATE -x assembler-with-cpp)
set_target_properties(PS2_CRT0 PROPERTIES OUTPUT_NAME "crt0.o")

set(EXECUTABLES)

# C files
set(SUBDIRS abe harada hato hoshino kazuya nakano okanoyo taka take okanoyo/debug vtusr hoshino/kit hoshino/take vtusr/taro okanoyo/linux)
foreach (SUBDIR IN LISTS SUBDIRS)
    file(GLOB files src/${SUBDIR}/*.c)
    foreach (file ${files})
        list(APPEND EXECUTABLES ${file})
    endforeach()
endforeach()

# C++ files
foreach (SUBDIR IN LISTS SUBDIRS)
    file(GLOB files src/${SUBDIR}/*.cc)
    foreach (file ${files})
        list(APPEND EXECUTABLES ${file})
    endforeach()
endforeach()

# DSM/VSM files
file(GLOB files vu/*.dsm)
foreach (file ${files})
    list(APPEND EXECUTABLES ${file})
endforeach()

add_executable(${PROJECT_NAME} ${EXECUTABLES})
add_dependencies(${PROJECT_NAME} PS2_CRT0 kl2_irx)
set(CMAKE_EXECUTABLE_SUFFIX ".elf")
target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<COMPILE_LANGUAGE:C>:-O1 -gstabs>
        $<$<COMPILE_LANGUAGE:CXX>:-O2 -gstabs -fno-exceptions>
        $<$<COMPILE_LANGUAGE:DSM>:-I${PROJECT_SOURCE_DIR}/vu/include -I${SCESDK}/ee/include>
)
target_compile_definitions(${PROJECT_NAME} PRIVATE SCE)
target_link_options(${PROJECT_NAME} PRIVATE -T${SCESDK}/ee/lib/app.cmd -Wall -Werror -Wa,-al -fno-common -mno-crt0)
target_include_directories(${PROJECT_NAME} PRIVATE ${SCESDK}/common/include ${SCESDK}/ee/include include src)
target_link_directories(${PROJECT_NAME} PRIVATE ${SCESDK}/ee/lib)
target_link_libraries(${PROJECT_NAME} PRIVATE m cdvd graph dma vu0 dev sdr pad ipu)

if (DEFINED KL2_RETAIL)
    target_compile_definitions(${PROJECT_NAME} PRIVATE RETAIL)
endif()

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
    target_link_libraries(${PROJECT_NAME} PRIVATE PS2_CRT0)
else()
    # Copy crt0.s object to build directory as crt0.o
    add_custom_command(TARGET ${PROJECT_NAME}
        PRE_LINK
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:PS2_CRT0> ${CMAKE_CURRENT_BINARY_DIR}/crt0.o)
endif()
