//
//  AXRXMLParser.h
//  mac
//
//  Created by Miro Keller on 02/04/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef AXR_XMLPARSER_H
#define AXR_XMLPARSER_H


#include <sys/types.h>
#include <string>
#include <expat.h>
#include "ExpatXMLParser.h"
#include "XMLParserExceptions.h"
#include "../AXRController.h"

namespace AXR {
    class XMLParser : public expatmm::ExpatXMLParser {
    public:
        //the caller is responsible for maintaining the existence of the controller
        //until after the parser is deallocated
        XMLParser(AXRController * controller, std::string filepath, std::string filename);
        virtual ~XMLParser();
        
        
    protected:
        virtual void StartElement(const XML_Char *name, const XML_Char **attrs);
        virtual void EndElement(const XML_Char *name);
        virtual ssize_t read_block(void);
        
        AXRController * controller;
        
    private:
        FILE *filehandle;
        std::string filepath;
        std::string filename;
    };
}


#endif