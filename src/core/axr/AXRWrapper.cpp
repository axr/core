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

#include <boost/lexical_cast.hpp>
#include <QFile>
#include "AXR.h"
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
    AXRCore::tp & axr = AXRCore::getInstance();
    axr->initialize(this);
}

AXRWrapper::~AXRWrapper()
{
}

AXRFile::p AXRWrapper::getFile(std::string url)
{
    AXRFile::p ret = AXRFile::p(new AXRFile());

    std::string fileProtocolPrefix = "file://";

    if (url.substr(0, fileProtocolPrefix.length()) == fileProtocolPrefix)
    {
        std::string clean_path = url.substr(7, url.size());
        int slashpos = clean_path.rfind("/");
        ret->setFileName(clean_path.substr(slashpos + 1, clean_path.size()));
        ret->setBasePath(clean_path.substr(0, slashpos));

        ret->setBuffer(QByteArray());
        ret->setFileHandle(fopen(clean_path.c_str(), "r"));

        if (!ret->getFileHandle())
        {
            throw AXRError::p(new AXRError("AXRWrapper", "the file " + url + " doesn't exist"));
        }
        else if (ferror(ret->getFileHandle()))
        {
            throw AXRError::p(new AXRError("AXRWrapper", "the file " + url + " couldn't be read"));
        }
    }
    else
    {
        std_log("http is not implemented yet");
    }

    return ret;
}

size_t AXRWrapper::readFile(AXRFile::p theFile)
{
    QFile file(QString::fromStdString(theFile->getBasePath() + "/" + theFile->getFileName()));
    if (file.open(QIODevice::ReadOnly))
    {
        theFile->setBuffer(file.readAll());
        if (file.error() != QFile::NoError)
            theFile->setAtEndOfFile(true);
    }

    if (file.error() != QFile::NoError)
    {
        AXRError::p(new AXRError("AXRWrapper", "The file " + theFile->getFileName() + " couldn't be read"))->raise();
        return -1;
    }

    return theFile->getBufferSize();
}

void AXRWrapper::closeFile(AXRFile::p theFile)
{
    fclose(theFile->getFileHandle());
    theFile->setFileHandle(NULL);
}

void AXRWrapper::handleError(AXRError::cp theError)
{
    std_log(theError->toString());
}

bool AXRWrapper::openFileDialog(std::string &filePath)
{
    char file[FILENAME_MAX];

    std::cout << "Open file: ";
    fgets(file, FILENAME_MAX, stdin);

    if (file[strlen(file) - 1] == '\n')
    {
        file[strlen(file) - 1] = '\0';
    }

    if (file[0] == '/')
    {
        filePath = file;
    }
    else
    {
        char cwd[FILENAME_MAX];
        getcwd(cwd, FILENAME_MAX);

        std::stringstream ss;
        ss << cwd << "/" << file;
        filePath = ss.str();
    }

    std::cout << std::endl;

    return true;
}

void AXRWrapper::setNeedsDisplay(bool newValue)
{
}

AXRFile::p AXRWrapper::createDummyXML(std::string stylesheet)
{
    AXRFile::p ret = AXRFile::p(new AXRFile());
    ret->setMimeType("text/xml");
    size_t slashpos = stylesheet.rfind("/");
    if (slashpos != std::string::npos)
    {
        ret->setBasePath(stylesheet.substr(0, slashpos));
        ret->setFileName("dummy-xml-for-" + stylesheet.substr(slashpos + 1) + ".xml");
    }
    else
    {
        AXRError::p(new AXRError("AXRWrapper", "Could not find a slash in the file path"))->raise();
        return ret;
    }
    ret->setExtension("xml");
    std::string dummyXML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><?xml-stylesheet type=\"application/x-hss\" src=\"file://" + stylesheet + "\" version=\"1.0\"?><root></root>";

    ret->setFileHandle(tmpfile());

    ret->setBuffer(QString::fromStdString(dummyXML).toUtf8());

    rewind(ret->getFileHandle());

    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRWrapper: creating dummy XML file");

    return ret;
}

bool AXRWrapper::loadFile()
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRWrapper: load file by showing open file dialog");
    std::string filepath = std::string();
    bool success = this->openFileDialog(filepath);
    if (success)
    {
        return this->loadFileByPath(filepath);
    }
    else
    {
        return false;
    }
}

bool AXRWrapper::loadFileByPath(std::string filepath)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRWrapper: loading file " + filepath.substr(filepath.rfind("/") + 1, filepath.length()) + " by path");
    axr_log(AXR_DEBUG_CH_FULL_FILENAMES, filepath);

    std::string fileextension = std::string();

    fileextension = filepath.substr(filepath.rfind(".") + 1, filepath.length());
    if (fileextension == "xml")
    {
        this->_isHSSOnly = false;
        return this->loadXMLFile(filepath);
    }
    else if (fileextension == "hss")
    {
        return this->loadHSSFile(filepath);
    }
    else
    {
        AXRError::p(new AXRError("AXRController", "Unknown file extension"))->raise();
        return false;
    }
}

