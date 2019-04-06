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

#include "axr.h"
#include "AXRController.h"

using namespace AXR;

namespace AXR
{
    class AXRControllerPrivate
    {
        friend class AXRController;

        std::vector<QSharedPointer<HSSContainer> > currentContext;

        QSharedPointer<HSSContainer> root;

        std::vector<QSharedPointer<HSSObjectDefinition> > objectTree;
        std::vector<AXRString> styleSheetUrls;
        std::list<QSharedPointer<HSSParserNode> > parserTree;
        std::vector<QSharedPointer<HSSRule> > rules;
        
        std::map<AXRString, QSharedPointer<HSSObject> > globals;

        QSharedPointer<HSSSelectorChain> currentChain;
        QSharedPointer<HSSParserNode> currentSelectorNode;
        size_t currentChainCount;
        size_t currentChainSize;

        AXRDocument *document;
        bool _logsSelections;
        QSharedPointer<HSSSimpleSelection> lastSelection;
        QSharedPointer<HSSEvent> currentEvent;
    };
}

AXRController::AXRController(AXRDocument *document)
: d(new AXRControllerPrivate)
{
    axr_log(LoggerChannelGeneralSpecific, "AXRController: creating controller");
    d->document = document;
    d->_logsSelections = true;
    d->lastSelection = QSharedPointer<HSSSimpleSelection>();
}

AXRController::AXRController()
: d(new AXRControllerPrivate)
{
    axr_log(LoggerChannelGeneralSpecific, "AXRController: creating controller");
    d->document = NULL;
    d->_logsSelections = true;
    d->lastSelection = QSharedPointer<HSSSimpleSelection>();
}

AXRController::~AXRController()
{
    axr_log(LoggerChannelGeneralSpecific, "AXRController: destructing controller");

    d->currentContext.clear();
    d->objectTree.clear();
    d->styleSheetUrls.clear();
    d->rules.clear();
    d->parserTree.clear();
    d->lastSelection.clear();
    d->root.clear();
    delete d;
}

AXRDocument* AXRController::document() const
{
    return d->document;
}

void AXRController::setDocument(AXRDocument * document) const
{
    d->document = document;
}

void AXRController::setUpTreeChangeObservers()
{
    axr_log(LoggerChannelController, "AXRController: setting up observers for content tree changes");
    std::vector<QSharedPointer<HSSRule> >::const_iterator it;
    for (it=rules().begin(); it!=rules().end(); ++it)
    {
        const QSharedPointer<HSSRule> & rule = *it;
        for (size_t k = 0; k < rule->childrenSize(); ++k)
        {
            const QSharedPointer<HSSRule> childRule = rule->childrenGet(k);
            this->recursiveSetUpTreeChangeObservers(childRule, this->root()->getChildren(), this->root(), true);
        }
    }
}

void AXRController::recursiveSetUpTreeChangeObservers(const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool subscribingToParent)
{
    std::vector<QSharedPointer<HSSSelectorChain> > selectorChains = rule->getSelectorChains();
    if (!selectorChains.empty())
    {
        QSharedPointer<HSSSelection> selection;
        if (!rule->getInstruction())
        {
            
            try
            {
                //the actual observing happens inside the select function
                selection = this->select(selectorChains, scope, thisObj, false, true, subscribingToParent);
            }
            catch (const AXRError &e)
            {
                e.raise();
            }
        }
        if (!selection) return;
        if (selection->isA(HSSSelectionTypeMultipleSelection))
        {
            QSharedPointer<HSSMultipleSelection> multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
            for (HSSMultipleSelection::iterator it = multiSel->begin(); it != multiSel->end(); ++it)
            {
                this->_setUpTreeChangeObservers(rule, qSharedPointerCast<HSSSimpleSelection>(*it), this->root(), subscribingToParent);
            }
        }
        else if (selection->isA(HSSSelectionTypeSimpleSelection))
        {
            this->_setUpTreeChangeObservers(rule, qSharedPointerCast<HSSSimpleSelection>(selection), this->root(), subscribingToParent);
        }
    }
}

