cmake_minimum_required(VERSION 3.5)

project(ImGuiFileDialog)

add_library(ImGuiFileDialog STATIC
    ImGuiFileDialog.cpp
    ImGuiFileDialog.h
    ImGuiFileDialogConfig.h
)

target_link_libraries(ImGuiFileDialog PRIVATE imgui-sdl2 ${CMAKE_THREAD_LIBS_INIT})
target_include_directories(ImGuiFileDialog PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
    target_compile_options(ImGuiFileDialog PUBLIC -Wno-unknown-pragmas)
endif()
