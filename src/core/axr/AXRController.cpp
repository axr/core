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

#include "AXRController.h"
#include "AXRDebugging.h"
#include "AXRDocument.h"
#include "AXRWarning.h"
#include "HSSDisplayObject.h"
#include "HSSFlag.h"
#include "HSSMultipleSelection.h"
#include "HSSNegation.h"
#include "HSSNumberConstant.h"
#include "HSSSelection.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"

using namespace AXR;

AXRController::AXRController(AXRDocument *document)
: document_(document)
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRController: creating controller");
    this->currentContext = std::stack<HSSContainer::p > ();
}

AXRController::~AXRController()
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRController: destructing controller");

    this->objectTree.clear();
    this->loadSheets.clear();
    this->rules.clear();
    this->parserTree.clear();
}

AXRDocument* AXRController::document() const
{
    return document_;
}

void AXRController::matchRulesToContentTree()
{
    const std::vector<HSSRule::p> rules = this->getRules();
    HSSSimpleSelection::p rootScope(new HSSSimpleSelection());
    rootScope->add(this->getRoot());

    for (unsigned i = 0, size = rules.size(); i < size; ++i)
    {
        const HSSRule::p& rule = rules[i];
        std::vector<HSSSelectorChain::p> selectorChains = rule->getSelectorChains();
        if (!selectorChains.empty())
        {
            HSSSelection::p selection;
            try
            {
                selection = this->select(selectorChains, rootScope, this->getRoot(), true);
            }
            catch (const AXRError &e)
            {
                e.raise();
            }

            if (selection->isA(HSSSelectionTypeMultipleSelection))
            {
                HSSMultipleSelection::p multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
                for (HSSMultipleSelection::iterator it = multiSel->begin(); it != multiSel->end(); ++it)
                {
                    this->_matchRuleToSelection(rule, qSharedPointerCast<HSSSimpleSelection>(*it));
                }
            }
            else if (selection->isA(HSSSelectionTypeSimpleSelection))
            {
                this->_matchRuleToSelection(rule, qSharedPointerCast<HSSSimpleSelection>(selection));
            }
        }
    }
}

inline void AXRController::_matchRuleToSelection(HSSRule::p rule, HSSSimpleSelection::p selection)
{
    for (HSSSimpleSelection::const_iterator it = selection->begin(); it != selection->end(); ++it)
    {
        const HSSDisplayObject::p & displayObject = *it;
        axr_log(AXR_DEBUG_CH_GENERAL, "AXRController: match " + displayObject->getElementName());
        displayObject->rulesAdd(rule, HSSRuleStateOff);

        displayObject->setNeedsRereadRules(true);
        displayObject->setNeedsSurface(true);
        displayObject->setDirty(true);

        //if it is a container it may have children
        if (displayObject->isA(HSSObjectTypeContainer))
        {
            HSSContainer::p selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
            this->currentContext.push(selectedContainer);
            for (unsigned k = 0, size3 = rule->childrenSize(); k < size3; ++k)
            {
                const HSSRule::p childRule = rule->childrenGet(k);
                this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer, true);
            }
            this->currentContext.pop();
        }

        displayObject->setNeedsRereadRules(true);
    }
}

