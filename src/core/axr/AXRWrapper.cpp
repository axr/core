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
#include <QFile>
#include <QUrl>
#include "AXRInitializer.h"
#include "AXRWarning.h"
#include "AXRWrapper.h"
#include "HSSFunction.h"
#include "HSSSelFunction.h"

using namespace AXR;

AXRWrapper::AXRWrapper()
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRWrapper: creating AXRWrapper");
    this->_isHSSOnly = false;
    this->_hasLoadedFile = false;
    this->_showLayoutSteps = false;
    this->_currentLayoutStep = 0;
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;
    this->_needsDisplay = true;
    AXRCore::tp & axr = AXRCore::getInstance();
    axr->initialize(this);
}

AXRWrapper::~AXRWrapper()
{
}

AXRBuffer::p AXRWrapper::getFile(AXRString url)
{
    QUrl u(url);
    if (u.isValid())
    {
        if (u.scheme() == "file")
        {
            QFileInfo fi(u.toLocalFile());
            AXRBuffer::p ret = AXRBuffer::p(new AXRBuffer(fi));
            if (!fi.exists())
            {
                throw AXRError::p(new AXRError("AXRWrapper", "the file " + url + " doesn't exist"));
            }
            else if (!ret->isValid())
            {
                throw AXRError::p(new AXRError("AXRWrapper", "the file " + url + " couldn't be read"));
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

bool AXRWrapper::needsDisplay() const
{
    return this->_needsDisplay;
}

void AXRWrapper::setNeedsDisplay(bool newValue)
{
    this->_needsDisplay = newValue;
}

AXRBuffer::p AXRWrapper::createDummyXML(AXRString stylesheet)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRWrapper: creating dummy XML file for HSS file " + stylesheet);

    QUrl url = stylesheet;
    if (url.isValid())
    {
        AXRString dummyXML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><?xml-stylesheet type=\"application/x-hss\" src=\"file://" + stylesheet + "\" version=\"1.0\"?><root></root>";
        return AXRBuffer::p(new AXRBuffer(dummyXML.toUtf8()));
    }
    else
    {
        // TODO: "Not a valid URL"
        AXRError::p(new AXRError("AXRWrapper", "Could not find a slash in the file path"))->raise();
        return AXRBuffer::p(new AXRBuffer());
    }
}

bool AXRWrapper::loadFileByPath(AXRString filepath)
{
    QFileInfo fi(filepath);
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRWrapper: loading file " + fi.fileName() + " by path");
    axr_log(AXR_DEBUG_CH_FULL_FILENAMES, fi.canonicalFilePath());

    if (fi.suffix() == "xml")
    {
        this->_isHSSOnly = false;
        return this->loadXMLFile(fi.canonicalFilePath());
    }
    else if (fi.suffix() == "hss")
    {
        return this->loadHSSFile(fi.canonicalFilePath());
    }
    else
    {
        AXRError::p(new AXRError("AXRController", "Unknown file extension"))->raise();
        return false;
    }
}

bool AXRWrapper::loadXMLFile(AXRString xmlfilepath)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, AXRString("AXRWrapper: opening XML document: ").append(xmlfilepath));

    this->_isHSSOnly = false;

    AXRCore::tp & core = AXRCore::getInstance();
    if (core->getFile())
    {
        core->reset();
    }

    AXRString fullpath = "file://" + xmlfilepath;
    try
    {
        AXRBuffer::p theFile = this->getFile(fullpath);
        core->setFile(theFile);

    }
    catch (const AXRError::p &e)
    {
        e->raise();
        return false;
    }

    try
    {
        core->run();
    }
    catch (const AXRError::p &e)
    {
        e->raise();
    }
    catch (const AXRWarning::p &e)
    {
        e->raise();
    }

    this->setNeedsDisplay(true);

    return true;
}

bool AXRWrapper::reload()
{
    this->_showLayoutSteps = false;
    this->_currentLayoutStep = 0;
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;

    AXRCore::tp & core = AXRCore::getInstance();
    if (!this->_isHSSOnly)
    {
        if (core->getFile()) {
            AXRString cur_path = core->getFile()->getBasePath() + "/" + core->getFile()->getFileName();
            return this->loadXMLFile(cur_path);
        } else {
            AXRWarning::p(new AXRWarning("AXRWrapper", "no file loaded"))->raise();
            return false;
        }
    }
    else
    {
        QUrl currentPath = core->getController()->loadSheetsGet(0);
        return this->loadHSSFile(currentPath.toLocalFile());
    }
}

bool AXRWrapper::loadHSSFile(AXRString hssfilepath)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, AXRString("AXRWrapper: opening HSS document: ").append(hssfilepath));

    this->_isHSSOnly = true;
    AXRCore::tp & core = AXRCore::getInstance();
    if (core->getFile())
    {
        core->reset();
    }

    core->setFile(this->createDummyXML(hssfilepath));
    core->run();
    this->setNeedsDisplay(true);

    return true;
}

