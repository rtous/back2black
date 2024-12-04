set(SEGMENTOR_LIB segmentor)

add_library(${SEGMENTOR_LIB} segmentor.h segmentor.cpp)

target_include_directories(${SEGMENTOR_LIB} PUBLIC .)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

install(TARGETS ${SEGMENTOR_LIB} RUNTIME)
target_link_libraries(${SEGMENTOR_LIB} PRIVATE 
    ${OpenCV_LIBS} 
    ${CMAKE_THREAD_LIBS_INIT})

target_compile_features(${SEGMENTOR_LIB} PUBLIC cxx_std_17)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${SEGMENTOR_LIB} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()

