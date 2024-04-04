find_package(SDL2 REQUIRED)
string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

#set(SDL2_LIBRARIES "C:/SDL/lib/libSDL2.a;C:/SDL/lib/libSDL2main.a;C:/SDL/lib/libSDL2_image.dll.a")
#message("RUBEN (in imgui CMakeLists.txt) SDL2_LIBRARIES set to ${SDL2_LIBRARIES}")


if (APPLE)
    set(ADDITIONAL_LIBRARIES "-framework Cocoa")
else (APPLE)
    unset(ADDITIONAL_LIBRARIES)
endif (APPLE)

add_library(imgui
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    )

target_include_directories(imgui INTERFACE
    .
    )

target_include_directories(imgui PRIVATE
    imgui
    )

target_link_libraries(imgui PRIVATE
    ${ADDITIONAL_LIBRARIES}
    )

target_compile_features(imgui PUBLIC cxx_std_11)

find_package(OpenGL REQUIRED)

add_library(imgui-sdl2
    imgui/backends/imgui_impl_sdl2.cpp
    imgui/backends/imgui_impl_opengl3.cpp
    )

target_include_directories(imgui-sdl2 PUBLIC
    ${SDL2_INCLUDE_DIRS}
    )

target_include_directories(imgui-sdl2 PUBLIC
    imgui
    imgui/backends
    )

target_link_libraries(imgui-sdl2 PUBLIC
    imgui
    ${OPENGL_LIBRARIES}
    ${SDL2_LIBRARIES}
    )

target_link_libraries(imgui-sdl2 PRIVATE
    ${CMAKE_DL_LIBS}
    ${ADDITIONAL_LIBRARIES}
        )
# endif()
