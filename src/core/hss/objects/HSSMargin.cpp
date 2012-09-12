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

#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSMargin.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"

using namespace AXR;

HSSMargin::HSSMargin()
: HSSObject(HSSObjectTypeMargin)
{
    this->top = this->right = this->bottom = this->left = 0.0;
    this->observedTop = this->observedRight = this->observedBottom = this->observedLeft = NULL;
}

HSSMargin::HSSMargin(const HSSMargin & orig)
: HSSObject(orig)
{
    this->top = orig.top;
    this->right = orig.right;
    this->bottom = orig.bottom;
    this->left = orig.left;
    this->observedTop = NULL;
    this->observedRight = NULL;
    this->observedBottom = NULL;
    this->observedLeft = NULL;
}

HSSMargin::p HSSMargin::clone() const
{
    return boost::static_pointer_cast<HSSMargin, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSMargin::cloneImpl() const
{
    return HSSClonable::p(new HSSMargin(*this));
}

HSSMargin::~HSSMargin()
{

}

std::string HSSMargin::toString()
{
    return "HSSMargin";
}

std::string HSSMargin::defaultObjectType()
{
    return "margin";
}

std::string HSSMargin::defaultObjectType(std::string property)
{
    return HSSObject::defaultObjectType(property);
}

bool HSSMargin::isKeyword(std::string value, std::string property)
{
    return false;
}

void HSSMargin::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name)
    {
    case HSSObservablePropertySize:
        this->setDSize(value);
        break;
    case HSSObservablePropertyLeft:
        this->setDLeft(value);
        break;
    case HSSObservablePropertyTop:
        this->setDTop(value);
        break;
    case HSSObservablePropertyRight:
        this->setDRight(value);
        break;
    case HSSObservablePropertyBottom:
        this->setDBottom(value);
        break;

    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

const HSSParserNode::p HSSMargin::getDSize() const
{
    return this->dSize;
}

void HSSMargin::setDSize(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTop)
        {
            this->observedTop->removeObserver(this->observedTopProperty, HSSObservablePropertySize, this);
        }
        this->top = this->_setLDProperty(
                                         &HSSMargin::topChanged,
                                         value,
                                         100.,
                                         HSSObservablePropertySize,
                                         this,
                                         HSSObservablePropertyValue,
                                         this->observedTop,
                                         this->observedTopProperty,
                                         NULL);
        if (this->observedRight)
        {
            this->observedRight->removeObserver(this->observedRightProperty, HSSObservablePropertySize, this);
        }
        this->right = this->_setLDProperty(
                                           &HSSMargin::rightChanged,
                                           value,
                                           100.,
                                           HSSObservablePropertySize,
                                           this,
                                           HSSObservablePropertyValue,
                                           this->observedRight,
                                           this->observedRightProperty,
                                           NULL);
        if (this->observedBottom)
        {
            this->observedBottom->removeObserver(this->observedBottomProperty, HSSObservablePropertySize, this);
        }
        this->bottom = this->_setLDProperty(
                                            &HSSMargin::bottomChanged,
                                            value,
                                            100.,
                                            HSSObservablePropertySize,
                                            this,
                                            HSSObservablePropertyValue,
                                            this->observedBottom,
                                            this->observedBottomProperty,
                                            NULL);
        if (this->observedLeft)
        {
            this->observedLeft->removeObserver(this->observedLeftProperty, HSSObservablePropertySize, this);
        }
        this->left = this->_setLDProperty(
                                          &HSSMargin::leftChanged,
                                          value,
                                          100.,
                                          HSSObservablePropertySize,
                                          this,
                                          HSSObservablePropertyValue,
                                          this->observedLeft,
                                          this->observedLeftProperty,
                                          NULL);
        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSMargin", "Invalid value for size of @margin object " + this->name));
    }

    this->dSize = value;
}

HSSUnit HSSMargin::getTop() const
{
    return this->top;
}

void HSSMargin::setDTop(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTop)
        {
            this->observedTop->removeObserver(this->observedTopProperty, HSSObservablePropertyTop, this);
        }
        this->top = this->_setLDProperty(
                                         &HSSMargin::topChanged,
                                         value,
                                         100.,
                                         HSSObservablePropertyTop,
                                         this,
                                         HSSObservablePropertyValue,
                                         this->observedTop,
                                         this->observedTopProperty,
                                         NULL);
        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSMargin", "Invalid value for corners of @margin object " + this->name));
    }

    this->notifyObservers(HSSObservablePropertyTop, &this->top);
}

