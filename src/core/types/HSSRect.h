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

#ifndef AXR_HSSRECT
#define AXR_HSSRECT

#include "AXRGlobal.h"
#include "HSSPoint.h"
#include "HSSSize.h"

typedef struct _RECT RECT;
typedef struct CGRect CGRect;

#if __LP64__ || NS_BUILD_32_LIKE_64
typedef CGRect NSRect;
#else
typedef struct _NSRect NSRect;
#endif

namespace AXR
{
    /**
     * @brief A basic rectangle.
     */
    class AXR_API HSSRect
    {
    public:
        HSSRect(const RECT &rect);
#if !__LP64__ && !NS_BUILD_32_LIKE_64
        HSSRect(const CGRect &rect);
#endif
        HSSRect(const NSRect &rect);

        HSSRect(const QRectF &rect);
        HSSRect(const QRect &rect);
        HSSRect();
        HSSRect(const HSSSize &rectSize);
        HSSRect(const HSSPoint &rectOrigin, const HSSSize &rectSize);
        HSSRect(HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height);

        bool operator==(const HSSRect &other) const;
        bool operator!=(const HSSRect &other) const;

        HSSPoint origin; /**< Where the rectangle starts. This point is on the top left corner of the rectangle. */
        HSSSize size; /**< The dimensions of the rectangle. */
    };
}

#endif
