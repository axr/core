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
    this->percentageBaseX = 0.;
    this->percentageBaseY = 0.;
    this->percentageObserved = NULL;
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
    this->percentageBaseX = orig.percentageBaseX;
    this->percentageBaseY = orig.percentageBaseY;
    this->percentageObserved = NULL;
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
        this->dSize = value;
        this->dTop = value;
        if (this->observedTop)
        {
            this->observedTop->removeObserver(this->observedTopProperty, HSSObservablePropertySize, this);
        }
        this->top = this->_evaluatePropertyValue(
                                         &HSSMargin::topChanged,
                                         value,
                                         this->percentageBaseY,
                                         HSSObservablePropertyInnerHeight,
                                         this->percentageObserved,
                                         HSSObservablePropertyTop,
                                         this->observedTop,
                                         this->observedTopProperty,
                                         HSSSimpleSelection::null());
        this->notifyObservers(HSSObservablePropertyTop, &this->top);

        this->dRight = value;
        if (this->observedRight)
        {
            this->observedRight->removeObserver(this->observedRightProperty, HSSObservablePropertySize, this);
        }
        this->right = this->_evaluatePropertyValue(
                                           &HSSMargin::rightChanged,
                                           value,
                                           this->percentageBaseX,
                                           HSSObservablePropertyInnerWidth,
                                           this->percentageObserved,
                                           HSSObservablePropertyRight,
                                           this->observedRight,
                                           this->observedRightProperty,
                                           HSSSimpleSelection::null());
        this->notifyObservers(HSSObservablePropertyRight, &this->right);

        this->dBottom = value;
        if (this->observedBottom)
        {
            this->observedBottom->removeObserver(this->observedBottomProperty, HSSObservablePropertySize, this);
        }
        this->bottom = this->_evaluatePropertyValue(
                                            &HSSMargin::bottomChanged,
                                            value,
                                            this->percentageBaseY,
                                            HSSObservablePropertyInnerHeight,
                                            this->percentageObserved,
                                            HSSObservablePropertyBottom,
                                            this->observedBottom,
                                            this->observedBottomProperty,
                                            HSSSimpleSelection::null());
        this->notifyObservers(HSSObservablePropertyBottom, &this->bottom);

        this->dLeft = value;
        if (this->observedLeft)
        {
            this->observedLeft->removeObserver(this->observedLeftProperty, HSSObservablePropertySize, this);
        }
        this->left = this->_evaluatePropertyValue(
                                          &HSSMargin::leftChanged,
                                          value,
                                          this->percentageBaseX,
                                          HSSObservablePropertyInnerWidth,
                                          this->percentageObserved,
                                          HSSObservablePropertyLeft,
                                          this->observedLeft,
                                          this->observedLeftProperty,
                                          HSSSimpleSelection::null());
        this->notifyObservers(HSSObservablePropertyLeft, &this->left);
        break;
    }

    default:
        throw AXRWarning("HSSMargin", "Invalid value for size of @margin object " + this->name);
    }
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
        this->dTop = value;
        if (this->observedTop)
        {
            this->observedTop->removeObserver(this->observedTopProperty, HSSObservablePropertyTop, this);
        }
        this->top = this->_evaluatePropertyValue(
                                         &HSSMargin::topChanged,
                                         value,
                                         this->percentageBaseY,
                                         HSSObservablePropertyInnerHeight,
                                         this->percentageObserved,
                                         HSSObservablePropertyTop,
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
    switch (this->dTop->getType()) {
        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSPercentageConstant> topValue = qSharedPointerCast<HSSPercentageConstant > (this->dTop);
            this->top = topValue->getValue(*(HSSUnit*) data);
            break;
        }
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
        {
            this->top = *(HSSUnit*) data;
            break;
        }

        default:
            break;
    }
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
        this->dRight = value;
        if (this->observedRight)
        {
            this->observedRight->removeObserver(this->observedRightProperty, HSSObservablePropertyRight, this);
        }
        this->right = this->_evaluatePropertyValue(
                                           &HSSMargin::rightChanged,
                                           value,
                                           this->percentageBaseX,
                                           HSSObservablePropertyInnerWidth,
                                           this->percentageObserved,
                                           HSSObservablePropertyRight,
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
    switch (this->dRight->getType()) {
        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSPercentageConstant> rightValue = qSharedPointerCast<HSSPercentageConstant > (this->dRight);
            this->right = rightValue->getValue(*(HSSUnit*) data);
            break;
        }
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
        {
            this->right = *(HSSUnit*) data;
            break;
        }

        default:
            break;
    }
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
        this->dBottom = value;
        if (this->observedBottom)
        {
            this->observedBottom->removeObserver(this->observedBottomProperty, HSSObservablePropertyBottom, this);
        }
        this->bottom = this->_evaluatePropertyValue(
                                            &HSSMargin::bottomChanged,
                                            value,
                                            this->percentageBaseY,
                                            HSSObservablePropertyInnerHeight,
                                            this->percentageObserved,
                                            HSSObservablePropertyBottom,
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
    switch (this->dBottom->getType()) {
        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSPercentageConstant> bottomValue = qSharedPointerCast<HSSPercentageConstant > (this->dBottom);
            this->bottom = bottomValue->getValue(*(HSSUnit*) data);
            break;
        }
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
        {
            this->bottom = *(HSSUnit*) data;
            break;
        }

        default:
            break;
    }
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
        this->dLeft = value;
        if (this->observedLeft)
        {
            this->observedLeft->removeObserver(this->observedLeftProperty, HSSObservablePropertyLeft, this);
        }
        this->left = this->_evaluatePropertyValue(
                                          &HSSMargin::leftChanged,
                                          value,
                                          this->percentageBaseX,
                                          HSSObservablePropertyInnerWidth,
                                          this->percentageObserved,
                                          HSSObservablePropertyLeft,
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
    switch (this->dLeft->getType()) {
        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSPercentageConstant> leftValue = qSharedPointerCast<HSSPercentageConstant > (this->dLeft);
            this->left = leftValue->getValue(*(HSSUnit*) data);
            break;
        }
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
        {
            this->left = *(HSSUnit*) data;
            break;
        }

        default:
            break;
    }
    this->notifyObservers(HSSObservablePropertyLeft, &this->left);
}

