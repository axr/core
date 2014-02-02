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
#include <limits>
#include <QtGlobal>
#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSAbstractVisitor.h"
#include "HSSStroke.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSExpression.h"
#include "HSSFunction.h"
#include "HSSLayoutLine.h"
#include "HSSKeywordConstant.h"
#include "HSSNumberConstant.h"
#include "HSSMultipleValue.h"
#include "HSSObjectDefinition.h"
#include "HSSObjectNameConstant.h"
#include "HSSPercentageConstant.h"
#include "HSSRectangle.h"
#include "HSSRoundedRect.h"
#include "HSSRule.h"
#include "HSSStringConstant.h"
#include "HSSTextBlock.h"
#include "HSSValue.h"

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

    this->children = QSharedPointer<HSSSimpleSelection>(new HSSSimpleSelection(this->getController()));
    this->allChildren = QSharedPointer<HSSSimpleSelection>(new HSSSimpleSelection(this->getController()));

    this->addCallback("contentAlignX", new HSSComputeCallback<HSSContainer>(this, &HSSContainer::computeContentAlignX));
    this->addNotifyCallback("contentAlignX", new HSSObserveCallback<HSSContainer>(this, &HSSContainer::notifyContentAlignX));
    this->addCallback("contentAlignY", new HSSComputeCallback<HSSContainer>(this, &HSSContainer::computeContentAlignY));
    this->addNotifyCallback("contentAlignY", new HSSObserveCallback<HSSContainer>(this, &HSSContainer::notifyContentAlignY));
    this->addCallback("content", new HSSComputeCallback<HSSContainer>(this, &HSSContainer::computeContent));
    this->addCallback("shape", new HSSComputeCallback<HSSContainer>(this, &HSSContainer::computeShape));
    this->addNotifyCallback("shape", new HSSObserveCallback<HSSContainer>(this, &HSSContainer::notifyShape));
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
    this->cleanTrackedObservers();
    if (this->children)
    {
        this->children->clear();
    }
    if (this->allChildren){
        this->allChildren->clear();
    }
}

void HSSContainer::setDefaults()
{
    HSSDisplayObject::setDefaults();
    this->setDefaultKw("contentAlignX", "left");
    this->setDefaultKw("contentAlignY", "top");
    this->setDefaultKw("direction", "ltr");
    this->setDefaultKw("wrapDirection", "ttb");
    this->setDefaultKw("shape", "default");
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

QSharedPointer<HSSParserNode> HSSContainer::getPercentageExpression(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    if (!parserNode || !parserNode->isA(HSSParserNodeTypePercentageConstant))
    {
        AXRError("HSSContainer", "You can only create a percentage expression from a percentage constant.");
        return QSharedPointer<HSSParserNode>();
    }

    static QMap<AXRString, AXRString> mappings;
    if (mappings.empty())
    {
        mappings.insert("contentAlignX", "innerWidth");
        mappings.insert("contentAlignY", "innerHeight");
    }

    if (mappings.contains(propertyName))
    {
        HSSUnit number = qSharedPointerCast<HSSPercentageConstant>(parserNode)->getNumber();
        //get the properties from itself
        return this->getPercentageExpressionFromThis(number, mappings[propertyName]);
    }

    return HSSDisplayObject::getPercentageExpression(parserNode, propertyName);
}

//children
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
    this->changeRulesNotifyAdd(child);
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
            this->changeRulesNotifyRemove(child);
        }
        else
        {
            ++it2;
        }
    }

    this->resetChildrenIndexes();
}

void HSSContainer::changeRulesAdd(QSharedPointer<HSSRule> theRule)
{
    if (!this->_changeRules.contains(theRule))
    {
        this->_changeRules.append(theRule);
    }
}

void HSSContainer::changeRulesNotifyAdd(QSharedPointer<HSSDisplayObject> theDO)
{
    Q_FOREACH(const QSharedPointer<HSSRule> & theRule, this->_changeRules)
    {
        QSharedPointer<HSSSimpleSelection> theScope(new HSSSimpleSelection(this->getController()));
        theScope->add(theDO);
        this->getController()->recursiveMatchRulesToDisplayObjects(theRule, theScope, this->shared_from_this(), true);
        this->getController()->recursiveSetRuleState(theRule, theScope, this->shared_from_this(), HSSRuleStateActivate);
    }
}

void HSSContainer::changeRulesNotifyRemove(QSharedPointer<HSSDisplayObject> theDO)
{
    Q_FOREACH(const QSharedPointer<HSSRule> & theRule, this->_changeRules)
    {
        theRule->removeFromDisplayObject(theDO);
    }
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

void HSSContainer::setContentText(const AXRString &contentText)
{
    AXRString text = contentText.trimmed();

    AXRController * controller = this->getController();
    QSharedPointer<HSSTextBlock> txtBlck;

    if (!text.isEmpty())
    {
        for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it!= this->allChildren->end(); ++it)
        {
            QSharedPointer<HSSDisplayObject> child = *it;
            if (child->isA(HSSObjectTypeTextBlock))
            {
                txtBlck = qSharedPointerCast<HSSTextBlock > (child);
            }
        }
        if ( ! txtBlck)
        {
            txtBlck = QSharedPointer<HSSTextBlock>(new HSSTextBlock(controller));
            this->add(txtBlck);
        }

        txtBlck->setText(text, std::numeric_limits<int>::max());
    }
}

