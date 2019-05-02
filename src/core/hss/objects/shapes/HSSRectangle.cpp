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

#include "axr.h"
#include "HSSRectangle.h"

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
    this->cleanTrackedObservers();
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

void HSSRectangle::createPath(QSharedPointer<HSSPath> &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::list<QSharedPointer<HSSParserNode> > segments)
{
    if(segments.size() > 0){
        std::list<QSharedPointer<HSSParserNode> >::const_iterator it;
        for (it = segments.begin(); it != segments.end(); ++it)
        {
            const QSharedPointer<HSSParserNode> & segment = *it;
            switch (segment->getType()) {
                case HSSParserNodeTypeKeywordConstant:
                {
                    QSharedPointer<HSSKeywordConstant> theKw = qSharedPointerCast<HSSKeywordConstant>(segment);
                    AXRString theValue = theKw->getValue();
                    if(theValue == "left")
                    {
                        path->moveTo(x, y+height);
                        path->lineTo(x, y);
                    }
                    else if (theValue == "top")
                    {
                        path->moveTo(x, y);
                        path->lineTo(x+width, y);
                    }
                    else if (theValue == "right")
                    {
                        path->moveTo(x+width, y);
                        path->lineTo(x+width, y+height);
                    }
                    else if (theValue == "bottom")
                    {
                        path->moveTo(x+width, y+height);
                        path->lineTo(x, y+height);
                    }
                    break;
                }

                default:
                    break;
            }
        }
    } else {
        path->addRect(x, y, width, height);
    }
}

