if(APPLE)
    set(TARGET_NAME "AXR Prototype")
else()
    set(TARGET_NAME "prototype")
endif()

if(NOT APPLE)
    include_directories(cairosdl)
    
    list(APPEND axr_SOURCES
        cairosdl/cairosdl.c
        cairosdl/cairosdl.h
    )
endif()

if(WIN32)
    add_definitions(-DAXR_PLATFORM_WIN)
    
    list(APPEND axr_SOURCES main_win32.cpp axr.rc axr.ico)
elseif(APPLE)
    add_definitions(-DAXR_PLATFORM_OSX)
    
    list(APPEND axr_SOURCES
        main.m
        AXRTestAppAppDelegate.h
        AXRTestAppAppDelegate.mm
        axr.icns
        Info.plist
        English.lproj/InfoPlist.strings
        English.lproj/MainMenu.nib
    )
else()
    add_definitions(-DAXR_PLATFORM_LINUX)
    
    list(APPEND axr_SOURCES main_x11.cpp)
endif()

# HSS framework source files
file(GLOB_RECURSE hss_SOURCES ../resources/framework/*.hss)
file(GLOB_RECURSE views_SOURCES ../resources/views/*.hss ../resources/views/*.xml)

add_executable("${TARGET_NAME}" WIN32 MACOSX_BUNDLE ${axr_SOURCES} ${hss_SOURCES} ${views_SOURCES})

target_link_libraries("${TARGET_NAME}" AXR_core ${EXPAT_LIBRARIES} ${Boost_LIBRARIES} ${PANGO_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})

if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE axr.icns)
    set_source_files_properties(axr.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    
    set_source_files_properties(English.lproj/InfoPlist.strings English.lproj/MainMenu.nib PROPERTIES MACOSX_PACKAGE_LOCATION Resources/English.lproj)
    set_source_files_properties(${hss_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/framework)
    set_source_files_properties(${views_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/views)
    
    set_target_properties("${TARGET_NAME}" PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")

    find_library(COCOA_LIBRARY Cocoa)
    mark_as_advanced(COCOA_LIBRARY)
    target_link_libraries("${TARGET_NAME}" ${COCOA_LIBRARY})
else()
    target_link_libraries("${TARGET_NAME}" ${SDLMAIN_LIBRARY} ${SDL_LIBRARY})
endif()