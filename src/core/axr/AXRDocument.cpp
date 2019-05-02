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

#include "axr.h"
#include "AXRDocument.h"

using namespace AXR;

namespace AXR
{
    class AXRDocumentPrivate
    {
        friend class AXRDocument;

        AXRDocumentPrivate()
        : delegate(NULL), isHSSOnly(), hasLoadedFile(), showLayoutSteps(), currentLayoutStep(), currentLayoutTick(),
          currentLayoutChild(), needsDisplay(true), windowWidth(), windowHeight(), visitorManager(),
        controller(), xmlfile(), directory(), parserXML(), parserHSS(), customFunctions()
        , cascadeVisitor(new HSSCascader)
        , layoutVisitor(new HSSLayout)
        , renderVisitor(new HSSRenderer)
        {
        }

        AXRDocumentDelegate * delegate;

        bool isHSSOnly;
        bool hasLoadedFile;
        bool showLayoutSteps;
        unsigned int currentLayoutStep;
        unsigned int currentLayoutTick;
        unsigned int currentLayoutChild;
        bool needsDisplay;

        int windowWidth;
        int windowHeight;

        QSharedPointer<HSSVisitorManager> visitorManager;
        QSharedPointer<AXRController> controller;
        QSharedPointer<AXRBuffer> xmlfile;
        AXRString directory;

        QSharedPointer<XMLParser> parserXML;
        QSharedPointer<HSSCodeParser> parserHSS;

        std::map<AXRString, HSSAbstractValueChangedCallback*> customFunctions;

        HSSCascader* cascadeVisitor;
        HSSLayout* layoutVisitor;
        HSSRenderer* renderVisitor;
        
        AXRPlatform * platform;
    };
}

AXRDocument::AXRDocument()
: d(new AXRDocumentPrivate)
{
    axr_log(LoggerChannelGeneralSpecific, "AXRDocument: initializing core for thread");

    QSharedPointer<AXRController> ctrlr = QSharedPointer<AXRController>(new AXRController(this));
    this->setController(ctrlr);
    this->setXmlParser(QSharedPointer<XMLParser>(new XMLParser(ctrlr.data())));
    QSharedPointer<HSSCodeParser> hssParser = QSharedPointer<HSSCodeParser>(new HSSCodeParser(this));
    hssParser->setController(ctrlr.data());
    this->setHssParser(hssParser);
    QSharedPointer<HSSVisitorManager>vm(new HSSVisitorManager(ctrlr.data()));
    this->setVisitorManager(vm);
    d->cascadeVisitor->setDocument(this);
    d->layoutVisitor->setDocument(this);
    d->renderVisitor->setDocument(this);
    vm->addVisitor(d->cascadeVisitor);
    vm->addVisitor(d->layoutVisitor);
    vm->addVisitor(d->renderVisitor);
    d->platform = new AXRPlatform();
    d->platform->setDocument(this);
}

AXRDocument::~AXRDocument()
{
    axr_log(LoggerChannelGeneralSpecific, "AXRDocument: destructing core");
    delete d;
}

AXRDocumentDelegate * AXRDocument::delegate()
{
    return d->delegate;
}

void AXRDocument::setDelegate(AXRDocumentDelegate * delegate)
{
    d->delegate = delegate;
}

AXRString AXRDocument::resourcesPath() const
{
    return d->platform->resourcesPath();
}

