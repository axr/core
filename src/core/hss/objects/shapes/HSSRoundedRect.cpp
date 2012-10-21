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

#include "AXRDebugging.h"
#include "AXRWarning.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRoundedRect.h"

using namespace AXR;

HSSRoundedRect::HSSRoundedRect(AXRController * controller)
: HSSShape(HSSShapeTypeRoundedRect, controller)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRoundedRect: creating rounded rectangle object");
    this->cornerTL = this->cornerTR = this->cornerBR = this->cornerBL = 0.;
    this->observedTLCorner = this->observedTRCorner = this->observedBRCorner = this->observedBLCorner = NULL;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("corners");
    this->setShorthandProperties(shorthandProperties);
}

HSSRoundedRect::HSSRoundedRect(const HSSRoundedRect & orig)
: HSSShape(orig)
{
    this->cornerTL = this->cornerTR = this->cornerBR = this->cornerBL = 0.;
    this->observedTLCorner = this->observedTRCorner = this->observedBRCorner = this->observedBLCorner = NULL;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("corners");
    this->setShorthandProperties(shorthandProperties);
}

HSSRoundedRect::p HSSRoundedRect::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRoundedRect: cloning rounded rectangle object");
    return qSharedPointerCast<HSSRoundedRect>(this->cloneImpl());
}

HSSClonable::p HSSRoundedRect::cloneImpl() const
{
    return HSSRoundedRect::p(new HSSRoundedRect(*this));
}

HSSRoundedRect::~HSSRoundedRect()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRoundedRect: destructing rounded rectangle object");
}

AXRString HSSRoundedRect::toString()
{
    return "HSSRoundedRect";
}

AXRString HSSRoundedRect::defaultObjectType()
{
    return "roundedRect";
}

AXRString HSSRoundedRect::defaultObjectType(AXRString property)
{
    return HSSShape::defaultObjectType(property);
}

bool HSSRoundedRect::isKeyword(AXRString value, AXRString property)
{
    return HSSShape::isKeyword(value, property);
}

void HSSRoundedRect::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name)
    {
    case HSSObservablePropertyCorners:
        this->setDCorners(value);
        break;
    case HSSObservablePropertyLeft:
        this->setDLeft(value);
        break;
    case HSSObservablePropertyLeftTop:
        this->setDLeftTop(value);
        break;
    case HSSObservablePropertyTop:
        this->setDTop(value);
        break;
    case HSSObservablePropertyRightTop:
        this->setDRightTop(value);
        break;
    case HSSObservablePropertyRight:
        this->setDRight(value);
        break;
    case HSSObservablePropertyRightBottom:
        this->setDRightBottom(value);
        break;
    case HSSObservablePropertyBottom:
        this->setDBottom(value);
        break;
    case HSSObservablePropertyLeftBottom:
        this->setDLeftBottom(value);
        break;

    default:
        HSSShape::setProperty(name, value);
        break;
    }
}

HSSMultipleValue::p HSSRoundedRect::getCorners()
{
    return this->corners;
}

void HSSRoundedRect::setDCorners(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTLCorner)
        {
            this->observedTLCorner->removeObserver(this->observedTLCornerProperty, HSSObservablePropertyCorners, this);
        }
        this->cornerTL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTLCorner,
                                              this->observedTLCornerProperty,
                                              this->scope);
        if (this->observedTRCorner)
        {
            this->observedTRCorner->removeObserver(this->observedTRCornerProperty, HSSObservablePropertyCorners, this);
        }
        this->cornerTR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTRCorner,
                                              this->observedTRCornerProperty,
                                              this->scope);
        if (this->observedBRCorner)
        {
            this->observedBRCorner->removeObserver(this->observedBRCornerProperty, HSSObservablePropertyCorners, this);
        }
        this->cornerBR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBRCorner,
                                              this->observedBRCornerProperty,
                                              this->scope);
        if (this->observedBLCorner)
        {
            this->observedBLCorner->removeObserver(this->observedBLCornerProperty, HSSObservablePropertyCorners, this);
        }
        this->cornerBL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBLCorner,
                                              this->observedBLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->dCorners = value;
    HSSMultipleValue::p newCorners(new HSSMultipleValue(this->getController()));
    newCorners->add(value);
    this->corners = newCorners;

    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::setDLeft(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTLCorner)
        {
            this->observedTLCorner->removeObserver(this->observedTLCornerProperty, HSSObservablePropertyLeft, this);
        }
        this->cornerTL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTLCorner,
                                              this->observedTLCornerProperty,
                                              this->scope);
        if (this->observedBLCorner)
        {
            this->observedBLCorner->removeObserver(this->observedBLCornerProperty, HSSObservablePropertyLeft, this);
        }
        this->cornerBL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBLCorner,
                                              this->observedBLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTL);
    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBL);
}

void HSSRoundedRect::setDLeftTop(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTLCorner)
        {
            this->observedTLCorner->removeObserver(this->observedTLCornerProperty, HSSObservablePropertyLeftTop, this);
        }
        this->cornerTL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTLCorner,
                                              this->observedTLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTL);
}

