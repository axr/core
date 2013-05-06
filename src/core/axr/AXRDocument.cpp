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

#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>
#include "AXRBuffer.h"
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSMouseEvent.h"
#include "HSSParser.h"
#include "HSSVisitorManager.h"
#include "XMLParser.h"

using namespace AXR;

namespace AXR
{
    class AXRDocumentPrivate
    {
        friend class AXRDocument;

        AXRDocumentPrivate()
        : isHSSOnly(), hasLoadedFile(), showLayoutSteps(), currentLayoutStep(), currentLayoutTick(),
          currentLayoutChild(), needsDisplay(true), windowWidth(), windowHeight(), visitorManager(),
          controller(), file(), directory(), parserXML(), parserHSS(), customFunctions()
        {
        }

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
        QSharedPointer<AXRBuffer> file;
        QDir directory;

        QSharedPointer<XMLParser> parserXML;
        QSharedPointer<HSSParser> parserHSS;

        QMap<AXRString, HSSCallback*> customFunctions;
    };
}

AXRDocument::AXRDocument()
: d(new AXRDocumentPrivate)
{
    axr_log(LoggerChannelGeneralSpecific, "AXRDocument: initializing core for thread");

    QSharedPointer<AXRController> ctrlr = QSharedPointer<AXRController>(new AXRController(this));
    this->setController(ctrlr);
    this->setXmlParser(QSharedPointer<XMLParser>(new XMLParser(ctrlr.data())));
    this->setHssParser(QSharedPointer<HSSParser>(new HSSParser(ctrlr.data())));
    this->setVisitorManager(QSharedPointer<HSSVisitorManager>(new HSSVisitorManager(ctrlr.data())));
}

AXRDocument::~AXRDocument()
{
    axr_log(LoggerChannelGeneralSpecific, "AXRDocument: destructing core");
    delete d;
}

AXRString AXRDocument::resourcesPath() const
{
    // TODO: it's best to do this in a different way entirely... the library
    // shouldn't be using the client process's directory to determine where
    // to load resources from. For example, two different browsers will be
    // at different file paths, while the resources path will be independent
    // of either of them. Perhaps an AXR configuration file at a standard
    // location? /etc/axr.conf on Unix and %WINDIR%/axr.ini on Windows?
    QDir dir(QCoreApplication::applicationDirPath());
#ifdef Q_OS_MAC
#ifndef Q_OS_IOS
    dir.cdUp();
#endif
    dir.cd("Resources");
#endif
    dir.cd("resources");
    return dir.canonicalPath();
}