void AXRController::_setUpTreeChangeObservers(const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSimpleSelection> scope, QSharedPointer<HSSContainer> thisObj, bool subscribingToParent)
{
    for (HSSSimpleSelection::const_iterator it = scope->begin(); it != scope->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & displayObject = *it;
        
        //if it is a container it may have children
        if (displayObject->isA(HSSObjectTypeContainer))
        {
            QSharedPointer<HSSContainer> selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
            for (size_t k = 0; k < rule->childrenSize(); ++k)
            {
                const QSharedPointer<HSSRule> childRule = rule->childrenGet(k);
                this->recursiveSetUpTreeChangeObservers(childRule, selectedContainer->getChildren(), selectedContainer, true);
            }
        }
    }
}

void AXRController::matchRulesToContentTree()
{
    QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection(this));
    rootScope->add(this->root());

    std::vector<QSharedPointer<HSSRule> >::const_iterator rulesIt;
    for (rulesIt=rules().begin(); rulesIt!=rules().end(); ++rulesIt)
    {
        const QSharedPointer<HSSRule> & rule = *rulesIt;
        rule->setOriginalScope(rootScope);

        std::vector<QSharedPointer<HSSSelectorChain> > selectorChains = rule->getSelectorChains();
        if (!selectorChains.empty())
        {
            QSharedPointer<HSSSelection> selection;
            try
            {
                selection = this->select(selectorChains, rootScope, this->root(), true, false, false);
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
    HSSSimpleSelection::const_iterator it;
    for (it=selection->begin(); it!=selection->end(); ++it) {
        const QSharedPointer<HSSDisplayObject> &displayObject = *it;
        axr_log(LoggerChannelGeneral, "AXRController: match " + displayObject->getElementName());
        displayObject->rulesAdd(rule, HSSRuleStateOff);

        displayObject->setNeedsRereadRules(true);
        displayObject->setNeedsSurface(true);
        displayObject->setDirty(true);

        //if it is a container it may have children
        if (displayObject->isA(HSSObjectTypeContainer))
        {
            QSharedPointer<HSSContainer> selectedContainer = qSharedPointerCast<HSSContainer>(displayObject);
            d->currentContext.push_back(selectedContainer);
            for (size_t k = 0; k < rule->childrenSize(); ++k)
            {
                const QSharedPointer<HSSRule> childRule = rule->childrenGet(k);
                this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer, true);
            }
            d->currentContext.pop_back();
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
                std::vector<QSharedPointer<HSSContainer> > newContainers = this->addContainerFromNewInstruction(instruction, rule, scope, container, false);
                std::vector<QSharedPointer<HSSContainer> >::const_iterator it;
                for (it=newContainers.begin(); it!=newContainers.end(); ++it) {
                    const QSharedPointer<HSSContainer> & newContainer = *it;
                    d->currentContext.push_back(newContainer);
                    for (size_t i = 0; i < rule->childrenSize(); ++i)
                    {
                        const QSharedPointer<HSSRule> childRule = rule->childrenGet(i);
                        this->recursiveMatchRulesToDisplayObjects(childRule, newContainer->getChildren(), newContainer, applyingInstructions);
                    }
                    newContainer->setNeedsRereadRules(true);
                    d->currentContext.pop_back();
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

                d->currentContext.push_back(container);
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
                            d->currentContext.push_back(theContainer);
                            //assign more rules
                            for (size_t i = 0; i < rule->childrenSize(); ++i)
                            {
                                const QSharedPointer<HSSRule> childRule = rule->childrenGet(i);
                                this->recursiveMatchRulesToDisplayObjects(childRule, theContainer->getChildren(), theContainer, applyingInstructions);
                            }
                            d->currentContext.pop_back();
                        }

                        theDO->setNeedsRereadRules(true);
                        //theDO->fireEvent(HSSEventTypeLoad);
                    }
                }
                d->currentContext.pop_back();
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
    else
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
                    rule->addOriginalScope(scope->joinAll());
                }

                try
                {
                    selection = this->select(selectorChains, scope, container, applyingInstructions, false, false);
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
                    selection = this->select(selectorChains, scope, this->root(), applyingInstructions, false, false);
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
                    this->recursiveApplyRulesToDisplayObjects(rule, *it, container, applyingInstructions);
                }
            }
            else if (selection->isA(HSSSelectionTypeSimpleSelection))
            {
                QSharedPointer<HSSSimpleSelection> simpleSel = qSharedPointerCast<HSSSimpleSelection>(selection);
                this->recursiveApplyRulesToDisplayObjects(rule, simpleSel, container, applyingInstructions);
            }
        }
    }
}