void HSSRoundedRect::setDTop(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTLCorner)
        {
            this->observedTLCorner->removeObserver(this->observedTLCornerProperty, HSSObservablePropertyTop, this);
        }
        this->cornerTL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTLCorner,
                                              this->observedTLCornerProperty,
                                              this->scope);
        if (this->observedTRCorner)
        {
            this->observedTRCorner->removeObserver(this->observedTRCornerProperty, HSSObservablePropertyTop, this);
        }
        this->cornerTR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTRCorner,
                                              this->observedTRCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTL);
    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTR);
}

void HSSRoundedRect::setDRightTop(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTRCorner)
        {
            this->observedTRCorner->removeObserver(this->observedTRCornerProperty, HSSObservablePropertyRightTop, this);
        }
        this->cornerTR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTRCorner,
                                              this->observedTRCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTR);
}

void HSSRoundedRect::setDRight(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTRCorner)
        {
            this->observedTRCorner->removeObserver(this->observedTRCornerProperty, HSSObservablePropertyRight, this);
        }
        this->cornerTR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTRCorner,
                                              this->observedTRCornerProperty,
                                              this->scope);
        if (this->observedBRCorner)
        {
            this->observedBRCorner->removeObserver(this->observedBRCornerProperty, HSSObservablePropertyRight, this);
        }
        this->cornerBR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBRCorner,
                                              this->observedBRCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTR);
    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBR);
}

void HSSRoundedRect::setDRightBottom(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedBRCorner)
        {
            this->observedBRCorner->removeObserver(this->observedBRCornerProperty, HSSObservablePropertyRightBottom, this);
        }
        this->cornerBR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBRCorner,
                                              this->observedBRCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBR);
}

void HSSRoundedRect::setDBottom(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedBRCorner)
        {
            this->observedBRCorner->removeObserver(this->observedBRCornerProperty, HSSObservablePropertyBottom, this);
        }
        this->cornerBR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBRCorner,
                                              this->observedBRCornerProperty,
                                              this->scope);
        if (this->observedBLCorner)
        {
            this->observedBLCorner->removeObserver(this->observedBLCornerProperty, HSSObservablePropertyBottom, this);
        }
        this->cornerBL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBLCorner,
                                              this->observedBLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBL);
    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBR);
}

void HSSRoundedRect::setDLeftBottom(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedBLCorner)
        {
            this->observedBLCorner->removeObserver(this->observedBLCornerProperty, HSSObservablePropertyLeftBottom, this);
        }
        this->cornerBL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBLCorner,
                                              this->observedBLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBL);
}

void HSSRoundedRect::cornerTLChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerTL = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerTRChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerTR = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerBRChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerBR = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerBLChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerBL = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height)
{
    this->createRoundedRect(path, x, y, width, height, 0.);
}

void HSSRoundedRect::createRoundedRect(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, HSSUnit offset)
{
    // Draw top-left corner
    QRectF topLeftBounds(x, y, (cornerTL*2+offset), (cornerTL*2+offset));
    QRectF bottomLeftBounds(x, y + height - (cornerBL*2+offset), (cornerBL*2+offset), (cornerBL*2+offset));
    QRectF bottomRightBounds(x + width - (cornerBR*2+offset), y + height - (cornerBR*2+offset), (cornerBR*2+offset), (cornerBR*2+offset));
    QRectF topRightBounds(x + width - (cornerTR*2+offset), y, (cornerTR*2+offset), (cornerTR*2+offset));

    QVector<QRectF> corners;
    corners << topLeftBounds << bottomLeftBounds << bottomRightBounds << topRightBounds;

    path.arcMoveTo(corners[0], 90);

    for (int i = 1; i <= corners.size(); ++i)
    {
        path.arcTo(corners[i - 1], i * 90, 90);
    }

    path.closeSubpath();
}

HSSUnit HSSRoundedRect::_evaluatePropertyValue(
                                           void(HSSRoundedRect::*callback)(HSSObservableProperty property, void* data),
                                           HSSParserNode::p value,
                                           HSSUnit percentageBase,
                                           HSSObservableProperty observedProperty,
                                           HSSObservable * observedObject,
                                           HSSObservableProperty observedSourceProperty,
                                           HSSObservable * &observedStore,
                                           HSSObservableProperty &observedStoreProperty,
                                           const std::vector<HSSDisplayObject::p> * scope
                                           )
{
    HSSUnit ret;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        HSSNumberConstant::p numberValue = qSharedPointerCast<HSSNumberConstant>(value);
        ret = numberValue->getValue();
        observedStore = NULL;
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant>(value);
        ret = percentageValue->getValue(percentageBase);
        if (callback)
        {
            observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSRoundedRect > (this, callback));
            observedStore = observedObject;
            observedStoreProperty = observedProperty;
        }
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = qSharedPointerCast<HSSExpression>(value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setPercentageObserved(observedProperty, observedObject);
        expressionValue->setScope(scope);
        expressionValue->setThisObj(this->getThisObj());
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSRoundedRect > (this, callback));
            observedStore = expressionValue.data();
            observedStoreProperty = HSSObservablePropertyValue;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction>(value)->clone();
        fnct->setPercentageBase(percentageBase);
        fnct->setPercentageObserved(observedProperty, observedObject);
        fnct->setScope(scope);
        fnct->setThisObj(this->getThisObj());

        QVariant remoteValue = fnct->evaluate();
        if (remoteValue.canConvert<HSSUnit>())
        {
            ret = remoteValue.value<HSSUnit>();
        }

        if (callback)
        {
            fnct->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSRoundedRect > (this, callback));
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
