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
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

/**
 *  @todo set defaults
 */

#include "axr.h"
#include "HSSRadialGradient.h"

using namespace AXR;

HSSRadialGradient::HSSRadialGradient(AXRController * controller)
: HSSGradient(HSSGradientTypeRadial, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRadialGradient: creating radial gradient object");
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("centerX");
    shorthandProperties.push_back("centerY");
    shorthandProperties.push_back("offsetX");
    shorthandProperties.push_back("offsetY");
    //shorthandProperties.push_back("roundness");
    this->setShorthandProperties(shorthandProperties);
}

HSSRadialGradient::HSSRadialGradient(const HSSRadialGradient & orig)
: HSSGradient(orig)
{
    this->_initialize();
}

void HSSRadialGradient::_initialize()
{
    this->setShorthandProperties(shorthandProperties);
}

QSharedPointer<HSSRadialGradient> HSSRadialGradient::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRadialGradient: cloning radial gradient object");
    return qSharedPointerCast<HSSRadialGradient> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSRadialGradient::cloneImpl() const
{
    return QSharedPointer<HSSRadialGradient>(new HSSRadialGradient(*this));
}

HSSRadialGradient::~HSSRadialGradient()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRadialGradient: destructing radial gradient object");
}

void HSSRadialGradient::setDefaults()
{
    HSSGradient::setDefaults();
    this->setDefaultPercentage("centerX", 50.);
    this->setDefaultPercentage("centerY", 50.);
    this->setDefaultPercentage("offsetX", 50.);
    this->setDefault("offsetY", 0.);
}

AXRString HSSRadialGradient::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSRadialGradient: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSRadialGradient";
    }
}

AXRString HSSRadialGradient::defaultObjectType()
{
    return "radialGradient";
}

AXRString HSSRadialGradient::defaultObjectType(AXRString property)
{
    //    if (property == "bla"){
    //
    //    } else {
    return HSSGradient::defaultObjectType(property);
    //    }
}

bool HSSRadialGradient::isKeyword(AXRString value, AXRString property)
{
    if (value == "top" || value == "bottom")
    {
        if (property == "centerX")
        {
            return true;
        }
    }
    else if (value == "left" || value == "right")
    {
        if (property == "centerY")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSGradient::isKeyword(value, property);
}

HSSUnit HSSRadialGradient::getCenterX() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("centerX");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

HSSUnit HSSRadialGradient::getCenterY() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("centerY");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

HSSUnit HSSRadialGradient::getOffsetX() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("offsetX");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

HSSUnit HSSRadialGradient::getOffsetY() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("offsetY");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}
