set(EXAMPLE_TARGET editor)

add_executable(${EXAMPLE_TARGET}
    colors.cpp
    gui_utils.cpp
    main.cpp
    editor.cpp
    file_dialog.cpp
    sam_utils.cpp
    video.cpp
)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

#dlib stuff (faces)
#https://medium.com/@abhishekjainindore24/face-detection-using-opencv-and-dlib-library-in-c-592d9bf269ff
#find_package(dlib REQUIRED)
include_directories(${dlib_INCLUDE_DIRS})


#TODO: for some strange reason if "common" appears before ${OpenCV_LIBS} then it cannot read "jpg" images...
install(TARGETS ${EXAMPLE_TARGET} RUNTIME)
target_link_libraries(${EXAMPLE_TARGET} PRIVATE 
    segmentor 
    segmentor_sam1
    segmentor_sam2 
    sam simplify 
    face 
    ${OpenCV_LIBS} 
    common 
    dlib 
    imgui-sdl2 
    ImGuiFileDialog  
    ${CMAKE_THREAD_LIBS_INIT}
    ${ONNXRUNTIME_LINK_LIBRARIES_GLOBAL}
    )

target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_11)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${EXAMPLE_TARGET} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()


if (WIN32)
# this code copies the sdl2.dll file to the same folder as the executable
add_custom_command(
    TARGET ${EXAMPLE_TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/third-party/sdl2/sdl2.dll"
    $<TARGET_FILE_DIR:${EXAMPLE_TARGET}>
    VERBATIM)
endif()

