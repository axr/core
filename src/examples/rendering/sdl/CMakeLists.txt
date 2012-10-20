find_package(SDL)
if(SDL_FOUND)
    if(BUILD_SHARED_LIBS)
        add_definitions(-DAXR_IMPORTS)
    endif()

    include_directories(${SDL_INCLUDE_DIR})

    set(sdl_SOURCES
        Info.plist
        main.cpp
        resources.rc

        # These need to be included so they can be copied to the bundle
        ${axr_icon_SOURCES}
        ${hss_SOURCES}
    )

    source_group("Resources\\HSS Framework" FILES ${hss_SOURCES})

    qt_transform_sources(sdl_SOURCES "${sdl_SOURCES}")

    # SDL application
    add_executable(${TARGET_NAME_APP_SDL} WIN32 MACOSX_BUNDLE ${axr_res_SOURCES} main.cpp ${axr_icon_SOURCES} ${hss_SOURCES})
    target_link_libraries(${TARGET_NAME_APP_SDL} ${TARGET_NAME_LIB_CORE} ${SDLMAIN_LIBRARY} ${SDL_LIBRARY})

    set(MACOSX_BUNDLE_ICON_FILE prototype.icns)

    set_source_files_properties(${axr_icon_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set_source_files_properties(${hss_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/framework)

    if(APPLE)
        set(TARGET_NAME_APP_SDL_OUTPUT_NAME "SDL Example")
    else()
        set(TARGET_NAME_APP_SDL_OUTPUT_NAME sdlexample)
    endif()

    set_target_properties(${TARGET_NAME_APP_SDL} PROPERTIES
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist
        OUTPUT_NAME "${TARGET_NAME_APP_SDL_OUTPUT_NAME}"
        FOLDER Examples
    )

    if(IS_RELEASE_BUILD)
        install(TARGETS ${TARGET_NAME_APP_SDL}
            BUNDLE DESTINATION Examples COMPONENT examples
            RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL}/examples COMPONENT examples
        )

        if(APPLE)
            set(app_path Examples)
        else()
            set(app_path ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL}/examples)
        endif()

        if(WIN32 OR APPLE)
            install_qt4_executable("${app_path}/${TARGET_NAME_APP_SDL_OUTPUT_NAME}${CMAKE_USER_EXECUTABLE_SUFFIX}" "qtaccessiblewidgets qcncodecs qjpcodecs qkrcodecs qtwcodecs qsvgicon qgif qico qjpeg qmng qsvg qtiff" "" "${ALL_LIBRARY_DIRS}" "" "" examples)
            empty_lproj(${app_path}/${TARGET_NAME_APP_SDL_OUTPUT_NAME})
        endif()
    endif()
else()
    message(STATUS "SDL was NOT found; will not build SDL example")
endif()
