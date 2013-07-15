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

HSSPolygon::HSSPolygon(AXRController * controller)
: HSSShape(HSSShapeTypePolygon, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSPolygon: creating polygon object");
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("sides");
    shorthandProperties.push_back("angle");
    this->setShorthandProperties(shorthandProperties);

}

HSSPolygon::HSSPolygon(const HSSPolygon & orig)
: HSSShape(orig)
{
    this->_initialize();
}

void HSSPolygon::_initialize()
{
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

