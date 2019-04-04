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

#ifndef AXR_HSSPOINT
#define AXR_HSSPOINT

#include "AXRGlobal.h"

#ifdef __APPLE__
#include "TargetConditionals.h"
#else
#define TARGET_OS_IPHONE 0
#endif

typedef struct tagPOINT POINT;
typedef struct CGPoint CGPoint;

#if __LP64__ || NS_BUILD_32_LIKE_64
typedef CGPoint NSPoint;
#else
typedef struct _NSPoint NSPoint;
#endif

namespace AXR
{
    /**
     * @brief A resolution independent point.
     */
    class AXR_API HSSPoint
    {
    public:
        HSSPoint(const POINT &point);
#if !__LP64__ && !NS_BUILD_32_LIKE_64
        HSSPoint(const CGPoint &point);
#endif
#if !TARGET_OS_IPHONE
        HSSPoint(const NSPoint &point);
#endif

        HSSPoint();
        HSSPoint(HSSUnit xAndY);
        HSSPoint(HSSUnit pointX, HSSUnit pointY);

        bool operator==(const HSSPoint &other) const;
        bool operator!=(const HSSPoint &other) const;

        HSSUnit x; /**< the location of the point in horizontal dimension. */
        HSSUnit y; /**< the location of the point in vertical dimension. */
    };
}

#endif