void AXRDocument::run()
{
    axr_log(LoggerChannelOverview, "AXRDocument: running");

    bool loadingSuccess = d->parserXML->loadFile(d->xmlfile);

    axr_log(LoggerChannelOverview, "AXRDocument: finished parsing " + d->xmlfile->sourceUrl());

    if (!loadingSuccess)
    {
        AXRError("AXRDocument", "Could not load the XML file").raise();
    }
    else
    {
        //needs reset on next load
        d->hasLoadedFile = true;

        QSharedPointer<HSSContainer> root = qSharedPointerCast<HSSContainer>(d->controller->root());

        const std::vector<AXRString> & styleSheetUrls = d->controller->styleSheetUrls();
        std::vector<AXRString>::const_iterator it;
        for (it = styleSheetUrls.begin(); it!=styleSheetUrls.end(); ++it)
        {
            const AXRString &styleSheetUrl = *it;
            QSharedPointer<AXRBuffer> hssfile;
            try
            {
                hssfile = this->createBufferFromUrl(styleSheetUrl);
            }
            catch (const AXRError &e)
            {
                e.raise();
                continue;
            }

            if (!d->parserHSS->loadFile(hssfile))
            {
                AXRError("AXRDocument", "Could not load the HSS file").raise();
            }
        }
        axr_log(LoggerChannelOverview, "AXRDocument: finished loading stylesheets for " + d->xmlfile->sourceUrl());

        axr_log(LoggerChannelOverview, "AXRDocument: matching rules to the content tree");
        //assign the rules to the objects
        d->controller->setUpTreeChangeObservers();
        d->controller->matchRulesToContentTree();
        d->controller->activateRules();
        root->setNeedsRereadRules(true);

        if (root)
        {
            QSharedPointer<HSSVisitorManager> visitorManager = this->visitorManager();
            visitorManager->runVisitors(HSSVisitorFilterCascading);
            HSSInputEvent event(HSSEventTypeLoad);
            root->handleEvent(&event);
        }
    }

    axr_log(LoggerChannelOverview, "AXRDocument: run complete, entering event loop\n\n\n");
}

void AXRDocument::runHSS(const AXRString & url)
{
    //needs reset on next load
    d->hasLoadedFile = true;

    QSharedPointer<HSSContainer> root = qSharedPointerCast<HSSContainer>(d->controller->root());

    QSharedPointer<AXRBuffer> hssfile;
    hssfile = this->createBufferFromUrl(url);
    if (!d->parserHSS->loadFile(hssfile))
    {
        AXRError("AXRDocument", "Could not load the HSS file").raise();
    }

    axr_log(LoggerChannelOverview, "AXRDocument: matching rules to the content tree");
    //assign the rules to the objects
    d->controller->setUpTreeChangeObservers();
    d->controller->matchRulesToContentTree();
    d->controller->activateRules();
    root->setNeedsRereadRules(true);

    if (root)
    {
        QSharedPointer<HSSVisitorManager> visitorManager = this->visitorManager();
        visitorManager->runVisitors(HSSVisitorFilterCascading);
        HSSInputEvent event(HSSEventTypeLoad);
        root->handleEvent(&event);
    }
}

void AXRDocument::reset()
{
    d->controller->reset();
    d->parserHSS->reset();
    d->visitorManager->reset();
    d->parserXML.clear();
    d->parserXML = QSharedPointer<XMLParser>(new XMLParser(d->controller.data()));
    d->hasLoadedFile = false;
    d->xmlfile.clear();
}

bool AXRDocument::isFileLoaded() const
{
    return d->hasLoadedFile;
}

QSharedPointer<AXRController> AXRDocument::controller() const
{
    return d->controller;
}

void AXRDocument::setController(QSharedPointer<AXRController> controller)
{
    d->controller = controller;
}

QSharedPointer<HSSVisitorManager> AXRDocument::visitorManager() const
{
    return d->visitorManager;
}

void AXRDocument::setVisitorManager(QSharedPointer<HSSVisitorManager> visitorManager)
{
    d->visitorManager = visitorManager;
}

QSharedPointer<AXRBuffer> AXRDocument::xmlFile() const
{
    return d->xmlfile;
}

void AXRDocument::setXmlFile(QSharedPointer<AXRBuffer> file)
{
    d->xmlfile = file;
    d->directory = d->platform->getDirectory(file);
}

AXRString AXRDocument::directory() const
{
    return d->directory;
}

QSharedPointer<XMLParser> AXRDocument::xmlParser() const
{
    return d->parserXML;
}

void AXRDocument::setXmlParser(QSharedPointer<XMLParser> parser)
{
    d->parserXML = parser;
}

