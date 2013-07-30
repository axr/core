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
#include "AXRLoggerManager.h"
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
#include "HSSValue.h"

using namespace AXR;

HSSLinearGradient::HSSLinearGradient(AXRController * controller)
: HSSGradient(HSSGradientTypeLinear, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLinearGradient: creating linear gradient object");
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("endY");
    this->setShorthandProperties(shorthandProperties);
}

HSSLinearGradient::HSSLinearGradient(const HSSLinearGradient & orig)
: HSSGradient(orig)
{
    this->_initialize();
}

void HSSLinearGradient::_initialize()
{
    this->addCallback("startX", new HSSObserveCallback<HSSLinearGradient>(this, &HSSLinearGradient::listenStartX), new HSSObserveCallback<HSSLinearGradient>(this, &HSSLinearGradient::notifyStartX));
    this->addCallback("startY", new HSSObserveCallback<HSSLinearGradient>(this, &HSSLinearGradient::listenStartY), new HSSObserveCallback<HSSLinearGradient>(this, &HSSLinearGradient::notifyStartY));
    this->addCallback("endX", new HSSObserveCallback<HSSLinearGradient>(this, &HSSLinearGradient::listenEndX), new HSSObserveCallback<HSSLinearGradient>(this, &HSSLinearGradient::notifyEndX));
    this->addCallback("endY", new HSSObserveCallback<HSSLinearGradient>(this, &HSSLinearGradient::listenEndY), new HSSObserveCallback<HSSLinearGradient>(this, &HSSLinearGradient::notifyEndY));
}

QSharedPointer<HSSLinearGradient> HSSLinearGradient::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLinearGradient: cloning linear gradient object");
    return qSharedPointerCast<HSSLinearGradient> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSLinearGradient::cloneImpl() const
{
    return QSharedPointer<HSSLinearGradient>(new HSSLinearGradient(*this));
}

HSSLinearGradient::~HSSLinearGradient()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLinearGradient: destructing linear gradient object");
    this->cleanTrackedObservers();
}

void HSSLinearGradient::setDefaults()
{
    this->setDefault("startX", 0.);
    this->setDefault("startY", 0.);
    this->setDefault("endX", 0.);
    this->setDefault("endY", 0.);
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

void HSSLinearGradient::setStartX(HSSUnit value)
{
    this->setComputedValue("startX", value);
}

const HSSUnit HSSLinearGradient::getStartX() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("startX");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

void HSSLinearGradient::listenStartX(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(theObj);
        QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
        valueObj->listen(this->getThisObj(), "width");
        valueObj->setPercentageBase(thisObj->getWidth());
        valueObj->observe("value", "startX", this, new HSSValueChangedCallback<HSSLinearGradient>(this, &HSSLinearGradient::startXChanged));
    }
}

void HSSLinearGradient::notifyStartX(QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("startX", theObj);
}

void HSSLinearGradient::startXChanged(const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("startX", theObj);
}

void HSSLinearGradient::setStartY(HSSUnit value)
{
    this->setComputedValue("startY", value);
}

const HSSUnit HSSLinearGradient::getStartY() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("startY");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

void HSSLinearGradient::listenStartY(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(theObj);
        QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
        valueObj->listen(thisObj, "height");
        valueObj->setPercentageBase(thisObj->getHeight());
        valueObj->observe("value", "startY", this, new HSSValueChangedCallback<HSSLinearGradient>(this, &HSSLinearGradient::startYChanged));
    }
}

void HSSLinearGradient::notifyStartY(QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("startY", theObj);
}

void HSSLinearGradient::startYChanged(const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("startY", theObj);
}

void HSSLinearGradient::setEndX(HSSUnit value)
{
    this->setComputedValue("endX", value);
}

const HSSUnit HSSLinearGradient::getEndX() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("endX");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

void HSSLinearGradient::listenEndX(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(theObj);
        QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
        valueObj->listen(this->getThisObj(), "width");
        valueObj->setPercentageBase(thisObj->getWidth());
        valueObj->observe("value", "endX", this, new HSSValueChangedCallback<HSSLinearGradient>(this, &HSSLinearGradient::endXChanged));
    }
}

void HSSLinearGradient::notifyEndX(QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("endX", theObj);
}

void HSSLinearGradient::endXChanged(const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("endX", theObj);
}

void HSSLinearGradient::setEndY(HSSUnit value)
{
    this->setComputedValue("endY", value);
}

const HSSUnit HSSLinearGradient::getEndY() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("endY");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

void HSSLinearGradient::listenEndY(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(theObj);
        QSharedPointer<HSSDisplayObject> thisObj = this->getThisObj();
        valueObj->listen(this->getThisObj(), "height");
        valueObj->setPercentageBase(thisObj->getHeight());
        valueObj->observe("value", "endY", this, new HSSValueChangedCallback<HSSLinearGradient>(this, &HSSLinearGradient::endYChanged));
    }
}

void HSSLinearGradient::notifyEndY(QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("endY", theObj);
}

void HSSLinearGradient::endYChanged(const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("endY", theObj);
}

