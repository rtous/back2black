set(EXAMPLE_TARGET cli_simplify)

add_executable(${EXAMPLE_TARGET}
    main.cpp
)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 

install(TARGETS ${EXAMPLE_TARGET} RUNTIME)
target_link_libraries(${EXAMPLE_TARGET} PRIVATE sam ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(${EXAMPLE_TARGET} PRIVATE imgui-sdl2 ${CMAKE_THREAD_LIBS_INIT})
target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_11)
#target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_23)


#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${EXAMPLE_TARGET} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()
