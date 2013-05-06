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

#include <cmath>
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSAbstractVisitor.h"
#include "HSSBorder.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSLayoutLine.h"
#include "HSSKeywordConstant.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRectangle.h"
#include "HSSRoundedRect.h"
#include "HSSStringConstant.h"
#include "HSSTextBlock.h"

using namespace AXR;

Q_DECLARE_METATYPE(HSSUnit*)

QSharedPointer<HSSDisplayObject> HSSContainer::asDisplayObject(QSharedPointer<HSSContainer> theContainer)
{
    return qSharedPointerCast<HSSDisplayObject > (theContainer);
}

//always test the return of this function. E.g. "QSharedPointer<HSSContainer> cont = HSSContainer::asContainer(myDO); if(cont) { /* ... */ }

QSharedPointer<HSSContainer> HSSContainer::asContainer(QSharedPointer<HSSDisplayObject> theDisplayObject)
{
    QSharedPointer<HSSContainer> ret;
    if (theDisplayObject->isA(HSSObjectTypeContainer))
    {
        ret = qSharedPointerCast<HSSContainer > (theDisplayObject);
    }
    return ret;
}

HSSContainer::HSSContainer(AXRController * controller)
: HSSDisplayObject(HSSObjectTypeContainer, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSContainer: creating container");
    this->initialize();
}

void HSSContainer::initialize()
{
    this->contentText = AXRString();

    this->contentAlignX = this->contentAlignY = 0;
    this->directionPrimary = HSSDirectionLeftToRight;
    this->directionSecondary = HSSDirectionTopToBottom;

    this->observedContentAlignX = this->observedContentAlignY
            = this->observedDirectionPrimary = this->observedDirectionSecondary
            = this->observedShape = this->observedTextAlign
            = NULL;

    this->distributeX = distributeXLinear
            = this->distributeY = distributeYLinear
            = false;

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("isA");
    shorthandProperties.push_back("width");
    shorthandProperties.push_back("height");
    shorthandProperties.push_back("contentAlignX");
    shorthandProperties.push_back("contentAlignY");
    shorthandProperties.push_back("font");
    shorthandProperties.push_back("background");
    shorthandProperties.push_back("shape");
    shorthandProperties.push_back("textAlign");

    this->setShorthandProperties(shorthandProperties);

    this->registerProperty(HSSObservablePropertyContentAlignX, QVariant::fromValue(&this->contentAlignX));
    this->registerProperty(HSSObservablePropertyContentAlignY, QVariant::fromValue(&this->contentAlignY));
    this->registerProperty(HSSObservablePropertyDirectionPrimary, QVariant::fromValue(&this->directionPrimary));
    this->registerProperty(HSSObservablePropertyDirectionSecondary, QVariant::fromValue(&this->directionSecondary));
    this->registerProperty(HSSObservablePropertyShape, QVariant::fromValue(&this->shape));
    this->registerProperty(HSSObservablePropertyTextAlign, QVariant::fromValue(&this->textAlign));

    this->children = QSharedPointer<HSSSimpleSelection>(new HSSSimpleSelection());
    this->allChildren = QSharedPointer<HSSSimpleSelection>(new HSSSimpleSelection());
}

HSSContainer::HSSContainer(const HSSContainer & orig)
: HSSDisplayObject(orig)
{
    this->initialize();
}

QSharedPointer<HSSContainer> HSSContainer::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSContainer: cloning container");
    return qSharedPointerCast<HSSContainer> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSContainer::cloneImpl() const
{
    return QSharedPointer<HSSContainer>(new HSSContainer(*this));
}

HSSContainer::~HSSContainer()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSContainer: destructing container");
    if (this->children)
    {
        this->children->clear();
    }
    if (this->allChildren){
        this->allChildren->clear();
    }

    if (this->observedContentAlignX)
    {
        this->observedContentAlignX->removeObserver(this->observedContentAlignXProperty, HSSObservablePropertyAlignX, this);
    }
    if (this->observedContentAlignY)
    {
        this->observedContentAlignY->removeObserver(this->observedContentAlignYProperty, HSSObservablePropertyAlignY, this);
    }
    if (this->observedDirectionPrimary)
    {
        this->observedDirectionPrimary->removeObserver(this->observedDirectionPrimaryProperty, HSSObservablePropertyDirectionPrimary, this);
    }
    if (this->observedDirectionSecondary)
    {
        this->observedDirectionSecondary->removeObserver(this->observedDirectionSecondaryProperty, HSSObservablePropertyDirectionSecondary, this);
    }
    if (this->observedShape)
    {
        this->observedShape->removeObserver(this->observedShapeProperty, HSSObservablePropertyShape, this);
    }
}

AXRString HSSContainer::toString()
{
    AXRString tempstr;

    if (this->isNamed())
    {
        tempstr = AXRString("HSSContainer: ").append(this->name);
    }
    else
    {
        tempstr = "Anonymous HSSContainer";
    }

    if (this->attributes.size() > 0)
    {
        tempstr.append("\n").append("with the following attributes:");
        for (std::map<AXRString, AXRString>::iterator it = this->attributes.begin(); it != this->attributes.end(); ++it)
        {
            tempstr.append("\n").append("- ").append((*it).first).append(": ").append((*it).second);
        }
        tempstr.append("\n");
    }

    if (this->children->size() > 0)
    {
        tempstr.append("\n").append("with the following children objects:");
        for (HSSSimpleSelection::iterator it = this->children->begin(); it!= this->children->end(); ++it)
        {
            tempstr.append("\n").append((*it)->toString());
        }
        tempstr.append("\n");
    }

    return tempstr;
}

