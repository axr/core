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

#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSMargin.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

HSSMargin::HSSMargin(AXRController * controller)
: HSSObject(HSSObjectTypeMargin, controller)
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

QSharedPointer<HSSMargin> HSSMargin::clone() const
{
    return qSharedPointerCast<HSSMargin> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSMargin::cloneImpl() const
{
    return QSharedPointer<HSSMargin>(new HSSMargin(*this));
}

HSSMargin::~HSSMargin()
{

}

AXRString HSSMargin::toString()
{
    return "HSSMargin";
}

AXRString HSSMargin::defaultObjectType()
{
    return "margin";
}

AXRString HSSMargin::defaultObjectType(AXRString property)
{
    return HSSObject::defaultObjectType(property);
}

bool HSSMargin::isKeyword(AXRString value, AXRString property)
{
    return false;
}

void HSSMargin::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
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

const QSharedPointer<HSSParserNode> HSSMargin::getDSize() const
{
    return this->dSize;
}

void HSSMargin::setDSize(QSharedPointer<HSSParserNode> value)
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
        this->top = this->_evaluatePropertyValue(
                                         &HSSMargin::topChanged,
                                         value,
                                         100.,
                                         HSSObservablePropertySize,
                                         this,
                                         HSSObservablePropertyValue,
                                         this->observedTop,
                                         this->observedTopProperty,
                                         HSSSimpleSelection::null());
        if (this->observedRight)
        {
            this->observedRight->removeObserver(this->observedRightProperty, HSSObservablePropertySize, this);
        }
        this->right = this->_evaluatePropertyValue(
                                           &HSSMargin::rightChanged,
                                           value,
                                           100.,
                                           HSSObservablePropertySize,
                                           this,
                                           HSSObservablePropertyValue,
                                           this->observedRight,
                                           this->observedRightProperty,
                                           HSSSimpleSelection::null());
        if (this->observedBottom)
        {
            this->observedBottom->removeObserver(this->observedBottomProperty, HSSObservablePropertySize, this);
        }
        this->bottom = this->_evaluatePropertyValue(
                                            &HSSMargin::bottomChanged,
                                            value,
                                            100.,
                                            HSSObservablePropertySize,
                                            this,
                                            HSSObservablePropertyValue,
                                            this->observedBottom,
                                            this->observedBottomProperty,
                                            HSSSimpleSelection::null());
        if (this->observedLeft)
        {
            this->observedLeft->removeObserver(this->observedLeftProperty, HSSObservablePropertySize, this);
        }
        this->left = this->_evaluatePropertyValue(
                                          &HSSMargin::leftChanged,
                                          value,
                                          100.,
                                          HSSObservablePropertySize,
                                          this,
                                          HSSObservablePropertyValue,
                                          this->observedLeft,
                                          this->observedLeftProperty,
                                          HSSSimpleSelection::null());
        break;
    }

    default:
        throw AXRWarning("HSSMargin", "Invalid value for size of @margin object " + this->name);
    }

    this->dSize = value;
}

HSSUnit HSSMargin::getTop() const
{
    return this->top;
}

void HSSMargin::setDTop(QSharedPointer<HSSParserNode> value)
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
        this->top = this->_evaluatePropertyValue(
                                         &HSSMargin::topChanged,
                                         value,
                                         100.,
                                         HSSObservablePropertyTop,
                                         this,
                                         HSSObservablePropertyValue,
                                         this->observedTop,
                                         this->observedTopProperty,
                                         HSSSimpleSelection::null());
        break;
    }

    default:
        throw AXRWarning("HSSMargin", "Invalid value for corners of @margin object " + this->name);
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

void HSSMargin::setDRight(QSharedPointer<HSSParserNode> value)
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
        this->right = this->_evaluatePropertyValue(
                                           &HSSMargin::rightChanged,
                                           value,
                                           100.,
                                           HSSObservablePropertyRight,
                                           this,
                                           HSSObservablePropertyValue,
                                           this->observedRight,
                                           this->observedRightProperty,
                                           HSSSimpleSelection::null());
        break;
    }

    default:
        throw AXRWarning("HSSMargin", "Invalid value for corners of @margin object " + this->name);
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

void HSSMargin::setDBottom(QSharedPointer<HSSParserNode> value)
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
        this->bottom = this->_evaluatePropertyValue(
                                            &HSSMargin::bottomChanged,
                                            value,
                                            100.,
                                            HSSObservablePropertyBottom,
                                            this,
                                            HSSObservablePropertyValue,
                                            this->observedBottom,
                                            this->observedBottomProperty,
                                            HSSSimpleSelection::null());
        break;
    }

    default:
        throw AXRWarning("HSSMargin", "Invalid value for corners of @margin object " + this->name);
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

void HSSMargin::setDLeft(QSharedPointer<HSSParserNode> value)
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
        this->left = this->_evaluatePropertyValue(
                                          &HSSMargin::leftChanged,
                                          value,
                                          100.,
                                          HSSObservablePropertyLeft,
                                          this,
                                          HSSObservablePropertyValue,
                                          this->observedLeft,
                                          this->observedLeftProperty,
                                          HSSSimpleSelection::null());
        break;
    }

    default:
        throw AXRWarning("HSSMargin", "Invalid value for corners of @margin object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyLeft, &this->left);
}

void HSSMargin::leftChanged(AXR::HSSObservableProperty source, void *data)
{
    this->left = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyLeft, &this->left);
}

HSSUnit HSSMargin::_evaluatePropertyValue(
                                      void(HSSMargin::*callback)(HSSObservableProperty property, void* data),
                                      QSharedPointer<HSSParserNode> value,
                                      HSSUnit percentageBase,
                                      HSSObservableProperty observedProperty,
                                      HSSObservable * observedObject,
                                      HSSObservableProperty observedSourceProperty,
                                      HSSObservable * &observedStore,
                                      HSSObservableProperty &observedStoreProperty,
                                      QSharedPointer<HSSSimpleSelection> scope
                                      )
{
    HSSUnit ret = 0;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        QSharedPointer<HSSNumberConstant> numberValue = qSharedPointerCast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        observedStore = NULL;
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (value);
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
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setPercentageObserved(observedProperty, observedObject);
        expressionValue->setScope(scope);
        expressionValue->setThisObj(this->getThisObj());
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSMargin > (this, callback));
            observedStore = expressionValue.data();
            observedStoreProperty = HSSObservablePropertyValue;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        fnct->setPercentageBase(percentageBase);
        fnct->setPercentageObserved(observedProperty, observedObject);
        fnct->setScope(scope);
        fnct->setThisObj(this->getThisObj());

        QVariant remoteValue = fnct->evaluate();
        if (remoteValue.canConvert<HSSUnit>())
        {
            ret = remoteValue.value<HSSUnit>();
        }
        else
        {
            ret = 0.;
        }

        if (callback)
        {
            fnct->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSMargin > (this, callback));
            observedStore = fnct.data();
            observedStoreProperty = HSSObservablePropertyValue;
        }
        break;
    }

    default:
        break;
    }

    return ret;
}
