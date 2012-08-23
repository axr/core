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
 ********************************************************************/

//This line is deditacted to the memory of Steven P. Jobs, may he rest in peace.
//Farewell, Steve!



/**
 *  @mainpage AXR Prototype Documentation Overview
 *   
 *  Welcome to the documentation of the AXR prototype! It was automatically generated from the
 *  JavaDoc-style comment blocks that are in the header (.h) files, with a tool called Doxygen.
 *  
 *  The goal of this document is to provide an overview of how the code is structured, how the
 *  different classes relate to each other and, in general terms, how text files containing
 *  source code is transformed into pixels on screen at the end.
 *  
 *  Most of the code is C++, which was chosen because of wide interoperability and existence of
 *  libraries and resources, as well as the native speed and fine grained control when flipping
 *  bits a rendering engine needs.
 *  
 *  We manage the code via GitHub, as well as the issues for bugs and stuff like that. If you'd
 *  like to contribute, you should create a GitHub account if you don't have one already and
 *  then click the "Fork" button on https://github.com/AXR/Prototype. You then need to download,
 *  or "clone" the repository to your computer (install git if you don't have it already).
 *  Execute the following commands in the command line:
 *  
 *  cd path/to/directory
 *  git clone https://github.com/yourusername/Prototype.git
 *
 *  @section folders Diving into the folder structure
 *  
 *  Let's have a look at what's inside the prototype folder:
 *  
 *  - core: this is the core library of the rendering engine
 *  - app: prototype app (currently a Cocoa app on OS X and SDL based on other platforms)
 *  - tests: this is a bunch of XML and HSS files that were used when developing the library
 *  
 *  The library follows a core/wrapper pattern that allows to embed it wherever needed, it's
 *  just a matter of creating a platform specific wrapper. On OS X you can use Interface Builder
 *  and make a custom view to be of the AXRView class and interact with it directly.
 *  
 *  Now let's focus on the core. In it we find the AXR.h file that contains the AXRCore class,
 *  which encapsulates the library, and the following folders:
 *  
 *  - axr: Here we find the guts of the library which is not related to one specific language
 *  but manage stuff across the rendering engine.
 *  - hss: In here we find the HSS parser, the parser nodes and all the object types.
 *  - os: This is the place for the platform specific wrappers that abstract calls to the
 *  operating system.
 *  - xml: In here we find the XML parser
 *
 *  @section parsing Parsing the content and getting up and runnig
 *  
 *  So you will probably be wondering, where do we start? This of course depends on how you
 *  are embedding it, but in a general sense, you (or in case of OS X, the provided NSView
 *  subclass) will create a platform specific wrapper and then tell it to load a file either
 *  via a file open dialog or by pathname. The implementation of this method is in the
 *  abstract superclass in axr/AXRWrapper.cpp. When the path is to a XML file, it gets the
 *  file (implemented by the platform specific wrapper) and sets it on the core, then calls
 *  the run() method on it. If it is to an HSS file, it will create a dummy XML file on the
 *  fly, which contains only one element called "root".
 *  
 *  So now let's jump to ../AXR.cpp, and have a look at the run() method. It gives the XML
 *  file to the XML parser. We currently use Expat, and the file XMLParser.cpp provides
 *  the glue code in form of callbacks for when the elements are parsed, adding them on
 *  axr/AXRController.cpp using enterElement(), addAttribute(), appendContentText() and
 *  exitElement().
 *  
 *  When it finds an XML instruction it will parse it and if it is a link to a HSS stylesheet
 *  it calls loadSheetsAdd() on the controller to take note. Which brings us back to
 *  AXRCore::run(), where we now pass all the stylesheets that were found to the HSS parser.
 *  
 *  Similarly to how the XMl parser does, the hss/parsing/HSSParser.cpp calls rulesAdd(),
 *  parserTreeAdd(), and objectTreeAdd() on the controller, and then we loop over the found
 *  rules and recursively match them to the content tree. On each of the matched elements,
 *  setNeedsRereadRules(true) is called, so that before the next (and first) time we draw, the
 *  properties from the rules are passed into each "definition object setter" (those which look
 *  like setD<propertynamehere>()), where they will be stored according to the value type that
 *  is passed.
 *  
 *  @section drawing Drawing on screen
 *  
 *  Currently for drawing we are using Cairo and Pango for text layout, but we have plans to
 *  switch to a library that allows more fine grained control, like AntiGrain Geometry or its
 *  successor Fog do, this is still under discussion.
 *
 *  When the OS asks for drawing, it is the duty of the wrapper to tell the core to draw in a given
 *  rectangle (the current implementation still always redraws the whole screen, though), which will then
 *  get get to the AXRRender class. This one will check if the window size has changed, and if so change
 *  the width and height of the root object. It will then tell the root element to recursively read the
 *  "definition objects", which are the HSSParserNodes that hold the values (inside property definitions,
 *  which in turn are inside the rules that are associated with the elements) into the instance
 *  variables that hold the actual calculated values that will determine how and where the stuff is rendered.
 *  
 *  Next, it tells it to recursively layout, which is a process in which display objects arrange the things
 *  that are inside, such as text in the case of HSSTextBlock elements or other display objects in the case of
 *  HSSContainer.
 *  
 *  Then it will regenerate all the surfaces as needed and finally it will tell it to draw, also as a
 *  recursive operation, passing the cairo object that holds the drawing context in which we will finally composite all
 *  the individual surfaces of the objects.
 *  
 *  In the process of drawing, each object type which draws (display objects) implements and handles
 *  all the properties inside to calculate the visual rendering, each generating bitmap surfaces for the
 *  background, borders, etc, and finally compositing it all together.
 *
 *  This is all done in a pretty straightforward an naïve way, and should receive quite a lot of facelifting
 *  and perfomance improvements in the near future.
 *
 *  @section diagram Diagram of the structure of the library
 *  
 *  In the following diagram you can see a graphical representation of how the AXR core library is structured:
 *  @image html library_structure.jpg
 */



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
#include "hss/various/HSSCallback.h"
#include <boost/thread/tss.hpp>

