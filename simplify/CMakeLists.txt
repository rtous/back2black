set(SIMPLIFY_LIB simplify) 

add_library(${SIMPLIFY_LIB} simplify.cpp simplify.h)

target_include_directories(${SIMPLIFY_LIB} PUBLIC .)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

install(TARGETS ${SIMPLIFY_LIB} RUNTIME)
target_link_libraries(${SIMPLIFY_LIB} PRIVATE 
    ${OpenCV_LIBS} 
    ${CMAKE_THREAD_LIBS_INIT} 
    sam
    common
    imgui-sdl2)

target_compile_features(${SIMPLIFY_LIB} PUBLIC cxx_std_17)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${SIMPLIFY_LIB} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()