void AXRController::recursiveMatchRulesToDisplayObjects(const HSSRule::p & rule, HSSSelection::p scope, HSSContainer::p container, bool applyingInstructions)
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRController: recursive matching rules to display objects");
    HSSInstruction::p instruction = rule->getInstruction();
    if (instruction && applyingInstructions)
    {
        switch (instruction->getInstructionType())
        {
        case HSSNewInstruction:
        {
            if (container)
            {
                HSSSimpleSelector::p subject = rule->selectorChainsLast()->subject();
                AXRString elementName = "";
                if (subject)
                {
                    HSSNameSelector::p nameSel = subject->getName();
                    elementName = nameSel->getElementName();
                }

                unsigned i;
                unsigned argssize = 1;
                HSSParserNode::p argument = instruction->getArgument();
                if (argument)
                {
                    if (argument->isA(HSSParserNodeTypeNumberConstant))
                    {
                        HSSNumberConstant::p argnum = qSharedPointerCast<HSSNumberConstant>(argument);
                        argssize = (int) argnum->getValue();
                    }
                }

                for (i = 0; i < argssize; ++i)
                {
                    HSSContainer::p newContainer = HSSContainer::p(new HSSContainer(this));
                    newContainer->setName(elementName);
                    newContainer->setElementName(elementName);
                    rule->setThisObj(newContainer);
                    newContainer->rulesAdd(rule, HSSRuleStateOn);
                    axr_log(AXR_DEBUG_CH_GENERAL, "AXRController: created " + newContainer->getElementName());
                    this->add(newContainer);
                    newContainer->setNeedsRereadRules(true);
                    newContainer->setNeedsSurface(true);
                    newContainer->setDirty(true);
                    unsigned i, size;
                    this->currentContext.push(newContainer);
                    for (i = 0, size = rule->childrenSize(); i < size; ++i)
                    {
                        const HSSRule::p childRule = rule->childrenGet(i);
                        this->recursiveMatchRulesToDisplayObjects(childRule, newContainer->getChildren(), newContainer, applyingInstructions);
                    }
                    newContainer->setNeedsRereadRules(true);
                    //newContainer->fireEvent(HSSEventTypeLoad);
                    this->currentContext.pop();
                }
            }
            else
            {
                AXRWarning("AXRController", "You cannot use the #new instruction at root level").raise();
            }

            break;
        }

        case HSSMoveInstruction:
        {
            if (container)
            {
                HSSContainer::p parent = container->getParent();
                if (parent)
                {
                    HSSSimpleSelection::p moveScope = parent->getChildren();
                    HSSSelection::p selection = this->select(rule->getSelectorChains(), moveScope, container);

                    this->currentContext.push(container);
                    //move the children over
                    HSSSimpleSelection::p inner = selection->joinAll();
                    for (HSSSimpleSelection::const_iterator it=inner->begin(); it!=inner->end(); ++it)
                    {
                        const HSSDisplayObject::p & theDO = *it;
                        if (theDO != container)
                        {
                            theDO->removeFromParent();
                            rule->setThisObj(theDO);
                            theDO->rulesAdd(rule, HSSRuleStateOn);
                            axr_log(AXR_DEBUG_CH_GENERAL, "AXRController: moved " + theDO->getElementName());
                            this->add(theDO);
                            theDO->setNeedsRereadRules(true);
                            theDO->setNeedsSurface(true);
                            theDO->setDirty(true);

                            if (theDO->isA(HSSObjectTypeContainer))
                            {
                                HSSContainer::p theContainer = qSharedPointerCast<HSSContainer>(theDO);
                                this->currentContext.push(theContainer);
                                //assign more rules
                                for (unsigned i = 0, size = rule->childrenSize(); i < size; ++i)
                                {
                                    const HSSRule::p childRule = rule->childrenGet(i);
                                    this->recursiveMatchRulesToDisplayObjects(childRule, theContainer->getChildren(), theContainer, applyingInstructions);
                                }
                                this->currentContext.pop();
                            }

                            theDO->setNeedsRereadRules(true);
                            //theDO->fireEvent(HSSEventTypeLoad);
                        }
                    }
                    this->currentContext.pop();
                }
                else
                {
                    AXRWarning("AXRController", "You cannot use the #move instruction at root level").raise();
                }
            }

            break;
        }

        case HSSDeleteInstruction:
        {
            if (container)
            {
                //select the items to be deleted
                HSSSelection::p selection = this->select(rule->getSelectorChains(), scope, container);
                HSSSimpleSelection::p inner = selection->joinAll();
                for (HSSSimpleSelection::const_iterator it = inner->begin(); it != inner->end(); ++it)
                {
                    (*it)->removeFromParent();
                }
            }
            else
            {
                AXRWarning("AXRController", "You cannot use the #delete instruction at root level").raise();
            }
            break;
        }

        default:
            break;
        }

    }
    else if (!instruction)
    {
        std::vector<HSSSelectorChain::p> selectorChains = rule->getSelectorChains();
        if (!selectorChains.empty())
        {
            HSSSelection::p selection;

            if (container)
            {
                if (applyingInstructions)
                {
                    //we observe the parent for dom changes
                    container->observe(HSSObservablePropertyTreeChange, HSSObservablePropertyValue, rule.data(), new HSSValueChangedCallback<HSSRule>(rule.data(), &HSSRule::treeChanged));
                    rule->setObservedTreeChanger(container.data());
                    rule->setThisObj(container);
                    rule->setOriginalScope(scope->joinAll());
                }

                try
                {
                    selection = this->select(selectorChains, scope, container, true);
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }
            else
            {
                try
                {
                    selection = this->select(selectorChains, scope, this->getRoot(), true);
                }
                catch (const AXRError &e)
                {
                    e.raise();
                }
            }

            if (selection->isA(HSSSelectionTypeMultipleSelection))
            {
                HSSMultipleSelection::p multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
                for (HSSMultipleSelection::iterator it = multiSel->begin(); it != multiSel->end(); ++it)
                {
                    this->_recursiveMatchRulesToDisplayObjects(rule, *it, container, applyingInstructions);
                }
            }
            else if (selection->isA(HSSSelectionTypeSimpleSelection))
            {
                HSSSimpleSelection::p simpleSel = qSharedPointerCast<HSSSimpleSelection>(selection);
                this->_recursiveMatchRulesToDisplayObjects(rule, simpleSel, container, applyingInstructions);
            }
        }
    }
}

