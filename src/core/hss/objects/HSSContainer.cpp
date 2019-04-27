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
#include "axr.h"
#include "AXRController.h"

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
    this->addCallback("content", new HSSObserveCallback<HSSContainer>(this, &HSSContainer::setContent));
    this->addNotifyCallback("content", new HSSObserveCallback<HSSContainer>(this, &HSSContainer::notifyContent));
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

    static std::map<AXRString, AXRString> mappings;
    if (mappings.empty())
    {
        mappings["contentAlignX"] = "innerWidth";
        mappings["contentAlignY"] = "innerHeight";
    }

    if (mappings.count(propertyName))
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
    bool isTxtBlock = child->isA(HSSObjectTypeTextBlock);
    if (!isTxtBlock)
    {
        this->children->add(child);
    }
    this->allChildren->add(child);
//    this->changeRulesNotifyAdd(child);
    if (!isTxtBlock)
    {
        this->notifyObservers("__impl_private__contentTreeChanged", sharedThis);
    }
}

void HSSContainer::insert(QSharedPointer<HSSDisplayObject> child, int index)
{
    QSharedPointer<HSSContainer> sharedThis = this->shared_from_this();
    child->setParent(sharedThis);
    axr_log(LoggerChannelGeneralSpecific, "HSSContainer: added child " + child->getElementName() + " to " + this->getElementName());
    child->setIndex(index);
    bool isTxtBlock = child->isA(HSSObjectTypeTextBlock);
    if (!isTxtBlock)
    {
        this->children->insert(child, index);
    }
    this->allChildren->insert(child, index);
    //    this->changeRulesNotifyAdd(child);
    if (!isTxtBlock)
    {
        this->notifyObservers("__impl_private__contentTreeChanged", sharedThis);
    }
}

void HSSContainer::addOffscreen(QSharedPointer<HSSDisplayObject> child)
{
    QSharedPointer<HSSContainer> sharedThis = this->shared_from_this();
    child->setParent(sharedThis);
    axr_log(LoggerChannelGeneralSpecific, "HSSContainer: added child " + child->getElementName() + " to " + this->getElementName() + "'s offscreen list");
    this->offscreenChildren.push_back(child);
}

void HSSContainer::removeFromOffscreen(QSharedPointer<HSSDisplayObject> child)
{
    std::vector<QSharedPointer<HSSDisplayObject> >::iterator it;
    it = find (this->offscreenChildren.begin(), this->offscreenChildren.end(), child);
    if (it != this->offscreenChildren.end()){
        this->offscreenChildren.erase(it);
    }
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
    if (!this->_changeRules.count(theRule))
    {
        this->_changeRules.insert(theRule);
    }
}

void HSSContainer::changeRulesNotifyAdd(QSharedPointer<HSSDisplayObject> theDO)
{
    std::set<QSharedPointer<HSSRule> >::const_iterator it;
    for (it=_changeRules.begin(); it!=_changeRules.end(); ++it)
    {
        const QSharedPointer<HSSRule> & theRule = *it;
        QSharedPointer<HSSSimpleSelection> theScope(new HSSSimpleSelection(this->getController()));
        theScope->add(theDO);
        this->getController()->recursiveMatchRulesToDisplayObjects(theRule, theScope, this->shared_from_this(), true);
        this->getController()->recursiveSetRuleState(theRule, theScope, this->shared_from_this(), HSSRuleStateActivate);
    }
}

