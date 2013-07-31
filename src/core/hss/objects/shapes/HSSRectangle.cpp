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

#include "AXRLoggerManager.h"
#include "HSSBorder.h"
#include "HSSDisplayObject.h"
#include "HSSMultipleValue.h"
#include "HSSKeywordConstant.h"
#include "HSSRectangle.h"
#include "HSSValue.h"
#include <QPainterPath>

using namespace AXR;

HSSRectangle::HSSRectangle(AXRController * controller)
: HSSShape(HSSShapeTypeRectangle, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRectangle: creating rectangle object");
}

HSSRectangle::HSSRectangle(const HSSRectangle & orig)
: HSSShape(orig)
{

}

QSharedPointer<HSSRectangle> HSSRectangle::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRectangle: cloning rectangle object");
    return qSharedPointerCast<HSSRectangle> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSRectangle::cloneImpl() const
{
    return QSharedPointer<HSSRectangle>(new HSSRectangle(*this));
}

HSSRectangle::~HSSRectangle()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRectangle: destructing rectangle object");
}

AXRString HSSRectangle::toString()
{
    return "HSSRectangle";
}

AXRString HSSRectangle::defaultObjectType()
{
    return "HSSRectangle";
}

AXRString HSSRectangle::defaultObjectType(AXRString property)
{
    return HSSShape::defaultObjectType(property);
}

bool HSSRectangle::isKeyword(AXRString value, AXRString property)
{
    return HSSShape::isKeyword(value, property);
}

void HSSRectangle::createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, QList<QSharedPointer<HSSParserNode> > segments)
{
    if(segments.size() > 0){
        Q_FOREACH(const QSharedPointer<HSSParserNode> & segment, segments)
        {
            switch (segment->getType()) {
                case HSSParserNodeTypeKeywordConstant:
                {
                    QSharedPointer<HSSKeywordConstant> theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                    AXRString theValue = theKw->getValue();
                    if(theValue == "left")
                    {
                        path.moveTo(x, y+height);
                        path.lineTo(x, y);
                    }
                    else if (theValue == "top")
                    {
                        path.moveTo(x, y);
                        path.lineTo(x+width, y);
                    }
                    else if (theValue == "right")
                    {
                        path.moveTo(x+width, y);
                        path.lineTo(x+width, y+height);
                    }
                    else if (theValue == "bottom")
                    {
                        path.moveTo(x+width, y+height);
                        path.lineTo(x, y+height);
                    }
                    break;
                }

                default:
                    break;
            }
        }
    } else {
        path.addRect(x, y, width, height);
    }
}