namespace AXR
{
    /**
     * @brief   A resolution independent point.
     */
    struct HSSPoint {
        long double x; /**< the location of the point in horizontal dimension. */
        long double y; /**< the location of the point in vertical dimension. */
    };
    
    /**
     * @brief   Represents a size measure.
     */
    struct HSSSize {
        long double width; /**< measure in the horizontal dimension. */
        long double height; /**< measure in the vertical dimension. */
    };
    
    
    /**
     * @brief   A basic rectangle.
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
         *  The thread specific pointer to AXRCore objects
         */
        typedef boost::thread_specific_ptr<AXRCore> tp;
        
        /**
         *  Whenever you need to access the core object use this method,
         *  as AXRCore is a per-thread-singleton, where 1 instance is created for each thread that uses AXRCore.
         *  @return A thread specific pointer to core for this thread
         */
        static AXRCore::tp &getInstance();
        
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
        
        /**
         *  When a function name in HSS is checked for existence, this tells wether
         *  it corresponds to a custom function that has been registered on the core.
         *  @param name     A string containing the function name.
         *  @return Wether the string is registered as a custom function
         */
        bool isCustomFunction(std::string name);
        
        /**
         *  Allows for registering a custom function name. @todo explain more
         *  @param name     A string containing the function name.
         *  @param fn       A pointer to the callback that encapsulates the
         *                  c++ function that will be called.
         */
        void registerCustomFunction(std::string name, HSSCallback* fn);
        void evaluateCustomFunction(std::string name, void* data);

    protected:
        AXRWrapper * wrapper;
        AXRRender::p render;
        AXRController::p controller;
        AXRFile::p file;
        
        XMLParser::p parserXML;
        HSSParser::p parserHSS;
        
        bool _hasLoadedFile;
        boost::unordered_map<std::string, HSSCallback*> _customFunctions;
    };
}

#endif //AXR_H

