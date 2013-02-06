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

#include <QPainterPath>
#include "AXRLoggerManager.h"
#include "HSSBorder.h"
#include "HSSCircle.h"
#include "HSSDisplayObject.h"

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

void HSSCircle::createPath(QPainterPath &path, HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, std::vector<QSharedPointer<HSSParserNode> > segments)
{
    path.addEllipse(x, y, width, height);
}

void HSSCircle::drawBorders(QPainter &painter, std::vector<QSharedPointer<HSSBorder> > borders, HSSUnit width, HSSUnit height, HSSUnit offsetX, HSSUnit offsetY)
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
