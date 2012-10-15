<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output encoding="utf-8" method="html" doctype-public="html" omit-xml-declaration="yes" indent="yes"/>
    <xsl:template match="/">
        <html>
        <head>
            <title></title>
            <style type="text/css">
            html, body, object
            {
                margin: 0;
                padding: 0;
                width: 100%;
                height: 100%;
                overflow: hidden;
            }

            .overlay
            {
                position: absolute;
                left: 0;
                top: 0;
                right: 0;
                bottom: 0;
            }

            #axr-error, #axr-loading
            {
                visibility: hidden;
            }
            </style>
        </head>
        <body>
            <object id="axr-plugin" type="application/x-axr">
                <param name="onload" value="axrloaded" />
            </object>
            <div id="axr-loading" class="overlay">
                Loading AXR plugin...
            </div>
            <div id="axr-error" class="overlay">
                Failed to load AXR plugin!
            </div>
            <script type="text/javascript">
            //<![CDATA[
            // Initial delay (ms) before showing a "loading..." indicator
            var pluginTroubleGrace = 1000;

            // Delay (ms) before showing error screen
            var pluginFailGrace = 3000;

            var axr = document.getElementById('axr-plugin');

            var troubleTimer = setTimeout(function () {
                // AXR is valid but takes a lot of time to start
                if (axr.valid) {
                    document.getElementById('axr-loading').style.visibility = 'visible';
                } else {
                    document.getElementById('axr-error').style.visibility = 'visible';
                }
            }, pluginTroubleGrace);

            var failTimer = setTimeout(function () {
                document.getElementById('axr-error').style.visibility = 'visible';
                document.getElementById('axr-loading').style.visibility = 'hidden';
            }, pluginFailGrace);

            window.axrloaded = function () {
                // AXR seems to have loaded successfully
                clearTimeout(troubleTimer);
                clearTimeout(failTimer);
                document.getElementById('axr-error').style.visibility = 'hidden';
                document.getElementById('axr-loading').style.visibility = 'hidden';
            };

            var $bind = window.attach ?
                function(type, callback) { this.attach('on' + type, callback); } :
                function(type, callback) { this.addEventListener(type, callback, false); };

            $bind.call(axr, 'titleChanged', function (e) {
                document.title = e.title;
            });

            $bind.call(axr, 'log', function (e) {
                if (console) {
                    console[e.level](e.data);
                }
            });
            //]]>
            </script>
        </body>
        </html>
    </xsl:template>
</xsl:stylesheet>
