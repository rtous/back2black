#TOP LEVEL CMAKE

cmake_minimum_required(VERSION 3.12)
project("back2black")

if (NOT XCODE AND NOT MSVC AND NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


#from: https://trenki2.github.io/blog/2017/06/02/using-sdl2-with-cmake/
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}")


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(third-party)
add_subdirectory(common)
add_subdirectory(test_opencv)
add_subdirectory(cli_sam)
add_subdirectory(cli_simplify)
add_subdirectory(editor)
add_subdirectory(cli_masks)
add_subdirectory(test_filesystem)
