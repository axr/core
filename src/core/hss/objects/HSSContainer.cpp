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
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include <cmath>
#include <QPainter>
#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRRender.h"
#include "AXRWarning.h"
#include "HSSBorder.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSKeywordConstant.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRectangle.h"
#include "HSSRoundedRect.h"
#include "HSSSimpleSelection.h"
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
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSContainer: creating container");
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
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSContainer: cloning container");
    return qSharedPointerCast<HSSContainer> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSContainer::cloneImpl() const
{
    return QSharedPointer<HSSContainer>(new HSSContainer(*this));
}

HSSContainer::~HSSContainer()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSContainer: destructing container");
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
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSContainer: added child " + child->getElementName() + " to " + this->getElementName());
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
    for (HSSSimpleSelection::iterator it = this->children->begin(); it != this->children->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> child = *it;
        if (child->getIndex() == index)
        {
            HSSSimpleSelection::iterator delIt = it;
            this->children->erase(delIt);
        }
    }

    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it != this->allChildren->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> child = *it;
        if (child->getIndex() == index)
        {
            HSSSimpleSelection::iterator delIt = it;
            this->allChildren->erase(delIt);
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
    std_log("whateva");
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

void HSSContainer::recursiveRegenerateSurfaces(bool force)
{
    this->regenerateSurfaces(force);

    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
    {
        (*it)->recursiveRegenerateSurfaces(force);
    }
}

void HSSContainer::recursiveDraw(QPainter &painter)
{
    this->draw(painter);

    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
    {
        (*it)->recursiveDraw(painter);
    }
}

void HSSContainer::drawBackground()
{
    this->backgroundSurface->fill(Qt::transparent);

    QPainter painter(this->backgroundSurface);
    if (this->getController()->document()->getRender()->globalAntialiasingEnabled())
        painter.setRenderHint(QPainter::Antialiasing);

    QPainterPath path;
    this->shape->createPath(path, 0, 0, this->width, this->height);
    HSSDisplayObject::_drawBackground(painter, path);
}

void HSSContainer::drawBorders()
{
    this->bordersSurface->fill(Qt::transparent);

    QPainter painter(this->bordersSurface);
    if (this->getController()->document()->getRender()->globalAntialiasingEnabled())
        painter.setRenderHint(QPainter::Antialiasing);
    this->getShape()->drawBorders(painter, this->border, this->width, this->height, this->borderBleeding);

//    // Calculate the combined thickness of all borders
//    HSSUnit combinedThickness = 0;
//    for (HSSBorder::it it = this->border.begin(); it != this->border.end(); ++it)
//    {
//        combinedThickness += (*it)->getSize();
//    }
//
//    // Correction if needed
//    HSSUnit correction;
//    if ((int) combinedThickness % 2)
//    {
//        correction = 0.5;
//    }
//
//    // Cumulative combined thickness
//    HSSUnit cumulativeThickness = 0;
//
//    // Use a temporary trick for not having path offsets yet (will be fixed in the future)
//    bool isRoundedRect = this->shape->isA(HSSShapeTypeRoundedRect);
//
//    // Draw all borders
//    for (HSSBorder::it it = this->border.begin(); it != this->border.end(); ++it)
//    {
//        QSharedPointer<HSSBorder> theBorder = *it;
//        HSSUnit theSize = theBorder->getSize();
//
//        HSSUnit offset = (combinedThickness / 2) - cumulativeThickness - (theSize / 2) + correction;
//
//        QPainterPath path;
//        if(isRoundedRect){
//            QSharedPointer<HSSRoundedRect> roundedRect = qSharedPointerCast<HSSRoundedRect>(this->shape);
//            roundedRect->createRoundedRect(path, this->borderBleeding + offset, this->borderBleeding + offset, this->width - offset * 2, this->height - offset * 2, -offset*2);
//        } else {
//            this->shape->createPath(path, this->borderBleeding + offset, this->borderBleeding + offset, this->width - offset * 2, this->height - offset * 2);
//        }
//        theBorder->draw(painter, path);
//
//        cumulativeThickness += theSize;
//    }
}

void HSSContainer::layout()
{
    bool done = false;
    if (this->allChildren->empty()) done = true;

    while (this->_needsLayout)
    {
        //we assume we don't need to re-layout
        this->_needsLayout = false;

        if (!done)
        {
            std::vector<displayGroup::p>primaryGroups;
            std::vector<displayGroup::p>secondaryGroups;
            size_t i = 0;
            size_t size;
            size_t j = 0;
            size_t k = 0;
            //HSSUnit acc2 = 0;
            security_brake_init();
            AXRDocument * document = this->getController()->document();

            //bool secondaryIsHorizontal = (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft);

            //create groups and lines
            for (HSSSimpleSelection::const_iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
            {
                if (!done)
                {
                    const QSharedPointer<HSSDisplayObject> & child = *it;
                    //place it on the alignment point
                    //horizontal
                    if (child->_anchorXdefault)
                    {
                        child->x = child->alignX - (child->outerWidth / 2) + child->leftMargin;
                    }
                    else
                    {
                        child->x = child->alignX - child->anchorX;
                    }

                    //vertical
                    if (child->_anchorXdefault)
                    {
                        child->y = child->alignY - (child->outerHeight / 2) + child->topMargin;
                    }
                    else
                    {
                        child->y = child->alignY - child->anchorY;
                    }

                    if (child->getContained())
                    {
                        if ((child->x + child->width + child->rightMargin) > (this->width - this->rightPadding)) child->x = (this->width - this->rightPadding) - (child->width + child->rightMargin);
                        if (child->x < child->leftMargin + this->leftPadding) child->x = child->leftMargin + this->leftPadding;

                        if ((child->y + child->height + child->bottomMargin) > (this->height - this->bottomPadding)) child->y = (this->height - this->bottomPadding) - (child->height + child->bottomMargin);
                        if (child->y < child->topMargin + this->topPadding) child->y = child->topMargin + this->topPadding;
                    }

                    if (document->showLayoutSteps())
                    {
                        document->nextLayoutTick();
                        document->nextLayoutChild();
                        document->breakIfNeeded();
                        if (document->layoutStepDone())
                        {
                            done = true;
                            break;
                        }
                    }

                    if (!done)
                    {
                        if (child->getFlow() == true)
                        {
                            bool addedToGroup = false;

                            if (i != 0)
                            {
                                j = 0;
                                while (j < primaryGroups.size() && done == false)
                                {
                                    if (primaryGroups[j]->lines.empty())
                                    {
                                        displayGroup::p currentPGroup = primaryGroups[j];
                                        addedToGroup = this->_addChildToGroupIfNeeded(child, currentPGroup, this->directionPrimary, true);
                                        if (!addedToGroup && currentPGroup->complete)
                                        {
                                            //transform the current group into a line
                                            displayGroup::p newGroup = displayGroup::p(new displayGroup());
                                            newGroup->x = currentPGroup->x;
                                            newGroup->y = currentPGroup->y;
                                            newGroup->width = currentPGroup->width;
                                            newGroup->height = currentPGroup->height;
                                            newGroup->complete = false;
                                            newGroup->lines.push_back(currentPGroup);

                                            displayGroup::p newLine = displayGroup::p(new displayGroup());
                                            newLine->x = child->x - child->leftMargin;
                                            newLine->y = child->y - child->topMargin;
                                            newLine->width = child->outerWidth;
                                            newLine->height = child->outerHeight;
                                            newLine->complete = false;
                                            newLine->objects.add(child);
                                            newGroup->lines.push_back(newLine);

                                            primaryGroups[j] = newGroup;

                                            addedToGroup = true;
                                        }

                                        if (addedToGroup)
                                        {
                                            k = 0;
                                            while (k < primaryGroups.size())
                                            {
                                                if (k != j)
                                                {
                                                    displayGroup::p & otherPGroup = primaryGroups[k];
                                                    bool merged = this->_mergeGroupsIfNeeded(otherPGroup, currentPGroup, this->directionPrimary);
                                                    if (merged)
                                                    {
                                                        primaryGroups.erase(primaryGroups.begin() + j);
                                                        j = k;
                                                    }
                                                    else
                                                    {
                                                        ++k;
                                                    }
                                                }
                                                else
                                                {
                                                    ++k;
                                                }
                                            }
                                        }

                                        if (document->showLayoutSteps())
                                        {
                                            document->nextLayoutTick();
                                            document->breakIfNeeded();
                                            if (document->layoutStepDone())
                                            {
                                                done = true;
                                                break;
                                            }
                                        }

                                    }
                                    else
                                    {
                                        displayGroup::p & currentPGroup = primaryGroups[j]->lines.back();
                                        addedToGroup = this->_addChildToGroupIfNeeded(child, currentPGroup, this->directionPrimary, true);
                                        if (!addedToGroup && currentPGroup->complete)
                                        {
                                            //create new line
                                            displayGroup::p newLine = displayGroup::p(new displayGroup());
                                            newLine->x = child->x - child->leftMargin;
                                            newLine->y = child->y - child->topMargin;
                                            newLine->width = child->outerWidth;
                                            newLine->height = child->outerHeight;
                                            newLine->complete = false;
                                            newLine->objects.add(child);
                                            primaryGroups[j]->lines.push_back(newLine);

                                            addedToGroup = true;
                                        }

                                        if (addedToGroup)
                                        {
                                            k = 0;
                                            while (k < primaryGroups.size())
                                            {
                                                if (k != j)
                                                {
                                                    displayGroup::p & otherPGroup = primaryGroups[k];
                                                    bool merged = this->_mergeGroupsIfNeeded(otherPGroup, currentPGroup, this->directionPrimary);
                                                    if (merged)
                                                    {
                                                        primaryGroups.erase(primaryGroups.begin() + j);
                                                        j = k;
                                                    }
                                                    else
                                                    {
                                                        ++k;
                                                    }
                                                }
                                                else
                                                {
                                                    ++k;
                                                }
                                            }
                                        }

                                        if (document->showLayoutSteps())
                                        {
                                            document->nextLayoutTick();
                                            document->breakIfNeeded();
                                            if (document->layoutStepDone())
                                            {
                                                done = true;
                                                break;
                                            }
                                        }
                                    }

                                    ++j;
                                    security_brake();
                                }
                            }

                            if (!addedToGroup)
                            {
                                displayGroup::p newGroup = displayGroup::p(new displayGroup());
                                newGroup->x = child->x - child->leftMargin;
                                newGroup->y = child->y - child->topMargin;
                                newGroup->width = child->outerWidth;
                                newGroup->height = child->outerHeight;
                                newGroup->complete = false;
                                newGroup->objects.add(child);
                                primaryGroups.push_back(newGroup);
                            }
                        }


                    }
                }
                else
                {
                    break;
                }
                ++i;
            }

            if (!done)
            {
                //distribute if necessary in the primary direction
                bool primaryIsHorizontal = (this->directionPrimary == HSSDirectionLeftToRight || this->directionPrimary == HSSDirectionRightToLeft);
                if ((this->distributeX && primaryIsHorizontal) || (this->distributeY && !primaryIsHorizontal))
                {
                    for (std::vector<displayGroup::p>::iterator pgIt = primaryGroups.begin(); pgIt != primaryGroups.end(); ++pgIt)
                    {
                        displayGroup::p & pgGrp = *pgIt;
                        if (pgGrp->lines.empty())
                        {
                            this->_distribute(pgGrp, this->directionPrimary);
                            if (document->showLayoutSteps())
                            {
                                document->nextLayoutTick();
                                document->breakIfNeeded();
                                if (document->layoutStepDone())
                                {
                                    done = true;
                                    break;
                                }
                            }
                        }
                        else
                        {
                            for (std::vector<displayGroup::p>::iterator pgLineIt = pgGrp->lines.begin(); pgLineIt != pgGrp->lines.end(); ++pgLineIt)
                            {
                                this->_distribute(*pgLineIt, this->directionPrimary);
                                if (document->showLayoutSteps())
                                {
                                    document->nextLayoutTick();
                                    document->breakIfNeeded();
                                    if (document->layoutStepDone())
                                    {
                                        done = true;
                                        break;
                                    }
                                }
                            }
                        }

                    }
                }

                security_brake_reset();

                if (!done)
                {
                    for (std::vector<displayGroup::p>::iterator pgIt = primaryGroups.begin(); pgIt != primaryGroups.end(); ++pgIt)
                    {
                        displayGroup::p theDG = *pgIt;
                        if (theDG->lines.size() > 0)
                        {
                            displayGroup::p lineA, lineB, targetA, targetB;
                            HSSSimpleSelection::iterator lineAIt, lineBIt;

                            std::vector<displayGroup::p>::iterator pglIt = theDG->lines.begin();
                            lineA = *pglIt;
                            lineAIt = lineA->objects.begin();

                            ++pglIt;
                            lineB = *pglIt;
                            lineBIt = lineB->objects.begin();
                            displayGroup::p newGroup;

                            this->_recursiveCreateSecondaryGroups(lineAIt, lineA->objects.end(), lineBIt, lineB->objects.end(), targetA, targetB, newGroup, pglIt, theDG->lines.end(), true, secondaryGroups, true, false);
                        }
                        else
                        {
                            displayGroup::p newGroup = displayGroup::p(new displayGroup());
                            newGroup->lines.push_back(theDG);
                            newGroup->height = 0.;

                            for (HSSSimpleSelection::iterator it = theDG->objects.begin(); it != theDG->objects.end(); ++it)
                            {
                                QSharedPointer<HSSDisplayObject> & theDO = *it;
                                if (theDO->outerHeight > newGroup->height) newGroup->height = theDO->outerHeight;
                            }
                            secondaryGroups.push_back(newGroup);
                        }

                    } //for each primary group
                } //if !done

            } //if !done

            //        for (std::vector<displayGroup::p>::iterator linesIt = newGroup->lines.begin(); linesIt != newGroup->lines.end(); ++linesIt) {
            //            newGroup->height += (*linesIt)->height;
            //        }

            // Sort tallest group first, shortest last
            std::sort(secondaryGroups.begin(), secondaryGroups.end(), displayGroup::heightGreater);

            bool first = true;
            for (std::vector<displayGroup::p>::iterator sgIt = secondaryGroups.begin(); sgIt != secondaryGroups.end(); ++sgIt)
            {
                if (!this->_arrangeLines(*sgIt, this->directionSecondary, first))
                {
                    break;
                }

                first = false;
            }


            //assign the globalX and globalY and clean up flags
            for (HSSSimpleSelection::const_iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
            {
                const QSharedPointer<HSSDisplayObject> &child = *it;
                child->setGlobalX(qRound(this->globalX + child->x));
                child->setGlobalY(qRound(this->globalY + child->y));
                child->_layoutFlagIsInSecondaryGroup = false;
                child->_layoutFlagIsInSecondaryLine = false;
                child->_layoutFlagLockTop = false;
                child->_layoutFlagLockBottom = false;
            }

            if (this->heightByContent)
            {
                HSSUnit maxHeight = 0.;

                for (i = 0, size = secondaryGroups.size(); i < size; ++i)
                {
                    if (secondaryGroups[i]->height > maxHeight)
                    {
                        maxHeight = secondaryGroups[i]->height;
                    }
                }
                if (secondaryGroups.size() > 0)
                {
                    HSSUnit newValue = maxHeight + this->topPadding + this->bottomPadding;
                    if (this->height != newValue)
                    {
                        this->height = newValue;
                        this->_setInnerHeight();
                        this->_setOuterHeight();
                        this->setNeedsSurface(true);
                        this->setDirty(true);
                        this->notifyObservers(HSSObservablePropertyHeight, &this->height);
                        //we'll need a re-layout
                        this->_needsLayout = true;
                    }
                }
            }
        }
    }
}

void HSSContainer::_recursiveCreateSecondaryGroups(
                                                   HSSSimpleSelection::iterator lineAIt,
                                                   HSSSimpleSelection::iterator lineAStopIt,
                                                   HSSSimpleSelection::iterator lineBIt,
                                                   HSSSimpleSelection::iterator lineBStopIt,
                                                   displayGroup::p &targetA,
                                                   displayGroup::p &targetB,
                                                   displayGroup::p &newGroup,
                                                   std::vector<displayGroup::p>::iterator pglIt,
                                                   std::vector<displayGroup::p>::iterator pglStopIt,
                                                   bool addToSecondaryGroups,
                                                   std::vector<displayGroup::p> &secondaryGroups,
                                                   bool needsShoveling,
                                                   bool onlyAddToBIfNotInGroupYet
                                                   )
{
    bool lineADone = false;
    bool lineBDone = false;
    //bool needsShoveling = true;


    QSharedPointer<HSSDisplayObject> objA, objB;
    HSSSimpleSelection::iterator firstBIt = lineBIt;
    objA = *lineAIt;
    objB = *lineBIt;

    while (!lineADone || !lineBDone)
    {
        if (this->_overlaps(objA, objB))
        {
            //add objA to first line in new group and objB into second line

            if (!targetA)
            {
                targetA = displayGroup::p(new displayGroup());
                targetA->objects.add(objA);
                targetA->height = objA->outerHeight;
                objA->_layoutFlagIsInSecondaryLine = true;

            }
            else
            {
                if (!objA->_layoutFlagIsInSecondaryLine)
                {
                    targetA->objects.add(objA);
                    objA->_layoutFlagIsInSecondaryLine = true;
                    HSSUnit currentHeight = targetA->height;
                    if (objA->outerHeight > currentHeight)
                    {
                        targetA->height = objA->outerHeight;
                        newGroup->height += currentHeight - targetA->height;
                    }
                }
            }
            if (!onlyAddToBIfNotInGroupYet || (onlyAddToBIfNotInGroupYet && !objB->_layoutFlagIsInSecondaryLine))
            {
                if (!targetB)
                {
                    targetB = displayGroup::p(new displayGroup());
                    targetB->objects.add(objB);
                    targetB->height = objB->outerHeight;
                    objB->_layoutFlagIsInSecondaryLine = true;
                }
                else
                {
                    targetB->objects.add(objB);
                    objB->_layoutFlagIsInSecondaryLine = true;
                    HSSUnit currentHeight = targetB->height;
                    if (objB->outerHeight > currentHeight)
                    {
                        targetB->height = objB->outerHeight;
                        newGroup->height += currentHeight - targetB->height;
                    }
                }
            }

            if (addToSecondaryGroups)
            {
                if (!newGroup)
                {
                    newGroup = displayGroup::p(new displayGroup());
                    newGroup->height = 0.;
                    newGroup->lines.push_back(targetA);
                    newGroup->lines.push_back(targetB);
                    secondaryGroups.push_back(newGroup);
                    newGroup->height = targetA->height + targetB->height;
                }
            }

            //try to shovel in more objects of the first line
            if (needsShoveling)
            {
                QSharedPointer<HSSDisplayObject> firstObj = objA;
                HSSSimpleSelection::iterator firstObjIt = lineAIt;
                bool shovelDone = false;
                while (!shovelDone)
                {
                    ++lineAIt;
                    if (lineAIt != lineAStopIt)
                    {
                        objA = *lineAIt;

                        if (this->_overlaps(objA, objB)
                                && (objA->x + objA->outerWidth < objB->x + objB->outerWidth)
                                )
                        {
                            targetA->objects.add(objA);
                            objA->_layoutFlagIsInSecondaryLine = true;
                            firstObj = objA;
                            firstObjIt = lineAIt;
                            HSSUnit currentHeight = targetA->height;
                            if (objA->outerHeight > currentHeight)
                            {
                                targetA->height = objA->outerHeight;
                                newGroup->height += currentHeight - targetA->height;
                            }

                        }
                        else
                        {
                            shovelDone = true;
                        }
                    }
                    else
                    {
                        shovelDone = true;
                    }
                }
                needsShoveling = false;
                objA = firstObj;
                lineAIt = firstObjIt;

            } //needs shoveling

            //get next objB
            ++lineBIt;
            if (lineBIt != lineBStopIt)
            {
                objB = *lineBIt;
            }
            else
            {
                lineBDone = true;
            }
            //if no overlap
        }
        else
        {
            //get next objB
            ++lineBIt;
            if (lineBIt != lineBStopIt)
            {
                objB = *lineBIt;
            }
            else
            {
                lineBDone = true;
            }
        }

        if (lineBDone)
        {
            std::vector<displayGroup::p>::iterator pglItStore = pglIt;
            ++pglIt;
            if (pglIt != pglStopIt)
            {
                displayGroup::p nextLine = *pglIt;
                if (targetB)
                {
                    displayGroup::p dummyTargetA;
                    displayGroup::p newTargetB;
                    this->_recursiveCreateSecondaryGroups(targetB->objects.begin(), targetB->objects.end(), nextLine->objects.begin(), nextLine->objects.end(), dummyTargetA, newTargetB, newGroup, pglIt, pglStopIt, false, secondaryGroups, false, onlyAddToBIfNotInGroupYet);
                    if (newTargetB)
                    {
                        newGroup->lines.push_back(newTargetB);
                    }
                }

                this->_recursiveCreateSecondaryGroups(lineAIt, lineAStopIt, nextLine->objects.begin(), nextLine->objects.end(), targetA, targetB, newGroup, pglIt, pglStopIt, false, secondaryGroups, false, true);
            }
            pglIt = pglItStore;
            ++lineAIt;
            if (lineAIt != lineAStopIt)
            {
                objA = *lineAIt;
                lineBIt = firstBIt;
                objB = *lineBIt;
                lineBDone = false;
                if (addToSecondaryGroups)
                {
                    targetA.clear();
                    targetB.clear();
                    newGroup.clear();
                    needsShoveling = true;
                }
            }
            else
            {
                lineADone = true;
            }

        } // lineBDone

    } //while !lineADone || !lineBDone
}

bool HSSContainer::_overlaps(QSharedPointer<HSSDisplayObject> & childA, QSharedPointer<HSSDisplayObject> & childB)
{
    return this->_overlaps_horizontal(childA, childB) && this->_overlaps_vertical(childA, childB);
}

bool HSSContainer::_overlaps_horizontal(QSharedPointer<HSSDisplayObject> & childA, QSharedPointer<HSSDisplayObject> & childB)
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

bool HSSContainer::_overlaps_vertical(QSharedPointer<HSSDisplayObject> & childA, QSharedPointer<HSSDisplayObject> & childB)
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

bool HSSContainer::_addChildToGroupIfNeeded(const QSharedPointer<HSSDisplayObject> &child, AXR::HSSContainer::displayGroup::p &group, HSSDirectionValue direction, bool contained)
{
    unsigned i = 0;
    bool isHorizontal = (direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft);
    bool addedToGroup = false;
    HSSUnit lineTotalPrimary = 0;

    HSSUnit originalX = child->x;
    HSSUnit originalY = child->y;

    HSSSimpleSelection objects = group->objects;

    for (HSSSimpleSelection::const_iterator it = objects.begin(); it != objects.end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & otherChild = *it;
        if (isHorizontal)
        {
            lineTotalPrimary += otherChild->outerWidth;
        }
        else
        {
            lineTotalPrimary += otherChild->outerHeight;
        }
        if (
                ((child->x + child->outerWidth) > (otherChild->x - otherChild->leftMargin)) && ((child->x - child->leftMargin) < (otherChild->x + otherChild->outerWidth))
                && ((child->y + child->outerHeight) > (otherChild->y - otherChild->topMargin)) && ((child->y - child->topMargin) < (otherChild->y + otherChild->outerHeight))
                )
        {
            //it will go into a group

            //if it is the last one
            if (i >= group->objects.size() - 1)
            {
                //check if we have enough space to add it to the end of the line
                if (contained)
                {
                    if (isHorizontal)
                    {
                        if (lineTotalPrimary + child->outerWidth > this->innerWidth)
                        {
                            group->complete = true;
                        }
                    }
                    else
                    {
                        if (lineTotalPrimary + child->outerHeight > this->innerHeight)
                        {
                            group->complete = true;
                        }
                    }
                }

                if (!group->complete)
                {
                    //put it into the group
                    group->objects.add(child);
                    addedToGroup = true;

                    switch (direction)
                    {
                    case HSSDirectionTopToBottom:
                    case HSSDirectionBottomToTop:
                        group->height += child->outerHeight;
                        break;

                    case HSSDirectionRightToLeft:
                    default:
                        group->width += child->outerWidth;
                        break;
                    }

                    this->_arrange(group, direction);
                }
                else
                {
                    //restore the original position
                    child->x = originalX;
                    child->y = originalY;
                }

            }
            else
            {

                //push it further in the primary direction, and check again
                switch (direction)
                {
                case HSSDirectionRightToLeft:
                {
                    child->x = otherChild->x - child->outerWidth;
                    break;
                }

                case HSSDirectionTopToBottom:
                {
                    child->y = otherChild->y + otherChild->outerHeight;
                    break;
                }

                case HSSDirectionBottomToTop:
                {
                    child->y = otherChild->y - child->outerHeight;
                    break;
                }

                default:
                {
                    child->x = otherChild->x + otherChild->outerWidth;
                    break;
                }
                }

            }
        }//if overlap

        ++i;

    }// for each child

    return addedToGroup;
}

//std::vector<HSSContainer::displayGroup::p> HSSContainer::_getGroupsOverlapping(QSharedPointer<HSSDisplayObject> &child, std::vector<HSSContainer::displayGroup::p> &groups, HSSDirectionValue direction)
//{
//    std::vector<HSSContainer::displayGroup::p>::iterator it;
//    std::vector<HSSContainer::displayGroup::p> ret;
//    unsigned i, size, j, size2;
//    bool overlaps = false;
//
//    for (it=groups.begin(); it!=groups.end(); ++it) {
//        HSSContainer::displayGroup::p group = *it;
//        for (j=0, size2=group->lines.size(); j<size2; ++j) {
//            const HSSContainer::displayGroup::p & otherGroup = group->lines[j];
//            if(
//               ((child->x + child->outerWidth) > otherGroup->x) && ((child->x - child->leftMargin) < (otherGroup->x + otherGroup->width))
//               && ((child->y + child->outerHeight) > otherGroup->y) && ((child->y - child->leftMargin) < (otherGroup->y + otherGroup->height))
//               ){
//                for (i=0, size = otherGroup->objects.size(); i<size; ++i) {
//                    const QSharedPointer<HSSDisplayObject> & otherChild = otherGroup->objects[i];
//                    if(direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft){
//                        if( ((child->y + child->outerHeight) > (otherChild->y - otherChild->topMargin)) && ((child->y - child->topMargin) < (otherChild->y + otherChild->outerHeight)) ){
//                            overlaps = true;
//                        }
//                    } else {
//                        if( ((child->x + child->outerWidth) > (otherChild->x - otherChild->leftMargin)) && ((child->x - child->leftMargin) < (otherChild->x + otherChild->outerWidth)) ){
//                            overlaps = true;
//                        }
//                    }
//
//                    if (overlaps) {
//                        ret.push_back(group);
//                        overlaps = false;
//                        continue;
//                    }
//                }
//            }
//        }
//    }
//
//    return ret;
//}

bool HSSContainer::_mergeGroupsIfNeeded(displayGroup::p &group, displayGroup::p &otherGroup, HSSDirectionValue direction)
{
    if (((group->x + group->width) > otherGroup->x) && (group->x < (otherGroup->x + otherGroup->width)) && ((group->y + group->height) > otherGroup->y) && (group->y < (otherGroup->y + otherGroup->height)))
    {
        //if the group bounds overlap, check each individual element against each other
        for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
        {
            QSharedPointer<HSSDisplayObject> & child = *it;
            for (HSSSimpleSelection::iterator it2 = otherGroup->objects.begin(); it2!= otherGroup->objects.end(); ++it2)
            {
                QSharedPointer<HSSDisplayObject> &otherChild = *it2;
                if (((child->x + child->outerWidth) > (otherChild->x - otherChild->leftMargin)) && ((child->x - child->leftMargin) < (otherChild->x + otherChild->outerWidth)) && ((child->y + child->outerHeight) > (otherChild->y - otherChild->topMargin)) && ((child->y - child->topMargin) < (otherChild->y + otherChild->outerHeight)))
                {
                    //there is an overlap, merge the groups
                    //add all the elements of this group to the overlapping one
                    group->objects.insert(group->objects.end(), otherGroup->objects.begin(), otherGroup->objects.end());
                    if (direction == HSSDirectionLeftToRight || direction == HSSDirectionRightToLeft)
                    {
                        group->width += otherGroup->width;
                    }
                    else
                    {
                        group->height += otherGroup->height;
                    }

                    this->_arrange(group, direction);

                    return true;
                }
            }
        }
    }

    return false;
}

void HSSContainer::_arrange(displayGroup::p &group, HSSDirectionValue direction)
{
    size_t i = 0, size = group->objects.size();

    switch (direction)
    {
    case HSSDirectionRightToLeft:
    {
        bool byAnchors = false;
        for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
        {
            if (!(*it)->_anchorXdefault)
            {
                byAnchors = true;
                break;
            }
        }
        if (byAnchors)
        {
            //calculate the new alignment and anchor point for the group
            const QSharedPointer<HSSDisplayObject> & groupFirst = group->objects.front();
            HSSUnit alignmentTotal = 0.;
            HSSUnit accWidth = groupFirst->anchorX;
            HSSUnit anchorsTotal = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
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
            if (startX - group->width < 0) startX = group->width;
            if (startX > this->width) startX = this->width;
            accWidth = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                otherChild2->x = startX - otherChild2->width - accWidth;
                accWidth += otherChild2->width;
            }
            group->x = group->objects.front()->x - group->objects.front()->leftMargin;
            group->y = group->objects.front()->y - group->objects.front()->topMargin;
        }
        else
        {

            //calculate the alignment point for the group
            HSSUnit alignmentTotal = 0.;
            HSSUnit accWidth = 0.;
            HSSUnit widthsTotal = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & currentChild = *it;
                alignmentTotal += currentChild->alignX;
                widthsTotal += currentChild->outerWidth;
            }
            HSSUnit groupAlignX = alignmentTotal / size;
            HSSUnit startX = groupAlignX + widthsTotal / 2;
            if (startX > (this->width - this->rightPadding)) startX = (this->width - this->rightPadding);
            if (startX - group->width < this->leftPadding) startX = this->leftPadding + group->width;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & currentChild = *it;
                accWidth += currentChild->outerWidth;
                currentChild->x = (startX - accWidth) - currentChild->rightMargin;
            }
            group->x = startX - group->width;
        }
        break;
    }

    case HSSDirectionTopToBottom:
    {
        bool byAnchors = false;
        for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
        {
            if (!(*it)->_anchorYdefault)
            {
                byAnchors = true;
                break;
            }
        }
        if (byAnchors)
        {
            //calculate the new alignment and anchor point for the group
            const QSharedPointer<HSSDisplayObject> & groupFirst = group->objects.front();
            HSSUnit alignmentTotal = 0.;
            HSSUnit accHeight = groupFirst->height - groupFirst->anchorY;
            HSSUnit anchorsTotal = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
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
            if (startY > this->height - group->height) startY = this->height - group->height;
            if (startY < 0) startY = 0.;
            accHeight = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                otherChild2->y = startY + accHeight;
                accHeight += otherChild2->height;
            }
            group->x = group->objects.front()->x - group->objects.front()->leftMargin;
            group->y = group->objects.front()->y - group->objects.front()->topMargin;
        }
        else
        {
            //calculate the alignment point for the group
            HSSUnit alignmentTotal = 0.;
            HSSUnit accHeight = 0.;
            HSSUnit heightsTotal = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & currentChild = *it;
                alignmentTotal += currentChild->alignY;
                heightsTotal += currentChild->outerHeight;
            }
            HSSUnit groupAlignY = alignmentTotal / size;
            HSSUnit startY = groupAlignY - heightsTotal / 2;
            if (startY > (this->width - this->bottomPadding) - group->height) startY = (this->height - this->bottomPadding) - group->height;
            if (startY < this->topPadding) startY = this->topPadding;
            group->y = startY;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
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
        for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
        {
            if (!(*it)->_anchorYdefault)
            {
                byAnchors = true;
                break;
            }
        }
        if (byAnchors)
        {
            //calculate the new alignment and anchor point for the group
            const QSharedPointer<HSSDisplayObject> & groupFirst = group->objects.front();
            HSSUnit alignmentTotal = 0;
            HSSUnit accHeight = groupFirst->anchorY;
            HSSUnit anchorsTotal = 0;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
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
            if (startY - group->height < 0) startY = group->height;
            if (startY > this->height) startY = this->height;
            accHeight = 0;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                otherChild2->y = startY - otherChild2->height - accHeight;
                accHeight += otherChild2->height;
            }
            group->x = group->objects.front()->x;
            group->y = group->objects.front()->y;
        }
        else
        {
            //calculate the alignment point for the group
            HSSUnit alignmentTotal = 0.;
            HSSUnit accHeight = 0.;
            HSSUnit heightsTotal = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & currentChild = *it;
                alignmentTotal += currentChild->alignY;
                heightsTotal += currentChild->outerHeight;
            }
            HSSUnit groupAlignY = alignmentTotal / size;
            HSSUnit startY = groupAlignY + heightsTotal / 2;
            if (startY > (this->height - this->bottomPadding)) startY = (this->height - this->bottomPadding);
            if (startY - group->height < this->topPadding) startY = this->topPadding + group->height;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & currentChild = *it;
                accHeight += currentChild->outerHeight;
                currentChild->y = (startY - accHeight) - currentChild->bottomMargin;
            }
            group->y = startY - group->height;
        }

        break;
    }

    default:
    {
        bool byAnchors = false;
        for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
        {
            if (!(*it)->_anchorXdefault)
            {
                byAnchors = true;
                break;
            }
        }
        if (byAnchors)
        {
            //calculate the new alignment and anchor point for the group
            const QSharedPointer<HSSDisplayObject> & groupFirst = group->objects.front();
            HSSUnit alignmentTotal = 0.;
            HSSUnit accWidth = groupFirst->outerWidth - groupFirst->anchorX;
            HSSUnit anchorsTotal = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
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
            if (startX > (this->width - this->rightPadding) - group->width) startX = (this->width - this->rightPadding) - group->width;
            if (startX < 0.) startX = 0.;
            accWidth = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & otherChild2 = *it;
                otherChild2->x = startX + accWidth + otherChild2->leftMargin;
                accWidth += otherChild2->outerWidth;
            }
            group->x = group->objects.front()->x - group->objects.front()->leftMargin;
            group->y = group->objects.front()->y - group->objects.front()->topMargin;

        }
        else
        {
            //calculate the alignment point for the group
            HSSUnit alignmentTotal = 0.;
            HSSUnit accWidth = 0.;
            HSSUnit widthsTotal = 0.;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & currentChild = *it;
                alignmentTotal += currentChild->alignX;
                widthsTotal += currentChild->outerWidth;
            }
            HSSUnit groupAlignX = alignmentTotal / size;
            HSSUnit startX = groupAlignX - widthsTotal / 2;
            if (startX > (this->width - this->rightPadding) - group->width) startX = (this->width - this->rightPadding) - group->width;
            if (startX < this->leftPadding) startX = this->leftPadding;
            group->x = startX;
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it!= group->objects.end(); ++it)
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

