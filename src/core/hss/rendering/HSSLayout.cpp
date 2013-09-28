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

#include <limits>
#include <QFont>
#include <QFontMetrics>
#include <QVector>
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRError.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSLayout.h"
#include "HSSLayoutLine.h"
#include "HSSContainer.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSRule.h"
#include "HSSTextBlock.h"
#include "HSSValue.h"

using namespace AXR;

class HSSLayout::Private
{
public:
    Private() : document()
    {
    }

    AXRDocument *document;
};

HSSLayout::HSSLayout()
: d(new Private)
{
    this->setFilterFlags(HSSVisitorFilterLayout);
}

HSSLayout::~HSSLayout()
{
    delete d;
}

void HSSLayout::setDocument(AXRDocument* document)
{
    d->document = document;
}

void HSSLayout::initializeVisit()
{
    axr_log(LoggerChannelRendering, "INITIALIZE_LAYOUT");
    //prepare values
    QSharedPointer<HSSContainer> root = d->document->controller()->root();
    if (!root)
    {
        AXRError("HSSLayout", "Fatal error: No root").raise();
    }
}

inline bool HSSLayout::_overlaps(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const
{
    return this->_overlaps_horizontal(childA, childB) && this->_overlaps_vertical(childA, childB);
}

inline bool HSSLayout::_overlaps_horizontal(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const
{
    HSSUnit widthA = qMax((HSSUnit)1, childA->getWidth() + childA->getRightMargin());
    HSSUnit widthB = qMax((HSSUnit)1, childB->getWidth()+ childB->getRightMargin());

    if (
        (childA->x + widthA) > (childB->x - childB->getLeftMargin())
        && (childA->x - childA->getLeftMargin()) < (childB->x + widthB)
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool HSSLayout::_overlaps_vertical(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const
{
    HSSUnit heightA = qMax((HSSUnit)1, childA->getHeight() + childA->getTopMargin());
    HSSUnit heightB = qMax((HSSUnit)1, childB->getHeight() + childB->getTopMargin());

    if (
        (childA->y + heightA) > (childB->y - childB->getTopMargin())
        && (childA->y - childA->getTopMargin()) < (childB->y + heightB)
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline void HSSLayout::_placeOnAlignmentPoint(const HSSContainer & container, const QSharedPointer<HSSDisplayObject> & child)
{
    if (child->_anchorXdefault)
    {
        child->x = container.getLeftPadding() + child->getAlignX() - (child->getOuterWidth() / 2) + child->getLeftMargin();
    }
    else
    {
        child->x = container.getLeftPadding()+ child->getAlignX() - child->getAnchorX();
    }

    if (child->_anchorYdefault)
    {
        child->y = container.getTopPadding() + child->getAlignY() - (child->getOuterHeight() / 2) + child->getTopMargin();
    }
    else
    {
        child->y = container.getTopPadding() + child->getAlignY() - child->getAnchorY();
    }
    if (child->getContained())
    {
        //on the right side
        if ((child->x + child->getWidth() + child->getRightMargin()) > (container.getWidth() - container.getRightPadding()))
        {
            child->x = (container.getWidth() - container.getRightPadding()) - (child->getWidth() + child->getRightMargin());
        }
        //on the left side
        if (child->x < child->getLeftMargin() + container.getLeftPadding())
        {
            child->x = child->getLeftMargin() + container.getLeftPadding();
        }
        //on the bottom
        if ((child->y + child->getHeight() + child->getBottomMargin()) > (container.getHeight() - container.getBottomPadding()))
        {
            child->y = (container.getHeight() - container.getBottomPadding()) - (child->getHeight() + child->getBottomMargin());
        }
        //on the top
        if (child->y < child->getTopMargin() + container.getTopPadding())
        {
            child->y = child->getTopMargin() + container.getTopPadding();
        }
    }
}

inline bool HSSLayout::_layoutTick() const
{
    AXRDocument * document = this->d->document;
    if (document->showLayoutSteps())
    {
        document->nextLayoutTick();
        document->nextLayoutChild();
        document->breakIfNeeded();
        if (document->layoutStepDone())
        {
            return true;
        }
    }
    return false;
}

inline void HSSLayout::_setGlobalPosition(const HSSContainer & container, const QSharedPointer<HSSDisplayObject> & child) const
{
    child->setGlobalX(qRound(container.globalX + child->x));
    child->setGlobalY(qRound(container.globalY + child->y));
}

inline const QSharedPointer<HSSLayoutLine> HSSLayout::_getTargetGroup(const QSharedPointer<HSSDisplayObject> & child, std::vector<QSharedPointer<HSSLayoutLine> > & groups) const
{
    QSharedPointer<HSSLayoutLine> ret;
    for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it=groups.begin(); it!=groups.end(); ++it)
    {
        const QSharedPointer<HSSLayoutLine> & group = *it;
        if (group->lines.size() > 0)
        {
            for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it2=group->lines.begin(); it2!=group->lines.end(); ++it2)
            {
                const QSharedPointer<HSSLayoutLine> & line = *it2;
                for (HSSSimpleSelection::const_iterator it3=line->objects->begin(); it3!=line->objects->end(); ++it3)
                {
                    if (this->_overlaps(child, *it3))
                    {
                        return group;
                    }
                }
            }
        }
        else
        {
            for (HSSSimpleSelection::const_iterator it2=group->objects->begin(); it2!=group->objects->end(); ++it2) {
                if (this->_overlaps(child, *it2))
                {
                    return group;
                }
            }
        }
    }

    return ret;
}

inline void HSSLayout::_setPositions(const HSSContainer & container, const QSharedPointer<HSSLayoutLine> & group) const
{
    for (HSSSimpleSelection::const_iterator it=group->objects->begin(); it!=group->objects->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & child = *it;
        this->_setGlobalPosition(container, child);
    }
}

inline void HSSLayout::_lineArrangeX(const QSharedPointer<HSSDisplayObject> & child, HSSUnit startX, HSSUnit lineWidth) const
{
    child->x = (startX + ((lineWidth / 2) - (child->getOuterWidth() / 2)) * child->getLineAlign() * 2) + child->getLeftMargin();
}

inline void HSSLayout::_lineArrangeY(const QSharedPointer<HSSDisplayObject> & child, HSSUnit startY, HSSUnit lineHeight) const
{
    child->y = (startY + ((lineHeight / 2) - (child->getOuterHeight() / 2)) * child->getLineAlign() * 2) + child->getTopMargin();
}

inline bool HSSLayout::_arrangeLines(const HSSContainer & container, const QSharedPointer<HSSLayoutLine> &group, const HSSDirectionValue direction) const
{
    switch (direction)
    {
        case HSSDirectionLeftToRight:
        {
            if (group->lines.size() > 0)
            {
                //calculate the starting point
                HSSUnit widthsTotal = 0.;
                HSSUnit groupAlignmentTotal = 0.;
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = group->lines.begin(); it!= group->lines.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    HSSUnit alignmentTotal = 0.;
                    for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                    {
                        QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                        alignmentTotal += currentChild->getAlignX();
                    }
                    groupAlignmentTotal += alignmentTotal / line->objects->size();
                    widthsTotal += line->width;
                }
                HSSUnit groupAlignX = groupAlignmentTotal / group->lines.size();
                HSSUnit startX = groupAlignX - (widthsTotal/2);
                if (startX > (container.getWidth() - container.getRightPadding()) - widthsTotal) startX = (container.getWidth() - container.getRightPadding()) - widthsTotal;
                if (startX < container.getLeftPadding()) startX = container.getLeftPadding();

                //reposition each element
                HSSUnit accWidth = startX;
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = group->lines.begin(); it!= group->lines.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    line->x = accWidth;
                    for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                    {
                        QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                        this->_lineArrangeX(currentChild, accWidth, line->width);
                    }
                    accWidth += line->width;
                    if (this->_layoutTick()){
                        return true;
                    }
                }
            }
            else
            {
                HSSUnit maxWidth = 0.;
                QSharedPointer<HSSDisplayObject> widestChild;
                for (HSSSimpleSelection::iterator it = group->objects->begin(); it!= group->objects->end(); ++it)
                {
                    const QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    if(currentChild->getOuterWidth() > maxWidth)
                    {
                        maxWidth = currentChild->getOuterWidth();
                        widestChild = currentChild;
                    }
                }
                group->x = widestChild->x;
                if (this->_layoutTick()){
                    return true;
                }
            }
            break;
        }

        case HSSDirectionRightToLeft:
        {
            if (group->lines.size() > 0)
            {
                //calculate the starting point
                HSSUnit widthsTotal = 0.;
                HSSUnit groupAlignmentTotal = 0.;
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = group->lines.begin(); it!= group->lines.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    HSSUnit alignmentTotal = 0.;
                    for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                    {
                        QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                        alignmentTotal += currentChild->getAlignX();
                    }
                    groupAlignmentTotal += alignmentTotal / line->objects->size();
                    widthsTotal += line->width;
                }
                HSSUnit groupAlignX = groupAlignmentTotal / group->lines.size();
                HSSUnit startX = groupAlignX - (widthsTotal/2);
                if (startX > (container.getWidth() - container.getRightPadding()) - widthsTotal) startX = (container.getWidth() - container.getRightPadding()) - widthsTotal;
                if (startX < container.getLeftPadding()) startX = container.getLeftPadding();

                //reposition each element
                HSSUnit accWidth = startX;
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_reverse_iterator it = group->lines.rbegin(); it!= group->lines.rend(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    line->x = accWidth;
                    for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                    {
                        QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                        this->_lineArrangeX(currentChild, accWidth, line->width);
                    }
                    accWidth += line->width;
                    if (this->_layoutTick()){
                        return true;
                    }
                }
            }
            else
            {
                HSSUnit maxWidth = 0.;
                QSharedPointer<HSSDisplayObject> widestChild;
                for (HSSSimpleSelection::iterator it = group->objects->begin(); it!= group->objects->end(); ++it)
                {
                    const QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    if(currentChild->getOuterWidth() > maxWidth)
                    {
                        maxWidth = currentChild->getOuterWidth();
                        widestChild = currentChild;
                    }
                }
                group->x = widestChild->x;
                if (this->_layoutTick()){
                    return true;
                }
            }
            break;
        }

        case HSSDirectionBottomToTop:
        {
            if (group->lines.size() > 0)
            {
                //calculate the starting point
                HSSUnit heightsTotal = 0.;
                HSSUnit groupAlignmentTotal = 0.;
                HSSUnit linesSize = group->lines.size();
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = group->lines.begin(); it!= group->lines.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    heightsTotal += line->height;
                }
                HSSUnit medianHeight = heightsTotal / linesSize;
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = group->lines.begin(); it!= group->lines.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    groupAlignmentTotal += line->getAlignY() * line->height / medianHeight;
                }
                HSSUnit groupAlignY = groupAlignmentTotal / linesSize;
                HSSUnit startY = groupAlignY - (heightsTotal/2);
                if (startY > (container.getHeight() - container.getBottomPadding()) - heightsTotal) startY = (container.getHeight() - container.getBottomPadding()) - heightsTotal;
                if (startY < container.getTopPadding()) startY = container.getTopPadding();

                //reposition each element
                HSSUnit accHeight = startY;
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_reverse_iterator it = group->lines.rbegin(); it!= group->lines.rend(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    line->y = accHeight;
                    for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                    {
                        QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                        this->_lineArrangeY(currentChild, accHeight, line->height);
                    }
                    accHeight += line->height;
                    if (this->_layoutTick()){
                        return true;
                    }
                }
                group->y = startY;
            }
            else
            {
                HSSUnit maxHeight = 0.;
                QSharedPointer<HSSDisplayObject> tallestChild;
                for (HSSSimpleSelection::iterator it = group->objects->begin(); it!= group->objects->end(); ++it)
                {
                    const QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    if(currentChild->getOuterHeight() > maxHeight)
                    {
                        maxHeight = currentChild->getOuterHeight();
                        tallestChild = currentChild;
                    }
                }
                group->y = tallestChild->y;

                if (this->_layoutTick()){
                    return true;
                }
            }
            break;
        }

        default:
        {
            if (group->lines.size() > 0)
            {
                //calculate the starting point
                HSSUnit heightsTotal = 0.;
                HSSUnit groupAlignmentTotal = 0.;
                HSSUnit linesSize = group->lines.size();
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = group->lines.begin(); it!= group->lines.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    heightsTotal += line->height;
                }
                HSSUnit medianHeight = heightsTotal / linesSize;
                if (medianHeight > 0)
                {
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = group->lines.begin(); it!= group->lines.end(); ++it)
                    {
                        const QSharedPointer<HSSLayoutLine> & line = *it;
                        groupAlignmentTotal += line->getAlignY() * line->height / medianHeight;
                    }
                }
                HSSUnit groupAlignY = groupAlignmentTotal / linesSize;
                HSSUnit startY = groupAlignY - (heightsTotal/2);
                if (startY > (container.getHeight() - container.getBottomPadding()) - heightsTotal) startY = (container.getHeight() - container.getBottomPadding()) - heightsTotal;
                if (startY < container.getTopPadding()) startY = container.getTopPadding();
                group->y = startY;

                //reposition each element
                HSSUnit accHeight = startY;
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = group->lines.begin(); it!= group->lines.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    line->y = accHeight;
                    if (line->lines.size() > 0)
                    {
                        for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it2 = line->lines.begin(); it2!= line->lines.end(); ++it2)
                        {
                            const QSharedPointer<HSSLayoutLine> & innerLine = *it2;
                            for (HSSSimpleSelection::iterator it3 = innerLine->objects->begin(); it3!= innerLine->objects->end(); ++it3)
                            {
                                QSharedPointer<HSSDisplayObject> & currentChild = *it3;
                                this->_lineArrangeY(currentChild, accHeight, innerLine->height);
                            }
                        }
                    }
                    else
                    {
                        for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                        {
                            QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                            this->_lineArrangeY(currentChild, accHeight, line->height);
                        }
                        accHeight += line->height;
                        if (this->_layoutTick()){
                            return true;
                        }
                    }
                }
            }
            else
            {
                HSSUnit groupHeight = group->height;
                HSSUnit startY = group->getAlignY() - (groupHeight/2);
                if (startY > (container.getHeight() - container.getBottomPadding()) - groupHeight) startY = (container.getHeight() - container.getBottomPadding()) - groupHeight;
                if (startY < container.getTopPadding()) startY = container.getTopPadding();
                for (HSSSimpleSelection::iterator it = group->objects->begin(); it!= group->objects->end(); ++it)
                {
                    const QSharedPointer<HSSDisplayObject> & currentChild = *it;
                    this->_lineArrangeY(currentChild, startY, group->height);
                }
                if (this->_layoutTick()){
                    return true;
                }
            }
            break;
        }
    }
    return false;
}

