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

#define _USE_MATH_DEFINES // for M_PI when using MSVC
#include <cmath>
#include <QPainterPath>
#include <QVector>
#include "AXRController.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSBorder.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSPercentageConstant.h"
#include "HSSPolygon.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

Q_DECLARE_METATYPE(HSSUnit*)

HSSPolygon::HSSPolygon(AXRController * controller)
: HSSShape(HSSShapeTypePolygon, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSPolygon: creating polygon object");
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("sides");
    shorthandProperties.push_back("angle");
    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertySides, QVariant::fromValue(&this->sides));
    this->registerProperty(HSSObservablePropertyAngle, QVariant::fromValue(&this->angle));
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

    this->registerProperty(HSSObservablePropertySides, QVariant::fromValue(&this->sides));
    this->registerProperty(HSSObservablePropertyAngle, QVariant::fromValue(&this->angle));
    this->sides = 3;
    this->angle = 0;

    this->observedSides = NULL;
    this->observedAngle = NULL;
}

QSharedPointer<HSSPolygon> HSSPolygon::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSPolygon: cloning polygon object");
    return qSharedPointerCast<HSSPolygon> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSPolygon::cloneImpl() const
{
    return QSharedPointer<HSSPolygon>(new HSSPolygon(*this));
}

HSSPolygon::~HSSPolygon()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSPolygon: destructing polygon object");
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

void HSSPolygon::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
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

void HSSPolygon::createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::vector<QSharedPointer<HSSParserNode> > segments)
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

void HSSPolygon::drawBorders(QPainter &painter, std::vector<QSharedPointer<HSSBorder> > borders, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY)
{
    // Calculate the combined thickness of all borders
    HSSUnit combinedThickness = 0;
    for (HSSBorder::it it = borders.begin(); it != borders.end(); ++it)
    {
        combinedThickness += (*it)->getSize();
    }

    // Correction if needed
    HSSUnit correction = 0;
    if ((int) combinedThickness % 2)
    {
        correction = 0.5;
    }

    // Cumulative combined thickness
    HSSUnit cumulativeThickness = 0;

    // Draw all borders
    for (HSSBorder::it it = borders.begin(); it != borders.end(); ++it)
    {
        QSharedPointer<HSSBorder> theBorder = *it;
        HSSUnit theSize = theBorder->getSize();

        HSSUnit offset = (combinedThickness / 2) - cumulativeThickness - (theSize / 2) + correction;

        QPainterPath path;
        HSSShape::createPath(path, offsetX + offset, offsetY + offset, width - offset * 2, height - offset * 2);
        theBorder->draw(painter, path);

        cumulativeThickness += theSize;
    }
}

unsigned int HSSPolygon::getSides()
{
    return this->sides;
}

QSharedPointer<HSSParserNode> HSSPolygon::getDSides()
{
    return this->dSides;
}

void HSSPolygon::setDSides(QSharedPointer<HSSParserNode> value)
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
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->thisObj);
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSUnit>())
            {
                this->sides = floor(remoteValue.value<HSSUnit>());
            }

            if (remoteValue.type() == QVariant::UInt)
            {
                this->sides = remoteValue.toUInt();
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertySides, this, new HSSValueChangedCallback<HSSPolygon > (this, &HSSPolygon::sidesChanged));
        }
        else
        {
            throw AXRWarning("HSSDGradient", "Invalid function type for sides of @polygon " + this->name);
        }

        break;
    }

    default:
        throw AXRWarning("HSSPolygon", "Invalid value for sides of @polygon " + this->name);
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
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dSides);
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

QSharedPointer<HSSParserNode> HSSPolygon::getDAngle()
{
    return this->dAngle;
}

void HSSPolygon::setDAngle(QSharedPointer<HSSParserNode> value)
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
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            fnct->setScope(this->scope);
            fnct->setThisObj(this->thisObj);
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSUnit>())
            {
                this->angle = remoteValue.value<HSSUnit>();
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyAngle, this, new HSSValueChangedCallback<HSSPolygon > (this, &HSSPolygon::angleChanged));
        }
        else
        {
            throw AXRWarning("HSSDGradient", "Invalid function type for angle of @polygon " + this->name);
        }

        break;
    }

    default:
        throw AXRWarning("HSSPolygon", "Invalid value for angle of @polygon " + this->name);
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
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dAngle);
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
        AXRWarning("HSSPolygon", "Unknown parser node type while setting value for @polygon property").raise();
        break;
    }

    return ret;
}
