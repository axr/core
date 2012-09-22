find_package(SDL)
if(SDL_FOUND)
    if(BUILD_SHARED_LIBS)
        add_definitions(-DAXR_IMPORTS)
    endif()

    include_directories(${SDL_INCLUDE_DIR})

    set(sdl_SOURCES
        axr.rc
        Info.plist
        main.cpp

        # These need to be included so they can be copied to the bundle
        ${axr_icon_SOURCES}
        ${hss_SOURCES}
    )

    source_group("Resources\\HSS Framework" FILES ${hss_SOURCES})

    qt_transform_sources(sdl_SOURCES "${sdl_SOURCES}")

    # SDL application
    add_executable(${TARGET_NAME_APP_SDL} WIN32 MACOSX_BUNDLE ${axr_res_SOURCES} main.cpp ${axr_icon_SOURCES} ${hss_SOURCES})
    target_link_libraries(${TARGET_NAME_APP_SDL} ${TARGET_NAME_LIB_CORE} ${Boost_PROGRAM_OPTIONS_LIBRARY} ${SDLMAIN_LIBRARY} ${SDL_LIBRARY})

    set(MACOSX_BUNDLE_ICON_FILE prototype.icns)

    set_source_files_properties(${axr_icon_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${hss_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/framework)

    set_target_properties(${TARGET_NAME_APP_SDL} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)

    install_bundle(TARGET ${TARGET_NAME_APP_SDL}
        COMPONENT examples
        BUNDLE_DESTINATION Examples
        RUNTIME_DESTINATION bin/examples
    )
else()
    message(STATUS "SDL was NOT found; will not build SDL example")
endif()