bool HSSContainer::_arrangeLines(displayGroup::p &group, HSSDirectionValue direction, bool isFirstGroup)
{
    if (group->lines.size() < 2)
    {
        return true;
    }

    AXRDocument * document = this->getController()->document();

    switch (direction)
    {
    case HSSDirectionRightToLeft:
    {

        break;
    }

    case HSSDirectionTopToBottom:
    {
        bool byAnchors = false;
        for (size_t i = 0; i < group->lines.size(); ++i)
        {
            displayGroup::p line = group->lines[i];
            for (HSSSimpleSelection::iterator it2 = line->objects.begin(); it2!= line->objects.end(); ++it2)
            {
                if (!(*it2)->_anchorYdefault)
                {
                    byAnchors = true;
                    break;
                }
            }
        }
        if (byAnchors)
        {

        }
        else
        {
            HSSUnit alignmentTotal = 0.;
            HSSUnit accHeight = 0.;
            HSSUnit totalSize = 0.;
            HSSUnit biggest = 0.;

            //create "push groups" to find out which one is the strongest
            displayGroup::p biggestGroup;
            bool pushGroupsDone = false;
            std::vector<displayGroup::p>::iterator pglIt = group->lines.begin();
            displayGroup::p lineA, lineB;
            HSSSimpleSelection::iterator lineAIt, lineBIt;
            QSharedPointer<HSSDisplayObject> objA, objB;
            lineA = *pglIt;
            lineAIt = lineA->objects.begin();
            objA = *lineAIt;
            ++pglIt;
            lineB = *pglIt;
            lineBIt = lineB->objects.begin();
            objB = *lineBIt;

            while (!pushGroupsDone)
            {
                displayGroup::p newGroup;
                this->_recursiveGetPushGroup(objA, objB, pglIt, group->lines.end(), newGroup);
                if (newGroup && newGroup->height >= biggest)
                {
                    biggestGroup = newGroup;
                    biggest = newGroup->height;
                }
                ++lineBIt;
                if (lineBIt != lineB->objects.end())
                {
                    objB = *lineBIt;
                }
                else
                {
                    lineBIt = lineB->objects.begin();
                    objB = *lineBIt;

                    ++lineAIt;
                    if (lineAIt != lineA->objects.end())
                    {
                        objA = *lineAIt;
                    }
                    else
                    {
                        pushGroupsDone = true;
                    }
                }
            }
            if (!biggestGroup)
            {
                biggestGroup = group->lines.back();
            }

            group->height = biggestGroup->height;

            for (HSSSimpleSelection::iterator it = biggestGroup->objects.begin(); it!= biggestGroup->objects.end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> & currentChild = *it;
                alignmentTotal += currentChild->alignY;
                totalSize += currentChild->outerHeight;
            }

            //calculate the alignment point for the biggest elements
            HSSUnit groupAlignY = alignmentTotal / biggestGroup->objects.size();

            //reposition the elements in the group
            HSSUnit startY = groupAlignY - totalSize / 2;
            if (startY > (this->height - this->bottomPadding) - group->height) startY = (this->height - this->bottomPadding) - group->height;
            if (startY < this->topPadding) startY = this->topPadding;

            size_t i = 0;
            size_t size = biggestGroup->objects.size();
            for (HSSSimpleSelection::iterator bobjIt = biggestGroup->objects.begin(); bobjIt != biggestGroup->objects.end(); ++bobjIt)
            {
                QSharedPointer<HSSDisplayObject> otherChild2 = *bobjIt;
                HSSUnit newValue = startY + accHeight + otherChild2->topMargin;
                if (!isFirstGroup)
                {
                    //find the bottom constraint of the previous line
                    HSSUnit prevConstraintBottom = this->topPadding;
                    bool hasPrevConstraintBottom = false;
                    if (i != 0)
                    {
                        displayGroup::p previousLine = group->lines[i - 1];
                        hasPrevConstraintBottom = this->_recursiveFindBottomConstraint(prevConstraintBottom, group, i, otherChild2);
                    }
                    //find the top constraint of the next line
                    HSSUnit nextConstraintTop = this->height - this->bottomPadding;
                    bool hasNextConstraintTop = false;
                    if (group->lines.size() > i + 1)
                    {
                        displayGroup::p nextLine = group->lines[i + 1];
                        hasNextConstraintTop = this->_recursiveFindTopConstraint(nextConstraintTop, group, i, otherChild2);
                    }

                    if ((!otherChild2->_layoutFlagLockTop || newValue > otherChild2->y)
                            && (!otherChild2->_layoutFlagLockBottom || newValue + otherChild2->height + otherChild2->bottomMargin < otherChild2->y + otherChild2->height + otherChild2->bottomMargin)
                            )
                    {
                        if (hasNextConstraintTop && newValue + otherChild2->outerHeight < nextConstraintTop)
                        {
                            otherChild2->y = nextConstraintTop - (otherChild2->height + otherChild2->topMargin);
                        }
                        else if (hasPrevConstraintBottom && newValue + otherChild2->outerHeight > prevConstraintBottom)
                        {
                            otherChild2->y = prevConstraintBottom + otherChild2->bottomMargin;
                        }
                        else
                        {
                            otherChild2->y = newValue;
                        }
                    }
                }
                else
                {
                    otherChild2->y = newValue;
                }

                accHeight += otherChild2->outerHeight;

                if (document->showLayoutSteps())
                {
                    document->nextLayoutTick();
                    document->breakIfNeeded();
                    if (document->layoutStepDone())
                    {
                        return false;
                    }
                }

                if (i == 0)
                {
                    otherChild2->_layoutFlagLockBottom = true;
                    //otherChild2->_layoutLockBottomPosition = otherChild2->y + otherChild2->height + otherChild2->bottomMargin;
                }
                else if (i == size - 1)
                {
                    //otherChild2->_layoutLockTopPosition = otherChild2->y + otherChild2->topMargin;
                    otherChild2->_layoutFlagLockTop = true;
                }
                else
                {
                    otherChild2->_layoutFlagLockTop = true;
                    //otherChild2->_layoutLockTopPosition = otherChild2->y + otherChild2->topMargin;
                    otherChild2->_layoutFlagLockBottom = true;
                    //otherChild2->_layoutLockBottomPosition = otherChild2->y + otherChild2->height + otherChild2->bottomMargin;
                }
                ++i;
            }

            for (i = 0, size = group->lines.size(); i < size; ++i)
            {
                displayGroup::p & line = group->lines[i];
                for (HSSSimpleSelection::iterator it2 = line->objects.begin(); it2!= line->objects.end(); ++it2)
                {
                    QSharedPointer<HSSDisplayObject> & currentChild = *it2;

                    if (!currentChild->_layoutFlagLockBottom)
                    {
                        //find the constraint towards the bottom
                        HSSUnit constraintBottom = this->height - this->bottomPadding;
                        bool needsConstraintBottom = false;
                        if (group->lines.size() > i + 1)
                        {
                            displayGroup::p nextLine = group->lines[i + 1];

                            for (HSSSimpleSelection::iterator nlIt = nextLine->objects.begin(); nlIt != nextLine->objects.end(); ++nlIt)
                            {
                                QSharedPointer<HSSDisplayObject> nlObj = *nlIt;
                                if (nlObj->_layoutFlagLockTop && this->_overlaps_horizontal(nlObj, currentChild))
                                {
                                    if (nlObj->y - nlObj->topMargin < constraintBottom)
                                    {
                                        constraintBottom = nlObj->y - nlObj->topMargin;
                                        needsConstraintBottom = true;
                                    }
                                }
                            }
                        }
                        if (needsConstraintBottom && currentChild->alignY + (currentChild->outerHeight / 2) > constraintBottom)
                        {
                            currentChild->y = constraintBottom - currentChild->bottomMargin - currentChild->height;
                            currentChild->_layoutFlagLockBottom = true;
                            if (document->showLayoutSteps())
                            {
                                document->nextLayoutTick();
                                document->breakIfNeeded();
                                if (document->layoutStepDone())
                                {
                                    return false;
                                }
                            }
                        }
                    }

                    if (!currentChild->_layoutFlagLockTop)
                    {
                        //find the constraint towards the top
                        HSSUnit constraintTop = this->topPadding;
                        bool needsConstraintTop = false;
                        if (i != 0)
                        {
                            displayGroup::p previousLine = group->lines[i - 1];

                            for (HSSSimpleSelection::iterator plIt = previousLine->objects.begin(); plIt != previousLine->objects.end(); ++plIt)
                            {
                                QSharedPointer<HSSDisplayObject> plObj = *plIt;
                                if (plObj->_layoutFlagLockBottom && this->_overlaps_horizontal(plObj, currentChild))
                                {
                                    if (plObj->y + plObj->outerHeight > constraintTop)
                                    {
                                        constraintTop = plObj->y + plObj->height + plObj->bottomMargin;
                                        needsConstraintTop = true;
                                    }
                                }
                            }
                        }
                        if (needsConstraintTop && currentChild->alignY - (currentChild->outerHeight / 2) < constraintTop)
                        {
                            currentChild->y = constraintTop + currentChild->topMargin;
                            currentChild->_layoutFlagLockTop = true;
                            if (document->showLayoutSteps())
                            {
                                document->nextLayoutTick();
                                document->breakIfNeeded();
                                if (document->layoutStepDone())
                                {
                                    return false;
                                }
                            }
                        }
                    }
                }
            }
        }

        break;
    }

    case HSSDirectionBottomToTop:
    {
        break;
    }

    default:
    {
        break;
    }
    }

    return true;
}