//has loaded file

bool AXRWrapper::hasLoadedFile()
{
    AXRCore::tp & core = AXRCore::getInstance();
    return core->hasLoadedFile();
}

void AXRWrapper::setShowLayoutSteps(bool value)
{
    this->_showLayoutSteps = value;
}

bool AXRWrapper::showLayoutSteps()
{
    return this->_showLayoutSteps;
}

void AXRWrapper::previousLayoutStep()
{
    if (this->_currentLayoutStep > 0)
    {
        this->_currentLayoutStep -= 1;
    }

    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;
    this->setNeedsDisplay(true);
}

void AXRWrapper::nextLayoutStep()
{
    this->_currentLayoutStep += 1;
    this->_currentLayoutTick = 0;
    this->_currentLayoutChild = 0;
    this->setNeedsDisplay(true);
}

void AXRWrapper::nextLayoutTick()
{
    this->_currentLayoutTick += 1;
}

void AXRWrapper::resetLayoutTicks()
{
    this->_currentLayoutTick = 0;
}

bool AXRWrapper::layoutStepDone()
{
    return this->_currentLayoutTick >= this->_currentLayoutStep;
}

void AXRWrapper::breakIfNeeded()
{
    bool breakvar = _currentLayoutTick >= (_currentLayoutStep - 1);

    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, AXRString("currentLayoutTick = %1, currentLayoutStep = %2, %3").arg(_currentLayoutTick).arg(_currentLayoutStep).arg(breakvar));
}

void AXRWrapper::nextLayoutChild()
{
    this->_currentLayoutChild += 1;
}

void AXRWrapper::resetLayoutChild()
{
    this->_currentLayoutChild = 0;
}

bool AXRWrapper::layoutChildDone()
{
    return this->_currentLayoutTick >= this->_currentLayoutChild + 1;
}

AXRString AXRWrapper::getPathToResources()
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

AXRString AXRWrapper::getPathToTestsFile()
{
    return _layoutTestsFilePath;
}

void AXRWrapper::executeLayoutTests(HSSObservableProperty passnull, void*data)
{
    HSSContainer::p status;
    AXRCore::tp & core = AXRCore::getInstance();
    HSSContainer::p root = core->getController()->getRoot();
    std::deque<HSSParserNode::p> arguments = *(std::deque<HSSParserNode::p>*)data;

    for (std::deque<HSSParserNode::p>::iterator it = arguments.begin(); it != arguments.end(); ++it)
    {
        HSSParserNode::p argument = *it;
        if (argument->isA(HSSFunctionTypeSel))
        {
            HSSSelFunction::p selFunction = boost::static_pointer_cast<HSSSelFunction > (argument);
            boost::any remoteValue = selFunction->evaluate();
            try
            {
                std::vector< std::vector<HSSDisplayObject::p> > selection = boost::any_cast< std::vector< std::vector<HSSDisplayObject::p> > >(remoteValue);
                std::vector<HSSDisplayObject::p> innerSelection = selection[0];
                status = HSSContainer::asContainer(innerSelection[0]);
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }
        }
    }

    AXRString filePath = this->getPathToTestsFile();
    boost::thread thrd(AXRTestThread(this, filePath, status));
    //        thrd.join();
}

AXRTestThread::AXRTestThread(AXRWrapper * wrapper, AXRString filePath, HSSContainer::p status)
{
    this->wrapper = wrapper;
    this->filePath = filePath;
    this->totalPassed = 0;
    this->totalTests = 0;
    this->status = status;
}

