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
#include "HSSRoundedRect.h"

using namespace AXR;

HSSRoundedRect::HSSRoundedRect(AXRController * controller)
: HSSShape(HSSShapeTypeRoundedRect, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSRoundedRect: creating rounded rectangle object");
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("corners");
    this->setShorthandProperties(shorthandProperties);
}

HSSRoundedRect::HSSRoundedRect(const HSSRoundedRect & orig)
: HSSShape(orig)
{
    this->_initialize();
}

void HSSRoundedRect::_initialize()
{
    this->addCallback("corners", new HSSStackCallback<HSSRoundedRect>(this, &HSSRoundedRect::stackCorners));
    this->addCallback("top", new HSSStackCallback<HSSRoundedRect>(this, &HSSRoundedRect::stackTop));
    this->addCallback("right", new HSSStackCallback<HSSRoundedRect>(this, &HSSRoundedRect::stackRight));
    this->addCallback("bottom", new HSSStackCallback<HSSRoundedRect>(this, &HSSRoundedRect::stackBottom));
    this->addCallback("left", new HSSStackCallback<HSSRoundedRect>(this, &HSSRoundedRect::stackLeft));
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
    this->cleanTrackedObservers();
}

void HSSRoundedRect::setDefaults()
{
    this->setDefault("leftTop", 0.);
    this->setDefault("rightTop", 0.);
    this->setDefault("rightBottom", 0.);
    this->setDefault("leftBottom", 0.);
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

void HSSRoundedRect::stackCorners(QSharedPointer<HSSParserNode> parserNode)
{
    this->stackLeft(parserNode);
    this->stackRight(parserNode);
}
void HSSRoundedRect::stackTop(QSharedPointer<HSSParserNode> parserNode)
{
    this->setStackNode("leftTop", parserNode);
    this->setStackNode("rightTop", parserNode);
}
void HSSRoundedRect::stackRight(QSharedPointer<HSSParserNode> parserNode)
{
    this->setStackNode("rightTop", parserNode);
    this->setStackNode("rightBottom", parserNode);
}
void HSSRoundedRect::stackBottom(QSharedPointer<HSSParserNode> parserNode)
{
    this->setStackNode("leftBottom", parserNode);
    this->setStackNode("rightBottom", parserNode);
}
void HSSRoundedRect::stackLeft(QSharedPointer<HSSParserNode> parserNode)
{
    this->setStackNode("leftTop", parserNode);
    this->setStackNode("leftBottom", parserNode);
}

void HSSRoundedRect::createPath(QSharedPointer<HSSPath> &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::list<QSharedPointer<HSSParserNode> > segments)
{
    this->createRoundedRect(path, x, y, width, height, 0.);
}

HSSUnit HSSRoundedRect::getLeftTop() const
{
    return this->getComputedNumber("leftTop");
}

HSSUnit HSSRoundedRect::getLeftBottom() const
{
    return this->getComputedNumber("leftBottom");
}

HSSUnit HSSRoundedRect::getRightTop() const
{
    return this->getComputedNumber("rightTop");
}

HSSUnit HSSRoundedRect::getRightBottom() const
{
    return this->getComputedNumber("rightBottom");
}

void HSSRoundedRect::createRoundedRect(QSharedPointer<HSSPath> &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, HSSUnit offset)
{
    HSSUnit cornerTL = this->getLeftTop();
    HSSUnit cornerBL = this->getLeftBottom();
    HSSUnit cornerBR = this->getRightBottom();
    HSSUnit cornerTR = this->getRightTop();

    HSSUnit topLeftOffset = (cornerTL*2+offset);
    if(topLeftOffset < 1) topLeftOffset = 1;
    HSSUnit bottomLeftOffset = (cornerBL*2+offset);
    if(bottomLeftOffset < 1) bottomLeftOffset = 1;
    HSSUnit bottomRightOffset = (cornerBR*2+offset);
    if(bottomRightOffset < 1) bottomRightOffset = 1;
    HSSUnit topRightOffset = (cornerTR*2+offset);
    if(topRightOffset < 1) topRightOffset = 1;

    HSSRect topLeftBounds(x, y, topLeftOffset, topLeftOffset);
    HSSRect bottomLeftBounds(x, y + height - bottomLeftOffset, bottomLeftOffset, bottomLeftOffset);
    HSSRect bottomRightBounds(x + width - bottomRightOffset, y + height - bottomRightOffset, bottomRightOffset, bottomRightOffset);
    HSSRect topRightBounds(x + width - topRightOffset, y, topRightOffset, topRightOffset);

    std::vector<HSSRect> corners;
    corners.push_back(topLeftBounds);
    corners.push_back(bottomLeftBounds);
    corners.push_back(bottomRightBounds);
    corners.push_back(topRightBounds);

    path->moveTo(topLeftBounds.origin.x + topLeftBounds.size.width/2, topLeftBounds.origin.y);

    for (int i = 1; i <= corners.size(); ++i)
    {
        path->arcTo(corners[i - 1], i * 90, 90);
    }

    path->closeSubpath();
}

void HSSRoundedRect::drawStrokes(std::list<QSharedPointer<HSSAbstractStroke> > strokes, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY)
{
    HSSUnit cornerTL = this->getLeftTop();
    HSSUnit cornerBL = this->getLeftBottom();
    HSSUnit cornerBR = this->getRightBottom();
    HSSUnit cornerTR = this->getRightTop();

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
        QSharedPointer<HSSObject> segmentsObj = theStroke->getSegments();
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
//    QPainterPath outerPath;
//    outerPath.addRect(0.5, 0.5, width+strokeBleeding*2-1, height+strokeBleeding*2-1);
//    painter.strokePath(outerPath, QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap, Qt::MiterJoin));

    HSSUnit topCumulative = 0., rightCumulative = 0., bottomCumulative = 0., leftCumulative = 0.;
    for (it = center.begin(); it != center.end(); ++it)
    {
        const QSharedPointer<HSSAbstractStroke> & theStroke = *it;
        HSSUnit theSize = theStroke->getSize();

        QSharedPointer<HSSObject> segmentsObj = theStroke->getSegments();
        bool hasAll = false;
        bool hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);
        if(hasSegments && !hasAll){
            QSharedPointer<HSSPath> path(new HSSPath());
            this->_drawCenteredStrokeBySegments(path, segmentsObj, theSize, width, height, offsetX, offsetY, cornerTL, cornerBL, cornerBR, cornerTR, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        } else {
            ///@todo uncomment this once there are stroke types
            //if(theStroke->isA(HSSStrokeTypeLineStroke){
                QSharedPointer<HSSPath> path(new HSSPath());
                QSharedPointer<HSSStroke> theLineStroke = qSharedPointerCast<HSSStroke>(theStroke);
                QSharedPointer<HSSObject> theColorObj = theLineStroke->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    QSharedPointer<HSSPath> innerPath(new HSSPath());
                    HSSUnit outerLeftOffset = (leftThickness/2)-leftCumulative-theSize+leftCorrection;
                    HSSUnit outerTopOffset = (topThickness/2)-topCumulative-theSize+topCorrection;
                    HSSUnit outerRightOffset = (rightThickness/2)-rightCumulative-theSize+rightCorrection;
                    HSSUnit outerBottomOffset = (bottomThickness/2)-bottomCumulative-theSize+bottomCorrection;
                    this->createRoundedRect(
                                            path,
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
                    path->subtract(innerPath);
                    this->getController()->document()->platform()->fillPath(path, theColor);
                }
            //}

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }
        //theStroke->draw(painter, path);
    }

    topCumulative = rightCumulative = bottomCumulative = leftCumulative = 0.;
    std::list<QSharedPointer<HSSAbstractStroke> >::const_iterator insideIt = inside.end();
    while (insideIt != inside.begin())
    {
        --insideIt;
        const QSharedPointer<HSSAbstractStroke> & theStroke = *insideIt;
        HSSUnit theSize = theStroke->getSize();
        QSharedPointer<HSSObject> segmentsObj = theStroke->getSegments();
        bool hasAll = false;
        bool hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);
        if(hasSegments && !hasAll){
            QSharedPointer<HSSPath> path(new HSSPath());
            ///@todo uncomment this once there are stroke types
            //if(theStroke->isA(HSSStrokeTypeLineStroke){
                QSharedPointer<HSSStroke> theLineStroke = qSharedPointerCast<HSSStroke>(theStroke);
                QSharedPointer<HSSObject> theColorObj = theLineStroke->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    this->_drawInsideStrokeBySegments(path, segmentsObj, theSize, width, height, offsetX, offsetY, cornerTL, cornerBL, cornerBR, cornerTR, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);

                    QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    this->getController()->document()->platform()->fillPath(path, theColor);
                }
            //}
        } else {
            ///@todo uncomment this once there are stroke types
            //if(theStroke->isA(HSSStrokeTypeLineStroke){
                QSharedPointer<HSSPath> path(new HSSPath());
                QSharedPointer<HSSStroke> theLineStroke = qSharedPointerCast<HSSStroke>(theStroke);
                QSharedPointer<HSSObject> theColorObj = theLineStroke->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    QSharedPointer<HSSPath> innerPath(new HSSPath());
                    HSSUnit outerLeftOffset = leftCumulative+leftCorrection;
                    HSSUnit outerTopOffset = topCumulative+topCorrection;
                    HSSUnit outerRightOffset = rightCumulative+rightCorrection;
                    HSSUnit outerBottomOffset = bottomCumulative+bottomCorrection;
                    this->createRoundedRect(
                                            path,
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
                    path->subtract(innerPath);
                    this->getController()->document()->platform()->fillPath(path, theColor);
                }
            //}

            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }
    }

    topCumulative = rightCumulative = bottomCumulative = leftCumulative = 0.;
    for (it = outside.begin(); it != outside.end(); ++it)
    {
        const QSharedPointer<HSSAbstractStroke> & theStroke = *it;
        HSSUnit theSize = theStroke->getSize();
        QSharedPointer<HSSObject> segmentsObj = theStroke->getSegments();
        bool hasAll = false;
        bool hasSegments = false;
        this->_hasAll(segmentsObj, hasAll, hasSegments);
        if(hasSegments && !hasAll){
            QSharedPointer<HSSPath> path(new HSSPath());
            this->_drawOutsideStrokeBySegments(path, segmentsObj, theSize, width, height, offsetX, offsetY, cornerTL, cornerBL, cornerBR, cornerTR, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
        } else {
            ///@todo uncomment this once there are stroke types
            //if(theStroke->isA(HSSStrokeTypeLineStroke){
                QSharedPointer<HSSPath> path(new HSSPath());
                QSharedPointer<HSSStroke> theLineStroke = qSharedPointerCast<HSSStroke>(theStroke);
                QSharedPointer<HSSObject> theColorObj = theLineStroke->getColor();
                if(theColorObj && theColorObj->isA(HSSObjectTypeRgb)){
                    QSharedPointer<HSSRgb> theColor = qSharedPointerCast<HSSRgb>(theColorObj);
                    QSharedPointer<HSSPath> innerPath(new HSSPath());
                    HSSUnit outerLeftOffset = theSize+leftCumulative+leftCorrection;
                    HSSUnit outerTopOffset = theSize+topCumulative+topCorrection;
                    HSSUnit outerRightOffset = theSize+rightCumulative+rightCorrection;
                    HSSUnit outerBottomOffset = theSize+bottomCumulative+bottomCorrection;
                    this->createRoundedRect(
                                            path,
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
                    path->subtract(innerPath);
                    this->getController()->document()->platform()->fillPath(path, theColor);
                }
            //}
            leftCumulative += theSize;
            topCumulative += theSize;
            rightCumulative += theSize;
            bottomCumulative += theSize;
        }
    }
}

void HSSRoundedRect::_increaseThickness(const QSharedPointer<HSSObject> & segmentsObj, const HSSUnit size, HSSUnit & top, HSSUnit & right, HSSUnit & bottom, HSSUnit & left) const
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

void HSSRoundedRect::__increaseThickness(const QSharedPointer<HSSParserNode> & parserNode, const HSSUnit size, HSSUnit & top, HSSUnit & right, HSSUnit & bottom, HSSUnit & left) const
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

void HSSRoundedRect::_hasAll(const QSharedPointer<HSSObject> & segmentsObj, bool & hasAll, bool & hasSegments) const
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

void HSSRoundedRect::_drawCenteredStrokeBySegments(
    QSharedPointer<HSSPath> & path,
    const QSharedPointer<HSSObject> & segmentsObj,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & cornerTL,
    HSSUnit & cornerBL,
    HSSUnit & cornerBR,
    HSSUnit & cornerTR,
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
            this->__drawCenteredStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, cornerTL, cornerBL, cornerBR, cornerTR, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
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
                    this->__drawCenteredStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, cornerTL, cornerBL, cornerBR, cornerTR, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
                }
            }
        }
    }
}

