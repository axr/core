import qbs 1.0

Application {
    name: "axrlayouttests"
    type: "application"

    destinationDirectory: qbs.targetOS.contains("unix") ? "bin" : undefined

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: product.destinationDirectory
    }

    Depends { name: "cpp" }
    Depends { name: "axrcore" }
    Depends { name: "Qt"; submodules: [ "core", "script" ] }

    cpp.includePaths: [ product.buildDirectory ]

    files: [ "*.h", "*.cpp" ]
}
