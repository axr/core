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

#include "AXRDebugging.h"
#include "AXRWarning.h"
#include "HSSBorder.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSLineBorder.h"
#include "HSSNumberConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRoundedRect.h"

using namespace AXR;

HSSRoundedRect::HSSRoundedRect(AXRController * controller)
: HSSShape(HSSShapeTypeRoundedRect, controller)
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRoundedRect: creating rounded rectangle object");
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

HSSRoundedRect::p HSSRoundedRect::clone() const
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRoundedRect: cloning rounded rectangle object");
    return qSharedPointerCast<HSSRoundedRect>(this->cloneImpl());
}

HSSClonable::p HSSRoundedRect::cloneImpl() const
{
    return HSSRoundedRect::p(new HSSRoundedRect(*this));
}

HSSRoundedRect::~HSSRoundedRect()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSRoundedRect: destructing rounded rectangle object");
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

void HSSRoundedRect::setProperty(HSSObservableProperty name, HSSParserNode::p value)
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

HSSMultipleValue::p HSSRoundedRect::getCorners()
{
    return this->corners;
}

void HSSRoundedRect::setDCorners(HSSParserNode::p value)
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
    HSSMultipleValue::p newCorners(new HSSMultipleValue(this->getController()));
    newCorners->add(value);
    this->corners = newCorners;

    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::setDLeft(HSSParserNode::p value)
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

void HSSRoundedRect::setDLeftTop(HSSParserNode::p value)
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

void HSSRoundedRect::setDTop(HSSParserNode::p value)
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

void HSSRoundedRect::setDRightTop(HSSParserNode::p value)
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

void HSSRoundedRect::setDRight(HSSParserNode::p value)
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

void HSSRoundedRect::setDRightBottom(HSSParserNode::p value)
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

void HSSRoundedRect::setDBottom(HSSParserNode::p value)
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

void HSSRoundedRect::setDLeftBottom(HSSParserNode::p value)
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

void HSSRoundedRect::createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::vector<HSSParserNode::p> segments)
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

