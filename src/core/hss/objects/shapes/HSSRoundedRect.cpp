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

#include <QPainter>
#include <QPainterPath>
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSBorder.h"
#include "HSSCallback.h"
#include "HSSDisplayObject.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSKeywordConstant.h"
#include "HSSLineBorder.h"
#include "HSSMultipleValue.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRgb.h"
#include "HSSRoundedRect.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

HSSRoundedRect::HSSRoundedRect(AXRController * controller)
: HSSShape(HSSShapeTypeRoundedRect, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRoundedRect: creating rounded rectangle object");
    this->cornerTL = this->cornerTR = this->cornerBR = this->cornerBL = 0.;
    this->observedTLCorner = this->observedTRCorner = this->observedBRCorner = this->observedBLCorner = NULL;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("corners");
    this->setShorthandProperties(shorthandProperties);
}

HSSRoundedRect::HSSRoundedRect(const HSSRoundedRect & orig)
: HSSShape(orig)
{
    this->cornerTL = this->cornerTR = this->cornerBR = this->cornerBL = 0.;
    this->observedTLCorner = this->observedTRCorner = this->observedBRCorner = this->observedBLCorner = NULL;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("corners");
    this->setShorthandProperties(shorthandProperties);
}

QSharedPointer<HSSRoundedRect> HSSRoundedRect::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRoundedRect: cloning rounded rectangle object");
    return qSharedPointerCast<HSSRoundedRect>(this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSRoundedRect::cloneImpl() const
{
    return QSharedPointer<HSSRoundedRect>(new HSSRoundedRect(*this));
}

HSSRoundedRect::~HSSRoundedRect()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRoundedRect: destructing rounded rectangle object");
}

AXRString HSSRoundedRect::toString()
{
    return "HSSRoundedRect";
}

AXRString HSSRoundedRect::defaultObjectType()
{
    return "roundedRect";
}

AXRString HSSRoundedRect::defaultObjectType(AXRString property)
{
    return HSSShape::defaultObjectType(property);
}

bool HSSRoundedRect::isKeyword(AXRString value, AXRString property)
{
    return HSSShape::isKeyword(value, property);
}

void HSSRoundedRect::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertyCorners:
        this->setDCorners(value);
        break;
    case HSSObservablePropertyLeft:
        this->setDLeft(value);
        break;
    case HSSObservablePropertyLeftTop:
        this->setDLeftTop(value);
        break;
    case HSSObservablePropertyTop:
        this->setDTop(value);
        break;
    case HSSObservablePropertyRightTop:
        this->setDRightTop(value);
        break;
    case HSSObservablePropertyRight:
        this->setDRight(value);
        break;
    case HSSObservablePropertyRightBottom:
        this->setDRightBottom(value);
        break;
    case HSSObservablePropertyBottom:
        this->setDBottom(value);
        break;
    case HSSObservablePropertyLeftBottom:
        this->setDLeftBottom(value);
        break;

    default:
        HSSShape::setProperty(name, value);
        break;
    }
}

QSharedPointer<HSSMultipleValue> HSSRoundedRect::getCorners()
{
    return this->corners;
}

void HSSRoundedRect::setDCorners(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTLCorner)
        {
            this->observedTLCorner->removeObserver(this->observedTLCornerProperty, HSSObservablePropertyCorners, this);
        }
        this->cornerTL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTLCorner,
                                              this->observedTLCornerProperty,
                                              this->scope);
        if (this->observedTRCorner)
        {
            this->observedTRCorner->removeObserver(this->observedTRCornerProperty, HSSObservablePropertyCorners, this);
        }
        this->cornerTR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTRCorner,
                                              this->observedTRCornerProperty,
                                              this->scope);
        if (this->observedBRCorner)
        {
            this->observedBRCorner->removeObserver(this->observedBRCornerProperty, HSSObservablePropertyCorners, this);
        }
        this->cornerBR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBRCorner,
                                              this->observedBRCornerProperty,
                                              this->scope);
        if (this->observedBLCorner)
        {
            this->observedBLCorner->removeObserver(this->observedBLCornerProperty, HSSObservablePropertyCorners, this);
        }
        this->cornerBL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBLCorner,
                                              this->observedBLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->dCorners = value;
    QSharedPointer<HSSMultipleValue> newCorners(new HSSMultipleValue(this->getController()));
    newCorners->add(value);
    this->corners = newCorners;

    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::setDLeft(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTLCorner)
        {
            this->observedTLCorner->removeObserver(this->observedTLCornerProperty, HSSObservablePropertyLeft, this);
        }
        this->cornerTL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTLCorner,
                                              this->observedTLCornerProperty,
                                              this->scope);
        if (this->observedBLCorner)
        {
            this->observedBLCorner->removeObserver(this->observedBLCornerProperty, HSSObservablePropertyLeft, this);
        }
        this->cornerBL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBLCorner,
                                              this->observedBLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTL);
    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBL);
}

