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
 *      Last changed: 2012/01/03
 *      HSS version: 1.0
 *      Core version: 0.44
 *      Revision: 5
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
    
    typedef struct {
        double width;
        double height;
    } AXRSize;
    
    typedef struct {
        double x;
        double y;
    } AXRPoint;
    
    typedef struct {
        AXRSize size;
        AXRPoint origin;
    } AXRRect;
    
    class AXRRender {
    public:
        typedef boost::shared_ptr<AXRRender> p;
        
        AXRRender(AXRController * controller, AXRCore * core);
        virtual ~AXRRender();
        
        //root surface should be created in platform specific subclass
        //before calling this as base class' method
        virtual void drawInRectWithBounds(AXRRect rect, AXRRect bounds);
        
        void mouseDown(long double x, long double y);
        void mouseUp(long double x, long double y);
        
        void reset();
        
        void setCairo(cairo_t * cairo);
        cairo_t * getCairo();
        
    protected:
        //weak pointer
        AXRController * controller;
        AXRCore * core;
        double windowWidth;
        double windowHeight;
        cairo_t * cairo;
        cairo_surface_t * rootSurface;
    };
}


#endif //AXRRENDER_H