inline void AXRController::_recursiveMatchRulesToDisplayObjects(const HSSRule::p & rule, HSSSimpleSelection::p scope, HSSContainer::p container, bool applyingInstructions)
{
    for (HSSSimpleSelection::const_iterator it = scope->begin(); it != scope->end(); ++it)
    {
        const HSSDisplayObject::p & displayObject = *it;
        axr_log(AXR_DEBUG_CH_GENERAL, "AXRController: match " + displayObject->getElementName());
        displayObject->rulesAdd(rule, HSSRuleStateOff);

        displayObject->setNeedsRereadRules(true);
        displayObject->setNeedsSurface(true);
        displayObject->setDirty(true);

        //if it is a container it may have children
        if (displayObject->isA(HSSObjectTypeContainer))
        {
            HSSContainer::p selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
            this->currentContext.push(selectedContainer);
            for (unsigned k = 0, size3 = rule->childrenSize(); k < size3; ++k)
            {
                const HSSRule::p childRule = rule->childrenGet(k);
                this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer, applyingInstructions);
            }
            this->currentContext.pop();
        }
        displayObject->setNeedsRereadRules(true);
    }
}

void AXRController::activateRules()
{
    const std::vector<HSSRule::p> rules = this->getRules();
    HSSSimpleSelection::p rootScope(new HSSSimpleSelection());
    rootScope->add(this->getRoot());
    
    for (unsigned i = 0, size = rules.size(); i < size; ++i)
    {
        const HSSRule::p& rule = rules[i];
        this->activateRules(rule, rootScope);
    }
}

void AXRController::activateRules(HSSRule::p rule, HSSSelection::p scope)
{
    if (!rule->getInstruction()) {
        std::vector<HSSSelectorChain::p> selectorChains = rule->getSelectorChains();
        if (!selectorChains.empty())
        {
            HSSSelection::p selection;
            try
            {
                selection = this->select(selectorChains, scope, this->getRoot());
            }
            catch (const AXRError &e)
            {
                e.raise();
            }

            if (selection->isA(HSSSelectionTypeMultipleSelection))
            {
                HSSMultipleSelection::p multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
                for (HSSMultipleSelection::iterator it = multiSel->begin(); it != multiSel->end(); ++it)
                {
                    this->_activateRuleOnSelection(rule, qSharedPointerCast<HSSSimpleSelection>(*it));
                }
            }
            else if (selection->isA(HSSSelectionTypeSimpleSelection))
            {
                this->_activateRuleOnSelection(rule, qSharedPointerCast<HSSSimpleSelection>(selection));
            }
        }
    }
}

void AXRController::_activateRuleOnSelection(HSSRule::p rule, HSSSimpleSelection::p selection)
{
    for (HSSSimpleSelection::const_iterator it = selection->begin(); it != selection->end(); ++it)
    {
        const HSSDisplayObject::p & displayObject = *it;
        axr_log(AXR_DEBUG_CH_GENERAL, "AXRController: activating rule on " + displayObject->getElementName());
        displayObject->setRuleStatus(rule, HSSRuleStateOn);

        //if it is a container it may have children
        if (displayObject->isA(HSSObjectTypeContainer))
        {
            HSSContainer::p selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
            for (unsigned k = 0, size3 = rule->childrenSize(); k < size3; ++k)
            {
                const HSSRule::p childRule = rule->childrenGet(k);
                HSSInstruction::p theInst = childRule->getInstruction();
                if (!theInst) {
                    HSSSimpleSelection::p children = this->select(childRule->getSelectorChains(), selectedContainer->getChildren(), selectedContainer)->joinAll();
                    this->_activateRuleOnSelection(childRule, children);
                }
            }
            this->currentContext.pop();
        }
        displayObject->setNeedsRereadRules(true);
    }
}

