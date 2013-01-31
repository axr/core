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

#include "HSSRect.h"
#include <QRect>

#ifdef Q_OS_WIN
#include <qt_windows.h>
#endif

using namespace AXR;

#ifdef Q_OS_WIN
HSSRect::HSSRect(const RECT &rect) : origin(HSSPoint(rect.left, rect.top)), size(HSSSize(rect.right - rect.left - 1, rect.bottom - rect.top - 1))
{
}
#endif

HSSRect::HSSRect(const QRectF &rect) : origin(rect.topLeft()), size(rect.size())
{
}

HSSRect::HSSRect(const QRect &rect) : origin(rect.topLeft()), size(rect.size())
{
}

HSSRect::HSSRect()
{
}

HSSRect::HSSRect(const HSSSize &rectSize) : origin(HSSPoint()), size(rectSize)
{
}

HSSRect::HSSRect(const HSSPoint &rectOrigin, const HSSSize &rectSize) : origin(rectOrigin), size(rectSize)
{
}

HSSRect::HSSRect(HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height) : origin(HSSPoint(x, y)), size(HSSSize(width, height))
{
}

bool HSSRect::operator==(const HSSRect &other) const
{
    return origin == other.origin && size == other.size;
}

bool HSSRect::operator!=(const HSSRect &other) const
{
    return !(*this == other);
}