AXRString HSSContainer::defaultObjectType()
{
    return "container";
}

AXRString HSSContainer::defaultObjectType(AXRString property)
{
    if (property == "shape")
    {
        return "roundedRect";
    }
    else
    {
        return HSSDisplayObject::defaultObjectType(property);
    }
}

bool HSSContainer::isKeyword(AXRString value, AXRString property)
{
    if (property == "contentAlignX" || property == "contentAlignY")
    {
        if (value == "center"
                || value == "middle"
                || value == "top"
                || value == "bottom"
                || value == "left"
                || value == "right"
                || value == "even"
                || value == "justify")
        {
            return true;
        }
    }
    else if (property == "direction" || property == "wrapDirection")
    {
        if (value == "ltr"
                || value == "rtl"
                || value == "ttb"
                || value == "btt")
        {
            return true;
        }
    }
    else if (property == "textAlign")
    {
        if (value == "left"
                || value == "right"
                || value == "center"
                || value == "justify")
        {
            return true;
        }
    }

    //if we reached this far, let the superclass handle it
    return HSSDisplayObject::isKeyword(value, property);
}

void HSSContainer::add(QSharedPointer<HSSDisplayObject> child)
{
    QSharedPointer<HSSContainer> sharedThis = this->shared_from_this();
    child->setParent(sharedThis);
    axr_log(LoggerChannelGeneralSpecific, "HSSContainer: added child " + child->getElementName() + " to " + this->getElementName());
    child->setIndex(this->allChildren->size());
    if (!child->isA(HSSObjectTypeTextBlock))
    {
        this->children->add(child);
    }
    this->allChildren->add(child);
    this->notifyObservers(HSSObservablePropertyTreeChange, this);
}

void HSSContainer::remove(size_t index)
{
    HSSSimpleSelection::iterator it = this->children->begin();
    while (it != this->children->end())
    {
        QSharedPointer<HSSDisplayObject> child = *it;
        if (child->getIndex() == index)
        {
            it = this->children->erase(it);
        }
        else
        {
            ++it;
        }
    }

    HSSSimpleSelection::iterator it2 = this->allChildren->begin();
    while (it2 != this->allChildren->end())
    {
        QSharedPointer<HSSDisplayObject> child = *it2;
        if (child->getIndex() == index)
        {
            it2 = this->allChildren->erase(it2);
        }
        else
        {
            ++it2;
        }
    }

    this->resetChildrenIndexes();
    this->notifyObservers(HSSObservablePropertyTreeChange, this);
}

void HSSContainer::clear()
{
    this->children->clear();
    this->allChildren->clear();
}

void HSSContainer::resetChildrenIndexes()
{
    unsigned i = 0;
    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
    {
        (*it)->setIndex(i);
        ++i;
    }
}

void HSSContainer::setContentText(const AXRString &contextText)
{
    AXRString text = contextText.trimmed();

    AXRController * controller = this->getController();

    if (!text.isEmpty())
    {
        if (this->allChildren->empty())
        {
            QSharedPointer<HSSTextBlock> txtBlck = QSharedPointer<HSSTextBlock>(new HSSTextBlock(controller));
            txtBlck->setDText(QSharedPointer<HSSStringConstant>(new HSSStringConstant(text, controller)));
            this->add(txtBlck);
        }
        else
        {
            QSharedPointer<HSSDisplayObject> lastChild = this->allChildren->back();
            if (lastChild->isA(HSSObjectTypeTextBlock))
            {
                QSharedPointer<HSSTextBlock> textBlock = qSharedPointerCast<HSSTextBlock > (lastChild);
                textBlock->setDText(QSharedPointer<HSSStringConstant>(new HSSStringConstant(text, controller)));
            }
            else
            {
                QSharedPointer<HSSTextBlock> txtBlck = QSharedPointer<HSSTextBlock>(new HSSTextBlock(controller));
                txtBlck->setDText(QSharedPointer<HSSStringConstant>(new HSSStringConstant(text, controller)));
                this->add(txtBlck);
            }
        }
    }

    this->registerProperty(HSSObservablePropertyContent, QVariant::fromValue(text));
}

void HSSContainer::appendContentText(const AXRString &contextText)
{
    AXRString text = contextText.trimmed();

    if (!text.isEmpty())
    {
        AXRController * controller = this->getController();

        if (this->allChildren->empty())
        {
            QSharedPointer<HSSTextBlock> txtBlck = QSharedPointer<HSSTextBlock>(new HSSTextBlock(controller));
            txtBlck->setDText(QSharedPointer<HSSStringConstant>(new HSSStringConstant(text, controller)));
            this->add(txtBlck);
        }
        else
        {
            QSharedPointer<HSSDisplayObject> lastChild = this->allChildren->back();
            if (lastChild->isA(HSSObjectTypeTextBlock))
            {
                QSharedPointer<HSSTextBlock> textBlock = qSharedPointerCast<HSSTextBlock > (lastChild);
                textBlock->setDText(QSharedPointer<HSSStringConstant>(new HSSStringConstant(textBlock->getText() + " " + text, controller)));
            }
            else
            {
                QSharedPointer<HSSTextBlock> txtBlck = QSharedPointer<HSSTextBlock>(new HSSTextBlock(controller));
                txtBlck->setDText(QSharedPointer<HSSStringConstant>(new HSSStringConstant(text, controller)));
                this->add(txtBlck);
            }
        }
    }
}