void HSSRoundedRect::setDLeftTop(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTLCorner)
        {
            this->observedTLCorner->removeObserver(this->observedTLCornerProperty, HSSObservablePropertyLeftTop, this);
        }
        this->cornerTL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTLCorner,
                                              this->observedTLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTL);
}

void HSSRoundedRect::setDTop(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTLCorner)
        {
            this->observedTLCorner->removeObserver(this->observedTLCornerProperty, HSSObservablePropertyTop, this);
        }
        this->cornerTL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTLCorner,
                                              this->observedTLCornerProperty,
                                              this->scope);
        if (this->observedTRCorner)
        {
            this->observedTRCorner->removeObserver(this->observedTRCornerProperty, HSSObservablePropertyTop, this);
        }
        this->cornerTR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTRCorner,
                                              this->observedTRCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTL);
    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTR);
}

void HSSRoundedRect::setDRightTop(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTRCorner)
        {
            this->observedTRCorner->removeObserver(this->observedTRCornerProperty, HSSObservablePropertyRightTop, this);
        }
        this->cornerTR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTRCorner,
                                              this->observedTRCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTR);
}

void HSSRoundedRect::setDRight(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedTRCorner)
        {
            this->observedTRCorner->removeObserver(this->observedTRCornerProperty, HSSObservablePropertyRight, this);
        }
        this->cornerTR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerTRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedTRCorner,
                                              this->observedTRCornerProperty,
                                              this->scope);
        if (this->observedBRCorner)
        {
            this->observedBRCorner->removeObserver(this->observedBRCornerProperty, HSSObservablePropertyRight, this);
        }
        this->cornerBR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBRCorner,
                                              this->observedBRCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerTR);
    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBR);
}

void HSSRoundedRect::setDRightBottom(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedBRCorner)
        {
            this->observedBRCorner->removeObserver(this->observedBRCornerProperty, HSSObservablePropertyRightBottom, this);
        }
        this->cornerBR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBRCorner,
                                              this->observedBRCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBR);
}

void HSSRoundedRect::setDBottom(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedBRCorner)
        {
            this->observedBRCorner->removeObserver(this->observedBRCornerProperty, HSSObservablePropertyBottom, this);
        }
        this->cornerBR = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBRChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBRCorner,
                                              this->observedBRCornerProperty,
                                              this->scope);
        if (this->observedBLCorner)
        {
            this->observedBLCorner->removeObserver(this->observedBLCornerProperty, HSSObservablePropertyBottom, this);
        }
        this->cornerBL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBLCorner,
                                              this->observedBLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBL);
    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBR);
}

void HSSRoundedRect::setDLeftBottom(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeFunctionCall:
    {
        if (this->observedBLCorner)
        {
            this->observedBLCorner->removeObserver(this->observedBLCornerProperty, HSSObservablePropertyLeftBottom, this);
        }
        this->cornerBL = this->_evaluatePropertyValue(
                                              &HSSRoundedRect::cornerBLChanged,
                                              value,
                                              100.,
                                              HSSObservablePropertyCorners,
                                              this,
                                              HSSObservablePropertyValue,
                                              this->observedBLCorner,
                                              this->observedBLCornerProperty,
                                              this->scope);
        break;
    }

    default:
        throw AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object " + this->name);
    }

    this->notifyObservers(HSSObservablePropertyCorners, &this->cornerBL);
}

void HSSRoundedRect::cornerTLChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerTL = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerTRChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerTR = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerBRChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerBR = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerBLChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerBL = *(HSSUnit*) data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::vector<QSharedPointer<HSSParserNode> > segments)
{
    this->createRoundedRect(path, x, y, width, height, 0.);
}

void HSSRoundedRect::createRoundedRect(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, HSSUnit offset)
{
    HSSUnit topLeftOffset = (cornerTL*2+offset);
    if(topLeftOffset < 1) topLeftOffset = 1;
    HSSUnit bottomLeftOffset = (cornerBL*2+offset);
    if(bottomLeftOffset < 1) bottomLeftOffset = 1;
    HSSUnit bottomRightOffset = (cornerBR*2+offset);
    if(bottomRightOffset < 1) bottomRightOffset = 1;
    HSSUnit topRightOffset = (cornerTR*2+offset);
    if(topRightOffset < 1) topRightOffset = 1;
    QRectF topLeftBounds(x, y, topLeftOffset, topLeftOffset);
    QRectF bottomLeftBounds(x, y + height - bottomLeftOffset, bottomLeftOffset, bottomLeftOffset);
    QRectF bottomRightBounds(x + width - bottomRightOffset, y + height - bottomRightOffset, bottomRightOffset, bottomRightOffset);
    QRectF topRightBounds(x + width - topRightOffset, y, topRightOffset, topRightOffset);

    QVector<QRectF> corners;
    corners << topLeftBounds << bottomLeftBounds << bottomRightBounds << topRightBounds;

    path.arcMoveTo(corners[0], 90);

    for (int i = 1; i <= corners.size(); ++i)
    {
        path.arcTo(corners[i - 1], i * 90, 90);
    }

    path.closeSubpath();
}

