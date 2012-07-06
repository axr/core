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
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
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

#include "AXRWrapper.h"
#include "../AXR.h"
#include <boost/lexical_cast.hpp>
#include "../hss/parsing/HSSFunction.h"
#include "../hss/parsing/HSSSelFunction.h"

using namespace AXR;

AXRWrapper::AXRWrapper()
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRWrapper: creating AXRWrapper");
    this->_isHSSOnly = false;
    this->_hasLoadedFile = false;
    AXRCore::tp & axr = AXRCore::getInstance();
    axr->initialize(this);
}

AXRWrapper::~AXRWrapper()
{
    
}

AXRFile::p AXRWrapper::createDummyXML(std::string stylesheet)
{
    AXRFile::p ret = AXRFile::p(new AXRFile());
    ret->setBufferSize(10240);
    ret->setMimeType("text/xml");
    size_t slashpos = stylesheet.rfind("/");
    if(slashpos != std::string::npos){
        ret->setBasePath(stylesheet.substr(0, slashpos));
        ret->setFileName("dummy-xml-for-"+stylesheet.substr(slashpos+1)+".xml");
    } else {
        AXRError::p(new AXRError("AXRWrapper", "Could not find a slash in the file path"))->raise();
        return ret;
    }
    ret->setExtension("xml");
    std::string dummyXML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><?xml-stylesheet type=\"application/x-hss\" src=\"file://"+stylesheet+"\" version=\"1.0\"?><root></root>";
    
    ret->setFileHandle(tmpfile());
    char * buffer = new char[ret->getBufferSize()];
    ret->setBuffer(buffer);
    dummyXML.copy(ret->getBuffer(), ret->getBufferSize(), 0);
    
    size_t bfsz = dummyXML.size();
    ret->setFileSize(bfsz);
    ret->setBufferSize(bfsz);
    
    fwrite(buffer, sizeof(buffer[0]), ret->getBufferSize(), ret->getFileHandle());
    rewind(ret->getFileHandle());
    
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRWrapper: creating dummy XML file");
    
    return ret;
}

bool AXRWrapper::loadFile()
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRWrapper: load file by showing open file dialog");
    std::string filepath = std::string();
    bool success = this->openFileDialog(filepath);
    if(success){
        return this->loadFileByPath(filepath);
    } else {
        return false;
    }
}

bool AXRWrapper::loadFileByPath(std::string filepath)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, "AXRWrapper: loading file "+filepath.substr(filepath.rfind("/") + 1, filepath.length())+" by path");
    axr_log(AXR_DEBUG_CH_FULL_FILENAMES, filepath);
    
    std::string filename = std::string();
    std::string fileextension = std::string();

    fileextension = filepath.substr(filepath.rfind(".") + 1, filepath.length());
    if(fileextension == "xml"){
        this->_isHSSOnly = false;
        return this->loadXMLFile(filepath);
    } else if (fileextension == "hss") {
        return this->loadHSSFile(filepath);
    } else {
        AXRError::p(new AXRError("AXRController", "Unknown file extension"))->raise();
        return false;
    }
}

bool AXRWrapper::loadXMLFile(std::string xmlfilepath)
{
    axr_log(AXR_DEBUG_CH_OVERVIEW, std::string("AXRWrapper: opening XML document: ").append(xmlfilepath));
    
    this->_isHSSOnly = false;
    
    AXRCore::tp & core = AXRCore::getInstance();
    if (core->getFile()) {
        core->reset();
    }
    
    std::string fullpath = "file://"+xmlfilepath;
    try {
        AXRFile::p theFile = this->getFile(fullpath);
        core->setFile(theFile);
        
    } catch (AXRError::p e) {
        e->raise();
        return false;
    }
    
    try {
        core->run();
    } catch (AXRError::p e) {
        e->raise();
    } catch (AXRWarning::p e) {
        e->raise();
    }
    
    this->setNeedsDisplay(true);
    
    return true;
}

