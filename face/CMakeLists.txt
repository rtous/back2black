set(FACE_LIB face)

add_library(${FACE_LIB} face.cpp face.h)

target_include_directories(${FACE_LIB} PUBLIC .)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

install(TARGETS ${FACE_LIB} RUNTIME)
target_link_libraries(${FACE_LIB} PRIVATE dlib ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

target_compile_features(${FACE_LIB} PUBLIC cxx_std_17)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${FACE_LIB} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()

