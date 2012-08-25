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

#include "AXRController.h"
#include "HSSFunction.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSValue.h"

using namespace AXR;

HSSValue::HSSValue()
: HSSObject(HSSObjectTypeValue)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSValue: creating value object");

    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyValue, &this->dValue);
}

HSSValue::HSSValue(const HSSValue & orig)
: HSSObject(orig)
{
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyValue, &this->dValue);
}

HSSValue::p HSSValue::clone() const{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSValue: cloning value object");
    return boost::static_pointer_cast<HSSValue, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSValue::cloneImpl() const{
    return HSSClonable::p(new HSSValue(*this));
}

HSSValue::~HSSValue()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSValue: destructing value object");
}

std::string HSSValue::toString()
{
    return "HSSValue: "+this->dValue->toString();
}

std::string HSSValue::defaultObjectType()
{
    return "value";
}

std::string HSSValue::defaultObjectType(std::string property)
{
    if (property == "value"){
        return "request";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

void HSSValue::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyValue:
            this->setDValue(value);
            break;

        default:
            HSSObject::setProperty(name, value);
            break;
    }
}

const HSSParserNode::p HSSValue::getDValue() const { return this->dValue; }
void HSSValue::setDValue(HSSParserNode::p value)
{
    this->dValue = value;
    this->addDValue(value);
}

void HSSValue::addDValue(HSSParserNode::p value)
{
    bool valid = true;
    if (!valid)
        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for value of @value "+this->name));

    this->notifyObservers(HSSObservablePropertyValue, &this->dValue);
}

void HSSValue::valueChanged(HSSObservableProperty source, void*data)
{
    std_log("HSSValue::valueChanged unimplemented");
}
