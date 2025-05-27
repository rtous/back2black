set(EXAMPLE_TARGET editor)
add_executable(
    ${EXAMPLE_TARGET}
    MACOSX_BUNDLE
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
#install(TARGETS ${EXAMPLE_TARGET} RUNTIME) #sense bundle
install(TARGETS ${EXAMPLE_TARGET} RUNTIME BUNDLE DESTINATION ${CMAKE_SOURCE_DIR}/editor/release_macos)
target_link_libraries(${EXAMPLE_TARGET} PRIVATE 
    segmentor 
    segmentor_sam1
    segmentor_sam2 
    sam 
    simplify 
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

##############
# MacOS bundle
##############

#add_subdirectory(release_macos)
#https://www.ferranpujolcamins.cat/2020/11/27/Notes-CMake-macOS-bundle.html
if(APPLE)
     # Bundling macOS application
     set_target_properties(
        ${CUR_TARGET} 
        PROPERTIES
        #BUNDLE True
        MACOSX_BUNDLE_GUI_IDENTIFIER my.domain.style.identifier.${EXAMPLE_TARGET}
        MACOSX_BUNDLE_BUNDLE_NAME ${EXAMPLE_TARGET}
        MACOSX_BUNDLE_BUNDLE_VERSION "0.1"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1"
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/editor/release_macos/customtemplate.plist.in
    )
endif()