inline bool HSSLayout::_distributeLines(const HSSContainer & container, const QSharedPointer<HSSLayoutLine> &group, const HSSDirectionValue direction) const
{
    switch (direction)
    {
        case HSSDirectionLeftToRight:
        {
            if (group->lines.size() == 0)
            {
                HSSUnit newValue = ((container.getInnerWidth() - group->width) / 2);
                HSSUnit startX = container.getTopPadding() + newValue;

                for (HSSSimpleSelection::iterator it2 = group->objects->begin(); it2!= group->objects->end(); ++it2)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                    this->_lineArrangeX(currentChild, startX, group->width);
                }
                if (this->_layoutTick()){
                    return true;
                }
            }
            else
            {
                HSSUnit accWidth = container.getTopPadding();
                HSSUnit totalWidth = 0.;

                //calculate the total width of the group
                for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                {
                    totalWidth += (*it)->width;
                }

                if (container.distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = container.getInnerWidth()- totalWidth;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk;
                    if(remainingSpace < 0){
                        spaceChunk = 0;
                    } else {
                        spaceChunk = remainingSpace / (group->lines.size() - 1);
                    }
                    unsigned i = 0;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                    {
                        QSharedPointer<HSSLayoutLine> &line = *it;
                        line->x = accWidth + (spaceChunk * i);
                        for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                        {
                            QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                            this->_lineArrangeX(currentChild, line->x, line->width);
                        }
                        accWidth += line->width;
                        ++i;
                        if (this->_layoutTick()){
                            return true;
                        }
                    }
                    group->x = group->lines.front()->x;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = container.getInnerWidth() - totalWidth;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk;
                    if(remainingSpace < 0){
                        spaceChunk = 0;
                    } else {
                        spaceChunk = remainingSpace / (group->lines.size() + 1);
                    }
                    unsigned i = 0;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                    {
                        QSharedPointer<HSSLayoutLine> &line = *it;
                        line->y = accWidth + (spaceChunk * i) + spaceChunk;
                        for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                        {
                            QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                            this->_lineArrangeX(currentChild, line->y, line->width);
                        }
                        accWidth += line->width;
                        ++i;
                        if (this->_layoutTick()){
                            return true;
                        }
                    }
                    group->y = group->lines.front()->y;
                }
            }
            break;
        }

        case HSSDirectionRightToLeft:
        {
            if (group->lines.size() == 0)
            {
                HSSUnit newValue = ((container.getInnerWidth() - group->width) / 2);
                HSSUnit startX = container.getTopPadding() + newValue;

                for (HSSSimpleSelection::reverse_iterator it2 = group->objects->rbegin(); it2!= group->objects->rend(); ++it2)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                    this->_lineArrangeX(currentChild, startX, group->width);
                }
                if (this->_layoutTick()){
                    return true;
                }
            }
            else
            {
                HSSUnit accWidth = container.getTopPadding();
                HSSUnit totalWidth = 0.;

                //calculate the total width of the group
                for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                {
                    totalWidth += (*it)->width;
                }

                if (container.distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = container.getInnerWidth() - totalWidth;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk;
                    if(remainingSpace < 0){
                        spaceChunk = 0;
                    } else {
                        spaceChunk = remainingSpace / (group->lines.size() - 1);
                    }
                    unsigned i = 0;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::reverse_iterator it = group->lines.rbegin(); it != group->lines.rend(); ++it)
                    {
                        QSharedPointer<HSSLayoutLine> &line = *it;
                        line->x = accWidth + (spaceChunk * i);
                        for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                        {
                            QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                            this->_lineArrangeX(currentChild, line->x, line->width);
                        }
                        accWidth += line->width;
                        ++i;
                        if (this->_layoutTick()){
                            return true;
                        }
                    }
                    group->x = group->lines.front()->x;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = container.getInnerWidth() - totalWidth;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk;
                    if(remainingSpace < 0){
                        spaceChunk = 0;
                    } else {
                        spaceChunk = remainingSpace / (group->lines.size() + 1);
                    }
                    unsigned i = 0;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::reverse_iterator it = group->lines.rbegin(); it != group->lines.rend(); ++it)
                    {
                        QSharedPointer<HSSLayoutLine> &line = *it;
                        line->y = accWidth + (spaceChunk * i) + spaceChunk;
                        for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                        {
                            QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                            this->_lineArrangeX(currentChild, line->y, line->width);
                        }
                        accWidth += line->width;
                        ++i;
                        if (this->_layoutTick()){
                            return true;
                        }
                    }
                    group->y = group->lines.front()->y;
                }
            }
            break;
        }

        case HSSDirectionBottomToTop:
        {
            if (group->lines.size() == 0)
            {
                HSSUnit newValue = ((container.getInnerHeight() - group->height) / 2);
                HSSUnit startY = container.getTopPadding() + newValue;

                for (HSSSimpleSelection::reverse_iterator it2 = group->objects->rbegin(); it2!= group->objects->rend(); ++it2)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                    this->_lineArrangeY(currentChild, startY, group->height);
                }
                if (this->_layoutTick()){
                    return true;
                }
            }
            else
            {
                HSSUnit accHeight = container.getTopPadding();
                HSSUnit totalHeight = 0.;

                //calculate the total height of the group
                for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                {
                    totalHeight += (*it)->height;
                }

                if (container.distributeYLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = container.getInnerHeight() - totalHeight;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk;
                    if(remainingSpace < 0){
                        spaceChunk = 0;
                    } else {
                        spaceChunk = remainingSpace / (group->lines.size() - 1);
                    }
                    unsigned i = 0;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::reverse_iterator it = group->lines.rbegin(); it != group->lines.rend(); ++it)
                    {
                        QSharedPointer<HSSLayoutLine> &line = *it;
                        line->y = accHeight + (spaceChunk * i);
                        for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                        {
                            QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                            this->_lineArrangeY(currentChild, line->y, line->height);
                        }
                        accHeight += line->height;
                        ++i;
                        if (this->_layoutTick()){
                            return true;
                        }
                    }
                    group->y = group->lines.front()->y;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = container.getInnerHeight() - totalHeight;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk;
                    if(remainingSpace < 0){
                        spaceChunk = 0;
                    } else {
                        spaceChunk = remainingSpace / (group->lines.size() + 1);
                    }
                    unsigned i = 0;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::reverse_iterator it = group->lines.rbegin(); it != group->lines.rend(); ++it)
                    {
                        QSharedPointer<HSSLayoutLine> &line = *it;
                        line->y = accHeight + (spaceChunk * i) + spaceChunk;
                        for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                        {
                            QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                            this->_lineArrangeY(currentChild, line->y, line->height);
                        }
                        accHeight += line->height;
                        ++i;
                        if (this->_layoutTick()){
                            return true;
                        }
                    }
                    group->y = group->lines.front()->y;                }
            }
            break;
        }

        default:
        {
            if (group->lines.size() == 0)
            {
                HSSUnit newValue = ((container.getInnerHeight() - group->height) / 2);
                HSSUnit startY = container.getTopPadding() + newValue;

                for (HSSSimpleSelection::iterator it2 = group->objects->begin(); it2!= group->objects->end(); ++it2)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                    this->_lineArrangeY(currentChild, startY, group->height);
                }
                if (this->_layoutTick()){
                    return true;
                }
            }
            else
            {
                HSSUnit accHeight = container.getTopPadding();
                HSSUnit totalHeight = 0.;

                //calculate the total height of the group
                for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                {
                    totalHeight += (*it)->height;
                }

                if (container.distributeYLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = container.getInnerHeight() - totalHeight;
                    //divide it by the number of elements-1
                    HSSUnit spaceChunk;
                    if(remainingSpace < 0){
                        spaceChunk = 0;
                    } else {
                        spaceChunk = remainingSpace / (group->lines.size() - 1);
                    }
                    unsigned i = 0;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                    {
                        QSharedPointer<HSSLayoutLine> &line = *it;
                        line->y = accHeight + (spaceChunk * i);
                        for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                        {
                            QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                            this->_lineArrangeY(currentChild, line->y, line->height);
                        }
                        accHeight += line->height;
                        ++i;
                        if (this->_layoutTick()){
                            return true;
                        }
                    }
                    group->y = group->lines.front()->y;
                }
                else
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = container.getInnerHeight() - totalHeight;
                    //divide it by the number of elements+1
                    HSSUnit spaceChunk;
                    if(remainingSpace < 0){
                        spaceChunk = 0;
                    } else {
                        spaceChunk = remainingSpace / (group->lines.size() + 1);
                    }

                    unsigned i = 0;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                    {
                        QSharedPointer<HSSLayoutLine> &line = *it;
                        line->y = accHeight + (spaceChunk * i) + spaceChunk;
                        for (HSSSimpleSelection::iterator it2 = line->objects->begin(); it2!= line->objects->end(); ++it2)
                        {
                            QSharedPointer<HSSDisplayObject> & currentChild = *it2;
                            this->_lineArrangeY(currentChild, line->y, line->height);
                        }
                        accHeight += line->height;
                        ++i;
                        if (this->_layoutTick()){
                            return true;
                        }
                    }
                    group->y = group->lines.front()->y;
                }
            }
            break;
        }
    }
    return false;
}

