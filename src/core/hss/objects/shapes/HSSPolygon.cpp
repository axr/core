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

#define _USE_MATH_DEFINES // for M_PI when using MSVC
#include <cmath>
#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRWarning.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSPercentageConstant.h"
#include "HSSPolygon.h"

using namespace AXR;

HSSPolygon::HSSPolygon()
: HSSShape(HSSShapeTypePolygon)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSPolygon: creating polygon object");
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("sides");
    shorthandProperties.push_back("angle");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertySides, & this->sides);
    this->registerProperty(HSSObservablePropertyAngle, & this->angle);
    this->sides = 3;
    this->angle = 0;

    this->observedSides = NULL;
    this->observedAngle = NULL;
}

HSSPolygon::HSSPolygon(const HSSPolygon & orig)
: HSSShape(orig)
{
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("sides");
    shorthandProperties.push_back("angle");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertySides, & this->sides);
    this->registerProperty(HSSObservablePropertyAngle, & this->angle);
    this->sides = 3;
    this->angle = 0;

    this->observedSides = NULL;
    this->observedAngle = NULL;
}

HSSPolygon::p HSSPolygon::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSPolygon: cloning polygon object");
    return boost::static_pointer_cast<HSSPolygon, HSSClonable > (this->cloneImpl());
}

HSSClonable::p HSSPolygon::cloneImpl() const
{
    return HSSClonable::p(new HSSPolygon(*this));
}

HSSPolygon::~HSSPolygon()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSPolygon: destructing polygon object");
}

AXRString HSSPolygon::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSPolygon: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSPolygon";
    }
}

AXRString HSSPolygon::defaultObjectType()
{
    return "polygon";
}

bool HSSPolygon::isKeyword(AXRString value, AXRString property)
{
    if (property == "angle")
    {
        if (value == "top" || value == "topRight" || value == "right" || value == "bottomRight" || value == "bottom" || value == "bottomLeft" || value == "left" || value == "topLeft")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSShape::isKeyword(value, property);
}

void HSSPolygon::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name)
    {
    case HSSObservablePropertySides:
        this->setDSides(value);
        break;
    case HSSObservablePropertyAngle:
        this->setDAngle(value);
        break;
    default:
        HSSObject::setProperty(name, value);
        break;
    }
}

void HSSPolygon::createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height)
{
    // The center point of the polygon
    const QPointF centerPoint(x + (width / 2.), y + (height / 2.));

    // 2 dimensional radius for stretched polygons
    const QSizeF radius(width / 2., height / 2.);

    // Number of sides in the polygon
    const unsigned int sides = qMax(3u, this->sides);

    // Angle of rotation of the entire polygon specified in radians
    const HSSUnit theta = this->angle * (M_PI / 180.);

    // Build a list of points comprising the polygon vertices
    QVector<QPointF> points;
    for (unsigned int i = 1; i <= this->sides; ++i)
    {
        double px = radius.width() * cos(((2 * M_PI * i) / sides) + theta) + centerPoint.x();
        double py = radius.height() * sin(((2 * M_PI * i) / sides) + theta) + centerPoint.y();
        points << QPointF(px, py);
    }

    // Create a polygon from the vertex list and add to the path
    path.addPolygon(QPolygonF(points));

    // Close the path
    path.closeSubpath();
}

unsigned int HSSPolygon::getSides()
{
    return this->sides;
}

HSSParserNode::p HSSPolygon::getDSides()
{
    return this->dSides;
}

void HSSPolygon::setDSides(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        this->dSides = value;
        this->sides = floor(this->_evaluatePropertyValue(
                                           &HSSPolygon::sidesChanged,
                                           value,
                                           1.,
                                           HSSObservablePropertySides,
                                           this->observedSides,
                                           this->observedSidesProperty
                                           ));

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        this->dSides = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->thisObj);
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->sides = floor(boost::any_cast<HSSUnit>(remoteValue));
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }
            try
            {
                this->sides = boost::any_cast<unsigned int>(remoteValue);
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertySides, this, new HSSValueChangedCallback<HSSPolygon > (this, &HSSPolygon::sidesChanged));

        }
        else
        {
            throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid function type for sides of @polygon " + this->name));
        }

        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSPolygon", "Invalid value for sides of @polygon " + this->name));
    }
    this->notifyObservers(HSSObservablePropertySides, &this->sides);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSPolygon::sidesChanged(HSSObservableProperty source, void*data)
{
    switch (this->dSides->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->sides = floor(*(HSSUnit*) data);
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (this->dSides);
        this->sides = floor(percentageValue->getValue(*(HSSUnit*) data));
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertySides, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSPolygon::getAngle()
{
    return this->angle;
}

HSSParserNode::p HSSPolygon::getDAngle()
{
    return this->dAngle;
}

void HSSPolygon::setDAngle(HSSParserNode::p value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
        this->dAngle = value;
        this->angle = this->_evaluatePropertyValue(
                                           &HSSPolygon::angleChanged,
                                           value,
                                           1.,
                                           HSSObservablePropertyAngle,
                                           this->observedAngle,
                                           this->observedAngleProperty
                                           );

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        this->dAngle = value;
        HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->thisObj);
            boost::any remoteValue = fnct->evaluate();
            try
            {
                this->angle = boost::any_cast<HSSUnit>(remoteValue);
            }
            catch (boost::bad_any_cast &)
            {
                //do nothing
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyAngle, this, new HSSValueChangedCallback<HSSPolygon > (this, &HSSPolygon::angleChanged));

        }
        else
        {
            throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid function type for angle of @polygon " + this->name));
        }

        break;
    }

    default:
        throw AXRWarning::p(new AXRWarning("HSSPolygon", "Invalid value for angle of @polygon " + this->name));
    }
    this->notifyObservers(HSSObservablePropertyAngle, &this->angle);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSPolygon::angleChanged(HSSObservableProperty source, void*data)
{
    switch (this->dAngle->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
        this->angle = *(HSSUnit*) data;
        break;

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant > (this->dAngle);
        this->angle = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertySides, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

HSSUnit HSSPolygon::_evaluatePropertyValue(
                                       void(HSSPolygon::*callback)(HSSObservableProperty property, void* data),
                                       HSSParserNode::p value,
                                       HSSUnit percentageBase,
                                       HSSObservableProperty observedSourceProperty,
                                       HSSObservable * &observedStore,
                                       HSSObservableProperty &observedStoreProperty
                                       )
{
    HSSUnit ret;

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
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setScope(this->scope);
        expressionValue->setThisObj(this->thisObj);
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSPolygon > (this, callback));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    default:
        AXRWarning::p(new AXRWarning("HSSPolygon", "Unknown parser node type while setting value for @polygon property"))->raise();
        break;
    }

    return ret;
}
