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
#include "HSSColorStop.h"
#include "HSSLinearGradient.h"
#include "HSSObjectDefinition.h"

using namespace AXR;

HSSLinearGradient::HSSLinearGradient()
: HSSGradient()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLinearGradient: creating linear gradient object");
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("endY");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyStartX, & this->startX);
    this->registerProperty(HSSObservablePropertyStartY, & this->startY);
    this->registerProperty(HSSObservablePropertyEndX, & this->startY);
    this->registerProperty(HSSObservablePropertyEndY, & this->startY);

    this->startX = this->startY = this->endX = endY = 0.;

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

    this->registerProperty(HSSObservablePropertyStartX, & this->startX);
    this->registerProperty(HSSObservablePropertyStartY, & this->startY);
    this->registerProperty(HSSObservablePropertyEndX, & this->startY);
    this->registerProperty(HSSObservablePropertyEndY, & this->startY);

    this->startX = this->startY = this->endX = endY = 0.;

    this->observedStartX = this->observedStartY
            = this->observedEndX = this->observedEndY
            = NULL;

}

HSSLinearGradient::p HSSLinearGradient::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSLinearGradient: cloning linear gradient object");
    return boost::static_pointer_cast<HSSLinearGradient, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSLinearGradient::cloneImpl() const
{
    return HSSClonable::p(new HSSLinearGradient(*this));
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

void HSSLinearGradient::setProperty(HSSObservableProperty name, HSSParserNode::p value)
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

long double HSSLinearGradient::getStartX()
{
    return this->startX;
}

HSSParserNode::p HSSLinearGradient::getDStartX()
{
    return this->dStartX;
}

void HSSLinearGradient::setDStartX(HSSParserNode::p value)
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

        this->startX = this->_setLDProperty(
                                            &HSSLinearGradient::startXChanged,
                                            value,
                                            this->getThisObj()->getWidth(),
                                            observedProperty,
                                            this->getThisObj().get(),
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
        this->startX = *(long double*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (this->dStartX);
        this->startX = percentageValue->getValue(*(long double*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyStartX, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSLinearGradient::getStartY()
{
    return this->startY;
}

HSSParserNode::p HSSLinearGradient::getDStartY()
{
    return this->dStartY;
}

void HSSLinearGradient::setDStartY(HSSParserNode::p value)
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

        this->startY = this->_setLDProperty(
                                            &HSSLinearGradient::startYChanged,
                                            value,
                                            this->getThisObj()->getHeight(),
                                            observedProperty,
                                            this->getThisObj().get(),
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
        this->startY = *(long double*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (this->dStartY);
        this->startY = percentageValue->getValue(*(long double*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyStartY, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSLinearGradient::getEndX()
{
    return this->endX;
}

HSSParserNode::p HSSLinearGradient::getDEndX()
{
    return this->dEndX;
}

void HSSLinearGradient::setDEndX(HSSParserNode::p value)
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

        this->endX = this->_setLDProperty(
                                          &HSSLinearGradient::endXChanged,
                                          value,
                                          this->getThisObj()->getWidth(),
                                          observedProperty,
                                          this->getThisObj().get(),
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
        this->endX = *(long double*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (this->dEndX);
        this->endX = percentageValue->getValue(*(long double*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyEndX, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSLinearGradient::getEndY()
{
    return this->endY;
}

HSSParserNode::p HSSLinearGradient::getDEndY()
{
    return this->dEndY;
}

void HSSLinearGradient::setDEndY(HSSParserNode::p value)
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

        this->endY = this->_setLDProperty(
                                          &HSSLinearGradient::endYChanged,
                                          value,
                                          this->getThisObj()->getHeight(),
                                          observedProperty,
                                          this->getThisObj().get(),
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
        this->endY = *(long double*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (this->dEndY);
        this->endY = percentageValue->getValue(*(long double*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyEndY, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSLinearGradient::_setLDProperty(
                                              void(HSSLinearGradient::*callback)(HSSObservableProperty property, void* data),
                                              HSSParserNode::p value,
                                              long double percentageBase,
                                              HSSObservableProperty observedProperty,
                                              HSSObservable * observedObject,
                                              HSSObservableProperty observedSourceProperty,
                                              HSSObservable * &observedStore,
                                              HSSObservableProperty &observedStoreProperty
                                              )
{
    long double ret;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        HSSNumberConstant::p numberValue = boost::static_pointer_cast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (value);
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
        HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression > (value);
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
        AXRWarning::p(new AXRWarning("HSSLinearGradient", "Unknown parser node type while setting value for HSSLinearGradient property"))->raise();
        break;
    }

    return ret;
}

void HSSLinearGradient::draw(QPainter &painter, const QPainterPath &path)
{
    HSSRgb::p prevColor;
    QLinearGradient pat(this->startX, this->startY, this->endX, this->endY);
    if (this->startColor)
    {
        pat.setColorAt(0, QColor(this->startColor->getRed(), this->startColor->getGreen(), this->startColor->getBlue(), this->startColor->getAlpha()));
        prevColor = this->startColor;
    }
    else
    {
        HSSRgb::p nextColor = this->getColorAfterFirst();
        pat.setColorAt(0, QColor(nextColor->getRed(), nextColor->getGreen(), nextColor->getBlue(), 0));
        prevColor = nextColor;
    }

    //add color stops
    for (std::vector<HSSObject::p>::iterator it = this->colorStops.begin(); it != this->colorStops.end(); ++it)
    {
        HSSObject::p theStopObj = *it;
        //if it's a color stop
        if (theStopObj->isA(HSSObjectTypeColorStop))
        {
            HSSColorStop::p theStop = boost::static_pointer_cast<HSSColorStop > (theStopObj);

            //calculate the position
            long double position;
            if (theStop->getDPosition()->isA(HSSParserNodeTypePercentageConstant))
            {
                position = theStop->getPosition();
            }
            else
            {
                long double width = (this->endX - this->startX);
                long double height = (this->endY - this->startY);
                long double hypotenuse = hypot(width, height);
                position = theStop->getPosition() / hypotenuse;
            }

            //determine the color
            HSSRgb::p theColor = theStop->getColor();
            if (theColor)
            {
                pat.setColorAt(position, QColor(theColor->getRed(), theColor->getGreen(), theColor->getBlue(), theColor->getAlpha()));
                prevColor = theColor;
            }
            else
            {
                //create two stops:
                //one with the previous color
                pat.setColorAt(position, QColor(prevColor->getRed(), prevColor->getGreen(), prevColor->getBlue(), 0));
                //and one with the next color
                std::vector<HSSObject::p>::iterator innerIt = it;
                ++innerIt;
                HSSRgb::p nextColor = this->getNextColorFromStops(innerIt, this->colorStops.end());
                pat.setColorAt(position, QColor(nextColor->getRed(), nextColor->getGreen(), nextColor->getBlue(), 0));
            }
        }
        //if it's a simple color
        else if (theStopObj->isA(HSSObjectTypeRgb))
        {
            HSSRgb::p theColor = boost::static_pointer_cast<HSSRgb > (theStopObj);
            pat.setColorAt(0.5, QColor(theColor->getRed(), theColor->getGreen(), theColor->getBlue(), theColor->getAlpha()));
        }
        else
        {
            AXRWarning::p(new AXRWarning("HSSLinearGradient", "The color stop had no color defined"))->raise();
        }

    }

    if (this->endColor)
    {
        pat.setColorAt(1, QColor(this->endColor->getRed(), this->endColor->getGreen(), this->endColor->getBlue(), this->endColor->getAlpha()));
    }
    else
    {
        HSSRgb::p prevColor = this->getColorBeforeLast();
        pat.setColorAt(1, QColor(prevColor->getRed(), prevColor->getGreen(), prevColor->getBlue(), 0));
    }

    QBrush brush(pat);
    painter.fillPath(path, brush);
}
