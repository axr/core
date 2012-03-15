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
 *      Last changed: 2012/03/14
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 7
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
    /**
     * A resolution independent point.
     */
    struct HSSPoint {
        long double x; /**< the location of the point in horizontal dimension. */
        long double y; /**< the location of the point in vertical dimension. */
    };
    
    /**
     * Represents a size measure.
     */
    struct HSSSize {
        long double width; /**< measure in the horizontal dimension. */
        long double height; /**< measure in the vertical dimension. */
    };
    
    
    /**
     * A basic rectangle.
     */
    struct HSSRect {
        HSSPoint origin; /**< Where the rectangle starts. This point is on the top left corner of the rectangle. */
        HSSSize size; /**< The dimensions of the rectangle. */
    };
    
    
    /**
     *  @brief This is the object that encapsulates the core library.
     *  
     *  Do not instantiate it directly. Instead, call the getInstance() method.
     */
    class AXRCore
    {
    public:
        /**
         *  The shared pointer to AXRCore objects
         */
        typedef boost::shared_ptr<AXRCore> p;
        
        /**
         *  Whenever you need to access the core object use this method,
         *  as AXRCore is a singleton.
         *  @return a shared pointer to the singleton instance.
         */
        static AXRCore::p &getInstance();
        
        /**
         *  Creates a new AXRCore object.
         *  @warning Do not call directly. Call getInstance() instead.
         */
        AXRCore();
        
        /**
         *  Initializes all default values.
         *  @param  wrpr    The platform-specific wrapper that is embedding the core library.
         *  This is a c++ pointer, not a shared (boost) one. Since the embedder owns the core,
         *  no memory management is done whatsoever.
         *  @warning Needs to be called before the first time the core is used.
         *  @see AXRWrapper
         */
        void initialize(AXRWrapper * wrpr);
        /**
         *  Destroys the object
         */
        virtual ~AXRCore();
        
        /**
         *  Sets up the Cairo object that will do the final compositing.
         *  @param cr A pointer to a cairo_t
         *  @warning This MUST be set before drawing.
         */
        void setCairo(cairo_t * cr);
        /**
         *  @return the Cairo object that is used to do the final compositing.
         */
        cairo_t * getCairo();
        
        /**
         *  Main drawing function.
         *  @param rect     The rectangle that should be drawn.
         *  @param bounds   The bounding box of the complete view.
         *  @warning The current implementation (0.441) always draws the entire bounds,
         *  the rect parameter is actually ignored. This will be fixed in the near future.
         */
        virtual void drawInRectWithBounds(AXRRect rect, AXRRect bounds);
        /**
         *  After everything is set up, this puts everything in motion:
         *  loads xml, loads hss, applies rules, etc
         */
        void run();
        /**
         *  Reset everything to default values, so that we can start
         *  from fresh.
         */
        void reset();
        /**
         *  @return tells wether a file has been loaded or not
         */
        bool hasLoadedFile();
        
        /**
         *  @return returns the current AXRFile that is opened. This can be either
         *  an XML or an HSS file.
         */
        AXRFile::p getFile();
        /**
         *  Sets the current AXRFile to be used as the base document.
         *  @param file     The AXRFile to store for later use.
         */
        void setFile(AXRFile::p file);
        /**
         *  @return a c++ pointer to the AXRWrapper
         */
        AXRWrapper * getWrapper();
        /**
         *  Sets the AXRWrapper that is embedding the core library.
         *  @param wrapper  A c++ pointer to the AXRWrapper subclass instance. Since
         *  the embedder owns the core, no memory management is done whatsoever.

         */
        void setWrapper(AXRWrapper * wrapper);
        /**
         *  Getter for shared pointer to the AXRController that is managing the
         *  object tree.
         *  @return Shared pointer to the AXRController
         */
        AXRController::p getController();
        /**
         *  Setter for the AXRController that is managing the object tree.
         *  @param controller   A shared pointer to the controller.
         */
        void setController(AXRController::p controller);
        /**
         *  Getter for shared pointer to the AXRRender object, which is an abstract interface
         *  to the computer's rendering system.
         *  @return A shared pointer to the AXRRender
         */
        AXRRender::p getRender();
        /**
         *  Setter for the AXRRender that will be used.
         *  @param render   A shared pointer to the render.
         */
        void setRender(AXRRender::p render);
        /**
         *  Getter for the shared pointer to the XML parser. 
         *  @return A shared pointer to the XML parser.
         */
        XMLParser::p getParserXML();
        /**
         *  Setter for the XMLParser that will be used to parse XML files.
         *  @param parser   A shared pointer to the XML parser.
         */
        void setParserXML(XMLParser::p parser);
        
        /**
         *  Getter for the shared pointer to the HSS parser. 
         *  @return A shared pointer to the HSS parser.
         */
        HSSParser::p getParserHSS();
        /**
         *  Setter for the XMLParser that will be used to parse HSS files.
         *  @param parser   A shared pointer to the HSS parser.
         */
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

//doxygen support:
/** @defgroup typeEnums Type enumerations */

#endif //AXR_H

