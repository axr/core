//
//  AXRXMLParser.cpp
//  mac
//
//  Created by Miro Keller on 02/04/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "XMLParser.h"
#include <iostream>
#include "AXR.h"

using namespace AXR;
XMLParser::XMLParser(AXRController * controller, std::string filepath, std::string filename) : expatmm::ExpatXMLParser() {
    this->filepath = filepath;
    this->filename = filename;
	this->filehandle = fopen(filepath.c_str(), "r");
	if(this->filehandle != NULL){
        expatmm::ExpatXMLParser::setReadiness(true);
    } else {
        this->setReadiness(false);
    }
    
    this->controller = controller;
}

XMLParser::~XMLParser() {
    fclose(this->filehandle);
    this->filepath = "";
    this->controller = NULL;
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
    HSSContainer * newContainer = new HSSContainer(std::string(name));
    for (i = 0; attrs[i]; i += 2) {
        newContainer->attributesAdd(attrs[i], attrs[i + 1]);
    }
    this->controller->add(newContainer);
    this->controller->setCurrent(newContainer);
}

void XMLParser::EndElement(const XML_Char *name)
{
    this->controller->setCurrent(this->controller->getCurrent()->getParent());
}