void AXRController::initializeSelectorChain(HSSSelectorChain::p selectorChain)
{
    this->currentChain = selectorChain;
    this->currentChainSize = selectorChain->size();
    this->currentChainCount = 0;
    this->currentSelectorNode = selectorChain->get(0);
}

void AXRController::readNextSelectorNode()
{
    if (this->currentChainCount < this->currentChainSize - 1)
    {
        this->currentSelectorNode = this->currentChain->get(this->currentChainCount + 1);
        this->currentChainCount++;
    }
}

bool AXRController::isAtEndOfSelector()
{
    return this->currentChainCount >= this->currentChainSize - 1;
}

HSSSelection::p AXRController::select(std::vector<HSSSelectorChain::p> selectorChains, HSSSelection::p scope, HSSDisplayObject::p thisObj)
{
    return this->select(selectorChains, scope, thisObj, false);
}

HSSSelection::p AXRController::select(std::vector<HSSSelectorChain::p> selectorChains, HSSSelection::p scope, HSSDisplayObject::p thisObj, bool processing)
{
    HSSMultipleSelection::p ret(new HSSMultipleSelection());

    for (HSSSelectorChain::const_it it = selectorChains.begin(); it != selectorChains.end(); ++it)
    {
        this->initializeSelectorChain(*it);

        //if we have a descendants combinator here, it means we were at root context, since the parser
        //automatically prepends a @this before it when in another context
        if(this->currentSelectorNode->isA(HSSSelectorTypeCombinator)){
            HSSCombinatorType combinatorType = this->currentSelectorNode->getCombinatorType();
            if(combinatorType == HSSCombinatorTypeDescendants){
                this->readNextSelectorNode();
                HSSSelection::p sel = this->selectAllHierarchical(scope, thisObj, true);
                ret->addSelection(sel);
            } else {
                AXRError("AXRController", "Selector chains at root level can't start with combinators except . (unimplemented) and ..").raise();
            }
        } else {
            HSSSelection::p sel = this->selectHierarchical(scope, thisObj, processing);
            ret->addSelection(sel);
        }
    }
    if (ret->size() == 1){
        for (HSSMultipleSelection::iterator it = ret->begin(); it!=ret->end(); ++it) {
            HSSSimpleSelection::p simpleSel = *it;
            return simpleSel;
        }
    }
    return ret;
}

HSSSelection::p AXRController::selectHierarchical(HSSSelection::p scope, HSSDisplayObject::p thisObj)
{
    return this->selectHierarchical(scope, thisObj, true);
}

HSSSelection::p AXRController::selectHierarchical(HSSSelection::p scope, HSSDisplayObject::p thisObj, bool processing)
{
    HSSSelection::p selection = this->selectOnLevel(scope, thisObj, processing);
    bool atEnd = this->isAtEndOfSelector();
    if (!atEnd)
    {
        HSSSimpleSelection::p ret(new HSSSimpleSelection());
        HSSCombinatorType combinatorType = this->currentSelectorNode->getCombinatorType();
        switch (combinatorType)
        {
        case HSSCombinatorTypeChildren:
        {
            //if found, create a new scope with the children of the previous selections
            HSSSimpleSelection::p newScope(new HSSSimpleSelection());
            this->readNextSelectorNode();
            HSSSimpleSelection::p inner = selection->joinAll();
            for (HSSSimpleSelection::iterator it2 = inner->begin(); it2 != inner->end(); ++it2)
            {
                HSSDisplayObject::p theDO = *it2;
                if (theDO->isA(HSSObjectTypeContainer))
                {
                    HSSContainer::p theContainer = qSharedPointerCast<HSSContainer>(theDO);
                    HSSSimpleSelection::p children = theContainer->getChildren();
                    for (HSSSimpleSelection::const_iterator it3 = children->begin(); it3 != children->end(); ++it3)
                    {
                        newScope->add(*it3);
                    }
                }
            }
            if(newScope->size() > 0){
                return this->selectHierarchical(newScope, thisObj, processing);
            }

            return ret;
        }

        case HSSCombinatorTypeDescendants:
        {
            //if found, create a new scope with the children of the previous selections
            HSSSimpleSelection::p newScope(new HSSSimpleSelection());
            this->readNextSelectorNode();
            HSSSimpleSelection::p inner = selection->joinAll();
            for (HSSSimpleSelection::iterator it2 = inner->begin(); it2 != inner->end(); ++it2)
            {
                HSSDisplayObject::p theDO = *it2;
                if (theDO->isA(HSSObjectTypeContainer))
                {
                    HSSContainer::p theContainer = qSharedPointerCast<HSSContainer>(theDO);
                    HSSSimpleSelection::p children = theContainer->getChildren();
                    for (HSSSimpleSelection::const_iterator it3 = children->begin(); it3 != children->end(); ++it3)
                    {
                        newScope->add(*it3);
                    }
                }
            }
            //recursively search for matches
            return this->selectAllHierarchical(newScope, thisObj, processing);
        }

        default:
            break;
        }
    }

    return selection;
}

