# Icons .rc, and property lists
file(GLOB axr_icon_SOURCES ${CMAKE_SOURCE_DIR}/share/icons/*.icns)
set(axr_res_SOURCES axr.rc Info.plist ${axr_icon_SOURCES})
source_group("Resources" FILES ${axr_res_SOURCES})

# SDL application
add_executable(${TARGET_NAME_APP_SDL} WIN32 MACOSX_BUNDLE ${axr_res_SOURCES} sdlmain.cpp)
target_link_libraries(${TARGET_NAME_APP_SDL} ${TARGET_NAME_LIB_CORE} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${SDLMAIN_LIBRARY} ${SDL_LIBRARY})

# Cocoa application
if(APPLE)
    list(APPEND axr_code_SOURCES main.mm AXRTestAppAppDelegate.h AXRTestAppAppDelegate.mm)

    # Xcode nib files
    set(axr_lproj_SOURCES English.lproj/InfoPlist.strings English.lproj/MainMenu.nib)
    source_group("Resources\\English.lproj" FILES ${axr_lproj_SOURCES})

    # HSS framework source files
    file(GLOB_RECURSE hss_SOURCES ${CMAKE_SOURCE_DIR}/share/hss-framework/*.hss)
    source_group("HSS Framework" FILES ${hss_SOURCES})

    # Prototype application dogfood
    file(GLOB_RECURSE views_SOURCES ../resources/views/*.hss ../resources/views/*.xml)
    source_group("Views" FILES ${views_SOURCES})

    set(axr_SOURCES ${axr_res_SOURCES} ${axr_lproj_SOURCES} ${axr_code_SOURCES} ${hss_SOURCES} ${views_SOURCES})

    add_executable("${TARGET_NAME_APP_COCOA}" MACOSX_BUNDLE ${axr_SOURCES})
    target_link_libraries("${TARGET_NAME_APP_COCOA}" ${TARGET_NAME_LIB_COCOA})

    set(MACOSX_BUNDLE_ICON_FILE prototype.icns)

    set_source_files_properties(${axr_icon_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${axr_lproj_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/English.lproj)
    set_source_files_properties(${hss_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/framework)
    set_source_files_properties(${views_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/views)

    set_target_properties("${TARGET_NAME_APP_COCOA}" PROPERTIES MACOSX_BUNDLE_INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist")
endif()
