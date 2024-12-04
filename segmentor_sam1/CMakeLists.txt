set(SAM1FACTORY_LIB segmentor_sam1)

add_library(${SAM1FACTORY_LIB} segmentor_sam1.cpp segmentor_sam1.h)

target_include_directories(${SAM1FACTORY_LIB} PUBLIC .)

install(TARGETS ${SAM1FACTORY_LIB} RUNTIME)

target_link_libraries(${SAM1FACTORY_LIB} PRIVATE 
    ${OpenCV_LIBS} 
    segmentor
    ${CMAKE_THREAD_LIBS_INIT})

target_compile_features(${SAM1FACTORY_LIB} PUBLIC cxx_std_17)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${SAM1FACTORY_LIB} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()

