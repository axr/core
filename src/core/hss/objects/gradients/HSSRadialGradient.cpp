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
#include "HSSRadialGradient.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

Q_DECLARE_METATYPE(HSSUnit*)

HSSRadialGradient::HSSRadialGradient(AXRController * controller)
: HSSGradient(HSSGradientTypeRadial, controller)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRadialGradient: creating radial gradient object");
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("centerX");
    shorthandProperties.push_back("centerY");
    shorthandProperties.push_back("offsetX");
    shorthandProperties.push_back("offsetY");
    //shorthandProperties.push_back("roundness");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyCenterX, QVariant::fromValue(&this->centerX));
    this->registerProperty(HSSObservablePropertyCenterY, QVariant::fromValue(&this->centerY));
    this->registerProperty(HSSObservablePropertyOffsetX, QVariant::fromValue(&this->offsetX));
    this->registerProperty(HSSObservablePropertyOffsetY, QVariant::fromValue(&this->offsetY));

    this->centerX = this->centerY = this->offsetX = this->offsetY = 0.;

    this->observedCenterX = this->observedCenterY
    = this->observedOffsetX = this->observedOffsetY
    = NULL;
}

HSSRadialGradient::HSSRadialGradient(const HSSRadialGradient & orig)
: HSSGradient(orig)
{
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("centerX");
    shorthandProperties.push_back("centerY");
    shorthandProperties.push_back("offsetX");
    shorthandProperties.push_back("offsetY");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyCenterX, QVariant::fromValue(&this->centerX));
    this->registerProperty(HSSObservablePropertyCenterY, QVariant::fromValue(&this->centerY));
    this->registerProperty(HSSObservablePropertyOffsetX, QVariant::fromValue(&this->offsetX));
    this->registerProperty(HSSObservablePropertyOffsetY, QVariant::fromValue(&this->offsetY));

    this->centerX = this->centerY = this->offsetX = offsetY = 0.;

    this->observedCenterX = this->observedCenterY
    = this->observedOffsetX = this->observedOffsetY
    = NULL;
}

QSharedPointer<HSSRadialGradient> HSSRadialGradient::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRadialGradient: cloning radial gradient object");
    return qSharedPointerCast<HSSRadialGradient> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSRadialGradient::cloneImpl() const
{
    return QSharedPointer<HSSRadialGradient>(new HSSRadialGradient(*this));
}

HSSRadialGradient::~HSSRadialGradient()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRadialGradient: destructing radial gradient object");
    if (this->observedCenterX)
    {
        this->observedCenterX->removeObserver(this->observedCenterXProperty, HSSObservablePropertyCenterX, this);
    }
    if (this->observedCenterY)
    {
        this->observedCenterY->removeObserver(this->observedCenterYProperty, HSSObservablePropertyCenterY, this);
    }
    if (this->observedOffsetX)
    {
        this->observedOffsetX->removeObserver(this->observedOffsetXProperty, HSSObservablePropertyOffsetX, this);
    }
    if (this->observedOffsetY)
    {
        this->observedOffsetY->removeObserver(this->observedOffsetYProperty, HSSObservablePropertyOffsetY, this);
    }
}

AXRString HSSRadialGradient::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSRadialGradient: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSRadialGradient";
    }
}

AXRString HSSRadialGradient::defaultObjectType()
{
    return "radialGradient";
}

AXRString HSSRadialGradient::defaultObjectType(AXRString property)
{
    //    if (property == "bla"){
    //
    //    } else {
    return HSSGradient::defaultObjectType(property);
    //    }
}

bool HSSRadialGradient::isKeyword(AXRString value, AXRString property)
{
    if (value == "top" || value == "bottom")
    {
        if (property == "centerX")
        {
            return true;
        }
    }
    else if (value == "left" || value == "right")
    {
        if (property == "centerY")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSGradient::isKeyword(value, property);
}

void HSSRadialGradient::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
        case HSSObservablePropertyCenterX:
            this->setDCenterX(value);
            break;
        case HSSObservablePropertyCenterY:
            this->setDCenterY(value);
            break;
        case HSSObservablePropertyOffsetX:
            this->setDOffsetX(value);
            break;
        case HSSObservablePropertyOffsetY:
            this->setDOffsetY(value);
            break;
        default:
            HSSGradient::setProperty(name, value);
            break;
    }
}

HSSUnit HSSRadialGradient::getCenterX()
{
    return this->centerX;
}

QSharedPointer<HSSParserNode> HSSRadialGradient::getDCenterX()
{
    return this->dCenterX;
}

void HSSRadialGradient::setDCenterX(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        {
            this->dCenterX = value;

            HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
            if (this->observedCenterX)
            {
                this->observedCenterX->removeObserver(this->observedCenterXProperty, HSSObservablePropertyCenterX, this);
            }

            this->centerX = this->_evaluatePropertyValue(
                                                        &HSSRadialGradient::centerXChanged,
                                                        value,
                                                        this->getThisObj()->getWidth(),
                                                        observedProperty,
                                                        this->getThisObj().data(),
                                                        HSSObservablePropertyCenterX,
                                                        this->observedCenterX,
                                                        this->observedCenterXProperty
                                                        );

            break;
        }

        default:
            break;
    }
}

