set(EXAMPLE_TARGET test_segmentor_sam21)

add_executable(${EXAMPLE_TARGET}
    main.cpp
)

#SETUP OPENCV
set(OpenCV_STATIC ON)
#set(OpenCV_DIR "/Users/rtous/dev/opencv/build")
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
#set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../third-party/opencv/build_macos")
find_package(OpenCV REQUIRED) 

include(CMakePrintHelpers)
cmake_print_variables(OpenCV_INCLUDE_DIRS)
cmake_print_variables(OpenCV_LIBS)

target_include_directories(
    ${EXAMPLE_TARGET} PUBLIC 
    ${ONNXRUNTIME_INCLUDES_GLOBAL}
)

target_link_libraries(
    ${EXAMPLE_TARGET} PRIVATE 
    gflags
    ${OpenCV_LIBS} 
    ${CMAKE_THREAD_LIBS_INIT}
    ${ONNXRUNTIME_LINK_LIBRARIES_GLOBAL}
    common
    sam #required as segmentor depends on it
    segmentor
    segmentor_sam21
    imgui-sdl2 #dependency of common
)

target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_17)

#set(CMAKE_CXX_STANDARD 17)

#If Microsoft Visual C++ or similar
#if(MSVC)
#    target_compile_definitions(${EXAMPLE_TARGET} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
#endif()

#To avoid macos code signing check error
#set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO")
#set_target_properties(test_sam2 PROPERTIES
#        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
#        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO" 
#    )