void HSSContainer::_recursiveGetPushGroup(QSharedPointer<HSSDisplayObject> objA, QSharedPointer<HSSDisplayObject> objB, std::vector<displayGroup::p>::iterator linesIt, std::vector<displayGroup::p>::iterator stopIt, displayGroup::p &ret)
{
    if (this->_overlaps_horizontal(objA, objB))
    {
        if (!ret)
        {
            ret = displayGroup::p(new displayGroup());
            ret->objects.add(objA);
            ret->objects.add(objB);
            ret->height = objA->outerHeight + objB->outerHeight;
        }
        else
        {
            ret->objects.add(objB);
            ret->height += objB->outerHeight;
        }
    }
    ++linesIt;
    if (linesIt != stopIt)
    {
        displayGroup::p nextLine = *linesIt;
        QSharedPointer<HSSDisplayObject> objC = *nextLine->objects.begin();
        this->_recursiveGetPushGroup(objB, objC, linesIt, stopIt, ret);
    }
}

bool HSSContainer::_recursiveFindTopConstraint(HSSUnit &constraint, displayGroup::p group, size_t i, QSharedPointer<HSSDisplayObject> child)
{
    bool ret = false;
    HSSUnit constraintStore = constraint;

    if (i >= group->lines.size() - 1)
    {
        return false;
    }

    displayGroup::p nextLine = group->lines[i + 1];

    for (HSSSimpleSelection::iterator nlIt = nextLine->objects.begin(); nlIt != nextLine->objects.end(); ++nlIt)
    {
        QSharedPointer<HSSDisplayObject> nlObj = *nlIt;
        if (nlObj->_layoutFlagLockTop && this->_overlaps_horizontal(nlObj, child))
        {
            if (nlObj->y - nlObj->topMargin < constraint)
            {
                constraint = nlObj->y - nlObj->topMargin;
                ret = true;
            }
        }
        else
        {
            if (i + 1 < group->lines.size())
            {
                HSSUnit newConstraint = constraintStore;
                bool needsNewConstraint = this->_recursiveFindTopConstraint(newConstraint, group, i + 1, nlObj);
                if (needsNewConstraint)
                {
                    HSSUnit newValue = newConstraint - nlObj->outerHeight;
                    if (newValue < constraint)
                    {
                        constraint = newValue;
                        ret = true;
                    }
                }
            }
        }
    }

    return ret;
}