HSSSelection::p AXRController::selectAllHierarchical(HSSSelection::p scope, HSSDisplayObject::p thisObj, bool processing)
{
    //get all the elements that we need to check
    HSSSimpleSelection::p newScope(new HSSSimpleSelection());
    this->_recursiveGetDescendants(newScope, scope->joinAll());

    HSSSelection::p selections = this->selectOnLevel(newScope, thisObj, processing);
    return selections;
}

HSSSelection::p AXRController::selectOnLevel(HSSSelection::p scope, HSSDisplayObject::p thisObj, bool processing)
{
    HSSSelection::p selection = this->selectSimple(scope, thisObj, processing);
    bool atEnd = this->isAtEndOfSelector();
    if (!atEnd)
    {
        HSSCombinatorType combinatorType = this->currentSelectorNode->getCombinatorType();
        if (
            combinatorType == HSSCombinatorTypeSiblings
            || combinatorType == HSSCombinatorTypeNextSiblings
            || combinatorType == HSSCombinatorTypePreviousSiblings
            ) {
            HSSSimpleSelection::p ret(new HSSSimpleSelection());
            this->readNextSelectorNode();
            if (selection->isA(HSSSelectionTypeSimpleSelection))
            {
                this->_selectOnLevelSimple(ret, combinatorType, qSharedPointerCast<HSSSimpleSelection>(selection), thisObj, processing);
            }
            else if (selection->isA(HSSSelectionTypeMultipleSelection))
            {
                HSSMultipleSelection::p multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
                for (HSSMultipleSelection::const_iterator it=multiSel->begin(); it!=multiSel->end(); ++it)
                {
                    this->_selectOnLevelSimple(ret, combinatorType, *it, thisObj, processing);
                }
            }
            else
            {
                AXRError("AXRController", "Unknown selection type encountered").raise();
                return ret;
            }
            return ret;
        }
    }

    return selection;
}