AXRString HSSContainer::getContentText()
{
    axr_log(LoggerChannelObsolete0, "whateva");
    return "bla";
}

void HSSContainer::setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value)
{
    switch (name)
    {
    case HSSObservablePropertyContentAlignX:
        this->setDContentAlignX(value);
        break;
    case HSSObservablePropertyContentAlignY:
        this->setDContentAlignY(value);
        break;
    case HSSObservablePropertyDirectionPrimary:
        this->setDDirectionPrimary(value);
        break;
    case HSSObservablePropertyDirectionSecondary:
        this->setDDirectionSecondary(value);
        break;
    case HSSObservablePropertyShape:
        this->setDShape(value);
        break;

    case HSSObservablePropertyTextAlign:
        this->setDTextAlign(value);
        break;

    default:
        HSSDisplayObject::setProperty(name, value);
        break;
    }
}

void HSSContainer::recursiveReadDefinitionObjects()
{
    this->readDefinitionObjects();

    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
    {
        (*it)->recursiveReadDefinitionObjects();
    }
}

void HSSContainer::accept(HSSAbstractVisitor* visitor, bool traverse)
{
    visitor->visit(*this);

    if (!traverse)
        return;

    for (HSSSimpleSelection::iterator child = this->allChildren->begin(); child != this->allChildren->end(); ++child)
    {
        (*child)->accept(visitor, traverse);
    }
}


