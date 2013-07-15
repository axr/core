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
    this->_initialize();
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
}

void HSSContainer::_initialize()
{
    this->distributeX = false;
    this->distributeXLinear = false;
    this->distributeY = false;
    this->distributeYLinear = false;

    this->children = QSharedPointer<HSSSimpleSelection>(new HSSSimpleSelection());
    this->allChildren = QSharedPointer<HSSSimpleSelection>(new HSSSimpleSelection());
}

HSSContainer::HSSContainer(const HSSContainer & orig)
: HSSDisplayObject(orig)
{
    this->_initialize();
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