inline void AXRController::_selectOnLevelSimple(HSSSimpleSelection::p & ret, HSSCombinatorType combinatorType, HSSSimpleSelection::p simpleSel, HSSDisplayObject::p thisObj, bool processing)
{
    HSSSimpleSelection::p retTemp(new HSSSimpleSelection());
    HSSSimpleSelection::const_iterator it=simpleSel->begin();
    if (combinatorType == HSSCombinatorTypeSiblings)
    {
        const HSSDisplayObject::p & firstItem = *it;
        HSSSimpleSelection::p firstSiblings;
        switch (combinatorType)
        {
            case HSSCombinatorTypeSiblings:
            {
                firstSiblings = firstItem->getSiblings();
                break;
            }
            case HSSCombinatorTypeNextSiblings:
            {
                firstSiblings = firstItem->getNextSiblings();
                break;
            }
            case HSSCombinatorTypePreviousSiblings:
            {
                firstSiblings = firstItem->getPreviousSiblings();
                break;
            }
            default:
            {
                break;
            }
        }
        HSSSimpleSelection::p firstSiblingsSelection = this->selectSimple(firstSiblings, thisObj, false)->joinAll();
        if(firstSiblingsSelection->size() > 0){
            retTemp->add(firstItem);
        }
    }
    
    for (; it!=simpleSel->end(); ++it)
    {
        const HSSDisplayObject::p & item = *it;
        switch (combinatorType)
        {
            case HSSCombinatorTypeSiblings:
            {
                HSSSimpleSelection::p siblings = item->getSiblings();
                for (HSSSimpleSelection::const_iterator it2=siblings->begin(); it2!=siblings->end(); ++it2)
                {
                    const HSSDisplayObject::p & siblingItem = *it2;
                    if (std::find(retTemp->begin(), retTemp->end(), siblingItem) == retTemp->end())
                    {
                        retTemp->add(siblingItem);
                    }
                }
                break;
            }

            case HSSCombinatorTypeNextSiblings:
            {
                HSSSimpleSelection::p siblings = item->getNextSiblings();
                for (HSSSimpleSelection::const_iterator it2=siblings->begin(); it2!=siblings->end(); ++it2)
                {
                    const HSSDisplayObject::p & siblingItem = *it2;
                    if (std::find(retTemp->begin(), retTemp->end(), siblingItem) == retTemp->end())
                    {
                        retTemp->add(siblingItem);
                    }
                }
                break;
            }

            case HSSCombinatorTypePreviousSiblings:
            {
                HSSSimpleSelection::p siblings = item->getPreviousSiblings();
                for (HSSSimpleSelection::const_iterator it2=siblings->begin(); it2!=siblings->end(); ++it2)
                {
                    const HSSDisplayObject::p & siblingItem = *it2;
                    if (std::find(retTemp->begin(), retTemp->end(), siblingItem) == retTemp->end())
                    {
                        retTemp->add(siblingItem);
                    }
                }
                break;
            }

            default:
                break;
        }
    }
    ret->addSelection(this->selectOnLevel(retTemp, thisObj, processing));
}

HSSSelection::p AXRController::selectSimple(HSSSelection::p scope, HSSDisplayObject::p thisObj, bool processing)
{
    bool done = false;
    bool needsReadNext = true;
    HSSSelection::p selection = scope;

    while (!done)
    {
        switch (this->currentSelectorNode->getSelectorType())
        {
        case HSSSelectorTypeSimpleSelector:
        {
            HSSSimpleSelector::p ss = qSharedPointerCast<HSSSimpleSelector>(this->currentSelectorNode);
            selection = ss->filterSelection(selection, thisObj, processing);
            break;
        }

        default:
        {
            done = true;
            needsReadNext = false;
            break;
        }
        }

        if (this->isAtEndOfSelector())
        {
            done = true;
        }
        else if (needsReadNext)
        {
            this->readNextSelectorNode();
        }
    }

    return selection;
}

AXRString AXRController::toString()
{
    AXRString tempstr = "AXR Controller\n";
    if (this->root)
    {
        tempstr.append("\n\n\nROOT:");
        tempstr = this->root->toString();
    }

    if (!this->objectTree.empty())
    {
        tempstr.append("\n\n\nOBJECT TREE:");
        for (unsigned i = 0; i < this->objectTree.size(); ++i)
        {
            tempstr.append("\n").append(this->objectTree[i]->toString());
        }
    }

    if (!this->parserTree.empty())
    {
        tempstr.append("\n\n\nPARSER TREE:");

        for (HSSParserNode::it pIt = this->parserTree.begin(); pIt != this->parserTree.end(); ++pIt)
        {
            tempstr.append("\n").append((*pIt)->toString());
        }
    }

    return tempstr;
}

void AXRController::reset()
{
    this->objectTree.clear();
    this->loadSheets.clear();
    this->parserTree.clear();
    this->rules.clear();
    this->root.clear();

    for (unsigned i = 0; i<this->currentContext.size(); ++i)
    {
        this->currentContext.pop();
    }

    this->currentChain.clear();
    this->currentSelectorNode.clear();
    this->currentChainCount = 0;
    this->currentChainSize = 0;
}

HSSContainer::p & AXRController::getRoot()
{
    return this->root;
}

void AXRController::setRoot(HSSContainer::p newRoot)
{
    this->root = newRoot;
    HSSParser::p hssparser = this->document()->getParserHSS();
    if (hssparser->currentObjectContextSize() == 0)
    {
        hssparser->currentObjectContextAdd(newRoot);
    }
    newRoot->setRoot(true);
}

