set(API lester-api) 

add_library(${API} api.cpp api.h)

target_include_directories(${API} PUBLIC .)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

install(TARGETS ${API} RUNTIME)
target_link_libraries(${API} PRIVATE ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT} common sam imgui-sdl2)

target_compile_features(${API} PUBLIC cxx_std_17)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${API} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()