void HSSContainer::changeRulesNotifyRemove(QSharedPointer<HSSDisplayObject> theDO)
{
    std::set<QSharedPointer<HSSRule> >::const_iterator it;
    for (it=_changeRules.begin(); it!=_changeRules.end(); ++it)
    {
        const QSharedPointer<HSSRule> & theRule = *it;
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
    AXRString text = contentText;

    AXRController * controller = this->getController();

    if (text == "\x7f")
    {
        if (!this->allChildren->empty())
        {
            QSharedPointer<HSSDisplayObject> lastChild = this->allChildren->back();
            if (lastChild->isA(HSSObjectTypeTextBlock))
            {
                QSharedPointer<HSSTextBlock> textBlock = qSharedPointerCast<HSSTextBlock > (lastChild);
                AXRString currentText = textBlock->getText();
                currentText.truncate(currentText.length()-1);
                textBlock->setText(currentText, std::numeric_limits<int>::max());
            }
        }
    }
    else
    {
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
                    text = textBlock->getText() + text;
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
}

void HSSContainer::removeContentText()
{
    std::vector<QSharedPointer<HSSDisplayObject> > items = this->allChildren->getItems();
    for (std::vector<QSharedPointer<HSSDisplayObject> >::const_iterator it = items.begin(); it!= items.end(); ++it) {
        const QSharedPointer<HSSDisplayObject> & theDO = *it;
        if (theDO->isA(HSSObjectTypeTextBlock))
        {
            theDO->removeFromParent();
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

    unsigned i;
    for (i = 0; i < this->allChildren->size(); ++i)
    {
        QSharedPointer<HSSDisplayObject> child = this->allChildren->itemAtIndex(i);
        child->accept(visitor, filterFlags);
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
    for (size_t i = 0, j = this->allChildren->size(); i<j; i+=1)
    {
        QSharedPointer<HSSDisplayObject> child = this->allChildren->itemAtIndex((int)i);
        bool childHandled = child->handleEvent(event);
        if (childHandled)
        {
            handled = true;
            break;
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

void HSSContainer::unsetAllActive()
{
    for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it < this->allChildren->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> child = *it;
        child->unsetAllActive();
    }
    HSSDisplayObject::unsetAllActive();
}

bool HSSContainer::handleSelection(HSSPoint thePoint)
{
    bool hasBeenHandled = false;
    if (this->allChildren->size() > 0)
    {
        for (HSSSimpleSelection::iterator it = this->allChildren->begin(); it < this->allChildren->end(); ++it)
        {
            QSharedPointer<HSSDisplayObject> child = *it;
            bool childHandled = child->handleSelection(thePoint);
            if (childHandled) {
                hasBeenHandled = true;
                break;
            }
        }
    }
    if (!hasBeenHandled)
    {
        hasBeenHandled = HSSDisplayObject::handleSelection(thePoint);
    }
    return hasBeenHandled;
}

void HSSContainer::_setIsA(QSharedPointer<HSSObject> theObj)
{
    std::deque<QSharedPointer<HSSRule> > newRuleSet;
    if (theObj->isA(HSSObjectTypeMultipleValue))
    {
        const std::vector<QSharedPointer<HSSObject> > multiValues = qSharedPointerCast<HSSMultipleValue>(theObj)->getValues();
        std::vector<QSharedPointer<HSSObject> >::const_iterator it;
        for (it = multiValues.begin(); it != multiValues.end(); ++it)
        {
            const QSharedPointer<HSSObject> mvObj = *it;
            const std::deque<QSharedPointer<HSSRule> > objDefRules = mvObj->getObjDefRules();
            std::deque<QSharedPointer<HSSRule> >::const_iterator it2;
            for (it2 = objDefRules.begin(); it2 != objDefRules.end(); ++it2)
            {
                const QSharedPointer<HSSRule> & tmprule = *it2;
                newRuleSet.push_back(tmprule);
            }
        }
        for (it = multiValues.begin(); it != multiValues.end(); ++it)
        {
            const QSharedPointer<HSSObject> mvObj = *it;
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
    std::vector<QSharedPointer<HSSRule> > currentRules(this->_appliedIsARules);
    std::vector<QSharedPointer<HSSRule> > newRules;

    //find the new rules to add
    const std::deque<QSharedPointer<HSSRule> > & objDefRules = theObj->getObjDefRules();
    std::deque<QSharedPointer<HSSRule> >::const_iterator it;
    for (it = objDefRules.begin(); it != objDefRules.end(); ++it)
    {
        const QSharedPointer<HSSRule> & rule = *it;
        bool found = false;
        
        std::vector<QSharedPointer<HSSRule> >::const_iterator it2;
        for (it2 = currentRules.begin(); it2 != currentRules.end(); ++it2)
        {
            const QSharedPointer<HSSRule> & tmprule = *it2;
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
    std::vector<QSharedPointer<HSSRule> >::const_iterator it3;
    for (it3 = currentRules.begin(); it3 != currentRules.end(); ++it3)
    {
        const QSharedPointer<HSSRule> & tmprule = *it3;
        bool found = false;
        
        std::deque<QSharedPointer<HSSRule> >::const_iterator it4;
        for (it4 = newRuleSet.begin(); it4 != newRuleSet.end(); ++it4)
        {
            const QSharedPointer<HSSRule> & tmprule2 = *it4;
            if (tmprule->equalTo(tmprule2))
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            tmprule->removeFromDisplayObjects();
            for (size_t i = 0, size = this->_appliedIsARules.size(); i<size; ++i)
            {
                if (this->_appliedIsARules[i] == tmprule)
                {
                    this->_appliedIsARules.erase(this->_appliedIsARules.begin() + i);
                    this->setNeedsRereadRules(true);
                    --i;
                    --size;
                }
            }
        }
    }
    std::vector<QSharedPointer<HSSRule> >::iterator it5;
    for (it5 = newRules.begin(); it5 != newRules.end(); ++it5)
    {
        const QSharedPointer<HSSRule> & rule = *it5;
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
void HSSContainer::setContent(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
        switch (parserNode->getType())
        {
            case HSSParserNodeTypeNumberConstant:
            {
                QSharedPointer<HSSNumberConstant> theNum = qSharedPointerCast<HSSNumberConstant>(parserNode);
                theObj = this->computeValue("content", HSSStringConstant::string(HSSString::number(theNum->getValue()), this->getController()));
                break;
            }
            case HSSParserNodeTypeFunction:
            {
                QSharedPointer<HSSFunction> function = qSharedPointerCast<HSSFunction>(parserNode);
                QSharedPointer<HSSObject> remoteObj = function->evaluate();
                if(remoteObj && remoteObj->isA(HSSObjectTypeValue)){
                    QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(remoteObj);
                    AXRString text = valueObj->getString();
                    this->setContentText(text.stripQuotes());
                }
                break;
            }
            case HSSParserNodeTypePropertyPath:
            {
                QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath> (parserNode);
                QSharedPointer<HSSObject> searchResult = ppath->evaluate();
                ppath->observe();
                if (searchResult && searchResult->isA(HSSObjectTypeValue))
                {
                    QSharedPointer<HSSValue> valueObj = qSharedPointerCast<HSSValue>(searchResult);
                    AXRString text = valueObj->getString();
                    this->setContentText(text.stripQuotes());
                }
                else
                {
                    AXRWarning("HSSContainer", "Path " + ppath->stringRep() + " did not yield any results.").raise();
                }
            }
            default:
                break;
        }
    }
    this->_simpleInsertComputed("content", theObj);
}

void HSSContainer::notifyContent(QSharedPointer<HSSObject> theObj)
{
    if (theObj)
    {
        if (theObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
            switch (parserNode->getType())
            {
                case HSSParserNodeTypeStringConstant:
                {
                    QSharedPointer<HSSStringConstant> theString = qSharedPointerCast<HSSStringConstant>(parserNode);
                    AXRString strWithQuotes = theString->getValue();
                    AXRString newStr = strWithQuotes.stripQuotes();
                    this->setContentText(newStr);
                    this->setNeedsLayout(true);
                    this->setDirty(true);
                    this->getController()->document()->setNeedsDisplay(true);
                    break;
                }
                case HSSParserNodeTypeNumberConstant:
                {
                    QSharedPointer<HSSNumberConstant> theNum = qSharedPointerCast<HSSNumberConstant>(parserNode);
                    this->setContentText(HSSString::number(theNum->getValue()));
                    this->setNeedsLayout(true);
                    this->setDirty(true);
                    this->getController()->document()->setNeedsDisplay(true);
                    break;
                }
                default:
                    break;
            }
        }
    }
    else
    {
        this->removeContentText();
        this->setNeedsLayout(true);
        this->setDirty(true);
        this->getController()->document()->setNeedsDisplay(true);
    }

    this->notifyObservers("content", theObj);
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
