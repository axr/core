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

#include "AXRInitializer.h"

using namespace AXR;

AXRCore::tp & AXRCore::getInstance()
{
    static AXRCore::tp theInstance;
    if (!theInstance.get())
    {
        theInstance.reset(new AXRCore());
    }
    return theInstance;
}

AXRCore::AXRCore()
{

}

void AXRCore::initialize(AXRWrapper * wrpr)
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRCore: initializing core for thread");

    AXRController::p ctrlr = AXRController::p(new AXRController());
    AXRRender::p rndr = AXRRender::p(new AXRRender(ctrlr.get()));
    this->setWrapper(wrpr);
    this->setController(ctrlr);
    this->setRender(rndr);
    this->setParserXML(XMLParser::p(new XMLParser(ctrlr.get())));
    this->setParserHSS(HSSParser::p(new HSSParser(ctrlr.get(), wrpr)));
    this->_hasLoadedFile = false;
}

AXRCore::~AXRCore()
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRCore: destructing core");
}

void AXRCore::drawInRectWithBounds(HSSRect rect, HSSRect bounds)
{
    if (this->render)
    {
        this->render->drawInRectWithBounds(rect, bounds);
    }
}

void AXRCore::run()
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRCore: running");
    //check for wrapper
    if (!this->getWrapper())
    {
        AXRError::p(new AXRError("AXRCore", "The wrapper was not defined"))->raise();
        return;
    }
    bool loadingSuccess = this->parserXML->loadFile(this->file);

    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRCore: finished parsing " + this->file->getFileName());
    axr_log(AXR_DEBUG_CH_FULL_FILENAMES, this->file->getBasePath() + "/" + this->file->getFileName());

    if (!loadingSuccess)
    {
        AXRError::p(new AXRError("AXRCore", "Could not load the XML file"))->raise();
    }
    else
    {
        //needs reset on next load
        this->_hasLoadedFile = true;

        HSSContainer::p root = boost::static_pointer_cast<HSSContainer > (this->controller->getRoot());
        AXRString hssfilename, hssfilepath;

        std::vector<AXRString> loadSheets = this->controller->loadSheetsGet();
        for (unsigned i = 0, size = loadSheets.size(); i < size; ++i)
        {

            hssfilename = loadSheets[i];
            if (hssfilename.startsWith("file://"))
            {
                hssfilepath = hssfilename;
            }
            else if (hssfilename.startsWith("http://"))
            {
                AXRError::p(new AXRError("AXRCore", "HTTP has not been implemented yet"))->raise();
            }
            else if (hssfilename.startsWith("/"))
            {
                hssfilepath = this->file->getBasePath() + hssfilename;
            }
            else
            {
                hssfilepath = "file://" + this->file->getBasePath() + "/" + hssfilename;
            }
            AXRFile::p hssfile;
            try
            {
                hssfile = this->wrapper->getFile(hssfilepath);
            }
            catch (const AXRError::p &e)
            {
                e->raise();
                continue;
            }

            this->parserHSS->setBasePath(this->file->getBasePath());
            if (!this->parserHSS->loadFile(hssfile))
            {
                AXRError::p(new AXRError("AXRCore", "Could not load the HSS file"))->raise();
            }
        }
        axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRCore: finished loading stylesheets for " + this->file->getFileName());
        axr_log(AXR_DEBUG_CH_FULL_FILENAMES, this->file->getBasePath() + "/" + this->file->getFileName());

        axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRCore: matching rules to the content tree");
        //assign the rules to the objects
        this->controller->matchRulesToContentTree();
        root->setNeedsRereadRules(true);

        if (root)
        {
            root->recursiveReadDefinitionObjects();
            root->handleEvent(HSSEventTypeLoad, NULL);
        }
    }

    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRCore: run complete, entering event loop\n\n\n");
}

void AXRCore::reset()
{
    this->controller->reset();
    this->parserHSS->reset();
    this->render->reset();
    this->parserXML.reset();
    this->parserXML = XMLParser::p(new XMLParser(this->controller.get()));
    this->_hasLoadedFile = false;
    this->file.reset();
}

//has loaded file

bool AXRCore::hasLoadedFile()
{
    return _hasLoadedFile;
}

AXRWrapper * AXRCore::getWrapper()
{
    return this->wrapper;
}

void AXRCore::setWrapper(AXRWrapper * wrapper)
{
    this->wrapper = wrapper;
}

AXRController::p AXRCore::getController()
{
    return this->controller;
}

void AXRCore::setController(AXRController::p controller)
{
    this->controller = controller;
}

AXRRender::p AXRCore::getRender()
{
    return this->render;
}

void AXRCore::setRender(AXRRender::p render)
{
    this->render = render;
}

AXRFile::p AXRCore::getFile()
{
    return this->file;
}

void AXRCore::setFile(AXRFile::p file)
{
    this->file = file;
}

XMLParser::p AXRCore::getParserXML()
{
    return this->parserXML;
}

void AXRCore::setParserXML(XMLParser::p parser)
{
    this->parserXML = parser;
}

HSSParser::p AXRCore::getParserHSS()
{
    return this->parserHSS;
}

void AXRCore::setParserHSS(HSSParser::p parser)
{
    this->parserHSS = parser;
}

bool AXRCore::isCustomFunction(AXRString name)
{
    bool ret = this->_customFunctions.find(name) != this->_customFunctions.end();
    return ret;
}

void AXRCore::registerCustomFunction(AXRString name, HSSCallback* fn)
{
    this->_customFunctions[name] = fn;
}

void AXRCore::evaluateCustomFunction(AXRString name, void* data)
{
    if (this->isCustomFunction(name))
    {
        this->_customFunctions[name]->call(HSSObservablePropertyValue, data);
    }
}
