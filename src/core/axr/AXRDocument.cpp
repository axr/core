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

#include <QCoreApplication>
#include <QDir>
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRWarning.h"

using namespace AXR;

AXRDocument::AXRDocument()
{
    this->_isHSSOnly = false;
    this->_hasLoadedFile = false;
    this->_showLayoutSteps = false;
    this->_currentLayoutStep = 0;
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;
    this->_needsDisplay = true;

    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRDocument: initializing core for thread");

    AXRController::p ctrlr = AXRController::p(new AXRController(this));
    this->setController(ctrlr);
    this->setRender(AXRRender::p(new AXRRender(ctrlr.data())));
    this->setParserXML(XMLParser::p(new XMLParser(ctrlr.data())));
    this->setParserHSS(HSSParser::p(new HSSParser(ctrlr.data())));
    this->_hasLoadedFile = false;
}

AXRDocument::~AXRDocument()
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRDocument: destructing core");
}

void AXRDocument::drawInRectWithBounds(HSSRect rect, HSSRect bounds)
{
    if (this->render)
    {
        this->render->drawInRectWithBounds(rect, bounds);
    }
}

AXRString AXRDocument::getPathToResources() const
{
    // TODO: it's best to do this in a different way entirely... the library
    // shouldn't be using the client process's directory to determine where
    // to load resources from. For example, two different browsers will be
    // at different file paths, while the resources path will be independent
    // of either of them. Perhaps an AXR configuration file at a standard
    // location? /etc/axr.conf on Unix and %WINDIR%/axr.ini on Windows?
    QDir dir(QCoreApplication::applicationDirPath());
#ifdef Q_WS_MACX
    dir.cdUp();
    dir.cd("Resources");
#endif
    dir.cd("resources");
    return dir.canonicalPath();
}

void AXRDocument::run()
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRDocument: running");

    bool loadingSuccess = this->parserXML->loadFile(this->file);

    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRDocument: finished parsing " + this->file->sourceUrl().toString());
    axr_log(AXR_DEBUG_CH_FULL_FILENAMES, this->file->sourceUrl().toString());

    if (!loadingSuccess)
    {
        AXRError("AXRDocument", "Could not load the XML file").raise();
    }
    else
    {
        //needs reset on next load
        this->_hasLoadedFile = true;

        HSSContainer::p root = qSharedPointerCast<HSSContainer>(this->controller->getRoot());

        std::vector<QUrl> loadSheets = this->controller->loadSheetsGet();
        for (unsigned i = 0, size = loadSheets.size(); i < size; ++i)
        {
            AXRBuffer::p hssfile;
            try
            {
                hssfile = this->getFile(loadSheets[i]);
            }
            catch (const AXRError &e)
            {
                e.raise();
                continue;
            }

            if (!this->parserHSS->loadFile(hssfile))
            {
                AXRError("AXRDocument", "Could not load the HSS file").raise();
            }
        }
        axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRDocument: finished loading stylesheets for " + this->file->sourceUrl().toString());
        axr_log(AXR_DEBUG_CH_FULL_FILENAMES, this->file->sourceUrl().toString());

        axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRDocument: matching rules to the content tree");
        //assign the rules to the objects
        this->controller->matchRulesToContentTree();
        root->setNeedsRereadRules(true);

        if (root)
        {
            root->recursiveReadDefinitionObjects();
            root->handleEvent(HSSEventTypeLoad, NULL);
        }
    }

    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRDocument: run complete, entering event loop\n\n\n");
}

void AXRDocument::reset()
{
    this->controller->reset();
    this->parserHSS->reset();
    this->render->reset();
    this->parserXML.clear();
    this->parserXML = XMLParser::p(new XMLParser(this->controller.data()));
    this->_hasLoadedFile = false;
    this->file.clear();
}

bool AXRDocument::hasLoadedFile()
{
    return _hasLoadedFile;
}

AXRController::p AXRDocument::getController()
{
    return this->controller;
}

