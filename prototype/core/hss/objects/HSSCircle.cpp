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
 *      Last changed: 2011/11/24
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 2
 *
 ********************************************************************/

#import "HSSCircle.h"

using namespace AXR;

HSSCircle::HSSCircle()
: HSSShape()
{
    this->shapeType = HSSShapeTypeCircle;
}

HSSCircle::HSSCircle(std::string name)
: HSSShape(name)
{
    this->shapeType = HSSShapeTypeCircle;
}

HSSCircle::~HSSCircle()
{
    
}

std::string HSSCircle::toString()
{
    return "HSSCircle";
}

std::string HSSCircle::defaultObjectType()
{
    return "HSSCircle";
}

std::string HSSCircle::defaultObjectType(std::string property)
{
    return HSSShape::defaultObjectType(property);
}

bool HSSCircle::isKeyword(std::string value, std::string property)
{
    return HSSShape::isKeyword(value, property);
}

void HSSCircle::draw(cairo_t * cairo, double long x, double long y, double long width, double long height)
{
    double long halfWidth = x + width * 0.5;
    cairo_arc(cairo, halfWidth, y + (height * 0.5), halfWidth, 0., 2*M_PI);
}