void HSSContainer::appendContentText(const AXRString &contentText)
{
    AXRString text = contentText.trimmed();

    AXRController * controller = this->getController();

    if (!text.isEmpty())
    {
        if (this->allChildren->empty())
        {
            QSharedPointer<HSSTextBlock> txtBlck = QSharedPointer<HSSTextBlock>(new HSSTextBlock(controller));
            txtBlck->setText(text, std::numeric_limits<int>::max());
            this->add(txtBlck);
        }
        else
        {
            QSharedPointer<HSSDisplayObject> lastChild = this->allChildren->back();
            if (lastChild->isA(HSSObjectTypeTextBlock))
            {
                QSharedPointer<HSSTextBlock> textBlock = qSharedPointerCast<HSSTextBlock > (lastChild);
                text = textBlock->getText() + " " + text;
                textBlock->setText(text, std::numeric_limits<int>::max());
            }
            else
            {
                QSharedPointer<HSSTextBlock> txtBlck = QSharedPointer<HSSTextBlock>(new HSSTextBlock(controller));
                txtBlck->setText(text, std::numeric_limits<int>::max());
                this->add(txtBlck);
            }
        }
    }
}

void HSSContainer::accept(HSSAbstractVisitor* visitor, HSSVisitorFilterFlags filterFlags)
{
    if ((filterFlags & HSSVisitorFilterLayout))
    {
        for (HSSSimpleSelection::iterator child = this->allChildren->begin(); child != this->allChildren->end(); ++child)
        {
            (*child)->accept(visitor, filterFlags);
        }
    }

    visitor->visit(*this);

    if (!(filterFlags & HSSVisitorFilterTraverse))
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

    if(!handled)
    {
        bool superHandled = HSSDisplayObject::handleEvent(event);
        if (superHandled)
        {
            handled = true;
        }
    }

    return handled;
}

void HSSContainer::_setIsA(QSharedPointer<HSSObject> theObj)
{
    QVector<QSharedPointer<HSSRule> > newRuleSet;
    if (theObj->isA(HSSObjectTypeMultipleValue))
    {
        Q_FOREACH(QSharedPointer<HSSObject> mvObj, qSharedPointerCast<HSSMultipleValue>(theObj)->getValues()){
            Q_FOREACH(QSharedPointer<HSSRule> tmprule, mvObj->getObjDefRules()){
                newRuleSet.push_back(tmprule);
            }
        }
        Q_FOREACH(QSharedPointer<HSSObject> mvObj, qSharedPointerCast<HSSMultipleValue>(theObj)->getValues())
        {
            HSSObject::_setIsA(mvObj);
        }
    }
    else
    {
        if (!theObj->isA(HSSObjectTypeContainer))
        {
            return;
        }

        newRuleSet = theObj->getObjDefRules();
        HSSObject::_setIsA(theObj);
    }

    QSharedPointer<HSSContainer> thisContainer = qSharedPointerCast<HSSContainer>(this->shared_from_this());
    QVector<QSharedPointer<HSSRule> > currentRules(this->_appliedIsARules);
    QVector<QSharedPointer<HSSRule> > newRules;

    //find the new rules to add
    Q_FOREACH(QSharedPointer<HSSRule> rule, theObj->getObjDefRules()){
        bool found = false;
        Q_FOREACH(QSharedPointer<HSSRule> tmprule, currentRules)
        {
            if (tmprule->equalTo(rule))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            newRules.push_back(rule);
        }
    }
    //remove obsolete rules
    Q_FOREACH(QSharedPointer<HSSRule> tmprule, currentRules)
    {
        bool found = false;
        Q_FOREACH(QSharedPointer<HSSRule> tmprule2, newRuleSet)
        {
            if (tmprule->equalTo(tmprule2))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            tmprule->removeFromDisplayObjects();
            for (int i = 0, size = this->_appliedIsARules.size(); i<size; ++i)
            {
                if (this->_appliedIsARules[i] == tmprule)
                {
                    this->_appliedIsARules.remove(i);
                    this->setNeedsRereadRules(true);
                    --i;
                    --size;
                }
            }
        }
    }
    Q_FOREACH(QSharedPointer<HSSRule> rule, newRules){
        rule->setSpecificity(theObj->getSpecificity());
        this->_appliedIsARules.push_back(rule);
        this->objDefRulesPrepend(rule);
        if (this->expectsIsAIncluded()) {
            AXRController * controller = this->getController();
            controller->currentContextPush(thisContainer);
            controller->recursiveMatchRulesToDisplayObjects(rule, thisContainer->getChildren(), thisContainer, true);
            controller->recursiveSetRuleState(rule, thisContainer->getChildren(), thisContainer, HSSRuleStateOn);
            controller->currentContextPop();
        }
    }
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

//contentAlignX
HSSUnit HSSContainer::getContentAlignX() const
{
    return this->getComputedNumber("contentAlignX");
}

QSharedPointer<HSSObject> HSSContainer::computeContentAlignX(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "left")
            {
                return this->computeValueObject(this->percentageToConstant(0), "contentAlignX");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "contentAlignX");
            }
            else if (kwValue == "right")
            {
                return this->computeValueObject(this->percentageToConstant(100), "contentAlignX");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "contentAlignX");
}

