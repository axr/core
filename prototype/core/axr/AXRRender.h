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

#ifndef AXRRENDER_H
#define AXRRENDER_H

#include "../hss/parsing/HSSNumberConstant.h"
#include <cairo/cairo.h>
#include <boost/shared_ptr.hpp>

namespace AXR {
    
    class AXRController;
    class AXRCore;
    
    /**
     * @brief   Deprecated struct used for measuring areas. Use HSSSize instead.
     */
    typedef struct {
        double width;
        double height;
    } AXRSize;
    
    /**
     * @brief   Deprecated struct used to represent a point. Use HSSPoint instead.
     */
    typedef struct {
        double x;
        double y;
    } AXRPoint;
    
    /**
     * @brief   Deprecated struct used to represent a rectangle. Use HSSRect instead.
     */
    typedef struct {
        AXRSize size;
        AXRPoint origin;
    } AXRRect;
    
    /**
     *  @brief This is the main control hub for rendering. Calls for drawing and handling
     *  errors should go through this class.
     */
    class AXRRender {
    public:
        /**
         *  The shared pointer to this class
         */
        typedef boost::shared_ptr<AXRRender> p;
        /**
         *  Creates a new instance of the render object.
         *  @param  controller  A regular pointer to the controller object that is associated with this renderer.
         */
        AXRRender(AXRController * controller);
        /**
         *  Destructs the render object.
         */
        virtual ~AXRRender();
        /**
         *  The main drawing function. Call this to redraw an area of the window.
         *  @param  rect    The rectangular section of the screen that should be redrawn
         *  @param  bounds  The rectangle describing the bounds of the window.
         *
         *  @warning The root surface should be created in platform specific subclass before calling
         *  this as base class' method
         *
         *  @todo change rect to be a HSSRect
         *  @todo change bounds to be a HSSRect
         */
        virtual void drawInRectWithBounds(AXRRect rect, AXRRect bounds);
        /**
         *  Creates a point from the x and y coordinates and sends the mouse down event to the root object,
         *  which will then further handle it.
         *  @param  x   A long double containing the horizontal position.
         *  @param  y   A long double containing the vertical position.
         */
        void mouseDown(long double x, long double y);
        /**
         *  Creates a point from the x and y coordinates and sends the mouse up event to the root object,
         *  which will then further handle it.
         *  @param  x   A long double containing the horizontal position.
         *  @param  y   A long double containing the vertical position.
         */
        void mouseUp(long double x, long double y);
        /**
         *  Reset the renderer to initial values, for example when reloading a file.
         */
        void reset();
        /**
         *  Setter for the pointer to the cairo handle for the window surface.
         *  @param  cairo   A regular pointer to a cairo handle.
         */
        void setCairo(cairo_t * cairo);
        /**
         *  Getter for the pointer to the cairo handle for the window surface.
         *  @return A regular pointer to a cairo handle.
         */
        cairo_t * getCairo();
        /**
         *  @return The width of the window.
         */
        double getWindowWidth();
        /**
         *  @return The height of the window.
         */
        double getWindowHeight();
        
        /**
         *  @todo create getter/setter and make private
         */
        double windowWidth;
        
        /**
         *  @todo create getter/setter and make private
         */
        double windowHeight;
        
    protected:
        //weak pointer
        AXRController * controller;
        cairo_t * cairo;
        cairo_surface_t * rootSurface;
    };
}


#endif //AXRRENDER_H