void HSSMargin::topChanged(AXR::HSSObservableProperty source, void *data)
{
    this->top = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyTop, &this->top);
}

HSSUnit HSSMargin::getRight() const
{
    return this->right;
}

void HSSMargin::setDRight(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedRight)
        {
            this->observedRight->removeObserver(this->observedRightProperty, HSSObservablePropertyRight, this);
        }
        this->right = this->_setLDProperty(
                                           &HSSMargin::rightChanged,
                                           value,
                                           100.,
                                           HSSObservablePropertyRight,
                                           this,
                                           HSSObservablePropertyValue,
                                           this->observedRight,
                                           this->observedRightProperty,
                                           NULL);
        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSMargin", "Invalid value for corners of @margin object " + this->name));
    }

    this->notifyObservers(HSSObservablePropertyRight, &this->right);
}

void HSSMargin::rightChanged(AXR::HSSObservableProperty source, void *data)
{
    this->right = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyRight, &this->right);
}

HSSUnit HSSMargin::getBottom() const
{
    return this->bottom;
}

void HSSMargin::setDBottom(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedBottom)
        {
            this->observedBottom->removeObserver(this->observedBottomProperty, HSSObservablePropertyBottom, this);
        }
        this->bottom = this->_setLDProperty(
                                            &HSSMargin::bottomChanged,
                                            value,
                                            100.,
                                            HSSObservablePropertyBottom,
                                            this,
                                            HSSObservablePropertyValue,
                                            this->observedBottom,
                                            this->observedBottomProperty,
                                            NULL);
        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSMargin", "Invalid value for corners of @margin object " + this->name));
    }

    this->notifyObservers(HSSObservablePropertyBottom, &this->bottom);
}

void HSSMargin::bottomChanged(AXR::HSSObservableProperty source, void *data)
{
    this->bottom = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyBottom, &this->bottom);
}

HSSUnit HSSMargin::getLeft() const
{
    return this->left;
}

void HSSMargin::setDLeft(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedLeft)
        {
            this->observedLeft->removeObserver(this->observedLeftProperty, HSSObservablePropertyLeft, this);
        }
        this->left = this->_setLDProperty(
                                          &HSSMargin::leftChanged,
                                          value,
                                          100.,
                                          HSSObservablePropertyLeft,
                                          this,
                                          HSSObservablePropertyValue,
                                          this->observedLeft,
                                          this->observedLeftProperty,
                                          NULL);
        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSMargin", "Invalid value for corners of @margin object " + this->name));
    }

    this->notifyObservers(HSSObservablePropertyLeft, &this->left);
}

void HSSMargin::leftChanged(AXR::HSSObservableProperty source, void *data)
{
    this->left = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyLeft, &this->left);
}

long double HSSMargin::_setLDProperty(
                                      void(HSSMargin::*callback)(HSSObservableProperty property, void* data),
                                      HSSParserNode::p value,
                                      long double percentageBase,
                                      HSSObservableProperty observedProperty,
                                      HSSObservable * observedObject,
                                      HSSObservableProperty observedSourceProperty,
                                      HSSObservable * &observedStore,
                                      HSSObservableProperty &observedStoreProperty,
                                      const std::vector<HSSDisplayObject::p> * scope
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
        observedStore = NULL;
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (value);
        ret = percentageValue->getValue(percentageBase);
        if (callback)
        {
            observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSMargin > (this, callback));
            observedStore = observedObject;
            observedStoreProperty = observedProperty;
        }
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setPercentageObserved(observedProperty, observedObject);
        expressionValue->setScope(scope);
        expressionValue->setThisObj(this->getThisObj());
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSMargin > (this, callback));
            observedStore = expressionValue.get();
            observedStoreProperty = HSSObservablePropertyValue;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        fnct->setPercentageBase(percentageBase);
        fnct->setPercentageObserved(observedProperty, observedObject);
        fnct->setScope(scope);
        fnct->setThisObj(this->getThisObj());

        boost::any remoteValue = fnct->evaluate();
        try
        {
            ret = boost::any_cast<long double>(remoteValue);
        }
        catch (boost::bad_any_cast & e)
        {
            ret = 0.;
        }

        if (callback)
        {
            fnct->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSMargin > (this, callback));
            observedStore = fnct.get();
            observedStoreProperty = HSSObservablePropertyValue;
        }
        break;
    }

    default:
        break;
    }

    return ret;
}
