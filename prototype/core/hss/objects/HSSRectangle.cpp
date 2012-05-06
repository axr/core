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
 *      Last changed: 2012/04/22
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 5
 *
 ********************************************************************/

#include "HSSRectangle.h"

using namespace AXR;

HSSRectangle::HSSRectangle()
: HSSShape()
{
    this->shapeType = HSSShapeTypeRectangle;
}

HSSRectangle::HSSRectangle(const HSSRectangle & orig)
: HSSShape(orig)
{
    
}

HSSRectangle::p HSSRectangle::clone() const{
    return boost::static_pointer_cast<HSSRectangle, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSRectangle::cloneImpl() const{
    return HSSClonable::p(new HSSRectangle(*this));
}

HSSRectangle::~HSSRectangle()
{
    
}

std::string HSSRectangle::toString()
{
    return "HSSRectangle";
}

std::string HSSRectangle::defaultObjectType()
{
    return "HSSRectangle";
}

std::string HSSRectangle::defaultObjectType(std::string property)
{
    return HSSShape::defaultObjectType(property);
}

bool HSSRectangle::isKeyword(std::string value, std::string property)
{
    return HSSShape::isKeyword(value, property);
}


void HSSRectangle::draw(cairo_t * cairo, double long x, double long y, double long width, double long height)
{
    cairo_rectangle(cairo, x, y, width, height);
}

