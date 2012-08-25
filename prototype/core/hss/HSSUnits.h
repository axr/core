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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
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
     * @brief Represents an HSS unit. Currently defined as a long double.
     */
    typedef long double HSSUnit;

    /**
     * @brief   A resolution independent point.
     */
    class HSSPoint
    {
    public:
#if defined(HSS_UNITS_NATIVE_CONSTRUCTORS_WIN32)
        HSSPoint(const POINT &point) : x(point.x), y(point.y) { }
#endif
#if defined(HSS_UNITS_NATIVE_CONSTRUCTORS_APPLE)
        HSSPoint(const NSPoint &point) : x(point.x), y(point.y) { }
#endif
        HSSPoint() : x(0), y(0) { }
        HSSPoint(HSSUnit xAndY) : x(xAndY), y(xAndY) { }
        HSSPoint(HSSUnit x, HSSUnit y) : x(x), y(y) { }

        HSSUnit x; /**< the location of the point in horizontal dimension. */
        HSSUnit y; /**< the location of the point in vertical dimension. */
    };

    /**
     * @brief   Represents a size measure.
     */
    class HSSSize
    {
    public:
#ifdef HSS_UNITS_NATIVE_CONSTRUCTORS_APPLE
        HSSSize(const NSSize &size) : width(size.width), height(size.height) { }
#endif
        HSSSize() : width(0), height(0) { }
        HSSSize(HSSUnit widthAndHeight) : width(widthAndHeight), height(widthAndHeight) { }
        HSSSize(HSSUnit width, HSSUnit height) : width(width), height(height) { }

        HSSUnit width; /**< measure in the horizontal dimension. */
        HSSUnit height; /**< measure in the vertical dimension. */
    };


    /**
     * @brief   A basic rectangle.
     */
    class HSSRect
    {
    public:
#ifdef HSS_UNITS_NATIVE_CONSTRUCTORS_WIN32
        HSSRect(const RECT &rect) : origin(HSSPoint(rect.left, rect.top)), size(HSSSize(rect.right - rect.left - 1, rect.bottom - rect.top - 1)) { }
#endif
#ifdef HSS_UNITS_NATIVE_CONSTRUCTORS_APPLE
        HSSRect(const NSRect &rect) : origin(HSSPoint(rect.origin.x, rect.origin.y)), size(HSSSize(rect.size.width, rect.size.height)) { }
#endif
        HSSRect() { }
        HSSRect(const HSSSize &size) : origin(HSSPoint()), size(size) { }
        HSSRect(const HSSPoint &origin, const HSSSize &size) : origin(origin), size(size) { }
        HSSRect(HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height) : origin(HSSPoint(x, y)), size(HSSSize(width, height)) { }

        HSSPoint origin; /**< Where the rectangle starts. This point is on the top left corner of the rectangle. */
        HSSSize size; /**< The dimensions of the rectangle. */
    };
}

#endif
