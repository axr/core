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
#include "HSSCircle.h"

using namespace AXR;

HSSCircle::HSSCircle(AXRController * controller)
: HSSShape(HSSShapeTypeCircle, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSCircle: creating circle object");
}

HSSCircle::HSSCircle(const HSSCircle & orig)
: HSSShape(orig)
{
}

QSharedPointer<HSSCircle> HSSCircle::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSCircle: cloning circle object");
    return qSharedPointerCast<HSSCircle> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSCircle::cloneImpl() const
{
    return QSharedPointer<HSSCircle>(new HSSCircle(*this));
}

HSSCircle::~HSSCircle()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSCircle: destructing circle object");
    this->cleanTrackedObservers();
}

AXRString HSSCircle::toString()
{
    return "HSSCircle";
}

AXRString HSSCircle::defaultObjectType()
{
    return "HSSCircle";
}

AXRString HSSCircle::defaultObjectType(AXRString property)
{
    return HSSShape::defaultObjectType(property);
}

bool HSSCircle::isKeyword(AXRString value, AXRString property)
{
    return HSSShape::isKeyword(value, property);
}

void HSSCircle::createPath(QSharedPointer<HSSPath> &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::list<QSharedPointer<HSSParserNode> > segments)
{
    path->addEllipse(HSSRect(x, y, width, height));
}

void HSSCircle::drawStrokes(std::list<QSharedPointer<HSSAbstractStroke> > strokes, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY)
{
    // Calculate the combined thickness of all strokes
    HSSUnit combinedThickness = 0;
    
    std::list<QSharedPointer<HSSAbstractStroke> >::const_iterator it;
    for (it = strokes.begin(); it != strokes.end(); ++it)
    {
        const QSharedPointer<HSSAbstractStroke> & theStroke = *it;
        combinedThickness += theStroke->getSize();
    }

    // Correction if needed
    HSSUnit correction = 0;
    if ((int) combinedThickness % 2)
    {
        correction = 0.5;
    }

    // Cumulative combined thickness
    HSSUnit cumulativeThickness = 0;

    // Draw all strokes
    for (it = strokes.begin(); it != strokes.end(); ++it)
    {
        const QSharedPointer<HSSAbstractStroke> & theStroke = *it;
        HSSUnit theSize = theStroke->getSize();

        HSSUnit offset = (combinedThickness / 2) - cumulativeThickness - (theSize / 2) + correction;

        QSharedPointer<HSSPath> path;
        HSSShape::createPath(path, offsetX + offset, offsetY + offset, width - offset * 2, height - offset * 2);
        this->getController()->document()->platform()->drawStroke(path, theStroke);

        cumulativeThickness += theSize;
    }
}
