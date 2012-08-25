# Cocoa application
if(APPLE)
    set(TARGET_NAME "AXR Prototype")

    list(APPEND axr_SOURCES
        main.m
        AXRTestAppAppDelegate.h
        AXRTestAppAppDelegate.mm
        axr.icns
        Info.plist
        English.lproj/InfoPlist.strings
        English.lproj/MainMenu.nib
    )

    # HSS framework source files
    file(GLOB_RECURSE hss_SOURCES ../resources/framework/*.hss)
    file(GLOB_RECURSE views_SOURCES ../resources/views/*.hss ../resources/views/*.xml)

    add_executable("${TARGET_NAME}" MACOSX_BUNDLE ${axr_SOURCES} ${hss_SOURCES} ${views_SOURCES})

    target_link_libraries("${TARGET_NAME}" AXR_core)

    set(MACOSX_BUNDLE_ICON_FILE axr.icns)
    set_source_files_properties(axr.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    set_source_files_properties(English.lproj/InfoPlist.strings English.lproj/MainMenu.nib PROPERTIES MACOSX_PACKAGE_LOCATION Resources/English.lproj)
    set_source_files_properties(${hss_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/framework)
    set_source_files_properties(${views_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/views)

    set_target_properties("${TARGET_NAME}" PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
endif()

# SDL application
include_directories(cairosdl)
add_executable("AXR SDL" WIN32 axr.ico axr.rc cairosdl/cairosdl.c cairosdl/cairosdl.h sdlmain.cpp)
target_link_libraries("AXR SDL" AXR_core ${SDLMAIN_LIBRARY} ${SDL_LIBRARY})
