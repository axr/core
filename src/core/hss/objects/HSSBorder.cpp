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
#include "AXRDebugging.h"
#include "HSSBorder.h"
#include "HSSConstants.h"
#include "HSSExpression.h"
#include "HSSFunction.h"

using namespace AXR;

HSSBorder::HSSBorder()
: HSSObject(HSSObjectTypeBorder)
{
    this->observedSize = NULL;
    this->registerProperty(HSSObservablePropertySize, (void *) & this->size);
}

HSSBorder::HSSBorder(const HSSBorder & orig)
: HSSObject(orig)
{
    this->observedSize = NULL;
    this->registerProperty(HSSObservablePropertySize, (void *) & this->size);
}

HSSBorder::~HSSBorder()
{

}

std::string HSSBorder::toString()
{
    if (this->isNamed())
    {
        return std::string("HSSBorder: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSBorder";
    }
}

std::string HSSBorder::defaultObjectType()
{
    return "border";
}

std::string HSSBorder::defaultObjectType(std::string property)
{
    if (property == "effects")
    {
        return "shadow";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSBorder::isKeyword(std::string value, std::string property)
{
    if (value == "inside" || value == "centered" || value == "outside")
    {
        if (property == "position")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

void HSSBorder::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name)
    {
    case HSSObservablePropertySize:
        this->setDSize(value);
        break;
    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

HSSUnit HSSBorder::getSize()
{
    return this->size;
}

void HSSBorder::setDSize(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        this->dSize = value;
        this->size = this->_setLDProperty(
                                          &HSSBorder::sizeChanged,
                                          value,
                                          18.,
                                          HSSObservablePropertySize,
                                          this->observedSize,
                                          this->observedSizeProperty
                                          );

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        this->dSize = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->getThisObj());
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->size = boost::any_cast<HSSUnit > (remoteValue);
            }
            catch (...)
            {
                this->size = 1.;
            }
            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertySize, this, new HSSValueChangedCallback<HSSBorder > (this, &HSSBorder::sizeChanged));


        }
        else
        {
            throw AXRWarning::p(new AXRWarning("HSSDBorder", "Invalid function type size of " + this->name));
        }

        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSBorder", "Invalid value for size of " + this->name));
    }
    this->notifyObservers(HSSObservablePropertySize, &this->size);
}

void HSSBorder::sizeChanged(AXR::HSSObservableProperty source, void *data)
{
    switch (this->dSize->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->size = *(HSSUnit*) data;
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertySize, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSBorder::_setLDProperty(
                                      void(HSSBorder::*callback)(HSSObservableProperty property, void* data),
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
        if (callback != NULL)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSBorder > (this, callback));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    default:
        AXRWarning::p(new AXRWarning("HSSLineBorder", "Unknown parser node type while setting value for HSSLineBorder property"))->raise();
        break;
    }

    return ret;
}
