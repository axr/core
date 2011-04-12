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
 *      Last changed: 2011/04/12
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 4
 *
 ********************************************************************/

#ifndef AXR_XMLPARSER_H
#define AXR_XMLPARSER_H


#include <sys/types.h>
#include <string>
#include <expat.h>
#include "ExpatXMLParser.h"
#include "XMLParserExceptions.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace AXR {
    class AXRController;
    
    class XMLParser : public expatmm::ExpatXMLParser {
    public:
        
        typedef boost::shared_ptr<XMLParser> p;
        typedef boost::weak_ptr<AXRController> controllerPointer;
        
        XMLParser(boost::shared_ptr<AXRController> controller);
        //the caller is responsible for maintaining the existence of the controller
        //until after the parser is deallocated
        //XMLParser(AXRController * controller, std::string filepath, std::string filename);
        virtual ~XMLParser();
        
        bool loadFile(std::string filepath, std::string filename);
        
    protected:
        virtual ssize_t read_block(void);
        virtual void StartElement(const XML_Char *name, const XML_Char **attrs);
        virtual void EndElement(const XML_Char *name);
        virtual void ProcessingInstruction(const XML_Char *target, const XML_Char *data);
        
        controllerPointer controller;
        
        boost::shared_ptr<AXRController> getController();
        
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