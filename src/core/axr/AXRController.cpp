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

#include "AXRController.h"
#include "AXRDocument.h"
#include "AXRLoggerManager.h"
#include "AXRWarning.h"
#include "HSSCallback.h"
#include "HSSContainer.h"
#include "HSSDisplayObject.h"
#include "HSSFlag.h"
#include "HSSInstruction.h"
#include "HSSMultipleSelection.h"
#include "HSSNegation.h"
#include "HSSNameSelector.h"
#include "HSSNumberConstant.h"
#include "HSSObjectDefinition.h"
#include "HSSParser.h"
#include "HSSRule.h"
#include "HSSSelection.h"
#include "HSSSelectorChain.h"
#include "HSSSimpleSelection.h"
#include "HSSSimpleSelector.h"

using namespace AXR;

AXRController::AXRController(AXRDocument *document)
: document_(document)
{
    axr_log(LoggerChannelGeneralSpecific, "AXRController: creating controller");
    this->currentContext = std::stack<QSharedPointer<HSSContainer> > ();
}

AXRController::~AXRController()
{
    axr_log(LoggerChannelGeneralSpecific, "AXRController: destructing controller");

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
    const std::vector<QSharedPointer<HSSRule> > rules = this->getRules();
    QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection());
    rootScope->add(this->getRoot());

    for (size_t i = 0; i < rules.size(); ++i)
    {
        const QSharedPointer<HSSRule>& rule = rules[i];

        rule->setOriginalScope(rootScope);

        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains = rule->getSelectorChains();
        if (!selectorChains.empty())
        {
            QSharedPointer<HSSSelection> selection;
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
                QSharedPointer<HSSMultipleSelection> multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
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

inline void AXRController::_matchRuleToSelection(QSharedPointer<HSSRule> rule, QSharedPointer<HSSSimpleSelection> selection)
{
    for (HSSSimpleSelection::const_iterator it = selection->begin(); it != selection->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & displayObject = *it;
        axr_log(LoggerChannelGeneral, "AXRController: match " + displayObject->getElementName());
        displayObject->rulesAdd(rule, HSSRuleStateOff);

        displayObject->setNeedsRereadRules(true);
        displayObject->setNeedsSurface(true);
        displayObject->setDirty(true);

        //if it is a container it may have children
        if (displayObject->isA(HSSObjectTypeContainer))
        {
            QSharedPointer<HSSContainer> selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
            this->currentContext.push(selectedContainer);
            for (size_t k = 0; k < rule->childrenSize(); ++k)
            {
                const QSharedPointer<HSSRule> childRule = rule->childrenGet(k);
                this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer, true);
            }
            this->currentContext.pop();
        }

        displayObject->setNeedsRereadRules(true);
    }
}

void AXRController::recursiveMatchRulesToDisplayObjects(const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSContainer> container, bool applyingInstructions)
{
    axr_log(LoggerChannelGeneralSpecific, "AXRController: recursive matching rules to display objects");
    QSharedPointer<HSSInstruction> instruction = rule->getInstruction();
    rule->setThisObj(container);
    if (instruction && applyingInstructions)
    {
        switch (instruction->getInstructionType())
        {
        case HSSNewInstruction:
        {
            if (container)
            {
                QSharedPointer<HSSSimpleSelector> subject = rule->selectorChainsLast()->subject();
                AXRString elementName = "";
                if (subject)
                {
                    QSharedPointer<HSSNameSelector> nameSel = subject->getName();
                    elementName = nameSel->getElementName();
                }

                uint argssize = 1;
                QSharedPointer<HSSParserNode> argument = instruction->getArgument();
                if (argument)
                {
                    if (argument->isA(HSSParserNodeTypeNumberConstant))
                    {
                        QSharedPointer<HSSNumberConstant> argnum = qSharedPointerCast<HSSNumberConstant>(argument);
                        argssize = static_cast<uint>(argnum->getValue());
                    }
                }

                for (uint i = 0; i < argssize; ++i)
                {
                    QSharedPointer<HSSContainer> newContainer = QSharedPointer<HSSContainer>(new HSSContainer(this));
                    newContainer->setName(elementName);
                    newContainer->setElementName(elementName);
                    newContainer->rulesAdd(rule, HSSRuleStateOn);
                    axr_log(LoggerChannelGeneral, "AXRController: created " + newContainer->getElementName());
                    this->add(newContainer);
                    newContainer->setNeedsRereadRules(true);
                    newContainer->setNeedsSurface(true);
                    newContainer->setDirty(true);
                    this->currentContext.push(newContainer);
                    for (size_t i = 0; i < rule->childrenSize(); ++i)
                    {
                        const QSharedPointer<HSSRule> childRule = rule->childrenGet(i);
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
                QSharedPointer<HSSSelection> selection = this->select(rule->getSelectorChains(), scope, container);

                this->currentContext.push(container);
                //move the children over
                QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();
                for (HSSSimpleSelection::const_iterator it=inner->begin(); it!=inner->end(); ++it)
                {
                    const QSharedPointer<HSSDisplayObject> & theDO = *it;
                    if (theDO != container)
                    {
                        theDO->removeFromParent();
                        theDO->rulesAdd(rule, HSSRuleStateOn);
                        axr_log(LoggerChannelGeneral, "AXRController: moved " + theDO->getElementName());
                        this->add(theDO);
                        theDO->setNeedsRereadRules(true);
                        theDO->setNeedsSurface(true);
                        theDO->setDirty(true);

                        if (theDO->isA(HSSObjectTypeContainer))
                        {
                            QSharedPointer<HSSContainer> theContainer = qSharedPointerCast<HSSContainer>(theDO);
                            this->currentContext.push(theContainer);
                            //assign more rules
                            for (size_t i = 0; i < rule->childrenSize(); ++i)
                            {
                                const QSharedPointer<HSSRule> childRule = rule->childrenGet(i);
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

            break;
        }

        case HSSDeleteInstruction:
        {
            if (container)
            {
                //select the items to be deleted
                QSharedPointer<HSSSelection> selection = this->select(rule->getSelectorChains(), scope, container);
                QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();
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
        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains = rule->getSelectorChains();
        if (!selectorChains.empty())
        {
            QSharedPointer<HSSSelection> selection;

            if (container)
            {
                if (applyingInstructions)
                {
                    //we observe the parent for dom changes
                    container->observe(HSSObservablePropertyTreeChange, HSSObservablePropertyValue, rule.data(), new HSSValueChangedCallback<HSSRule>(rule.data(), &HSSRule::treeChanged));
                    rule->setObservedTreeChanger(container.data());
                    rule->addOriginalScope(scope->joinAll());
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
                QSharedPointer<HSSMultipleSelection> multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
                for (HSSMultipleSelection::iterator it = multiSel->begin(); it != multiSel->end(); ++it)
                {
                    this->_recursiveMatchRulesToDisplayObjects(rule, *it, container, applyingInstructions);
                }
            }
            else if (selection->isA(HSSSelectionTypeSimpleSelection))
            {
                QSharedPointer<HSSSimpleSelection> simpleSel = qSharedPointerCast<HSSSimpleSelection>(selection);
                this->_recursiveMatchRulesToDisplayObjects(rule, simpleSel, container, applyingInstructions);
            }
        }
    }
}

inline void AXRController::_recursiveMatchRulesToDisplayObjects(const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSimpleSelection> scope, QSharedPointer<HSSContainer> container, bool applyingInstructions)
{
    for (HSSSimpleSelection::const_iterator it = scope->begin(); it != scope->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & displayObject = *it;
        axr_log(LoggerChannelGeneral, "AXRController: match " + displayObject->getElementName());
        displayObject->rulesAdd(rule, HSSRuleStateOff);

        displayObject->setNeedsRereadRules(true);
        displayObject->setNeedsSurface(true);
        displayObject->setDirty(true);

        //if it is a container it may have children
        if (displayObject->isA(HSSObjectTypeContainer))
        {
            QSharedPointer<HSSContainer> selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
            this->currentContext.push(selectedContainer);
            for (size_t k = 0; k < rule->childrenSize(); ++k)
            {
                const QSharedPointer<HSSRule> childRule = rule->childrenGet(k);
                this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer, applyingInstructions);
            }
            this->currentContext.pop();
        }
        displayObject->setNeedsRereadRules(true);
    }
}

void AXRController::activateRules()
{
    const std::vector<QSharedPointer<HSSRule> > rules = this->getRules();
    QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection());
    rootScope->add(this->getRoot());

    for (size_t i = 0; i < rules.size(); ++i)
    {
        const QSharedPointer<HSSRule>& rule = rules[i];
        this->recursiveSetRuleState(rule, rootScope, this->getRoot(), HSSRuleStateOn);
    }
}

void AXRController::recursiveSetRuleState(QSharedPointer<HSSRule> rule, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, HSSRuleState state)
{
    if (!rule->getInstruction()) {
        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains = rule->getSelectorChains();
        if (!selectorChains.empty())
        {
            QSharedPointer<HSSSelection> selection;
            try
            {
                selection = this->select(selectorChains, scope, thisObj);
            }
            catch (const AXRError &e)
            {
                e.raise();
            }

            if (selection->isA(HSSSelectionTypeMultipleSelection))
            {
                QSharedPointer<HSSMultipleSelection> multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
                for (HSSMultipleSelection::iterator it = multiSel->begin(); it != multiSel->end(); ++it)
                {
                    this->setRuleStateOnSelection(rule, qSharedPointerCast<HSSSimpleSelection>(*it), state);
                }
            }
            else if (selection->isA(HSSSelectionTypeSimpleSelection))
            {
                this->setRuleStateOnSelection(rule, qSharedPointerCast<HSSSimpleSelection>(selection), state);
            }
        }
    }
}

void AXRController::setRuleStateOnSelection(QSharedPointer<HSSRule> rule, QSharedPointer<HSSSimpleSelection> selection, HSSRuleState state)
{
    for (HSSSimpleSelection::const_iterator it = selection->begin(); it != selection->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & displayObject = *it;
        axr_log(LoggerChannelGeneral, "AXRController: activating rule on " + displayObject->getElementName());
        displayObject->setRuleStatus(rule, state);

        //if it is a container it may have children
        if (displayObject->isA(HSSObjectTypeContainer))
        {
            QSharedPointer<HSSContainer> selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
            for (size_t k = 0; k < rule->childrenSize(); ++k)
            {
                const QSharedPointer<HSSRule> childRule = rule->childrenGet(k);
                QSharedPointer<HSSInstruction> theInst = childRule->getInstruction();
                if(theInst)
                {
                    std::vector<QWeakPointer<HSSDisplayObject> > appliedTo = childRule->getAppliedTo();
                    QSharedPointer<HSSSimpleSelection> children(new HSSSimpleSelection());
                    for (std::vector<QWeakPointer<HSSDisplayObject> >::iterator it = appliedTo.begin(); it != appliedTo.end(); ++it) {
                        children->add((*it).toStrongRef());
                    }
                    this->setRuleStateOnSelection(childRule, children, state);
                }
                else
                {
                    QSharedPointer<HSSSimpleSelection> children = this->select(childRule->getSelectorChains(), selectedContainer->getChildren(), selectedContainer)->joinAll();
                    this->setRuleStateOnSelection(childRule, children, state);
                }
            }
        }
        displayObject->setNeedsRereadRules(true);
    }
}

void AXRController::initializeSelectorChain(QSharedPointer<HSSSelectorChain> selectorChain)
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

QSharedPointer<HSSSelection> AXRController::select(std::vector<QSharedPointer<HSSSelectorChain> > selectorChains, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj)
{
    return this->select(selectorChains, scope, thisObj, false);
}

QSharedPointer<HSSSelection> AXRController::select(std::vector<QSharedPointer<HSSSelectorChain> > selectorChains, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing)
{
    QSharedPointer<HSSMultipleSelection> ret(new HSSMultipleSelection());

    for (HSSSelectorChain::const_it it = selectorChains.begin(); it != selectorChains.end(); ++it)
    {
        this->initializeSelectorChain(*it);

        //if we have a descendants combinator here, it means we were at root context, since the parser
        //automatically prepends a @this before it when in another context
        if(this->currentSelectorNode->isA(HSSSelectorTypeCombinator)){
            HSSCombinatorType combinatorType = this->currentSelectorNode->getCombinatorType();
            if(combinatorType == HSSCombinatorTypeDescendants){
                this->readNextSelectorNode();
                QSharedPointer<HSSSelection> sel = this->selectAllHierarchical(scope, thisObj, true);
                ret->addSelection(sel);
            } else {
                AXRError("AXRController", "Selector chains at root level can't start with combinators except . (unimplemented) and ..").raise();
            }
        } else {
            QSharedPointer<HSSSelection> sel = this->selectHierarchical(scope, thisObj, processing);
            ret->addSelection(sel);
        }
    }
    if (ret->size() == 1){
        for (HSSMultipleSelection::iterator it = ret->begin(); it!=ret->end(); ++it) {
            QSharedPointer<HSSSimpleSelection> simpleSel = *it;
            return simpleSel;
        }
    }
    return ret;
}

QSharedPointer<HSSSelection> AXRController::selectHierarchical(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj)
{
    return this->selectHierarchical(scope, thisObj, true);
}

QSharedPointer<HSSSelection> AXRController::selectHierarchical(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing)
{
    QSharedPointer<HSSSelection> selection = this->selectOnLevel(scope, thisObj, processing);
    bool atEnd = this->isAtEndOfSelector();
    if (!atEnd)
    {
        QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection());
        HSSCombinatorType combinatorType = this->currentSelectorNode->getCombinatorType();
        switch (combinatorType)
        {
        case HSSCombinatorTypeChildren:
        {
            //if found, create a new scope with the children of the previous selections
            QSharedPointer<HSSSimpleSelection> newScope(new HSSSimpleSelection());
            this->readNextSelectorNode();
            QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();
            for (HSSSimpleSelection::iterator it2 = inner->begin(); it2 != inner->end(); ++it2)
            {
                QSharedPointer<HSSDisplayObject> theDO = *it2;
                if (theDO->isA(HSSObjectTypeContainer))
                {
                    QSharedPointer<HSSContainer> theContainer = qSharedPointerCast<HSSContainer>(theDO);
                    QSharedPointer<HSSSimpleSelection> children = theContainer->getChildren();
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
            QSharedPointer<HSSSimpleSelection> newScope(new HSSSimpleSelection());
            this->readNextSelectorNode();
            QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();
            for (HSSSimpleSelection::iterator it2 = inner->begin(); it2 != inner->end(); ++it2)
            {
                QSharedPointer<HSSDisplayObject> theDO = *it2;
                if (theDO->isA(HSSObjectTypeContainer))
                {
                    QSharedPointer<HSSContainer> theContainer = qSharedPointerCast<HSSContainer>(theDO);
                    QSharedPointer<HSSSimpleSelection> children = theContainer->getChildren();
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

QSharedPointer<HSSSelection> AXRController::selectAllHierarchical(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing)
{
    //get all the elements that we need to check
    QSharedPointer<HSSSimpleSelection> newScope(new HSSSimpleSelection());
    this->_recursiveGetDescendants(newScope, scope->joinAll());

    QSharedPointer<HSSSelection> selections = this->selectOnLevel(newScope, thisObj, processing);
    return selections;
}

QSharedPointer<HSSSelection> AXRController::selectOnLevel(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing)
{
    QSharedPointer<HSSSelection> selection = this->selectSimple(scope, thisObj, processing);
    bool atEnd = this->isAtEndOfSelector();
    if (!atEnd)
    {
        HSSCombinatorType combinatorType = this->currentSelectorNode->getCombinatorType();
        if (
            combinatorType == HSSCombinatorTypeSiblings
            || combinatorType == HSSCombinatorTypeNextSiblings
            || combinatorType == HSSCombinatorTypePreviousSiblings
            ) {
            QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection());
            this->readNextSelectorNode();
            if (selection->isA(HSSSelectionTypeSimpleSelection))
            {
                this->_selectOnLevelSimple(ret, combinatorType, qSharedPointerCast<HSSSimpleSelection>(selection), thisObj, processing);
            }
            else if (selection->isA(HSSSelectionTypeMultipleSelection))
            {
                QSharedPointer<HSSMultipleSelection> multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
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

inline void AXRController::_selectOnLevelSimple(QSharedPointer<HSSSimpleSelection> & ret, HSSCombinatorType combinatorType, QSharedPointer<HSSSimpleSelection> simpleSel, QSharedPointer<HSSDisplayObject> thisObj, bool processing)
{
    QSharedPointer<HSSSimpleSelection> retTemp(new HSSSimpleSelection());
    HSSSimpleSelection::const_iterator it=simpleSel->begin();
    if (combinatorType == HSSCombinatorTypeSiblings)
    {
        const QSharedPointer<HSSDisplayObject> & firstItem = *it;
        QSharedPointer<HSSSimpleSelection> firstSiblings;
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
        QSharedPointer<HSSSimpleSelection> firstSiblingsSelection = this->selectSimple(firstSiblings, thisObj, false)->joinAll();
        if(firstSiblingsSelection->size() > 0){
            retTemp->add(firstItem);
        }
    }

    for (; it!=simpleSel->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & item = *it;
        switch (combinatorType)
        {
            case HSSCombinatorTypeSiblings:
            {
                QSharedPointer<HSSSimpleSelection> siblings = item->getSiblings();
                for (HSSSimpleSelection::const_iterator it2=siblings->begin(); it2!=siblings->end(); ++it2)
                {
                    const QSharedPointer<HSSDisplayObject> & siblingItem = *it2;
                    if (std::find(retTemp->begin(), retTemp->end(), siblingItem) == retTemp->end())
                    {
                        retTemp->add(siblingItem);
                    }
                }
                break;
            }

            case HSSCombinatorTypeNextSiblings:
            {
                QSharedPointer<HSSSimpleSelection> siblings = item->getNextSiblings();
                for (HSSSimpleSelection::const_iterator it2=siblings->begin(); it2!=siblings->end(); ++it2)
                {
                    const QSharedPointer<HSSDisplayObject> & siblingItem = *it2;
                    if (std::find(retTemp->begin(), retTemp->end(), siblingItem) == retTemp->end())
                    {
                        retTemp->add(siblingItem);
                    }
                }
                break;
            }

            case HSSCombinatorTypePreviousSiblings:
            {
                QSharedPointer<HSSSimpleSelection> siblings = item->getPreviousSiblings();
                for (HSSSimpleSelection::const_iterator it2=siblings->begin(); it2!=siblings->end(); ++it2)
                {
                    const QSharedPointer<HSSDisplayObject> & siblingItem = *it2;
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

QSharedPointer<HSSSelection> AXRController::selectSimple(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing)
{
    bool done = false;
    bool needsReadNext = true;
    QSharedPointer<HSSSelection> selection = scope;

    while (!done)
    {
        switch (this->currentSelectorNode->getSelectorType())
        {
        case HSSSelectorTypeSimpleSelector:
        {
            QSharedPointer<HSSSimpleSelector> ss = qSharedPointerCast<HSSSimpleSelector>(this->currentSelectorNode);
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

QSharedPointer<HSSContainer> & AXRController::getRoot()
{
    return this->root;
}

void AXRController::setRoot(QSharedPointer<HSSContainer> newRoot)
{
    this->root = newRoot;
    QSharedPointer<HSSParser> hssparser = this->document()->getParserHSS();
    if (hssparser->currentObjectContextSize() == 0)
    {
        hssparser->currentObjectContextAdd(newRoot);
    }
    newRoot->setRoot(true);
}

void AXRController::enterElement(AXRString elementName)
{
    //axr_log(LoggerChannelObsolete1, "enter element " + elementName);
    QSharedPointer<HSSContainer> newContainer(new HSSContainer(this));
    newContainer->setName(elementName + "_source_obj");
    newContainer->setElementName(elementName);
    this->add(newContainer);
    this->currentContext.push(newContainer);
}

void AXRController::addAttribute(AXRString name, AXRString value)
{
    //axr_log(LoggerChannelObsolete1, AXRString("adding attribute " + name + " and value " + value));
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

void AXRController::add(QSharedPointer<HSSDisplayObject> newElement)
{
    if (!this->root)
    {
        QSharedPointer<HSSContainer> cont = HSSContainer::asContainer(newElement);
        if (cont)
        {
            this->setRoot(cont);
        }
        else
        {
            axr_log(LoggerChannelObsolete0, "############## HSSController: cannot add non-controller as root");
        }
    }
    else
    {
        if (!this->currentContext.empty())
        {
            QSharedPointer<HSSContainer> theCurrent = this->currentContext.top();
            theCurrent->add(newElement);
        }
        else
        {
            axr_log(LoggerChannelObsolete0, "############## HSSController: tried to add a container to nonexistent current");
        }
    }
}

//object tree

void AXRController::objectTreeAdd(QSharedPointer<HSSObjectDefinition> & newObject)
{
    this->objectTree.push_back(newObject);
}

void AXRController::objectTreeRemove(off_t index)
{
    this->objectTree.erase(this->objectTree.begin() + index);
}

QSharedPointer<HSSObjectDefinition> & AXRController::objectTreeGet(size_t index)
{
    return this->objectTree[index];
}

QSharedPointer<HSSObjectDefinition> & AXRController::objectTreeGet(AXRString name)
{
    /**
     *  @todo do this with an unordered_map for better performance
     */
    for (size_t i = 0; i < this->objectTree.size(); ++i)
    {
        QSharedPointer<HSSObjectDefinition> & theObj = this->objectTreeGet(i);
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

void AXRController::loadSheetsRemove(off_t index)
{
    this->loadSheets.erase(this->loadSheets.begin() + index);
}

QUrl AXRController::loadSheetsGet(size_t index)
{
    return this->loadSheets[index];
}

const std::vector<QUrl> AXRController::loadSheetsGet() const
{
    return this->loadSheets;
}

//parser node tree

void AXRController::setParserTree(std::vector<QSharedPointer<HSSParserNode> > newTree)
{
    this->parserTree = newTree;
}

const std::vector<QSharedPointer<HSSParserNode> > AXRController::getParserTree() const
{
    return this->parserTree;
}

void AXRController::parserTreeAdd(QSharedPointer<HSSParserNode> node)
{
    this->parserTree.push_back(node);
}

void AXRController::parserTreeRemove(QSharedPointer<HSSParserNode> node)
{
    HSSParserNode::it it = find(this->parserTree.begin(), this->parserTree.end(), node);
    if (it != this->parserTree.end())
    {
        this->parserTree.erase(it);
    }
}

//rules

const std::vector<QSharedPointer<HSSRule> >& AXRController::getRules() const
{
    return this->rules;
}

void AXRController::rulesAdd(QSharedPointer<HSSRule> & rule)
{
    this->rules.push_back(rule);
}

void AXRController::rulesRemove(off_t index)
{
    this->rules.erase(this->rules.begin() + index);
}

QSharedPointer<HSSRule> & AXRController::rulesGet(size_t index)
{
    return this->rules[index];
}

size_t AXRController::rulesSize() const
{
    return this->rules.size();
}

void AXRController::_recursiveGetDescendants(QSharedPointer<HSSSimpleSelection> & ret, QSharedPointer<HSSSimpleSelection> scope)
{
    for (HSSSimpleSelection::const_iterator it = scope->begin(); it!=scope->end(); ++it) {
        const QSharedPointer<HSSDisplayObject> & theDO = *it;
        ret->add(theDO);
        QSharedPointer<HSSContainer> cont = HSSContainer::asContainer(theDO);
        if (cont)
        {
            this->_recursiveGetDescendants(ret, cont->getChildren());
        }
    }
}