bool HSSContainer::_recursiveFindBottomConstraint(HSSUnit &constraint, displayGroup::p group, size_t i, QSharedPointer<HSSDisplayObject> child)
{
    bool ret = false;
    HSSUnit constraintStore = constraint;

    if (i < 1)
    {
        return false;
    }

    displayGroup::p nextLine = group->lines[i - 1];

    for (HSSSimpleSelection::iterator nlIt = nextLine->objects.begin(); nlIt != nextLine->objects.end(); ++nlIt)
    {
        QSharedPointer<HSSDisplayObject> nlObj = *nlIt;
        if (nlObj->_layoutFlagLockBottom && this->_overlaps_horizontal(nlObj, child))
        {
            if (nlObj->y + nlObj->outerHeight > constraint)
            {
                constraint = nlObj->y + nlObj->height + nlObj->bottomMargin;
                ret = true;
            }
        }
        else
        {
            if (i < 2)
            {
                HSSUnit newConstraint = constraintStore;
                bool needsNewConstraint = this->_recursiveFindBottomConstraint(newConstraint, group, i - 1, nlObj);
                if (needsNewConstraint)
                {
                    HSSUnit newValue = newConstraint + nlObj->outerHeight;
                    if (newValue > constraint)
                    {
                        constraint = newValue;
                        ret = true;
                    }
                }
            }
        }
    }

    return ret;
}

