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

bool HSSLayoutLine::heightGreater(const QSharedPointer<HSSLayoutLine>& x, const QSharedPointer<HSSLayoutLine>& y)
{
    return x->height > y->height;
}

void HSSLayoutLine::convertObjectsIntoLines()
{
    //create the line
    QSharedPointer<HSSLayoutLine> line = QSharedPointer<HSSLayoutLine>(new HSSLayoutLine(this->x, this->y, this->width, this->height, this->direction, this->owner));
    for (HSSSimpleSelection::const_iterator it=this->objects->begin(); it!=this->objects->end(); ++it)
    {
        line->objects->add(*it);
    }
    this->objects->clear();
    this->lines.push_back(line);
}

void HSSLayoutLine::mergeWith(const QSharedPointer<HSSLayoutLine> & groupB)
{
    if(groupB->lines.size() > 0){
        for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it=groupB->lines.begin(); it!=groupB->lines.end(); ++it)
        {
            const QSharedPointer<HSSLayoutLine> & line = *it;
            for (HSSSimpleSelection::const_iterator it2=line->objects->begin(); it2!=line->objects->end(); ++it2)
            {
                const QSharedPointer<HSSDisplayObject> & child = *it2;
                this->add(child);
            }
        }
    }
    else
    {
        for (HSSSimpleSelection::const_iterator it=groupB->objects->begin(); it!=groupB->objects->end(); ++it)
        {
            const QSharedPointer<HSSDisplayObject> & child = *it;
            this->add(child);
        }
    }
}

void HSSLayoutLine::add(const QSharedPointer<HSSDisplayObject> & child)
{
    bool horizontal = (this->direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft);
    HSSLayoutLine * measureGroup;
    bool basicGroup = false;
    if (this->lines.size() == 0)
    {
        measureGroup = this;
        basicGroup = true;
    }
    else
    {
        measureGroup = this->lines.back().data();
    }
    if (measureGroup->overlapsX(child)) {
        if (
            (horizontal && (measureGroup->width + child->outerWidth > this->owner->innerWidth))
            || (!horizontal && (measureGroup->height + child->outerHeight > this->owner->innerHeight))
            )
        {
            if (basicGroup) {
                measureGroup->convertObjectsIntoLines();
            }

            QSharedPointer<HSSLayoutLine> newGroup = QSharedPointer<HSSLayoutLine>(new HSSLayoutLine(child, this->direction, this->owner));
            this->lines.push_back(newGroup);
            if (horizontal)
            {
                this->height += newGroup->height;
                if (newGroup->width > this->width)
                {
                    this->width = newGroup->width;
                }
            }
            else
            {
                this->width += newGroup->width;
                if (newGroup->height > this->height)
                {
                    this->height = newGroup->height;
                }
            }
        }
        else
        {
            measureGroup->objects->add(child);
            if (horizontal)
            {
                measureGroup->width += child->outerWidth;
                if (this->width < measureGroup->width)
                {
                    this->width = measureGroup->width;
                }
                if (measureGroup->height < child->outerHeight)
                {
                    measureGroup->height = child->outerHeight;
                }
            }
            else
            {
                measureGroup->height += child->outerHeight;
                if (this->height < measureGroup->height)
                {
                    this->height = measureGroup->height;
                }
                if (!horizontal && measureGroup->width < child->outerWidth)
                {
                    measureGroup->width = child->outerWidth;
                }
            }
        }
    }
    else
    {
        if (basicGroup)
        {
            AXRError("HSSLayoutLine", "Unexpected adding of child to a basic group with no overlaps").raise();
            return;
        }
        else
        {
            measureGroup->convertObjectsIntoLines();
            QSharedPointer<HSSLayoutLine> newGroup = QSharedPointer<HSSLayoutLine>(new HSSLayoutLine(child, this->direction, this->owner));
            measureGroup->lines.push_back(newGroup);
            if (measureGroup->height < child->outerHeight)
            {
                measureGroup->height = child->outerHeight;
            }
        }
    }

}