void HSSMargin::setPercentageBaseY(HSSUnit value)
{
    this->percentageBaseY = value;

    //propagate values
    if (this->dTop)
    {
        switch (this->dTop->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> topExpression = qSharedPointerCast<HSSExpression > (this->dTop);
                topExpression->setPercentageBase(value);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> topFunction = qSharedPointerCast<HSSFunction > (this->dTop);
                topFunction->setPercentageBase(value);
                break;
            }

            default:
                break;
        }
    }

    if (this->dBottom)
    {
        switch (this->dBottom->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> bottomExpression = qSharedPointerCast<HSSExpression > (this->dBottom);
                bottomExpression->setPercentageBase(value);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> bottomFunction = qSharedPointerCast<HSSFunction > (this->dBottom);
                bottomFunction->setPercentageBase(value);
                break;
            }

            default:
                break;
        }
    }
}

void HSSMargin::setPercentageBaseX(HSSUnit value)
{
    this->percentageBaseX = value;

    if (this->dRight)
    {
        switch (this->dRight->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> rightExpression = qSharedPointerCast<HSSExpression > (this->dRight);
                rightExpression->setPercentageBase(value);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> rightFunction = qSharedPointerCast<HSSFunction > (this->dRight);
                rightFunction->setPercentageBase(value);
                break;
            }

            default:
                break;
        }
    }

    if (this->dLeft)
    {
        switch (this->dLeft->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> leftExpression = qSharedPointerCast<HSSExpression > (this->dLeft);
                leftExpression->setPercentageBase(value);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> leftFunction = qSharedPointerCast<HSSFunction > (this->dLeft);
                leftFunction->setPercentageBase(value);
                break;
            }

            default:
                break;
        }
    }
}