void HSSContainer::notifyContentAlignX(QSharedPointer<HSSObject> theObject)
{
    this->distributeX = false;
    this->distributeXLinear = false;

    if (theObject->isA(HSSObjectTypeValue) && qSharedPointerCast<HSSValue>(theObject)->getValue()->isA(HSSParserNodeTypeKeywordConstant))
    {
        QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(qSharedPointerCast<HSSValue>(theObject)->getValue());
        AXRString kwValue = theKW->getValue();
        if (kwValue == "even")
        {
            this->distributeX = true;
            this->distributeXLinear = false;
        }
        else if (kwValue == "justify")
        {
            this->distributeX = true;
            this->distributeXLinear = true;
        }
    }
    this->notifyObservers("contentAlignX", theObject);
}

//contentAlignY
HSSUnit HSSContainer::getContentAlignY() const
{
    return this->getComputedNumber("contentAlignY");
}

QSharedPointer<HSSObject> HSSContainer::computeContentAlignY(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "top")
            {
                return this->computeValueObject(this->percentageToConstant(0), "contentAlignY");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "contentAlignY");
            }
            else if (kwValue == "bottom")
            {
                return this->computeValueObject(this->percentageToConstant(100), "contentAlignY");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "contentAlignY");
}

void HSSContainer::notifyContentAlignY(QSharedPointer<HSSObject> theObject)
{
    this->distributeY = false;
    this->distributeYLinear = false;

    if (theObject->isA(HSSObjectTypeValue) && qSharedPointerCast<HSSValue>(theObject)->getValue()->isA(HSSParserNodeTypeKeywordConstant))
    {
        QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(qSharedPointerCast<HSSValue>(theObject)->getValue());
        AXRString kwValue = theKW->getValue();
        if (kwValue == "even")
        {
            this->distributeY = true;
            this->distributeYLinear = false;
        }
        else if (kwValue == "justify")
        {
            this->distributeY = true;
            this->distributeYLinear = true;
        }
    }
    this->notifyObservers("contentAlignY", theObject);
}

//direction
HSSDirectionValue HSSContainer::getDirection() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("direction");
    if (value && value->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(value)->getValue();
        if (parserNode && parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            AXRString kwValue = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (kwValue == "ltr")
            {
                return HSSDirectionLeftToRight;
            }
            if (kwValue == "rtl")
            {
                return HSSDirectionRightToLeft;
            }
            if (kwValue == "ttb")
            {
                return HSSDirectionTopToBottom;
            }
            if (kwValue == "btt")
            {
                return HSSDirectionBottomToTop;
            }
        }
    }
    return HSSDirectionLeftToRight;
}

//wrapDirection
HSSDirectionValue HSSContainer::getWrapDirection() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("wrapDirection");
    if (value && value->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(value)->getValue();
        if (parserNode && parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            AXRString kwValue = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (kwValue == "ltr")
            {
                return HSSDirectionLeftToRight;
            }
            if (kwValue == "rtl")
            {
                return HSSDirectionRightToLeft;
            }
            if (kwValue == "ttb")
            {
                return HSSDirectionTopToBottom;
            }
            if (kwValue == "btt")
            {
                return HSSDirectionBottomToTop;
            }
        }
    }
    return HSSDirectionLeftToRight;
}

//content
AXRString HSSContainer::getContent() const
{
    return this->getComputedString("content");
}
QSharedPointer<HSSObject> HSSContainer::computeContent(QSharedPointer<HSSParserNode> parserNode)
{
    if (parserNode->isA(HSSParserNodeTypeStringConstant))
    {
        AXRString text = qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
        this->setContentText(text);
    }
    return this->computeObject(parserNode, "content");
}

//shape
QSharedPointer<HSSShape> HSSContainer::getShape()
{
    return qSharedPointerCast<HSSShape>(this->getComputedValue("shape"));
}

QSharedPointer<HSSObject> HSSContainer::computeShape(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "default")
            {
                return QSharedPointer<HSSRectangle>(new HSSRectangle(this->getController()));
            }
            break;
        }
        case HSSParserNodeTypeNumberConstant:
        {
            QSharedPointer<HSSNumberConstant> theNum = qSharedPointerCast<HSSNumberConstant>(parserNode);
            QSharedPointer<HSSRoundedRect> ret = QSharedPointer<HSSRoundedRect>(new HSSRoundedRect(this->getController()));
            ret->stackCorners(theNum);
            return ret;
        }

        default:
            break;
    }

    return this->computeObject(parserNode, "shape");
}

void HSSContainer::notifyShape(QSharedPointer<HSSObject> theObj)
{
    this->setDirty(true);
    this->notifyObservers("shape", theObj);
}

void HSSContainer::shapeChanged(const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->setDirty(true);
    this->notifyObservers("shape", theObj);
}