void HSSRoundedRect::__drawCenteredStrokeBySegments(
    QSharedPointer<HSSPath> & path,
    const AXRString & segment,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & cornerTL,
    HSSUnit & cornerBL,
    HSSUnit & cornerBR,
    HSSUnit & cornerTR,
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
///@todo this doesn't seem right
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

void HSSRoundedRect::_drawInsideStrokeBySegments(
    QSharedPointer<HSSPath> & path,
    const QSharedPointer<HSSObject> & segmentsObj,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & cornerTL,
    HSSUnit & cornerBL,
    HSSUnit & cornerBR,
    HSSUnit & cornerTR,
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
            this->__drawInsideStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, cornerTL, cornerBL, cornerBR, cornerTR, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
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
                    this->__drawInsideStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, cornerTL, cornerBL, cornerBR, cornerTR, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
                }
            }
        }
    }
}

void HSSRoundedRect::__drawInsideStrokeBySegments(
    QSharedPointer<HSSPath> & path,
    const AXRString & segment,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & cornerTL,
    HSSUnit & cornerBL,
    HSSUnit & cornerBR,
    HSSUnit & cornerTR,
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
        HSSUnit leftOffset = leftCumulative+leftCorrection;
        HSSUnit topOffset = topCumulative+topCorrection;
        HSSUnit rightOffset = rightCumulative+rightCorrection+(cornerTR*2);

        HSSRect curve1(offsetX+leftOffset, offsetY+topOffset, cornerTL*2, cornerTL*2);
        if (cornerTL != 0)
        {
            path->moveTo(curve1.origin.x, curve1.origin.y);
            path->arcTo(curve1, 90, 90);
        }
        else
        {
            path->moveTo(offsetX+leftOffset, offsetY+topOffset);
            path->arcTo(curve1, 90, 90);
        }
        if(cornerTL != 0)
        {
            HSSRect curve2(offsetX+leftOffset, offsetY+topOffset+theSize, cornerTL*2, (cornerTL*2)-theSize);
            path->arcTo(curve2, 180, -90);
        }
        else
        {
            path->lineTo(offsetX+leftOffset, offsetY+topOffset+theSize);
        }
        qreal trsize = (cornerTR*2)-theSize;
        if(trsize > 0)
        {
            HSSRect curve3(offsetX+width-rightOffset, offsetY+topOffset+theSize, cornerTR*2, (cornerTR*2)-theSize);
            path->arcTo(curve3, 90, -90);
        }
        else
        {
            path->lineTo(offsetX+width-rightOffset, offsetY+topOffset+theSize);
        }
        if (cornerTR != 0) {
            HSSRect curve4(offsetX+width-rightOffset, offsetY+topOffset, cornerTR*2, cornerTR*2);
            path->arcTo(curve4, 0, 90);
        }
        else
        {
            path->lineTo(offsetX+width-rightOffset, offsetY+topOffset);
        }
        path->closeSubpath();

        topCumulative += theSize;
    }
    else if (segment == "right")
    {
        HSSUnit topOffset = topCumulative+topCorrection;
        HSSUnit rightOffset = rightCumulative+rightCorrection;
        HSSUnit bottomOffset = bottomCumulative+bottomCorrection;

        if (cornerTR != 0)
        {
            HSSRect curve1(offsetX+width-rightOffset-(cornerTR*2), offsetY+topOffset, cornerTR*2, cornerTR*2);
            path->moveTo(curve1.origin.x, curve1.origin.y);
            path->arcTo(curve1, 90, -90);
        }
        else
        {
            path->moveTo(offsetX+width-rightOffset, offsetY+topOffset);
            path->lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset-cornerBR);
        }
        if (cornerBR != 0)
        {
            HSSRect curve2(offsetX+width-rightOffset-(cornerBR*2), offsetY+height-(cornerBR*2)-bottomOffset, cornerBR*2, (cornerBR*2));
            path->arcTo(curve2, 0, -90);
        }
        else
        {
            path->lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
        }
        qreal brsize = (cornerBR*2)-theSize;
        if (brsize > 0)
        {
            HSSRect curve3(offsetX+width-(cornerBR*2)-rightOffset, offsetY+height-(cornerBR*2)-bottomOffset, brsize, cornerBR*2);
            path->arcTo(curve3, 270, 90);
        }
        else
        {
            path->lineTo(offsetX+width-rightOffset-theSize, offsetY+height-bottomOffset);
        }
        qreal trsize = (cornerTR*2)-theSize;
        if (trsize > 0)
        {
            HSSRect curve4(offsetX+width-(cornerTR*2)-rightOffset, offsetY+topOffset, trsize, cornerTR*2);
            path->arcTo(curve4, 0, 90);
        }
        else
        {
            path->lineTo(offsetX+width-rightOffset-theSize, offsetY+topOffset);
        }
        path->closeSubpath();
        rightCumulative += theSize;
    }
    else if (segment == "bottom")
    {
        HSSUnit rightOffset = rightCumulative+rightCorrection;
        HSSUnit bottomOffset = bottomCumulative+bottomCorrection;
        HSSUnit leftOffset = leftCumulative+leftCorrection;

        if(cornerBL != 0)
        {
            HSSRect curve1(offsetX+leftOffset, offsetY+height-(cornerBL*2)-bottomOffset, cornerBL*2, cornerBL*2);
            path->moveTo(curve1.origin.x, curve1.origin.y + curve1.size.height);
            path->arcTo(curve1, 180, 90);
        }
        else
        {
            path->moveTo(offsetX+leftOffset, offsetY+height-bottomOffset-theSize);
            path->lineTo(offsetX+leftOffset, offsetY+height-bottomOffset);
        }
        if (cornerBR != 0)
        {
            HSSRect curve2(offsetX+width-(cornerBR*2)-rightOffset, offsetY+height-(cornerBR*2)-bottomOffset, cornerBR*2, cornerBR*2);
            path->arcTo(curve2, 270, 90);
        }
        else
        {
            path->lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset);
        }
        qreal brsize = (cornerBR*2)-theSize;
        if (brsize > 0)
        {
            HSSRect curve3(offsetX+width-(cornerBR*2)-rightOffset, offsetY+height-(cornerBR*2)-bottomOffset, cornerBR*2, brsize);
            path->arcTo(curve3, 0, -90);
        }
        else
        {
            path->lineTo(offsetX+width-rightOffset, offsetY+height-bottomOffset-theSize);
        }
        qreal blsize = (cornerBL*2)-theSize;
        if (blsize > 0)
        {
            HSSRect curve4(offsetX+leftOffset, offsetY+height-(cornerBL*2)-bottomOffset, cornerBL*2, blsize);
            path->arcTo(curve4, 270, -90);
        }
        else
        {
            path->lineTo(offsetX+leftOffset, offsetY+height-bottomOffset-theSize);
        }
        path->closeSubpath();
        bottomCumulative += theSize;
    }
    else if(segment == "left")
    {
        HSSUnit bottomOffset = bottomCumulative+bottomCorrection;
        HSSUnit leftOffset = leftCumulative+leftCorrection;
        HSSUnit topOffset = topCumulative+topCorrection;

        HSSRect curve1(offsetX+leftOffset, offsetY+topOffset, cornerTL*2, cornerTL*2);
        if(cornerTL != 0){
            path->moveTo(curve1.origin.x, curve1.origin.y);
            path->arcTo(curve1, 90, 90);
        } else {
            path->moveTo(offsetX+leftOffset, offsetY+topOffset);
            path->arcTo(curve1, 90, 90);
        }
        if(cornerBL != 0){
            HSSRect curve2(offsetX+leftOffset, offsetY+height-(cornerBL*2)-bottomOffset, cornerBL*2, cornerBL*2);
            path->arcTo(curve2, 180, 90);
        } else {
            path->lineTo(offsetX+leftOffset, offsetY+height-bottomOffset);
        }

        if((cornerBL*2)-theSize > 0){
            HSSRect curve3(offsetX+leftOffset+theSize, offsetY+height-(cornerBL*2)-bottomOffset, (cornerBL*2)-theSize, (cornerBL*2));
            path->arcTo(curve3, 270, -90);
        } else {
            path->lineTo(offsetX+leftOffset+theSize, offsetY+height-bottomOffset);
        }

        if(cornerTL != 0){
            HSSRect curve4(offsetX+leftOffset+theSize, offsetY+topOffset, (cornerTL*2)-theSize, cornerTL*2);
            path->arcTo(curve4, 180, -90);
        } else {
            path->lineTo(offsetX+leftOffset+theSize, offsetY+topOffset);
        }

        path->closeSubpath();
        leftCumulative += theSize;
    }
}

