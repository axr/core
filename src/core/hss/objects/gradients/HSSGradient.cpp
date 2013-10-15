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

#include "AXRController.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSColorStop.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSGradient.h"
#include "HSSKeywordConstant.h"
#include "HSSMultipleValue.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

AXRString HSSGradient::gradientTypeStringRepresentation(HSSGradientType gradientType)
{
    static QMap<HSSGradientType, AXRString> types;
    if (types.isEmpty())
    {
        types[HSSGradientTypeLinear] = "HSSGradientTypeLinear";
        types[HSSGradientTypeRadial] = "HSSGradientTypeRadial";
    }

    return types[gradientType];
}


HSSGradient::HSSGradient(HSSGradientType type, AXRController * controller)
: HSSObject(HSSObjectTypeGradient, controller)
{
    this->_initialize();
    this->gradientType = type;
}

HSSGradient::HSSGradient(const HSSGradient & orig)
: HSSObject(orig)
{
    this->_initialize();
    this->gradientType = orig.gradientType;
}

void HSSGradient::_initialize()
{
    this->addCallback("startColor", new HSSComputeCallback<HSSGradient>(this, &HSSGradient::computeStartColor));
    this->addCallback("endColor", new HSSComputeCallback<HSSGradient>(this, &HSSGradient::computeEndColor));
    this->addCallback("colorStops", new HSSComputeCallback<HSSGradient>(this, &HSSGradient::computeColorStops));
}

HSSGradient::~HSSGradient()
{
}

