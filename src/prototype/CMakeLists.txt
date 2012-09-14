set(axr_res_SOURCES Info.plist)
source_group("Resources" FILES ${axr_res_SOURCES})

file(GLOB_RECURSE prototype_SOURCES *.h *.cpp *.ui)

qt4_wrap_ui(uic_stuff PrototypeWindow.ui LogWindow.ui)
qt4_wrap_cpp(moc_stuff PrototypeWindow.h LogWindow.h)

list(APPEND prototype_SOURCES ${axr_icon_SOURCES} ${hss_SOURCES} ${axr_res_SOURCES} ${uic_stuff} ${moc_stuff})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(${TARGET_NAME_APP_PROTOTYPE} WIN32 MACOSX_BUNDLE ${prototype_SOURCES})
target_link_libraries(${TARGET_NAME_APP_PROTOTYPE} ${TARGET_NAME_LIB_CORE})

set(MACOSX_BUNDLE_ICON_FILE prototype.icns)

set_source_files_properties(${axr_icon_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(${hss_SOURCES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources/framework)

set_target_properties(${TARGET_NAME_APP_PROTOTYPE} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
