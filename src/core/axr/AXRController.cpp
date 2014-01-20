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

#include <QStack>
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

namespace AXR
{
    class AXRControllerPrivate
    {
        friend class AXRController;

        QStack<QSharedPointer<HSSContainer> > currentContext;

        QSharedPointer<HSSContainer> root;

        QList<QSharedPointer<HSSObjectDefinition> > objectTree;
        QList<QUrl> styleSheetUrls;
        QList<QSharedPointer<HSSParserNode> > parserTree;
        QList<QSharedPointer<HSSRule> > rules;

        QSharedPointer<HSSSelectorChain> currentChain;
        QSharedPointer<HSSParserNode> currentSelectorNode;
        size_t currentChainCount;
        size_t currentChainSize;

        AXRDocument *document;
    };
}

AXRController::AXRController(AXRDocument *document)
: d(new AXRControllerPrivate)
{
    axr_log(LoggerChannelGeneralSpecific, "AXRController: creating controller");
    d->document = document;
    d->currentContext = QStack<QSharedPointer<HSSContainer> > ();
}

AXRController::~AXRController()
{
    axr_log(LoggerChannelGeneralSpecific, "AXRController: destructing controller");

    d->objectTree.clear();
    d->styleSheetUrls.clear();
    d->rules.clear();
    d->parserTree.clear();
    delete d;
}

AXRDocument* AXRController::document() const
{
    return d->document;
}

void AXRController::matchRulesToContentTree()
{
    QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection(this));
    rootScope->add(this->root());

    Q_FOREACH (const QSharedPointer<HSSRule> &rule, rules())
    {
        rule->setOriginalScope(rootScope);

        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains = rule->getSelectorChains();
        if (!selectorChains.empty())
        {
            QSharedPointer<HSSSelection> selection;
            try
            {
                selection = this->select(selectorChains, rootScope, this->root(), true);
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
    Q_FOREACH (const QSharedPointer<HSSDisplayObject> &displayObject, *selection.data())
    {
        axr_log(LoggerChannelGeneral, "AXRController: match " + displayObject->getElementName());
        displayObject->rulesAdd(rule, HSSRuleStateOff);

        displayObject->setNeedsRereadRules(true);
        displayObject->setNeedsSurface(true);
        displayObject->setDirty(true);

        //if it is a container it may have children
        if (displayObject->isA(HSSObjectTypeContainer))
        {
            QSharedPointer<HSSContainer> selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
            d->currentContext.push(selectedContainer);
            for (size_t k = 0; k < rule->childrenSize(); ++k)
            {
                const QSharedPointer<HSSRule> childRule = rule->childrenGet(k);
                this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer, true);
            }
            d->currentContext.pop();
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
                    d->currentContext.push(newContainer);
                    for (size_t i = 0; i < rule->childrenSize(); ++i)
                    {
                        const QSharedPointer<HSSRule> childRule = rule->childrenGet(i);
                        this->recursiveMatchRulesToDisplayObjects(childRule, newContainer->getChildren(), newContainer, applyingInstructions);
                    }
                    newContainer->setNeedsRereadRules(true);
                    d->currentContext.pop();
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

                d->currentContext.push(container);
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
                            d->currentContext.push(theContainer);
                            //assign more rules
                            for (size_t i = 0; i < rule->childrenSize(); ++i)
                            {
                                const QSharedPointer<HSSRule> childRule = rule->childrenGet(i);
                                this->recursiveMatchRulesToDisplayObjects(childRule, theContainer->getChildren(), theContainer, applyingInstructions);
                            }
                            d->currentContext.pop();
                        }

                        theDO->setNeedsRereadRules(true);
                        //theDO->fireEvent(HSSEventTypeLoad);
                    }
                }
                d->currentContext.pop();
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
                    container->changeRulesAdd(rule);
                }
                rule->addOriginalScope(scope->joinAll());

                try
                {
                    selection = this->select(selectorChains, scope, container, applyingInstructions);
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
                    selection = this->select(selectorChains, scope, this->root(), applyingInstructions);
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
            d->currentContext.push(selectedContainer);
            for (size_t k = 0; k < rule->childrenSize(); ++k)
            {
                const QSharedPointer<HSSRule> childRule = rule->childrenGet(k);
                this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer, applyingInstructions);
            }
            d->currentContext.pop();
        }
        displayObject->setNeedsRereadRules(true);
    }
}

QStack<QSharedPointer<HSSContainer> >& AXRController::currentContext() const
{
    return d->currentContext;
}

void AXRController::currentContextPush(QSharedPointer<HSSContainer> container)
{
    d->currentContext.push(container);
}