inline bool HSSContainer::_overlaps(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const
{
    return this->_overlaps_horizontal(childA, childB) && this->_overlaps_vertical(childA, childB);
}

inline bool HSSContainer::_overlaps_horizontal(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const
{
    if (
        (childA->x + childA->width + childA->rightMargin) > (childB->x - childB->leftMargin)
        && (childA->x - childA->leftMargin) < (childB->x + childB->width + childB->rightMargin)
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool HSSContainer::_overlaps_vertical(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const
{
    if (
        (childA->y + childA->height + childA->bottomMargin) > (childB->y - childB->topMargin)
        && (childA->y - childA->topMargin) < (childB->y + childB->height + childB->bottomMargin)
        )
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline void HSSContainer::_placeOnAlignmentPoint(const QSharedPointer<HSSDisplayObject> & child)
{
    if (child->_anchorXdefault)
    {
        child->x = child->alignX - (child->outerWidth / 2) + child->leftMargin;
    }
    else
    {
        child->x = child->alignX - child->anchorX;
    }

    if (child->_anchorYdefault)
    {
        child->y = child->alignY - (child->outerHeight / 2) + child->topMargin;
    }
    else
    {
        child->y = child->alignY - child->anchorY;
    }
    if (child->getContained())
    {
        //on the right side
        if ((child->x + child->width + child->rightMargin) > (this->width - this->rightPadding))
        {
            child->x = (this->width - this->rightPadding) - (child->width + child->rightMargin);
        }
        //on the left side
        if (child->x < child->leftMargin + this->leftPadding)
        {
            child->x = child->leftMargin + this->leftPadding;
        }
        //on the bottom
        if ((child->y + child->height + child->bottomMargin) > (this->height - this->bottomPadding))
        {
            child->y = (this->height - this->bottomPadding) - (child->height + child->bottomMargin);
        }
        //on the top
        if (child->y < child->topMargin + this->topPadding)
        {
            child->y = child->topMargin + this->topPadding;
        }
    }
}

inline bool HSSContainer::_layoutTick() const
{
    AXRDocument * document = this->getController()->document();
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

inline void HSSContainer::_setGlobalPosition(const QSharedPointer<HSSDisplayObject> & child) const
{
    child->setGlobalX(qRound(this->globalX + child->x));
    child->setGlobalY(qRound(this->globalY + child->y));
}

inline const QSharedPointer<HSSLayoutLine> HSSContainer::_getTargetGroup(const QSharedPointer<HSSDisplayObject> & child, std::vector<QSharedPointer<HSSLayoutLine> > & groups) const
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

inline void HSSContainer::_setPositions(const QSharedPointer<HSSLayoutLine> & group) const
{
    for (HSSSimpleSelection::const_iterator it=group->objects->begin(); it!=group->objects->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & child = *it;
        this->_setGlobalPosition(child);
    }
}

inline void HSSContainer::_lineArrangeX(const QSharedPointer<HSSDisplayObject> & child, HSSUnit startX, HSSUnit lineWidth) const
{
    child->x = (startX + ((lineWidth / 2) - (child->outerWidth / 2)) * child->lineAlign * 2) + child->leftMargin;
}

inline void HSSContainer::_lineArrangeY(const QSharedPointer<HSSDisplayObject> & child, HSSUnit startY, HSSUnit lineHeight) const
{
    child->y = (startY + ((lineHeight / 2) - (child->outerHeight / 2)) * child->lineAlign * 2) + child->topMargin;
}

inline bool HSSContainer::_arrangeLines(const QSharedPointer<HSSLayoutLine> &group, const HSSDirectionValue direction) const
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
                        alignmentTotal += currentChild->alignX;
                    }
                    groupAlignmentTotal += alignmentTotal / line->objects->size();
                    widthsTotal += line->width;
                }
                HSSUnit groupAlignX = groupAlignmentTotal / group->lines.size();
                HSSUnit startX = groupAlignX - (widthsTotal/2);
                if (startX > (this->width - this->rightPadding) - widthsTotal) startX = (this->width - this->rightPadding) - widthsTotal;
                if (startX < this->leftPadding) startX = this->leftPadding;

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
                    if(currentChild->outerWidth > maxWidth)
                    {
                        maxWidth = currentChild->outerWidth;
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
                        alignmentTotal += currentChild->alignX;
                    }
                    groupAlignmentTotal += alignmentTotal / line->objects->size();
                    widthsTotal += line->width;
                }
                HSSUnit groupAlignX = groupAlignmentTotal / group->lines.size();
                HSSUnit startX = groupAlignX - (widthsTotal/2);
                if (startX > (this->width - this->rightPadding) - widthsTotal) startX = (this->width - this->rightPadding) - widthsTotal;
                if (startX < this->leftPadding) startX = this->leftPadding;

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
                    if(currentChild->outerWidth > maxWidth)
                    {
                        maxWidth = currentChild->outerWidth;
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
                if (startY > (this->height - this->bottomPadding) - heightsTotal) startY = (this->height - this->bottomPadding) - heightsTotal;
                if (startY < this->topPadding) startY = this->topPadding;

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
                    if(currentChild->outerHeight > maxHeight)
                    {
                        maxHeight = currentChild->outerHeight;
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
                for (std::vector<QSharedPointer<HSSLayoutLine> >::const_iterator it = group->lines.begin(); it!= group->lines.end(); ++it)
                {
                    const QSharedPointer<HSSLayoutLine> & line = *it;
                    groupAlignmentTotal += line->getAlignY() * line->height / medianHeight;
                }
                HSSUnit groupAlignY = groupAlignmentTotal / linesSize;
                HSSUnit startY = groupAlignY - (heightsTotal/2);
                if (startY > (this->height - this->bottomPadding) - heightsTotal) startY = (this->height - this->bottomPadding) - heightsTotal;
                if (startY < this->topPadding) startY = this->topPadding;
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
                if (startY > (this->height - this->bottomPadding) - groupHeight) startY = (this->height - this->bottomPadding) - groupHeight;
                if (startY < this->topPadding) startY = this->topPadding;
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

inline bool HSSContainer::_distributeLines(const QSharedPointer<HSSLayoutLine> &group, const HSSDirectionValue direction) const
{
    switch (direction)
    {
        case HSSDirectionLeftToRight:
        {
            if (group->lines.size() == 0)
            {
                HSSUnit newValue = ((this->innerWidth - group->width) / 2);
                HSSUnit startX = this->topPadding + newValue;

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
                HSSUnit accWidth = this->topPadding;
                HSSUnit totalWidth = 0.;

                //calculate the total width of the group
                for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                {
                    totalWidth += (*it)->width;
                }

                if (this->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->innerWidth - totalWidth;
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
                    HSSUnit remainingSpace = this->innerWidth - totalWidth;
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
                HSSUnit newValue = ((this->innerWidth - group->width) / 2);
                HSSUnit startX = this->topPadding + newValue;

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
                HSSUnit accWidth = this->topPadding;
                HSSUnit totalWidth = 0.;

                //calculate the total width of the group
                for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                {
                    totalWidth += (*it)->width;
                }

                if (this->distributeXLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->innerWidth - totalWidth;
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
                    HSSUnit remainingSpace = this->innerWidth - totalWidth;
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
                HSSUnit newValue = ((this->innerHeight - group->height) / 2);
                HSSUnit startY = this->topPadding + newValue;

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
                HSSUnit accHeight = this->topPadding;
                HSSUnit totalHeight = 0.;

                //calculate the total height of the group
                for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                {
                    totalHeight += (*it)->height;
                }

                if (this->distributeYLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->innerHeight - totalHeight;
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
                    HSSUnit remainingSpace = this->innerHeight - totalHeight;
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
                HSSUnit newValue = ((this->innerHeight - group->height) / 2);
                HSSUnit startY = this->topPadding + newValue;

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
                HSSUnit accHeight = this->topPadding;
                HSSUnit totalHeight = 0.;

                //calculate the total height of the group
                for (std::vector<QSharedPointer<HSSLayoutLine> >::iterator it = group->lines.begin(); it != group->lines.end(); ++it)
                {
                    totalHeight += (*it)->height;
                }

                if (this->distributeYLinear)
                {
                    //now get the remaining space
                    HSSUnit remainingSpace = this->innerHeight - totalHeight;
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
                    HSSUnit remainingSpace = this->innerHeight - totalHeight;
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

void HSSContainer::layout()
{
    bool done = false;
    if (this->allChildren->empty()) return;

    bool primaryIsHorizontal = (this->directionPrimary == HSSDirectionLeftToRight || this->directionPrimary == HSSDirectionRightToLeft);
    while (!done && this->_needsLayout)
    {
        //we assume we don't need to re-layout
        this->_needsLayout = false;

        for (HSSSimpleSelection::const_iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
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
            for (HSSSimpleSelection::const_iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
            {
                const QSharedPointer<HSSDisplayObject> & child = *it;

                this->_placeOnAlignmentPoint(child);

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
                    QSharedPointer<HSSLayoutLine> newGroup = QSharedPointer<HSSLayoutLine>(new HSSLayoutLine(child, this->directionPrimary, this));
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
                            if ((this->distributeX && primaryIsHorizontal) || (this->distributeY && !primaryIsHorizontal))
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
                        if ((this->distributeX && primaryIsHorizontal) || (this->distributeY && !primaryIsHorizontal))
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
                        if ((this->distributeY && primaryIsHorizontal) || (this->distributeX && !primaryIsHorizontal))
                        {
                            mustBreak = this->_distributeLines(group, this->directionSecondary);
                        }
                        else
                        {
                            mustBreak = this->_arrangeLines(group, this->directionSecondary);
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
            }
        }

        //assign global positions
        for (HSSSimpleSelection::const_iterator it=this->allChildren->begin(); it!=this->allChildren->end(); ++it)
        {
            const QSharedPointer<HSSDisplayObject> & child = *it;
            this->_setGlobalPosition(child);
        }
    }
}

void HSSContainer::recursiveLayout()
{
    for (HSSSimpleSelection::const_iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
    {
        (*it)->recursiveLayout();
    }

    this->layout();
}

void HSSContainer::recursiveResetLayout()
{
    for (HSSSimpleSelection::const_iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
    {
        (*it)->recursiveResetLayout();
    }

    this->setNeedsLayout(true);
}

void HSSContainer::setGlobalX(HSSUnit newValue)
{
    HSSUnit delta = newValue - this->globalX;
    HSSDisplayObject::setGlobalX(newValue);

    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> theChild = *it;
        theChild->setGlobalX(theChild->globalX + delta);
    }
}

void HSSContainer::setGlobalY(HSSUnit newValue)
{
    HSSUnit delta = newValue - this->globalY;
    HSSDisplayObject::setGlobalY(newValue);

    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> theChild = *it;
        theChild->setGlobalY(theChild->globalY + delta);
    }
}

void HSSContainer::setChildren(QSharedPointer<HSSSimpleSelection> newChildren)
{
    this->children = newChildren;
    this->allChildren = newChildren;

    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
    {
        (*it)->setParent(this->shared_from_this());
    }
}

QSharedPointer<HSSSimpleSelection> HSSContainer::getChildren() const
{
    return this->getChildren(false);
}

QSharedPointer<HSSSimpleSelection> HSSContainer::getChildren(bool includeTextBlocks) const
{
    if (includeTextBlocks)
    {
        return this->allChildren;
    }
    else
    {
        return this->children;
    }
}

//contentAlignX

QSharedPointer<HSSParserNode> HSSContainer::getDContentAlignX()
{
    return this->dContentAlignX;
}

void HSSContainer::setDContentAlignX(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeKeywordConstant:
    case HSSParserNodeTypeFunctionCall:
        break;
    default:
        throw AXRWarning("HSSContainer", "Invalid value for contentAlignX of " + this->getElementName());
    }

    if (this->observedContentAlignX)
    {
        this->observedContentAlignX->removeObserver(this->observedContentAlignXProperty, HSSObservablePropertyAlignX, this);
        this->observedContentAlignX = NULL;
    }
    this->dContentAlignX = value;

    if (value->isA(HSSParserNodeTypeKeywordConstant))
    {
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "left")
        {
            this->setDContentAlignX(QSharedPointer<HSSParserNode>(new HSSNumberConstant(0, this->getController())));
        }
        else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center")
        {
            this->setDContentAlignX(QSharedPointer<HSSParserNode>(new HSSPercentageConstant(50, this->getController())));
        }
        else if (keywordValue->getValue() == "right")
        {
            this->setDContentAlignX(QSharedPointer<HSSParserNode>(new HSSPercentageConstant(100, this->getController())));
        }
        else if (keywordValue->getValue() == "even")
        {
            this->distributeX = true;
            this->distributeXLinear = false;
        }
        else if (keywordValue->getValue() == "justify")
        {
            this->distributeX = true;
            this->distributeXLinear = true;
        }
        else
        {
            throw AXRWarning("HSSContainer", "Invalid value for contentAlignX of " + this->getElementName());
        }

    }
    else
    {
        QSharedPointer<HSSContainer> parentContainer = this->getParent();
        QSharedPointer<HSSSimpleSelection> scope;
        if (parentContainer)
        {
            scope = parentContainer->getChildren();
        }
        else
        {
            scope = this->getChildren();
        }
        this->contentAlignX = this->_evaluatePropertyValue(
                                                   &HSSContainer::contentAlignXChanged,
                                                   value,
                                                   this->width,
                                                   HSSObservablePropertyWidth,
                                                   this,
                                                   HSSObservablePropertyAlignX,
                                                   this->observedContentAlignX,
                                                   this->observedContentAlignXProperty,
                                                   scope
                                                   );
        this->notifyObservers(HSSObservablePropertyContentAlignX, &this->contentAlignX);
    }
}

void HSSContainer::contentAlignXChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dContentAlignX->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dContentAlignX);
        this->contentAlignX = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (this->dContentAlignX);
        this->contentAlignX = expressionValue->evaluate();
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyContentAlignX, &this->contentAlignX);
}

//contentAlignY

QSharedPointer<HSSParserNode> HSSContainer::getDContentAlignY()
{
    return this->dContentAlignY;
}

void HSSContainer::setDContentAlignY(QSharedPointer<HSSParserNode> value)
{
    switch (value->getType())
    {
    case HSSParserNodeTypeNumberConstant:
    case HSSParserNodeTypePercentageConstant:
    case HSSParserNodeTypeExpression:
    case HSSParserNodeTypeKeywordConstant:
    case HSSParserNodeTypeFunctionCall:
        break;
    default:
        throw AXRWarning("HSSContainer", "Invalid value for contentAlignY of " + this->getElementName());
    }

    if (this->observedContentAlignY)
    {
        this->observedContentAlignY->removeObserver(this->observedContentAlignYProperty, HSSObservablePropertyAlignY, this);
        this->observedContentAlignY = NULL;
    }
    this->dContentAlignY = value;


    if (value->isA(HSSParserNodeTypeKeywordConstant))
    {
        QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant > (value);
        if (keywordValue->getValue() == "top")
        {
            this->setDContentAlignY(QSharedPointer<HSSParserNode>(new HSSNumberConstant(0, this->getController())));
        }
        else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center")
        {
            this->setDContentAlignY(QSharedPointer<HSSParserNode>(new HSSPercentageConstant(50, this->getController())));
        }
        else if (keywordValue->getValue() == "bottom")
        {
            this->setDContentAlignY(QSharedPointer<HSSParserNode>(new HSSPercentageConstant(100, this->getController())));
        }
        else if (keywordValue->getValue() == "even")
        {
            this->distributeY = true;
            this->distributeYLinear = false;
        }
        else if (keywordValue->getValue() == "justify")
        {
            this->distributeY = true;
            this->distributeYLinear = true;
        }
        else
        {
            throw AXRWarning("HSSContainer", "Invalid value for contentAlignY of " + this->getElementName());
        }
    }
    else
    {
        HSSObservableProperty observedProperty = HSSObservablePropertyHeight;

        QSharedPointer<HSSContainer> parentContainer = this->getParent();
        QSharedPointer<HSSSimpleSelection> scope;
        if (parentContainer)
        {
            scope = parentContainer->getChildren();
        }
        this->contentAlignY = this->_evaluatePropertyValue(
                                                   &HSSContainer::contentAlignYChanged,
                                                   value,
                                                   this->height,
                                                   observedProperty,
                                                   this,
                                                   HSSObservablePropertyAlignY,
                                                   this->observedContentAlignY,
                                                   this->observedContentAlignYProperty,
                                                   scope
                                                   );
        this->notifyObservers(HSSObservablePropertyContentAlignY, &this->contentAlignY);
    }
}

void HSSContainer::contentAlignYChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dContentAlignY->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (this->dContentAlignY);
        this->contentAlignY = percentageValue->getValue(*(HSSUnit*) data);
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (this->dContentAlignY);
        this->contentAlignY = expressionValue->evaluate();
        break;
    }

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyContentAlignY, &this->contentAlignY);
}


//directionPrimary

QSharedPointer<HSSParserNode> HSSContainer::getDDirectionPrimary()
{
    return this->dDirectionPrimary;
}

void HSSContainer::setDDirectionPrimary(QSharedPointer<HSSParserNode> value)
{
    this->dDirectionPrimary = value;
    if (this->observedDirectionPrimary)
    {
        this->observedDirectionPrimary->removeObserver(this->observedDirectionPrimaryProperty, HSSObservablePropertyDirectionPrimary, this);
    }

    if (value->isA(HSSParserNodeTypeKeywordConstant))
    {
        AXRString stringValue = qSharedPointerCast<HSSKeywordConstant > (value)->getValue();
        if (stringValue == "ltr" || stringValue == "rtl")
        {
            if (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft)
            {
                this->directionSecondary = HSSDirectionTopToBottom;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if (stringValue == "ltr")
            {
                this->directionPrimary = HSSDirectionLeftToRight;
            }
            else
            {
                this->directionPrimary = HSSDirectionRightToLeft;
            }

        }
        else if (stringValue == "ttb" || stringValue == "btt")
        {
            if (this->directionSecondary == HSSDirectionTopToBottom || this->directionSecondary == HSSDirectionBottomToTop)
            {
                this->directionSecondary = HSSDirectionLeftToRight;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if (stringValue == "ttb")
            {
                this->directionPrimary = HSSDirectionTopToBottom;
            }
            else
            {
                this->directionPrimary = HSSDirectionBottomToTop;
            }
        }
    }

    this->notifyObservers(HSSObservablePropertyDirectionPrimary, &this->directionPrimary);
}

void HSSContainer::directionPrimaryChanged(HSSObservableProperty source, void *data)
{
    if (this->dDirectionPrimary->isA(HSSParserNodeTypeKeywordConstant))
    {

    }

    this->notifyObservers(HSSObservablePropertyDirectionPrimary, &this->directionPrimary);
}

//directionSecondary

QSharedPointer<HSSParserNode> HSSContainer::getDDirectionSecondary()
{
    return this->dDirectionSecondary;
}

void HSSContainer::setDDirectionSecondary(QSharedPointer<HSSParserNode> value)
{
    this->dDirectionSecondary = value;
    if (this->observedDirectionSecondary)
    {
        this->observedDirectionSecondary->removeObserver(this->observedDirectionSecondaryProperty, HSSObservablePropertyDirectionSecondary, this);
    }

    if (value->isA(HSSParserNodeTypeKeywordConstant))
    {
        AXRString stringValue = qSharedPointerCast<HSSKeywordConstant > (value)->getValue();
        if (stringValue == "ltr" || stringValue == "rtl")
        {
            if (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft)
            {
                this->directionSecondary = HSSDirectionTopToBottom;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if (stringValue == "ltr")
            {
                this->directionSecondary = HSSDirectionLeftToRight;
            }
            else
            {
                this->directionSecondary = HSSDirectionRightToLeft;
            }
        }
        else if (stringValue == "ttb" || stringValue == "btt")
        {
            if (this->directionSecondary == HSSDirectionTopToBottom || this->directionSecondary == HSSDirectionBottomToTop)
            {
                this->directionSecondary = HSSDirectionLeftToRight;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if (stringValue == "ttb")
            {
                this->directionSecondary = HSSDirectionTopToBottom;
            }
            else
            {
                this->directionSecondary = HSSDirectionBottomToTop;
            }
        }
    }

    this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
}

void HSSContainer::directionSecondaryChanged(HSSObservableProperty source, void *data)
{
    if (this->dDirectionSecondary->isA(HSSParserNodeTypeKeywordConstant))
    {

    }

    this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
}


//shape

QSharedPointer<HSSShape> HSSContainer::getShape()
{
    return this->shape;
}

QSharedPointer<HSSParserNode> HSSContainer::getDShape()
{
    return this->dShape;
}

void HSSContainer::setDShape(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    this->dShape = value;
    if (this->observedShape)
    {
        this->observedShape->removeObserver(this->observedShapeProperty, HSSObservablePropertyShape, this);
    }

    switch (value->getType())
    {
    case HSSParserNodeTypeKeywordConstant:
    {
        AXRString stringValue = qSharedPointerCast<HSSKeywordConstant > (value)->getValue();
        if (stringValue == "default")
        {
            this->shape = QSharedPointer<HSSRectangle>(new HSSRectangle(this->getController()));
            valid = true;
        }
        break;
    }

    case HSSParserNodeTypeObjectNameConstant:
    {
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());
            objdef->setThisObj(this->shared_from_this());
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                objdef->setScope(parent->getChildren());
            }
            else
            {
                objdef->setScope(this->getChildren());
            }
            objdef->apply();
            QSharedPointer<HSSObject> theObject = objdef->getObject();
            if (theObject->isA(HSSObjectTypeShape))
            {
                this->shape = qSharedPointerCast<HSSShape > (theObject);
                valid = true;
            }

        }
        catch (const AXRError &e)
        {
            e.raise();

        }

        break;
    }

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else
            {
                fnct->setScope(this->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<QSharedPointer<HSSShape>* >())
            {
                this->shape = *remoteValue.value<QSharedPointer<HSSShape>* >();
                valid = true;
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyTextAlign, this, new HSSValueChangedCallback<HSSContainer > (this, &HSSContainer::textAlignChanged));
        }

        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        objdef->setThisObj(this->shared_from_this());
        QSharedPointer<HSSContainer> parent = this->getParent();
        if (parent)
        {
            objdef->setScope(parent->getChildren());
        }
        else
        {
            objdef->setScope(this->getChildren());
        }
        objdef->apply();
        QSharedPointer<HSSObject> objValue = objdef->getObject();
        if (objValue->isA(HSSObjectTypeShape))
        {
            this->shape = qSharedPointerCast<HSSShape > (objValue);
            valid = true;
        }
        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDisplayObject", "Invalid value for shape of " + this->getElementName());

    this->notifyObservers(HSSObservablePropertyShape, &this->shape);
}

void HSSContainer::shapeChanged(HSSObservableProperty source, void *data)
{
    this->setDirty(true);
    this->shape = *(QSharedPointer<HSSShape> *)data;
    this->notifyObservers(HSSObservablePropertyShape, &this->shape);
}

HSSTextAlignType HSSContainer::getTextAlign()
{
    return this->textAlign;
}

QSharedPointer<HSSParserNode> HSSContainer::getDTextAlign()
{
    return this->dTextAlign;
}

void HSSContainer::setDTextAlign(QSharedPointer<HSSParserNode> value)
{
    bool valid = false;

    switch (value->getType())
    {

    case HSSParserNodeTypeObjectNameConstant:
    {
        this->dTextAlign = value;
        try
        {
            QSharedPointer<HSSObjectNameConstant> objname = qSharedPointerCast<HSSObjectNameConstant > (value);
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeNodeNamed(objname->getValue());
            this->setDTextAlign(objdef);
            valid = true;
        }
        catch (const AXRError &e)
        {
            e.raise();
        }

        break;
    }


    case HSSParserNodeTypeFunctionCall:
    {
        this->dTextAlign = value;
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        if (fnct && fnct->isA(HSSFunctionTypeRef))
        {
            QSharedPointer<HSSContainer> parent = this->getParent();
            if (parent)
            {
                fnct->setScope(parent->getChildren());
            }
            else
            {
                fnct->setScope(this->getChildren());
            }
            fnct->setThisObj(this->shared_from_this());
            QVariant remoteValue = fnct->evaluate();
            if (remoteValue.canConvert<HSSTextAlignType>())
            {
                this->textAlign = remoteValue.value<HSSTextAlignType>();
                valid = true;
            }

            fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyTextAlign, this, new HSSValueChangedCallback<HSSContainer > (this, &HSSContainer::textAlignChanged));
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
    {
        this->dTextAlign = value;
        this->textAlign = HSSTextBlock::textAlignTypeFromString(qSharedPointerCast<HSSKeywordConstant > (value)->getValue());
        valid = true;
        break;
    }

    default:
        break;
    }

    switch (value->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
    {
        this->dTextAlign = value;
        QSharedPointer<HSSObjectDefinition> objdef = qSharedPointerCast<HSSObjectDefinition > (value);
        objdef->setThisObj(this->shared_from_this());
        QSharedPointer<HSSContainer> parent = this->getParent();
        if (parent)
        {
            objdef->setScope(parent->getChildren());
        }
        else
        {
            objdef->setScope(this->getChildren());
        }

        objdef->apply();
        QSharedPointer<HSSObject> theobj = objdef->getObject();
        if (theobj && theobj->isA(HSSObjectTypeValue))
        {
            //this->textAlign = HSSTextBlock::textAlignTypeFromString(qSharedPointerCast<HSSValue>(theobj)->getStringValue());
            axr_log(LoggerChannelObsolete0, "######## FIXME ################");
            valid = true;
        }

        break;
    }

    default:
        break;
    }

    if (!valid)
        throw AXRWarning("HSSDGradient", "Invalid value for textAlign of " + this->name);

    this->notifyObservers(HSSObservablePropertyTextAlign, &this->textAlign);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSContainer::textAlignChanged(HSSObservableProperty source, void *data)
{
    switch (this->dTextAlign->getType())
    {
    case HSSParserNodeTypeFunctionCall:
        this->textAlign = *(HSSTextAlignType *) data;
        break;

    default:
        break;
    }

    switch (this->dTextAlign->getStatementType())
    {
    case HSSStatementTypeObjectDefinition:
        this->textAlign = *(HSSTextAlignType *) data;
        break;

    default:
        break;
    }

    this->notifyObservers(HSSObservablePropertyTextAlign, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSContainer::setDefaults()
{
    HSSDisplayObject::setDefaults();
    AXRController * controller = this->getController();

    //contentAlignX
    QSharedPointer<HSSKeywordConstant> newDContentAlignX(new HSSKeywordConstant("left", controller));
    this->setDContentAlignX(newDContentAlignX);
    //contentAlignY
    QSharedPointer<HSSKeywordConstant> newDContentAlignY(new HSSKeywordConstant("top", controller));
    this->setDContentAlignY(newDContentAlignY);
    //directionPrimary
    QSharedPointer<HSSKeywordConstant> newDDirectionPrimary(new HSSKeywordConstant("ltr", controller));
    this->setDDirectionPrimary(newDDirectionPrimary);
    //directionSecondary
    QSharedPointer<HSSKeywordConstant> newDDirectionSecondary(new HSSKeywordConstant("ttb", controller));
    this->setDDirectionSecondary(newDDirectionSecondary);
    //shape
    QSharedPointer<HSSKeywordConstant> newDShape(new HSSKeywordConstant("default", controller));
    this->setDShape(newDShape);
    //textAlign
    this->setDTextAlign(QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant("left", controller)));
}

HSSUnit HSSContainer::_evaluatePropertyValue(
                                         void(HSSContainer::*callback)(HSSObservableProperty property, void* data),
                                         QSharedPointer<HSSParserNode> value,
                                         HSSUnit percentageBase,
                                         HSSObservableProperty observedProperty,
                                         HSSObservable * observedObject,
                                         HSSObservableProperty observedSourceProperty,
                                         HSSObservable * &observedStore,
                                         HSSObservableProperty &observedStoreProperty,
                                         QSharedPointer<HSSSimpleSelection> scope
                                         )
{
    HSSUnit ret = 0;

    HSSParserNodeType nodeType = value->getType();
    switch (nodeType)
    {
    case HSSParserNodeTypeNumberConstant:
    {
        QSharedPointer<HSSNumberConstant> numberValue = qSharedPointerCast<HSSNumberConstant > (value);
        ret = numberValue->getValue();
        break;
    }

    case HSSParserNodeTypePercentageConstant:
    {
        QSharedPointer<HSSPercentageConstant> percentageValue = qSharedPointerCast<HSSPercentageConstant > (value);
        ret = percentageValue->getValue(percentageBase);
        if (callback)
        {
            observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSContainer > (this, callback));
            observedStore = observedObject;
            observedStoreProperty = observedProperty;
        }
        break;
    }

    case HSSParserNodeTypeExpression:
    {
        QSharedPointer<HSSExpression> expressionValue = qSharedPointerCast<HSSExpression > (value);
        expressionValue->setPercentageBase(percentageBase);
        expressionValue->setPercentageObserved(observedProperty, observedObject);
        expressionValue->setScope(scope);
        ret = expressionValue->evaluate();
        if (callback)
        {
            expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSContainer > (this, callback));
            observedStore = expressionValue.data();
            observedStoreProperty = HSSObservablePropertyValue;
        }

        break;
    }

    case HSSParserNodeTypeKeywordConstant:
        break;

    case HSSParserNodeTypeFunctionCall:
    {
        QSharedPointer<HSSFunction> fnct = qSharedPointerCast<HSSFunction > (value)->clone();
        fnct->setPercentageBase(percentageBase);
        fnct->setPercentageObserved(observedProperty, observedObject);
        fnct->setScope(scope);
        fnct->setThisObj(this->shared_from_this());
        QVariant remoteValue = fnct->evaluate();
        if (remoteValue.canConvert<HSSUnit*>())
        {
            ret = *remoteValue.value<HSSUnit*>();
        }
        else
        {
            ret = 0.;
        }
        if (callback)
        {
            fnct->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSContainer > (this, callback));
            observedStore = fnct.data();
            observedStoreProperty = HSSObservablePropertyValue;
        }
        break;
    }

    default:
        throw AXRError("HSSContainer", "Unknown parser node type while setting LDProperty");
    }

    return ret;
}

bool HSSContainer::handleEvent(HSSInputEvent *event)
{
    bool handled = false;
    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it < this->allChildren->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> child = *it;
        bool childHandled = child->handleEvent(event);
        if (childHandled)
        {
            handled = true;
        }
    }

    bool superHandled = HSSDisplayObject::handleEvent(event);
    if (superHandled)
    {
        handled = true;
    }

    return handled;
}

void HSSContainer::setController(AXRController * controller)
{
    //propagate
    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it < this->allChildren->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> child = *it;
        child->setController(controller);
    }

    HSSDisplayObject::setController(controller);
}

QSharedPointer<HSSContainer> HSSContainer::shared_from_this()
{
    return qSharedPointerCast<HSSContainer > (HSSDisplayObject::shared_from_this());
}