QSharedPointer<HSSCodeParser> AXRDocument::hssParser() const
{
    return d->parserHSS;
}

void AXRDocument::setHssParser(QSharedPointer<HSSCodeParser> parser)
{
    d->parserHSS = parser;
}

AXRPlatform * AXRDocument::platform() const
{
    return d->platform;
}

bool AXRDocument::isCustomFunction(const AXRString &name) const
{
    return d->customFunctions.count(name);
}

void AXRDocument::registerCustomFunction(const AXRString &name, HSSAbstractValueChangedCallback* fn)
{
    d->customFunctions[name] = fn;
}

void AXRDocument::evaluateCustomFunction(const AXRString &name, const QSharedPointer<HSSObject> theObj)
{
    if (this->isCustomFunction(name))
    {
        d->customFunctions[name]->call("", "", theObj);
    }
}

HSSRenderer * AXRDocument::getRenderVisitor() const
{
    return d->renderVisitor;
}

QSharedPointer<AXRBuffer> AXRDocument::createBufferFromUrl(const AXRString &url)
{
    QSharedPointer<AXRBuffer> errorState;
    if (!d->platform->urlIsValid(url))
    {
        AXRError("AXRDocument", "Cannot load invalid URL - " + url).raise();
        return QSharedPointer<AXRBuffer>();
    }
    
    HSSString filePath;
    std::string urldata = url.data();
    if (url.data().substr(0, 6) == "axr://")
    {
        filePath = this->resourcesPath() + "/" + url;
    }
    else if (urldata.substr(0, 7) == "file://")
    {
        //skip the "file://" part
        filePath = urldata.substr(7);
    }
    else if (urldata.substr(0, 7) == "http://" || urldata.substr(0, 8) == "https://")
    {
        // TODO: Download resource and cache at local path, then localResource = QFileInfo(<path to that>);
        AXRError("AXRDocument", "http/https not implemented yet").raise();
        return QSharedPointer<AXRBuffer>();
    }
    else if (urldata.find("//") == std::string::npos)
    {
        filePath = this->getDirectory(this->xmlFile()) + "/" + url;
    }
    else
    {
        AXRError("AXRDocument", "Invalid URL " + url ).raise();
        return QSharedPointer<AXRBuffer>();
    }
    
    // Make sure the file at that path actually exists
    if (! d->platform->fileExists(filePath))
    {
        AXRError("AXRDocument", "the file " + url + " doesn't exist").raise();
        return errorState;
    }
    
    QSharedPointer<AXRBuffer> buffer = QSharedPointer<AXRBuffer>(new AXRBuffer(this, filePath));
    if (!buffer->isValid())
    {
        AXRError("AXRDocument", "the file " + filePath + " couldn't be read").raise();
        return errorState;
    }
    
    return buffer;
}

HSSString AXRDocument::getDirectory(QSharedPointer<AXRBuffer> theFile) const
{
    std::string filepath = theFile->sourceUrl().data();
    HSSString ret;
    if (filepath.substr(filepath.length() - 1) == "/")
    {
        filepath = filepath.substr(0, filepath.length() - 1);
    }
    ret = filepath.substr(0, filepath.find_last_of("/"));
    return ret;
}

bool AXRDocument::needsDisplay() const
{
    return d->needsDisplay;
}

void AXRDocument::setNeedsDisplay(bool newValue)
{
    d->needsDisplay = newValue;
    if (d->delegate)
        d->delegate->setNeedsDisplay(newValue);
}

QSharedPointer<AXRBuffer> AXRDocument::createDummyXml(const AXRString &hssUrl)
{
    axr_log(LoggerChannelOverview, "AXRDocument: creating dummy XML file for HSS file " + hssUrl);

    if (d->platform->urlIsValid(hssUrl))
    {
        AXRString dummyXML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><?xml-stylesheet type=\"application/x-hss\" href=\"" + hssUrl + "\" version=\"1.0\"?><root></root>";
        return QSharedPointer<AXRBuffer>(new AXRBuffer(dummyXML, "dummyXML"));
    }
    else
    {
        AXRError("AXRDocument", "Could not create dummy XML for invalid HSS file URL").raise();
        return QSharedPointer<AXRBuffer>();
    }
}