inline HSSUnit HSSLayout::_getLineHeight(const QSharedPointer<HSSLayoutLine> & line) const
{
    HSSUnit ret = 0;
    for (HSSSimpleSelection::iterator it = line->objects->begin(); it!= line->objects->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & currentChild = *it;
        if(currentChild->hasOwnHeight() && currentChild->getOuterHeight() > ret){
            ret = currentChild->getOuterHeight();
        }
    }
    return ret;
}

void HSSLayout::visit(HSSContainer &container)
{
    if (!container._needsLayout) return;
    bool done = false;
    if (container.allChildren->empty()) return;
    //short path
    if (container.allChildren->size() == 1)
    {
        const QSharedPointer<HSSDisplayObject> & child = container.allChildren->front();
        if (container.heightByContent)
        {
            if(child->hasOwnHeight())
            {
                container.setHeight(child->getOuterHeight() + container.getTopPadding() + container.getBottomPadding());
            }
        }
        this->_placeOnAlignmentPoint(container, child);
        this->_setGlobalPosition(container, child);
        return;
    }

    bool primaryIsHorizontal = (container.getDirection() == HSSDirectionLeftToRight || container.getDirection() == HSSDirectionRightToLeft);
    bool needsHeightByContent = container.heightByContent;

    while (!done && container._needsLayout)
    {
        //we assume we don't need to re-layout
        container._needsLayout = false;

        for (HSSSimpleSelection::const_iterator it = container.allChildren->begin(); it!= container.allChildren->end(); ++it)
        {
            const QSharedPointer<HSSDisplayObject> & child = *it;
            child->x = 0;
            child->y = 0;
        }

        if (this->_layoutTick()){
            done = true;
        }

        std::vector<QSharedPointer<HSSLayoutLine> > groups;

        if (!done)
        {
            //place them on the alignment points and try to put them into groups
            for (HSSSimpleSelection::const_iterator it = container.allChildren->begin(); it!= container.allChildren->end(); ++it)
            {
                const QSharedPointer<HSSDisplayObject> & child = *it;

                this->_placeOnAlignmentPoint(container, child);
                //if the child is not in flow, we are done with it
                if(!child->getFlow())
                {
                    continue;
                }

                if (this->_layoutTick()){
                    done = true;
                    break;
                }

                //try to add it to a group
                QSharedPointer<HSSLayoutLine> targetGroup = this->_getTargetGroup(child, groups);
                if (targetGroup)
                {
                    //found one
                    targetGroup->add(child);
                }
                else
                {
                    //we'll create a new one
                    QSharedPointer<HSSLayoutLine> newGroup = QSharedPointer<HSSLayoutLine>(new HSSLayoutLine(child, container.getDirection(), &container));
                    groups.push_back(newGroup);
                }
            }
        }

        bool foundOverlaps = true;
        while (!done && foundOverlaps)
        {
            //reset value
            foundOverlaps = false;

            //arrange in the primary direction
            if (!done)
            {
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = groups.begin(); it!= groups.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & group = *it;

                    if (group->lines.size() > 0)
                    {
                        for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it2=group->lines.begin(); it2!=group->lines.end(); ++it2)
                        {
                            const QSharedPointer<HSSLayoutLine> & line = *it2;
                            if ((container.distributeX && primaryIsHorizontal) || (container.distributeY && !primaryIsHorizontal))
                            {
                                line->distribute();
                            }
                            else
                            {
                                line->arrange();
                            }

                            if (this->_layoutTick()){
                                done = true;
                                break;
                            }
                        }
                    }
                    else
                    {
                        if ((container.distributeX && primaryIsHorizontal) || (container.distributeY && !primaryIsHorizontal))
                        {
                            group->distribute();
                        }
                        else
                        {
                            group->arrange();
                        }
                        if (this->_layoutTick()){
                            done = true;
                            break;
                        }
                    }
                }
            }

            //arrange in the secondary direction
            if (!done) {
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = groups.begin(); it!= groups.end(); ++it)
                {
                    if(!done)
                    {
                        const QSharedPointer<HSSLayoutLine> & group = *it;
                        bool mustBreak = false;
                        if ((container.distributeY && primaryIsHorizontal) || (container.distributeX && !primaryIsHorizontal))
                        {
                            mustBreak = this->_distributeLines(container, group, container.getWrapDirection());
                        }
                        else
                        {
                            mustBreak = this->_arrangeLines(container, group, container.getWrapDirection());
                        }

                        if (mustBreak){
                            done = true;
                            break;
                        }
                    }
                }
            }

            if (!done)
            {
                //merge groups that overlap
                for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = groups.begin(); it!= groups.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & group = *it;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it2 = groups.begin(); it2!= groups.end(); ++it2)
                    {
                        const QSharedPointer<HSSLayoutLine> & otherGroup = *it2;
                        if(group != otherGroup && group->overlaps(otherGroup))
                        {
                            foundOverlaps = true;
                            group->mergeWith(otherGroup);
                            if (it2+1 == groups.end())
                            {
                                groups.erase(it2);
                                break;
                            }
                            else
                            {
                                groups.erase(it2);
                            }
                        }
                    }
                }
                if (foundOverlaps)
                {
                    continue;
                }
            }

            if (!done)
            {
                //handle height by content
                if (container.heightByContent)
                {
                    HSSUnit contentHeight = 0;
                    for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = groups.begin(); it!= groups.end(); ++it)
                    {
                        HSSUnit groupHeight = 0;
                        const QSharedPointer<HSSLayoutLine> & group = *it;
                        if (group->lines.size() > 0)
                        {
                            for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it2 = group->lines.begin(); it2!= group->lines.end(); ++it2)
                            {
                                groupHeight += this->_getLineHeight(*it2);
                            }
                        } else {
                            groupHeight = this->_getLineHeight(group);
                        }
                        if (groupHeight > contentHeight)
                        {
                            contentHeight = groupHeight;
                        }
                    }
                    container.setHeight(contentHeight + container.getTopPadding() + container.getBottomPadding());
                    if (needsHeightByContent)
                    {
                        container._needsLayout = true;
                        needsHeightByContent = false;
                    }
                }
            }
        }

        //assign global positions
        for (HSSSimpleSelection::const_iterator it=container.allChildren->begin(); it!=container.allChildren->end(); ++it)
        {
            const QSharedPointer<HSSDisplayObject> & child = *it;
            this->_setGlobalPosition(container, child);
        }
    }
}

