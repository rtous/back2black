set(EXAMPLE_TARGET test_sam)

add_executable(${EXAMPLE_TARGET}
    main.cpp
)

#OpenCV stuff
set(OpenCV_STATIC ON)
set(OpenCV_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../opencv/build")
find_package(OpenCV REQUIRED) 


install(TARGETS ${EXAMPLE_TARGET} RUNTIME)
#NOTE: If it uses the library common, then it need to add imgui-sdl2 too
target_link_libraries(${EXAMPLE_TARGET} PRIVATE sam imgui-sdl2 common ${OpenCV_LIBS} ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(${EXAMPLE_TARGET} PRIVATE imgui-sdl2 ${CMAKE_THREAD_LIBS_INIT})
target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_11)

#If Microsoft Visual C++ or similar
#if(MSVC)
#	message(STATUS "RUBEN: MSVC DETECTED")
#    target_compile_definitions(${EXAMPLE_TARGET} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
#	set_property(TARGET ${EXAMPLE_TARGET} PROPERTY
#		MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#	get_property(RUNTIME_LIB TARGET ${EXAMPLE_TARGET} PROPERTY MSVC_RUNTIME_LIBRARY)
#	message(STATUS "Runtime library: ${RUNTIME_LIB}")
#else()
#	message(STATUS "RUBEN: NO MSVC DETECTED")
#endif()
