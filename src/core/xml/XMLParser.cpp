/********************************************************************
 *             a  A
 *            AM\/MA
 *           (MA:MMD
 *            :: VD
 *           ::  º
 *          ::
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM
 *      TMMM MMMM
 *     TMMM  .MMM
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING
 *     TMM    MMA       ====================================
 *     TMN    MM
 *      MN    ZM
 *            MM,
 *
 *
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include <QtCore>
#include "AXRBuffer.h"
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRError.h"
#include "AXRLoggerManager.h"
#include "XMLParser.h"

using namespace AXR;

namespace AXR
{
    class XMLParserPrivate
    {
        friend class XMLParser;

        XMLParserPrivate()
        : controller()
        {
        }

        AXRController *controller;
    };
}

XMLParser::XMLParser(AXRController *controller)
: d(new XMLParserPrivate)
{
    axr_log(LoggerChannelGeneralSpecific, "XMLParser: creating XML parser");
    d->controller = controller;
}

XMLParser::~XMLParser()
{
    axr_log(LoggerChannelGeneralSpecific, "XMLParser: destroying XML parser");
    delete d;
}

bool XMLParser::loadFile(QSharedPointer<AXRBuffer> file)
{
    if (!d->controller)
    {
        throw AXRError("XMLParser", "The controller was not set on the XML parser");
    }

    axr_log(LoggerChannelOverview, "XMLParser: loading file " + file->sourceUrl().toString());

    if (!file->isValid())
    {
        axr_log(LoggerChannelOverview, "XMLParser: failed to load file " + file->sourceUrl().toString());
        return false;
    }

    try
    {
        // Parse the XML file...
        QXmlStreamReader xml(file->getBuffer());
        while (!xml.atEnd() && !xml.hasError())
        {
            xml.readNext();
            if (xml.isStartElement())
            {
                AXRString name = xml.name().toString();
                axr_log(LoggerChannelXMLParser, "XMLParser: found opening tag with name " + name);
                d->controller->enterElement(name);

                Q_FOREACH (const QXmlStreamAttribute &attr, xml.attributes())
                {
                    d->controller->addAttribute(attr.name().toString(), attr.value().toString());
                }
            }
            else if (xml.isEndElement())
            {
                axr_log(LoggerChannelXMLParser, "XMLParser: found closing tag with name " + xml.name().toString());
                d->controller->exitElement();
            }
            else if (xml.isCharacters())
            {
                axr_log(LoggerChannelXMLParser, AXRString("XMLParser: reading character data: \"%1\"").arg(xml.text().toString()));
                d->controller->appendContentText(xml.text().toString());
            }
            else if (xml.isProcessingInstruction())
            {
                AXRString instructionName = xml.processingInstructionTarget().toString();
                axr_log(LoggerChannelXMLParser, "XMLParser: found XML instruction with name " + instructionName);

                // Probable HSS stylesheet encountered, try to load it into the controller
                if (instructionName == "xml-stylesheet")
                {
                    QMap<AXRString, AXRString> instructionAttributes;

                    // A little trickery to easily parse the key-value pairs from the instruction...
                    // We basically embed the processing instruction value, which looks like a set of XML
                    // attributes, within a fake XML tag to create a document that we can parse, allowing
                    // the XML parser to handle the syntax instead of doing it manually
                    QXmlStreamReader instructionParser(AXRString("<node %1 />").arg(xml.processingInstructionData().toString()));
                    while (!instructionParser.atEnd() && !instructionParser.hasError())
                    {
                        instructionParser.readNext();
                        if (instructionParser.isStartElement())
                        {
                            // Read all the attributes from our fake tag, which are the xml-stylesheet parameters
                            Q_FOREACH (const QXmlStreamAttribute &attr, instructionParser.attributes())
                            {
                                instructionAttributes.insert(attr.name().toString(), attr.value().toString());
                            }
                        }
                    }

                    // The xml-stylesheet instruction was invalid, error out
                    if (instructionParser.hasError())
                    {
                        throw AXRError("XMLParser", "Malformed processing instruction - " + instructionParser.errorString(), file->sourceUrl(), xml.lineNumber());
                    }

                    // href is required for any xml-stylesheet instruction according to W3C
                    if (!instructionAttributes.contains("href"))
                    {
                        throw AXRError("XMLParser", AXRString("Invalid xml-stylesheet instruction - missing href attribute"), file->sourceUrl(), xml.lineNumber());
                    }

                    // Verify the stylesheet has the correct MIME type for HSS
                    const AXRString &sheetType = instructionAttributes.value("type");
                    if (sheetType == "application/x-hss" || sheetType == "text/hss")
                    {
                        // We've got the right MIME type, now do we even have a valid URL?
                        const QUrl sheetUrl(instructionAttributes.value("href"));
                        if (!sheetUrl.isValid())
                        {
                            throw AXRError("XMLParser", "Invalid xml-stylesheet instruction - malformed URL in href attribute - " + sheetUrl.errorString(), file->sourceUrl(), xml.lineNumber());
                        }

                        // Do we have a valid version?
                        // TODO: This should be a QVersion *type*; if (!sheetVersion.isValid()) ...
                        // I have an implementation lying around somewhere...
                        const AXRString &sheetVersion = instructionAttributes.value("version");
                        if (sheetVersion.isEmpty())
                        {
                            AXRString message = instructionAttributes.contains("version")
                                ? "malformed version attribute - " + sheetVersion
                                : "missing version attribute";

                            throw AXRError("XMLParser", AXRString("Invalid xml-stylesheet instruction - %1").arg(message), file->sourceUrl(), xml.lineNumber());
                        }

                        if (sheetUrl.isRelative())
                            d->controller->addStyleSheetUrl(file->sourceUrl().resolved(sheetUrl));
                        else
                            d->controller->addStyleSheetUrl(sheetUrl);
                    }
                    else
                    {
                        axr_log(LoggerChannelXMLParser, "Ignoring stylesheet of type: " + sheetType + " in file " + file->sourceUrl().toString());
                    }
                }
            }
        }

        if (xml.hasError())
        {
            throw AXRError("XMLParser", "XML error - " + xml.errorString(), file->sourceUrl(), xml.lineNumber(), xml.columnNumber());
        }

        return true;
    }
    catch (const AXRError &e)
    {
        e.raise();
        return false;
    }
}