void HSSRectangle::drawBorders(QPainter &painter, QList<QSharedPointer<HSSBorder> > borders, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY)
{
    //sort borders in three groups
    QList<QSharedPointer<HSSBorder> > center, inside, outside;
    Q_FOREACH(const QSharedPointer<HSSBorder> & theBorder, borders)
    {
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
    Q_FOREACH(const QSharedPointer<HSSBorder> & theBorder, center)
    {
        const QSharedPointer<HSSObject> & segmentsObj = theBorder->getSegments();
        bool hasAll = false;
        bool hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);
        if(!hasSegments || hasAll){
            topThickness += theBorder->getSize();
            rightThickness += theBorder->getSize();
            bottomThickness += theBorder->getSize();
            leftThickness += theBorder->getSize();
        }
        else
        {
            this->_increaseThickness(segmentsObj, theBorder->getSize(), topThickness, rightThickness, bottomThickness, leftThickness);
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
    //QPainterPath outerPath;
    //outerPath.addRect(0.5, 0.5, width+borderBleeding*2-1, height+borderBleeding*2-1);
    //painter.strokePath(outerPath, QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

    HSSUnit topCumulative = 0., rightCumulative = 0., bottomCumulative = 0., leftCumulative = 0.;
    Q_FOREACH(const QSharedPointer<HSSBorder> & theBorder, center)
    {
        HSSUnit theSize = theBorder->getSize();

        QPainterPath path;
        QSharedPointer<HSSObject> segmentsObj = theBorder->getSegments();
        bool hasAll = false, hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);
        if(hasSegments && !hasAll){
            const QSharedPointer<HSSObject> & segmentsObj = theBorder->getSegments();
            this->_drawCenteredBorderBySegments(path, segmentsObj, theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        } else {
            HSSUnit leftOffset = (leftThickness / 2) - leftCumulative - (theSize / 2) + leftCorrection;
            HSSUnit topOffset = (topThickness / 2) - topCumulative - (theSize / 2) + topCorrection;
            HSSUnit rightOffset = (rightThickness / 2) - rightCumulative - (theSize / 2) + rightCorrection;
            HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative - (theSize / 2) + bottomCorrection;

            path.addRect(offsetX + leftOffset, offsetY + topOffset, width-rightOffset-leftOffset, height-bottomOffset-topOffset);

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }
        theBorder->draw(painter, path);
    }

    topCumulative = rightCumulative = bottomCumulative = leftCumulative = 0.;
    QList<QSharedPointer<HSSBorder> >::const_iterator insideIt = inside.constEnd();
    while (insideIt != inside.constBegin())
    {
        --insideIt;
        const QSharedPointer<HSSBorder> & theBorder = *insideIt;
        HSSUnit theSize = theBorder->getSize();

        QPainterPath path;

        QSharedPointer<HSSObject> segmentsObj = theBorder->getSegments();
        bool hasAll = false, hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);

        if(hasSegments && !hasAll){
            const QSharedPointer<HSSObject> & segmentsObj = theBorder->getSegments();
            this->_drawInsideBorderBySegments(path, segmentsObj, theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        } else {
            HSSUnit leftOffset = (leftThickness / 2) + leftCumulative + (theSize / 2) + leftCorrection;
            HSSUnit topOffset = (topThickness / 2) + topCumulative + (theSize / 2) + topCorrection;
            HSSUnit rightOffset = (rightThickness / 2) + rightCumulative + (theSize / 2) + rightCorrection;
            HSSUnit bottomOffset = (bottomThickness / 2) + bottomCumulative + (theSize / 2) + bottomCorrection;

            path.addRect(offsetX + leftOffset, offsetY + topOffset, width-rightOffset-leftOffset, height-bottomOffset-topOffset);

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }

        theBorder->draw(painter, path);
    }

    topCumulative = rightCumulative = bottomCumulative = leftCumulative = 0.;
    Q_FOREACH(const QSharedPointer<HSSBorder> & theBorder, outside)
    {
        HSSUnit theSize = theBorder->getSize();

        QPainterPath path;
        QSharedPointer<HSSObject> segmentsObj = theBorder->getSegments();
        bool hasAll = false, hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);

        if(hasSegments && !hasAll){
            const QSharedPointer<HSSObject> & segmentsObj = theBorder->getSegments();
            this->_drawOutsideBorderBySegments(path, segmentsObj, theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        } else {
            HSSUnit leftOffset = (leftThickness / 2) + leftCumulative + (theSize / 2) + leftCorrection;
            HSSUnit topOffset = (topThickness / 2) + topCumulative + (theSize / 2) + topCorrection;
            HSSUnit rightOffset = (rightThickness / 2) + rightCumulative + (theSize / 2) + rightCorrection;
            HSSUnit bottomOffset = (bottomThickness / 2) + bottomCumulative + (theSize / 2) + bottomCorrection;

            path.addRect(offsetX - leftOffset, offsetY - topOffset, width+rightOffset+leftOffset, height+bottomOffset+topOffset);

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }

        theBorder->draw(painter, path);
    }
}

void HSSRectangle::_increaseThickness(const QSharedPointer<HSSObject> & segmentsObj, const HSSUnit size, HSSUnit & top, HSSUnit & right, HSSUnit & bottom, HSSUnit & left) const
{
    if (segmentsObj->isA(HSSObjectTypeValue))
    {
        this->__increaseThickness(qSharedPointerCast<HSSValue>(segmentsObj)->getValue(), size, top, right, bottom, left);
    }
    else if (segmentsObj->isA(HSSObjectTypeMultipleValue))
    {
        Q_FOREACH(const QSharedPointer<HSSObject> & theObj, qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues())
        {
            if (theObj->isA(HSSObjectTypeValue))
            {
                this->__increaseThickness(qSharedPointerCast<HSSValue>(theObj)->getValue(), size, top, right, bottom, left);
            }
        }
    }
}

void HSSRectangle::__increaseThickness(const QSharedPointer<HSSParserNode> & parserNode, const HSSUnit size, HSSUnit & top, HSSUnit & right, HSSUnit & bottom, HSSUnit & left) const
{
    if (parserNode->isA(HSSParserNodeTypeKeywordConstant)) {
        AXRString segment = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
        if(segment == "top"){
            top += size;
        }
        else if(segment == "right")
        {
            right += size;
        }
        else if(segment == "bottom")
        {
            bottom += size;
        }
        else if(segment == "left")
        {
            left += size;
        }
    }
}

void HSSRectangle::_hasAll(const QSharedPointer<HSSObject> & segmentsObj, bool & hasAll, bool & hasSegments) const
{
    if (segmentsObj && segmentsObj->isA(HSSObjectTypeValue))
    {
        hasSegments = true;
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(segmentsObj)->getValue();
        if (parserNode && parserNode->isA(HSSParserNodeTypeKeywordConstant) && qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "all")
        {
            hasAll = true;
        }
    }
    else if (segmentsObj && segmentsObj->isA(HSSObjectTypeMultipleValue))
    {
        Q_FOREACH(const QSharedPointer<HSSObject> & theObj, qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues())
        {
            if (theObj->isA(HSSObjectTypeValue))
            {
                hasSegments = true;
                QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
                if (parserNode && parserNode->isA(HSSParserNodeTypeKeywordConstant) && qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "all")
                {
                    hasAll = true;
                    break;
                }
            }
        }
    }
}

void HSSRectangle::_drawCenteredBorderBySegments(
    QPainterPath & path,
    const QSharedPointer<HSSObject> & segmentsObj,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & topThickness,
    HSSUnit & topCumulative,
    HSSUnit & topCorrection,
    HSSUnit & rightThickness,
    HSSUnit & rightCumulative,
    HSSUnit & rightCorrection,
    HSSUnit & bottomThickness,
    HSSUnit & bottomCumulative,
    HSSUnit & bottomCorrection,
    HSSUnit & leftThickness,
    HSSUnit & leftCumulative,
    HSSUnit & leftCorrection
) const
{
    if (segmentsObj->isA(HSSObjectTypeValue))
    {
        const QSharedPointer<HSSParserNode> & parserNode = qSharedPointerCast<HSSValue>(segmentsObj)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            this->__drawCenteredBorderBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        }
    }
    else if (segmentsObj->isA(HSSObjectTypeMultipleValue))
    {
        Q_FOREACH(const QSharedPointer<HSSObject> & theObj, qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues())
        {
            if (theObj->isA(HSSObjectTypeValue))
            {
                const QSharedPointer<HSSParserNode> & parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
                if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
                {
                    this->__drawCenteredBorderBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
                }
            }
        }
    }
}

void HSSRectangle::__drawCenteredBorderBySegments(
    QPainterPath & path,
    const AXRString & segment,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & topThickness,
    HSSUnit & topCumulative,
    HSSUnit & topCorrection,
    HSSUnit & rightThickness,
    HSSUnit & rightCumulative,
    HSSUnit & rightCorrection,
    HSSUnit & bottomThickness,
    HSSUnit & bottomCumulative,
    HSSUnit & bottomCorrection,
    HSSUnit & leftThickness,
    HSSUnit & leftCumulative,
    HSSUnit & leftCorrection
) const
{
    if (segment == "top")
    {
        HSSUnit leftOffset = (leftThickness / 2) - leftCumulative + leftCorrection;
        HSSUnit topOffset = (topThickness / 2) - topCumulative - (theSize / 2) + topCorrection;
        HSSUnit rightOffset = (rightThickness / 2) - rightCumulative + rightCorrection;

        path.moveTo(offsetX+leftOffset, offsetY+topOffset);
        path.lineTo(offsetX+width-rightOffset, offsetY+topOffset);
        topCumulative += theSize;
    }
    else if (segment == "right")
    {
        HSSUnit topOffset = (topThickness / 2) - topCumulative + topCorrection;
        HSSUnit rightOffset = (rightThickness / 2) - rightCumulative - (theSize / 2) + rightCorrection;
        HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative + bottomCorrection;

        path.moveTo(offsetX+width-rightOffset, offsetY+topOffset);
        path.lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
        rightCumulative += theSize;
    }
    else if (segment == "bottom")
    {
        HSSUnit leftOffset = (leftThickness / 2) - leftCumulative + leftCorrection;
        HSSUnit rightOffset = (rightThickness / 2) - rightCumulative + rightCorrection;
        HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative - (theSize / 2) + bottomCorrection;

        path.moveTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
        path.lineTo(offsetX+leftOffset, offsetY+height-bottomOffset);
        bottomCumulative += theSize;
    }
    else if(segment == "left")
    {
        HSSUnit leftOffset = (leftThickness / 2) - leftCumulative - (theSize / 2) + leftCorrection;
        HSSUnit topOffset = (topThickness / 2) - topCumulative + topCorrection;
        HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative + bottomCorrection;

        path.moveTo(offsetX+leftOffset, offsetY+height-bottomOffset);
        path.lineTo(offsetX+leftOffset, offsetY+topOffset);
        leftCumulative += theSize;
    }
}

void HSSRectangle::_drawInsideBorderBySegments(
    QPainterPath & path,
    const QSharedPointer<HSSObject> & segmentsObj,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & topThickness,
    HSSUnit & topCumulative,
    HSSUnit & topCorrection,
    HSSUnit & rightThickness,
    HSSUnit & rightCumulative,
    HSSUnit & rightCorrection,
    HSSUnit & bottomThickness,
    HSSUnit & bottomCumulative,
    HSSUnit & bottomCorrection,
    HSSUnit & leftThickness,
    HSSUnit & leftCumulative,
    HSSUnit & leftCorrection
) const
{
    if (segmentsObj->isA(HSSObjectTypeValue))
    {
        const QSharedPointer<HSSParserNode> & parserNode = qSharedPointerCast<HSSValue>(segmentsObj)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            this->__drawInsideBorderBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        }
    }
    else if (segmentsObj->isA(HSSObjectTypeMultipleValue))
    {
        Q_FOREACH(const QSharedPointer<HSSObject> & theObj, qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues())
        {
            if (theObj->isA(HSSObjectTypeValue))
            {
                const QSharedPointer<HSSParserNode> & parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
                if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
                {
                    this->__drawInsideBorderBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
                }
            }
        }
    }
}

