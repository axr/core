/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MA:MMD                                                         
 *            :: VD
 *           ::  º                                                         
 *          ::                                                              
 *         ::   NZ      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       6MMZ. MMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      6MDAMMDJMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      $$  MMMMMC         ___MMMM     VMMMMMMM6       MMMM                   
 *     CMM  MMMMMMM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *    ::MMM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *   ::  MMMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        MMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       MMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      MMMM MMMM                                                           
 *     AMMM  .MMM                                         
 *     MMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     MMM    MMA       ====================================                              
 *     DMN    MM                               
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
 *      Last changed: 2011/04/09
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 2
 *
 ********************************************************************/

#ifndef AXR_XMLPARSER_H
#define AXR_XMLPARSER_H


#include <sys/types.h>
#include <string>
#include <expat.h>
#include "ExpatXMLParser.h"
#include "XMLParserExceptions.h"
#include "../axr/AXRController.h"

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