void AXRTestThread::operator () ()
{
    try
    {
        //load the XML file
        AXRWrapper * wrapper = this->wrapper;
        AXRCore::tp & core = AXRCore::getInstance();
        XMLParser::p parser = core->getParserXML();
        AXRString fullPath = "file://" + this->filePath;
        HSSContainer::p status = this->status;
        AXRBuffer::p testsFile = wrapper->getFile(fullPath);
        bool loadingSuccess = parser->loadFile(testsFile);
        if (loadingSuccess)
        {
            //find all the tests that need to be executed
            std::vector<std::vector<AXRString> > tests;
            AXRController::p controller = core->getController();
            HSSContainer::p root = controller->getRoot();
            const std::vector<HSSDisplayObject::p> & children = root->getChildren(true);

            for (std::vector<HSSDisplayObject::p>::const_iterator it = children.begin(); it != children.end(); ++it)
            {
                const HSSDisplayObject::p & child = *it;
                if (child->attributes.find("src") != child->attributes.end() && child->attributes.find("expect") != child->attributes.end())
                {
                    const AXRString test[2] = {child->attributes["src"], child->attributes["expect"]};
                    std::vector<AXRString>testVect(test, test + 2);
                    tests.push_back(testVect);
                    this->totalTests += 1;
                }
                else
                {
                    std_log("the test element needs to have expect and src attributes");
                }
            }
            //execute all the tests
            boost::thread_group producers;

            for (std::vector<std::vector<AXRString> >::iterator it2 = tests.begin(); it2 != tests.end(); ++it2)
            {
                AXRTestProducer prdcr(this->wrapper, testsFile->getBasePath(), *it2, &this->totalTests, &this->totalPassed, status);
                producers.create_thread(prdcr);
                boost::this_thread::yield();
            }

            producers.join_all();

            std_log("\n\nTEST RESULTS SUMMARY");
            std_log("===============================");
            std_log(AXRString("Passed %1 out of %2").arg(this->totalPassed).arg(this->totalTests));
            std_log("===============================");

        }
        else
        {
            std_log("Could not load the tests XML file");
        }

    }
    catch (const AXRError::p &e)
    {
        e->raise();
    }
    catch (const AXRWarning::p &e)
    {
        e->raise();
    }
}

// Constructor with name and the queue to use

AXRTestProducer::AXRTestProducer(AXRWrapper * wrapper, AXRString basePath, std::vector<AXRString> test, unsigned * totalTests, unsigned * totalPassed, HSSContainer::p status)
{
    this->wrapper = wrapper;
    this->basePath = basePath;
    this->test = test;
    this->totalPassed = totalPassed;
    this->totalTests = totalTests;
    this->status = status;
}

// The thread function fills the queue with data
boost::mutex AXRTestProducer::statusMutex;

void AXRTestProducer::operator () ()
{

    bool testLoaded = false;
    bool expectedLoaded = false;
    bool result = false;
    AXRString testRep;
    AXRString expectedRep;

    //load the XML
    AXRCore core = *AXRCore::getInstance();
    AXRWrapper * wrapper = this->wrapper;
    core = *AXRCore::getInstance();

    testLoaded = wrapper->loadXMLFile(this->basePath + "/" + this->test[0]);

    if (testLoaded)
    {
        AXRCore::tp & core = AXRCore::getInstance();
        AXRController::p controller = core->getController();
        HSSContainer::p root = controller->getRoot();
        core->getRender()->windowWidth = 400.;
        core->getRender()->windowHeight = 400.;
        root->recursiveReadDefinitionObjects();
        root->recursiveLayout();
        testRep = root->toString();
        //std_log(testRep);
    }
    else
    {
        std_log("could not load the test file");
    }

    //load the "expected" file
    if (testLoaded)
    {
        AXRBuffer::p expectedFile = this->wrapper->getFile("file://" + this->basePath + "/" + test[1]);
        if (!expectedFile->isValid())
        {
            std_log("could not load file with expected results");
        }
        else
        {
            expectedLoaded = true;
            expectedRep = AXRString(expectedFile->getBuffer());
            //std_log(expectedRep);
        }
    }

    //compare the two
    if (testLoaded && expectedLoaded)
    {
        result = testRep.compare(expectedRep) == 0;
    }

    if (result)
    {
        std_log("PASSED test " + test[0]);
        *this->totalPassed += 1;
        this->statusMutex.lock();
        this->status->setContentText(AXRString("Passed %1 out of %2").arg(*this->totalPassed).arg(*this->totalTests));
        this->statusMutex.unlock();
        this->wrapper->setNeedsDisplay(true);
    }
    else
    {
        std_log("FAILED test " + test[0]);
        std_log("This is the dump of the test:");
        std_log("===============================");
        std_log(testRep);
        std_log("===============================");
    }
}
