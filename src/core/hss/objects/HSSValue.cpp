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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
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

#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRWarning.h"
#include "HSSFunction.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSValue.h"

using namespace AXR;

HSSValue::HSSValue(AXRController * controller)
: HSSObject(HSSObjectTypeValue, controller)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSValue: creating value object");

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyValue, QVariant::fromValue(&this->dValue));
}

HSSValue::HSSValue(const HSSValue & orig)
: HSSObject(orig)
{
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyValue, QVariant::fromValue(&this->dValue));
}

HSSValue::p HSSValue::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSValue: cloning value object");
    return qSharedPointerCast<HSSValue>(this->cloneImpl());
}

HSSClonable::p HSSValue::cloneImpl() const
{
    return HSSClonable::p(new HSSValue(*this));
}

HSSValue::~HSSValue()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSValue: destructing value object");
}

AXRString HSSValue::toString()
{
    return "HSSValue: " + (dValue ? dValue->toString() : "(null)");
}

AXRString HSSValue::defaultObjectType()
{
    return "value";
}

AXRString HSSValue::defaultObjectType(AXRString property)
{
    if (property == "value")
    {
        return "request";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

void HSSValue::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name)
    {
    case HSSObservablePropertyValue:
        this->setDValue(value);
        break;

    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

const HSSParserNode::p HSSValue::getDValue() const
{
    return this->dValue;
}

void HSSValue::setDValue(HSSParserNode::p value)
{
    this->dValue = value;
    this->addDValue(value);
}

void HSSValue::addDValue(HSSParserNode::p value)
{
    bool valid = true;
    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for value of @value " + this->name);

    this->notifyObservers(HSSObservablePropertyValue, &this->dValue);
}

void HSSValue::valueChanged(HSSObservableProperty source, void*data)
{
    std_log("HSSValue::valueChanged unimplemented");
}