void HSSLayout::visit(HSSTextBlock &textBlock)
{
    if (textBlock.needsLayout())
    {
        int flags = Qt::AlignLeft;
        switch (textBlock.getTextAlign())
        {
            case HSSTextAlignTypeLeft:
                flags = Qt::AlignLeft;
                break;
            case HSSTextAlignTypeRight:
                flags = Qt::AlignRight;
                break;
            case HSSTextAlignTypeCenter:
                flags = Qt::AlignCenter;
                break;
            case HSSTextAlignTypeJustify:
                flags = Qt::AlignJustify;
                break;
            default:
                break;
        }

        QFontMetrics fontMetrics(textBlock.getQFont());
        HSSUnit allowedWidth = textBlock.getParent()->getInnerWidth();
        AXRString text = textBlock.getText();
        QRect bounds = fontMetrics.boundingRect(0, 0, static_cast<int>(allowedWidth), std::numeric_limits<int>::max(), flags | Qt::TextWordWrap, text);

        textBlock.setWidth(bounds.width());
        textBlock.setHeight(bounds.height());
        textBlock.setNeedsSurface(true);

        textBlock._needsLayout = false;
    }
}

void HSSLayout::finalizeVisit()
{
    axr_log(LoggerChannelRendering, "FINALIZE_LAYOUT");
}
