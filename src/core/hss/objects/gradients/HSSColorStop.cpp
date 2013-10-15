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
#include "HSSCallback.h"
#include "HSSColorStop.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSKeywordConstant.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSSimpleSelection.h"
#include "HSSValue.h"

using namespace AXR;

HSSColorStop::HSSColorStop(AXRController * controller)
: HSSObject(HSSObjectTypeColorStop, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSColorStop: creating color stop object");
    this->_initialize();

    this->_positionIsPercentage = false;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("position");
    shorthandProperties.push_back("balance");
    this->setShorthandProperties(shorthandProperties);
}

HSSColorStop::HSSColorStop(const HSSColorStop & orig)
: HSSObject(orig)
{
    this->_initialize();
    this->_positionIsPercentage = orig._positionIsPercentage;
}

void HSSColorStop::_initialize()
{
    this->addCallback("color", new HSSComputeCallback<HSSColorStop>(this, &HSSColorStop::computeColor));
}

QSharedPointer<HSSColorStop> HSSColorStop::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSColorStop: cloning color stop object");
    return qSharedPointerCast<HSSColorStop> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSColorStop::cloneImpl() const
{
    return QSharedPointer<HSSColorStop>(new HSSColorStop(*this));
}

HSSColorStop::~HSSColorStop()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSColorStop: destructing color stop object");
    this->cleanTrackedObservers();
}

void HSSColorStop::setDefaults()
{
    HSSObject::setDefaults();
    this->setDefaultKw("color", "transparent");
    this->setDefault("position", 0.5);
    this->setDefault("balance", 0.5);
}

AXRString HSSColorStop::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSColorStop: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSColorStop";
    }
}

AXRString HSSColorStop::defaultObjectType()
{
    return "colorStop";
}

AXRString HSSColorStop::defaultObjectType(AXRString property)
{
    if (property == "color")
    {
        return "rgb";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSColorStop::isKeyword(AXRString value, AXRString property)
{
    if (value == "start" || value == "middle" || value == "end")
    {
        if (property == "position" || property == "balance")
        {
            return true;
        }
    }
    else if (value == "transparent"){
        if (property == "color" ) {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

QSharedPointer<HSSParserNode> HSSColorStop::getPercentageExpression(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    if (propertyName == "position")
    {
        this->_positionIsPercentage = true;
        return parserNode;
    }

    return HSSObject::getPercentageExpression(parserNode, propertyName);
}

QSharedPointer<HSSObject> HSSColorStop::getColor() const
{
    return this->getComputedValue("color");
}

QSharedPointer<HSSObject> HSSColorStop::computeColor(QSharedPointer<HSSParserNode> parserNode)
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
    return this->computeObject(parserNode, "color");
}

HSSUnit HSSColorStop::getPosition() const
{
    return this->getComputedNumber("position");
}

HSSUnit HSSColorStop::getBalance() const
{
    return this->getComputedNumber("balance");
}

bool HSSColorStop::positionIsPercentage() const
{
    return this->_positionIsPercentage;
}