void AXRController::enterElement(AXRString elementName)
{
    //std_log1("enter element " + elementName);
    HSSContainer::p newContainer(new HSSContainer(this));
    newContainer->setName(elementName + "_source_obj");
    newContainer->setElementName(elementName);
    this->add(newContainer);
    this->currentContext.push(newContainer);
}

void AXRController::addAttribute(AXRString name, AXRString value)
{
    //std_log1(AXRString("adding attribute " + name + " and value " + value));
    this->currentContext.top()->attributesAdd(name, value);
}

void AXRController::setContentText(AXRString text)
{
    this->currentContext.top()->setContentText(text);
}

void AXRController::appendContentText(AXRString text)
{
    this->currentContext.top()->appendContentText(text);
}

void AXRController::exitElement()
{
    this->currentContext.pop();
}

void AXRController::add(HSSDisplayObject::p newElement)
{
    if (!this->root)
    {
        HSSContainer::p cont = HSSContainer::asContainer(newElement);
        if (cont)
        {
            this->setRoot(cont);
        }
        else
        {
            std_log("############## HSSController: cannot add non-controller as root");
        }
    }
    else
    {
        if (!this->currentContext.empty())
        {
            HSSContainer::p theCurrent = this->currentContext.top();
            theCurrent->add(newElement);
        }
        else
        {
            std_log("############## HSSController: tried to add a container to nonexistent current");
        }
    }
}

//object tree

void AXRController::objectTreeAdd(HSSObjectDefinition::p & newObject)
{
    this->objectTree.push_back(newObject);
}

void AXRController::objectTreeRemove(unsigned index)
{
    this->objectTree.erase(this->objectTree.begin() + index);
}

HSSObjectDefinition::p & AXRController::objectTreeGet(unsigned index)
{
    return this->objectTree[index];
}

HSSObjectDefinition::p & AXRController::objectTreeGet(AXRString name)
{
    /**
     *  @todo do this with an unordered_map for better performance
     */
    for (unsigned i = 0, size = this->objectTree.size(); i < size; ++i)
    {
        HSSObjectDefinition::p & theObj = this->objectTreeGet(i);
        if (theObj && theObj->getObject()->name == name)
        {
            return theObj;
        }
    }

    throw AXRError("AXRController", "No object with name " + name + " was found");
}

//loadSheets

void AXRController::loadSheetsAdd(QUrl url)
{
    this->loadSheets.push_back(url);
}

void AXRController::loadSheetsRemove(unsigned index)
{
    this->loadSheets.erase(this->loadSheets.begin() + index);
}

QUrl AXRController::loadSheetsGet(unsigned index)
{
    return this->loadSheets[index];
}

const std::vector<QUrl> AXRController::loadSheetsGet() const
{
    return this->loadSheets;
}

//parser node tree

void AXRController::setParserTree(std::vector<HSSParserNode::p> newTree)
{
    this->parserTree = newTree;
}

const std::vector<HSSParserNode::p> AXRController::getParserTree() const
{
    return this->parserTree;
}

void AXRController::parserTreeAdd(HSSParserNode::p node)
{
    this->parserTree.push_back(node);
}

void AXRController::parserTreeRemove(HSSParserNode::p node)
{
    HSSParserNode::it it = find(this->parserTree.begin(), this->parserTree.end(), node);
    if (it != this->parserTree.end())
    {
        this->parserTree.erase(it);
    }
}

//rules

const std::vector<HSSRule::p>& AXRController::getRules() const
{
    return this->rules;
}

void AXRController::rulesAdd(HSSRule::p & rule)
{
    this->rules.push_back(rule);
}

void AXRController::rulesRemove(unsigned index)
{
    this->rules.erase(this->rules.begin() + index);
}

HSSRule::p & AXRController::rulesGet(unsigned index)
{
    return this->rules[index];
}

unsigned AXRController::rulesSize()
{
    return this->rules.size();
}

void AXRController::_recursiveGetDescendants(HSSSimpleSelection::p & ret, HSSSimpleSelection::p scope)
{
    for (HSSSimpleSelection::const_iterator it = scope->begin(); it!=scope->end(); ++it) {
        const HSSDisplayObject::p & theDO = *it;
        ret->add(theDO);
        HSSContainer::p cont = HSSContainer::asContainer(theDO);
        if (cont)
        {
            this->_recursiveGetDescendants(ret, cont->getChildren());
        }
    }
}
