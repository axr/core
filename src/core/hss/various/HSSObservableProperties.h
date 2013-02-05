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

#ifndef HSSOBSERVABLEPROPERTIES_H
#define HSSOBSERVABLEPROPERTIES_H

#include "AXRGlobal.h"

namespace AXR
{
    enum AXR_API HSSObservableProperty
    {
        //HSSObject
        HSSObservablePropertyNone = 0,
        HSSObservablePropertyIsA,

        //HSSDisplayObject
        HSSObservablePropertyValue,
        HSSObservablePropertyWidth,
        HSSObservablePropertyInnerWidth,
        HSSObservablePropertyInnerHeight,
        HSSObservablePropertyOuterWidth,
        HSSObservablePropertyOuterHeight,
        HSSObservablePropertyHeight,
        HSSObservablePropertyAnchorX,
        HSSObservablePropertyAnchorY,
        HSSObservablePropertyFlow,
        HSSObservablePropertyAlignX,
        HSSObservablePropertyAlignY,
        HSSObservablePropertyMargin,
        HSSObservablePropertyPadding,
        HSSObservablePropertyBorder,
        HSSObservablePropertyFlagChanged,

        //HSSContainer
        HSSObservablePropertyContentAlignX,
        HSSObservablePropertyContentAlignY,
        HSSObservablePropertyDirectionPrimary,
        HSSObservablePropertyDirectionSecondary,
        HSSObservablePropertyBackground,
        HSSObservablePropertyFont,
        HSSObservablePropertyShape,
        HSSObservablePropertyOn,

        //HSSTextBlock
        HSSObservablePropertyTransform,
        HSSObservablePropertyTextAlign,
        HSSObservablePropertyText,

        //HSSRgb
        HSSObservablePropertyRed,
        HSSObservablePropertyGreen,
        HSSObservablePropertyBlue,
        HSSObservablePropertyAlpha,

        //HSSGradient
        HSSObservablePropertyStartColor,
        HSSObservablePropertyEndColor,
        HSSObservablePropertyColorStops,
        HSSObservablePropertyBalance,

        //HSSLinearGradient
        HSSObservablePropertyStartX,
        HSSObservablePropertyStartY,
        HSSObservablePropertyEndX,
        HSSObservablePropertyEndY,

        //HSSFont
        HSSObservablePropertySize,
        HSSObservablePropertyFace,
        HSSObservablePropertyColor,
        HSSObservablePropertyWeight,

        //HSSRoundedRect
        HSSObservablePropertyCorners,
        HSSObservablePropertyLeft,
        HSSObservablePropertyLeftTop,
        HSSObservablePropertyTop,
        HSSObservablePropertyRightTop,
        HSSObservablePropertyRight,
        HSSObservablePropertyRightBottom,
        HSSObservablePropertyBottom,
        HSSObservablePropertyLeftBottom,

        //HSSEvent
        HSSObservablePropertyAction,

        //HSSRequest
        HSSObservablePropertySrc,
        HSSObservablePropertyTarget,
        HSSObservablePropertyMode,
        HSSObservablePropertyContentTarget,

        //HSSColorStop
        HSSObservablePropertyPosition,

        HSSObservablePropertyTreeChange,
        HSSObservablePropertyContained,

        //HSSPolygon
        HSSObservablePropertySides,
        HSSObservablePropertyAngle,

        //HSSDisplayObject
        HSSObservablePropertyContent,
        HSSObservablePropertyVisible,

        //HSSBorder
        HSSObservablePropertySegments,

        //HSSRadialGradient
        HSSObservablePropertyCenterX,
        HSSObservablePropertyCenterY,
        HSSObservablePropertyOffsetX,
        HSSObservablePropertyOffsetY,
        HSSObservablePropertyRoundness,
    };
}

#endif //HSSOBSERVABLEPROPERTIES_H