void HSSRoundedRect::drawBorders(QPainter &painter, std::vector<QSharedPointer<HSSBorder> > borders, HSSUnit width, HSSUnit height, HSSUnit borderBleeding)
{
    //sort borders in three groups
    std::vector<HSSBorder::p> centered, inside, outside;
    for (HSSBorder::it it=borders.begin(); it!=borders.end(); ++it) {
        const HSSBorder::p & theBorder = *it;
        HSSBorderPosition thePos = theBorder->getPosition();
        if (thePos == HSSBorderPositionCentered)
        {
            centered.push_back(theBorder);
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
    for (HSSBorder::it it=centered.begin(); it!=centered.end(); ++it) {
        const HSSBorder::p & theBorder = *it;
        const std::vector<HSSParserNode::p> & segments = theBorder->getSegments();
        bool hasAll = false;
        for (std::vector<HSSParserNode::p>::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
            const HSSParserNode::p & segment = *it;
            if(segment->isA(HSSParserNodeTypeKeywordConstant))
            {
                HSSKeywordConstant::p theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
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
                const HSSParserNode::p & theSeg = *it2;
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
    for (HSSBorder::it it=centered.begin(); it!=centered.end(); ++it) {
        const HSSBorder::p & theBorder = *it;
        HSSUnit theSize = theBorder->getSize();

        std::vector<HSSParserNode::p> segments = theBorder->getSegments();

        bool hasAll = false;
        for (std::vector<HSSParserNode::p>::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
            const HSSParserNode::p & segment = *it;
            if(segment->isA(HSSParserNodeTypeKeywordConstant))
            {
                HSSKeywordConstant::p theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                if (theKw->getValue() == "all")
                {
                    hasAll = true;
                    break;
                }
            }
        }
        if(segments.size() > 0 && !hasAll){
            for (std::vector<HSSParserNode::p>::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
                QPainterPath path;
                const HSSParserNode::p & segment = *it;
                switch (segment->getType()) {
                    case HSSParserNodeTypeKeywordConstant:
                    {
                        HSSKeywordConstant::p theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                        AXRString theValue = theKw->getValue();
                        if (theValue == "top")
                        {
                            HSSUnit leftOffset = (leftThickness / 2) - leftCumulative + leftCorrection;
                            HSSUnit topOffset = (topThickness / 2) - topCumulative - (theSize / 2) + topCorrection;
                            HSSUnit rightOffset = (rightThickness / 2) - rightCumulative + rightCorrection;

                            path.moveTo(borderBleeding+leftOffset, borderBleeding+topOffset);
                            path.lineTo(borderBleeding+width-rightOffset, borderBleeding+topOffset);
                            topCumulative += theSize;
                        }
                        else if (theValue == "right")
                        {
                            HSSUnit topOffset = (topThickness / 2) - topCumulative + topCorrection;
                            HSSUnit rightOffset = (rightThickness / 2) - rightCumulative - (theSize / 2) + rightCorrection;
                            HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative + bottomCorrection;

                            path.moveTo(borderBleeding+width-rightOffset, borderBleeding+topOffset);
                            path.lineTo(borderBleeding+width-rightOffset, borderBleeding+height-bottomOffset);
                            rightCumulative += theSize;
                        }
                        else if (theValue == "bottom")
                        {
                            HSSUnit leftOffset = (leftThickness / 2) - leftCumulative + leftCorrection;
                            HSSUnit rightOffset = (rightThickness / 2) - rightCumulative + rightCorrection;
                            HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative - (theSize / 2) + bottomCorrection;

                            path.moveTo(borderBleeding+width-rightOffset, borderBleeding+height-bottomOffset);
                            path.lineTo(borderBleeding+leftOffset, borderBleeding+height-bottomOffset);
                            bottomCumulative += theSize;
                        }
                        else if(theValue == "left")
                        {
                            HSSUnit leftOffset = (leftThickness / 2) - leftCumulative - (theSize / 2) + leftCorrection;
                            HSSUnit topOffset = (topThickness / 2) - topCumulative + topCorrection;
                            HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative + bottomCorrection;

                            path.moveTo(borderBleeding+leftOffset, borderBleeding+height-bottomOffset);
                            path.lineTo(borderBleeding+leftOffset, borderBleeding+topOffset);
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
                HSSLineBorder::p theLineBorder = qSharedPointerCast<HSSLineBorder>(theBorder);
                HSSObject::p theColorObj = theLineBorder->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    HSSRgb::p theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    QPainterPath outerPath, innerPath;
                    HSSUnit outerLeftOffset = (leftThickness/2)-leftCumulative-theSize+leftCorrection;
                    HSSUnit outerTopOffset = (topThickness/2)-topCumulative-theSize+topCorrection;
                    HSSUnit outerRightOffset = (rightThickness/2)-rightCumulative-theSize+rightCorrection;
                    HSSUnit outerBottomOffset = (bottomThickness/2)-bottomCumulative-theSize+bottomCorrection;
                    this->createRoundedRect(
                                            outerPath,
                                            borderBleeding+outerLeftOffset,
                                            borderBleeding+outerTopOffset,
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
                                            borderBleeding+innerLeftOffset,
                                            borderBleeding+innerTopOffset,
                                            -innerLeftOffset+width-innerRightOffset,
                                            -innerTopOffset+height-innerBottomOffset,
                                            -innerLeftOffset*2);
                    path = outerPath.subtracted(innerPath);
                    painter.fillPath(path, QColor(theColor->getRed(), theColor->getGreen(), theColor->getBlue(), theColor->getAlpha()));
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
    for (std::vector<HSSBorder::p>::reverse_iterator it=inside.rbegin(); it!=inside.rend(); ++it) {
        const HSSBorder::p & theBorder = *it;
        HSSUnit theSize = theBorder->getSize();

        std::vector<HSSParserNode::p> segments = theBorder->getSegments();

        bool hasAll = false;
        for (std::vector<HSSParserNode::p>::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
            const HSSParserNode::p & segment = *it;
            if(segment->isA(HSSParserNodeTypeKeywordConstant))
            {
                HSSKeywordConstant::p theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                if (theKw->getValue() == "all")
                {
                    hasAll = true;
                    break;
                }
            }
        }
        if(segments.size() > 0 && !hasAll){
            for (std::vector<HSSParserNode::p>::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
                QPainterPath path;
                const HSSParserNode::p & segment = *it;
                switch (segment->getType()) {
                    case HSSParserNodeTypeKeywordConstant:
                    {
                        ///@todo uncomment this once there are border types
                        //if(theBorder->isA(HSSBorderTypeLineBorder){
                            HSSLineBorder::p theLineBorder = qSharedPointerCast<HSSLineBorder>(theBorder);
                            HSSObject::p theColorObj = theLineBorder->getColor();
                            if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                                HSSRgb::p theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                                HSSKeywordConstant::p theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                                AXRString theValue = theKw->getValue();
                                if (theValue == "top")
                                {
                                    HSSUnit leftOffset = leftCumulative+leftCorrection;
                                    HSSUnit topOffset = topCumulative+topCorrection;
                                    HSSUnit rightOffset = rightCumulative+rightCorrection+(this->cornerTR*2);

                                    QRectF curve1(borderBleeding+leftOffset, borderBleeding+topOffset, this->cornerTL*2, this->cornerTL*2);
                                    QRectF curve2(borderBleeding+leftOffset, borderBleeding+topOffset+theSize, this->cornerTL*2, (this->cornerTL*2)-theSize);
                                    QRectF curve3(borderBleeding+leftOffset+width-rightOffset-leftOffset, borderBleeding+topOffset+theSize, this->cornerTR*2, (this->cornerTR*2)-theSize);
                                    QRectF curve4(borderBleeding+leftOffset+width-rightOffset-leftOffset, borderBleeding+topOffset, this->cornerTR*2, this->cornerTR*2);

                                    path.arcMoveTo(curve1, 90);
                                    path.arcTo(curve1, 90, 90);
                                    path.arcTo(curve2, 180, -90);
                                    path.arcTo(curve3, 90, -90);
                                    path.arcTo(curve4, 0, 90);
                                    path.closeSubpath();
                                    painter.fillPath(path, QColor(theColor->getRed(), theColor->getGreen(), theColor->getBlue(), theColor->getAlpha()));

                                    topCumulative += theSize;
                                }
                                else if (theValue == "right")
                                {
                                    HSSUnit topOffset = topCumulative+topCorrection;
                                    HSSUnit rightOffset = rightCumulative+rightCorrection;
                                    HSSUnit bottomOffset = bottomCumulative+bottomCorrection;

                                    QRectF curve1(borderBleeding+width-rightOffset-(this->cornerTR*2), borderBleeding+topOffset, this->cornerTR*2, this->cornerTR*2);
                                    QRectF curve2(borderBleeding+width-rightOffset-(this->cornerTR*2), borderBleeding+height-(this->cornerTR*2)-bottomOffset, this->cornerTR*2, (this->cornerTR*2));
                                    QRectF curve3(borderBleeding+width-(this->cornerBR*2)-rightOffset, borderBleeding+height-(this->cornerBR*2)-bottomOffset, (this->cornerTR*2)-theSize, (this->cornerTR*2));
                                    QRectF curve4(borderBleeding+width-(this->cornerTR*2)-rightOffset, borderBleeding+topOffset, (this->cornerTR*2)-theSize, this->cornerTR*2);

                                    path.arcMoveTo(curve1, 90);
                                    path.arcTo(curve1, 90, -90);
                                    path.arcTo(curve2, 0, -90);
                                    path.arcTo(curve3, 270, 90);
                                    path.arcTo(curve4, 0, 90);
                                    path.closeSubpath();
                                    painter.fillPath(path, QColor(theColor->getRed(), theColor->getGreen(), theColor->getBlue(), theColor->getAlpha()));

                                    rightCumulative += theSize;
                                }
                                else if (theValue == "bottom")
                                {
                                    HSSUnit rightOffset = rightCumulative+rightCorrection;
                                    HSSUnit bottomOffset = bottomCumulative+bottomCorrection;
                                    HSSUnit leftOffset = leftCumulative+leftCorrection;

                                    QRectF curve1(borderBleeding+leftOffset, borderBleeding+height-(this->cornerBL*2)-bottomOffset, this->cornerBR*2, this->cornerBR*2);
                                    QRectF curve2(borderBleeding+leftOffset+width-(this->cornerBR*2)-leftOffset-rightOffset, borderBleeding+height+theSize-(this->cornerBR*2)-bottomOffset, this->cornerBR*2, (this->cornerBR*2)-theSize);
                                    QRectF curve3(borderBleeding+leftOffset+width-(this->cornerBR*2)-leftOffset-rightOffset, borderBleeding+height-(this->cornerBR*2)-bottomOffset, this->cornerBR*2, (this->cornerBR*2)-theSize);
                                    QRectF curve4(borderBleeding+leftOffset, borderBleeding+height-(this->cornerBL*2)-bottomOffset, cornerBL*2, (this->cornerBL*2)-theSize);

                                    path.arcMoveTo(curve1, 180);
                                    path.arcTo(curve1, 180, 90);
                                    path.arcTo(curve2, 270, 90);
                                    path.arcTo(curve3, 0, -90);
                                    path.arcTo(curve4, 270, -90);
                                    path.closeSubpath();
                                    painter.fillPath(path, QColor(theColor->getRed(), theColor->getGreen(), theColor->getBlue(), theColor->getAlpha()));

                                    bottomCumulative += theSize;
                                }
                                else if(theValue == "left")
                                {
                                    HSSUnit bottomOffset = bottomCumulative+bottomCorrection;
                                    HSSUnit leftOffset = leftCumulative+leftCorrection;
                                    HSSUnit topOffset = topCumulative+topCorrection;

                                    QRectF curve1(borderBleeding+leftOffset, borderBleeding+topOffset, this->cornerTL*2, this->cornerTL*2);
                                    QRectF curve2(borderBleeding+leftOffset, borderBleeding+height-(this->cornerTL*2)-bottomOffset, this->cornerTL*2, (this->cornerTL*2));
                                    QRectF curve3(borderBleeding+leftOffset+theSize, borderBleeding+height-(this->cornerBL*2)-bottomOffset, (this->cornerBL*2)-theSize, (this->cornerBL*2));
                                    QRectF curve4(borderBleeding+leftOffset+theSize, borderBleeding+topOffset, (this->cornerTL*2)-theSize, this->cornerTL*2);

                                    path.arcMoveTo(curve1, 90);
                                    path.arcTo(curve1, 90, 90);
                                    path.arcTo(curve2, 180, 90);
                                    path.arcTo(curve3, 270, -90);
                                    path.arcTo(curve4, 180, -90);
                                    path.closeSubpath();
                                    painter.fillPath(path, QColor(theColor->getRed(), theColor->getGreen(), theColor->getBlue(), theColor->getAlpha()));

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
                HSSLineBorder::p theLineBorder = qSharedPointerCast<HSSLineBorder>(theBorder);
                HSSObject::p theColorObj = theLineBorder->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    HSSRgb::p theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    QPainterPath outerPath, innerPath;
                    HSSUnit outerLeftOffset = leftCumulative+leftCorrection;
                    HSSUnit outerTopOffset = topCumulative+topCorrection;
                    HSSUnit outerRightOffset = rightCumulative+rightCorrection;
                    HSSUnit outerBottomOffset = bottomCumulative+bottomCorrection;
                    this->createRoundedRect(
                                            outerPath,
                                            borderBleeding+outerLeftOffset,
                                            borderBleeding+outerTopOffset,
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
                                            borderBleeding+innerLeftOffset,
                                            borderBleeding+innerTopOffset,
                                            -innerLeftOffset+width-innerRightOffset,
                                            -innerTopOffset+height-innerBottomOffset,
                                            -innerLeftOffset*2);
                    path = outerPath.subtracted(innerPath);
                    painter.fillPath(path, QColor(theColor->getRed(), theColor->getGreen(), theColor->getBlue(), theColor->getAlpha()));
                }
            //}

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }
    }

    topCumulative = rightCumulative = bottomCumulative = leftCumulative = 0.;
    for (std::vector<HSSBorder::p>::iterator it=outside.begin(); it!=outside.end(); ++it) {
        const HSSBorder::p & theBorder = *it;
        HSSUnit theSize = theBorder->getSize();

        std::vector<HSSParserNode::p> segments = theBorder->getSegments();

        bool hasAll = false;
        for (std::vector<HSSParserNode::p>::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
            const HSSParserNode::p & segment = *it;
            if(segment->isA(HSSParserNodeTypeKeywordConstant))
            {
                HSSKeywordConstant::p theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                if (theKw->getValue() == "all")
                {
                    hasAll = true;
                    break;
                }
            }
        }
        if(segments.size() > 0 && !hasAll){
            for (std::vector<HSSParserNode::p>::const_iterator it = segments.begin(); it!=segments.end(); ++it) {
                QPainterPath path;
                const HSSParserNode::p & segment = *it;
                switch (segment->getType()) {
                    case HSSParserNodeTypeKeywordConstant:
                    {
                        HSSUnit leftOffset = (leftThickness / 2) + leftCumulative + leftCorrection;
                        HSSUnit topOffset = (topThickness / 2) + topCumulative + topCorrection;
                        HSSUnit rightOffset = (rightThickness / 2) + rightCumulative + rightCorrection;
                        HSSUnit bottomOffset = (bottomThickness / 2) + bottomCumulative + bottomCorrection;

                        HSSKeywordConstant::p theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                        AXRString theValue = theKw->getValue();
                        if (theValue == "top")
                        {
                            path.moveTo(borderBleeding-leftOffset, borderBleeding-(topOffset+(theSize/2)));
                            path.lineTo(borderBleeding+width+rightOffset, borderBleeding-(topOffset+(theSize/2)));
                            topCumulative += theSize;
                        }
                        else if (theValue == "right")
                        {
                            path.moveTo(borderBleeding+width+(rightOffset+(theSize/2)), borderBleeding-topOffset);
                            path.lineTo(borderBleeding+width+(rightOffset+(theSize/2)), borderBleeding+height+bottomOffset);
                            rightCumulative += theSize;
                        }
                        else if (theValue == "bottom")
                        {
                            path.moveTo(borderBleeding+width+rightOffset, borderBleeding+height+(bottomOffset+(theSize/2)));
                            path.lineTo(borderBleeding-leftOffset, borderBleeding+height+(bottomOffset+(theSize/2)));
                            bottomCumulative += theSize;
                        }
                        else if(theValue == "left")
                        {
                            path.moveTo(borderBleeding-(leftOffset+(theSize/2)), borderBleeding+height+bottomOffset);
                            path.lineTo(borderBleeding-(leftOffset+(theSize/2)), borderBleeding-topOffset);
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
                HSSLineBorder::p theLineBorder = qSharedPointerCast<HSSLineBorder>(theBorder);
                HSSObject::p theColorObj = theLineBorder->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    HSSRgb::p theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    QPainterPath outerPath, innerPath;
                    HSSUnit outerLeftOffset = theSize+leftCumulative+leftCorrection;
                    HSSUnit outerTopOffset = theSize+topCumulative+topCorrection;
                    HSSUnit outerRightOffset = theSize+rightCumulative+rightCorrection;
                    HSSUnit outerBottomOffset = theSize+bottomCumulative+bottomCorrection;
                    this->createRoundedRect(
                                            outerPath,
                                            borderBleeding-outerLeftOffset,
                                            borderBleeding-outerTopOffset,
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
                                            borderBleeding-innerLeftOffset,
                                            borderBleeding-innerTopOffset,
                                            innerLeftOffset+width+innerRightOffset,
                                            innerTopOffset+height+innerBottomOffset,
                                            innerLeftOffset*2);
                    path = outerPath.subtracted(innerPath);
                    painter.fillPath(path, QColor(theColor->getRed(), theColor->getGreen(), theColor->getBlue(), theColor->getAlpha()));
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
                                           HSSParserNode::p value,
                                           HSSUnit percentageBase,
                                           HSSObservableProperty observedProperty,
                                           HSSObservable * observedObject,
                                           HSSObservableProperty observedSourceProperty,
                                           HSSObservable * &observedStore,
                                           HSSObservableProperty &observedStoreProperty,
                                           HSSSimpleSelection::p scope
                                           )
{
    HSSUnit ret = 0;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        HSSNumberConstant::p numberValue = qSharedPointerCast<HSSNumberConstant>(value);
        ret = numberValue->getValue();
        observedStore = NULL;
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        HSSPercentageConstant::p percentageValue = qSharedPointerCast<HSSPercentageConstant>(value);
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
        HSSExpression::p expressionValue = qSharedPointerCast<HSSExpression>(value);
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
        HSSFunction::p fnct = qSharedPointerCast<HSSFunction>(value)->clone();
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
