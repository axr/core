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
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include "HSSPoint.h"
#include <QPoint>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

using namespace AXR;

#ifdef Q_OS_WIN
HSSPoint::HSSPoint(const POINT &point) : x(point.x), y(point.y)
{
}
#endif

HSSPoint::HSSPoint(const QPointF &point) : x(point.x()), y(point.y())
{
}

HSSPoint::HSSPoint(const QPoint &point) : x(point.x()), y(point.y())
{
}

HSSPoint::HSSPoint() : x(0), y(0)
{
}

HSSPoint::HSSPoint(HSSUnit xAndY) : x(xAndY), y(xAndY)
{
}

HSSPoint::HSSPoint(HSSUnit pointX, HSSUnit pointY) : x(pointX), y(pointY)
{
}

bool HSSPoint::operator==(const HSSPoint &other) const
{
    return qFuzzyIsNull(x - other.x) && qFuzzyIsNull(y - other.y);
}

bool HSSPoint::operator!=(const HSSPoint &other) const
{
    return !(*this == other);
}