void AXRDocument::setController(AXRController::p controller)
{
    this->controller = controller;
}

AXRRender::p AXRDocument::getRender()
{
    return this->render;
}

void AXRDocument::setRender(AXRRender::p render)
{
    this->render = render;
}

AXRBuffer::p AXRDocument::getFile()
{
    return this->file;
}

void AXRDocument::setFile(AXRBuffer::p file)
{
    this->file = file;
}

XMLParser::p AXRDocument::getParserXML()
{
    return this->parserXML;
}

void AXRDocument::setParserXML(XMLParser::p parser)
{
    this->parserXML = parser;
}

HSSParser::p AXRDocument::getParserHSS()
{
    return this->parserHSS;
}

void AXRDocument::setParserHSS(HSSParser::p parser)
{
    this->parserHSS = parser;
}

bool AXRDocument::isCustomFunction(AXRString name)
{
    bool ret = this->_customFunctions.find(name) != this->_customFunctions.end();
    return ret;
}

void AXRDocument::registerCustomFunction(AXRString name, HSSCallback* fn)
{
    this->_customFunctions[name] = fn;
}

void AXRDocument::evaluateCustomFunction(AXRString name, void* data)
{
    if (this->isCustomFunction(name))
    {
        this->_customFunctions[name]->call(HSSObservablePropertyValue, data);
    }
}

AXRBuffer::p AXRDocument::getFile(QUrl u)
{
    if (u.isValid())
    {
        if (u.scheme() == "file")
        {
            QFileInfo fi(u.toLocalFile());
            AXRBuffer::p ret = AXRBuffer::p(new AXRBuffer(fi));
            if (!fi.exists())
            {
                throw AXRError("AXRDocument", "the file " + u.toLocalFile() + " doesn't exist");
            }
            else if (!ret->isValid())
            {
                throw AXRError("AXRDocument", "the file " + u.toLocalFile() + " couldn't be read");
            }

            return ret;
        }
        else if (u.scheme() == "http" || u.scheme() == "https")
        {
            std_log("http is not implemented yet");
        }
    }

    return AXRBuffer::p(new AXRBuffer());
}

bool AXRDocument::needsDisplay() const
{
    return this->_needsDisplay;
}

void AXRDocument::setNeedsDisplay(bool newValue)
{
    this->_needsDisplay = newValue;
}

AXRBuffer::p AXRDocument::createDummyXML(QUrl hssUrl)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRDocument: creating dummy XML file for HSS file " + hssUrl.toString());

    if (hssUrl.isValid())
    {
        AXRString dummyXML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><?xml-stylesheet type=\"application/x-hss\" href=\"" + hssUrl.toString() + "\" version=\"1.0\"?><root></root>";
        return AXRBuffer::p(new AXRBuffer(dummyXML.toUtf8()));
    }
    else
    {
        AXRError("AXRDocument", "Could not create dummy XML for invalid HSS file URL").raise();
        return AXRBuffer::p(new AXRBuffer());
    }
}

bool AXRDocument::loadFileByPath(QUrl url)
{
    if (url.isRelative())
    {
        if (url.isLocalFile())
        {
            url = QUrl::fromLocalFile(QFileInfo(url.path()).canonicalFilePath());
        }
        else
        {
            AXRError("AXRDocument", "Could not load relative URL as main file", url).raise();
            return false;
        }
    }

    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRDocument: loading file " + url.toString());
    axr_log(AXR_DEBUG_CH_FULL_FILENAMES, url.toString());

    QFileInfo pathInfo(url.path());
    if (pathInfo.suffix() == "xml")
    {
        this->_isHSSOnly = false;
        return this->loadXMLFile(url);
    }
    else if (pathInfo.suffix() == "hss")
    {
        return this->loadHSSFile(url);
    }
    else
    {
        AXRError("AXRController", "Unknown file extension").raise();
        return false;
    }
}

