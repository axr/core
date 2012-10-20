if(BUILD_SHARED_LIBS)
    add_definitions(-DAXR_IMPORTS)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_BINARY_DIR})

set(prototype_SOURCES
    Info.plist
    LogWindow.cpp
    LogWindow.h
    LogWindow.ui
    main.cpp
    PreferencesDialog.cpp
    PreferencesDialog.h
    PreferencesDialog.ui
    PrototypeApplication.cpp
    PrototypeApplication.h
    PrototypeSettings.cpp
    PrototypeSettings.h
    PrototypeWindow.cpp
    PrototypeWindow.h
    PrototypeWindow.ui
    Resources.qrc
    resources.rc

    # These need to be included so they can be copied to the bundle
    ${axr_icon_SOURCES}
    ${hss_SOURCES}
)

source_group("Resources\\HSS Framework" FILES ${hss_SOURCES})

qt_transform_sources(prototype_SOURCES "${prototype_SOURCES}")

add_executable(${TARGET_NAME_APP_PROTOTYPE} WIN32 MACOSX_BUNDLE ${prototype_SOURCES})
target_link_libraries(${TARGET_NAME_APP_PROTOTYPE} ${TARGET_NAME_LIB_CORE} ${TARGET_NAME_LIB_QT} ${QT_QTMAIN_LIBRARY})

# TODO: Temporary until TEST target is removed
target_link_libraries(${TARGET_NAME_APP_PROTOTYPE} TEST)

set(MACOSX_BUNDLE_ICON_FILE prototype.icns)

set_source_files_properties(${axr_icon_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(${hss_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/framework)

set_target_properties(${TARGET_NAME_APP_PROTOTYPE} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)

if(IS_RELEASE_BUILD)
    install(TARGETS ${TARGET_NAME_APP_PROTOTYPE}
        BUNDLE DESTINATION . COMPONENT prototype
        RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL} COMPONENT prototype
    )

    if(WIN32 OR APPLE)
        install_qt4_executable("${TARGET_NAME_APP_PROTOTYPE}${CMAKE_USER_EXECUTABLE_SUFFIX}" "qtaccessiblewidgets qcncodecs qjpcodecs qkrcodecs qtwcodecs qsvgicon qgif qico qjpeg qmng qsvg qtiff" "" "${ALL_LIBRARY_DIRS}" "" "" prototype)
        empty_lproj(${TARGET_NAME_APP_PROTOTYPE})
    endif()
endif()
