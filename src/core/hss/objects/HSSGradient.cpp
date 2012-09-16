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

#include "errors.h"
#include "AXRController.h"
#include "AXRDebugging.h"
#include "HSSConstants.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSGradient.h"
#include "HSSObjectDefinition.h"

using namespace AXR;

HSSGradient::HSSGradient()
: HSSObject(HSSObjectTypeGradient)
{
    this->registerProperty(HSSObservablePropertyStartColor, & this->startColor);
    this->registerProperty(HSSObservablePropertyEndColor, & this->endColor);
    this->registerProperty(HSSObservablePropertyBalance, & this->balance);
    this->registerProperty(HSSObservablePropertyColorStops, & this->colorStops);
}

HSSGradient::HSSGradient(const HSSGradient & orig)
: HSSObject(orig)
{
    this->registerProperty(HSSObservablePropertyStartColor, & this->startColor);
    this->registerProperty(HSSObservablePropertyEndColor, & this->endColor);
    this->registerProperty(HSSObservablePropertyBalance, & this->balance);
    this->registerProperty(HSSObservablePropertyColorStops, & this->colorStops);
}

HSSGradient::~HSSGradient()
{
}

std::string HSSGradient::toString()
{
    if (this->isNamed())
    {
        return std::string("HSSGradient: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSGradient";
    }
}

std::string HSSGradient::defaultObjectType()
{
    return "linearGradient";
}

std::string HSSGradient::defaultObjectType(std::string property)
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

bool HSSGradient::isKeyword(std::string value, std::string property)
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
                HSSColorStop::p theStop = boost::static_pointer_cast<HSSColorStop>(theStopObj);
                ret = theStop->getColor();
                if(ret){
                    break;
                }
            } else if (theStopObj->isA(HSSObjectTypeRgb)) {
                ret = boost::static_pointer_cast<HSSRgb>(theStopObj);
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
        ret = HSSRgb::transparentColor();
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
                HSSColorStop::p theStop = boost::static_pointer_cast<HSSColorStop>(theStopObj);
                ret = theStop->getColor();
                if(ret){
                    break;
                }
            } else if (theStopObj->isA(HSSObjectTypeRgb)) {
                ret = boost::static_pointer_cast<HSSRgb>(theStopObj);
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
        ret = HSSRgb::transparentColor();
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
            ret = boost::static_pointer_cast<HSSRgb>(stopObj);
            break;
        }
        else if (stopObj->isA(HSSObjectTypeColorStop))
        {
            const HSSColorStop::p theStop = boost::static_pointer_cast<HSSColorStop>(stopObj);
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
        ret = HSSRgb::transparentColor();
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
            HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
            this->setDStartColor(objdef);
            valid = true;

        }
        catch (const AXRError::p &e)
        {
            e->raise();
        }
        catch (const AXRWarning::p &e)
        {
            e->raise();
        }

        break;
    }


    case HSSParserNodeTypeFunctionCall:
    {
        this->dStartColor = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->startColor = boost::any_cast<HSSRgb::p > (remoteValue);
                valid = true;
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyStartColor, this, new HSSValueChangedCallback<HSSGradient > (this, &HSSGradient::startColorChanged));

        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p theKW = boost::static_pointer_cast<HSSKeywordConstant>(value);
        if (theKW->getValue() == "none")
        {
            ///@todo should we deprecate this in favor of `transparent`?
            this->startColor.reset();
            valid = true;
        }
        else if (theKW->getValue() == "black")
        {
            this->startColor = HSSRgb::blackColor();
            valid = true;
        }
        else if (theKW->getValue() == "white")
        {
            this->startColor = HSSRgb::whiteColor();
            valid = true;
        }
        else if (theKW->getValue() == "transparent")
        {
            this->startColor.reset();
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
        HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getThisObj());
        objdef->apply();
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeRgb))
        {
            this->startColor = boost::static_pointer_cast<HSSRgb > (theobj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for startColor of " + this->name));

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
            HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant > (value);
            HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
            this->setDEndColor(objdef);
            valid = true;

        }
        catch (const AXRError::p &e)
        {
            e->raise();
        }
        catch (const AXRWarning::p &e)
        {
            e->raise();
        }


        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        this->dEndColor = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->endColor = boost::any_cast<HSSRgb::p > (remoteValue);
                valid = true;
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyEndColor, this, new HSSValueChangedCallback<HSSGradient > (this, &HSSGradient::endColorChanged));
        }
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p theKW = boost::static_pointer_cast<HSSKeywordConstant>(value);
        if (theKW->getValue() == "none")
        {
            ///@todo should we deprecate this in favor of `transparent`?
            this->endColor.reset();
            valid = true;
        }
        else if (theKW->getValue() == "black")
        {
            this->endColor = HSSRgb::blackColor();
            valid = true;
        }
        else if (theKW->getValue() == "white")
        {
            this->endColor = HSSRgb::whiteColor();
            valid = true;
        }
        else if (theKW->getValue() == "transparent")
        {
            this->endColor.reset();
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
        HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition > (value);
        objdef->setScope(this->scope);
        objdef->setThisObj(this->getThisObj());
        objdef->apply();
        HSSObject::p theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeRgb))
        {
            this->endColor = boost::static_pointer_cast<HSSRgb > (theobj);
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for endColor of " + this->name));

    this->notifyObservers(HSSObservablePropertyEndColor, &this->endColor);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSGradient::endColorChanged(HSSObservableProperty source, void*data)
{

}

long double HSSGradient::getBalance()
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
        this->balance = this->_setLDProperty(
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
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->balance = boost::any_cast<long double>(remoteValue);
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyBalance, this, new HSSValueChangedCallback<HSSGradient > (this, &HSSGradient::balanceChanged));

        }
        else
        {
            throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid function type size of " + this->name));
        }

        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSGradient", "Invalid value for size of " + this->name));
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
        this->balance = *(long double*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (this->dBalance);
        this->balance = percentageValue->getValue(*(long double*) data);
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
        HSSMultipleValueDefinition::p multiDef = boost::static_pointer_cast<HSSMultipleValueDefinition > (value);
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
            HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant > (value);
            this->addDColorStops(this->axrController->objectTreeGet(objname->getValue()));
            valid = true;
        }
        catch (const AXRError::p &e)
        {
            e->raise();
        }

        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->getScope());
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                HSSParserNode::p theVal = boost::any_cast<HSSParserNode::p > (remoteValue);
                this->addDColorStops(theVal);
                valid = true;
            }
            catch (const AXRError::p &e)
            {
                e->raise();
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }
        }
        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        HSSKeywordConstant::p theKW = boost::static_pointer_cast<HSSKeywordConstant > (value);
        if (theKW->getValue() == "none")
        {
            ///@todo should we deprecate this in favor of `transparent`?
            valid = true;
        }
        else if (theKW->getValue() == "white")
        {
            this->colorStops.push_back(HSSRgb::whiteColor());
            valid = true;
        }
        else if (theKW->getValue() == "black")
        {
            this->colorStops.push_back(HSSRgb::blackColor());
            valid = true;
        }
        else if (theKW->getValue() == "transparent")
        {
            //the stop's color will remain empty, we just define the position
            HSSColorStop::p theStop(new HSSColorStop());
            theStop->setDPosition(HSSPercentageConstant::p(new HSSPercentageConstant(50.)));
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
        HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition > (value);
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
        throw AXRWarning::p(new AXRWarning("HSSGradient", "Invalid value for colorStops of " + this->getName()));

    this->notifyObservers(HSSObservablePropertyColorStops, &this->colorStops);
}

void HSSGradient::colorStopsChanged(HSSObservableProperty source, void*data)
{

}

long double HSSGradient::_setLDProperty(
                                        void(HSSGradient::*callback)(HSSObservableProperty property, void* data),
                                        HSSParserNode::p value,
                                        long double percentageBase,
                                        HSSObservableProperty observedSourceProperty,
                                        HSSObservable * &observedStore,
                                        HSSObservableProperty &observedStoreProperty
                                        )
{
    long double ret;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        HSSNumberConstant::p numberValue = boost::static_pointer_cast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (value);
        ret = percentageValue->getValue(percentageBase);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression > (value);
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
        AXRWarning::p(new AXRWarning("HSSGradient", "Unknown parser node type while setting value for HSSLineGradient property"))->raise();
        break;
    }

    return ret;
}
