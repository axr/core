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
 *      COPYRIGHT: ©2019 - All Rights Reserved
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
#include "HSSPathArcTo.h"

using namespace AXR;

namespace AXR
{
    class HSSPathArcToPrivate
    {
        friend class HSSPathArcTo;
        
        HSSPathArcToPrivate()
        : x(0)
        , y(0)
        , width(0)
        , height(0)
        , angle(0)
        , sweepLength(0)
        {
        }
        HSSUnit x;
        HSSUnit y;
        HSSUnit width;
        HSSUnit height;
        HSSUnit angle;
        HSSUnit sweepLength;
    };
}

HSSPathArcTo::HSSPathArcTo(HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, HSSUnit angle, HSSUnit sweepLength)
: HSSPathCommand(HSSPathCommandTypeArcTo)
, d(new HSSPathArcToPrivate)
{
    d->x = x;
    d->y = y;
    d->width = width;
    d->height = height;
    d->angle = angle;
    d->sweepLength = sweepLength;
}

HSSPathArcTo::~HSSPathArcTo()
{
    delete d;
}

HSSUnit HSSPathArcTo::getX() const
{
    return d->x;
}

HSSUnit HSSPathArcTo::getY() const
{
    return d->y;
}

HSSUnit HSSPathArcTo::getWidth() const
{
    return d->width;
}

HSSUnit HSSPathArcTo::getHeight() const
{
    return d->height;
}

HSSUnit HSSPathArcTo::getAngle() const
{
    return d->angle;
}

HSSUnit HSSPathArcTo::getSweepLength() const
{
    return d->sweepLength;
}
