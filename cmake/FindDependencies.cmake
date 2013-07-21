if(AXR_BUILD_CORE)
    ### Qt ###
    find_package(Qt4 4.8 COMPONENTS QtCore QtGui REQUIRED)
    find_package(Qt4 4.8 COMPONENTS QtScript)
    if(QT_QTCORE_FOUND AND QT_QTGUI_FOUND)
        set(QT4_FOUND TRUE) # Optional means optional.
    endif()
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS} -DNOMINMAX)

    ### DPKG ###
    find_program(DPKG NAMES "dpkg-deb" PATHS "/usr/bin")
    if(DPKG)
        get_filename_component(DPKG_PATH ${DPKG} ABSOLUTE)
        message(STATUS "Found dpkg-deb: ${DPKG_PATH}")
        set(DPKG_FOUND TRUE)
    else()
        message(STATUS "Could not find dpkg-deb; will not build Debian packages")
        set(DPKG_FOUND FALSE)
    endif()

    ### RPMBUILD ###
    find_program(RPMBUILD NAMES "rpmbuild" PATHS "/usr/bin")
    if(RPMBUILD)
        get_filename_component(RPMBUILD_PATH ${RPMBUILD} ABSOLUTE)
        message(STATUS "Found rpmbuild: ${RPMBUILD_PATH}")
        set(RPMBUILD_FOUND TRUE)
    else()
        message(STATUS "Could not find rpmbuild; will not build RPM packages")
        set(RPMBUILD_FOUND FALSE)
    endif()

    ### pandoc ###
    find_program(PANDOC NAMES "pandoc.exe" "pandoc" PATHS "C:\\Program Files (x86)\\Pandoc\\bin" "/usr/local/bin" "/usr/bin")
    if(PANDOC)
        message(STATUS "Found pandoc: ${PANDOC}")
    else()
        message(STATUS "Could not find pandoc; will not build distribution packages")
    endif()
endif()

if(AXR_BUILD_DOCS)
    find_package(Doxygen REQUIRED)
endif()
