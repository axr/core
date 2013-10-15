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
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSBorder.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSKeywordConstant.h"
#include "HSSMultipleValueDefinition.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSValue.h"

using namespace AXR;

HSSBorder::HSSBorder(AXRController * controller)
: HSSObject(HSSObjectTypeBorder, controller)
{
}

HSSBorder::HSSBorder(const HSSBorder & orig)
: HSSObject(orig)
{
}

HSSBorder::~HSSBorder()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSBorder: destructing border object");
    this->cleanTrackedObservers();
}

AXRString HSSBorder::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSBorder: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSBorder";
    }
}

void HSSBorder::setDefaults()
{
    this->setDefault("size", 1);
    this->setDefaultKw("position", "center");
    this->setDefaultKw("segments", "all");
}

AXRString HSSBorder::defaultObjectType()
{
    return "border";
}

AXRString HSSBorder::defaultObjectType(AXRString property)
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

bool HSSBorder::isKeyword(AXRString value, AXRString property)
{
    if (property == "position")
    {
        if (value == "inside" || value == "center" || value == "outside")
        {
            return true;
        }
    }
    else if (property == "segments")
    {
        if (value == "left" || value == "top" || value == "right" || value == "bottom" || value == "all")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

HSSUnit HSSBorder::getSize() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("size");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

HSSBorderPosition HSSBorder::getPosition() const
{
    QSharedPointer<HSSObject> computedValue = this->getComputedValue("position");
    if (computedValue && computedValue->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSValue> theValue = qSharedPointerCast<HSSValue>(computedValue);
        switch (theValue->getValue()->getType())
        {
            case HSSParserNodeTypeKeywordConstant:
            {
                AXRString kwValue = qSharedPointerCast<HSSKeywordConstant>(theValue->getValue())->getValue();
                if (kwValue == "inside")
                {
                    return HSSBorderPositionInside;
                }
                if (kwValue == "outside")
                {
                    return HSSBorderPositionOutside;
                }
                if (kwValue == "center")
                {
                    return HSSBorderPositionCenter;
                }
                break;
            }

            default:
                break;
        }
    }
    return HSSBorderPositionInside; ///@todo set defaults and change to None
}

QSharedPointer<HSSObject> HSSBorder::getSegments() const
{
    return this->getComputedValue("segments");
}