void HSSLayoutLine::arrange()
{
    size_t i = 0, size = this->objects->size();

    switch (this->direction)
    {
        case HSSDirectionTopToBottom:
        {
            bool byAnchors = false;
            for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
            {
                if (!(*it)->_anchorYdefault)
                {
                    byAnchors = true;
                    break;
                }
            }
            //sort the elements by alignment point
            std::sort(this->objects->begin(), this->objects->end(), HSSContainer::alignYSmaller);
            if (byAnchors)
            {
                //calculate the new alignment and anchor point for the group
                const QSharedPointer<HSSDisplayObject> & groupFirst = this->objects->front();
                HSSUnit alignmentTotal = 0.;
                HSSUnit accHeight = groupFirst->height - groupFirst->anchorY;
                HSSUnit anchorsTotal = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->alignY;
                    if (i > 0)
                    {
                        anchorsTotal += accHeight + currentChild->anchorY;
                        accHeight += currentChild->outerHeight;
                    }
                }
                HSSUnit groupAlignY = alignmentTotal / size;
                HSSUnit groupAnchorY = anchorsTotal / size;

                //reposition the elements in the group
                HSSUnit startY = groupAlignY - groupAnchorY - groupFirst->anchorY;
                startY = this->_clamp(startY, this->owner->topPadding, (this->owner->height - this->owner->bottomPadding) - this->height);
                accHeight = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                    otherChild2->y = startY + accHeight;
                    accHeight += otherChild2->height;
                }
                this->x = this->objects->front()->x - this->objects->front()->leftMargin;
                this->y = this->objects->front()->y - this->objects->front()->topMargin;
            }
            else
            {
                //calculate the alignment point for the group
                HSSUnit alignmentTotal = 0.;
                HSSUnit accHeight = 0.;
                HSSUnit heightsTotal = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    heightsTotal += currentChild->outerHeight;
                }
                HSSUnit medianHeight = heightsTotal / size;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->alignY * currentChild->outerHeight / medianHeight;
                }
                HSSUnit groupAlignY = alignmentTotal / size;

                //reposition the elements in the group
                HSSUnit startY = groupAlignY - heightsTotal / 2;
                startY = this->_clamp(startY, this->owner->topPadding, (this->owner->height - this->owner->bottomPadding) - this->height);
                this->y = startY;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    currentChild->y = startY + accHeight + currentChild->topMargin;
                    accHeight += currentChild->outerHeight;
                }
            }
            break;
        }

        case HSSDirectionBottomToTop:
        {
            bool byAnchors = false;
            for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
            {
                if (!(*it)->_anchorYdefault)
                {
                    byAnchors = true;
                    break;
                }
            }
            //sort the elements by alignment point
            std::sort(this->objects->rbegin(), this->objects->rend(), HSSContainer::alignYSmaller);
            if (byAnchors)
            {
                //calculate the new alignment and anchor point for the group
                const QSharedPointer<HSSDisplayObject> & groupFirst = this->objects->front();
                HSSUnit alignmentTotal = 0;
                HSSUnit accHeight = groupFirst->anchorY;
                HSSUnit anchorsTotal = 0;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->alignY;
                    if (i > 0)
                    {
                        anchorsTotal += accHeight + currentChild->anchorY;
                        accHeight += currentChild->outerHeight;
                    }
                }
                HSSUnit groupAlignY = alignmentTotal / size;
                HSSUnit groupAnchorY = anchorsTotal / size;

                //reposition the elements in the group
                HSSUnit startY = groupAlignY + groupAnchorY + (groupFirst->height - groupFirst->anchorY);
                startY = this->_clamp(startY, this->owner->topPadding, this->owner->height - this->owner->bottomPadding);
                accHeight = 0;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                    otherChild2->y = startY - otherChild2->height - accHeight;
                    accHeight += otherChild2->height;
                }
                this->x = this->objects->front()->x;
                this->y = this->objects->front()->y;
            }
            else
            {
                //calculate the alignment point for the group
                HSSUnit alignmentTotal = 0.;
                HSSUnit accHeight = 0.;
                HSSUnit heightsTotal = 0.;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    heightsTotal += currentChild->outerHeight;
                }
                HSSUnit medianHeight = heightsTotal / size;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->alignY * currentChild->outerHeight / medianHeight;
                }
                HSSUnit groupAlignY = alignmentTotal / size;

                //reposition the elements in the group
                HSSUnit startY = groupAlignY - heightsTotal / 2;
                startY = this->_clamp(startY, this->owner->topPadding, (this->owner->height - this->owner->bottomPadding) - this->height);
                this->y = startY;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    currentChild->y = startY + accHeight + currentChild->topMargin;
                    accHeight += currentChild->outerHeight;
                }
            }

            break;
        }

        case HSSDirectionRightToLeft:
        {
            bool byAnchors = false;
            for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
            {
                if (!(*it)->_anchorXdefault)
                {
                    byAnchors = true;
                    break;
                }
            }
            //sort the elements by alignment point
            std::sort(this->objects->rbegin(), this->objects->rend(), HSSContainer::alignXSmaller);
            if (byAnchors)
            {
                //calculate the new alignment and anchor point for the group
                const QSharedPointer<HSSDisplayObject> & groupFirst = this->objects->front();
                HSSUnit alignmentTotal = 0.;
                HSSUnit accWidth = groupFirst->anchorX;
                HSSUnit anchorsTotal = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->alignX;
                    if (i > 0)
                    {
                        anchorsTotal += accWidth + currentChild->anchorX;
                        accWidth += currentChild->outerWidth;
                    }
                }
                HSSUnit groupAlignX = alignmentTotal / size;
                HSSUnit groupAnchorX = anchorsTotal / size;

                //reposition the elements in the group
                HSSUnit startX = groupAlignX + groupAnchorX + (groupFirst->width - groupFirst->anchorX);
                if (startX - this->width < 0) startX = this->width;
                if (startX > this->width) startX = this->width;
                accWidth = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                    otherChild2->x = startX - otherChild2->width - accWidth;
                    accWidth += otherChild2->width;
                }
                this->x = this->objects->front()->x - this->objects->front()->leftMargin;
                this->y = this->objects->front()->y - this->objects->front()->topMargin;
            }
            else
            {
                //calculate the alignment point for the group
                HSSUnit alignmentTotal = 0.;
                HSSUnit accWidth = 0.;
                HSSUnit widthsTotal = 0.;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    widthsTotal += currentChild->outerWidth;
                }
                HSSUnit medianWidth = widthsTotal / size;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->alignX * currentChild->outerWidth / medianWidth;
                }
                HSSUnit groupAlignX = alignmentTotal / size;

                //reposition the elements in the group
                HSSUnit startX = groupAlignX - widthsTotal / 2;
                startX = this->_clamp(startX, this->owner->leftPadding, (this->owner->width - this->owner->rightPadding) - this->width);
                this->x = startX;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    currentChild->x = startX + accWidth + currentChild->leftMargin;
                    accWidth += currentChild->outerWidth;
                }
            }
            break;
        }

        default:
        {
            bool byAnchors = false;
            for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
            {
                if (!(*it)->_anchorXdefault)
                {
                    byAnchors = true;
                    break;
                }
            }
            //sort the elements by alignment point
            std::sort(this->objects->begin(), this->objects->end(), HSSContainer::alignXSmaller);
            if (byAnchors)
            {
                //calculate the new alignment and anchor point for the group
                const QSharedPointer<HSSDisplayObject> & groupFirst = this->objects->front();
                HSSUnit alignmentTotal = 0.;
                HSSUnit accWidth = groupFirst->outerWidth - groupFirst->anchorX;
                HSSUnit anchorsTotal = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->alignX;
                    if (i > 0)
                    {
                        anchorsTotal += accWidth + currentChild->anchorX;
                        accWidth += currentChild->outerWidth;
                    }
                }
                HSSUnit groupAlignX = alignmentTotal / size;
                HSSUnit groupAnchorX = anchorsTotal / size;

                //reposition the elements in the group
                HSSUnit startX = groupAlignX - groupAnchorX - groupFirst->anchorX;
                startX = this->_clamp(startX, this->owner->leftPadding, (this->owner->width - this->owner->rightPadding) - this->width);

                accWidth = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                    otherChild2->x = startX + accWidth + otherChild2->leftMargin;
                    accWidth += otherChild2->outerWidth;
                }
                this->x = this->objects->front()->x - this->objects->front()->leftMargin;
                this->y = this->objects->front()->y - this->objects->front()->topMargin;

            }
            else
            {
                //calculate the alignment point for the group
                HSSUnit alignmentTotal = 0.;
                HSSUnit accWidth = 0.;
                HSSUnit widthsTotal = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    widthsTotal += currentChild->outerWidth;
                }
                HSSUnit medianWidth = widthsTotal / size;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->alignX * currentChild->outerWidth / medianWidth;
                }
                HSSUnit groupAlignX = alignmentTotal / size;

                //reposition the elements in the group
                HSSUnit startX = groupAlignX - widthsTotal / 2;
                startX = this->_clamp(startX, this->owner->leftPadding, (this->owner->width - this->owner->rightPadding) - this->width);
                this->x = startX;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    currentChild->x = startX + accWidth + currentChild->leftMargin;
                    accWidth += currentChild->outerWidth;
                }
            }
            break;
        }
    }
}