void HSSRectangle::__drawInsideBorderBySegments(
    QPainterPath & path,
    const AXRString & segment,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & topThickness,
    HSSUnit & topCumulative,
    HSSUnit & topCorrection,
    HSSUnit & rightThickness,
    HSSUnit & rightCumulative,
    HSSUnit & rightCorrection,
    HSSUnit & bottomThickness,
    HSSUnit & bottomCumulative,
    HSSUnit & bottomCorrection,
    HSSUnit & leftThickness,
    HSSUnit & leftCumulative,
    HSSUnit & leftCorrection
) const
{
    HSSUnit leftOffset = (leftThickness / 2) + leftCumulative + leftCorrection;
    HSSUnit topOffset = (topThickness / 2) + topCumulative + topCorrection;
    HSSUnit rightOffset = (rightThickness / 2) + rightCumulative + rightCorrection;
    HSSUnit bottomOffset = (bottomThickness / 2) + bottomCumulative + bottomCorrection;

    if (segment == "top")
    {
        path.moveTo(offsetX+leftOffset, offsetY+topOffset+(theSize/2));
        path.lineTo(offsetX+width-rightOffset, offsetY+topOffset+(theSize/2));
        topCumulative += theSize;
    }
    else if (segment == "right")
    {
        qreal rightValue = offsetX+width-rightOffset-(theSize/2);
        path.moveTo(rightValue, offsetY+topOffset);
        path.lineTo(rightValue, offsetY+height-bottomOffset);
        rightCumulative += theSize;
    }
    else if (segment == "bottom")
    {
        path.moveTo(offsetX+width-rightOffset, offsetY+height-(bottomOffset+(theSize/2)));
        path.lineTo(offsetX+leftOffset, offsetY+height-(bottomOffset+(theSize/2)));
        bottomCumulative += theSize;
    }
    else if(segment == "left")
    {
        path.moveTo(offsetX+leftOffset+(theSize/2), offsetY+height-bottomOffset);
        path.lineTo(offsetX+leftOffset+(theSize/2), offsetY+topOffset);
        leftCumulative += theSize;
    }
}