inline void AXRController::recursiveApplyRulesToDisplayObjects(const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSimpleSelection> scope, QSharedPointer<HSSContainer> container, bool applyingInstructions)
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
            d->currentContext.push_back(selectedContainer);
            for (size_t k = 0; k < rule->childrenSize(); ++k)
            {
                const QSharedPointer<HSSRule> childRule = rule->childrenGet(k);
                this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer, applyingInstructions);
            }
            d->currentContext.pop_back();
        }
    }
}

std::vector<QSharedPointer<HSSContainer> > AXRController::addContainerFromNewInstruction(const QSharedPointer<HSSInstruction> & instruction, const QSharedPointer<HSSRule> & rule, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSContainer> container, bool activeByDefault)
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
    
    std::vector<QSharedPointer<HSSContainer> > ret;
    
    for (uint i = 0; i < argssize; ++i)
    {
        QSharedPointer<HSSContainer> newContainer = QSharedPointer<HSSContainer>(new HSSContainer(this));
        newContainer->setName(elementName);
        newContainer->setElementName(elementName);
        newContainer->rulesAdd(rule, activeByDefault ? HSSRuleStateOn : HSSRuleStateOff);
        axr_log(LoggerChannelGeneral, "AXRController: created " + newContainer->getElementName());
        if(rule->isConditional())
        {
            this->addOffscreen(newContainer);
        } else {
            this->add(newContainer);
        }
        newContainer->setNeedsRereadRules(true);
        newContainer->setNeedsSurface(true);
        newContainer->setDirty(true);
        
        ret.push_back(newContainer);
    }
    return ret;
}

std::vector<QSharedPointer<HSSContainer> >& AXRController::currentContext() const
{
    return d->currentContext;
}

void AXRController::currentContextPush(QSharedPointer<HSSContainer> container)
{
    d->currentContext.push_back(container);
}

const QSharedPointer<HSSContainer> AXRController::currentContextPop() const
{
    QSharedPointer<HSSContainer> backObj = d->currentContext.back();
    d->currentContext.pop_back();
    return backObj;
}

void AXRController::activateRules()
{
    QSharedPointer<HSSSimpleSelection> rootScope(new HSSSimpleSelection(this));
    rootScope->add(this->root());

    std::vector<QSharedPointer<HSSRule> >::const_iterator it;
    for (it=rules().begin(); it!=rules().end(); ++it)
    {
        const QSharedPointer<HSSRule> & rule = *it;
        this->recursiveSetRuleState(rule, rootScope, this->root(), HSSRuleStateOn);
    }
}

void AXRController::recursiveSetRuleState(QSharedPointer<HSSRule> rule, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, HSSRuleState state)
{
    axr_log(LoggerChannelController, "AXRController: setting rule status");
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
            HSSMultipleSelection::const_iterator it;
            for (it=multiSel->begin(); it!=multiSel->end(); ++it)
            {
                const QSharedPointer<HSSSimpleSelection> & sel = *it;
                this->setRuleStateOnSelection(rule, sel, state);
            }
        }
        else if (selection->isA(HSSSelectionTypeSimpleSelection))
        {
            this->setRuleStateOnSelection(rule, qSharedPointerCast<HSSSimpleSelection>(selection), state);
        }
    }
}

