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
    this->gradientType = type;
}

HSSGradient::HSSGradient(const HSSGradient & orig)
: HSSObject(orig)
{
    this->gradientType = orig.gradientType;
}

HSSGradient::~HSSGradient()
{
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
    if (this->colorStops.size() > 0) {
        for (std::vector<QSharedPointer<HSSObject> >::const_iterator it = this->colorStops.begin(); it != this->colorStops.end(); ++it) {
            const QSharedPointer<HSSObject> & theStopObj = *it;
            if (theStopObj->isA(HSSObjectTypeColorStop)) {
                QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop>(theStopObj);
                ret = theStop->getColor();
                if(ret){
                    break;
                }
            } else if (theStopObj->isA(HSSObjectTypeRgb)) {
                ret = qSharedPointerCast<HSSRgb>(theStopObj);
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
        ret = HSSRgb::transparentColor(this->getController());
    }

    return ret;
}

QSharedPointer<HSSRgb> HSSGradient::getColorBeforeLast()
{
    //first, look into the color stops (in reverse order) to see if we find a suitable color
    QSharedPointer<HSSRgb> ret;
    if (this->colorStops.size() > 0) {
        for (std::vector<QSharedPointer<HSSObject> >::const_reverse_iterator it = this->colorStops.rbegin(); it != this->colorStops.rend(); ++it) {
            const QSharedPointer<HSSObject> & theStopObj = *it;
            if (theStopObj->isA(HSSObjectTypeColorStop)) {
                QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop>(theStopObj);
                ret = theStop->getColor();
                if(ret){
                    break;
                }
            } else if (theStopObj->isA(HSSObjectTypeRgb)) {
                ret = qSharedPointerCast<HSSRgb>(theStopObj);
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
        ret = HSSRgb::transparentColor(this->getController());
    }

    return ret;
}

QSharedPointer<HSSRgb> HSSGradient::getNextColorFromStops(std::vector<QSharedPointer<HSSObject> >::iterator it, std::vector<QSharedPointer<HSSObject> >::iterator endIt)
{
    //first, look into the color stops to see if we find a suitable color
    QSharedPointer<HSSRgb> ret;
    for (; it != endIt; ++it) {
        const QSharedPointer<HSSObject> & stopObj = *it;
        if (stopObj->isA(HSSObjectTypeRgb)) {
            ret = qSharedPointerCast<HSSRgb>(stopObj);
            break;
        }
        else if (stopObj->isA(HSSObjectTypeColorStop))
        {
            const QSharedPointer<HSSColorStop> theStop = qSharedPointerCast<HSSColorStop>(stopObj);
            if(QSharedPointer<HSSRgb> theColor = theStop->getColor())
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
        ret = HSSRgb::transparentColor(this->getController());
    }

    return ret;
}

bool HSSGradient::isA(HSSGradientType type)
{
    return type == this->gradientType;
}

HSSGradientType HSSGradient::getGradientType()
{
    return this->gradientType;
}

