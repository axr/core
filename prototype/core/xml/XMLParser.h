/**********************************************************
 *
 *      d8888 Y88b   d88P 8888888b.  
 *      d88888  Y88b d88P  888   Y88b 
 *      d88P888   Y88o88P   888    888 
 *      d88P 888    Y888P    888   d88P 
 *      88P  888    d888b    8888888P"  
 *      d88P   888   d88888b   888 T88b   
 *      d8888888888  d88P Y88b  888  T88b  
 *      d88P     888 d88P   Y88b 888   T88b 
 *      
 *      ARBITRARY·······XML········RENDERING
 *      ====================================
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
 **********************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 **********************************************************/

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
        virtual ssize_t read_block(void);
        virtual void StartElement(const XML_Char *name, const XML_Char **attrs);
        virtual void EndElement(const XML_Char *name);
        virtual void ProcessingInstruction(const XML_Char *target, const XML_Char *data);
        
        AXRController * controller;
        
    private:
        FILE *filehandle;
        std::string filepath;
        std::string filename;
        
        XML_Char currentPIChar;
        void readPIChar(XML_Char nextChar);
        void readNextPIAttr();
        
    };
}


#endif