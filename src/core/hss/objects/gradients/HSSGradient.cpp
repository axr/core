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
#include "HSSGradient.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"

using namespace AXR;

HSSGradient::HSSGradient(AXRController * controller)
: HSSObject(HSSObjectTypeGradient, controller)
{
    this->registerProperty(HSSObservablePropertyStartColor, QVariant::fromValue(&this->startColor));
    this->registerProperty(HSSObservablePropertyEndColor, QVariant::fromValue(&this->endColor));
    this->registerProperty(HSSObservablePropertyBalance, QVariant::fromValue(&this->balance));
    this->registerProperty(HSSObservablePropertyColorStops, QVariant::fromValue(&this->colorStops));
}

HSSGradient::HSSGradient(const HSSGradient & orig)
: HSSObject(orig)
{
    this->registerProperty(HSSObservablePropertyStartColor, QVariant::fromValue(&this->startColor));
    this->registerProperty(HSSObservablePropertyEndColor, QVariant::fromValue(&this->endColor));
    this->registerProperty(HSSObservablePropertyBalance, QVariant::fromValue(&this->balance));
    this->registerProperty(HSSObservablePropertyColorStops, QVariant::fromValue(&this->colorStops));
}

HSSGradient::~HSSGradient()
{
}

AXRString HSSGradient::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSGradient: ").append(this->name);
    }
    else
    {
        return "Anonymous HSSGradient";
    }
}

AXRString HSSGradient::defaultObjectType()
{
    return "linearGradient";
}