void AXRController::setRuleStateOnSelection(QSharedPointer<HSSRule> rule, QSharedPointer<HSSSimpleSelection> selection, HSSRuleState state)
{
    for (HSSSimpleSelection::iterator it = selection->begin(); it != selection->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> & displayObject = *it;
        axr_log(LoggerChannelGeneral, "AXRController: setting rule state on " + displayObject->getElementName());
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
                    std::vector<QWeakPointer<HSSDisplayObject> >::const_iterator it2;
                    for (it2=appliedTo.begin(); it2!=appliedTo.end(); ++it2)
                    {
                        const QWeakPointer<HSSDisplayObject> & weakObject = *it2;
                        QSharedPointer<HSSDisplayObject> theDO = weakObject.toStrongRef();
                        children->add(theDO);
                        if(childRule->isConditional() && theInst->isA(HSSNewInstruction))
                        {
                            if(state == HSSRuleStateOn || state == HSSRuleStateActivate){
                                theDO->moveToContentTree();
                            } else {
                                theDO->moveToOffscreen();
                            }
                        }
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
    size_t theIndex = selectorChain->getStartingSelectorIndex();
    d->currentChain = selectorChain;
    d->currentChainSize = selectorChain->size();
    d->currentChainCount = theIndex;
    d->currentSelectorNode = selectorChain->get(theIndex);
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
    return this->select(selectorChains, scope, thisObj, false, false, false);
}

QSharedPointer<HSSSelection> AXRController::select(std::vector<QSharedPointer<HSSSelectorChain> > selectorChains, QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent)
{
    QSharedPointer<HSSMultipleSelection> ret(new HSSMultipleSelection(this));

    std::vector<QSharedPointer<HSSSelectorChain> >::iterator it;
    for (it=selectorChains.begin(); it!=selectorChains.end(); ++it)
    {
        QSharedPointer<HSSSelectorChain> chain = *it;
        this->initializeSelectorChain(chain);

        //if we have a descendants combinator here, it means we were at root context, since the parser
        //automatically prepends a @this before it when in another context
        if(d->currentSelectorNode->isA(HSSSelectorTypeCombinator)){
            HSSCombinatorType combinatorType = d->currentSelectorNode->getCombinatorType();
            if(combinatorType == HSSCombinatorTypeDescendants){
                this->readNextSelectorNode();
                QSharedPointer<HSSSelection> sel = this->selectAllHierarchical(scope, thisObj, true, subscribingToNotifications, subscribingToParent);
                ret->addSelection(sel);
            } else {
                AXRError("AXRController", "Selector chains at root level can't start with combinators except . (unimplemented) and ..").raise();
            }
        } else {
            QSharedPointer<HSSSelection> sel = this->selectHierarchical(scope, thisObj, processing, subscribingToNotifications, subscribingToParent);
            ret->addSelection(sel);
        }
    }
    d->lastSelection.clear();
    if (ret->size() == 1){
        QSharedPointer<HSSSimpleSelection> simpleSel = *(ret->begin());
        if (this->logsSelections())
        {
            axr_log(LoggerChannelController, AXRString("AXRController:").append(HSSSelection::logSelection(simpleSel.data(), selectorChains)));
        }
        return simpleSel;
    }
    if (this->logsSelections())
    {
        axr_log(LoggerChannelController, AXRString("AXRController:").append(HSSSelection::logSelection(ret.data(), selectorChains)));
    }
    return ret;
}

QSharedPointer<HSSSelection> AXRController::selectHierarchical(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent)
{
    QSharedPointer<HSSSelection> selection = this->selectOnLevel(scope, thisObj, processing, subscribingToParent, subscribingToParent);
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
            d->lastSelection = inner;

            for (HSSSimpleSelection::iterator it = inner->begin(); it!=inner->end(); ++it) {
                QSharedPointer<HSSDisplayObject> theDO = *it;
                if (theDO->isA(HSSObjectTypeContainer))
                {
                    QSharedPointer<HSSContainer> theContainer = qSharedPointerCast<HSSContainer>(theDO);
                    QSharedPointer<HSSSimpleSelection> children = theContainer->getChildren();
                    for (HSSSimpleSelection::iterator it2 = children->begin(); it2!=children->end(); ++it2) {
                        QSharedPointer<HSSDisplayObject> displayObject = *it2;
                        newScope->add(displayObject);
                    }
                }
            }
            if (selection->size() > 0)
            {
                return this->selectHierarchical(newScope, thisObj, processing, subscribingToNotifications, subscribingToNotifications);
            }
            return ret;
        }

        case HSSCombinatorTypeDescendants:
        {
            //if found, create a new scope with the children of the previous selections
            QSharedPointer<HSSSimpleSelection> newScope(new HSSSimpleSelection(this));
            this->readNextSelectorNode();
            QSharedPointer<HSSSimpleSelection> inner = selection->joinAll();
            for (HSSSimpleSelection::iterator it = inner->begin(); it != inner->end(); ++it)
            {
                QSharedPointer<HSSDisplayObject> theDO = *it;
                if (theDO->isA(HSSObjectTypeContainer))
                {
                    QSharedPointer<HSSContainer> theContainer = qSharedPointerCast<HSSContainer>(theDO);
                    QSharedPointer<HSSSimpleSelection> children = theContainer->getChildren();
                    for (HSSSimpleSelection::iterator it2 = children->begin(); it2 != children->end(); ++it2) {
                        QSharedPointer<HSSDisplayObject> childDO = *it2;
                        newScope->add(childDO);
                    }
                }
            }
            //recursively search for matches
            return this->selectAllHierarchical(newScope, thisObj, processing, subscribingToNotifications, subscribingToParent);
        }

        default:
            break;
        }
    }

    return selection;
}

