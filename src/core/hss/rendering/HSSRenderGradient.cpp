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
#include "HSSRenderGradient.h"

using namespace AXR;

HSSRenderColorStop::HSSRenderColorStop()
: _position(0)
{
    
}

HSSRenderColorStop::~HSSRenderColorStop()
{
    
}

void HSSRenderColorStop::setColor(QSharedPointer<HSSRgb> color)
{
    this->_color = color;
}

QSharedPointer<HSSRgb> HSSRenderColorStop::color() const
{
    return this->_color;
}

void HSSRenderColorStop::setPosition(HSSUnit pos)
{
    this->_position = pos;
}

HSSUnit HSSRenderColorStop::position() const
{
    return this->_position;
}

namespace AXR
{
    class HSSRenderGradientPrivate
    {
        friend class HSSRenderGradient;
        
        HSSRenderGradientPrivate()
        : colorStops()
        , startX(0)
        , endX(0)
        , startY(0)
        , endY(0)
        {
        }
        std::vector<QSharedPointer<HSSRenderColorStop> > colorStops;
        HSSUnit startX;
        HSSUnit startY;
        HSSUnit endX;
        HSSUnit endY;
    };
}

HSSRenderGradient::HSSRenderGradient()
: d(new HSSRenderGradientPrivate)
{
    
}

HSSRenderGradient::~HSSRenderGradient()
{
    delete d;
}

void HSSRenderGradient::addColorStop(QSharedPointer<HSSRenderColorStop> colorStop)
{
    d->colorStops.push_back(colorStop);
}

const std::vector<QSharedPointer<HSSRenderColorStop> > HSSRenderGradient::getColorStops() const
{
    return d->colorStops;
}

void HSSRenderGradient::setColorAt(HSSUnit value, QSharedPointer<HSSRgb> color)
{
    QSharedPointer<HSSRenderColorStop> stop(new HSSRenderColorStop());
    stop->setPosition(value);
    stop->setColor(color);
    this->addColorStop(stop);
}

void HSSRenderGradient::setStartX(HSSUnit value)
{
    d->startX = value;
}

HSSUnit HSSRenderGradient::startX() const
{
    return d->startX;
}

void HSSRenderGradient::setEndX(HSSUnit value)
{
    d->endX = value;
}

HSSUnit HSSRenderGradient::endX() const
{
    return d->endX;
}

void HSSRenderGradient::setStartY(HSSUnit value)
{
    d->startY = value;
}

HSSUnit HSSRenderGradient::startY() const
{
    return d->startY;
}

void HSSRenderGradient::setEndY(HSSUnit value)
{
    d->endY = value;
}

HSSUnit HSSRenderGradient::endY() const
{
    return d->endY;
}
