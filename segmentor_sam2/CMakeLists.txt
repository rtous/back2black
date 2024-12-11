set(SEGMENTOR_SAM2 segmentor_sam2) 

#add_library(${SEGMENTOR_SAM2} segmentor_sam2.cpp segmentor_sam2.h sam2.cpp sam2.h)
add_library(${SEGMENTOR_SAM2} segmentor_sam2.cpp segmentor_sam2.h sam2.cpp sam2.h)

target_include_directories(${SEGMENTOR_SAM2} PUBLIC 
    . 
    ${ONNXRUNTIME_INCLUDES_GLOBAL}
)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

install(TARGETS ${SEGMENTOR_SAM2} RUNTIME)

#WARNING: need to include dependencies of the dependencies
target_link_libraries(${SEGMENTOR_SAM2} PRIVATE 
    ${OpenCV_LIBS} 
    ${CMAKE_THREAD_LIBS_INIT} 
    sam #dependency of common
    segmentor 
    imgui-sdl2 #dependency of common
    common 
    #${PROTOBUF_LINK_LIBRARIES_GLOBAL}
    ${ONNXRUNTIME_LINK_LIBRARIES_GLOBAL}
    )

target_compile_features(${SEGMENTOR_SAM2} PUBLIC cxx_std_17)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${SEGMENTOR_SAM2} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()
