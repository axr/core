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

#include "HSSCircle.h"

using namespace AXR;

HSSCircle::HSSCircle()
: HSSShape(HSSShapeTypeCircle)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSCircle: creating circle object");
}

HSSCircle::HSSCircle(const HSSCircle & orig)
: HSSShape(orig)
{

}

HSSCircle::p HSSCircle::clone() const{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSCircle: cloning circle object");
    return boost::static_pointer_cast<HSSCircle, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSCircle::cloneImpl() const{
    return HSSClonable::p(new HSSCircle(*this));
}

HSSCircle::~HSSCircle()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSCircle: destructing circle object");
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

void HSSCircle::draw(cairo_t * cairo, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height)
{
    cairo_save(cairo);
    cairo_translate(cairo, x + width / 2., y + height / 2.);
    cairo_scale(cairo, 1. * (width / 2.), 1. * (height / 2.));
    cairo_arc(cairo, 0., 0., 1., 0., 2 * M_PI);
    cairo_restore(cairo);
}
