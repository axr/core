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
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include <QColor>
#include "AXRDebugging.h"
#include "AXRError.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

QSharedPointer<HSSRgb> HSSRgb::blackColor(AXRController * controller)
{
    //create a new color object, it will have all its color channels set to
    //0 and the alpha to 255 by default in the constructor
    return QSharedPointer<HSSRgb>(new HSSRgb(controller));
}

QSharedPointer<HSSRgb> HSSRgb::whiteColor(AXRController * controller)
{
    //create a new color object with default value
    QSharedPointer<HSSRgb> whiteColor = QSharedPointer<HSSRgb>(new HSSRgb(controller));
    //set all the color channels to the maximum value
    whiteColor->setDRed(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(255.0, controller)));
    whiteColor->setDGreen(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(255.0, controller)));
    whiteColor->setDBlue(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(255.0, controller)));
    return whiteColor;
}

HSSRgb::HSSRgb(AXRController * controller, HSSUnit red, HSSUnit green, HSSUnit blue, HSSUnit alpha)
: HSSObject(HSSObjectTypeRgb, controller), red(red), green(green), blue(blue), alpha(alpha)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRgb: creating rgb object");

    this->observedRed = this->observedGreen = this->observedBlue = this->observedAlpha = NULL;

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
    this->red = this->green = this->blue = 0.;
    this->alpha = 255.;
    this->observedRed = this->observedGreen = this->observedBlue = this->observedAlpha = NULL;
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("isA");
    shorthandProperties.push_back("alpha");
    shorthandProperties.push_back("red");
    shorthandProperties.push_back("green");
    shorthandProperties.push_back("blue");

    this->setShorthandProperties(shorthandProperties);
}

QSharedPointer<HSSRgb> HSSRgb::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRgb: cloning rgb object");
    return qSharedPointerCast<HSSRgb> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSRgb::cloneImpl() const
{
    return QSharedPointer<HSSRgb>(new HSSRgb(*this));
}

HSSRgb::~HSSRgb()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRgb: destructing rgb object");
}

AXRString HSSRgb::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSRgb: ").append(this->name);
    }
    else
    {
        return AXRString("Annonymous HSSRgb with red: %1 green: %2 blue: %3 alpha: %4").arg(this->red).arg(this->green).arg(this->blue).arg(this->alpha);
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

//bool HSSRgb::isKeyword(AXRString value, AXRString property)
//{
//    if (value == "transparent"){
//        return true;
//    }
//
//    //if we reached this far, let the superclass handle it
//    return HSSObject::isKeyword(value, property);
//}

void HSSRgb::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertyRed:
        this->setDRed(value);
        break;
    case HSSObservablePropertyGreen:
        this->setDGreen(value);
        break;
    case HSSObservablePropertyBlue:
        this->setDBlue(value);
        break;
    case HSSObservablePropertyAlpha:
        this->setDAlpha(value);
        break;

    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

HSSUnit HSSRgb::getRed()
{
    return this->red;
}

void HSSRgb::setDRed(QSharedPointer<HSSParserNode> value)
{
    this->dRed = value;
    this->red = this->_evaluatePropertyValue(
                                     &HSSRgb::redChanged,
                                     value,
                                     255.,
                                     HSSObservablePropertyRed,
                                     this->observedRed,
                                     this->observedRedProperty
                                     );
}

void HSSRgb::redChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** redChanged unimplemented ****************************");
}

HSSUnit HSSRgb::getGreen()
{
    return this->green;
}

void HSSRgb::setDGreen(QSharedPointer<HSSParserNode> value)
{
    this->dGreen = value;
    this->green = this->_evaluatePropertyValue(
                                       &HSSRgb::greenChanged,
                                       value,
                                       255.,
                                       HSSObservablePropertyGreen,
                                       this->observedGreen,
                                       this->observedGreenProperty
                                       );
}

void HSSRgb::greenChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** greenChanged unimplemented ****************************");
}

HSSUnit HSSRgb::getBlue()
{
    return this->blue;
}

void HSSRgb::setDBlue(QSharedPointer<HSSParserNode> value)
{
    this->dBlue = value;
    this->blue = this->_evaluatePropertyValue(
                                      &HSSRgb::blueChanged,
                                      value,
                                      255.,
                                      HSSObservablePropertyBlue,
                                      this->observedBlue,
                                      this->observedBlueProperty
                                      );
}

void HSSRgb::blueChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** blueChanged unimplemented ****************************");
}

HSSUnit HSSRgb::getAlpha()
{
    return this->alpha;
}

void HSSRgb::setDAlpha(QSharedPointer<HSSParserNode> value)
{
    this->dAlpha = value;
    this->alpha = this->_evaluatePropertyValue(
                                       &HSSRgb::alphaChanged,
                                       value,
                                       255.,
                                       HSSObservablePropertyAlpha,
                                       this->observedAlpha,
                                       this->observedAlphaProperty
                                       );
}

void HSSRgb::alphaChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** alphaChanged unimplemented ****************************");
}

HSSUnit HSSRgb::_evaluatePropertyValue(
                                   void(HSSRgb::*callback)(HSSObservableProperty property, void* data),
                                   QSharedPointer<HSSParserNode> value,
                                   HSSUnit percentageBase,
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
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setScope(this->getScope());
        expressionValue->setThisObj(this->getThisObj());
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSRgb > (this, callback));
            observedStore = expressionValue.data();
            observedStoreProperty = HSSObservablePropertyValue;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    default:
        throw AXRError("HSSRgb", "Unknown parser node type " + HSSParserNode::parserNodeStringRepresentation(nodeType) + " while setting value for HSSRgb property");
        break;
    }

    return ret;
}

QColor HSSRgb::toQColor() const
{
    return QColor(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue), static_cast<int>(alpha));
}

QColor HSSRgb::toQColorWithAlpha(int alpha) const
{
    QColor color = toQColor();
    color.setAlpha(alpha);
    return color;
}
