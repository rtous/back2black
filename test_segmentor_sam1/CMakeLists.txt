set(EXAMPLE_TARGET test_segmentor_sam1)

add_executable(${EXAMPLE_TARGET}
    main.cpp
)

#opencv
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

#libraries
target_link_libraries(${EXAMPLE_TARGET} PRIVATE 
    ${OpenCV_LIBS} 
    common
    sam
    segmentor
    segmentor_sam1
    imgui-sdl2
    ${CMAKE_THREAD_LIBS_INIT} 
)

#target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_11)