void HSSRadialGradient::centerXChanged(HSSObservableProperty source, void *data)
{
    switch (this->dCenterX->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->centerX = *(HSSUnit*) data;
            break;

        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dCenterX);
            this->centerX = percentageValue->getValue(*(HSSUnit*) data);
            break;
        }

        default:
            break;
    }

    this->notifyObservers(HSSObservablePropertyCenterX, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSRadialGradient::getCenterY()
{
    return this->centerY;
}

QSharedPointer<HSSParserNode> HSSRadialGradient::getDCenterY()
{
    return this->dCenterY;
}

void HSSRadialGradient::setDCenterY(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        {
            this->dCenterY = value;

            HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
            if (this->observedCenterY)
            {
                this->observedCenterY->removeObserver(this->observedCenterYProperty, HSSObservablePropertyCenterY, this);
            }

            this->centerY = this->_evaluatePropertyValue(
                                                        &HSSRadialGradient::centerYChanged,
                                                        value,
                                                        this->getThisObj()->getHeight(),
                                                        observedProperty,
                                                        this->getThisObj().data(),
                                                        HSSObservablePropertyCenterY,
                                                        this->observedCenterY,
                                                        this->observedCenterYProperty
                                                        );

            break;
        }

        default:
            break;
    }
}

void HSSRadialGradient::centerYChanged(HSSObservableProperty source, void *data)
{
    switch (this->dCenterY->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->centerY = *(HSSUnit*) data;
            break;

        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dCenterY);
            this->centerY = percentageValue->getValue(*(HSSUnit*) data);
            break;
        }

        default:
            break;
    }

    this->notifyObservers(HSSObservablePropertyCenterY, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSRadialGradient::getOffsetX()
{
    return this->offsetX;
}

QSharedPointer<HSSParserNode> HSSRadialGradient::getDOffsetX()
{
    return this->dOffsetX;
}

void HSSRadialGradient::setDOffsetX(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        {
            this->dOffsetX = value;

            HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
            if (this->observedOffsetX)
            {
                this->observedOffsetX->removeObserver(this->observedOffsetXProperty, HSSObservablePropertyOffsetX, this);
            }

            this->offsetX = this->_evaluatePropertyValue(
                                                      &HSSRadialGradient::offsetXChanged,
                                                      value,
                                                      this->getThisObj()->getWidth(),
                                                      observedProperty,
                                                      this->getThisObj().data(),
                                                      HSSObservablePropertyOffsetX,
                                                      this->observedOffsetX,
                                                      this->observedOffsetXProperty
                                                      );

            break;
        }

        default:
            break;
    }
}

void HSSRadialGradient::offsetXChanged(HSSObservableProperty source, void *data)
{
    switch (this->dOffsetX->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->offsetX = *(HSSUnit*) data;
            break;

        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dOffsetX);
            this->offsetX = percentageValue->getValue(*(HSSUnit*) data);
            break;
        }

        default:
            break;
    }

    this->notifyObservers(HSSObservablePropertyOffsetX, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSRadialGradient::getOffsetY()
{
    return this->offsetY;
}

QSharedPointer<HSSParserNode> HSSRadialGradient::getDOffsetY()
{
    return this->dOffsetY;
}

void HSSRadialGradient::setDOffsetY(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        {
            this->dOffsetY = value;

            HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
            if (this->observedOffsetY)
            {
                this->observedOffsetY->removeObserver(this->observedOffsetYProperty, HSSObservablePropertyOffsetY, this);
            }

            this->offsetY = this->_evaluatePropertyValue(
                                                      &HSSRadialGradient::offsetYChanged,
                                                      value,
                                                      this->getThisObj()->getHeight(),
                                                      observedProperty,
                                                      this->getThisObj().data(),
                                                      HSSObservablePropertyOffsetY,
                                                      this->observedOffsetY,
                                                      this->observedOffsetYProperty
                                                      );

            break;
        }

        default:
            break;
    }
}

void HSSRadialGradient::offsetYChanged(HSSObservableProperty source, void *data)
{
    switch (this->dOffsetY->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->offsetY = *(HSSUnit*) data;
            break;

        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dOffsetY);
            this->offsetY = percentageValue->getValue(*(HSSUnit*) data);
            break;
        }

        default:
            break;
    }

    this->notifyObservers(HSSObservablePropertyOffsetY, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSRadialGradient::_evaluatePropertyValue(
                                                  void(HSSRadialGradient::*callback)(HSSObservableProperty property, void* data),
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
                observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSRadialGradient > (this, callback));
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
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSRadialGradient > (this, callback));
            }

            break;
        }

        case HSSParserNodeTypeKeywordConstant:

            break;

        default:
            AXRWarning("HSSRadialGradient", "Unknown parser node type while setting value for HSSRadialGradient property").raise();
            break;
    }

    return ret;
}