bool AXRWrapper::loadXMLFile(std::string xmlfilepath)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, std::string("AXRWrapper: opening XML document: ").append(xmlfilepath));

    this->_isHSSOnly = false;

    AXRCore::tp & core = AXRCore::getInstance();
    if (core->getFile())
    {
        core->reset();
    }

    std::string fullpath = "file://" + xmlfilepath;
    try
    {
        AXRFile::p theFile = this->getFile(fullpath);
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
            std::string cur_path = core->getFile()->getBasePath() + "/" + core->getFile()->getFileName();
            return this->loadXMLFile(cur_path);
        } else {
            AXRWarning::p(new AXRWarning("AXRWrapper", "no file loaded"))->raise();
            return false;
        }
    }
    else
    {
        std::string cur_path = core->getController()->loadSheetsGet(0);
        cur_path = cur_path.substr(7);
        return this->loadHSSFile(cur_path);
    }
}

bool AXRWrapper::loadHSSFile(std::string hssfilepath)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, std::string("AXRWrapper: opening HSS document: ").append(hssfilepath));

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

    std::stringstream ss;
    ss << "currentLayoutTick = " << _currentLayoutTick << ", "
        << "currentLayoutStep = " << _currentLayoutStep << ", "
        << breakvar;
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, ss.str());
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

std::string AXRWrapper::getPathToResources()
{
    // Code taken from: http://www.gamedev.net/community/forums/topic.asp?topic_id=459511
    std::string path = "";
    pid_t pid = getpid();
    char buf[20] = {0};
    sprintf(buf, "%d", pid);
    std::string _link = "/proc/";
    _link.append(buf);
    _link.append("/exe");
    char proc[512];
    int ch = readlink(_link.c_str(), proc, 512);
    if (ch != -1)
    {
        proc[ch] = 0;
        path = proc;
        std::string::size_type t = path.find_last_of("/");
        path = path.substr(0, t);
    }

    return path + std::string("/resources");
}

std::string AXRWrapper::getPathToTestsFile()
{
    std::string filePath;
    this->openFileDialog(filePath);

    return filePath;
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

    std::string filePath = this->getPathToTestsFile();
    boost::thread thrd(AXRTestThread(this, filePath, status));
    //        thrd.join();
}

AXRTestThread::AXRTestThread(AXRWrapper * wrapper, std::string filePath, HSSContainer::p status)
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
        AXRWrapper * wrapper = this->wrapper->createWrapper();
        AXRCore::tp & core = AXRCore::getInstance();
        XMLParser::p parser = core->getParserXML();
        std::string fullPath = "file://" + this->filePath;
        HSSContainer::p status = this->status;
        AXRFile::p testsFile = wrapper->getFile(fullPath);
        bool loadingSuccess = parser->loadFile(testsFile);
        if (loadingSuccess)
        {
            //find all the tests that need to be executed
            std::vector<std::vector<std::string> > tests;
            AXRController::p controller = core->getController();
            HSSContainer::p root = controller->getRoot();
            const std::vector<HSSDisplayObject::p> & children = root->getChildren(true);

            for (std::vector<HSSDisplayObject::p>::const_iterator it = children.begin(); it != children.end(); ++it)
            {
                const HSSDisplayObject::p & child = *it;
                if (child->attributes.find("src") != child->attributes.end() && child->attributes.find("expect") != child->attributes.end())
                {
                    const std::string test[2] = {child->attributes["src"], child->attributes["expect"]};
                    std::vector<std::string>testVect(test, test + 2);
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

            for (std::vector<std::vector<std::string> >::iterator it2 = tests.begin(); it2 != tests.end(); ++it2)
            {
                AXRTestProducer prdcr(this->wrapper, testsFile->getBasePath(), *it2, &this->totalTests, &this->totalPassed, status);
                producers.create_thread(prdcr);
                boost::this_thread::yield();
            }

            producers.join_all();

            std_log("\n\nTEST RESULTS SUMMARY");
            std_log("===============================");
            std_log("Passed " + boost::lexical_cast<std::string > (this->totalPassed) + " out of " + boost::lexical_cast<std::string > (this->totalTests));
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

AXRTestProducer::AXRTestProducer(AXRWrapper * wrapper, std::string basePath, std::vector<std::string> test, unsigned * totalTests, unsigned * totalPassed, HSSContainer::p status)
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
    std::string testRep;
    std::string expectedRep;

    //load the XML
    AXRCore core = *AXRCore::getInstance();
    AXRWrapper * wrapper = this->wrapper->createWrapper();
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
        AXRFile::p expectedFile = this->wrapper->getFile("file://" + this->basePath + "/" + test[1]);
        size_t fileLength = this->wrapper->readFile(expectedFile);
        if (ferror(expectedFile->getFileHandle()))
        {
            std_log("could not load file with expected results");
        }
        else
        {
            expectedLoaded = true;
            expectedRep = std::string(expectedFile->getBuffer(), fileLength);
            //std_log(expectedRep);
        }
        this->wrapper->closeFile(expectedFile);
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
        this->status->setContentText("Passed " + boost::lexical_cast<std::string > (*this->totalPassed) + " out of " + boost::lexical_cast<std::string > (*this->totalTests));
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
