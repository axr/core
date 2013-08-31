import qbs 1.0

Product {
    name: "axrqt"
    type: "staticlibrary"

    destinationDirectory: qbs.targetOS.contains("unix") ? "lib" : undefined

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: product.destinationDirectory
    }

    Depends { name: "cpp" }
    Depends { name: "axrcore" }
    Depends { name: "config_header" }
    Depends { name: "Qt"; submodules: [ "core", "gui", "widgets" ] }

    files: [
        "QAXRWidget.h",
        "QAXRWidget.cpp"
    ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: "."
    }
}