void HSSRoundedRect::_drawOutsideStrokeBySegments(
    QSharedPointer<HSSPath> & path,
    const QSharedPointer<HSSObject> & segmentsObj,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & cornerTL,
    HSSUnit & cornerBL,
    HSSUnit & cornerBR,
    HSSUnit & cornerTR,
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
            this->__drawOutsideStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, cornerTL, cornerBL, cornerBR, cornerTR, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
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
                    this->__drawOutsideStrokeBySegments(path, qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue(), theSize, width, height, offsetX, offsetY, cornerTL, cornerBL, cornerBR, cornerTR, topThickness, topCumulative, topCorrection, rightThickness, rightCumulative, rightCorrection, bottomThickness, bottomCumulative, bottomCorrection, leftThickness, leftCumulative, leftCorrection);
                }
            }
        }
    }
}

void HSSRoundedRect::__drawOutsideStrokeBySegments(
    QSharedPointer<HSSPath> & path,
    const AXRString & segment,
    HSSUnit & theSize,
    HSSUnit & width,
    HSSUnit & height,
    HSSUnit & offsetX,
    HSSUnit & offsetY,
    HSSUnit & cornerTL,
    HSSUnit & cornerBL,
    HSSUnit & cornerBR,
    HSSUnit & cornerTR,
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
///@todo this doesn't seem right
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
