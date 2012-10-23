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
#include "HSSColorStop.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"

using namespace AXR;

HSSColorStop::HSSColorStop(AXRController * controller)
: HSSObject(HSSObjectTypeColorStop, controller)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSColorStop: creating color stop object");

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("position");
    shorthandProperties.push_back("balance");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyColor, QVariant::fromValue(&this->color));
    this->registerProperty(HSSObservablePropertyPosition, QVariant::fromValue(&this->position));
    this->registerProperty(HSSObservablePropertyBalance, QVariant::fromValue(&this->balance));
    this->position = this->balance = 0.5;
}

HSSColorStop::HSSColorStop(const HSSColorStop & orig)
: HSSObject(orig)
{
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("position");
    shorthandProperties.push_back("balance");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyColor, QVariant::fromValue(&this->color));
    this->registerProperty(HSSObservablePropertyPosition, QVariant::fromValue(&this->position));
    this->registerProperty(HSSObservablePropertyBalance, QVariant::fromValue(&this->balance));
    this->position = this->balance = 0.5;
}

HSSColorStop::p HSSColorStop::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSColorStop: cloning color stop object");
    return qSharedPointerCast<HSSColorStop> (this->cloneImpl());
}

HSSClonable::p HSSColorStop::cloneImpl() const
{
    return HSSColorStop::p(new HSSColorStop(*this));
}

HSSColorStop::~HSSColorStop()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSColorStop: destructing color stop object");
}

AXRString HSSColorStop::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSColorStop: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSColorStop";
    }
}

AXRString HSSColorStop::defaultObjectType()
{
    return "colorStop";
}

AXRString HSSColorStop::defaultObjectType(AXRString property)
{
    if (property == "color")
    {
        return "rgb";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSColorStop::isKeyword(AXRString value, AXRString property)
{
    if (value == "start" || value == "middle" || value == "end")
    {
        if (property == "position" || property == "balance")
        {
            return true;
        }
    }
    else if (value == "transparent"){
        if (property == "color" ) {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

void HSSColorStop::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name)
    {
    case HSSObservablePropertyColor:
        this->setDColor(value);
        break;
    case HSSObservablePropertyPosition:
        this->setDPosition(value);
        break;
    case HSSObservablePropertyBalance:
        this->setDBalance(value);
        break;
    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

HSSRgb::p HSSColorStop::getColor()
{
    return this->color;
}

HSSParserNode::p HSSColorStop::getDColor()
{
    return this->dColor;
}

void HSSColorStop::setDColor(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dColor = value;
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
            this->setDColor(objdef);
            valid = true;

        }
        catch (const AXRError &e)
        {
            e.raise();
        }

        break;
    }


    case HSSParserNodeTypeFunctionCall:
    {
        this->dColor = value;
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSRgb::p>())
            {
                this->color = remoteValue.value<HSSRgb::p>();
                valid = true;
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyColor, this, new HSSValueChangedCallback<HSSColorStop > (this, &HSSColorStop::colorChanged));
        }
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p theKW = qSharedPointerCast<HSSKeywordConstant>(value);
        AXRString kwValue = theKW->getValue();

        if (kwValue == "black")
        {
            this->color = HSSRgb::blackColor(this->getController());
            valid = true;
        }
        else if (kwValue == "white")
        {
            this->color = HSSRgb::whiteColor(this->getController());
            valid = true;
        }
        else if (kwValue == "transparent")
        {
            //the color will remain empty for transparent
            this->color.clear();
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dColor = value;
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getThisObj());
        objdef->apply();
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeRgb))
        {
            this->color = qSharedPointerCast<HSSRgb > (theobj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSColorStop", "Invalid value for color of " + this->name);

    this->notifyObservers(HSSObservablePropertyColor, &this->color);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSColorStop::colorChanged(HSSObservableProperty source, void*data)
{
    switch (this->dColor->getType())
    {
    case HSSParserNodeTypeFunctionCall:
        this->color = *(HSSRgb::p*)data;
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyPosition, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSColorStop::getPosition()
{
    return this->position;
}

HSSParserNode::p HSSColorStop::getDPosition()
{
    return this->dPosition;
}

void HSSColorStop::setDPosition(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        this->dPosition = value;
        this->position = this->_evaluatePropertyValue(
                                              &HSSColorStop::positionChanged,
                                              value,
                                              1.,
                                              HSSObservablePropertyPosition,
                                              this->observedPosition,
                                              this->observedPositionProperty
                                              );

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        this->dPosition = value;
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSUnit>())
            {
                this->position = remoteValue.value<HSSUnit>();
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyPosition, this, new HSSValueChangedCallback<HSSColorStop > (this, &HSSColorStop::positionChanged));

        }
        else
        {
            throw AXRWarning("HSSDGradient", "Invalid function type for position of " + this->name);
        }

        break;
    }

    default:
        throw AXRWarning("HSSColorStop", "Invalid value for position of " + this->name);
    }
    this->notifyObservers(HSSObservablePropertyPosition, &this->position);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSColorStop::positionChanged(HSSObservableProperty source, void*data)
{
    switch (this->dPosition->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->position = *(HSSUnit*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dPosition);
        this->position = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyPosition, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSColorStop::getBalance()
{
    return this->balance;
}

HSSParserNode::p HSSColorStop::getDBalance()
{
    return this->dBalance;
}

void HSSColorStop::setDBalance(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        this->dBalance = value;
        this->balance = this->_evaluatePropertyValue(
                                             &HSSColorStop::balanceChanged,
                                             value,
                                             1.,
                                             HSSObservablePropertyBalance,
                                             this->observedBalance,
                                             this->observedBalanceProperty
                                             );

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        this->dBalance = value;
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSUnit>())
            {
                this->balance = remoteValue.value<HSSUnit>();
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyBalance, this, new HSSValueChangedCallback<HSSColorStop > (this, &HSSColorStop::balanceChanged));

        }
        else
        {
            throw AXRWarning("HSSDGradient", "Invalid function type for balance of " + this->name);
        }

        break;
    }

    default:
        throw AXRWarning("HSSColorStop", "Invalid value for balance of " + this->name);
    }
    this->notifyObservers(HSSObservablePropertyBalance, &this->balance);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSColorStop::balanceChanged(AXR::HSSObservableProperty source, void *data)
{
    switch (this->dBalance->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->balance = *(HSSUnit*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dBalance);
        this->balance = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyBalance, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSColorStop::_evaluatePropertyValue(
                                         void(HSSColorStop::*callback)(HSSObservableProperty property, void* data),
                                         HSSParserNode::p value,
                                         HSSUnit percentageBase,
                                         HSSObservableProperty observedSourceProperty,
                                         HSSObservable * &observedStore,
                                         HSSObservableProperty &observedStoreProperty
                                         )
{
    HSSUnit ret;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        HSSNumberConstant::p numberValue = qSharedPointerCast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant > (value);
        ret = percentageValue->getValue(percentageBase);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = qSharedPointerCast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setScope(this->scope);
        expressionValue->setThisObj(this->getThisObj());
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSColorStop > (this, callback));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    default:
        AXRWarning("HSSColorStop", "Unknown parser node type while setting value for HSSLineGradient property").raise();
        break;
    }

    return ret;
}
