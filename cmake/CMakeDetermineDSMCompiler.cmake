find_program(
        CMAKE_DSM_COMPILER
        NAMES "ee-dvp-as"
        HINTS "${CMAKE_SOURCE_DIR}"
        DOC "DSM compiler"
)
mark_as_advanced(CMAKE_DSM_COMPILER)

set(CMAKE_DSM_SOURCE_FILE_EXTENSIONS dsm;DSM)
set(CMAKE_DSM_OUTPUT_EXTENSION .o)
set(CMAKE_DSM_COMPILER_ENV_VAR "DSM")

configure_file(${CMAKE_CURRENT_LIST_DIR}/CMakeDSMCompiler.cmake.in
        ${CMAKE_PLATFORM_INFO_DIR}/CMakeDSMCompiler.cmake)
