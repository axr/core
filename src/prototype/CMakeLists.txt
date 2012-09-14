include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(prototype_SOURCES
    AXRWidget.cpp
    AXRWidget.h
    Info.plist
    LogWindow.cpp
    LogWindow.h
    LogWindow.ui
    main.cpp
    PrototypeWindow.cpp
    PrototypeWindow.h
    PrototypeWindow.ui

    # These need to be included so they can be copied to the bundle
    ${axr_icon_SOURCES}
    ${hss_SOURCES}
)

source_group("Resources\\HSS Framework" FILES ${hss_SOURCES})

qt_transform_sources(prototype_SOURCES "${prototype_SOURCES}")

add_executable(${TARGET_NAME_APP_PROTOTYPE} WIN32 MACOSX_BUNDLE ${prototype_SOURCES})
target_link_libraries(${TARGET_NAME_APP_PROTOTYPE} ${TARGET_NAME_LIB_CORE})

set(MACOSX_BUNDLE_ICON_FILE prototype.icns)

set_source_files_properties(${axr_icon_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(${hss_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/framework)

set_target_properties(${TARGET_NAME_APP_PROTOTYPE} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
