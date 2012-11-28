# Performs common Linux packaging functionality:
# - Installs Debian copyright file from common
# - Generates and installs Debian changelog
# - Generates RPM changelog that can be read from $CMAKE_BINARY_DIR/rpm/changelog

if(DPKG_FOUND OR RPMBUILD_FOUND)
    # Temporary directories for intermediary files
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/deb")
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/rpm")

    # Generate Debian and RPM changelogs from our markdown formatted changelog
    find_package(PythonInterp REQUIRED)
    execute_process(COMMAND ${PYTHON_EXECUTABLE} common/changelog.py --deb "${CMAKE_BINARY_DIR}/deb/changelog" --rpm "${CMAKE_BINARY_DIR}/rpm/changelog" "${BROWSER_PACKAGE_PREFIX}" CHANGELOG.md WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
endif()

if(DPKG_FOUND)
    # Configure and install Debian copyright file
    execute_process(COMMAND date -R OUTPUT_VARIABLE DATE_R OUTPUT_STRIP_TRAILING_WHITESPACE)
    configure_file("${CMAKE_SOURCE_DIR}/common/debian-copyright.txt" "${CMAKE_BINARY_DIR}/copyright")

    # Configure and install changelog file
    file(REMOVE "${CMAKE_BINARY_DIR}/deb/changelog.gz")
    execute_process(COMMAND gzip -9 changelog WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/deb")
endif()
