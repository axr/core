import qbs 1.0

Project {
    references: [
        "src/cocoa/cocoa.qbs",
        "src/core/core.qbs",
        "src/qt/qt.qbs"
    ]

    property int axr_version_major: 0
    property int axr_version_minor: 4
    property int axr_version_patch: 8
    property int axr_version_build: 0
    property string axr_version_string: "0.4.8"
    property string axr_version_string_real: axr_version_major + "." + axr_version_minor + "." + axr_version_patch + "." + axr_version_build

    property string axr_vendor: "AXR Project Contributors"
    property string axr_domain: "axrproject.org"
    property string axr_contact: "info@axrproject.org"
    property string axr_web_url: "http://" + axr_domain + "/"

    Product {
        name: "config_header"
        type: "hpp"
        files: "config.in.h"

        property int axr_version_major: project.axr_version_major
        property int axr_version_minor: project.axr_version_minor
        property int axr_version_patch: project.axr_version_patch
        property int axr_version_build: project.axr_version_build
        property string axr_version_string: project.axr_version_string
        property string axr_version_string_real: project.axr_version_string_real

        property string axr_vendor: project.axr_vendor
        property string axr_domain: project.axr_domain
        property string axr_contact: project.axr_contact
        property string axr_web_url: project.axr_web_url

        Transformer {
            inputs: [ "config.in.h" ]

            Artifact {
                fileName: "axr-config.h"
                fileTags: "hpp"
            }

            prepare: {
                var cmd = new JavaScriptCommand();
                cmd.description = "generating axr-config.h";
                cmd.highlight = "codegen";
                cmd.sourceCode = function() {
                    var file = new TextFile(input.fileName);
                    var content = file.readAll();

                    content = content.replace("@AXR_VERSION_MAJOR@", product.axr_version_major);
                    content = content.replace("@AXR_VERSION_MINOR@", product.axr_version_minor);
                    content = content.replace("@AXR_VERSION_PATCH@", product.axr_version_patch);
                    content = content.replace("@AXR_VERSION_BUILD@", product.axr_version_build);
                    content = content.replace("@AXR_VERSION_STRING@", product.axr_version_string);

                    content = content.replace("@AXR_VENDOR@", product.axr_vendor);
                    content = content.replace("@AXR_DOMAIN@", product.axr_domain);
                    content = content.replace("@AXR_CONTACT@", product.axr_contact);

                    file = new TextFile(output.fileName, TextFile.WriteOnly);
                    file.truncate();
                    file.write(content);
                    file.close();
                }

                return cmd;
            }
        }

        Export {
            Depends { name: "cpp" }
            cpp.includePaths: product.buildDirectory
        }
    }
}