void HSSContainer::_distribute(displayGroup::p &group, HSSDirectionValue direction)
{
    switch (direction)
    {
    case HSSDirectionRightToLeft:
    {
        if (group->objects.size() == 1)
        {
            const QSharedPointer<HSSDisplayObject> &theDO = group->objects.front();
            HSSUnit newValue = ((this->innerWidth - theDO->outerWidth) / 2) + theDO->leftMargin;
            theDO->x = this->leftPadding + newValue;
            group->x = 0.;
            group->y = theDO->y;
        }
        else
        {
            HSSUnit accWidth = this->rightPadding;
            HSSUnit totalWidth = 0.;

            //calculate the total width of the group
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
            {
                totalWidth += (*it)->outerWidth;
            }

            if (this->distributeXLinear)
            {
                //now get the remaining space
                HSSUnit remainingSpace = this->innerWidth - totalWidth;
                //divide it by the number of elements-1
                HSSUnit spaceChunk = remainingSpace / (group->objects.size() - 1);
                unsigned i = 0;
                for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> &theDO = *it;
                    theDO->x = this->width - accWidth - (spaceChunk * i) - theDO->width - theDO->rightMargin;
                    accWidth += theDO->outerWidth;
                    ++i;
                }
                group->x = 0.;
                group->y = group->objects.back()->y;
            }
            else
            {
                //now get the remaining space
                HSSUnit remainingSpace = this->innerWidth - totalWidth;
                //divide it by the number of elements+1
                HSSUnit spaceChunk = remainingSpace / (group->objects.size() + 1);
                unsigned i = 0;
                for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> &theDO = *it;
                    theDO->x = this->width - accWidth - spaceChunk - (spaceChunk * i) - theDO->width - theDO->leftMargin;
                    accWidth += theDO->outerWidth;
                    ++i;
                }
                group->x = 0.;
                group->y = group->objects.front()->y;
            }
        }

        break;
    }

    case HSSDirectionTopToBottom:
    {
        if (group->objects.size() == 1)
        {
            const QSharedPointer<HSSDisplayObject> &theDO = group->objects.front();
            HSSUnit newValue = ((this->innerHeight - theDO->outerHeight) / 2) + theDO->topMargin;
            theDO->y = this->topPadding + newValue;
            group->y = 0.;
            group->x = theDO->x;
        }
        else
        {
            HSSUnit accHeight = this->topPadding;
            HSSUnit totalHeight = 0.;

            //calculate the total height of the group
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
            {
                totalHeight += (*it)->outerHeight;
            }

            if (this->distributeXLinear)
            {
                //now get the remaining space
                HSSUnit remainingSpace = this->innerHeight - totalHeight;
                //divide it by the number of elements-1
                HSSUnit spaceChunk = remainingSpace / (group->objects.size() - 1);
                unsigned i = 0;
                for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> &theDO = *it;
                    theDO->y = accHeight + (spaceChunk * i) + theDO->topMargin;
                    accHeight += theDO->outerHeight;
                    ++i;
                }
                group->x = 0.;
                group->y = group->objects.front()->y;
            }
            else
            {
                //now get the remaining space
                HSSUnit remainingSpace = this->innerHeight - totalHeight;
                //divide it by the number of elements+1
                HSSUnit spaceChunk = remainingSpace / (group->objects.size() + 1);
                unsigned i = 0;
                for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> &theDO = *it;
                    theDO->y = accHeight + spaceChunk + (spaceChunk * i) + theDO->topMargin;
                    accHeight += theDO->outerHeight;
                    ++i;
                }
                group->y = 0.;
                group->x = group->objects.front()->x;
            }
        }
        break;
    }

    case HSSDirectionBottomToTop:
    {
        if (group->objects.size() == 1)
        {
            const QSharedPointer<HSSDisplayObject> &theDO = group->objects.front();
            HSSUnit newValue = ((this->innerHeight - theDO->outerHeight) / 2) + theDO->topMargin;
            theDO->y = this->topPadding + newValue;
            group->y = 0.;
            group->x = theDO->x;
        }
        else
        {
            HSSUnit accHeight = this->bottomPadding;
            HSSUnit totalHeight = 0.;

            //calculate the total height of the group
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
            {
                totalHeight += (*it)->outerHeight;
            }

            if (this->distributeXLinear)
            {
                //now get the remaining space
                HSSUnit remainingSpace = this->innerHeight - totalHeight;
                //divide it by the number of elements-1
                HSSUnit spaceChunk = remainingSpace / (group->objects.size() - 1);
                unsigned i = 0;
                for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> &theDO = *it;
                    theDO->y = this->height - accHeight - (spaceChunk * i) - theDO->height - theDO->bottomMargin;
                    accHeight += theDO->outerHeight;
                    ++i;
                }
                group->y = 0.;
                group->x = group->objects.back()->x;
            }
            else
            {
                //now get the remaining space
                HSSUnit remainingSpace = this->innerHeight - totalHeight;
                //divide it by the number of elements+1
                HSSUnit spaceChunk = remainingSpace / (group->objects.size() + 1);
                unsigned i = 0;
                for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
                {
                    QSharedPointer<HSSDisplayObject> &theDO = *it;
                    theDO->y = this->height - accHeight - spaceChunk - (spaceChunk * i) - theDO->height - theDO->topMargin;
                    accHeight += theDO->outerHeight;
                    ++i;
                }
                group->y = 0.;
                group->x = group->objects.front()->x;
            }
        }
        break;
    }

    default:
    {
        if (group->objects.size() == 1)
        {
            const QSharedPointer<HSSDisplayObject> &theDO = group->objects.front();
            HSSUnit newValue = ((this->innerWidth - theDO->outerWidth) / 2) + theDO->leftMargin;
            theDO->x = this->leftPadding + newValue;
            group->x = 0.;
            group->y = theDO->y;
        }
        else
        {
            HSSUnit accWidth = this->leftPadding;
            HSSUnit totalWidth = 0.;

            //calculate the total width of the group
            for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
            {
                totalWidth += (*it)->outerWidth;
            }

            if (this->distributeXLinear)
            {
                //now get the remaining space
                HSSUnit remainingSpace = this->innerWidth - totalWidth;
                //divide it by the number of elements-1
                HSSUnit spaceChunk = remainingSpace / (group->objects.size() - 1);
                unsigned i = 0;
                for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
                {
                    (*it)->x = accWidth + (spaceChunk * i) + (*it)->leftMargin;
                    accWidth += (*it)->outerWidth;
                    ++i;
                }
                group->x = 0.;
                group->y = group->objects.front()->y;
            }
            else
            {
                //now get the remaining space
                HSSUnit remainingSpace = this->innerWidth - totalWidth;
                //divide it by the number of elements+1
                HSSUnit spaceChunk = remainingSpace / (group->objects.size() + 1);
                unsigned i = 0;
                for (HSSSimpleSelection::iterator it = group->objects.begin(); it != group->objects.end(); ++it)
                {
                    (*it)->x = accWidth + spaceChunk + (spaceChunk * i) + (*it)->leftMargin;
                    accWidth += (*it)->outerWidth;
                    ++i;
                }
                group->x = 0.;
                group->y = group->objects.front()->y;
            }
        }

        break;
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
        if (stringValue == "leftToRight" || stringValue == "rightToLeft")
        {
            if (this->directionSecondary == HSSDirectionLeftToRight || this->directionSecondary == HSSDirectionRightToLeft)
            {
                this->directionSecondary = HSSDirectionTopToBottom;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if (stringValue == "leftToRight")
            {
                this->directionSecondary = HSSDirectionLeftToRight;
            }
            else
            {
                this->directionSecondary = HSSDirectionRightToLeft;
            }
        }
        else if (stringValue == "topToBottom" || stringValue == "bottomToTop")
        {
            if (this->directionSecondary == HSSDirectionTopToBottom || this->directionSecondary == HSSDirectionBottomToTop)
            {
                this->directionSecondary = HSSDirectionLeftToRight;
                this->notifyObservers(HSSObservablePropertyDirectionSecondary, &this->directionSecondary);
            }
            if (stringValue == "topToBottom")
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
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
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
            QSharedPointer<HSSObjectDefinition> objdef = this->getController()->objectTreeGet(objname->getValue());
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
            std_log("######## FIXME ################");
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

bool HSSContainer::handleEvent(HSSEventType type, void* data)
{
    bool handled = false;
    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it < this->allChildren->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> child = *it;
        bool childHandled = child->handleEvent(type, data);
        if (childHandled)
        {
            handled = true;
        }
    }

    bool superHandled = HSSDisplayObject::handleEvent(type, data);
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
