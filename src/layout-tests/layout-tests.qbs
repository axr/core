import qbs 1.0

Project {
    Application {
        name: "axrlayouttests"
        type: "application"

        Depends { name: "cpp" }
        Depends { name: "axrcore" }
        Depends { name: "Qt"; submodules: [ "core", "script" ] }

        cpp.includePaths: [ product.buildDirectory ]

        files: [ "*.h", "*.cpp" ]
    }
}
