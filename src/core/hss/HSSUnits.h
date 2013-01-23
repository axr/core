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

#ifndef AXR_HSSUNITS_H
#define AXR_HSSUNITS_H

#include <QMetaType>
#include <QPoint>
#include <QRect>
#include "AXRGlobal.h"

#if defined(_WIN32)
#include <windows.h>
#define HSS_UNITS_NATIVE_CONSTRUCTORS_WIN32
#endif

#if defined(__APPLE__) && defined(__OBJC__)
#import <Foundation/Foundation.h>
#define HSS_UNITS_NATIVE_CONSTRUCTORS_APPLE
#endif

namespace AXR
{
    /**
     * @brief   A resolution independent point.
     */
    class AXR_API HSSPoint
    {
    public:
#if defined(HSS_UNITS_NATIVE_CONSTRUCTORS_WIN32)

        HSSPoint(const POINT &point) : x(point.x), y(point.y)
        {
        }
#endif
#if defined(HSS_UNITS_NATIVE_CONSTRUCTORS_APPLE)

        HSSPoint(const NSPoint &point) : x(point.x), y(point.y)
        {
        }
#endif

        HSSPoint(const QPointF &point) : x(point.x()), y(point.y())
        {
        }

        HSSPoint(const QPoint &point) : x(point.x()), y(point.y())
        {
        }

        HSSPoint() : x(0), y(0)
        {
        }

        HSSPoint(HSSUnit xAndY) : x(xAndY), y(xAndY)
        {
        }

        HSSPoint(HSSUnit pointX, HSSUnit pointY) : x(pointX), y(pointY)
        {
        }

        bool operator==(const HSSPoint &other) const
        {
            return qFuzzyIsNull(x - other.x) && qFuzzyIsNull(y - other.y);
        }

        bool operator!=(const HSSPoint &other) const
        {
            return !(*this == other);
        }

        HSSUnit x; /**< the location of the point in horizontal dimension. */
        HSSUnit y; /**< the location of the point in vertical dimension. */
    };

    /**
     * @brief   Represents a size measure.
     */
    class AXR_API HSSSize
    {
    public:
#ifdef HSS_UNITS_NATIVE_CONSTRUCTORS_APPLE

        HSSSize(const NSSize &size) : width(size.width), height(size.height)
        {
        }
#endif

        HSSSize(const QSizeF &size) : width(size.width()), height(size.height())
        {
        }

        HSSSize(const QSize &size) : width(size.width()), height(size.height())
        {
        }

        HSSSize() : width(0), height(0)
        {
        }

        HSSSize(HSSUnit widthAndHeight) : width(widthAndHeight), height(widthAndHeight)
        {
        }

        HSSSize(HSSUnit sizeWidth, HSSUnit sizeHeight) : width(sizeWidth), height(sizeHeight)
        {
        }

        bool operator==(const HSSSize &other) const
        {
            return qFuzzyIsNull(width - other.width) && qFuzzyIsNull(height - other.height);
        }

        bool operator!=(const HSSSize &other) const
        {
            return !(*this == other);
        }

        HSSUnit width; /**< measure in the horizontal dimension. */
        HSSUnit height; /**< measure in the vertical dimension. */
    };

    /**
     * @brief   A basic rectangle.
     */
    class AXR_API HSSRect
    {
    public:
#ifdef HSS_UNITS_NATIVE_CONSTRUCTORS_WIN32

        HSSRect(const RECT &rect) : origin(HSSPoint(rect.left, rect.top)), size(HSSSize(rect.right - rect.left - 1, rect.bottom - rect.top - 1))
        {
        }
#endif
#ifdef HSS_UNITS_NATIVE_CONSTRUCTORS_APPLE

        HSSRect(const NSRect &rect) : origin(HSSPoint(rect.origin.x, rect.origin.y)), size(HSSSize(rect.size.width, rect.size.height))
        {
        }
#endif

        HSSRect(const QRectF &rect) : origin(rect.topLeft()), size(rect.size())
        {
        }

        HSSRect(const QRect &rect) : origin(rect.topLeft()), size(rect.size())
        {
        }

        HSSRect()
        {
        }

        HSSRect(const HSSSize &rectSize) : origin(HSSPoint()), size(rectSize)
        {
        }

        HSSRect(const HSSPoint &rectOrigin, const HSSSize &rectSize) : origin(rectOrigin), size(rectSize)
        {
        }

        HSSRect(HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height) : origin(HSSPoint(x, y)), size(HSSSize(width, height))
        {
        }

        bool operator==(const HSSRect &other) const
        {
            return origin == other.origin && size == other.size;
        }

        bool operator!=(const HSSRect &other) const
        {
            return !(*this == other);
        }

        HSSPoint origin; /**< Where the rectangle starts. This point is on the top left corner of the rectangle. */
        HSSSize size; /**< The dimensions of the rectangle. */
    };
}

#endif
