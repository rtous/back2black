set(EXAMPLE_TARGET cli)

add_executable(${EXAMPLE_TARGET}
    main.cpp
)

install(TARGETS ${EXAMPLE_TARGET} RUNTIME)
target_link_libraries(${EXAMPLE_TARGET} PRIVATE sam ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(${EXAMPLE_TARGET} PRIVATE imgui-sdl2 ${CMAKE_THREAD_LIBS_INIT})
target_compile_features(${EXAMPLE_TARGET} PUBLIC cxx_std_11)

#If Microsoft Visual C++ or similar
if(MSVC)
    target_compile_definitions(${EXAMPLE_TARGET} PRIVATE -D_CRT_SECURE_NO_WARNINGS=1)
endif()
