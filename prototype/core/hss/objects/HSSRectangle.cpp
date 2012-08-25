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
 *      COPYRIGHT: ©2012 - All Rights Reserved
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

#include "HSSRectangle.h"

using namespace AXR;

HSSRectangle::HSSRectangle()
: HSSShape(HSSShapeTypeRectangle)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRectangle: creating rectangle object");
}

HSSRectangle::HSSRectangle(const HSSRectangle & orig)
: HSSShape(orig)
{

}

HSSRectangle::p HSSRectangle::clone() const{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRectangle: cloning rectangle object");
    return boost::static_pointer_cast<HSSRectangle, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSRectangle::cloneImpl() const{
    return HSSClonable::p(new HSSRectangle(*this));
}

HSSRectangle::~HSSRectangle()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRectangle: destructing rectangle object");
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


void HSSRectangle::draw(cairo_t * cairo, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height)
{
    cairo_rectangle(cairo, x, y, width, height);
}
