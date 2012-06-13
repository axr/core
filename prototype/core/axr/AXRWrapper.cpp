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
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2012/06/11
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 3
 *
 ********************************************************************/

#include "AXRWrapper.h"
#include "../AXR.h"

using namespace AXR;

AXRWrapper::AXRWrapper(std::string str)
{
    this->_isHSSOnly = false;
    AXRCore::tp & axr = AXRCore::getInstance();
    axr->initialize(this);
}

AXRWrapper::~AXRWrapper()
{
    
}

AXRFile::p AXRWrapper::createDummyXML(std::string stylesheet)
{
    AXRFile::p ret = AXRFile::p(new AXRFile());
    ret->bufferSize = 10240;
    ret->mimeType = "text/xml";
    size_t slashpos = stylesheet.rfind("/");
    if(slashpos != std::string::npos){
        ret->basePath = stylesheet.substr(0, slashpos);
        ret->setFileName(stylesheet.substr(slashpos+1));
    } else {
        AXRError::p(new AXRError("AXRWrapper", "Could not find a slash in the file path"))->raise();
        return ret;
    }
    ret->extension = "xml";
    std::string dummyXML = "<?xml version=\"1.0\" encoding=\"UTF-8\"?><?xml-stylesheet type=\"application/x-hss\" src=\"file://"+stylesheet+"\" version=\"1.0\"?><root></root>";
    
    ret->fileHandle = tmpfile();
    ret->buffer = new char[ret->bufferSize];
    dummyXML.copy(ret->buffer, ret->bufferSize, 0);
    
    size_t bfsz = dummyXML.size();
    ret->fileSize = ret->bufferSize = bfsz;
    
    fwrite(ret->buffer, sizeof(ret->buffer[0]), ret->bufferSize, ret->fileHandle);
    rewind(ret->fileHandle);
    
    return ret;
}

bool AXRWrapper::loadFile()
{
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
    std::string filename = std::string();
    std::string fileextension = std::string();

    fileextension = filepath.substr(filepath.rfind(".") + 1, filepath.length());
    if(fileextension == "xml"){
        this->_isHSSOnly = false;
        return this->loadXMLFile(filepath);
    } else if (fileextension == "hss") {
        this->_isHSSOnly = true;
        return this->loadHSSFile(filepath);
    } else {
        AXRError::p(new AXRError("AXRController", "Unknown file extension"))->raise();
        return false;
    }
}

bool AXRWrapper::loadXMLFile(std::string xmlfilepath)
{
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
    
    core->run();
    this->setNeedsDisplay(true);
    
    return true;
}

bool AXRWrapper::reload()
{
    AXRCore::tp & core = AXRCore::getInstance();
    std::string cur_path = core->getFile()->basePath+"/"+core->getFile()->getFileName();
    std::string fileextension = cur_path.substr(cur_path.rfind(".") + 1, cur_path.length());
    core->reset();
    if(fileextension == "xml"){
        this->_isHSSOnly = false;
        return this->loadXMLFile(cur_path);
    } else if (fileextension == "hss") {
        this->_isHSSOnly = true;
        return this->loadHSSFile(cur_path);
    } else {
        AXRError::p(new AXRError("AXRController", "Unknown file extension"))->raise();
        return false;
    }
}

bool AXRWrapper::loadHSSFile(std::string hssfilepath)
{
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