void HSSGradient::setDefaults()
{
    this->setDefaultKw("startColor", "black");
    this->setDefaultKw("endColor", "black");
    this->setDefaultKw("colorStops", "no");
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

bool HSSGradient::equalTo(QSharedPointer<HSSObject> otherObj)
{
    //check wether pointers are the same
    if (this == otherObj.data()) return true;
    //other checks
    if ( ! HSSObject::equalTo(otherObj)) return false;
    QSharedPointer<HSSGradient> castedObj = qSharedPointerCast<HSSGradient>(otherObj);
    if ( this->gradientType != castedObj->gradientType ) return false;
    return true;
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

QSharedPointer<HSSRgb> HSSGradient::getColorAfterFirst()
{
    //first, look into the color stops to see if we find a suitable color
    QSharedPointer<HSSRgb> ret;
    const QSharedPointer<HSSObject> colorStops = this->getColorStops();
    if (colorStops->isA(HSSObjectTypeColorStop))
    {
        QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop>(colorStops);
        QSharedPointer<HSSObject> stopColorObj = theStop->getColor();
        if(stopColorObj && stopColorObj->isA(HSSObjectTypeRgb)){
            ret = qSharedPointerCast<HSSRgb>(stopColorObj);
        }
    }
    else if (colorStops->isA(HSSObjectTypeMultipleValue))
    {
        QSharedPointer<HSSMultipleValue> multiValue = qSharedPointerCast<HSSMultipleValue>(colorStops);
        const QList<QSharedPointer<HSSObject> > stopsList = multiValue->getValues();
        Q_FOREACH(QSharedPointer<HSSObject> stopObj, stopsList)
        {
            if (stopObj->isA(HSSObjectTypeColorStop))
            {
                QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop>(colorStops);
                QSharedPointer<HSSObject> stopColorObj = theStop->getColor();
                if(stopColorObj && stopColorObj->isA(HSSObjectTypeRgb)){
                    ret = qSharedPointerCast<HSSRgb>(stopColorObj);
                    break;
                }
            }
            else if (stopObj->isA(HSSObjectTypeRgb))
            {
                ret = qSharedPointerCast<HSSRgb>(stopObj);
                break;
            }
        }
    }
    //not found yet? we'll use the endColor
    if (!ret)
    {
        const QSharedPointer<HSSObject> endColorObj = this->getEndColor();
        if (endColorObj && endColorObj->isA(HSSObjectTypeRgb))
        {
            ret = qSharedPointerCast<HSSRgb>(endColorObj);
        }
    }

    //not even that? use transparent black
    if (!ret)
    {
        ret = HSSRgb::transparentColor(this->getController());
    }

    return ret;
}

QSharedPointer<HSSRgb> HSSGradient::getColorBeforeLast()
{
    //first, look into the color stops (in reverse order) to see if we find a suitable color
    QSharedPointer<HSSRgb> ret;

    const QSharedPointer<HSSObject> colorStops = this->getColorStops();
    if (colorStops->isA(HSSObjectTypeColorStop))
    {
        QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop>(colorStops);
        QSharedPointer<HSSObject> stopColorObj = theStop->getColor();
        if(stopColorObj && stopColorObj->isA(HSSObjectTypeRgb)){
            ret = qSharedPointerCast<HSSRgb>(stopColorObj);
        }
    }
    else if (colorStops->isA(HSSObjectTypeMultipleValue))
    {
        QSharedPointer<HSSMultipleValue> multiValue = qSharedPointerCast<HSSMultipleValue>(colorStops);
        const QList<QSharedPointer<HSSObject> > stopsList = multiValue->getValues();
        QListIterator<QSharedPointer<HSSObject> > i(stopsList);
        i.toBack();
        while (i.hasPrevious()){
            QSharedPointer<HSSObject> stopObj = i.previous();
            if (stopObj->isA(HSSObjectTypeColorStop))
            {
                QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop>(colorStops);
                QSharedPointer<HSSObject> stopColorObj = theStop->getColor();
                if(stopColorObj && stopColorObj->isA(HSSObjectTypeRgb)){
                    ret = qSharedPointerCast<HSSRgb>(stopColorObj);
                    break;
                }
            }
            else if (stopObj->isA(HSSObjectTypeRgb))
            {
                ret = qSharedPointerCast<HSSRgb>(stopObj);
                break;
            }
        }
    }
    //not found yet? we'll use the startColor
    if (!ret){
        const QSharedPointer<HSSObject> startColorObj = this->getStartColor();
        if (startColorObj && startColorObj->isA(HSSObjectTypeRgb))
        {
            ret = qSharedPointerCast<HSSRgb>(startColorObj);
        }
    }

    //not even that? use transparent black
    if (!ret){
        ret = HSSRgb::transparentColor(this->getController());
    }

    return ret;
}

QSharedPointer<HSSRgb> HSSGradient::getNextColorFromStops(QListIterator<QSharedPointer<HSSObject> > it)
{
    //first, look into the color stops to see if we find a suitable color
    QSharedPointer<HSSRgb> ret;
    while (it.hasNext()) {
        const QSharedPointer<HSSObject> & stopObj = it.next();
        if (stopObj->isA(HSSObjectTypeRgb)) {
            ret = qSharedPointerCast<HSSRgb>(stopObj);
            break;
        }
        else if (stopObj->isA(HSSObjectTypeColorStop))
        {
            const QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop>(stopObj);
            QSharedPointer<HSSObject> stopColorObj = theStop->getColor();
            if(stopColorObj && stopColorObj->isA(HSSObjectTypeRgb)){
                ret = qSharedPointerCast<HSSRgb>(stopColorObj);
                break;
            }
        }
    }

    //not found yet? we'll use the endColor
    if(!ret){
        const QSharedPointer<HSSObject> endColorObj = this->getEndColor();
        if (endColorObj && endColorObj->isA(HSSObjectTypeRgb))
        {
            ret = qSharedPointerCast<HSSRgb>(endColorObj);
        }
    }

    //not even that? use transparent black
    if (!ret){
        ret = HSSRgb::transparentColor(this->getController());
    }

    return ret;
}

QSharedPointer<HSSObject> HSSGradient::getStartColor() const
{
    return this->getComputedValue("startColor");
}

QSharedPointer<HSSObject> HSSGradient::computeStartColor(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString theKw = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (theKw == "black")
            {
                return HSSRgb::blackColor(this->getController());
            }
            else if (theKw == "white")
            {
                return HSSRgb::whiteColor(this->getController());
            }
            else if (theKw == "transparent")
            {
                return this->computeValueObject(parserNode);
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "startColor");
}

QSharedPointer<HSSObject> HSSGradient::getEndColor() const
{
    return this->getComputedValue("endColor");
}

QSharedPointer<HSSObject> HSSGradient::computeEndColor(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString theKw = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (theKw == "black")
            {
                return HSSRgb::blackColor(this->getController());
            }
            else if (theKw == "white")
            {
                return HSSRgb::whiteColor(this->getController());
            }
            else if (theKw == "transparent")
            {
                return this->computeValueObject(parserNode);
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "endColor");
}

QSharedPointer<HSSObject> HSSGradient::getBalance() const
{
    return this->getComputedValue("balance");
}

//colorStops
QSharedPointer<HSSObject> HSSGradient::getColorStops() const
{
    return this->getComputedValue("colorStops");
}

QSharedPointer<HSSObject> HSSGradient::computeColorStops(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString theKw = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (theKw == "black")
            {
                return HSSRgb::blackColor(this->getController());
            }
            else if (theKw == "white")
            {
                return HSSRgb::whiteColor(this->getController());
            }
            else if (theKw == "transparent")
            {
                return this->computeValueObject(parserNode);
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "colorStops");
}

bool HSSGradient::isA(HSSGradientType type) const
{
    return type == this->gradientType;
}

HSSGradientType HSSGradient::getGradientType() const
{
    return this->gradientType;
}
