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
 *      Last changed: 2012/06/03
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 6
 *
 ********************************************************************/

#ifndef HSSSHAPE_H
#define HSSSHAPE_H

#include "HSSObject.h"
#include <string>
#include <boost/shared_ptr.hpp>
#include <cairo/cairo.h>

namespace AXR {
    
    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSShapeType
     *  The type of the shape object, specific for each subclass.
     */
    enum HSSShapeType {
        HSSShapeTypeRectangle, /**< The default shape for all containers, a rectangle. */
        HSSShapeTypeRoundedRect, /**< A rectangle with rounded corners. */
        HSSShapeTypeCircle, /**< A circle (or ellipse if width !== height). */
        HSSShapeTypePolygon, /**< A regular polygon with all its sides of the same length. */
        HSSShapeTypeStar /**< A star, with parameterized number and angles of spikes. */
    };
    /** @} */
    
    /**
     *  @brief Abstract base class for all shape objects.
     *  Do not use directly, use a specific subclass instead.
     */
    class HSSShape : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSShape> p;
        
        /**
         *  Creates a new instance of a shape object. Do call directly, use one of the sublcasses instead.
         */
        HSSShape();
        
        /**
         *  Copy constructor for HSSShape objects. Do not call directly, use clone() instead.
         */
        HSSShape(const HSSShape & orig);
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSShape();
        
        /**
         *  Allows you to check if this shape is of the given type.
         *  @param  type    The shape type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSShapeType type);
        
        /**
         *  @return The shape type of this instance.
         */
        HSSShapeType getShapeType();
        
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        
        /**
         *  Each shape object subclass implements its own drawing routines, creating the path as needed.
         *  The x, y, width and height parameters are used to define the size of the resulting shape. Usually
         *  theses are the x: 0, y:0 (upper left corner) and as wide and tall as the container, but they can be
         *  different in case of joining paths together, for example.
         *
         *  @param cairo    A regular pointer to the cairo handle in which to create the path.
         *  @param x        The horizontal coordinate for the upper left corner of the bounding box of the shape.
         *  @param y        The vertical coordinate for the upper left corner of the bounding box of the shape.
         *  @param width    The width of the bounding box of the shape.
         *  @param height   The height of the bounding box of the shape.
         */
        virtual void draw(cairo_t * cairo, double long x, double long y, double long width, double long height) = 0;
        
    protected:
        HSSShapeType shapeType;
    };
}


#endif