void HSSRectangle::_drawOutsideBorderBySegments(
    QPainterPath & path,
    const QSharedPointer<HSSObject> & segmentsObj,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & topThickness,
    HSSUnit & topCumulative,
    HSSUnit & topCorrection,
    HSSUnit & rightThickness,
    HSSUnit & rightCumulative,
    HSSUnit & rightCorrection,
    HSSUnit & bottomThickness,
    HSSUnit & bottomCumulative,
    HSSUnit & bottomCorrection,
    HSSUnit & leftThickness,
    HSSUnit & leftCumulative,
    HSSUnit & leftCorrection
) const
{
    if (segmentsObj->isA(HSSObjectTypeValue))
    {
        const QSharedPointer<HSSParserNode> & parserNode = qSharedPointerCast<HSSValue>(segmentsObj)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            this->__drawOutsideBorderBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        }
    }
    else if (segmentsObj->isA(HSSObjectTypeMultipleValue))
    {
        Q_FOREACH(const QSharedPointer<HSSObject> & theObj, qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues())
        {
            if (theObj->isA(HSSObjectTypeValue))
            {
                const QSharedPointer<HSSParserNode> & parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
                if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
                {
                    this->__drawOutsideBorderBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
                }
            }
        }
    }
}

void HSSRectangle::__drawOutsideBorderBySegments(
    QPainterPath & path,
    const AXRString & segment,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & topThickness,
    HSSUnit & topCumulative,
    HSSUnit & topCorrection,
    HSSUnit & rightThickness,
    HSSUnit & rightCumulative,
    HSSUnit & rightCorrection,
    HSSUnit & bottomThickness,
    HSSUnit & bottomCumulative,
    HSSUnit & bottomCorrection,
    HSSUnit & leftThickness,
    HSSUnit & leftCumulative,
    HSSUnit & leftCorrection
) const
{
    HSSUnit leftOffset = (leftThickness / 2) + leftCumulative + leftCorrection;
    HSSUnit topOffset = (topThickness / 2) + topCumulative + topCorrection;
    HSSUnit rightOffset = (rightThickness / 2) + rightCumulative + rightCorrection;
    HSSUnit bottomOffset = (bottomThickness / 2) + bottomCumulative + bottomCorrection;

    if (segment == "top")
    {
        path.moveTo(offsetX-leftOffset, offsetY-(topOffset+(theSize/2)));
        path.lineTo(offsetX+width+rightOffset, offsetY-(topOffset+(theSize/2)));
        topCumulative += theSize;
    }
    else if (segment == "right")
    {
        path.moveTo(offsetX+width+(rightOffset+(theSize/2)), offsetY-topOffset);
        path.lineTo(offsetX+width+(rightOffset+(theSize/2)), offsetY+height+bottomOffset);
        rightCumulative += theSize;
    }
    else if (segment == "bottom")
    {
        path.moveTo(offsetX+width+rightOffset, offsetY+height+(bottomOffset+(theSize/2)));
        path.lineTo(offsetX-leftOffset, offsetY+height+(bottomOffset+(theSize/2)));
        bottomCumulative += theSize;
    }
    else if(segment == "left")
    {
        path.moveTo(offsetX-(leftOffset+(theSize/2)), offsetY+height+bottomOffset);
        path.lineTo(offsetX-(leftOffset+(theSize/2)), offsetY-topOffset);
        leftCumulative += theSize;
    }
}
