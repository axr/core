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
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2012/06/22
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 33
 *
 ********************************************************************/

#include "AXRController.h"
#include "../AXR.h"
#include "AXRExceptions.h"
#include <sstream>
#include "../axr/AXRDebugging.h"
#include "../axr/errors/errors.h"
#include "../hss/objects/HSSDisplayObject.h"
#include <boost/pointer_cast.hpp>
#include "../hss/parsing/HSSSelectorChain.h"
#include "../hss/parsing/HSSFilters.h"
#include "../hss/parsing/HSSNegation.h"
#include "../hss/parsing/HSSFlag.h"

using namespace AXR;


AXRController::AXRController()
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRController: creating controller");
    this->currentContext = std::stack<HSSContainer::p>();
}

AXRController::~AXRController()
{
    axr_log(AXR_DEBUG_CH_GENERAL | AXR_DEBUG_CH_GENERAL_SPECIFIC, "AXRController: destructing controller");
    
    this->objectTree.clear();
    this->loadSheets.clear();
    this->rules.clear();
    this->parserTree.clear();
}

void AXRController::recursiveMatchRulesToDisplayObjects(const HSSRule::p & rule, const std::vector<HSSDisplayObject::p> & scope, HSSContainer::p container, bool applyingInstructions)
{
    axr_log(AXR_DEBUG_CH_GENERAL, "AXRController: recursive matching rules to display objects");
    HSSInstruction::p instruction = rule->getInstruction();
    if (instruction && applyingInstructions) {
        switch (instruction->getInstructionType()) {
            case HSSNewInstruction:
            {
                if(container){
                    std::string elementName = rule->selectorChainsLast()->subject()->getElementName();
                    unsigned i;
                    unsigned argssize = 1;
                    HSSParserNode::p argument = instruction->getArgument();
                    if(argument){
                        if (argument->isA(HSSParserNodeTypeNumberConstant)) {
                            HSSNumberConstant::p argnum = boost::static_pointer_cast<HSSNumberConstant>(argument);
                            argssize = (int)argnum->getValue();
                        }
                    }
                    
                    for (i=0; i<argssize; i++) {
                        HSSContainer::p newContainer = HSSContainer::p(new HSSContainer());
                        newContainer->setName(elementName);
                        newContainer->setElementName(elementName);
                        rule->setThisObj(newContainer);
                        newContainer->setController(this);
                        newContainer->rulesAdd(rule, (rule->getActiveByDefault() ? HSSRuleStateOn : HSSRuleStateOff ));
                        axr_log(AXR_DEBUG_CH_GENERAL, "AXRController: created "+newContainer->getElementName());
                        this->add(newContainer);
                        newContainer->setNeedsRereadRules(true);
                        newContainer->setNeedsSurface(true);
                        newContainer->setDirty(true);
                        unsigned i, size;
                        this->currentContext.push(newContainer);
                        for (i=0, size=rule->childrenSize(); i<size; i++) {
                            const HSSRule::p childRule = rule->childrenGet(i);
                            this->recursiveMatchRulesToDisplayObjects(childRule, newContainer->getChildren(), newContainer, applyingInstructions);
                        }
                        newContainer->setNeedsRereadRules(true);
                        //newContainer->fireEvent(HSSEventTypeLoad);
                        this->currentContext.pop();
                    }
                } else {
                    AXRWarning::p(new AXRWarning("AXRController", "You cannot use the #new instruction at root level"))->raise();
                }
                
                break;
            }
                
            case HSSMoveInstruction:
            {
                if(container){
                    HSSContainer::p parent = container->getParent();
                    if (parent) {
                        std::vector<HSSDisplayObject::p> moveScope = parent->getChildren();
                        std::vector< std::vector<HSSDisplayObject::p> > selection = this->select(rule->getSelectorChains(), moveScope, container);
                        unsigned i, j, k, size, size2, size3;
                        
                        this->currentContext.push(container);
                        //move the children over
                        for (i=0, size=selection.size(); i<size; i++) {
                            std::vector<HSSDisplayObject::p> inner = selection[i];
                            for (j=0, size2=inner.size(); j<size2; j++) {
                                HSSDisplayObject::p theDO = inner[j];
                                if(theDO != container){
                                    theDO->removeFromParent();
                                    rule->setThisObj(theDO);
                                    theDO->setController(this);
                                    theDO->rulesAdd(rule, (rule->getActiveByDefault() ? HSSRuleStateOn : HSSRuleStateOff ));
                                    axr_log(AXR_DEBUG_CH_GENERAL, "AXRController: moved "+theDO->getElementName());
                                    this->add(theDO);
                                    theDO->setNeedsRereadRules(true);
                                    theDO->setNeedsSurface(true);
                                    theDO->setDirty(true);
                                    
                                    if(theDO->isA(HSSObjectTypeContainer)){
                                        HSSContainer::p theContainer = boost::static_pointer_cast<HSSContainer>(theDO);
                                        this->currentContext.push(theContainer);
                                        //assign more rules
                                        for (k=0, size3=rule->childrenSize(); k<size3; k++) {
                                            const HSSRule::p childRule = rule->childrenGet(k);
                                            this->recursiveMatchRulesToDisplayObjects(childRule, theContainer->getChildren(), theContainer, applyingInstructions);
                                        }
                                        this->currentContext.pop();
                                    }
                                    
                                    theDO->setNeedsRereadRules(true);
                                    //theDO->fireEvent(HSSEventTypeLoad);
                                }
                                
                            }
                        }
                        this->currentContext.pop();
                    } else {
                        AXRWarning::p(new AXRWarning("AXRController", "You cannot use the #move instruction at root level"))->raise();
                    }
                }
                
                break;
            }
                
            case HSSDeleteInstruction:
            {
                if(container){
                    //select the items to be deleted
                    std::vector< std::vector<HSSDisplayObject::p> > selection = this->select(rule->getSelectorChains(), scope, container);
                    unsigned i, j, size, size2;
                    for (i=0, size=selection.size(); i<size; i++) {
                        std::vector<HSSDisplayObject::p> inner = selection[i];
                        for (j=0, size2=inner.size(); j<size2; j++) {
                            inner[j]->removeFromParent();
                        }
                    }
                } else {
                    AXRWarning::p(new AXRWarning("AXRController", "You cannot use the #delete instruction at root level"))->raise();
                }
                break;
            }
            
            default:
                break;
        }
        
    } else if(!instruction) {
        std::vector<HSSSelectorChain::p> selectorChains = rule->getSelectorChains();
        if(selectorChains.size() > 0){
            std::vector< std::vector<HSSDisplayObject::p> > selection;
            
            if (container){
                //if it starts with a combinator, adjust the scope and selector chain
                bool useAdjustedScope = false;
                std::vector<HSSDisplayObject::p> adjustedScope;
                if(this->currentSelectorNode->isA(HSSParserNodeTypeCombinator)){
                    useAdjustedScope = true;
                    adjustedScope.push_back(container);
                    this->currentChain->prepend(HSSSelector::p(new HSSSelector(container->getElementName())));
                    this->currentChainSize ++;
                    this->currentSelectorNode = this->currentChain->get(0);
                }
                
                if (applyingInstructions) {
                    //we observe the parent for dom changes
                    container->observe(HSSObservablePropertyTreeChange, HSSObservablePropertyValue, rule.get(), new HSSValueChangedCallback<HSSRule>(rule.get(), &HSSRule::treeChanged));
                    rule->setObservedTreeChanger(container.get());
                    rule->setThisObj(container);
                    rule->setOriginalScope(scope);
                }
                
                selection = this->select(selectorChains, (useAdjustedScope ? adjustedScope : scope), container);
            } else {
                selection = this->select(selectorChains, scope, this->getRoot());
            }
            
            unsigned i, j, k, size, size2, size3;
            for (i=0, size=selection.size(); i<size; i++) {
                std::vector<HSSDisplayObject::p> inner = selection[i];
                for (j=0, size2=inner.size(); j<size2; j++) {
                    const HSSDisplayObject::p & displayObject = inner[j];
                    axr_log(AXR_DEBUG_CH_GENERAL, "AXRController: match "+displayObject->getElementName());
                    displayObject->rulesAdd(rule, (rule->getActiveByDefault() ? HSSRuleStateOn : HSSRuleStateOff ));
                    
                    displayObject->setNeedsRereadRules(true);
                    displayObject->setNeedsSurface(true);
                    displayObject->setDirty(true);
                    
                    //if it is a container it may have children
                    if(displayObject->isA(HSSObjectTypeContainer)){
                        HSSContainer::p selectedContainer = boost::static_pointer_cast<HSSContainer>(displayObject);
                        this->currentContext.push(selectedContainer);
                        for (k=0, size3=rule->childrenSize(); k<size3; k++) {
                            const HSSRule::p childRule = rule->childrenGet(k);
                            this->recursiveMatchRulesToDisplayObjects(childRule, selectedContainer->getChildren(), selectedContainer, applyingInstructions);
                        }
                        this->currentContext.pop();
                    }
                    
                    displayObject->setNeedsRereadRules(true);
                }
            }
        }
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
    if(this->currentChainCount < this->currentChainSize-1){
        this->currentSelectorNode = this->currentChain->get(this->currentChainCount+1);
        this->currentChainCount++;
    }
}

bool AXRController::isAtEndOfSelector()
{
    return this->currentChainCount >= this->currentChainSize-1;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::select(std::vector<HSSSelectorChain::p> selectorChains, const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj)
{
    return this->select(selectorChains, scope, thisObj, true);
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::select(std::vector<HSSSelectorChain::p> selectorChains, const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool processing)
{
    std::vector< std::vector<HSSDisplayObject::p> > ret;
    std::vector< std::vector<HSSDisplayObject::p> > selections;
    HSSSelectorChain::const_it it;
    
    for (it=selectorChains.begin(); it!=selectorChains.end(); it++) {
        this->initializeSelectorChain(*it);
        selections = this->selectHierarchical(scope, thisObj, false, processing);
        if(selections.size() > 0){
            ret.insert(ret.end(), selections.begin(), selections.end());
        }
    }
    
    return ret;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::selectHierarchical(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj)
{
    
    return this->selectHierarchical(scope, thisObj, false, true);
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::selectHierarchical(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool negating, bool processing)
{
    std::vector< std::vector<HSSDisplayObject::p> > selections = this->selectOnLevel(scope, thisObj, negating, processing);
    unsigned i, size;
    for (i=0, size=selections.size(); i<size; i++) {
        std::vector<HSSDisplayObject::p> selection = selections[i];
        if(selection.size() > 0){
            if(!this->isAtEndOfSelector()){
                if(this->currentSelectorNode->isA(HSSParserNodeTypeCombinator)){
                    HSSCombinator::p combinator = boost::static_pointer_cast<HSSCombinator>(this->currentSelectorNode);
                    HSSCombinatorType combinatorType = combinator->getCombinatorType();
                    switch (combinatorType) {
                        case HSSCombinatorTypeChildren:
                        {
                            std::vector<HSSDisplayObject::p> newScope;
                            unsigned j, size2, k, size3;
                            for(j=0, size2 = selection.size(); j<size2; j++){
                                if(selection[j]->isA(HSSObjectTypeContainer)){
                                    HSSContainer::p theContainer = boost::static_pointer_cast<HSSContainer>(selection[j]);
                                    const std::vector<HSSDisplayObject::p> children = theContainer->getChildren();
                                    for(k=0, size3 = children.size(); k<size3; k++){
                                        newScope.push_back(children[k]);
                                    }
                                }
                            }
                            this->readNextSelectorNode();
                            return this->selectHierarchical(newScope, thisObj, false, processing);
                            break;
                        }
                            
                        case HSSCombinatorTypeDescendants:
                        {
                            std::vector<HSSDisplayObject::p> newScope;
                            unsigned j, size2, k, size3;
                            for(j=0, size2 = selection.size(); j<size2; j++){
                                if(selection[j]->isA(HSSObjectTypeContainer)){
                                    HSSContainer::p theContainer = boost::static_pointer_cast<HSSContainer>(selection[j]);
                                    const std::vector<HSSDisplayObject::p> children = theContainer->getChildren();
                                    for(k=0, size3 = children.size(); k<size3; k++){
                                        newScope.push_back(children[k]);
                                    }
                                }
                            }
                            
                            this->readNextSelectorNode();
                            return this->selectAllHierarchical(newScope, thisObj, false, processing);
                            break;
                        }
                            
                        default:
                            break;
                    }
                }
            }
        }
    }
    
    return selections;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::selectAllHierarchical(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool negating, bool processing)
{
    std::vector< std::vector<HSSDisplayObject::p> > selections = this->selectOnLevel(scope, thisObj, negating, processing);
    
    unsigned i, j, k, size, size2, size3;
    for (i=0, size=selections.size(); i<size; i++) {
        
        std::vector<HSSDisplayObject::p> newSearch;
        for(j=0, size2=scope.size(); j<size2; j++){
            if(scope[j]->isA(HSSObjectTypeContainer)){
                HSSContainer::p theContainer = boost::static_pointer_cast<HSSContainer>(scope[j]);
                const std::vector<HSSDisplayObject::p> children = theContainer->getChildren();
                for(k=0, size3 = children.size(); k<size3; k++){
                    newSearch.push_back(children[k]);
                }
            }
        }
        
        std::vector< std::vector<HSSDisplayObject::p> > childSelection;
        if(newSearch.size() > 0){
            this->currentChainCount -= 1;
            this->readNextSelectorNode();
            childSelection = this->selectAllHierarchical(newSearch, thisObj, false, processing);
            for (j=0, size2=selections.size(); j<size2; j++) {
                //std::vector<HSSDisplayObject::p> & selection = selections[j];
                for (k=0, size3=childSelection.size(); k<size3; k++) {
                    selections[j].insert(selections[j].end(), childSelection[k].begin(), childSelection[k].end());
                }
            }
        }
    }
    
    return selections;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::selectOnLevel(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool negating, bool processing)
{
    std::vector< std::vector<HSSDisplayObject::p> > selections = this->selectSimple(scope, thisObj, negating, processing);
    bool atEnd = this->isAtEndOfSelector();
    if(!atEnd){
        if(this->currentSelectorNode->isA(HSSParserNodeTypeCombinator))
        {
            std::vector< std::vector<HSSDisplayObject::p> > ret;
            HSSCombinator::p combinator = boost::static_pointer_cast<HSSCombinator>(this->currentSelectorNode);
            HSSCombinatorType combinatorType = combinator->getCombinatorType();
            switch (combinatorType) {
                case HSSCombinatorTypeSiblings:
                {
                    //if found, just select the right part
                    this->readNextSelectorNode();
                    unsigned i, size;
                    for (i=0, size=selections.size(); i<size; i++) {
                        std::vector<HSSDisplayObject::p> selection = selections[i];
                        std::vector< std::vector<HSSDisplayObject::p> > newSelection = this->selectSimple(scope, thisObj, negating, processing);
                        ret.insert(ret.end(), newSelection.begin(), newSelection.end());
                    }
                    return ret;
                }
                    
                case HSSCombinatorTypeNextSiblings:
                {
                    //find the selected ones on the right, and select all the following ones that match
                    unsigned i, size;
                    for (i=0, size=selections.size(); i<size; i++) {
                        std::vector<HSSDisplayObject::p> selection = selections[i];
                        std::vector<HSSDisplayObject::p> vect(scope);
                        if(selection.size() > 0){
                            std::vector<HSSDisplayObject::p>::iterator it;
                            it = std::find(vect.begin(), vect.end(), selection.front());
                            std::vector<HSSDisplayObject::p> right(it+1, vect.end());
                            this->readNextSelectorNode();
                            std::vector< std::vector<HSSDisplayObject::p> > newSelection = this->selectSimple(right, thisObj, negating, processing);
                            ret.insert(ret.end(), newSelection.begin(), newSelection.end());
                        }
                    }
                    return ret;
                }
                    
                case HSSCombinatorTypePreviousSiblings:
                {
                    //find the selected ones on the left, and select all the previous ones that match
                    this->readNextSelectorNode();
                    unsigned i, size;
                    for (i=0, size=selections.size(); i<size; i++) {
                        std::vector<HSSDisplayObject::p> selection = selections[i];
                        if(selection.size() > 0){
                            std::vector<HSSDisplayObject::p> vect(scope);
                            std::vector<HSSDisplayObject::p>::iterator it;
                            it = std::find(vect.begin(), vect.end(), selection.back());
                            std::vector<HSSDisplayObject::p> right(vect.begin(), it);
                            this->readNextSelectorNode();
                            std::vector< std::vector<HSSDisplayObject::p> > newSelection = this->selectSimple(right, thisObj, negating, processing);
                            ret.insert(ret.end(), newSelection.begin(), newSelection.end());
                        }
                        
                    }
                    return ret;
                }
                    
                default:
                    break;
            }
        }  
    }
    
    return selections;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::selectSimple(const std::vector<HSSDisplayObject::p> & scope, HSSDisplayObject::p thisObj, bool negating, bool processing)
{
    std::vector< std::vector<HSSDisplayObject::p> >ret;
    std::vector<HSSDisplayObject::p> currentSelection;
    
    unsigned i, size;
    HSSParserNodeType selectorType = this->currentSelectorNode->getType();
    switch (selectorType) {
        case HSSParserNodeTypeSelector:
        {
            //select only elements with matching element name
            HSSSelector::p selector = boost::static_pointer_cast<HSSSelector>(this->currentSelectorNode);
            for (i=0, size=scope.size(); i<size; i++) {
                bool match = scope[i]->getElementName() == selector->getElementName();
                if((match && !negating) || (!match && negating) ){
                    currentSelection.push_back(scope[i]);
                }
            }
            //we're done negating for now
            negating = false;
            
            this->readNextSelectorNode();
            break;
        }
        
        case HSSParserNodeTypeUniversalSelector:
        {
            if(negating){
                //nothing will be selected
            } else {
                //the entire scope will be selected
                currentSelection = scope;
            }
            //we're done negating for now
            negating = false;
            this->readNextSelectorNode();
            break;
        }
            
        case HSSParserNodeTypeFilter:
        case HSSParserNodeTypeFlag:
        {
            //the entire scope will be selected
            currentSelection = scope;
            break;
        }
            
        case HSSParserNodeTypeThisSelector:
        {
            if(negating){
                throw AXRError::p(new AXRError("AXRController", "Cannot negate the @this object yet "));
            }
            const HSSDisplayObject::p tObjArr[1] = {thisObj};
            const std::vector<HSSDisplayObject::p>tObjScope(tObjArr, tObjArr+1);
            currentSelection = tObjScope;
            this->readNextSelectorNode();
            //we're done negating for now
            negating = false;
            break;
        }
            
        case HSSParserNodeTypeRootSelector:
        {
            if(negating){
                throw AXRError::p(new AXRError("AXRController", "Cannot negate the @root object yet "));
            }
            const HSSDisplayObject::p rootObjArr[1] = {this->getRoot()};
            const std::vector<HSSDisplayObject::p>rObjScope(rootObjArr, rootObjArr+1);
            currentSelection = rObjScope;
            this->readNextSelectorNode();
            //we're done negating for now
            negating = false;
            break;
        }
            
        case HSSParserNodeTypeNegation:
        {
            HSSNegation::p negation = boost::static_pointer_cast<HSSNegation>(this->currentSelectorNode);
            this->readNextSelectorNode();
            return this->selectSimple(scope, thisObj, true, processing);
        }
        
        default:
            throw AXRError::p(new AXRError("AXRController", "Unknown node type "+HSSParserNode::parserNodeStringRepresentation(selectorType)+" in selector"));
    }
    
    ret = this->filterSelection(currentSelection, negating, processing);
    
    return ret;
}

std::vector< std::vector<HSSDisplayObject::p> > AXRController::filterSelection( std::vector<HSSDisplayObject::p> &selection, bool negating, bool processing)
{
    std::vector< std::vector<HSSDisplayObject::p> >ret;
    
    switch (this->currentSelectorNode->getType()) {
        case HSSParserNodeTypeFilter:
        {
            HSSFilter::p theFilter = boost::static_pointer_cast<HSSFilter>(this->currentSelectorNode);
            switch (theFilter->getFilterType()) {
                case HSSFilterTypeEach:
                {
                    unsigned i, size;
                    for (i=0, size=selection.size(); i<size; i++) {
                        std::vector<HSSDisplayObject::p> sel;
                        sel.push_back(selection[i]);
                        ret.push_back(sel);
                    }
                    this->readNextSelectorNode();
                    break;
                }
                    
                default:
                {
                    std::vector<HSSDisplayObject::p> tempSel = theFilter->apply(selection, negating);
                    if(!this->isAtEndOfSelector()){
                        this->readNextSelectorNode();
                        return this->filterSelection(tempSel, false, processing);
                    } else {
                        this->readNextSelectorNode();
                        ret.push_back(tempSel);
                    }
                    break;
                }
            }
            break;
        }
            
        case HSSParserNodeTypeFlag:
        {
            HSSFlag::p theFlag = boost::static_pointer_cast<HSSFlag>(this->currentSelectorNode);
            if(processing){
                HSSDisplayObject::const_it it;
                for (it=selection.begin(); it!=selection.end(); it++) {
                    HSSDisplayObject::p sel = *it;
                    theFlag->apply(sel, negating);
                }
                if(!this->isAtEndOfSelector()){
                    this->readNextSelectorNode();
                    return this->filterSelection(selection, false, processing);
                    
                } else {
                    this->readNextSelectorNode();
                    ret.push_back(selection);
                }
            } else {
                std::vector<HSSDisplayObject::p> tempSel = theFlag->filter(selection, negating);
                if(!this->isAtEndOfSelector()){
                    this->readNextSelectorNode();
                    //if(!theFlag->getPurging()){
                    //    return this->filterSelection(selection, false, processing);
                    //} else {
                        return this->filterSelection(tempSel, false, processing);
                    //}
                    
                } else {
                    this->readNextSelectorNode();
                    ret.push_back(tempSel);
                }
            }
            
                        
            break;
        }
            
        case HSSParserNodeTypeNegation:
        {
            this->readNextSelectorNode();
            return this->filterSelection(selection, true, processing);
        }
            
            
        default:
        {
            ret.push_back(selection);
            break;
        }
    }
    
    return ret;
}

std::string AXRController::toString()
{
    std::string tempstr = "AXR Controller\n";
    if(this->root != NULL){
        tempstr.append("\n\n\nROOT:");
        tempstr = this->root->toString();  
    }
    unsigned i;
    if(this->objectTree.size() > 0)
    {
        tempstr.append("\n\n\nOBJECT TREE:");
        for (i=0; i<this->objectTree.size(); i++) {
            tempstr.append("\n").append(this->objectTree[i]->toString());
        }
    }
    
    if(this->parserTree.size() > 0)
    {
        tempstr.append("\n\n\nPARSER TREE:");
        HSSParserNode::it pIt;
        for (pIt=this->parserTree.begin(); pIt!=this->parserTree.end(); pIt++) {
            tempstr.append("\n").append((*pIt)->toString());
        }
    }
    
    return tempstr;
}


void AXRController::reset()
{
    unsigned i;
    
    this->objectTree.clear();
    this->loadSheets.clear();
    this->parserTree.clear();
    this->rules.clear();
    this->root.reset();
    for(i=0; i<this->currentContext.size(); i++)
    {
        this->currentContext.pop();
    }
    
    this->currentChain.reset();
    this->currentSelectorNode.reset();
    this->currentChainCount = 0;
    this->currentChainSize = 0;
}

HSSContainer::p & AXRController::getRoot()
{
    return this->root;
}

void AXRController::setRoot(HSSContainer::p newRoot){
    this->root = newRoot;
    HSSParser::p hssparser = AXRCore::getInstance()->getParserHSS();
    if(hssparser->currentObjectContextSize() == 0){
        hssparser->currentObjectContextAdd(newRoot);
    }
    newRoot->setRoot(true);
}

void AXRController::enterElement(std::string elementName)
{
    //std_log1("enter element " + elementName);
    HSSContainer::p newContainer(new HSSContainer());
    newContainer->setName(elementName+"_source_obj");
    newContainer->setElementName(elementName);
    this->add(newContainer);
    this->currentContext.push(newContainer);
}

void AXRController::addAttribute(std::string name, std::string value)
{
    //std_log1(std::string("adding attribute " + name + " and value " + value));
    this->currentContext.top()->attributesAdd(name, value);
}

void AXRController::setContentText(std::string text)
{
    this->currentContext.top()->setContentText(text);
}

void AXRController::appendContentText(std::string text)
{
    this->currentContext.top()->appendContentText(text);
}

void AXRController::exitElement()
{
    this->currentContext.pop();
}


void AXRController::add(HSSDisplayObject::p newElement)
{
    newElement->setController(this);
    
    if(!this->root){
        HSSContainer::p cont = HSSContainer::asContainer(newElement);
        if (cont){
            this->setRoot(cont);
        } else {
            std_log1("############## HSSController: cannot add non-controller as root");
        }
        
    } else {
        if(this->currentContext.size() != 0){
            HSSContainer::p theCurrent = this->currentContext.top();
            theCurrent->add(newElement);
            
        } else {
            std_log1("############## HSSController: tried to add a container to nonexistent current");
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
    this->objectTree.erase(this->objectTree.begin()+index);
}

HSSObjectDefinition::p & AXRController::objectTreeGet(unsigned index)
{
    return this->objectTree[index];
}

HSSObjectDefinition::p & AXRController::objectTreeGet(std::string name)
{
    /**
     *  @todo do this with an unordered_map for better performance
     */
    unsigned i, size;
    for (i=0, size=this->objectTree.size(); i<size; i++) {
        HSSObjectDefinition::p & theObj = this->objectTreeGet(i);
        if(theObj && theObj->getObject()->name == name){
            return theObj;
        }
    }
    
    throw AXRError::p(new AXRError("AXRController", "No object with name "+name+" was found"));
}

//loadSheets

void AXRController::loadSheetsAdd(std::string sheet)
{
    this->loadSheets.push_back(sheet);
}

void AXRController::loadSheetsRemove(unsigned index)
{
    this->loadSheets.erase(this->loadSheets.begin()+index);
}

std::string AXRController::loadSheetsGet(unsigned index)
{
    return this->loadSheets[index];
}

const std::vector<std::string> AXRController::loadSheetsGet() const
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
    if(it != this->parserTree.end()){
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
    this->rules.erase(this->rules.begin()+index);
}

HSSRule::p & AXRController::rulesGet(unsigned index)
{
    return this->rules[index];
}

unsigned const AXRController::rulesSize()
{
    return this->rules.size();
}





