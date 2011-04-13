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
 *      Last changed: 2011/04/14
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 5
 *
 ********************************************************************/

#include "XMLParser.h"
#include <iostream>
#include "../AXR.h"

using namespace AXR;

XMLParser::XMLParser(boost::shared_ptr<AXRController> controller)
: expatmm::ExpatXMLParser()
{
    this->filepath = "";
    this->filename = "";
    this->controller = controllerPointer(controller);
    this->filehandle = NULL;
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

XMLParser::~XMLParser() {
    if(this->filehandle != NULL){
        fclose(this->filehandle);
    }
    this->filepath = "";
}

bool XMLParser::loadFile(std::string filepath, std::string filename)
{
    this->filepath = filepath;
    this->filename = filename;
	this->filehandle = fopen(filepath.c_str(), "r");
	if(this->filehandle != NULL){
        expatmm::ExpatXMLParser::setReadiness(true);
    } else {
        this->setReadiness(false);
        return false;
    }
    
    return this->Parse();
}

ssize_t XMLParser::read_block(void) {
	if(this->filehandle == NULL){
        return -1;
    }
    
	size_t size = fread(this->getBuffer(), sizeof(this->getBuffer()[0]), this->getBlockSize(), this->filehandle);
	ssize_t code = (ssize_t)size;
    
	if(size < this->getBlockSize()) {
        if(feof(this->filehandle)) {
            this->setLastError(XML_ERROR_FINISHED);
            return size;
        }
        if(ferror(this->filehandle)) {
            this->setStatus(XML_STATUS_ERROR);
            this->setLastError(XML_ERROR_NO_ELEMENTS);
            throw XMLNoElementsException(this->filename, (int)XML_GetCurrentLineNumber(this->expat_parser), (int)XML_GetCurrentColumnNumber(this->expat_parser));
            fseek(this->filehandle, -size, SEEK_CUR);
        }
	}
    
	if(size == 0) {
        this->setStatus(XML_STATUS_OK);
        this->setLastError(XML_ERROR_FINISHED);
        code = -1;
	}
	return code;
}

void XMLParser::StartElement(const XML_Char *name, const XML_Char **attrs)
{
    unsigned i;
    HSSContainer::p newContainer = HSSContainer::p(new HSSContainer(std::string(name)));
    for (i = 0; attrs[i]; i += 2) {
        newContainer->attributesAdd(attrs[i], attrs[i + 1]);
    }
    
    boost::shared_ptr<AXRController>theController = this->controller.lock();
    
    theController->add(newContainer);
    //theController->add(newContainer);
    theController->setCurrent(newContainer);
}

void XMLParser::EndElement(const XML_Char *name)
{
    boost::shared_ptr<AXRController>theController = this->getController();
    HSSContainer::p parentContainer = theController->getCurrent()->getParent();
    theController->setCurrent(parentContainer);
}


//FIXME: this should be fleshed out into specialized tokenizing and parsing classes
//to better handle unexpected characters and such
void XMLParser::ProcessingInstruction(const XML_Char *target, const XML_Char *data)
{
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
            if (isspace(data[datai])){
                //ignore the whitespace
                //FIXME: set behavior according to XML spec
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
                        throw XMLMalformedProcessingInstructionException(this->filename, (int)XML_GetCurrentLineNumber(this->expat_parser), (int)XML_GetCurrentColumnNumber(this->expat_parser)+datai+strlen(target)+4);
                    }
                    
                    //std_log1(attribute);
                } else {
                    //this must be part of the value
                    temp[tempi] = data[datai];
                    tempi ++;
                }
                
            } else if(data[datai] == '"'){
                if(readingAttr){
                    throw XMLMalformedProcessingInstructionException(this->filename, (int)XML_GetCurrentLineNumber(this->expat_parser), (int)XML_GetCurrentColumnNumber(this->expat_parser)+datai+strlen(target)+4);
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
        
        if(sheetType != "application/x-hss" && sheetType != "text/hss"){
            throw XMLUnknownSheetTypeException(this->filename, (int)XML_GetCurrentLineNumber(this->expat_parser), (int)XML_GetCurrentColumnNumber(this->expat_parser));
        }
        
        //add to load later
        this->getController()->loadSheetsAdd(sheetName);
        
    } else {
        //balk
        throw XMLUnknownProcessingInstructionException(this->filename, (int)XML_GetCurrentLineNumber(this->expat_parser), (int)XML_GetCurrentColumnNumber(this->expat_parser));
    }
    delete [] temp;
}



boost::shared_ptr<AXRController> XMLParser::getController()
{
    return this->controller.lock();
}

std::string XMLParser::getFilePath(){
    return this->filepath;
}

std::string XMLParser::getFileName(){
    return this->filename;
}


