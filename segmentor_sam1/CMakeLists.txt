set(API2 segmentor_sam1) 

add_library(${API2} segmentor_sam1.cpp segmentor_sam1.h)

target_include_directories(${API2} PUBLIC .)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

install(TARGETS ${API2} RUNTIME)

#WARNING: need to include dependencies of the dependencies
target_link_libraries(${API2} PRIVATE 
    ${OpenCV_LIBS} 
    ${CMAKE_THREAD_LIBS_INIT} 
    segmentor 
    sam
    imgui-sdl2 #dependency of common
    common 
    )

target_compile_features(${API2} PUBLIC cxx_std_17)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${API2} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()
