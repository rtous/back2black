set(SEGMENTOR_SAM21 segmentor_sam21) 

add_library(${SEGMENTOR_SAM21} 
    segmentor_sam21.cpp 
    segmentor_sam21.h 
    sam21.cpp 
    sam21.h 
    tensor_utils.h 
    onnxruntime_utils.h)

target_include_directories(${SEGMENTOR_SAM21} PUBLIC 
    . 
    ${ONNXRUNTIME_INCLUDES_GLOBAL}
)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

install(TARGETS ${SEGMENTOR_SAM21} RUNTIME)

#WARNING: need to include dependencies of the dependencies
target_link_libraries(${SEGMENTOR_SAM21} PRIVATE 
    ${OpenCV_LIBS} 
    ${CMAKE_THREAD_LIBS_INIT} 
    sam #dependency of common
    segmentor 
    imgui-sdl2 #dependency of common
    common 
    #${PROTOBUF_LINK_LIBRARIES_GLOBAL}
    ${ONNXRUNTIME_LINK_LIBRARIES_GLOBAL}
    )

target_compile_features(${SEGMENTOR_SAM21} PUBLIC cxx_std_17)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${SEGMENTOR_SAM21} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()
