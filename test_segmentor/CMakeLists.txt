set(EXAMPLE_TARGET test_segmentor)

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
    segmentor_sam2
    imgui-sdl2 #dependency of common
    ${CMAKE_THREAD_LIBS_INIT} 
    #${PROTOBUF_LINK_LIBRARIES_GLOBAL}
    ${ONNXRUNTIME_LINK_LIBRARIES_GLOBAL}
)

#target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_11)