void HSSLayoutLine::distribute()
{
    switch (direction)
    {
        case HSSDirectionRightToLeft:
        {
            if (this->objects->size() == 1)
            {
                const QSharedPointer<HSSDisplayObject> &theDO = this->objects->front();
                HSSUnit newValue = ((this->owner->innerWidth - theDO->outerWidth) / 2) + theDO->leftMargin;
                theDO->x = this->owner->leftPadding + newValue;
                this->x = 0.;
                this->y = theDO->y;
            }
            else
            {
                HSSUnit accWidth = this->owner->rightPadding;
                HSSUnit totalWidth = 0.;

                //calculate the total width of the group
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                {
                    totalWidth += (*it)->outerWidth;
                }

                if (this->owner->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->innerWidth - totalWidth;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() - 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->x = this->owner->width - accWidth - (spaceChunk * i) - theDO->width - theDO->rightMargin;
                        accWidth += theDO->outerWidth;
                        ++i;
                    }
                    this->x = 0.;
                    this->y = this->objects->back()->y;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->innerWidth - totalWidth;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() + 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->x = this->owner->width - accWidth - spaceChunk - (spaceChunk * i) - theDO->width - theDO->leftMargin;
                        accWidth += theDO->outerWidth;
                        ++i;
                    }
                    this->x = 0.;
                    this->y = this->objects->front()->y;
                }
            }

            break;
        }

        case HSSDirectionTopToBottom:
        {
            if (this->objects->size() == 1)
            {
                const QSharedPointer<HSSDisplayObject> &theDO = this->objects->front();
                HSSUnit newValue = ((this->owner->innerHeight - theDO->outerHeight) / 2) + theDO->topMargin;
                theDO->y = this->owner->topPadding + newValue;
                this->y = 0.;
                this->x = theDO->x;
            }
            else
            {
                HSSUnit accHeight = this->owner->topPadding;
                HSSUnit totalHeight = 0.;

                //calculate the total height of the group
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                {
                    totalHeight += (*it)->outerHeight;
                }

                if (this->owner->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->innerHeight - totalHeight;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() - 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->y = accHeight + (spaceChunk * i) + theDO->topMargin;
                        accHeight += theDO->outerHeight;
                        ++i;
                    }
                    this->x = 0.;
                    this->y = this->objects->front()->y;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->innerHeight - totalHeight;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() + 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->y = accHeight + spaceChunk + (spaceChunk * i) + theDO->topMargin;
                        accHeight += theDO->outerHeight;
                        ++i;
                    }
                    this->y = 0.;
                    this->x = this->objects->front()->x;
                }
            }
            break;
        }

        case HSSDirectionBottomToTop:
        {
            if (this->objects->size() == 1)
            {
                const QSharedPointer<HSSDisplayObject> &theDO = this->objects->front();
                HSSUnit newValue = ((this->owner->innerHeight - theDO->outerHeight) / 2) + theDO->topMargin;
                theDO->y = this->owner->topPadding + newValue;
                this->y = 0.;
                this->x = theDO->x;
            }
            else
            {
                HSSUnit accHeight = this->owner->bottomPadding;
                HSSUnit totalHeight = 0.;

                //calculate the total height of the group
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                {
                    totalHeight += (*it)->outerHeight;
                }

                if (this->owner->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->innerHeight - totalHeight;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() - 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->y = this->owner->height - accHeight - (spaceChunk * i) - theDO->height - theDO->bottomMargin;
                        accHeight += theDO->outerHeight;
                        ++i;
                    }
                    this->y = 0.;
                    this->x = this->objects->back()->x;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->innerHeight - totalHeight;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() + 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->y = this->owner->height - accHeight - spaceChunk - (spaceChunk * i) - theDO->height - theDO->topMargin;
                        accHeight += theDO->outerHeight;
                        ++i;
                    }
                    this->y = 0.;
                    this->x = this->objects->front()->x;
                }
            }
            break;
        }

        default:
        {
            if (this->objects->size() == 1)
            {
                const QSharedPointer<HSSDisplayObject> &theDO = this->objects->front();
                HSSUnit newValue = ((this->owner->innerWidth - theDO->outerWidth) / 2) + theDO->leftMargin;
                theDO->x = this->owner->leftPadding + newValue;
                this->x = 0.;
                this->y = theDO->y;
            }
            else
            {
                HSSUnit accWidth = this->owner->leftPadding;
                HSSUnit totalWidth = 0.;

                //calculate the total width of the group
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                {
                    totalWidth += (*it)->outerWidth;
                }

                if (this->owner->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->innerWidth - totalWidth;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() - 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        (*it)->x = accWidth + (spaceChunk * i) + (*it)->leftMargin;
                        accWidth += (*it)->outerWidth;
                        ++i;
                    }
                    this->x = 0.;
                    this->y = this->objects->front()->y;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->innerWidth - totalWidth;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() + 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        (*it)->x = accWidth + spaceChunk + (spaceChunk * i) + (*it)->leftMargin;
                        accWidth += (*it)->outerWidth;
                        ++i;
                    }
                    this->x = 0.;
                    this->y = this->objects->front()->y;
                }
            }

            break;
        }
    }
}

