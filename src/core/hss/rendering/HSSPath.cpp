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
#include "HSSPath.h"

using namespace AXR;

namespace AXR
{
    class HSSPathPrivate
    {
        friend class HSSPath;
        
        HSSPathPrivate()
        : commands()
        {
        }
        std::vector<QSharedPointer<HSSPathCommand> > commands;
    };
}

HSSPath::HSSPath()
: d(new HSSPathPrivate)
{
    
}

HSSPath::~HSSPath()
{
    delete d;
}

void HSSPath::addCommand(QSharedPointer<HSSPathCommand> command)
{
    d->commands.push_back(command);
}

const std::vector<QSharedPointer<HSSPathCommand> > HSSPath::getCommands() const
{
    return d->commands;
}

void HSSPath::moveTo(HSSUnit x, HSSUnit y)
{
    QSharedPointer<HSSPathMoveTo> cmd(new HSSPathMoveTo(x, y));
    this->addCommand(cmd);
}

void HSSPath::lineTo(HSSUnit x, HSSUnit y)
{
    QSharedPointer<HSSPathLineTo> cmd(new HSSPathLineTo(x, y));
    this->addCommand(cmd);
}

void HSSPath::arcTo(HSSRect rect, HSSUnit angle, HSSUnit sweepLength)
{
    QSharedPointer<HSSPathArcTo> cmd(new HSSPathArcTo(rect.origin.x, rect.origin.y, rect.size.width, rect.size.height, angle, sweepLength));
    this->addCommand(cmd);
}

void HSSPath::subtract(QSharedPointer<HSSPath> otherPath)
{
    QSharedPointer<HSSPathSubtract> cmd(new HSSPathSubtract(otherPath));
    this->addCommand(cmd);
}

void HSSPath::closeSubpath()
{
    QSharedPointer<HSSPathCloseSubpath> cmd(new HSSPathCloseSubpath());
    this->addCommand(cmd);
}

void HSSPath::addRect(HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height)
{
    this->moveTo(x, y);
    this->lineTo(x+width, y);
    this->lineTo(x+width, y+height);
    this->lineTo(x, y+height);
    this->lineTo(x, y);
    this->closeSubpath();
}

void HSSPath::addEllipse(HSSRect rect)
{
    QSharedPointer<HSSPathAddEllipse> cmd(new HSSPathAddEllipse(rect));
    this->addCommand(cmd);
}

void HSSPath::addPolygon(std::vector<HSSPoint> points)
{
    QSharedPointer<HSSPathAddPolygon> cmd(new HSSPathAddPolygon(points));
    this->addCommand(cmd);
}