AXRString HSSGradient::defaultObjectType(AXRString property)
{
    if (property == "startColor"
            || property == "endColor")
    {
        return "rgb";
    }
    else if (property == "colorStops")
    {
        return "colorStop";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSGradient::isKeyword(AXRString value, AXRString property)
{
    if ((value == "inside" || value == "centered" || value == "outside") && (property == "position"))
    {
        return true;
    }
    else if ( value == "black" || value == "white" || value == "transparent")
    {
        if (property == "startColor" || property == "endColor" || property == "colorStops")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

void HSSGradient::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name)
    {
    case HSSObservablePropertyStartColor:
        this->setDStartColor(value);
        break;
    case HSSObservablePropertyEndColor:
        this->setDEndColor(value);
        break;
    case HSSObservablePropertyBalance:
        this->setDBalance(value);
        break;
    case HSSObservablePropertyColorStops:
        this->setDColorStops(value);
        break;
    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

HSSRgb::p HSSGradient::getColorAfterFirst()
{
    //first, look into the color stops to see if we find a suitable color
    HSSRgb::p ret;
    if (this->colorStops.size() > 0) {
        for (std::vector<HSSObject::p>::const_iterator it = this->colorStops.begin(); it != this->colorStops.end(); ++it) {
            const HSSObject::p & theStopObj = *it;
            if (theStopObj->isA(HSSObjectTypeColorStop)) {
                HSSColorStop::p theStop = qSharedPointerCast<HSSColorStop>(theStopObj);
                ret = theStop->getColor();
                if(ret){
                    break;
                }
            } else if (theStopObj->isA(HSSObjectTypeRgb)) {
                ret = qSharedPointerCast<HSSRgb>(theStopObj);
                break;
            }
        }
    }
    //not found yet? we'll use the endColor
    if (!ret){
        ret = this->endColor;
    }

    //not even that? use transparent black
    if (!ret){
        ret = HSSRgb::transparentColor(this->getController());
    }

    return ret;
}

HSSRgb::p HSSGradient::getColorBeforeLast()
{
    //first, look into the color stops (in reverse order) to see if we find a suitable color
    HSSRgb::p ret;
    if (this->colorStops.size() > 0) {
        for (std::vector<HSSObject::p>::const_reverse_iterator it = this->colorStops.rbegin(); it != this->colorStops.rend(); ++it) {
            const HSSObject::p & theStopObj = *it;
            if (theStopObj->isA(HSSObjectTypeColorStop)) {
                HSSColorStop::p theStop = qSharedPointerCast<HSSColorStop>(theStopObj);
                ret = theStop->getColor();
                if(ret){
                    break;
                }
            } else if (theStopObj->isA(HSSObjectTypeRgb)) {
                ret = qSharedPointerCast<HSSRgb>(theStopObj);
                break;
            }
        }
    }
    //not found yet? we'll use the startColor
    if (!ret){
        ret = this->startColor;
    }

    //not even that? use transparent black
    if (!ret){
        ret = HSSRgb::transparentColor(this->getController());
    }

    return ret;
}

HSSRgb::p HSSGradient::getNextColorFromStops(std::vector<HSSObject::p>::iterator it, std::vector<HSSObject::p>::iterator endIt)
{
    //first, look into the color stops to see if we find a suitable color
    HSSRgb::p ret;
    for (; it != endIt; ++it) {
        const HSSObject::p & stopObj = *it;
        if (stopObj->isA(HSSObjectTypeRgb)) {
            ret = qSharedPointerCast<HSSRgb>(stopObj);
            break;
        }
        else if (stopObj->isA(HSSObjectTypeColorStop))
        {
            const HSSColorStop::p theStop = qSharedPointerCast<HSSColorStop>(stopObj);
            if(HSSRgb::p theColor = theStop->getColor())
            {
                ret = theColor;
                break;
            }
        }
    }

    //not found yet? we'll use the endColor
    if(!ret){
        ret = this->endColor;
    }

    //not even that? use transparent black
    if (!ret){
        ret = HSSRgb::transparentColor(this->getController());
    }

    return ret;
}

HSSRgb::p HSSGradient::getStartColor()
{
    return this->startColor;
}

HSSParserNode::p HSSGradient::getDStartColor()
{
    return this->dStartColor;
}

void HSSGradient::setDStartColor(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dStartColor = value;
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
            this->setDStartColor(objdef);
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
        this->dStartColor = value;
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<std::vector<HSSObject::p> *>())
            {
                std::vector<HSSObject::p> theObjVec = *remoteValue.value<std::vector<HSSObject::p> *>();
                HSSObject::p theObj = theObjVec.front();
                if (theObj->isA(HSSObjectTypeRgb))
                {
                    this->startColor = qSharedPointerCast<HSSRgb>(theObj);
                    valid = true;
                }
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyStartColor, this, new HSSValueChangedCallback<HSSGradient > (this, &HSSGradient::startColorChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p theKW = qSharedPointerCast<HSSKeywordConstant>(value);
        if (theKW->getValue() == "black")
        {
            this->startColor = HSSRgb::blackColor(this->getController());
            valid = true;
        }
        else if (theKW->getValue() == "white")
        {
            this->startColor = HSSRgb::whiteColor(this->getController());
            valid = true;
        }
        else if (theKW->getValue() == "transparent")
        {
            this->startColor.clear();
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
        this->dStartColor = value;
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getThisObj());
        objdef->apply();
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeRgb))
        {
            this->startColor = qSharedPointerCast<HSSRgb > (theobj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDGradient", "Invalid value for startColor of " + this->name);

    this->notifyObservers(HSSObservablePropertyStartColor, &this->startColor);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSGradient::startColorChanged(HSSObservableProperty source, void*data)
{

}

HSSRgb::p HSSGradient::getEndColor()
{
    return this->endColor;
}

HSSParserNode::p HSSGradient::getDEndColor()
{
    return this->dEndColor;
}

void HSSGradient::setDEndColor(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dEndColor = value;
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->getController()->objectTreeGet(objname->getValue());
            this->setDEndColor(objdef);
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
        this->dEndColor = value;
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<std::vector<HSSObject::p> *>())
            {
                std::vector<HSSObject::p> theObjVec = *remoteValue.value<std::vector<HSSObject::p> *>();
                HSSObject::p theObj = theObjVec.front();
                if (theObj->isA(HSSObjectTypeRgb))
                {
                    this->endColor = qSharedPointerCast<HSSRgb>(theObj);
                    valid = true;
                }
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyEndColor, this, new HSSValueChangedCallback<HSSGradient > (this, &HSSGradient::endColorChanged));
        }
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p theKW = qSharedPointerCast<HSSKeywordConstant>(value);
        if (theKW->getValue() == "black")
        {
            this->endColor = HSSRgb::blackColor(this->getController());
            valid = true;
        }
        else if (theKW->getValue() == "white")
        {
            this->endColor = HSSRgb::whiteColor(this->getController());
            valid = true;
        }
        else if (theKW->getValue() == "transparent")
        {
            this->endColor.clear();
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
        this->dEndColor = value;
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getThisObj());
        objdef->apply();
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeRgb))
        {
            this->endColor = qSharedPointerCast<HSSRgb > (theobj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDGradient", "Invalid value for endColor of " + this->name);

    this->notifyObservers(HSSObservablePropertyEndColor, &this->endColor);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSGradient::endColorChanged(HSSObservableProperty source, void*data)
{

}

HSSUnit HSSGradient::getBalance()
{
    return this->balance;
}

HSSParserNode::p HSSGradient::getDBalance()
{
    return this->dBalance;
}

void HSSGradient::setDBalance(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        this->dBalance = value;
        this->balance = this->_evaluatePropertyValue(
                                             &HSSGradient::balanceChanged,
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

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyBalance, this, new HSSValueChangedCallback<HSSGradient > (this, &HSSGradient::balanceChanged));

        }
        else
        {
            throw AXRWarning("HSSDGradient", "Invalid function type size of " + this->name);
        }

        break;
    }

    default:
        throw AXRWarning("HSSGradient", "Invalid value for size of " + this->name);
    }
    this->notifyObservers(HSSObservablePropertyBalance, &this->balance);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSGradient::balanceChanged(AXR::HSSObservableProperty source, void *data)
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

//colorStops

HSSParserNode::p HSSGradient::getDColorStops()
{
    return this->dColorStops;
}

void HSSGradient::setDColorStops(HSSParserNode::p value)
{
    this->colorStops.clear();
    this->dColorStops = value;
    this->addDColorStops(value);
}

void HSSGradient::addDColorStops(HSSParserNode::p value)
{
    bool valid = false;

    switch (value->getType())
    {
    case HSSParserNodeTypeMultipleValueDefinition:
    {
        HSSMultipleValueDefinition::p multiDef = qSharedPointerCast<HSSMultipleValueDefinition > (value);
        std::vector<HSSParserNode::p> values = multiDef->getValues();
        for (HSSParserNode::it iterator = values.begin(); iterator != values.end(); ++iterator)
        {
            this->addDColorStops(*iterator);
        }

        valid = true;
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            HSSObjectNameConstant::p objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            this->addDColorStops(this->getController()->objectTreeGet(objname->getValue()));
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->getScope());
            fnct->setThisObj(this->getThisObj());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSParserNode::p>())
            {
                try
                {
                    HSSParserNode::p theVal = remoteValue.value<HSSParserNode::p>();
                    this->addDColorStops(theVal);
                    valid = true;
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }
        }
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p theKW = qSharedPointerCast<HSSKeywordConstant > (value);
        if (theKW->getValue() == "white")
        {
            this->colorStops.push_back(HSSRgb::whiteColor(this->getController()));
            valid = true;
        }
        else if (theKW->getValue() == "black")
        {
            this->colorStops.push_back(HSSRgb::blackColor(this->getController()));
            valid = true;
        }
        else if (theKW->getValue() == "transparent")
        {
            //the stop's color will remain empty, we just define the position
            HSSColorStop::p theStop(new HSSColorStop(this->getController()));
            theStop->setDPosition(HSSPercentageConstant::p(new HSSPercentageConstant(50., this->getController())));
            this->colorStops.push_back(theStop);
            valid = true;
        }
        else
        {
            valid = false;
        }

        break;
    }

    default:
        valid = false;
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dColorStops = value;
        HSSObjectDefinition::p objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        if (objdef->getObject()->isA(HSSObjectTypeColorStop) || objdef->getObject()->isA(HSSObjectTypeRgb))
        {
            objdef->setScope(this->getScope());
            objdef->setThisObj(this->getThisObj());
            objdef->apply();
            HSSObject::p theObj = objdef->getObject();
            theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyColorStops, this, new HSSValueChangedCallback<HSSGradient > (this, &HSSGradient::colorStopsChanged));
            this->colorStops.push_back(theObj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSGradient", "Invalid value for colorStops of " + this->getName());

    this->notifyObservers(HSSObservablePropertyColorStops, &this->colorStops);
}

void HSSGradient::colorStopsChanged(HSSObservableProperty source, void*data)
{

}

HSSUnit HSSGradient::_evaluatePropertyValue(
                                        void(HSSGradient::*callback)(HSSObservableProperty property, void* data),
                                        HSSParserNode::p value,
                                        HSSUnit percentageBase,
                                        HSSObservableProperty observedSourceProperty,
                                        HSSObservable * &observedStore,
                                        HSSObservableProperty &observedStoreProperty
                                        )
{
    HSSUnit ret = 0;

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
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSGradient > (this, callback));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
        break;

    default:
        AXRWarning("HSSGradient", "Unknown parser node type while setting value for HSSLineGradient property").raise();
        break;
    }

    return ret;
}
