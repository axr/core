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

/**
 *  @todo set defaults
 */

#include <cmath>
#include <QVariant>
#include "AXRDebugging.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSColorStop.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSLinearGradient.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

Q_DECLARE_METATYPE(HSSUnit*)

HSSLinearGradient::HSSLinearGradient(AXRController * controller)
: HSSGradient(HSSGradientTypeLinear, controller)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLinearGradient: creating linear gradient object");
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("endY");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyStartX, QVariant::fromValue(&this->startX));
    this->registerProperty(HSSObservablePropertyStartY, QVariant::fromValue(&this->startY));
    this->registerProperty(HSSObservablePropertyEndX, QVariant::fromValue(&this->endX));
    this->registerProperty(HSSObservablePropertyEndY, QVariant::fromValue(&this->endY));

    this->startX = this->startY = this->endX = this->endY = 0.;

    this->observedStartX = this->observedStartY
            = this->observedEndX = this->observedEndY
            = NULL;
}

HSSLinearGradient::HSSLinearGradient(const HSSLinearGradient & orig)
: HSSGradient(orig)
{
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("endY");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyStartX, QVariant::fromValue(&this->startX));
    this->registerProperty(HSSObservablePropertyStartY, QVariant::fromValue(&this->startY));
    this->registerProperty(HSSObservablePropertyEndX, QVariant::fromValue(&this->endX));
    this->registerProperty(HSSObservablePropertyEndY, QVariant::fromValue(&this->endY));

    this->startX = this->startY = this->endX = endY = 0.;

    this->observedStartX = this->observedStartY
            = this->observedEndX = this->observedEndY
            = NULL;

}

QSharedPointer<HSSLinearGradient> HSSLinearGradient::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLinearGradient: cloning linear gradient object");
    return qSharedPointerCast<HSSLinearGradient> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSLinearGradient::cloneImpl() const
{
    return QSharedPointer<HSSLinearGradient>(new HSSLinearGradient(*this));
}

HSSLinearGradient::~HSSLinearGradient()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLinearGradient: destructing linear gradient object");
    if (this->observedStartX)
    {
        this->observedStartX->removeObserver(this->observedStartXProperty, HSSObservablePropertyStartX, this);
    }
    if (this->observedStartY)
    {
        this->observedStartY->removeObserver(this->observedStartYProperty, HSSObservablePropertyStartY, this);
    }
    if (this->observedEndX)
    {
        this->observedEndX->removeObserver(this->observedEndXProperty, HSSObservablePropertyEndX, this);
    }
    if (this->observedEndY)
    {
        this->observedEndY->removeObserver(this->observedEndYProperty, HSSObservablePropertyEndY, this);
    }
}

AXRString HSSLinearGradient::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSLinearGradient: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSLinearGradient";
    }
}

AXRString HSSLinearGradient::defaultObjectType()
{
    return "linearGradient";
}

AXRString HSSLinearGradient::defaultObjectType(AXRString property)
{
    //    if (property == "bla"){
    //
    //    } else {
    return HSSGradient::defaultObjectType(property);
    //    }
}

bool HSSLinearGradient::isKeyword(AXRString value, AXRString property)
{
    if (value == "top" || value == "bottom")
    {
        if (property == "startX")
        {
            return true;
        }
    }
    else if (value == "left" || value == "right")
    {
        if (property == "startY")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSGradient::isKeyword(value, property);
}

void HSSLinearGradient::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertyStartX:
        this->setDStartX(value);
        break;
    case HSSObservablePropertyStartY:
        this->setDStartY(value);
        break;
    case HSSObservablePropertyEndX:
        this->setDEndX(value);
        break;
    case HSSObservablePropertyEndY:
        this->setDEndY(value);
        break;
    default:
        HSSGradient::setProperty(name, value);
        break;
    }
}

HSSUnit HSSLinearGradient::getStartX()
{
    return this->startX;
}

QSharedPointer<HSSParserNode> HSSLinearGradient::getDStartX()
{
    return this->dStartX;
}

void HSSLinearGradient::setDStartX(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    {
        this->dStartX = value;

        HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
        if (this->observedStartX)
        {
            this->observedStartX->removeObserver(this->observedStartXProperty, HSSObservablePropertyStartX, this);
        }

        this->startX = this->_evaluatePropertyValue(
                                            &HSSLinearGradient::startXChanged,
                                            value,
                                            this->getThisObj()->getWidth(),
                                            observedProperty,
                                            this->getThisObj().data(),
                                            HSSObservablePropertyStartX,
                                            this->observedStartX,
                                            this->observedStartXProperty
                                            );

        break;
    }

    default:
        break;
    }
}

