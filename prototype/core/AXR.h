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
 *      Last changed: 2012/01/21
 *      HSS version: 1.0
 *      Core version: 0.44
 *      Revision: 6
 *
 ********************************************************************/

//This line is deditacted to the memory of Steven P. Jobs, may he rest in peace.
//Farewell, Steve!

#ifndef AXR_H
#define AXR_H

#include "axr/AXRDebugging.h"
#include "axr/AXRRender.h"
#include "axr/AXRController.h"
#include "axr/AXRWrapper.h"
#include "xml/xml.h"
#include "hss/hss.h"
#include <cairo/cairo.h>
#include <boost/shared_ptr.hpp>

namespace AXR
{
    class AXRCore
    {
    public:
        typedef boost::shared_ptr<AXRCore> p;
        
        static AXRCore::p &getInstance();
        
        AXRCore();
        //needs to be called the first time the core is used
        void initialize(AXRWrapper * wrpr);
        virtual ~AXRCore();
        
        //set this before drawing
        void setCairo(cairo_t * cr);
        cairo_t * getCairo();
        
        //whenever you need to draw, call this
        virtual void drawInRectWithBounds(AXRRect rect, AXRRect bounds);
        //this puts everything in motion: loads xml, loads hss, applies rules, etc
        void run();
        //restart from fresh
        void reset();
        //tells wether a file has been loaded or not
        bool hasLoadedFile();
        
        AXRFile::p getFile();
        void setFile(AXRFile::p file);
        
        AXRWrapper * getWrapper();
        void setWrapper(AXRWrapper * wrapper);
        
        AXRController::p getController();
        void setController(AXRController::p controller);
        
        AXRRender::p getRender();
        void setRender(AXRRender::p parser);
        
        XMLParser::p getParserXML();
        void setParserXML(XMLParser::p parser);
        
        HSSParser::p getParserHSS();
        void setParserHSS(HSSParser::p parser);
        
    protected:
        AXRWrapper * wrapper;
        AXRRender::p render;
        AXRController::p controller;
        AXRFile::p file;
        
        XMLParser::p parserXML;
        HSSParser::p parserHSS;
        
        bool _hasLoadedFile;
    };
}

#endif //AXR_H

