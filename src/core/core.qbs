import qbs 1.0

Product {
    name: "axrcore"
    type: "dynamiclibrary"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: [ "core", "gui" ] }

    // TODO: how can we do this in a less verbose manner?
    files: [
        "*",
        "axr/*",
        "debug/*",
        "hss/*",
        "hss/enums/*",
        "hss/input/*",
        "hss/objects/*",
        "hss/objects/actions/*",
        "hss/objects/borders/*",
        "hss/objects/gradients/*",
        "hss/objects/shapes/*",
        "hss/parsing/*",
        "hss/parsing/constants/*",
        "hss/parsing/expressions/*",
        "hss/parsing/filters/*",
        "hss/parsing/functions/*",
        "hss/parsing/selectors/*",
        "hss/parsing/statements/*",
        "hss/processing/*",
        "hss/rendering/*",
        "hss/tokenizing/*",
        "hss/various/*",
        "logging/*",
        "types/*",
        "xml/*"
    ]

    Properties {
        condition: type === "dynamiclibrary"
        cpp.defines: [ "AXR_EXPORTS" ]
    }

    Properties {
        condition: qbs.targetOS === "mac"
        cpp.frameworks: [ "Foundation" ]
    }

    cpp.includePaths: [
        ".",
        "axr",
        "debug",
        "hss",
        "hss/enums",
        "hss/input",
        "hss/objects",
        "hss/objects/actions",
        "hss/objects/borders",
        "hss/objects/gradients",
        "hss/objects/shapes",
        "hss/parsing",
        "hss/parsing/constants",
        "hss/parsing/expressions",
        "hss/parsing/filters",
        "hss/parsing/functions",
        "hss/parsing/selectors",
        "hss/parsing/statements",
        "hss/processing",
        "hss/rendering",
        "hss/tokenizing",
        "hss/various",
        "logging",
        "types",
        "xml",
        product.buildDirectory
    ]

    // TODO: can we avoid duplicating this?
    ProductModule {
        Depends { name: "cpp" }
        cpp.includePaths: [
            ".",
            "axr",
            "debug",
            "hss",
            "hss/enums",
            "hss/input",
            "hss/objects",
            "hss/objects/actions",
            "hss/objects/borders",
            "hss/objects/gradients",
            "hss/objects/shapes",
            "hss/parsing",
            "hss/parsing/constants",
            "hss/parsing/expressions",
            "hss/parsing/filters",
            "hss/parsing/functions",
            "hss/parsing/selectors",
            "hss/parsing/statements",
            "hss/processing",
            "hss/rendering",
            "hss/tokenizing",
            "hss/various",
            "logging",
            "types",
            "xml"
        ]
    }
}
