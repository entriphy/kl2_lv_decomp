if(NOT CMAKE_DSM_COMPILE_OBJECT)
    set(CMAKE_DSM_COMPILE_OBJECT "<CMAKE_DSM_COMPILER> <FLAGS> <SOURCE> -o <OBJECT>")
endif()
set(CMAKE_DSM_INFORMATION_LOADED 1)