void HSSMargin::setPercentageObserved(HSSDisplayObject *observed)
{
    this->percentageObserved = observed;

    //propagate values
    if (this->dTop)
    {
        switch (this->dTop->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> topExpression = qSharedPointerCast<HSSExpression > (this->dTop);
                topExpression->setPercentageObserved(HSSObservablePropertyInnerHeight, observed);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> topFunction = qSharedPointerCast<HSSFunction > (this->dTop);
                topFunction->setPercentageObserved(HSSObservablePropertyInnerHeight, observed);
                break;
            }

            default:
                break;
        }
    }

    if (this->dBottom)
    {
        switch (this->dBottom->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> bottomExpression = qSharedPointerCast<HSSExpression > (this->dBottom);
                bottomExpression->setPercentageObserved(HSSObservablePropertyInnerHeight, observed);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> bottomFunction = qSharedPointerCast<HSSFunction > (this->dBottom);
                bottomFunction->setPercentageObserved(HSSObservablePropertyInnerHeight, observed);
                break;
            }

            default:
                break;
        }
    }

    if (this->dRight)
    {
        switch (this->dRight->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> rightExpression = qSharedPointerCast<HSSExpression > (this->dRight);
                rightExpression->setPercentageObserved(HSSObservablePropertyInnerWidth, observed);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> rightFunction = qSharedPointerCast<HSSFunction > (this->dRight);
                rightFunction->setPercentageObserved(HSSObservablePropertyInnerWidth, observed);
                break;
            }

            default:
                break;
        }
    }

    if (this->dLeft)
    {
        switch (this->dLeft->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> leftExpression = qSharedPointerCast<HSSExpression > (this->dLeft);
                leftExpression->setPercentageObserved(HSSObservablePropertyInnerWidth, observed);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> leftFunction = qSharedPointerCast<HSSFunction > (this->dLeft);
                leftFunction->setPercentageObserved(HSSObservablePropertyInnerWidth, observed);
                break;
            }

            default:
                break;
        }
    }
}

void HSSMargin::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSObject::setScope(newScope);
    //propagate values
    if (this->dTop)
    {
        switch (this->dTop->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> topExpression = qSharedPointerCast<HSSExpression > (this->dTop);
                topExpression->setScope(newScope);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> topFunction = qSharedPointerCast<HSSFunction > (this->dTop);
                topFunction->setScope(newScope);
                break;
            }

            default:
                break;
        }
    }

    if (this->dBottom)
    {
        switch (this->dBottom->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> bottomExpression = qSharedPointerCast<HSSExpression > (this->dBottom);
                bottomExpression->setScope(newScope);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> bottomFunction = qSharedPointerCast<HSSFunction > (this->dBottom);
                bottomFunction->setScope(newScope);
                break;
            }

            default:
                break;
        }
    }

    if (this->dRight)
    {
        switch (this->dRight->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> rightExpression = qSharedPointerCast<HSSExpression > (this->dRight);
                rightExpression->setScope(newScope);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> rightFunction = qSharedPointerCast<HSSFunction > (this->dRight);
                rightFunction->setScope(newScope);
                break;
            }

            default:
                break;
        }
    }

    if (this->dLeft)
    {
        switch (this->dLeft->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> leftExpression = qSharedPointerCast<HSSExpression > (this->dLeft);
                leftExpression->setScope(newScope);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> leftFunction = qSharedPointerCast<HSSFunction > (this->dLeft);
                leftFunction->setScope(newScope);
                break;
            }

            default:
                break;
        }
    }
}

void HSSMargin::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    HSSObject::setThisObj(value);
    //propagate values
    if (this->dTop)
    {
        switch (this->dTop->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> topExpression = qSharedPointerCast<HSSExpression > (this->dTop);
                topExpression->setThisObj(value);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> topFunction = qSharedPointerCast<HSSFunction > (this->dTop);
                topFunction->setThisObj(value);
                break;
            }

            default:
                break;
        }
    }

    if (this->dBottom)
    {
        switch (this->dBottom->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> bottomExpression = qSharedPointerCast<HSSExpression > (this->dBottom);
                bottomExpression->setThisObj(value);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> bottomFunction = qSharedPointerCast<HSSFunction > (this->dBottom);
                bottomFunction->setThisObj(value);
                break;
            }

            default:
                break;
        }
    }

    if (this->dRight)
    {
        switch (this->dRight->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> rightExpression = qSharedPointerCast<HSSExpression > (this->dRight);
                rightExpression->setThisObj(value);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> rightFunction = qSharedPointerCast<HSSFunction > (this->dRight);
                rightFunction->setThisObj(value);
                break;
            }

            default:
                break;
        }
    }

    if (this->dLeft)
    {
        switch (this->dLeft->getType()) {
            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> leftExpression = qSharedPointerCast<HSSExpression > (this->dLeft);
                leftExpression->setThisObj(value);
                break;
            }
            case HSSParserNodeTypeFunctionCall:
            {
                QSharedPointer<HSSFunction> leftFunction = qSharedPointerCast<HSSFunction > (this->dLeft);
                leftFunction->setThisObj(value);
                break;
            }

            default:
                break;
        }
    }
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
