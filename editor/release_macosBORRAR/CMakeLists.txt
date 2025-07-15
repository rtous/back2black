set(TARGET myMacOSBundle)

add_executable(${TARGET}
    colors.cpp
    gui_utils.cpp
    main.cpp
    editor.cpp
    file_dialog.cpp
    sam_utils.cpp
    video.cpp
)

set(CPACK_BUNDLE_NAME ${TARGET})
set(CPACK_GENERATOR "DragNDrop")

# add_subdirectory(release_macos)
# MacOS bundle
if(APPLE)
    # Bundling macOS application
    set(CPACK_BUNDLE_NAME ${TARGET})
    set(CPACK_GENERATOR "DragNDrop")
    set_target_properties(${TARGET} PROPERTIES
        BUNDLE True
        MACOSX_BUNDLE_BUNDLE_NAME ${TARGET}
        MACOSX_BUNDLE_BUNDLE_VERSION ${CMAKE_PROJECT_VERSION}
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.${TARGET}
        MACOSX_BUNDLE_ICON_FILE AppIcon
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${CMAKE_PROJECT_VERSION}
        #MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/cmake/customtemplate.plist.in
    )
endif()