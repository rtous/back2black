set(COMMON_LIB common) 

add_library(${COMMON_LIB} common1.cpp common1.h data_structures.h)
target_include_directories(${COMMON_LIB} PUBLIC .)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

install(TARGETS ${COMMON_LIB} RUNTIME)
target_link_libraries(${COMMON_LIB} PRIVATE sam imgui-sdl2 ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})

#disabled:
#target_link_libraries(${COMMON_LIB} PRIVATE imgui-sdl2 ${CMAKE_THREAD_LIBS_INIT})
target_compile_features(${COMMON_LIB} PUBLIC cxx_std_11)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${COMMON_LIB} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()