HSSUnit HSSLayoutLine::getAlignX()
{
    HSSUnit alignmentTotal = 0.;
    HSSUnit widthsTotal = 0.;
    HSSUnit size = this->objects->size();
    if(size == 0){
        return 0.;
    }
    for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> & currentChild = *it;
        widthsTotal += currentChild->outerWidth;
    }
    HSSUnit medianWidth = widthsTotal / size;
    for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> & currentChild = *it;
        alignmentTotal += currentChild->alignX * currentChild->outerWidth / medianWidth;
    }
    return alignmentTotal / size;
}

HSSUnit HSSLayoutLine::getAlignY()
{
    HSSUnit alignmentTotal = 0.;
    HSSUnit heightsTotal = 0.;
    HSSUnit size = this->objects->size();
    if(size == 0){
        return 0.;
    }

    for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
    {
        QSharedPointer<HSSDisplayObject> & currentChild = *it;
        heightsTotal += currentChild->outerHeight;
    }
    HSSUnit medianHeight = heightsTotal / size;
    for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
    {
        QSharedPointer<HSSDisplayObject> & currentChild = *it;
        alignmentTotal += currentChild->alignY * currentChild->outerHeight / medianHeight;
    }
    return alignmentTotal / size;
}

inline HSSUnit HSSLayoutLine::_clamp(const HSSUnit & value, const HSSUnit & min, const HSSUnit & max) const
{
    if (value > max)
    {
        return max;
    }
    if (value < min)
    {
        return min;
    }

    return value;
}