void HSSRoundedRect::drawBorders(QPainter &painter, std::vector<QSharedPointer<HSSBorder> > borders, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY)
{
    //sort borders in three groups
    std::vector<QSharedPointer<HSSBorder> > center, inside, outside;
    for (HSSBorder::it it=borders.begin(); it!=borders.end(); ++it) {
        const QSharedPointer<HSSBorder> & theBorder = *it;
        HSSBorderPosition thePos = theBorder->getPosition();
        if (thePos == HSSBorderPositionCenter)
        {
            center.push_back(theBorder);
        }
        else if (thePos == HSSBorderPositionInside)
        {
            inside.push_back(theBorder);
        }
        else if (thePos == HSSBorderPositionOutside)
        {
            outside.push_back(theBorder);
        }
    }

    HSSUnit topThickness = 0., rightThickness = 0., bottomThickness = 0., leftThickness = 0.;
    for (HSSBorder::it it=center.begin(); it!=center.end(); ++it) {
        const QSharedPointer<HSSBorder> & theBorder = *it;
        const std::vector<QSharedPointer<HSSParserNode> > & segments = theBorder->getSegments();
        bool hasAll = false;
        for (std::vector<QSharedPointer<HSSParserNode> >::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
            const QSharedPointer<HSSParserNode> & segment = *it;
            if(segment->isA(HSSParserNodeTypeKeywordConstant))
            {
                QSharedPointer<HSSKeywordConstant> theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                if (theKw->getValue() == "all")
                {
                    hasAll = true;
                    break;
                }
            }
        }
        if(segments.size() == 0 || hasAll){
            topThickness += theBorder->getSize();
            rightThickness += theBorder->getSize();
            bottomThickness += theBorder->getSize();
            leftThickness += theBorder->getSize();
        }
        else
        {
            for (HSSParserNode::const_it it2=segments.begin(); it2!=segments.end(); ++it2) {
                const QSharedPointer<HSSParserNode> & theSeg = *it2;
                if (theSeg->isA(HSSParserNodeTypeKeywordConstant)) {
                    AXRString segment = qSharedPointerCast<HSSKeywordConstant>(theSeg)->getValue();
                    if(segment == "top"){
                        topThickness += theBorder->getSize();
                    }
                    else if(segment == "right")
                    {
                        rightThickness += theBorder->getSize();
                    }
                    else if(segment == "bottom")
                    {
                        bottomThickness += theBorder->getSize();
                    }
                    else if(segment == "left")
                    {
                        leftThickness += theBorder->getSize();
                    }
                }
            }
        }
    }

    HSSUnit topCorrection = 0., rightCorrection = 0., bottomCorrection = 0., leftCorrection = 0.;
    if ((int) topThickness % 2) {
        topCorrection = 0.5;
    }
    if ((int) rightThickness % 2) {
        rightCorrection = 0.5;
    }
    if ((int) bottomThickness % 2) {
        bottomCorrection = 0.5;
    }
    if ((int) leftThickness % 2) {
        leftCorrection = 0.5;
    }

    // Uncomment this to see the dimensions of the borders surface
//    QPainterPath outerPath;
//    outerPath.addRect(0.5, 0.5, width+borderBleeding*2-1, height+borderBleeding*2-1);
//    painter.strokePath(outerPath, QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

    HSSUnit topCumulative = 0., rightCumulative = 0., bottomCumulative = 0., leftCumulative = 0.;
    for (HSSBorder::it it=center.begin(); it!=center.end(); ++it) {
        const QSharedPointer<HSSBorder> & theBorder = *it;
        HSSUnit theSize = theBorder->getSize();

        std::vector<QSharedPointer<HSSParserNode> > segments = theBorder->getSegments();

        bool hasAll = false;
        for (std::vector<QSharedPointer<HSSParserNode> >::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
            const QSharedPointer<HSSParserNode> & segment = *it;
            if(segment->isA(HSSParserNodeTypeKeywordConstant))
            {
                QSharedPointer<HSSKeywordConstant> theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                if (theKw->getValue() == "all")
                {
                    hasAll = true;
                    break;
                }
            }
        }
        if(segments.size() > 0 && !hasAll){
            for (std::vector<QSharedPointer<HSSParserNode> >::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
                QPainterPath path;
                const QSharedPointer<HSSParserNode> & segment = *it;
                switch (segment->getType()) {
                    case HSSParserNodeTypeKeywordConstant:
                    {
                        QSharedPointer<HSSKeywordConstant> theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                        AXRString theValue = theKw->getValue();
                        if (theValue == "top")
                        {
                            HSSUnit leftOffset = (leftThickness / 2) - leftCumulative + leftCorrection;
                            HSSUnit topOffset = (topThickness / 2) - topCumulative - (theSize / 2) + topCorrection;
                            HSSUnit rightOffset = (rightThickness / 2) - rightCumulative + rightCorrection;

                            path.moveTo(offsetX+leftOffset, offsetY+topOffset);
                            path.lineTo(offsetX+width-rightOffset, offsetY+topOffset);
                            topCumulative += theSize;
                        }
                        else if (theValue == "right")
                        {
                            HSSUnit topOffset = (topThickness / 2) - topCumulative + topCorrection;
                            HSSUnit rightOffset = (rightThickness / 2) - rightCumulative - (theSize / 2) + rightCorrection;
                            HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative + bottomCorrection;

                            path.moveTo(offsetX+width-rightOffset, offsetY+topOffset);
                            path.lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
                            rightCumulative += theSize;
                        }
                        else if (theValue == "bottom")
                        {
                            HSSUnit leftOffset = (leftThickness / 2) - leftCumulative + leftCorrection;
                            HSSUnit rightOffset = (rightThickness / 2) - rightCumulative + rightCorrection;
                            HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative - (theSize / 2) + bottomCorrection;

                            path.moveTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
                            path.lineTo(offsetX+leftOffset, offsetY+height-bottomOffset);
                            bottomCumulative += theSize;
                        }
                        else if(theValue == "left")
                        {
                            HSSUnit leftOffset = (leftThickness / 2) - leftCumulative - (theSize / 2) + leftCorrection;
                            HSSUnit topOffset = (topThickness / 2) - topCumulative + topCorrection;
                            HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative + bottomCorrection;

                            path.moveTo(offsetX+leftOffset, offsetY+height-bottomOffset);
                            path.lineTo(offsetX+leftOffset, offsetY+topOffset);
                            leftCumulative += theSize;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
        } else {
            ///@todo uncomment this once there are border types
            //if(theBorder->isA(HSSBorderTypeLineBorder){
                QPainterPath path;
                QSharedPointer<HSSLineBorder> theLineBorder = qSharedPointerCast<HSSLineBorder>(theBorder);
                QSharedPointer<HSSObject> theColorObj = theLineBorder->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    QPainterPath outerPath, innerPath;
                    HSSUnit outerLeftOffset = (leftThickness/2)-leftCumulative-theSize+leftCorrection;
                    HSSUnit outerTopOffset = (topThickness/2)-topCumulative-theSize+topCorrection;
                    HSSUnit outerRightOffset = (rightThickness/2)-rightCumulative-theSize+rightCorrection;
                    HSSUnit outerBottomOffset = (bottomThickness/2)-bottomCumulative-theSize+bottomCorrection;
                    this->createRoundedRect(
                                            outerPath,
                                            offsetX+outerLeftOffset,
                                            offsetY+outerTopOffset,
                                            -outerLeftOffset+width-outerRightOffset,
                                            -outerTopOffset+height-outerBottomOffset,
                                            -outerLeftOffset*2
                    );
                    HSSUnit innerLeftOffset = (leftThickness/2)-leftCumulative+leftCorrection;
                    HSSUnit innerTopOffset = (topThickness/2)-topCumulative+topCorrection;
                    HSSUnit innerRightOffset = (rightThickness/2)-rightCumulative+rightCorrection;
                    HSSUnit innerBottomOffset = (bottomThickness/2)-bottomCumulative+bottomCorrection;
                    this->createRoundedRect(
                                            innerPath,
                                            offsetX+innerLeftOffset,
                                            offsetY+innerTopOffset,
                                            -innerLeftOffset+width-innerRightOffset,
                                            -innerTopOffset+height-innerBottomOffset,
                                            -innerLeftOffset*2);
                    path = outerPath.subtracted(innerPath);
                    painter.fillPath(path, theColor->toQColor());
                }
            //}

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }
        //theBorder->draw(painter, path);
    }

    topCumulative = rightCumulative = bottomCumulative = leftCumulative = 0.;
    for (std::vector<QSharedPointer<HSSBorder> >::reverse_iterator it=inside.rbegin(); it!=inside.rend(); ++it) {
        const QSharedPointer<HSSBorder> & theBorder = *it;
        HSSUnit theSize = theBorder->getSize();

        std::vector<QSharedPointer<HSSParserNode> > segments = theBorder->getSegments();

        bool hasAll = false;
        for (std::vector<QSharedPointer<HSSParserNode> >::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
            const QSharedPointer<HSSParserNode> & segment = *it;
            if(segment->isA(HSSParserNodeTypeKeywordConstant))
            {
                QSharedPointer<HSSKeywordConstant> theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                if (theKw->getValue() == "all")
                {
                    hasAll = true;
                    break;
                }
            }
        }
        if(segments.size() > 0 && !hasAll){
            for (std::vector<QSharedPointer<HSSParserNode> >::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
                QPainterPath path;
                const QSharedPointer<HSSParserNode> & segment = *it;
                switch (segment->getType()) {
                    case HSSParserNodeTypeKeywordConstant:
                    {
                        ///@todo uncomment this once there are border types
                        //if(theBorder->isA(HSSBorderTypeLineBorder){
                            QSharedPointer<HSSLineBorder> theLineBorder = qSharedPointerCast<HSSLineBorder>(theBorder);
                            QSharedPointer<HSSObject> theColorObj = theLineBorder->getColor();
                            if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                                QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                                QSharedPointer<HSSKeywordConstant> theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                                AXRString theValue = theKw->getValue();
                                if (theValue == "top")
                                {
                                    HSSUnit leftOffset = leftCumulative+leftCorrection;
                                    HSSUnit topOffset = topCumulative+topCorrection;
                                    HSSUnit rightOffset = rightCumulative+rightCorrection+(this->cornerTR*2);

                                    QRectF curve1(offsetX+leftOffset, offsetY+topOffset, this->cornerTL*2, this->cornerTL*2);
                                    if (this->cornerTL != 0)
                                    {
                                        path.arcMoveTo(curve1, 90);
                                        path.arcTo(curve1, 90, 90);
                                    }
                                    else
                                    {
                                        path.moveTo(offsetX+leftOffset, offsetY+topOffset);
                                        path.arcTo(curve1, 90, 90);
                                    }
                                    if(this->cornerTL != 0)
                                    {
                                        QRectF curve2(offsetX+leftOffset, offsetY+topOffset+theSize, this->cornerTL*2, (this->cornerTL*2)-theSize);
                                        path.arcTo(curve2, 180, -90);
                                    }
                                    else
                                    {
                                        path.lineTo(offsetX+leftOffset, offsetY+topOffset+theSize);
                                    }
                                    qreal trsize = (this->cornerTR*2)-theSize;
                                    if(trsize > 0)
                                    {
                                        QRectF curve3(offsetX+width-rightOffset, offsetY+topOffset+theSize, this->cornerTR*2, (this->cornerTR*2)-theSize);
                                        path.arcTo(curve3, 90, -90);
                                    }
                                    else
                                    {
                                        path.lineTo(offsetX+width-rightOffset, offsetY+topOffset+theSize);
                                    }
                                    if (this->cornerTR != 0) {
                                        QRectF curve4(offsetX+width-rightOffset, offsetY+topOffset, this->cornerTR*2, this->cornerTR*2);
                                        path.arcTo(curve4, 0, 90);
                                    }
                                    else
                                    {
                                        path.lineTo(offsetX+width-rightOffset, offsetY+topOffset);
                                    }
                                    path.closeSubpath();
                                    painter.fillPath(path, theColor->toQColor());

                                    topCumulative += theSize;
                                }
                                else if (theValue == "right")
                                {
                                    HSSUnit topOffset = topCumulative+topCorrection;
                                    HSSUnit rightOffset = rightCumulative+rightCorrection;
                                    HSSUnit bottomOffset = bottomCumulative+bottomCorrection;

                                    if (this->cornerTR != 0)
                                    {
                                        QRectF curve1(offsetX+width-rightOffset-(this->cornerTR*2), offsetY+topOffset, this->cornerTR*2, this->cornerTR*2);
                                        path.arcMoveTo(curve1, 90);
                                        path.arcTo(curve1, 90, -90);
                                    }
                                    else
                                    {
                                        path.moveTo(offsetX+width-rightOffset, offsetY+topOffset);
                                        path.lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset-this->cornerBR);
                                    }
                                    if (this->cornerBR != 0)
                                    {
                                        QRectF curve2(offsetX+width-rightOffset-(this->cornerBR*2), offsetY+height-(this->cornerBR*2)-bottomOffset, this->cornerBR*2, (this->cornerBR*2));
                                        path.arcTo(curve2, 0, -90);
                                    }
                                    else
                                    {
                                        path.lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
                                    }
                                    qreal brsize = (this->cornerBR*2)-theSize;
                                    if (brsize > 0)
                                    {
                                        QRectF curve3(offsetX+width-(this->cornerBR*2)-rightOffset, offsetY+height-(this->cornerBR*2)-bottomOffset, brsize, this->cornerBR*2);
                                        path.arcTo(curve3, 270, 90);
                                    }
                                    else
                                    {
                                        path.lineTo(offsetX+width-rightOffset-theSize, offsetY+height-bottomOffset);
                                    }
                                    qreal trsize = (this->cornerTR*2)-theSize;
                                    if (trsize > 0)
                                    {
                                        QRectF curve4(offsetX+width-(this->cornerTR*2)-rightOffset, offsetY+topOffset, trsize, this->cornerTR*2);
                                        path.arcTo(curve4, 0, 90);
                                    }
                                    else
                                    {
                                        path.lineTo(offsetX+width-rightOffset-theSize, offsetY+topOffset);
                                    }
                                    path.closeSubpath();
                                    painter.fillPath(path, theColor->toQColor());

                                    rightCumulative += theSize;
                                }
                                else if (theValue == "bottom")
                                {
                                    HSSUnit rightOffset = rightCumulative+rightCorrection;
                                    HSSUnit bottomOffset = bottomCumulative+bottomCorrection;
                                    HSSUnit leftOffset = leftCumulative+leftCorrection;

                                    if(this->cornerBL != 0)
                                    {
                                        QRectF curve1(offsetX+leftOffset, offsetY+height-(this->cornerBL*2)-bottomOffset, this->cornerBL*2, this->cornerBL*2);
                                        path.arcMoveTo(curve1, 180);
                                        path.arcTo(curve1, 180, 90);
                                    }
                                    else
                                    {
                                        path.moveTo(offsetX+leftOffset, offsetY+height-bottomOffset-theSize);
                                        path.lineTo(offsetX+leftOffset, offsetY+height-bottomOffset);
                                    }
                                    if (this->cornerBR != 0)
                                    {
                                        QRectF curve2(offsetX+width-(this->cornerBR*2)-rightOffset, offsetY+height-(this->cornerBR*2)-bottomOffset, this->cornerBR*2, this->cornerBR*2);
                                        path.arcTo(curve2, 270, 90);
                                    }
                                    else
                                    {
                                        path.lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
                                    }
                                    qreal brsize = (this->cornerBR*2)-theSize;
                                    if (brsize > 0)
                                    {
                                        QRectF curve3(offsetX+width-(this->cornerBR*2)-rightOffset, offsetY+height-(this->cornerBR*2)-bottomOffset, this->cornerBR*2, brsize);
                                        path.arcTo(curve3, 0, -90);
                                    }
                                    else
                                    {
                                        path.lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset-theSize);
                                    }
                                    qreal blsize = (this->cornerBL*2)-theSize;
                                    if (blsize > 0)
                                    {
                                        QRectF curve4(offsetX+leftOffset, offsetY+height-(this->cornerBL*2)-bottomOffset, cornerBL*2, blsize);
                                        path.arcTo(curve4, 270, -90);
                                    }
                                    else
                                    {
                                        path.lineTo(offsetX+leftOffset, offsetY+height-bottomOffset-theSize);
                                    }
                                    path.closeSubpath();
                                    painter.fillPath(path, theColor->toQColor());

                                    bottomCumulative += theSize;
                                }
                                else if(theValue == "left")
                                {
                                    HSSUnit bottomOffset = bottomCumulative+bottomCorrection;
                                    HSSUnit leftOffset = leftCumulative+leftCorrection;
                                    HSSUnit topOffset = topCumulative+topCorrection;

                                    QRectF curve1(offsetX+leftOffset, offsetY+topOffset, this->cornerTL*2, this->cornerTL*2);
                                    if(this->cornerTL != 0){
                                        path.arcMoveTo(curve1, 90);
                                        path.arcTo(curve1, 90, 90);
                                    } else {
                                        path.moveTo(offsetX+leftOffset, offsetY+topOffset);
                                        path.arcTo(curve1, 90, 90);
                                    }
                                    if(this->cornerBL != 0){
                                        QRectF curve2(offsetX+leftOffset, offsetY+height-(this->cornerBL*2)-bottomOffset, this->cornerBL*2, this->cornerBL*2);
                                        path.arcTo(curve2, 180, 90);
                                    } else {
                                        path.lineTo(offsetX+leftOffset, offsetY+height-bottomOffset);
                                    }

                                    if((this->cornerBL*2)-theSize > 0){
                                        QRectF curve3(offsetX+leftOffset+theSize, offsetY+height-(this->cornerBL*2)-bottomOffset, (this->cornerBL*2)-theSize, (this->cornerBL*2));
                                        path.arcTo(curve3, 270, -90);
                                    } else {
                                        path.lineTo(offsetX+leftOffset+theSize, offsetY+height-bottomOffset);
                                    }

                                    if(this->cornerTL != 0){
                                        QRectF curve4(offsetX+leftOffset+theSize, offsetY+topOffset, (this->cornerTL*2)-theSize, this->cornerTL*2);
                                        path.arcTo(curve4, 180, -90);
                                    } else {
                                        path.lineTo(offsetX+leftOffset+theSize, offsetY+topOffset);
                                    }

                                    path.closeSubpath();
                                    painter.fillPath(path, theColor->toQColor());

                                    leftCumulative += theSize;
                                }
                            }
                        //}
                        break;
                    }

                    default:
                        break;
                }
            }
        } else {
            ///@todo uncomment this once there are border types
            //if(theBorder->isA(HSSBorderTypeLineBorder){
                QPainterPath path;
                QSharedPointer<HSSLineBorder> theLineBorder = qSharedPointerCast<HSSLineBorder>(theBorder);
                QSharedPointer<HSSObject> theColorObj = theLineBorder->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    QPainterPath outerPath, innerPath;
                    HSSUnit outerLeftOffset = leftCumulative+leftCorrection;
                    HSSUnit outerTopOffset = topCumulative+topCorrection;
                    HSSUnit outerRightOffset = rightCumulative+rightCorrection;
                    HSSUnit outerBottomOffset = bottomCumulative+bottomCorrection;
                    this->createRoundedRect(
                                            outerPath,
                                            offsetX+outerLeftOffset,
                                            offsetY+outerTopOffset,
                                            -outerLeftOffset+width-outerRightOffset,
                                            -outerTopOffset+height-outerBottomOffset,
                                            -outerLeftOffset*2
                                            );
                    HSSUnit innerLeftOffset = theSize+leftCumulative+leftCorrection;
                    HSSUnit innerTopOffset = theSize+topCumulative+topCorrection;
                    HSSUnit innerRightOffset = theSize+rightCumulative+rightCorrection;
                    HSSUnit innerBottomOffset = theSize+bottomCumulative+bottomCorrection;
                    this->createRoundedRect(
                                            innerPath,
                                            offsetX+innerLeftOffset,
                                            offsetY+innerTopOffset,
                                            -innerLeftOffset+width-innerRightOffset,
                                            -innerTopOffset+height-innerBottomOffset,
                                            -innerLeftOffset*2);
                    path = outerPath.subtracted(innerPath);
                    painter.fillPath(path, theColor->toQColor());
                }
            //}

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }
    }

    topCumulative = rightCumulative = bottomCumulative = leftCumulative = 0.;
    for (std::vector<QSharedPointer<HSSBorder> >::iterator it=outside.begin(); it!=outside.end(); ++it) {
        const QSharedPointer<HSSBorder> & theBorder = *it;
        HSSUnit theSize = theBorder->getSize();

        std::vector<QSharedPointer<HSSParserNode> > segments = theBorder->getSegments();

        bool hasAll = false;
        for (std::vector<QSharedPointer<HSSParserNode> >::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
            const QSharedPointer<HSSParserNode> & segment = *it;
            if(segment->isA(HSSParserNodeTypeKeywordConstant))
            {
                QSharedPointer<HSSKeywordConstant> theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                if (theKw->getValue() == "all")
                {
                    hasAll = true;
                    break;
                }
            }
        }
        if(segments.size() > 0 && !hasAll){
            for (std::vector<QSharedPointer<HSSParserNode> >::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
                QPainterPath path;
                const QSharedPointer<HSSParserNode> & segment = *it;
                switch (segment->getType()) {
                    case HSSParserNodeTypeKeywordConstant:
                    {
                        HSSUnit leftOffset = (leftThickness / 2) + leftCumulative + leftCorrection;
                        HSSUnit topOffset = (topThickness / 2) + topCumulative + topCorrection;
                        HSSUnit rightOffset = (rightThickness / 2) + rightCumulative + rightCorrection;
                        HSSUnit bottomOffset = (bottomThickness / 2) + bottomCumulative + bottomCorrection;

                        QSharedPointer<HSSKeywordConstant> theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                        AXRString theValue = theKw->getValue();
                        if (theValue == "top")
                        {
                            path.moveTo(offsetX-leftOffset, offsetY-(topOffset+(theSize/2)));
                            path.lineTo(offsetX+width+rightOffset, offsetY-(topOffset+(theSize/2)));
                            topCumulative += theSize;
                        }
                        else if (theValue == "right")
                        {
                            path.moveTo(offsetX+width+(rightOffset+(theSize/2)), offsetY-topOffset);
                            path.lineTo(offsetX+width+(rightOffset+(theSize/2)), offsetY+height+bottomOffset);
                            rightCumulative += theSize;
                        }
                        else if (theValue == "bottom")
                        {
                            path.moveTo(offsetX+width+rightOffset, offsetY+height+(bottomOffset+(theSize/2)));
                            path.lineTo(offsetX-leftOffset, offsetY+height+(bottomOffset+(theSize/2)));
                            bottomCumulative += theSize;
                        }
                        else if(theValue == "left")
                        {
                            path.moveTo(offsetX-(leftOffset+(theSize/2)), offsetY+height+bottomOffset);
                            path.lineTo(offsetX-(leftOffset+(theSize/2)), offsetY-topOffset);
                            leftCumulative += theSize;
                        }
                        break;
                    }

                    default:
                        break;
                }
            }
        } else {
            ///@todo uncomment this once there are border types
            //if(theBorder->isA(HSSBorderTypeLineBorder){
                QPainterPath path;
                QSharedPointer<HSSLineBorder> theLineBorder = qSharedPointerCast<HSSLineBorder>(theBorder);
                QSharedPointer<HSSObject> theColorObj = theLineBorder->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    QPainterPath outerPath, innerPath;
                    HSSUnit outerLeftOffset = theSize+leftCumulative+leftCorrection;
                    HSSUnit outerTopOffset = theSize+topCumulative+topCorrection;
                    HSSUnit outerRightOffset = theSize+rightCumulative+rightCorrection;
                    HSSUnit outerBottomOffset = theSize+bottomCumulative+bottomCorrection;
                    this->createRoundedRect(
                                            outerPath,
                                            offsetX-outerLeftOffset,
                                            offsetY-outerTopOffset,
                                            outerLeftOffset+width+outerRightOffset,
                                            outerTopOffset+height+outerBottomOffset,
                                            outerLeftOffset*2
                                            );
                    HSSUnit innerLeftOffset = leftCumulative+leftCorrection;
                    HSSUnit innerTopOffset = topCumulative+topCorrection;
                    HSSUnit innerRightOffset = rightCumulative+rightCorrection;
                    HSSUnit innerBottomOffset = bottomCumulative+bottomCorrection;
                    this->createRoundedRect(
                                            innerPath,
                                            offsetX-innerLeftOffset,
                                            offsetY-innerTopOffset,
                                            innerLeftOffset+width+innerRightOffset,
                                            innerTopOffset+height+innerBottomOffset,
                                            innerLeftOffset*2);
                    path = outerPath.subtracted(innerPath);
                    painter.fillPath(path, theColor->toQColor());
                }
            //}
            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }
    }
}

