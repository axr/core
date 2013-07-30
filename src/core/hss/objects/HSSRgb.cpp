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

#include <QColor>
#include <QVariant>
#include "AXRError.h"
#include "AXRLoggerManager.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSKeywordConstant.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSSimpleSelection.h"
#include "HSSValue.h"

using namespace AXR;

QSharedPointer<HSSRgb> HSSRgb::blackColor(AXRController * controller)
{
    //create a new color object, it will have all its color channels set to
    //0 and the alpha to 255 by default in the constructor
    QSharedPointer<HSSRgb> blackColor(new HSSRgb(controller));
    blackColor->setDefaults();
    return blackColor;
}

QSharedPointer<HSSRgb> HSSRgb::whiteColor(AXRController * controller)
{
    //create a new color object with default value
    QSharedPointer<HSSRgb> whiteColor = QSharedPointer<HSSRgb>(new HSSRgb(controller));
    whiteColor->setDefaults();
    //set all the color channels to the maximum value
    whiteColor->setRed(255.0);
    whiteColor->setGreen(255.0);
    whiteColor->setBlue(255.0);
    return whiteColor;
}

HSSRgb::HSSRgb(AXRController * controller)
: HSSObject(HSSObjectTypeRgb, controller)
{
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("isA");
    shorthandProperties.push_back("alpha");
    shorthandProperties.push_back("red");
    shorthandProperties.push_back("green");
    shorthandProperties.push_back("blue");
    this->setShorthandProperties(shorthandProperties);
}

HSSRgb::HSSRgb(const HSSRgb & orig)
: HSSObject(orig)
{
    this->_initialize();
}

void HSSRgb::_initialize()
{
    this->addCallback("alpha", new HSSComputeCallback<HSSRgb>(this, &HSSRgb::computeAlpha));
    this->addCallback("red", new HSSComputeCallback<HSSRgb>(this, &HSSRgb::computeRed));
    this->addCallback("green", new HSSComputeCallback<HSSRgb>(this, &HSSRgb::computeGreen));
    this->addCallback("blue", new HSSComputeCallback<HSSRgb>(this, &HSSRgb::computeBlue));
}

QSharedPointer<HSSRgb> HSSRgb::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRgb: cloning rgb object");
    return qSharedPointerCast<HSSRgb> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSRgb::cloneImpl() const
{
    return QSharedPointer<HSSRgb>(new HSSRgb(*this));
}

HSSRgb::~HSSRgb()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRgb: destructing rgb object");
    this->cleanTrackedObservers();
}

void HSSRgb::setDefaults()
{
    this->setDefault("alpha", 255.);
    this->setDefault("red", 0.);
    this->setDefault("green", 0.);
    this->setDefault("blue", 0.);
}

AXRString HSSRgb::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSRgb: ").append(this->name);
    }
    else
    {
        return AXRString("Annonymous HSSRgb with red: %1 green: %2 blue: %3 alpha: %4").arg(this->getRed()).arg(this->getGreen()).arg(this->getBlue()).arg(this->getAlpha());
    }
}

AXRString HSSRgb::defaultObjectType()
{
    return "rgb";
}

AXRString HSSRgb::defaultObjectType(AXRString property)
{
    if (property == "red" || property == "green" || property == "blue" || property == "alpha")
    {
        return "value";
    }
    else
    {
        return HSSObject::defaultObjectType(property);
    }
}

QSharedPointer<HSSObject> HSSRgb::computeRed(QSharedPointer<HSSParserNode> parserNode)
{
    return this->_computeChannelValue(parserNode);
}

const HSSUnit HSSRgb::getRed() const
{
    return this->getComputedNumber("red");
}

void HSSRgb::setRed(HSSUnit value)
{
    this->setComputedValue("red", QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(value, this->getController())));
}

QSharedPointer<HSSObject> HSSRgb::computeGreen(QSharedPointer<HSSParserNode> parserNode)
{
    return this->_computeChannelValue(parserNode);
}

const HSSUnit HSSRgb::getGreen() const
{
    return this->getComputedNumber("green");
}

void HSSRgb::setGreen(HSSUnit value)
{
    this->setComputedValue("green", QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(value, this->getController())));
}

QSharedPointer<HSSObject> HSSRgb::computeBlue(QSharedPointer<HSSParserNode> parserNode)
{
    return this->_computeChannelValue(parserNode);
}

const HSSUnit HSSRgb::getBlue() const
{
    return this->getComputedNumber("blue");
}

void HSSRgb::setBlue(HSSUnit value)
{
    this->setComputedValue("blue", QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(value, this->getController())));
}

QSharedPointer<HSSObject> HSSRgb::computeAlpha(QSharedPointer<HSSParserNode> parserNode)
{
    return this->_computeChannelValue(parserNode);
}

const HSSUnit HSSRgb::getAlpha() const
{
    return this->getComputedNumber("alpha");
}

void HSSRgb::setAlpha(HSSUnit value)
{
    this->setComputedValue("alpha", value);
}

QColor HSSRgb::toQColor() const
{
    return QColor(static_cast<int>(this->getRed()), static_cast<int>(this->getGreen()), static_cast<int>(this->getBlue()), static_cast<int>(this->getAlpha()));
}

QColor HSSRgb::toQColorWithAlpha(int alpha) const
{
    QColor color = toQColor();
    color.setAlpha(alpha);
    return color;
}

QSharedPointer<HSSValue> HSSRgb::_computeChannelValue(QSharedPointer<HSSParserNode> parserNode) const
{
    HSSUnit ret = 0.;
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        {
            QSharedPointer<HSSNumberConstant> numberValue = qSharedPointerCast<HSSNumberConstant>(parserNode);
            ret = numberValue->getValue();
            break;
        }
        case HSSParserNodeTypePercentageConstant:
        {
            QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant>(parserNode);
            ret = 255. * percentageValue->getNumber();
            break;
        }
        case HSSParserNodeTypeExpression:
        {
            QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression>(parserNode);
            ret = expressionValue->evaluate();
            break;
        }
        default:
            break;
    }

    return HSSValue::valueFromParserNode(this->getController(), QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(ret, this->getController())), this->getThisObj(), this->getScope());
}