const QSharedPointer<HSSContainer> AXRController::currentContextPop() const
{
    return d->currentContext.pop();
}

void AXRController::activateRules()
{
    QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection(this));
    rootScope->add(this->root());

    Q_FOREACH (const QSharedPointer<HSSRule> &rule, rules())
    {
        this->recursiveSetRuleState(rule, rootScope, this->root(), HSSRuleStateOn);
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
                Q_FOREACH (const QSharedPointer<HSSSimpleSelection> &sel, *multiSel.data())
                {
                    this->setRuleStateOnSelection(rule, sel, state);
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
    Q_FOREACH (const QSharedPointer<HSSDisplayObject> & displayObject, *selection.data())
    {
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
                    QSharedPointer<HSSSimpleSelection> children(new HSSSimpleSelection(this));
                    Q_FOREACH (const QWeakPointer<HSSDisplayObject> &displayObject, appliedTo)
                    {
                        children->add(displayObject.toStrongRef());
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
    d->currentChain = selectorChain;
    d->currentChainSize = selectorChain->size();
    d->currentChainCount = 0;
    d->currentSelectorNode = selectorChain->get(0);
}

void AXRController::readNextSelectorNode()
{
    if (d->currentChainCount < d->currentChainSize - 1)
    {
        d->currentSelectorNode = d->currentChain->get(d->currentChainCount + 1);
        d->currentChainCount++;
    }
}

bool AXRController::isAtEndOfSelector() const
{
    return d->currentChainCount >= d->currentChainSize - 1;
}

QSharedPointer<HSSSelection> AXRController::select(std::vector<QSharedPointer<HSSSelectorChain> > selectorChains, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj)
{
    return this->select(selectorChains, scope, thisObj, false);
}

QSharedPointer<HSSSelection> AXRController::select(std::vector<QSharedPointer<HSSSelectorChain> > selectorChains, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing)
{
    QSharedPointer<HSSMultipleSelection> ret(new HSSMultipleSelection(this));

    Q_FOREACH (QSharedPointer<HSSSelectorChain> chain, selectorChains)
    {
        this->initializeSelectorChain(chain);

        //if we have a descendants combinator here, it means we were at root context, since the parser
        //automatically prepends a @this before it when in another context
        if(d->currentSelectorNode->isA(HSSSelectorTypeCombinator)){
            HSSCombinatorType combinatorType = d->currentSelectorNode->getCombinatorType();
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
            axr_log(LoggerChannelController, AXRString("AXRController:").append(HSSSelection::logSelection(simpleSel.data(), selectorChains)));
            return simpleSel;
        }
    }
    axr_log(LoggerChannelController, AXRString("AXRController:").append(HSSSelection::logSelection(ret.data(), selectorChains)));
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
        QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection(this));
        HSSCombinatorType combinatorType = d->currentSelectorNode->getCombinatorType();
        switch (combinatorType)
        {
        case HSSCombinatorTypeChildren:
        {
            //if found, create a new scope with the children of the previous selections
            QSharedPointer<HSSSimpleSelection> newScope(new HSSSimpleSelection(this));
            this->readNextSelectorNode();
            QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();

            Q_FOREACH (QSharedPointer<HSSDisplayObject> theDO, *inner.data())
            {
                if (theDO->isA(HSSObjectTypeContainer))
                {
                    QSharedPointer<HSSContainer> theContainer = qSharedPointerCast<HSSContainer>(theDO);
                    Q_FOREACH (QSharedPointer<HSSDisplayObject> displayObject, *theContainer->getChildren().data())
                    {
                        newScope->add(displayObject);
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
            QSharedPointer<HSSSimpleSelection> newScope(new HSSSimpleSelection(this));
            this->readNextSelectorNode();
            QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();
            Q_FOREACH (QSharedPointer<HSSDisplayObject> theDO, *inner.data())
            {
                if (theDO->isA(HSSObjectTypeContainer))
                {
                    QSharedPointer<HSSContainer> theContainer = qSharedPointerCast<HSSContainer>(theDO);
                    Q_FOREACH (QSharedPointer<HSSDisplayObject> displayObject, *theContainer->getChildren().data())
                    {
                        newScope->add(displayObject);
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
    QSharedPointer<HSSSimpleSelection> newScope(new HSSSimpleSelection(this));
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
        HSSCombinatorType combinatorType = d->currentSelectorNode->getCombinatorType();
        if (
            combinatorType == HSSCombinatorTypeSiblings
            || combinatorType == HSSCombinatorTypeNextSiblings
            || combinatorType == HSSCombinatorTypePreviousSiblings
            ) {
            QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection(this));
            this->readNextSelectorNode();
            if (selection->isA(HSSSelectionTypeSimpleSelection))
            {
                this->_selectOnLevelSimple(ret, combinatorType, qSharedPointerCast<HSSSimpleSelection>(selection), thisObj, processing);
            }
            else if (selection->isA(HSSSelectionTypeMultipleSelection))
            {
                Q_FOREACH (QSharedPointer<HSSSimpleSelection> simpleSel, *qSharedPointerCast<HSSMultipleSelection>(selection).data())
                {
                    this->_selectOnLevelSimple(ret, combinatorType, simpleSel, thisObj, processing);
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
    QSharedPointer<HSSSimpleSelection> retTemp(new HSSSimpleSelection(this));
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
        switch (d->currentSelectorNode->getSelectorType())
        {
        case HSSSelectorTypeSimpleSelector:
        {
            QSharedPointer<HSSSimpleSelector> ss = qSharedPointerCast<HSSSimpleSelector>(d->currentSelectorNode);
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

QSharedPointer<HSSSimpleSelection> AXRController::selectFromTop(QSharedPointer<HSSRule> theRule, QSharedPointer<HSSDisplayObject> finalThisObj)
{
    //climb the tree
    QSharedPointer<HSSRule> rule = theRule;
    std::deque<QSharedPointer<HSSRule> > rules;
    while (rule->hasParent())
    {
        rules.push_front(rule);
        rule = rule->getParent();
    }
    rules.push_front(rule);
    //select from the topmost rule down to our subjects
    std::deque<QSharedPointer<HSSRule> >::iterator it = rules.begin();
    QSharedPointer<HSSRule> topRule = *it;
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains;
    QSharedPointer<HSSSimpleSelection> scope = topRule->getOriginalScope();
    QSharedPointer<HSSSimpleSelection> selection;
    for (; it != rules.end(); ++it)
    {
        QSharedPointer<HSSRule> itRule = *it;
        // TODO: check @this object
        QSharedPointer<HSSInstruction> instruction = itRule->getInstruction();
        if (instruction) {
            selection = QSharedPointer<HSSSimpleSelection>(new HSSSimpleSelection(itRule->getAppliedTo(), this));
            axr_log(LoggerChannelController, HSSSelection::logSelection(selection.data(), itRule->getSelectorChains()));
        } else {
            if(it+1 == rules.end())
            {
                selection = this->select(itRule->getSelectorChains(), scope, finalThisObj)->joinAll();
            }
            else
            {
                selection = this->select(itRule->getSelectorChains(), scope, itRule->getThisObj())->joinAll();
            }
        }
        if(selection->size() == 0)
        {
            break;
        }
        if ((it != rules.end()) && (it+1 != rules.end()))
        {
            QSharedPointer<HSSSimpleSelection> nextLevel(new HSSSimpleSelection(this));
            for (HSSSimpleSelection::const_iterator it2 = selection->begin(); it2 != selection->end(); ++it2)
            {
                const QSharedPointer<HSSDisplayObject> & displayObject = *it2;
                //if it is a container it may have children
                if (displayObject->isA(HSSObjectTypeContainer))
                {
                    QSharedPointer<HSSContainer> selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
                    QSharedPointer<HSSSimpleSelection> children = selectedContainer->getChildren();
                    if(children)
                    {
                        nextLevel->addSelection(children);
                    }
                }
            }
            scope = nextLevel;
        }
    }

    return selection;
}

AXRString AXRController::toString() const
{
    AXRString tempstr = "AXR Controller\n";
    if (d->root)
    {
        tempstr.append("\n\n\nROOT:");
        tempstr = d->root->toString();
    }

    if (!d->objectTree.empty())
    {
        tempstr.append("\n\n\nOBJECT TREE:");

        Q_FOREACH (const QSharedPointer<HSSObjectDefinition> &node, d->objectTree)
        {
            tempstr.append("\n").append(node->toString());
        }
    }

    if (!d->parserTree.empty())
    {
        tempstr.append("\n\n\nPARSER TREE:");

        Q_FOREACH (const QSharedPointer<HSSParserNode> &node, d->parserTree)
        {
            tempstr.append("\n").append(node->toString());
        }
    }

    return tempstr;
}

void AXRController::reset()
{
    d->objectTree.clear();
    d->styleSheetUrls.clear();
    d->parserTree.clear();
    d->rules.clear();
    d->root.clear();

    for (unsigned i = 0; i<d->currentContext.size(); ++i)
    {
        d->currentContext.pop();
    }

    d->currentChain.clear();
    d->currentSelectorNode.clear();
    d->currentChainCount = 0;
    d->currentChainSize = 0;
}

QSharedPointer<HSSContainer> & AXRController::root() const
{
    return d->root;
}

void AXRController::setRoot(QSharedPointer<HSSContainer> newRoot)
{
    d->root = newRoot;
    QSharedPointer<HSSParser> hssparser = this->document()->hssParser();
    if (hssparser->currentObjectContextSize() == 0)
    {
        hssparser->currentObjectContextAdd(newRoot);
    }
    newRoot->setRoot(true);
}

void AXRController::enterElement(const AXRString &elementName)
{
    QSharedPointer<HSSContainer> newContainer(new HSSContainer(this));
    newContainer->setName(elementName + "_source_obj");
    newContainer->setElementName(elementName);
    this->add(newContainer);
    d->currentContext.push(newContainer);
}

void AXRController::addAttribute(const AXRString &name, const AXRString &value)
{
    d->currentContext.top()->attributesAdd(name, value);
}

void AXRController::setContentText(const AXRString &text)
{
    d->currentContext.top()->setContentText(text);
}

void AXRController::appendContentText(const AXRString &text)
{
    d->currentContext.top()->appendContentText(text);
}

void AXRController::exitElement()
{
    d->currentContext.pop();
}

void AXRController::add(QSharedPointer<HSSDisplayObject> newElement)
{
    if (!d->root)
    {
        QSharedPointer<HSSContainer> cont = HSSContainer::asContainer(newElement);
        if (cont)
        {
            this->setRoot(cont);
        }
        else
        {
            AXRError("HSSController", "cannot add non-controller as root").raise();
        }
    }
    else
    {
        if (!d->currentContext.empty())
        {
            QSharedPointer<HSSContainer> theCurrent = d->currentContext.top();
            theCurrent->add(newElement);
        }
        else
        {
            AXRError("HSSController", "tried to add a container to nonexistent current").raise();
        }
    }
}

//object tree

void AXRController::addObjectTreeNode(const QSharedPointer<HSSObjectDefinition> &newObject)
{
    d->objectTree.append(newObject);
}

void AXRController::removeObjectTreeNodeAt(int index)
{
    d->objectTree.removeAt(index);
}

QSharedPointer<HSSObjectDefinition> AXRController::objectTreeNodeAt(int index)
{
    return d->objectTree[index];
}

QSharedPointer<HSSObjectDefinition> AXRController::objectTreeNodeNamed(const AXRString &name)
{
    /**
     *  @todo do this with an unordered_map for better performance
     */
    Q_FOREACH (const QSharedPointer<HSSObjectDefinition> &theObj, d->objectTree)
    {
        if (theObj && theObj->getObject()->name == name)
        {
            return theObj;
        }
    }

    throw AXRError("AXRController", "No object with name " + name + " was found");
}

//loadSheets

void AXRController::addStyleSheetUrl(const QUrl &url)
{
    d->styleSheetUrls.push_back(url);
}

void AXRController::removeStyleSheetUrlAt(int index)
{
    d->styleSheetUrls.removeAt(index);
}

QUrl AXRController::styleSheetUrlAt(int index) const
{
    return d->styleSheetUrls[index];
}

const QList<QUrl>& AXRController::styleSheetUrls() const
{
    return d->styleSheetUrls;
}

//parser node tree

const QList<QSharedPointer<HSSParserNode> >& AXRController::parserTree() const
{
    return d->parserTree;
}

void AXRController::setParserTree(const QList<QSharedPointer<HSSParserNode> > &newTree)
{
    d->parserTree = newTree;
}

void AXRController::addParserTreeNode(QSharedPointer<HSSParserNode> node)
{
    d->parserTree.append(node);
}

void AXRController::removeParserTreeNode(QSharedPointer<HSSParserNode> node)
{
    d->parserTree.removeOne(node);
}

//rules

const QList<QSharedPointer<HSSRule> >& AXRController::rules() const
{
    return d->rules;
}

void AXRController::addRule(QSharedPointer<HSSRule> &rule)
{
    d->rules.append(rule);
}

void AXRController::removeRuleAt(int index)
{
    d->rules.removeAt(index);
}

QSharedPointer<HSSRule> AXRController::ruleAt(int index)
{
    return d->rules[index];
}

int AXRController::ruleCount() const
{
    return d->rules.size();
}

void AXRController::_recursiveGetDescendants(QSharedPointer<HSSSimpleSelection> & ret, QSharedPointer<HSSSimpleSelection> scope)
{
    Q_FOREACH (QSharedPointer<HSSDisplayObject> theDO, *scope.data())
    {
        ret->add(theDO);
        QSharedPointer<HSSContainer> cont = HSSContainer::asContainer(theDO);
        if (cont)
        {
            this->_recursiveGetDescendants(ret, cont->getChildren());
        }
    }
}