void AXRDocument::run()
{
    axr_log(LoggerChannelOverview, "AXRDocument: running");

    bool loadingSuccess = d->parserXML->loadFile(d->file);

    axr_log(LoggerChannelOverview, "AXRDocument: finished parsing " + d->file->sourceUrl().toString());

    if (!loadingSuccess)
    {
        AXRError("AXRDocument", "Could not load the XML file").raise();
    }
    else
    {
        //needs reset on next load
        d->hasLoadedFile = true;

        QSharedPointer<HSSContainer> root = qSharedPointerCast<HSSContainer>(d->controller->root());

        const QList<QUrl> &styleSheetUrls = d->controller->styleSheetUrls();
        Q_FOREACH (const QUrl &styleSheetUrl, styleSheetUrls)
        {
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
        axr_log(LoggerChannelOverview, "AXRDocument: finished loading stylesheets for " + d->file->sourceUrl().toString());

        axr_log(LoggerChannelOverview, "AXRDocument: matching rules to the content tree");
        //assign the rules to the objects
        d->controller->matchRulesToContentTree();
        d->controller->activateRules();
        root->setNeedsRereadRules(true);

        if (root)
        {
            root->recursiveReadDefinitionObjects();
            HSSInputEvent event(HSSEventTypeLoad);
            root->handleEvent(&event);
        }
    }

    axr_log(LoggerChannelOverview, "AXRDocument: run complete, entering event loop\n\n\n");
}

void AXRDocument::reset()
{
    d->controller->reset();
    d->parserHSS->reset();
    d->visitorManager->reset();
    d->parserXML.clear();
    d->parserXML = QSharedPointer<XMLParser>(new XMLParser(d->controller.data()));
    d->hasLoadedFile = false;
    d->file.clear();
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

QSharedPointer<AXRBuffer> AXRDocument::file() const
{
    return d->file;
}

void AXRDocument::setFile(QSharedPointer<AXRBuffer> file)
{
    d->file = file;
    QUrl filePath = file->sourceUrl();
    d->directory = QDir(QFileInfo(filePath.toLocalFile()).canonicalPath());
}

QDir AXRDocument::directory() const
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

QSharedPointer<HSSParser> AXRDocument::hssParser() const
{
    return d->parserHSS;
}

void AXRDocument::setHssParser(QSharedPointer<HSSParser> parser)
{
    d->parserHSS = parser;
}

bool AXRDocument::isCustomFunction(const AXRString &name) const
{
    return d->customFunctions.contains(name);
}

void AXRDocument::registerCustomFunction(const AXRString &name, HSSCallback* fn)
{
    d->customFunctions[name] = fn;
}

void AXRDocument::evaluateCustomFunction(const AXRString &name, void* data)
{
    if (this->isCustomFunction(name))
    {
        d->customFunctions[name]->call(HSSObservablePropertyValue, data);
    }
}

/*!
 * \brief sanitizeUrlPath
 * Sanitize a URL path so that it is safe for joining with a local directory
 * path to read resources from the local file system.
 *
 * This forces the path to become relative if it is absolute. A leading slash
 * will be removed and any attempt to cdup outside the root simply resolves
 * back to root.
 *
 * All backslashes in the path are replaced with forward slashes. Then all
 * multiple directory separators are removed and any "." or ".."s found in
 * the path are resolved. Any leading "."s or ".."s are removed such that
 * a URL path of "../foo.html" or "/../foo.html" is resolved to "foo.html"
 * in the same manner that /../ resolves to / on a UNIX system.
 *
 * An example usage might be: \code QFileInfo(getPathToResources(), sanitizeRelativePath(resourceUrl.path())).canonicalFilePath() \endcode
 * which would allow a URL with a custom protocol to refer to resources within
 * a special directory on the local filesystem, or allow a web server to safely
 * map HTTP URLs to the corresponding directory from which they should be served,
 * on the local filesystem.
 *
 * \param path The path to sanitize.
 */
QString sanitizeRelativePath(const QString &path)
{
    // Swap backslashes for forward slashes
    QString canonicallySlashedPath = path;
    canonicallySlashedPath = canonicallySlashedPath.replace('\\', '/');

    // Get a list of all path components after switching backslashes for slashes and resolving
    // "."s and ".."s and removing redundant directory separators
    QStringList pathParts = QDir::cleanPath(canonicallySlashedPath).split('/', QString::SkipEmptyParts);

    // Remove any leading "."s or ".."s that were not resolved by QDir::cleanPath
    pathParts.removeAll(".");
    pathParts.removeAll("..");

    // Rejoin the path components into a URL that is guaranteed relative and has no "."s or ".."s
    // or redundant slashes
    return pathParts.join(QDir::separator());
}

QSharedPointer<AXRBuffer> AXRDocument::createBufferFromUrl(const QUrl &resourceUrl)
{
    if (!resourceUrl.isValid())
    {
        AXRError("AXRDocument", "Cannot load invalid URL - " + resourceUrl.toString()).raise();
        return QSharedPointer<AXRBuffer>(new AXRBuffer());
    }

    // Map the URL into a local file path we can load
    QFileInfo localResource;
    if (resourceUrl.scheme() == "axr")
    {
        localResource = QFileInfo(this->resourcesPath(), sanitizeRelativePath(resourceUrl.path()));
    }
    else if (resourceUrl.scheme() == "file")
    {
        // This is a workaround for a QUrl bug where QUrl::toLocalFile()
        // for the URL file://localhost/Users/bob/example.xml => //localhost/Users/bob/example.xml,
        // which cannot be opened directly as a file; we need to handle this centrally elsewhere
        // If the host isn't localhost the we'll still get a non-loadable file path but at least
        // we know that file://localhost/ is almost always going to be equal to file:///
        // We should still perhaps look at alternative ways of handling this issue as it seems hacky
        QUrl fixedUrl = resourceUrl;
        if (resourceUrl.host() == "localhost")
            fixedUrl.setHost(QString());

        // TODO: Make sure this only unconditionally loads file URLs when appropriate
        localResource = QFileInfo(fixedUrl.toLocalFile());
    }
    else if (resourceUrl.scheme() == "http" || resourceUrl.scheme() == "https")
    {
        // TODO: Download resource and cache at local path, then localResource = QFileInfo(<path to that>);
        AXRError("AXRDocument", "http/https not implemented yet").raise();
        return QSharedPointer<AXRBuffer>(new AXRBuffer());
    }
    else if (resourceUrl.scheme() == "")
    {
        QString fileScheme = this->file()->sourceUrl().scheme();
        if (fileScheme == "file")
        {
            // TODO: this needs to be sanitized in another way, we can't break the usage of .. in
            // relative paths for legal uses. We need something like a cross-origin policy or similar.
            localResource = QFileInfo(this->directory(), sanitizeRelativePath(resourceUrl.path()));
        }
        else if (fileScheme == "http" || fileScheme == "https")
        {
            AXRError("AXRDocument", "http/https not implemented yet").raise();
            return QSharedPointer<AXRBuffer>(new AXRBuffer());
        }
    }
    else
    {
        AXRError("AXRDocument", "Unsupported URL scheme " + resourceUrl.scheme()).raise();
        return QSharedPointer<AXRBuffer>(new AXRBuffer());
    }

    // Make sure the file at that path actually exists
    if (!localResource.exists())
    {
        throw AXRError("AXRDocument", "the file " + localResource.filePath() + " doesn't exist");
    }

    QSharedPointer<AXRBuffer> buffer = QSharedPointer<AXRBuffer>(new AXRBuffer(localResource));
    if (!buffer->isValid())
    {
        throw AXRError("AXRDocument", "the file " + localResource.filePath() + " couldn't be read");
    }

    return buffer;

    // TODO: AXRDocument should be able to return an error code from an enum stating WHY a document
    // failed to load so that failures can be handled programmatically depending on the reason
}

bool AXRDocument::needsDisplay() const
{
    return d->needsDisplay;
}

void AXRDocument::setNeedsDisplay(bool newValue)
{
    d->needsDisplay = newValue;
}

QSharedPointer<AXRBuffer> AXRDocument::createDummyXml(const QUrl &hssUrl)
{
    axr_log(LoggerChannelOverview, "AXRDocument: creating dummy XML file for HSS file " + hssUrl.toString());

    if (hssUrl.isValid())
    {
        AXRString dummyXML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><?xml-stylesheet type=\"application/x-hss\" href=\"" + hssUrl.toString() + "\" version=\"1.0\"?><root></root>";
        return QSharedPointer<AXRBuffer>(new AXRBuffer(dummyXML.toUtf8()));
    }
    else
    {
        AXRError("AXRDocument", "Could not create dummy XML for invalid HSS file URL").raise();
        return QSharedPointer<AXRBuffer>(new AXRBuffer());
    }
}

bool AXRDocument::loadFileByPath(const QUrl &url)
{
    if (url.isRelative() && !url.isLocalFile())
    {
        AXRError("AXRDocument", "Could not load relative URL as main file", url).raise();
        return false;
    }

    axr_log(LoggerChannelOverview, "AXRDocument: loading file " + url.toString());

    QFileInfo pathInfo(url.path());
    if (pathInfo.suffix() == "xml")
    {
        d->isHSSOnly = false;
        return this->loadXmlFile(url);
    }
    else if (pathInfo.suffix() == "hss")
    {
        return this->loadHssFile(url);
    }
    else
    {
        AXRError("AXRController", "Unknown file extension").raise();
        return false;
    }
}

bool AXRDocument::loadXmlFile(const QUrl &url)
{
    axr_log(LoggerChannelOverview, AXRString("AXRDocument: opening XML document: %1").arg(url.toString()));

    d->isHSSOnly = false;
    d->showLayoutSteps = false;
    d->currentLayoutStep = 0;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;

    try
    {
        QSharedPointer<AXRBuffer> theFile = this->createBufferFromUrl(url);
        if (this->file())
        {
            this->reset();
        }
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

bool AXRDocument::loadXmlFile(QSharedPointer<AXRBuffer> buffer)
{
    axr_log(LoggerChannelOverview, AXRString("AXRDocument: opening XML document from buffer"));

    d->isHSSOnly = false;
    d->showLayoutSteps = false;
    d->currentLayoutStep = 0;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;

    if (this->file())
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
    d->showLayoutSteps = false;
    d->currentLayoutStep = 0;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;
    this->controller()->root()->recursiveResetLayout();

    if (!d->isHSSOnly)
    {
        if (this->file())
        {
            return this->loadXmlFile(this->file()->sourceUrl());
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
        return root->handleEvent(event);

    return false;
}

bool AXRDocument::loadHssFile(const QUrl &url)
{
    axr_log(LoggerChannelOverview, AXRString("AXRDocument: opening HSS document: %1").arg(url.toString()));

    d->isHSSOnly = true;
    d->showLayoutSteps = false;
    d->currentLayoutStep = 0;
    d->currentLayoutTick = 0;
    d->currentLayoutChild = 0;

    if (this->file())
    {
        this->reset();
    }

    this->setFile(this->createDummyXml(url));
    this->run();
    return true;
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

    axr_log(LoggerChannelLayout, AXRString("currentLayoutTick = %1, currentLayoutStep = %2, %3").arg(d->currentLayoutTick).arg(d->currentLayoutStep).arg(breakvar));
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
