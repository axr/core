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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include <QtCore>
#include <QtXml>
#include "errors.h"
#include "AXR.h"
#include "AXRController.h"
#include "AXRDebugging.h"
#include "XMLParser.h"

using namespace AXR;

XMLParser::XMLParser(AXRController *theController)
: controller(theController)
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "XMLParser: creating XML parser");
}

XMLParser::~XMLParser()
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "XMLParser: destroying XML parser");
}

bool XMLParser::loadFile(AXRFile::p file)
{
    if (!controller)
    {
        throw AXRError::p(new AXRError("XMLParser", "The controller was not set on the XML parser"));
    }

    axr_log(AXR_DEBUG_CH_OVERVIEW, "XMLParser: loading file " + file->getFileName());
    axr_log(AXR_DEBUG_CH_FULL_FILENAMES, file->getBasePath() + "/" + file->getFileName());

    this->file = file;
    if (!file->getFileHandle())
    {
        return false;
    }

    try
    {
        // Try to open the file from which to read the XML file
        // TODO: Replace or augment AXRFile with the QIODevice infrastructure
        if (!AXRCore::getInstance()->getWrapper()->readFile(file))
        {
            axr_log(AXR_DEBUG_CH_OVERVIEW, "XMLParser: failed to load file " + file->getFileName());
            return false;
        }

        // Parse the XML file...
        QXmlStreamReader xml(file->getBuffer());
        while (!xml.atEnd() && !xml.hasError())
        {
            xml.readNext();
            if (xml.isStartElement())
            {
                QString name = xml.name().toString();
                axr_log(AXR_DEBUG_CH_XML, "XMLParser: found opening tag with name " + name);
                controller->enterElement(name.toStdString());

                Q_FOREACH (const QXmlStreamAttribute &attr, xml.attributes())
                {
                    controller->addAttribute(attr.name().toString().toStdString(), attr.value().toString().toStdString());
                }
            }
            else if (xml.isEndElement())
            {
                axr_log(AXR_DEBUG_CH_XML, "XMLParser: found closing tag with name " + xml.name().toString());
                controller->exitElement();
            }
            else if (xml.isCDATA())
            {
                axr_log(AXR_DEBUG_CH_XML, QString("XMLParser: reading character data: \"%1\"").arg(xml.text().toString()));
                controller->appendContentText(xml.text().toString().toStdString());
            }
            else if (xml.isProcessingInstruction())
            {
                QString instructionName = xml.processingInstructionTarget().toString();
                axr_log(AXR_DEBUG_CH_XML, "XMLParser: found XML instruction with name " + instructionName);

                // Probable HSS stylesheet encountered, try to load it into the controller
                if (instructionName == "xml-stylesheet")
                {
                    QMap<QString, QString> instructionAttributes;

                    // A little trickery to easily parse the key-value pairs from the instruction...
                    // We basically embed the processing instruction value, which looks like a set of XML
                    // attributes, within a fake XML tag to create a document that we can parse, allowing
                    // the XML parser to handle the syntax instead of doing it manually
                    QXmlStreamReader instructionParser(QString("<node %1 />").arg(xml.processingInstructionData().toString()));
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
                        throw AXRError::p(new AXRError("XMLParser", "Malformed processing instruction - " + instructionParser.errorString().toStdString(), file->getFileName(), xml.lineNumber()));
                    }

                    // Require a type attribute so we can verify the stylesheet has the correct MIME type for HSS
                    if (!instructionAttributes.contains("type"))
                    {
                        throw AXRError::p(new AXRError("XMLParser", "Invalid xml-stylesheet instruction - missing type attribute", file->getFileName(), xml.lineNumber()));
                    }

                    // Require an src attribute so we know where to load the HSS file from
                    if (!instructionAttributes.contains("src"))
                    {
                        throw AXRError::p(new AXRError("XMLParser", "Invalid xml-stylesheet instruction - missing src attribute", file->getFileName(), xml.lineNumber()));
                    }

                    const QUrl &sheetUrl = instructionAttributes.value("src");
                    const QString &sheetType = instructionAttributes.value("type");
                    if (sheetType == "application/x-hss" || sheetType == "text/hss")
                    {
                        // We've got the right MIME type, now do we even have a URL?
                        if (!sheetUrl.isValid())
                        {
                            throw AXRError::p(new AXRError("XMLParser", "Invalid xml-stylesheet instruction - malformed URL in src attribute - " + sheetUrl.errorString().toStdString(), file->getFileName(), xml.lineNumber()));
                        }

                        // TODO: "Add to load later"?
                        controller->loadSheetsAdd(sheetUrl.toString().toStdString());
                    }
                    else if (sheetType == "application/xsl")
                    {
                        axr_log(AXR_DEBUG_CH_XML, "Ignoring XSL stylesheet in file " + file->getFileName());
                    }
                    else
                    {
                        axr_log(AXR_DEBUG_CH_XML, "Ignoring stylesheet of unknown type: " + sheetType.toStdString() + " in file " + file->getFileName());
                    }
                }
            }
        }

        if (xml.hasError())
        {
            throw AXRError::p(new AXRError("XMLParser", "XML error - " + xml.errorString().toStdString(), file->getFileName(), xml.lineNumber(), xml.columnNumber()));
        }

        return true;
    }
    catch (const AXRError::p &e)
    {
        e->raise();
        return false;
    }
}

std::string XMLParser::getFilePath()
{
    return this->file->getBasePath() + "/" + this->file->getFileName();
}

std::string XMLParser::getFileName()
{
    return this->file->getFileName();
}