bool AXRWrapper::reload()
{
    AXRCore::tp & core = AXRCore::getInstance();
    if(!this->_isHSSOnly){
        std::string cur_path = core->getFile()->getBasePath()+"/"+core->getFile()->getFileName();
        return this->loadXMLFile(cur_path);
    } else {
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
    if (core->getFile()) {
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

void AXRWrapper::executeLayoutTests(HSSObservableProperty passnull, void*data)
{
    HSSContainer::p status;
    AXRCore::tp & core = AXRCore::getInstance();
    HSSContainer::p root = core->getController()->getRoot();
    std::deque<HSSParserNode::p> arguments = *(std::deque<HSSParserNode::p>*)data;
    std::deque<HSSParserNode::p>::iterator it;
    for (it=arguments.begin(); it!=arguments.end(); it++) {
        HSSParserNode::p argument = *it;
        if(argument->isA(HSSParserNodeTypeFunctionCall)){
            HSSFunction::p theFunction = boost::static_pointer_cast<HSSFunction>(argument);
            if(theFunction->isA(HSSFunctionTypeSel)){
                HSSSelFunction::p selFunction = boost::static_pointer_cast<HSSSelFunction>(theFunction);
                std::vector< std::vector<HSSDisplayObject::p> > selection = *(std::vector< std::vector<HSSDisplayObject::p> >*)selFunction->evaluate();
                std::vector<HSSDisplayObject::p> innerSelection = selection[0];
                status = HSSContainer::asContainer(innerSelection[0]);
            }
        }
    }
    
    //ask for a file path
    std::string filePath;
	bool result = this->openFileDialog(filePath);
    if(result){
        boost::thread thrd(AXRTestThread(this, filePath, status));
        //        thrd.join();
        
    }
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
    try {
        //load the XML file
        AXRWrapper * wrapper = this->wrapper->createWrapper();
        AXRCore::tp & core = AXRCore::getInstance();
        XMLParser::p parser = core->getParserXML();
        std::string fullPath = "file://"+this->filePath;
        HSSContainer::p status = this->status;
        AXRFile::p testsFile = wrapper->getFile(fullPath);
        bool loadingSuccess = parser->loadFile(testsFile);
        if (loadingSuccess) {
            
            //find all the tests that need to be executed
            std::vector<std::vector<std::string> > tests;
            AXRController::p controller = core->getController();
            HSSContainer::p root = controller->getRoot();
            const std::vector<HSSDisplayObject::p> & children = root->getChildren(true);
            std::vector<HSSDisplayObject::p>::const_iterator it;
            
            for (it=children.begin(); it!=children.end(); it++) {
                const HSSDisplayObject::p & child = *it;
                if(child->attributes.find("src") != child->attributes.end() && child->attributes.find("expect") != child->attributes.end()){
                    const std::string test[2] = {child->attributes["src"], child->attributes["expect"]};
                    std::vector<std::string>testVect(test, test+2);
                    tests.push_back(testVect);
                    this->totalTests += 1;
                } else {
                    std_log("the test element needs to have expect and src attributes");
                }
            }
            //execute all the tests
            boost::thread_group producers;
            std::vector<std::vector<std::string> >::iterator it2;
            for (it2=tests.begin(); it2!=tests.end(); it2++) {
                AXRTestProducer prdcr(this->wrapper, testsFile->getBasePath(), *it2, &this->totalTests, &this->totalPassed, status );
                producers.create_thread(prdcr);
                boost::this_thread::yield();
            }
            
            producers.join_all();
            
            std_log("\n\nTEST RESULTS SUMMARY");
            std_log("===============================");
            std_log("Passed "+boost::lexical_cast<std::string>(this->totalPassed)+" out of "+boost::lexical_cast<std::string>(this->totalTests));
            std_log("===============================");
            
        } else {
            std_log("Could not load the tests XML file");
        }
        
    } catch (AXRError::p e) {
        e->raise();
    } catch (AXRWarning::p e) {
        e->raise();
    }
}

// Constructor with name and the queue to use
AXRTestProducer::AXRTestProducer(AXRWrapper * wrapper, std::string basePath, std::vector<std::string> test, unsigned * totalTests, unsigned * totalPassed, HSSContainer::p status)
{
    this->wrapper = wrapper;
    this->basePath = basePath;
    this->test=test;
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
    
    testLoaded = wrapper->loadXMLFile(this->basePath+"/"+this->test[0]);
    
    if(testLoaded){
        AXRCore::tp & core = AXRCore::getInstance();
        AXRController::p controller = core->getController();
        HSSContainer::p root = controller->getRoot();
        core->getRender()->windowWidth = 400.;
        core->getRender()->windowHeight = 400.;
        root->recursiveReadDefinitionObjects();
        root->recursiveLayout();
        testRep = root->toString();
        //std_log(testRep);
    } else {
        std_log("could not load the test file");
    }
    
    //load the "expected" file
    if(testLoaded){
        AXRFile::p expectedFile = this->wrapper->getFile("file://"+this->basePath+"/"+test[1]);
        size_t fileLength = this->wrapper->readFile(expectedFile);
        if (ferror(expectedFile->getFileHandle())) {
            std_log("could not load file with expected results");
        } else {
            expectedLoaded = true;
            expectedRep = std::string(expectedFile->getBuffer(), fileLength);
            //std_log(expectedRep);
        }
        this->wrapper->closeFile(expectedFile);
    }
    
    //compare the two
    if(testLoaded && expectedLoaded){
        result = testRep.compare(expectedRep) == 0;
    }
    
    if (result) {
        std_log("PASSED test "+test[0]);
        *this->totalPassed += 1;
        this->statusMutex.lock();
        this->status->setContentText("Passed "+boost::lexical_cast<std::string>(*this->totalPassed)+" out of "+boost::lexical_cast<std::string>(*this->totalTests));
        this->statusMutex.unlock();
        this->wrapper->setNeedsDisplay(true);
    } else {
        std_log("FAILED test "+test[0]);
        std_log("This is the dump of the test:");
        std_log("===============================");
        std_log(testRep);
        std_log("===============================");
    }
}