bool AXRDocument::loadFileByPath(const HSSString &url)
{
    axr_log(LoggerChannelOverview, "AXRDocument: loading file " + url);

    if (url.substr(url.length() - 3) == "xml")
    {
        d->isHSSOnly = false;
        return this->loadXmlFile(url);
    }
    else if (url.substr(url.length() - 3) == "hss")
    {
        return this->loadHssFile(url);
    }
    else
    {
        AXRError("AXRController", "Unknown file extension").raise();
        return false;
    }
}

bool AXRDocument::loadXmlFile(const AXRString &url)
{
    axr_log(LoggerChannelOverview, HSSString::format("AXRDocument: opening XML document: %s", url.chardata()));

    d->isHSSOnly = false;
    d->showLayoutSteps = false;
    d->currentLayoutStep = 0;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;

    try
    {
        QSharedPointer<AXRBuffer> theFile = this->createBufferFromUrl(url);
        if (this->xmlFile())
        {
            this->reset();
        }
        this->setXmlFile(theFile);
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

bool AXRDocument::loadXmlFile(QSharedPointer<AXRBuffer> buffer)
{
    axr_log(LoggerChannelOverview, AXRString("AXRDocument: opening XML document from buffer"));

    d->isHSSOnly = false;
    d->showLayoutSteps = false;
    d->currentLayoutStep = 0;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;

    if (this->xmlFile())
    {
        this->reset();
    }

    try
    {
        this->setXmlFile(buffer);
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
    d->showLayoutSteps = false;
    d->currentLayoutStep = 0;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;
    this->controller()->root()->recursiveResetLayout();

    if (!d->isHSSOnly)
    {
        if (this->xmlFile())
        {
            return this->loadXmlFile(this->xmlFile()->sourceUrl());
        }
        else
        {
            AXRWarning("AXRDocument", "no file loaded").raise();
            return false;
        }
    }
    else
    {
        return this->loadHssFile(this->controller()->styleSheetUrlAt(0));
    }
}

bool AXRDocument::handleEvent(HSSInputEvent *event)
{
    QSharedPointer<HSSContainer> root = controller()->root();
    if (root)
    {
        controller()->setCurrentEvent(HSSEvent::createEvent(controller().data(), event));
        return root->handleEvent(event);
    }

    return false;
}

void AXRDocument::handleSelection(HSSPoint thePoint)
{
    QSharedPointer<HSSContainer> root = controller()->root();
    if (root)
        root->handleSelection(thePoint);
}

void AXRDocument::selectionChanged(QSharedPointer<HSSDisplayObject> theDO)
{
    if (d->delegate != NULL)
    {
        d->delegate->selectionChanged(theDO);
    }
}

void AXRDocument::startTimer(AXRString timerName, HSSFunction * timerFunction, unsigned int ms, bool repeats)
{
    this->delegate()->startTimer(timerName, timerFunction, ms, repeats);
}

void AXRDocument::startTimerOnce(HSSFunction * timerFunction, unsigned int ms)
{
    this->delegate()->startTimerOnce(timerFunction, ms);
}

void AXRDocument::stopTimer(AXRString timerName)
{
    this->delegate()->stopTimer(timerName);
}

bool AXRDocument::hasTimer(AXRString timerName)
{
    return this->delegate()->hasTimer(timerName);
}

bool AXRDocument::loadHssFile(const AXRString &url)
{
    axr_log(LoggerChannelOverview, HSSString::format("AXRDocument: opening HSS document: %s", url.chardata()));

    d->isHSSOnly = true;
    d->showLayoutSteps = false;
    d->currentLayoutStep = 0;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;

    if (this->xmlFile())
    {
        this->reset();
    }

    this->setXmlFile(this->createDummyXml(url));
    this->run();
    return true;
}

bool AXRDocument::loadHssFile(QSharedPointer<AXRBuffer> buffer)
{
    d->controller->enterElement("document");
    d->controller->exitElement();
    QSharedPointer<HSSContainer> root = d->controller->root();

    //needs reset on next load
    d->hasLoadedFile = true;

    d->isHSSOnly = true;
    d->showLayoutSteps = false;
    d->currentLayoutStep = 0;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;
    if (!d->parserHSS->loadFile(buffer))
    {
        AXRError("AXRDocument", "Could not load the HSS file").raise();
    }
    axr_log(LoggerChannelOverview, "AXRDocument: matching rules to the content tree");
    //assign the rules to the objects
    d->controller->setUpTreeChangeObservers();
    d->controller->matchRulesToContentTree();
    d->controller->activateRules();

    root->setNeedsRereadRules(true);
    QSharedPointer<HSSVisitorManager> visitorManager = this->visitorManager();
    visitorManager->runVisitors(HSSVisitorFilterCascading);
    HSSInputEvent event(HSSEventTypeLoad);
    root->handleEvent(&event);
    return true;
}

void AXRDocument::receiveParserEvent(HSSParserEvent eventType, QSharedPointer<HSSParserNode> node)
{
    switch (node->getType()) {
        case HSSParserNodeTypeStatement:
        {
            QSharedPointer<HSSStatement> statementNode = qSharedPointerCast<HSSStatement>(node);
            switch (statementNode->getStatementType()) {
                case HSSStatementTypeRule:
                {
                    QSharedPointer<HSSRule> ruleNode = qSharedPointerCast<HSSRule>(node);
                    d->controller->addRule(ruleNode);
                    break;
                }
                case HSSStatementTypeInstruction:
                {
                    d->controller->addParserTreeNode(node);
                    break;
                }
                case HSSStatementTypeObjectDefinition:
                {
                    QSharedPointer<HSSObjectDefinition> objDefNode = qSharedPointerCast<HSSObjectDefinition>(node);
                    HSSString objDefName = objDefNode->name();
                    if (!objDefName.isEmpty())
                    {
                        QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection(d->controller.data()));
                        rootScope->add(d->controller->root());
                        QSharedPointer<HSSValue> theVal = HSSValue::valueFromParserNode(d->controller.data(), objDefNode, objDefNode->getSpecificity(), objDefNode->getThisObj(), rootScope);
                        d->controller->setGlobalVariable(objDefName, theVal);
                    }
                    else
                    {
                        AXRWarning("HSSDocument", "Ignoring annonymous object definition.").raise();
                    }
                    break;
                }
                    
                case HSSStatementTypeVarDeclaration:
                {
                    QSharedPointer<HSSVarDeclaration> varDecl = qSharedPointerCast<HSSVarDeclaration>(node);
                    d->controller->setGlobalVariable(varDecl->name(), varDecl->assignment()->evaluate());
                    break;
                }
                case HSSStatementTypeAssignment:
                {
                    QSharedPointer<HSSAssignment> asgmt = qSharedPointerCast<HSSAssignment>(node);
                    QSharedPointer<HSSPropertyPath> ppath = asgmt->propertyPath();
                    if (ppath->size() == 1)
                    {
                        QSharedPointer<HSSPropertyPathNode> pnode = ppath->front();
                        d->controller->setGlobalVariable(pnode->evaluate(), asgmt->evaluate());
                    }
                    break;
                }

                default:
                {
                    d->controller->addParserTreeNode(node);
                    break;
                }
            }
            break;
        }

        case HSSParserNodeTypeWhitespaceNode:
        {
            d->controller->addParserTreeNode(node);
            break;
        }

        default:
            d->controller->addParserTreeNode(node);
            break;
    }
}

void AXRDocument::setShowLayoutSteps(bool value)
{
    d->showLayoutSteps = value;
}

bool AXRDocument::showLayoutSteps() const
{
    return d->showLayoutSteps;
}

void AXRDocument::previousLayoutStep()
{
    if (d->currentLayoutStep > 0)
    {
        d->currentLayoutStep -= 1;
    }
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;
    this->controller()->root()->recursiveResetLayout();
    this->setNeedsDisplay(true);
}

void AXRDocument::nextLayoutStep()
{
    d->currentLayoutStep += 1;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;
    this->controller()->root()->recursiveResetLayout();
    this->setNeedsDisplay(true);
}

void AXRDocument::nextLayoutTick()
{
    d->currentLayoutTick += 1;
}

void AXRDocument::resetLayoutTicks()
{
    d->currentLayoutTick = 0;
}

bool AXRDocument::layoutStepDone() const
{
    return d->currentLayoutTick >= d->currentLayoutStep;
}

void AXRDocument::breakIfNeeded()
{
    int breakvar = 0;
    if(d->currentLayoutTick >= d->currentLayoutStep-1){
        breakvar = 1; //we need something to break on
    }

    axr_log(LoggerChannelLayout, HSSString::format("currentLayoutTick = %u, currentLayoutStep = %u, %i", d->currentLayoutTick, d->currentLayoutStep, breakvar));
}

int AXRDocument::windowWidth() const
{
    return d->windowWidth;
}

int AXRDocument::windowHeight() const
{
    return d->windowHeight;
}

void AXRDocument::setWindowSize(int width, int height)
{
    d->windowWidth = width;
    d->windowHeight = height;
}

void AXRDocument::nextLayoutChild()
{
    d->currentLayoutChild += 1;
}

void AXRDocument::resetLayoutChild()
{
    d->currentLayoutChild = 0;
}

bool AXRDocument::layoutChildDone() const
{
    return d->currentLayoutTick >= d->currentLayoutChild + 1;
}

bool AXRDocument::isFunction(AXRString value)
{
    if (value == "min"
        || value == "max"
        || value == "floor"
        || value == "ceil"
        || value == "round"
        || value == "ref"
        || value == "sel"
        || value == "flag"
        || value == "unflag"
        || value == "addFlag"
        || value == "takeFlag"
        || value == "replaceFlag"
        || value == "attr"
        || value == "log"
        || value == "override"
        || value == "startTimer"
        || value == "stopTimer"
        || value == "toggleTimer"
        || value == "insert"
        || value == "if"
        || value == "else"
        || value == "switch"
        || value == "function"
        || value == "return"
        )
    {
        return true;
    }
    else
    {
        return this->isCustomFunction(value);
    }
}



bool AXRDocument::isKeyword(AXRString value)
{
    if (value == "default"
        || value == "null"
        || value == "undefined"
        || value == "true"
        || value == "false"
        || value == "yes"
        || value == "no"
        || value == "inherit"
        || value == "content"
        || value == "start"
        || value == "middle"
        || value == "center"
        || value == "end"
        || value == "left"
        || value == "right"
        || value == "top"
        || value == "topLeft"
        || value == "topRight"
        || value == "bottom"
        || value == "bottomLeft"
        || value == "bottomRight"
        || value == "all"
        || value == "inside"
        || value == "centered"
        || value == "outside"
        || value == "black"
        || value == "white"
        || value == "transparent"
        || value == "even"
        || value == "justify"
        || value == "ltr"
        || value == "rtl"
        || value == "ttb"
        || value == "bbt"
        || value == "regular"
        || value == "normal"
        || value == "bold"
        || value == "medium"
        || value == "thin"
        || value == "light"
        || value == "book"
        || value == "black"
        || value == "heavy"
        || value == "ultralight"
        || value == "semibold"
        || value == "ultrabold"
        || value == "ultraheavy"
        || value == "lowercase"
        || value == "uppercase"
        || value == "capitalize"
        || value == "humanize"
        || value == "rounded"
        || value == "projected"
        )
    {
        return true;
    }
    else
    {
        //return this->isCustomKeyword(value);
    }
    return false;
    return (value == "default" || value == "inherit" || value == "undefined");
}
