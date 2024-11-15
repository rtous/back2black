set(EXAMPLE_TARGET test_face)

add_executable(${EXAMPLE_TARGET}
    main.cpp
)

set(OpenCV_STATIC ON)
#set(OpenCV_DIR "/Users/rtous/dev/opencv/build")
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
#set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../third-party/opencv/build_macos")
find_package(OpenCV REQUIRED) 

include(CMakePrintHelpers)
cmake_print_variables(OpenCV_INCLUDE_DIRS)
cmake_print_variables(OpenCV_LIBS)

target_link_libraries(${EXAMPLE_TARGET} PRIVATE ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} dlib common sam face imgui-sdl2)
target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_11)

#If Microsoft Visual C++ or similar
#if(MSVC)
#    target_compile_definitions(${EXAMPLE_TARGET} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
#endif()
