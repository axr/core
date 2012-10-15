if(AXR_BUILD_PROTOTYPE)
    ### Qt ###
    find_package(Qt4 4.8 COMPONENTS QtCore QtGui QtXml REQUIRED)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS} -DNOMINMAX)

    ### Boost ###
    # Don't use static Boost because this will result in relocation errors due to
    # Boost not being compiled with -fPIC (at least on 64-bit Linux)
    if(MSVC)
        set(Boost_USE_STATIC_LIBS ON)
        link_directories(${Boost_LIBRARY_DIRS})
    else()
        set(Boost_USE_STATIC_LIBS OFF)
    endif()
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
    find_package(Boost COMPONENTS thread system REQUIRED)
    include_directories(${Boost_INCLUDE_DIRS})

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
endif()
