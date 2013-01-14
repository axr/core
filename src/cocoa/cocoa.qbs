import qbs.base 1.0

Product {
    name: "axrcocoa"
    type: "staticlibrary"
    condition: qbs.targetOS === "mac"

    Depends { name: "cpp" }
    Depends { name: "axrcore" }
    Depends { name: "config_header" }
    Depends { name: "Qt"; submodules: [ "core", "gui" ] }

    files: [
        "NSAXRView.h",
        "NSAXRView.mm"
    ]

    Properties {
        condition: qbs.targetOS === "mac"
        cpp.frameworks: [ "Cocoa" ]
    }
}
