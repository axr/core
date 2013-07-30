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
: x(child->x - child->getLeftMargin())
, y(child->y - child->getTopMargin())
, direction(direction)
, owner(owner)
, complete(false)
, objects(new HSSSimpleSelection())
{
    this->objects->add(child);
    this->width = child->getOuterWidth();
    this->height = child->getOuterHeight();
    if (direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft)
    {
        if (child->hasOwnHeight())
        {
            this->height = child->getOuterHeight();
        }
        else
        {
            this->height = 0;
        }
    }
    else
    {
        if (child->hasOwnWidth())
        {
            this->width = child->getOuterWidth();
        }
        else
        {
            this->width = 0;
        }
    }
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
    this->cleanTrackedObservers();
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
            (horizontal && (measureGroup->width + child->getOuterWidth() > this->owner->getInnerWidth()))
            || (!horizontal && (measureGroup->height + child->getOuterHeight() > this->owner->getInnerHeight()))
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
                measureGroup->width += child->getOuterWidth();
                if (this->width < measureGroup->width)
                {
                    this->width = measureGroup->width;
                }
                if (child->hasOwnHeight() && measureGroup->height < child->getOuterHeight())
                {
                    measureGroup->height = child->getOuterHeight();
                }
            }
            else
            {
                measureGroup->height += child->getOuterHeight();
                if (this->height < measureGroup->height)
                {
                    this->height = measureGroup->height;
                }
                if (child->hasOwnWidth() && measureGroup->width < child->getOuterWidth())
                {
                    measureGroup->width = child->getOuterWidth();
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
            if (measureGroup->height < child->getOuterHeight())
            {
                measureGroup->height = child->getOuterHeight();
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
                HSSUnit accHeight = groupFirst->getHeight() - groupFirst->getAnchorY();
                HSSUnit anchorsTotal = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->getAlignY();
                    if (i > 0)
                    {
                        anchorsTotal += accHeight + currentChild->getAnchorY();
                        accHeight += currentChild->getOuterHeight();
                    }
                }
                HSSUnit groupAlignY = alignmentTotal / size;
                HSSUnit groupAnchorY = anchorsTotal / size;

                //reposition the elements in the group
                HSSUnit startY = groupAlignY - groupAnchorY - groupFirst->getAnchorY();
                startY = this->_clamp(startY, this->owner->getTopPadding(), (this->owner->getHeight() - this->owner->getBottomPadding()) - this->height);
                accHeight = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                    otherChild2->y = startY + accHeight;
                    accHeight += otherChild2->getHeight();
                }
                this->x = this->objects->front()->x - this->objects->front()->getLeftMargin();
                this->y = this->objects->front()->y - this->objects->front()->getTopMargin();
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
                    heightsTotal += currentChild->getOuterHeight();
                }
                HSSUnit medianHeight = heightsTotal / size;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->getAlignY() * currentChild->getOuterHeight() / medianHeight;
                }
                HSSUnit groupAlignY = alignmentTotal / size;

                //reposition the elements in the group
                HSSUnit startY = groupAlignY - heightsTotal / 2;
                startY = this->_clamp(startY, this->owner->getTopPadding(), (this->owner->getHeight() - this->owner->getBottomPadding()) - this->height);
                this->y = startY;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    currentChild->y = startY + accHeight + currentChild->getTopMargin();
                    accHeight += currentChild->getOuterHeight();
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
                HSSUnit accHeight = groupFirst->getAnchorY();
                HSSUnit anchorsTotal = 0;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->getAlignY();
                    if (i > 0)
                    {
                        anchorsTotal += accHeight + currentChild->getAnchorY();
                        accHeight += currentChild->getOuterHeight();
                    }
                }
                HSSUnit groupAlignY = alignmentTotal / size;
                HSSUnit groupAnchorY = anchorsTotal / size;

                //reposition the elements in the group
                HSSUnit startY = groupAlignY + groupAnchorY + (groupFirst->getHeight() - groupFirst->getAnchorY());
                startY = this->_clamp(startY, this->owner->getTopPadding(), this->owner->getHeight() - this->owner->getBottomPadding());
                accHeight = 0;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                    otherChild2->y = startY - otherChild2->getHeight() - accHeight;
                    accHeight += otherChild2->getHeight();
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
                    heightsTotal += currentChild->getOuterHeight();
                }
                HSSUnit medianHeight = heightsTotal / size;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->getAlignY() * currentChild->getOuterHeight() / medianHeight;
                }
                HSSUnit groupAlignY = alignmentTotal / size;

                //reposition the elements in the group
                HSSUnit startY = groupAlignY - heightsTotal / 2;
                startY = this->_clamp(startY, this->owner->getTopPadding(), (this->owner->getHeight() - this->owner->getBottomPadding()) - this->height);
                this->y = startY;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    currentChild->y = startY + accHeight + currentChild->getTopMargin();
                    accHeight += currentChild->getOuterHeight();
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
                HSSUnit accWidth = groupFirst->getAnchorX();
                HSSUnit anchorsTotal = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->getAlignX();
                    if (i > 0)
                    {
                        anchorsTotal += accWidth + currentChild->getAnchorX();
                        accWidth += currentChild->getOuterWidth();
                    }
                }
                HSSUnit groupAlignX = alignmentTotal / size;
                HSSUnit groupAnchorX = anchorsTotal / size;

                //reposition the elements in the group
                HSSUnit startX = groupAlignX + groupAnchorX + (groupFirst->getWidth() - groupFirst->getAnchorX());
                if (startX - this->width < 0) startX = this->width;
                if (startX > this->width) startX = this->width;
                accWidth = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                    otherChild2->x = startX - otherChild2->getWidth() - accWidth;
                    accWidth += otherChild2->getWidth();
                }
                this->x = this->objects->front()->x - this->objects->front()->getLeftMargin();
                this->y = this->objects->front()->y - this->objects->front()->getTopMargin();
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
                    widthsTotal += currentChild->getOuterWidth();
                }
                HSSUnit medianWidth = widthsTotal / size;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->getAlignX() * currentChild->getOuterWidth() / medianWidth;
                }
                HSSUnit groupAlignX = alignmentTotal / size;

                //reposition the elements in the group
                HSSUnit startX = groupAlignX - widthsTotal / 2;
                startX = this->_clamp(startX, this->owner->getLeftPadding(), (this->owner->getWidth() - this->owner->getRightPadding()) - this->width);
                this->x = startX;
                for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    currentChild->x = startX + accWidth + currentChild->getLeftMargin();
                    accWidth += currentChild->getOuterWidth();
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
                HSSUnit firstAnchorX = groupFirst->getAnchorX();
                HSSUnit accWidth = groupFirst->getOuterWidth() - firstAnchorX;
                HSSUnit anchorsTotal = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->getAlignX();
                    if (i > 0)
                    {
                        anchorsTotal += accWidth + currentChild->getAnchorX();
                        accWidth += currentChild->getOuterWidth();
                    }
                }
                HSSUnit groupAlignX = alignmentTotal / size;
                HSSUnit groupAnchorX = anchorsTotal / size;

                //reposition the elements in the group
                HSSUnit startX = groupAlignX - groupAnchorX - firstAnchorX;
                startX = this->_clamp(startX, this->owner->getLeftPadding(), (this->owner->getWidth() - this->owner->getRightPadding()) - this->width);

                accWidth = 0.;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                    otherChild2->x = startX + accWidth + otherChild2->getLeftMargin();
                    accWidth += otherChild2->getOuterWidth();
                }
                this->x = this->objects->front()->x - this->objects->front()->getLeftMargin();
                this->y = this->objects->front()->y - this->objects->front()->getTopMargin();

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
                    widthsTotal += currentChild->getOuterWidth();
                }
                HSSUnit medianWidth = widthsTotal / size;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    alignmentTotal += currentChild->getAlignX() * currentChild->getOuterWidth() / medianWidth;
                }
                HSSUnit groupAlignX = alignmentTotal / size;

                //reposition the elements in the group
                HSSUnit startX = groupAlignX - widthsTotal / 2;
                startX = this->_clamp(startX, this->owner->getLeftPadding(), (this->owner->getWidth() - this->owner->getRightPadding()) - this->width);
                this->x = startX;
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    currentChild->x = startX + accWidth + currentChild->getLeftMargin();
                    accWidth += currentChild->getOuterWidth();
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
                HSSUnit newValue = ((this->owner->getInnerWidth() - theDO->getOuterWidth()) / 2) + theDO->getLeftMargin();
                theDO->x = this->owner->getLeftPadding() + newValue;
                this->x = 0.;
                this->y = theDO->y;
            }
            else
            {
                HSSUnit accWidth = this->owner->getRightPadding();
                HSSUnit totalWidth = 0.;

                //calculate the total width of the group
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                {
                    totalWidth += (*it)->getOuterWidth();
                }

                if (this->owner->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->getInnerWidth() - totalWidth;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() - 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->x = this->owner->getWidth() - accWidth - (spaceChunk * i) - theDO->getWidth() - theDO->getRightMargin();
                        accWidth += theDO->getOuterWidth();
                        ++i;
                    }
                    this->x = 0.;
                    this->y = this->objects->back()->y;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->getInnerWidth() - totalWidth;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() + 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->x = this->owner->getWidth() - accWidth - spaceChunk - (spaceChunk * i) - theDO->getWidth() - theDO->getLeftMargin();
                        accWidth += theDO->getOuterWidth();
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
                HSSUnit newValue = ((this->owner->getInnerHeight() - theDO->getOuterHeight()) / 2) + theDO->getTopMargin();
                theDO->y = this->owner->getTopPadding() + newValue;
                this->y = 0.;
                this->x = theDO->x;
            }
            else
            {
                HSSUnit accHeight = this->owner->getTopPadding();
                HSSUnit totalHeight = 0.;

                //calculate the total height of the group
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                {
                    totalHeight += (*it)->getOuterHeight();
                }

                if (this->owner->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->getInnerHeight() - totalHeight;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() - 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->y = accHeight + (spaceChunk * i) + theDO->getTopMargin();
                        accHeight += theDO->getOuterHeight();
                        ++i;
                    }
                    this->x = 0.;
                    this->y = this->objects->front()->y;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->getInnerHeight() - totalHeight;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() + 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->y = accHeight + spaceChunk + (spaceChunk * i) + theDO->getTopMargin();
                        accHeight += theDO->getOuterHeight();
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
                HSSUnit newValue = ((this->owner->getInnerHeight() - theDO->getOuterHeight()) / 2) + theDO->getTopMargin();
                theDO->y = this->owner->getTopPadding() + newValue;
                this->y = 0.;
                this->x = theDO->x;
            }
            else
            {
                HSSUnit accHeight = this->owner->getBottomPadding();
                HSSUnit totalHeight = 0.;

                //calculate the total height of the group
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                {
                    totalHeight += (*it)->getOuterHeight();
                }

                if (this->owner->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->getInnerHeight() - totalHeight;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() - 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->y = this->owner->getHeight() - accHeight - (spaceChunk * i) - theDO->getHeight() - theDO->getBottomMargin();
                        accHeight += theDO->getOuterHeight();
                        ++i;
                    }
                    this->y = 0.;
                    this->x = this->objects->back()->x;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->getInnerHeight() - totalHeight;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() + 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        QSharedPointer<HSSDisplayObject> &theDO = *it;
                        theDO->y = this->owner->getHeight() - accHeight - spaceChunk - (spaceChunk * i) - theDO->getHeight() - theDO->getTopMargin();
                        accHeight += theDO->getOuterHeight();
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
                HSSUnit newValue = ((this->owner->getInnerWidth() - theDO->getOuterWidth()) / 2) + theDO->getLeftMargin();
                theDO->x = this->owner->getLeftPadding() + newValue;
                this->x = 0.;
                this->y = theDO->y;
            }
            else
            {
                HSSUnit accWidth = this->owner->getLeftPadding();
                HSSUnit totalWidth = 0.;

                //calculate the total width of the group
                for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                {
                    totalWidth += (*it)->getOuterWidth();
                }

                if (this->owner->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->getInnerWidth() - totalWidth;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() - 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        (*it)->x = accWidth + (spaceChunk * i) + (*it)->getLeftMargin();
                        accWidth += (*it)->getOuterWidth();
                        ++i;
                    }
                    this->x = 0.;
                    this->y = this->objects->front()->y;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->owner->getInnerWidth() - totalWidth;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk = remainingSpace / (this->objects->size() + 1);
                    unsigned i = 0;
                    for (HSSSimpleSelection::iterator it = this->objects->begin(); it != this->objects->end(); ++it)
                    {
                        (*it)->x = accWidth + spaceChunk + (spaceChunk * i) + (*it)->getLeftMargin();
                        accWidth += (*it)->getOuterWidth();
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
        widthsTotal += currentChild->getOuterWidth();
    }
    HSSUnit medianWidth = widthsTotal / size;
    for (HSSSimpleSelection::iterator it = this->objects->begin(); it!= this->objects->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> & currentChild = *it;
        alignmentTotal += currentChild->getAlignX() * currentChild->getOuterWidth() / medianWidth;
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
        heightsTotal += currentChild->getOuterHeight();
    }
    HSSUnit medianHeight = heightsTotal / size;
    for (HSSSimpleSelection::reverse_iterator it = this->objects->rbegin(); it!= this->objects->rend(); ++it)
    {
        QSharedPointer<HSSDisplayObject> & currentChild = *it;
        alignmentTotal += currentChild->getAlignY() * currentChild->getOuterHeight() / medianHeight;
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
