import qbs 1.0

Product {
    name: "axrcore"
    type: {
        if (qbs.targetOS.contains("osx"))
            return "frameworkbundle"
        else if (qbs.targetOS.contains("ios"))
            return "staticlibrary"
        else
            return "dynamiclibrary"
    }

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: [ "core", "gui" ] }

    targetName: type.contains("frameworkbundle") ? "AXRCore" : name

    destinationDirectory: {
        if (type.contains("frameworkbundle"))
            return "Library/Frameworks"
        else if (qbs.targetOS.contains("unix"))
            return "lib"
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: product.destinationDirectory
    }

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
        "types/*.cpp",
        "xml/*"
    ]

    Group {
        condition: qbs.targetOS.contains("darwin")
        files: [ "types/*.mm" ]
    }

    Properties {
        condition: type.contains("dynamiclibrary")
        cpp.defines: [ "AXR_EXPORTS" ]
    }

    Properties {
        condition: qbs.targetOS.contains("darwin")
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
    Export {
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
