set(EXAMPLE_TARGET test_sam2)

add_executable(${EXAMPLE_TARGET}
    main.cpp
    sam.cpp
)

#SETUP OPENCV
#OPENCV sources build (static) in ../opencv/build (too big to include in the repo)
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

include(CMakePrintHelpers)
cmake_print_variables(OpenCV_INCLUDE_DIRS)
cmake_print_variables(OpenCV_LIBS)

target_link_libraries(${EXAMPLE_TARGET} PRIVATE gflags ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} ${myonnxruntimelib} )
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