void HSSLinearGradient::startXChanged(HSSObservableProperty source, void *data)
{
    switch (this->dStartX->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->startX = *(HSSUnit*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dStartX);
        this->startX = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyStartX, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSLinearGradient::getStartY()
{
    return this->startY;
}

QSharedPointer<HSSParserNode> HSSLinearGradient::getDStartY()
{
    return this->dStartY;
}

void HSSLinearGradient::setDStartY(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    {
        this->dStartY = value;

        HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
        if (this->observedStartY)
        {
            this->observedStartY->removeObserver(this->observedStartYProperty, HSSObservablePropertyStartY, this);
        }

        this->startY = this->_evaluatePropertyValue(
                                            &HSSLinearGradient::startYChanged,
                                            value,
                                            this->getThisObj()->getHeight(),
                                            observedProperty,
                                            this->getThisObj().data(),
                                            HSSObservablePropertyStartY,
                                            this->observedStartY,
                                            this->observedStartYProperty
                                            );

        break;
    }

    default:
        break;
    }
}

void HSSLinearGradient::startYChanged(HSSObservableProperty source, void *data)
{
    switch (this->dStartY->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->startY = *(HSSUnit*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dStartY);
        this->startY = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyStartY, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSLinearGradient::getEndX()
{
    return this->endX;
}

QSharedPointer<HSSParserNode> HSSLinearGradient::getDEndX()
{
    return this->dEndX;
}

void HSSLinearGradient::setDEndX(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    {
        this->dEndX = value;

        HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
        if (this->observedEndX)
        {
            this->observedEndX->removeObserver(this->observedEndXProperty, HSSObservablePropertyEndX, this);
        }

        this->endX = this->_evaluatePropertyValue(
                                          &HSSLinearGradient::endXChanged,
                                          value,
                                          this->getThisObj()->getWidth(),
                                          observedProperty,
                                          this->getThisObj().data(),
                                          HSSObservablePropertyEndX,
                                          this->observedEndX,
                                          this->observedEndXProperty
                                          );

        break;
    }

    default:
        break;
    }
}

void HSSLinearGradient::endXChanged(HSSObservableProperty source, void *data)
{
    switch (this->dEndX->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->endX = *(HSSUnit*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dEndX);
        this->endX = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyEndX, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSLinearGradient::getEndY()
{
    return this->endY;
}

QSharedPointer<HSSParserNode> HSSLinearGradient::getDEndY()
{
    return this->dEndY;
}

void HSSLinearGradient::setDEndY(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    {
        this->dEndY = value;

        HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
        if (this->observedEndY)
        {
            this->observedEndY->removeObserver(this->observedEndYProperty, HSSObservablePropertyEndY, this);
        }

        this->endY = this->_evaluatePropertyValue(
                                          &HSSLinearGradient::endYChanged,
                                          value,
                                          this->getThisObj()->getHeight(),
                                          observedProperty,
                                          this->getThisObj().data(),
                                          HSSObservablePropertyEndY,
                                          this->observedEndY,
                                          this->observedEndYProperty
                                          );

        break;
    }

    default:
        break;
    }
}

void HSSLinearGradient::endYChanged(HSSObservableProperty source, void *data)
{
    switch (this->dEndY->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->endY = *(HSSUnit*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dEndY);
        this->endY = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyEndY, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSLinearGradient::_evaluatePropertyValue(
                                              void(HSSLinearGradient::*callback)(HSSObservableProperty property, void* data),
                                              QSharedPointer<HSSParserNode> value,
                                              HSSUnit percentageBase,
                                              HSSObservableProperty observedProperty,
                                              HSSObservable * observedObject,
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
        QSharedPointer<HSSNumberConstant> numberValue = qSharedPointerCast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (value);
        ret = percentageValue->getValue(percentageBase);
        if (callback)
        {
            observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSLinearGradient > (this, callback));
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
        expressionValue->setScope(this->scope);
        expressionValue->setThisObj(this->getThisObj());
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSLinearGradient > (this, callback));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    default:
        AXRWarning("HSSLinearGradient", "Unknown parser node type while setting value for HSSLinearGradient property").raise();
        break;
    }

    return ret;
}