void HSSRectangle::drawStrokes(std::list<QSharedPointer<HSSAbstractStroke> > strokes, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY)
{
    //sort strokes in three groups
    std::list<QSharedPointer<HSSAbstractStroke> > center, inside, outside;
    std::list<QSharedPointer<HSSAbstractStroke> >::const_iterator it;
    for (it = strokes.begin(); it != strokes.end(); ++it)
    {
        const QSharedPointer<HSSAbstractStroke> & theStroke = *it;
        HSSStrokePosition thePos = theStroke->getPosition();
        if (thePos == HSSStrokePositionCenter)
        {
            center.push_back(theStroke);
        }
        else if (thePos == HSSStrokePositionInside)
        {
            inside.push_back(theStroke);
        }
        else if (thePos == HSSStrokePositionOutside)
        {
            outside.push_back(theStroke);
        }
    }

    HSSUnit topThickness = 0., rightThickness = 0., bottomThickness = 0., leftThickness = 0.;
    for (it = center.begin(); it != center.end(); ++it)
    {
        const QSharedPointer<HSSAbstractStroke> & theStroke = *it;
        const QSharedPointer<HSSObject> & segmentsObj = theStroke->getSegments();
        bool hasAll = false;
        bool hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);
        if(!hasSegments || hasAll){
            topThickness += theStroke->getSize();
            rightThickness += theStroke->getSize();
            bottomThickness += theStroke->getSize();
            leftThickness += theStroke->getSize();
        }
        else
        {
            this->_increaseThickness(segmentsObj, theStroke->getSize(), topThickness, rightThickness, bottomThickness, leftThickness);
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

    // Uncomment this to see the dimensions of the strokes surface
    //QPainterPath outerPath;
    //outerPath.addRect(0.5, 0.5, width+strokeBleeding*2-1, height+strokeBleeding*2-1);
    //painter.strokePath(outerPath, QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

    HSSUnit topCumulative = 0., rightCumulative = 0., bottomCumulative = 0., leftCumulative = 0.;
    for (it = center.begin(); it != center.end(); ++it)
    {
        const QSharedPointer<HSSAbstractStroke> & theStroke = *it;
        HSSUnit theSize = theStroke->getSize();

        QSharedPointer<HSSPath> path(new HSSPath());
        QSharedPointer<HSSObject> segmentsObj = theStroke->getSegments();
        bool hasAll = false, hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);
        if(hasSegments && !hasAll){
            const QSharedPointer<HSSObject> & segmentsObj = theStroke->getSegments();
            this->_drawCenteredStrokeBySegments(path, segmentsObj, theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        } else {
            HSSUnit leftOffset = (leftThickness / 2) - leftCumulative - (theSize / 2) + leftCorrection;
            HSSUnit topOffset = (topThickness / 2) - topCumulative - (theSize / 2) + topCorrection;
            HSSUnit rightOffset = (rightThickness / 2) - rightCumulative - (theSize / 2) + rightCorrection;
            HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative - (theSize / 2) + bottomCorrection;

            path->addRect(offsetX + leftOffset, offsetY + topOffset, width-rightOffset-leftOffset, height-bottomOffset-topOffset);

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }
        this->getController()->document()->platform()->drawStroke(path, theStroke);
    }

    topCumulative = rightCumulative = bottomCumulative = leftCumulative = 0.;
    std::list<QSharedPointer<HSSAbstractStroke> >::const_iterator insideIt = inside.end();
    while (insideIt != inside.begin())
    {
        --insideIt;
        const QSharedPointer<HSSAbstractStroke> & theStroke = *insideIt;
        HSSUnit theSize = theStroke->getSize();

        QSharedPointer<HSSPath> path(new HSSPath());

        QSharedPointer<HSSObject> segmentsObj = theStroke->getSegments();
        bool hasAll = false, hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);

        if(hasSegments && !hasAll){
            const QSharedPointer<HSSObject> & segmentsObj = theStroke->getSegments();
            this->_drawInsideStrokeBySegments(path, segmentsObj, theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        } else {
            HSSUnit leftOffset = (leftThickness / 2) + leftCumulative + (theSize / 2) + leftCorrection;
            HSSUnit topOffset = (topThickness / 2) + topCumulative + (theSize / 2) + topCorrection;
            HSSUnit rightOffset = (rightThickness / 2) + rightCumulative + (theSize / 2) + rightCorrection;
            HSSUnit bottomOffset = (bottomThickness / 2) + bottomCumulative + (theSize / 2) + bottomCorrection;

            path->addRect(offsetX + leftOffset, offsetY + topOffset, width-rightOffset-leftOffset, height-bottomOffset-topOffset);

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }

        this->getController()->document()->platform()->drawStroke(path, theStroke);
    }

    topCumulative = rightCumulative = bottomCumulative = leftCumulative = 0.;
    for (it = outside.begin(); it != outside.end(); ++it)
    {
        const QSharedPointer<HSSAbstractStroke> & theStroke = *it;
        HSSUnit theSize = theStroke->getSize();

        QSharedPointer<HSSPath> path(new HSSPath());
        QSharedPointer<HSSObject> segmentsObj = theStroke->getSegments();
        bool hasAll = false, hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);

        if(hasSegments && !hasAll){
            const QSharedPointer<HSSObject> & segmentsObj = theStroke->getSegments();
            this->_drawOutsideStrokeBySegments(path, segmentsObj, theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        } else {
            HSSUnit leftOffset = (leftThickness / 2) + leftCumulative + (theSize / 2) + leftCorrection;
            HSSUnit topOffset = (topThickness / 2) + topCumulative + (theSize / 2) + topCorrection;
            HSSUnit rightOffset = (rightThickness / 2) + rightCumulative + (theSize / 2) + rightCorrection;
            HSSUnit bottomOffset = (bottomThickness / 2) + bottomCumulative + (theSize / 2) + bottomCorrection;

            path->addRect(offsetX - leftOffset, offsetY - topOffset, width+rightOffset+leftOffset, height+bottomOffset+topOffset);

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }

        this->getController()->document()->platform()->drawStroke(path, theStroke);
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
        std::vector<QSharedPointer<HSSObject> >::const_iterator it;
        const std::vector<QSharedPointer<HSSObject> > & values = qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues();
        for (it = values.begin(); it != values.end(); ++it)
        {
            const QSharedPointer<HSSObject> & theObj = *it;
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
        std::vector<QSharedPointer<HSSObject> >::const_iterator it;
        const std::vector<QSharedPointer<HSSObject> > & values = qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues();
        for (it = values.begin(); it != values.end(); ++it)
        {
            const QSharedPointer<HSSObject> & theObj = *it;
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

void HSSRectangle::_drawCenteredStrokeBySegments(
    QSharedPointer<HSSPath> & path,
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
            this->__drawCenteredStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        }
    }
    else if (segmentsObj->isA(HSSObjectTypeMultipleValue))
    {
        std::vector<QSharedPointer<HSSObject> >::const_iterator it;
        const std::vector<QSharedPointer<HSSObject> > & values = qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues();
        for (it = values.begin(); it != values.end(); ++it)
        {
            const QSharedPointer<HSSObject> & theObj = *it;
                    if (theObj->isA(HSSObjectTypeValue))
            {
                const QSharedPointer<HSSParserNode> & parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
                if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
                {
                    this->__drawCenteredStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
                }
            }
        }
    }
}

void HSSRectangle::__drawCenteredStrokeBySegments(
    QSharedPointer<HSSPath> & path,
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

        path->moveTo(offsetX+leftOffset, offsetY+topOffset);
        path->lineTo(offsetX+width-rightOffset, offsetY+topOffset);
        topCumulative += theSize;
    }
    else if (segment == "right")
    {
        HSSUnit topOffset = (topThickness / 2) - topCumulative + topCorrection;
        HSSUnit rightOffset = (rightThickness / 2) - rightCumulative - (theSize / 2) + rightCorrection;
        HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative + bottomCorrection;

        path->moveTo(offsetX+width-rightOffset, offsetY+topOffset);
        path->lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
        rightCumulative += theSize;
    }
    else if (segment == "bottom")
    {
        HSSUnit leftOffset = (leftThickness / 2) - leftCumulative + leftCorrection;
        HSSUnit rightOffset = (rightThickness / 2) - rightCumulative + rightCorrection;
        HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative - (theSize / 2) + bottomCorrection;

        path->moveTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
        path->lineTo(offsetX+leftOffset, offsetY+height-bottomOffset);
        bottomCumulative += theSize;
    }
    else if(segment == "left")
    {
        HSSUnit leftOffset = (leftThickness / 2) - leftCumulative - (theSize / 2) + leftCorrection;
        HSSUnit topOffset = (topThickness / 2) - topCumulative + topCorrection;
        HSSUnit bottomOffset = (bottomThickness / 2) - bottomCumulative + bottomCorrection;

        path->moveTo(offsetX+leftOffset, offsetY+height-bottomOffset);
        path->lineTo(offsetX+leftOffset, offsetY+topOffset);
        leftCumulative += theSize;
    }
}

void HSSRectangle::_drawInsideStrokeBySegments(
    QSharedPointer<HSSPath> & path,
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
            this->__drawInsideStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        }
    }
    else if (segmentsObj->isA(HSSObjectTypeMultipleValue))
    {
        std::vector<QSharedPointer<HSSObject> >::const_iterator it;
        const std::vector<QSharedPointer<HSSObject> > & values = qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues();
        for (it = values.begin(); it != values.end(); ++it)
        {
            const QSharedPointer<HSSObject> & theObj = *it;
            if (theObj->isA(HSSObjectTypeValue))
            {
                const QSharedPointer<HSSParserNode> & parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
                if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
                {
                    this->__drawInsideStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
                }
            }
        }
    }
}

