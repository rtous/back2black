#(optional) Added this to include the imgui tool binary_to_compressed_c
add_executable(imgui_binary_to_compressed_c
    imgui/misc/fonts/binary_to_compressed_c.cpp
)

#Locate SDL2
if (APPLE)
    find_package(SDL2 NO_MODULE) #To avoid using FindSDL2.cmake 
    #Note: Without FindSDL2.cmake it finds a config in homebrew dirs
    #can check with --debug-find option in cmake
else ()
    find_package(SDL2 REQUIRED) #To force using FindSDL2.cmake
endif ()

string(STRIP "${SDL2_LIBRARIES}" SDL2_LIBRARIES)

#set(SDL2_LIBRARIES "C:/SDL/lib/libSDL2.a;C:/SDL/lib/libSDL2main.a;C:/SDL/lib/libSDL2_image.dll.a")
message("RUBEN (in imgui CMakeLists.txt) SDL2_LIBRARIES set to ${SDL2_LIBRARIES}")


if (APPLE)
    set(ADDITIONAL_LIBRARIES "-framework Cocoa")
else (APPLE)
    unset(ADDITIONAL_LIBRARIES)
	#set(SDL2_LIBRARY C:/SDL/bin)
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