bool AXRDocument::loadXMLFile(QUrl url)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, AXRString("AXRDocument: opening XML document: %1").arg(url.toString()));

    this->_isHSSOnly = false;
    this->_showLayoutSteps = false;
    this->_currentLayoutStep = 0;
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;

    if (this->getFile())
    {
        this->reset();
    }

    try
    {
        AXRBuffer::p theFile = this->getFile(url);
        this->setFile(theFile);
    }
    catch (const AXRError &e)
    {
        e.raise();
        return false;
    }

    try
    {
        this->run();
    }
    catch (const AXRError &e)
    {
        e.raise();
    }

    this->setNeedsDisplay(true);

    return true;
}

bool AXRDocument::loadXMLFile(AXRBuffer::p buffer)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, AXRString("AXRDocument: opening XML document from buffer"));

    this->_isHSSOnly = false;
    this->_showLayoutSteps = false;
    this->_currentLayoutStep = 0;
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;

    if (this->getFile())
    {
        this->reset();
    }

    try
    {
        this->setFile(buffer);
    }
    catch (const AXRError &e)
    {
        e.raise();
        return false;
    }

    try
    {
        this->run();
    }
    catch (const AXRError &e)
    {
        e.raise();
    }

    this->setNeedsDisplay(true);

    return true;
}

bool AXRDocument::reload()
{
    this->_showLayoutSteps = false;
    this->_currentLayoutStep = 0;
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;
    this->getController()->getRoot()->recursiveResetLayout();

    if (!this->_isHSSOnly)
    {
        if (this->getFile())
        {
            return this->loadXMLFile(this->getFile()->sourceUrl());
        }
        else
        {
            AXRWarning("AXRDocument", "no file loaded").raise();
            return false;
        }
    }
    else
    {
        return this->loadHSSFile(this->getController()->loadSheetsGet(0));
    }
}

bool AXRDocument::loadHSSFile(QUrl url)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, AXRString("AXRDocument: opening HSS document: %1").arg(url.toString()));

    this->_isHSSOnly = true;
    this->_showLayoutSteps = false;
    this->_currentLayoutStep = 0;
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;

    if (this->getFile())
    {
        this->reset();
    }

    this->setFile(this->createDummyXML(url));
    this->run();
    this->setNeedsDisplay(true);

    return true;
}

void AXRDocument::setShowLayoutSteps(bool value)
{
    this->_showLayoutSteps = value;
}

bool AXRDocument::showLayoutSteps()
{
    return this->_showLayoutSteps;
}

void AXRDocument::previousLayoutStep()
{
    if (this->_currentLayoutStep > 0)
    {
        this->_currentLayoutStep -= 1;
    }
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;
    this->getController()->getRoot()->recursiveResetLayout();
    this->setNeedsDisplay(true);
}

void AXRDocument::nextLayoutStep()
{
    this->_currentLayoutStep += 1;
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;
    this->getController()->getRoot()->recursiveResetLayout();
    this->setNeedsDisplay(true);
}

void AXRDocument::nextLayoutTick()
{
    this->_currentLayoutTick += 1;
}

void AXRDocument::resetLayoutTicks()
{
    this->_currentLayoutTick = 0;
}

bool AXRDocument::layoutStepDone()
{
    return this->_currentLayoutTick >= this->_currentLayoutStep;
}

void AXRDocument::breakIfNeeded()
{
    bool breakvar = _currentLayoutTick >= (_currentLayoutStep - 1);

    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, AXRString("currentLayoutTick = %1, currentLayoutStep = %2, %3").arg(_currentLayoutTick).arg(_currentLayoutStep).arg(breakvar));
}

void AXRDocument::nextLayoutChild()
{
    this->_currentLayoutChild += 1;
}

void AXRDocument::resetLayoutChild()
{
    this->_currentLayoutChild = 0;
}

bool AXRDocument::layoutChildDone()
{
    return this->_currentLayoutTick >= this->_currentLayoutChild + 1;
}
