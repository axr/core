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

#include "AXRError.h"
#include "HSSContainer.h"
#include "HSSDisplayObject.h"
#include "HSSLayoutLine.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

HSSLayoutLine::HSSLayoutLine(HSSUnit x, HSSUnit y, HSSUnit width, HSSUnit height, HSSDirectionValue direction, HSSContainer * owner)
: x(x)
, y(y)
, width(width)
, height(height)
, direction(direction)
, owner(owner)
, complete(false)
, objects(new HSSSimpleSelection())
{

}

HSSLayoutLine::HSSLayoutLine(const QSharedPointer<HSSDisplayObject> & child, HSSDirectionValue direction, HSSContainer * owner)
: x(child->x - child->leftMargin)
, y(child->y - child->topMargin)
, width(child->outerWidth)
, height(child->outerHeight)
, direction(direction)
, owner(owner)
, complete(false)
, objects(new HSSSimpleSelection())
{
    this->objects->add(child);
}

HSSLayoutLine::HSSLayoutLine(const HSSLayoutLine & orig)
: x(orig.x)
, y(orig.y)
, width(orig.width)
, height(orig.height)
, direction(orig.direction)
, owner(orig.owner)
, complete(orig.complete)
, lines(orig.lines)
, objects(orig.objects)
{

}

QSharedPointer<HSSLayoutLine> HSSLayoutLine::clone() const
{
    return qSharedPointerCast<HSSLayoutLine> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSLayoutLine::cloneImpl() const
{
    return QSharedPointer<HSSLayoutLine>(new HSSLayoutLine(*this));
}

HSSLayoutLine::~HSSLayoutLine()
{
    delete this->objects;
}
