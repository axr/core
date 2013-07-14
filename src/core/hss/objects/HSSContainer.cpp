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
#include <QtGlobal>
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

QVariantMap HSSContainer::toMap() const
{
    QVariantMap map = HSSObject::toMap();

    if (this->children->size() > 0)
    {
        QList<QVariant> childrenList;

        for (HSSSimpleSelection::iterator it = this->children->begin(); it!= this->children->end(); ++it)
        {
            childrenList.append((*it)->toMap());
        }

        map.insert("children", QVariant(childrenList));
    }
    else
    {
        map.insert("children", QVariant(false));
    }

    return map;
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
    this->notifyObservers("treeChange", sharedThis);
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
    this->notifyObservers("treeChange", this->shared_from_this());
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


void HSSContainer::accept(HSSAbstractVisitor* visitor, bool traverse)
{
    visitor->visit(*this);

    if (!traverse)
        return;

    for (HSSSimpleSelection::iterator child = this->allChildren->begin(); child != this->allChildren->end(); ++child)
    {
        (*child)->accept(visitor, filterFlags);
    }
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