void HSSRectangle::__drawInsideStrokeBySegments(
    QSharedPointer<HSSPath> & path,
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
        path->moveTo(offsetX+leftOffset, offsetY+topOffset+(theSize/2));
        path->lineTo(offsetX+width-rightOffset, offsetY+topOffset+(theSize/2));
        topCumulative += theSize;
    }
    else if (segment == "right")
    {
        qreal rightValue = offsetX+width-rightOffset-(theSize/2);
        path->moveTo(rightValue, offsetY+topOffset);
        path->lineTo(rightValue, offsetY+height-bottomOffset);
        rightCumulative += theSize;
    }
    else if (segment == "bottom")
    {
        path->moveTo(offsetX+width-rightOffset, offsetY+height-(bottomOffset+(theSize/2)));
        path->lineTo(offsetX+leftOffset, offsetY+height-(bottomOffset+(theSize/2)));
        bottomCumulative += theSize;
    }
    else if(segment == "left")
    {
        path->moveTo(offsetX+leftOffset+(theSize/2), offsetY+height-bottomOffset);
        path->lineTo(offsetX+leftOffset+(theSize/2), offsetY+topOffset);
        leftCumulative += theSize;
    }
}

void HSSRectangle::_drawOutsideStrokeBySegments(
    QSharedPointer<HSSPath> & path,
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
            this->__drawOutsideStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        }
    }
    else if (segmentsObj->isA(HSSObjectTypeMultipleValue))
    {
        std::vector<QSharedPointer<HSSObject> >::const_iterator it;
        const std::vector<QSharedPointer<HSSObject> > & values = qSharedPointerCast<HSSMultipleValue>(segmentsObj)->getValues();
        for (it = values.begin(); it != values.end(); ++it)
        {
            const QSharedPointer<HSSObject> & theObj = *it;
            if (theObj->isA(HSSObjectTypeValue))
            {
                const QSharedPointer<HSSParserNode> & parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
                if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
                {
                    this->__drawOutsideStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
                }
            }
        }
    }
}

void HSSRectangle::__drawOutsideStrokeBySegments(
    QSharedPointer<HSSPath> & path,
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
        path->moveTo(offsetX-leftOffset, offsetY-(topOffset+(theSize/2)));
        path->lineTo(offsetX+width+rightOffset, offsetY-(topOffset+(theSize/2)));
        topCumulative += theSize;
    }
    else if (segment == "right")
    {
        path->moveTo(offsetX+width+(rightOffset+(theSize/2)), offsetY-topOffset);
        path->lineTo(offsetX+width+(rightOffset+(theSize/2)), offsetY+height+bottomOffset);
        rightCumulative += theSize;
    }
    else if (segment == "bottom")
    {
        path->moveTo(offsetX+width+rightOffset, offsetY+height+(bottomOffset+(theSize/2)));
        path->lineTo(offsetX-leftOffset, offsetY+height+(bottomOffset+(theSize/2)));
        bottomCumulative += theSize;
    }
    else if(segment == "left")
    {
        path->moveTo(offsetX-(leftOffset+(theSize/2)), offsetY+height+bottomOffset);
        path->lineTo(offsetX-(leftOffset+(theSize/2)), offsetY-topOffset);
        leftCumulative += theSize;
    }
}