HSSUnit HSSRoundedRect::_evaluatePropertyValue(
                                           void(HSSRoundedRect::*callback)(HSSObservableProperty property, void* data),
                                           QSharedPointer<HSSParserNode> value,
                                           HSSUnit percentageBase,
                                           HSSObservableProperty observedProperty,
                                           HSSObservable * observedObject,
                                           HSSObservableProperty observedSourceProperty,
                                           HSSObservable * &observedStore,
                                           HSSObservableProperty &observedStoreProperty,
                                           QSharedPointer<HSSSimpleSelection> scope
                                           )
{
    HSSUnit ret = 0;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        QSharedPointer<HSSNumberConstant> numberValue = qSharedPointerCast<HSSNumberConstant>(value);
        ret = numberValue->getValue();
        observedStore = NULL;
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant>(value);
        ret = percentageValue->getValue(percentageBase);
        if (callback)
        {
            observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSRoundedRect > (this, callback));
            observedStore = observedObject;
            observedStoreProperty = observedProperty;
        }
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression>(value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setPercentageObserved(observedProperty, observedObject);
        expressionValue->setScope(scope);
        expressionValue->setThisObj(this->getThisObj());
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSRoundedRect > (this, callback));
            observedStore = expressionValue.data();
            observedStoreProperty = HSSObservablePropertyValue;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:

        break;

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction>(value)->clone();
        fnct->setPercentageBase(percentageBase);
        fnct->setPercentageObserved(observedProperty, observedObject);
        fnct->setScope(scope);
        fnct->setThisObj(this->getThisObj());

        QVariant remoteValue = fnct->evaluate();
        if (remoteValue.canConvert<HSSUnit>())
        {
            ret = remoteValue.value<HSSUnit>();
        }

        if (callback)
        {
            fnct->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSRoundedRect > (this, callback));
            observedStore = fnct.data();
            observedStoreProperty = HSSObservablePropertyValue;
        }
        break;
    }

    default:
        break;
    }

    return ret;
}
