import qbs.base 1.0

Product {
    name: "axrcore"
    type: "dynamiclibrary"

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: [ "core", "gui", "xml" ] }

    // TODO: how can we do this in a less verbose manner?
    files: [
        "*",
        "axr/*",
        "debug/*",
        "hss/*",
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
        "hss/tokenizing/*",
        "hss/various/*",
        "rendering/*",
        "types/*",
        "xml/*"
    ]

    Properties {
        condition: product.type === "dynamiclibrary"
        cpp.defines: [ "AXR_EXPORTS" ]
    }

    cpp.includePaths: [
        ".",
        "axr",
        "debug",
        "hss",
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
        "hss/tokenizing",
        "hss/various",
        "rendering",
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
            "hss/tokenizing",
            "hss/various",
            "rendering",
            "types",
            "xml"
        ]
    }
}
