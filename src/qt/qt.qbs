import qbs.base 1.0

Product {
    name: "axrqt"
    type: "staticlibrary"

    Depends { name: "cpp" }
    Depends { name: "axrcore" }
    Depends { name: "config_header" }
    Depends { name: "Qt"; submodules: [ "core", "gui", "widgets" ] }

    files: [
        "QAXRWidget.h",
        "QAXRWidget.cpp"
    ]
}
