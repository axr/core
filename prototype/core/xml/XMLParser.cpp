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
 *      Last changed: 2012/03/15
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 13
 *
 ********************************************************************/

#include "XMLParser.h"
#include "../AXR.h"
#include <iostream>
#include "../axr/AXRDebugging.h"
#include "../axr/errors/errors.h"
#include "../axr/AXRController.h"

using namespace AXR;

XMLParser::XMLParser(AXRController * theController)
: expatmm::ExpatXMLParser()
{
    this->controller = theController;
}

//XMLParser::XMLParser(AXRController * controller, std::string filepath, std::string filename) : expatmm::ExpatXMLParser() {
//    this->filepath = filepath;
//    this->filename = filename;
//	this->filehandle = fopen(filepath.c_str(), "r");
//	if(this->filehandle != NULL){
//        expatmm::ExpatXMLParser::setReadiness(true);
//    } else {
//        this->setReadiness(false);
//    }
//    
//    this->controller = controller;
//}

XMLParser::~XMLParser()
{
    
}

bool XMLParser::loadFile(AXRFile::p file)
{
    this->file = file;
	if(file->fileHandle != NULL){
        expatmm::ExpatXMLParser::setReadiness(true);
    } else {
        this->setReadiness(false);
        return false;
    }
    
    bool ret;
    
    try {
        ret = this->Parse();
    } catch (AXRError::p e) {
        e->raise();
        ret = false;
    }
    
    return ret;
}

XML_Char * XMLParser::getBuffer(void)
{
    return this->file->buffer;
}

size_t XMLParser::getBlockSize(void)
{
    return this->file->bufferSize;
}

ssize_t XMLParser::read_block(void) {
    AXRFile::p file = this->file;
	if(file->fileHandle == NULL){
        return -1;
    }
    
    size_t bytesRead = AXRCore::getInstance()->getWrapper()->readFile(this->file);
	ssize_t code = (ssize_t)bytesRead;
    
	if(bytesRead <= this->getBlockSize()) {
        if(feof(file->fileHandle)) {
            this->setLastError(XML_ERROR_FINISHED);
            return bytesRead;
        }
        if(ferror(file->fileHandle)) {
            this->setStatus(XML_STATUS_ERROR);
            
        }
	}
    
	if(bytesRead == 0) {
        this->setStatus(XML_STATUS_OK);
        this->setLastError(XML_ERROR_FINISHED);
        code = -1;
	}
	return code;
}

void XMLParser::StartElement(const XML_Char *name, const XML_Char **attrs)
{
    this->controller->enterElement(std::string(name));
    unsigned i;
    for (i = 0; attrs[i]; i += 2) {
        this->controller->addAttribute(std::string(attrs[i]), std::string(attrs[i + 1]));
    }
}

void XMLParser::CharacterData(const XML_Char *s, int len)
{
    /**
     *  @todo make this better
     */
    //for now, just ignore pure newlines and tabs
    std::string tempstr = std::string(s, len);
    if(tempstr != "\n" && tempstr != "\r" && tempstr != "\t"){
        this->controller->appendContentText(tempstr);
    }
}

void XMLParser::EndElement(const XML_Char *name)
{
    this->controller->exitElement();
}

/**
 *  @todo this should be fleshed out into specialized tokenizing and parsing classes
 *  to better handle unexpected characters and such
 */
void XMLParser::ProcessingInstruction(const XML_Char *target, const XML_Char *data)
{
    if(this->controller == NULL){
        throw AXRError::p(new AXRError("XMLParser", "The controller was not set on the XML parser"));
    }
    
    std::string instructionName = std::string(target);
    //maximum size of temp is the same as data (+1 for the \0)
    XML_Char * temp = new XML_Char[strlen(data)+1];
    std::string attribute;
    std::string content;
    std::string sheetType;
    std::string sheetName;
    
    bool readingAttr = true;
    
    unsigned datai = 0;
    unsigned tempi = 0;
    security_brake_init();
    if(instructionName == "xml-stylesheet"){
        while (data[datai] != '\0') {
            if (readingAttr && isspace(data[datai])){
                //ignore the whitespace
                /**
                 *  @todo set behavior according to XML spec
                 */
            } else if(data[datai] == '=') {
                if(readingAttr){
                    //finished reading the attribute
                    readingAttr = false;
                    temp[tempi] = '\0';
                    attribute = std::string(temp);
                    tempi=0;
                    //we now expect a double quote
                    datai++;
                    if(data[datai] != '"'){
                        throw AXRError::p(new AXRError("XMLParser", "Malformed processing instruction", this->file->getFileName(), (int)XML_GetCurrentLineNumber(this->expat_parser), (int)XML_GetCurrentColumnNumber(this->expat_parser)+datai+strlen(target)+4));
                    }
                    
                    //std_log1(attribute);
                } else {
                    //this must be part of the value
                    temp[tempi] = data[datai];
                    tempi ++;
                }
                
            } else if(data[datai] == '"'){
                if(readingAttr){
                    throw AXRError::p(new AXRError("XMLParser", "Malformed processing instruction", this->file->getFileName(), (int)XML_GetCurrentLineNumber(this->expat_parser), (int)XML_GetCurrentColumnNumber(this->expat_parser)+datai+strlen(target)+4));
                    
                } else {
                    readingAttr = true;
                    temp[tempi] = '\0';
                    content = std::string(temp);
                    //std_log1(attribute);
                    if(attribute == "type"){
                        sheetType = content;
                    } else if(attribute == "src"){
                        sheetName = content;
                    }
                    tempi=0;
                }
            } else {
                temp[tempi] = data[datai];
                tempi ++;
            }
            datai++;
            security_brake();
        }
        
        if(sheetType == "application/x-hss" || sheetType == "text/hss"){
            //add to load later
            this->controller->loadSheetsAdd(sheetName);
        } else if (sheetType == "application/xsl"){
            //ignore silently
        } else {
            //warn
            AXRWarning::p(new AXRWarning("XMLParser", "Ignoring stylesheet of unknown type", this->file->getFileName(), (int)XML_GetCurrentLineNumber(this->expat_parser), (int)XML_GetCurrentColumnNumber(this->expat_parser)+1))->raise();
        }
    }
    
    delete [] temp;
}


std::string XMLParser::getFilePath(){
    return this->file->basePath + "/" + this->file->getFileName();
}

std::string XMLParser::getFileName(){
    return this->file->getFileName();
}




