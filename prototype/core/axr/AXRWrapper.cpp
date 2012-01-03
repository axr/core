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
 *      Last changed: 2012/01/03
 *      HSS version: 1.0
 *      Core version: 0.44
 *      Revision: 1
 *
 ********************************************************************/

#include "AXRWrapper.h"
#include "../AXR.h"

using namespace AXR;

AXRWrapper::AXRWrapper()
{
    this->_isHSSOnly = false;
    AXRCore::p axr = AXRCore::p(new AXRCore(this));
    this->setCore(axr);
}

AXRWrapper::~AXRWrapper()
{
    
}

boost::shared_ptr<AXRCore> AXRWrapper::getCore() { return this->core; }
void AXRWrapper::setCore(boost::shared_ptr<AXRCore> xcr) { this->core = xcr; }

AXRFile::p AXRWrapper::createDummyXML(std::string stylesheet)
{
    AXRFile::p ret = AXRFile::p(new AXRFile());
    ret->fileName = "dummyXML";
    ret->mimeType = "text/xml";
    size_t found = stylesheet.rfind("/");
    if(found != std::string::npos){
        ret->basePath = stylesheet.substr(0, found+1);
    }
    ret->extension = "xml";
    std::string dummyXML = "\
<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<?xml-stylesheet type=\"application/x-hss\" src=\""+stylesheet+"\" version=\"1.0\"?>\
<root></root>\
";
    ret->bufferSize = dummyXML.size();
    ret->buffer = new char[ret->bufferSize];
    dummyXML.copy(ret->buffer, ret->bufferSize, 0);
    ret->bufferSize = ret->fileSize = dummyXML.size();
    
    return ret;
}


bool AXRWrapper::loadFile()
{
    std::string filepath = std::string();
    std::string filename = std::string();
    std::string fileextension = std::string();
    
    bool success = this->openFileDialog(filepath);
    if(success){
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
        
    } else {
        return false;
    }
}

bool AXRWrapper::loadXMLFile(std::string xmlfilepath)
{
    if (!this->core->getFile()) {
        std::string fullpath = "file://"+xmlfilepath;
        try {
            AXRFile::p theFile = this->getFile(fullpath);
            this->core->setFile(theFile);
            
        } catch (AXRError::p e) {
            e->raise();
            return false;
        }
    }
    
    this->core->run();
    
    return true;
}

bool AXRWrapper::reload()
{
    std::string cur_path = "file://"+this->core->getFile()->basePath+"/"+this->core->getFile()->fileName;
    this->core->reset();
    this->core->setFile(this->getFile(cur_path));
    return this->loadXMLFile(cur_path);
}

bool AXRWrapper::loadHSSFile(std::string hssfilepath)
{
    return false;
}

//has loaded file
bool AXRWrapper::hasLoadedFile()
{
    return this->core->hasLoadedFile();
}
