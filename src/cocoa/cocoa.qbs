import qbs 1.0

Product {
    name: "axrcocoa"
    type: "staticlibrary"
    condition: qbs.targetOS.contains("osx")

    destinationDirectory: "lib"

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: product.destinationDirectory
    }

    Depends { name: "cpp" }
    Depends { name: "axrcore" }
    Depends { name: "config_header" }
    Depends { name: "Qt"; submodules: [ "core", "gui" ] }

    files: [ "*.h", "*.mm" ]

    cpp.frameworks: [ "Cocoa" ]

    Export {
        Depends { name: "cpp" }
        cpp.includePaths: "."
    }
}