QSharedPointer<HSSSelection> AXRController::selectAllHierarchical(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent)
{
    //get all the elements that we need to check
    QSharedPointer<HSSSimpleSelection> newScope(new HSSSimpleSelection(this));
    this->_recursiveGetDescendants(newScope, scope->joinAll());

    QSharedPointer<HSSSelection> selections = this->selectOnLevel(newScope, thisObj, processing, subscribingToNotifications, subscribingToParent);
    return selections;
}

QSharedPointer<HSSSelection> AXRController::selectOnLevel(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent)
{
    QSharedPointer<HSSSelection> selection = this->selectSimple(scope, thisObj, processing, subscribingToParent, subscribingToParent);
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
                this->_selectOnLevelSimple(ret, combinatorType, qSharedPointerCast<HSSSimpleSelection>(selection), thisObj, processing, subscribingToNotifications, subscribingToParent);
            }
            else if (selection->isA(HSSSelectionTypeMultipleSelection))
            {
                QSharedPointer<HSSMultipleSelection> multiSel = qSharedPointerCast<HSSMultipleSelection>(selection);
                std::vector<QSharedPointer<HSSSimpleSelection> >::iterator it;
                for (it=multiSel->begin(); it!=multiSel->end(); ++it)
                {
                    QSharedPointer<HSSSimpleSelection> simpleSel = *it;
                    this->_selectOnLevelSimple(ret, combinatorType, simpleSel, thisObj, processing, subscribingToNotifications, subscribingToParent);
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

inline void AXRController::_selectOnLevelSimple(QSharedPointer<HSSSimpleSelection> & ret, HSSCombinatorType combinatorType, QSharedPointer<HSSSimpleSelection> simpleSel, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent)
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
        QSharedPointer<HSSSimpleSelection> firstSiblingsSelection = this->selectSimple(firstSiblings, thisObj, false, false, false)->joinAll();
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
    ret->addSelection(this->selectOnLevel(retTemp, thisObj, processing, subscribingToNotifications, subscribingToParent));
}

QSharedPointer<HSSSelection> AXRController::selectSimple(QSharedPointer<HSSSelection> scope, QSharedPointer<HSSDisplayObject> thisObj, bool processing, bool subscribingToNotifications, bool subscribingToParent)
{
    bool done = false;
    bool needsReadNext = true;
    QSharedPointer<HSSSelection> selection = scope;
    
    QSharedPointer<HSSDisplayObject> currentThisObj = thisObj;

    while (!done)
    {
        switch (d->currentSelectorNode->getSelectorType())
        {
        case HSSSelectorTypeSimpleSelector:
        {
            QSharedPointer<HSSSimpleSelector> ss = qSharedPointerCast<HSSSimpleSelector>(d->currentSelectorNode);
            selection = ss->filterSelection(selection, currentThisObj, processing, subscribingToNotifications);

            QSharedPointer<HSSNameSelector> nameSel = ss->getName();
            if (nameSel && nameSel->isA(HSSSelectorTypeParentSelector))
            {
                QSharedPointer<HSSSimpleSelection> joinedSel = selection->joinAll();
                if (joinedSel->size() > 0)
                {
                    currentThisObj = joinedSel->itemAtIndex(0);
                }
            }
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

        std::vector<QSharedPointer<HSSObjectDefinition> >::const_iterator it;
        for (it=d->objectTree.begin(); it!=d->objectTree.end(); ++it)
        {
            const QSharedPointer<HSSObjectDefinition> &node = *it;
            tempstr.append("\n").append(node->toString());
            
        }
    }

    if (!d->parserTree.empty())
    {
        tempstr.append("\n\n\nPARSER TREE:");

        std::list<QSharedPointer<HSSParserNode> >::const_iterator it;
        for (it = d->parserTree.begin(); it != d->parserTree.end(); ++it)
        {
            const QSharedPointer<HSSParserNode> &node = *it;
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
    d->currentContext.clear();

    d->currentChain.clear();
    d->currentSelectorNode.clear();
    d->currentChainCount = 0;
    d->currentChainSize = 0;
    d->lastSelection.clear();
}

QSharedPointer<HSSContainer> & AXRController::root() const
{
    return d->root;
}

void AXRController::setRoot(QSharedPointer<HSSContainer> newRoot)
{
    d->root = newRoot;
    newRoot->setRoot(true);
}

void AXRController::enterElement(const AXRString &elementName)
{
    QSharedPointer<HSSContainer> newContainer(new HSSContainer(this));
    newContainer->setName(elementName);
    newContainer->setElementName(elementName);
    this->add(newContainer);
    d->currentContext.push_back(newContainer);
}

void AXRController::addAttribute(const AXRString &name, const AXRString &value)
{
    d->currentContext.back()->attributesAdd(name, value);
}

void AXRController::setContentText(const AXRString &text)
{
    d->currentContext.back()->setContentText(text);
}

void AXRController::appendContentText(const AXRString &text)
{
    d->currentContext.back()->appendContentText(text);
}

void AXRController::exitElement()
{
    d->currentContext.pop_back();
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
            QSharedPointer<HSSContainer> theCurrent = d->currentContext.back();
            theCurrent->add(newElement);
        }
        else
        {
            AXRError("HSSController", "tried to add a container to nonexistent current").raise();
        }
    }
}

void AXRController::addOffscreen(QSharedPointer<HSSDisplayObject> newElement)
{
    if (!d->currentContext.empty())
    {
        QSharedPointer<HSSContainer> theCurrent = d->currentContext.back();
        theCurrent->addOffscreen(newElement);
    }
    else
    {
        AXRError("HSSController", "tried to add a container to nonexistent current").raise();
    }
}

//object tree

void AXRController::addObjectTreeNode(const QSharedPointer<HSSObjectDefinition> &newObject)
{
    d->objectTree.push_back(newObject);
}

void AXRController::removeObjectTreeNodeAt(int index)
{
    
    d->objectTree.erase(d->objectTree.begin() + index);
}

QSharedPointer<HSSObjectDefinition> AXRController::objectTreeNodeAt(int index)
{
    return d->objectTree[index];
}

QSharedPointer<HSSObjectDefinition> AXRController::objectTreeNodeNamed(const AXRString &name)
{
    QSharedPointer<HSSObjectDefinition> errorState;
    /**
     *  @todo do this with an unordered_map for better performance
     */
    std::vector<QSharedPointer<HSSObjectDefinition> >::const_iterator it;
    for (it = d->objectTree.begin(); it != d->objectTree.end(); ++it)
    {
        const QSharedPointer<HSSObjectDefinition> &theObj = *it;
        if (theObj && theObj->name() == name)
        {
            return theObj;
        }
    }

    return errorState;
}

//loadSheets

void AXRController::addStyleSheetUrl(const AXRString &url)
{
    d->styleSheetUrls.push_back(url);
}

void AXRController::removeStyleSheetUrlAt(int index)
{
    d->styleSheetUrls.erase(d->styleSheetUrls.begin()+index);
}

AXRString AXRController::styleSheetUrlAt(int index) const
{
    return d->styleSheetUrls[index];
}

const std::vector<AXRString>& AXRController::styleSheetUrls() const
{
    return d->styleSheetUrls;
}

//parser node tree

const std::list<QSharedPointer<HSSParserNode> >& AXRController::parserTree() const
{
    return d->parserTree;
}

void AXRController::setParserTree(const std::list<QSharedPointer<HSSParserNode> > &newTree)
{
    d->parserTree = newTree;
}

void AXRController::addParserTreeNode(QSharedPointer<HSSParserNode> node)
{
    d->parserTree.push_back(node);
}

void AXRController::removeParserTreeNode(QSharedPointer<HSSParserNode> node)
{
    std::list<QSharedPointer<HSSParserNode> >::iterator searchResult = std::find(d->parserTree.begin(), d->parserTree.end(), node);
    if(searchResult != d->parserTree.end()){
        d->parserTree.erase(searchResult);
    }
}

//rules

const std::vector<QSharedPointer<HSSRule> >& AXRController::rules() const
{
    return d->rules;
}

void AXRController::addRule(QSharedPointer<HSSRule> &rule)
{
    d->rules.push_back(rule);
}

void AXRController::removeRuleAt(int index)
{
    d->rules.erase(d->rules.begin()+index);
}

QSharedPointer<HSSRule> AXRController::ruleAt(int index)
{
    return d->rules.at(index);
}

int AXRController::ruleCount() const
{
    return (int)d->rules.size();
}

void AXRController::_recursiveGetDescendants(QSharedPointer<HSSSimpleSelection> & ret, QSharedPointer<HSSSimpleSelection> scope)
{
    for (HSSSimpleSelection::iterator it = scope->begin(); it != scope->end(); ++it)
    {
        QSharedPointer<HSSDisplayObject> theDO = *it;
        ret->add(theDO);
        QSharedPointer<HSSContainer> cont = HSSContainer::asContainer(theDO);
        if (cont)
        {
            this->_recursiveGetDescendants(ret, cont->getChildren());
        }
    }
}

void AXRController::setGlobalVariable(AXRString name, QSharedPointer<HSSObject> obj)
{
    if (obj)
    {
        d->globals[name] = obj;
    }
}

QSharedPointer<HSSObject> AXRController::getGlobalVariable(AXRString name)
{
    return d->globals[name];
}

void AXRController::setLogsSelections(bool newValue)
{
    d->_logsSelections = newValue;
}

const bool AXRController::logsSelections() const
{
    return d->_logsSelections;
}

const QSharedPointer<HSSSimpleSelection> AXRController::getLastSelection() const
{
    return d->lastSelection;
}

void AXRController::setCurrentEvent(QSharedPointer<HSSEvent> newEvent) const
{
    d->currentEvent = newEvent;
}

const QSharedPointer<HSSEvent> AXRController::getCurrentEvent() const
{
    return d->currentEvent;
}
