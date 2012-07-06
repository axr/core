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
 ********************************************************************/

#include "HSSRule.h"
#include <iostream>
#include "../../axr/AXRDebugging.h"
#include "../../axr/AXRController.h"
#include "../objects/HSSDisplayObject.h"
#include "../objects/HSSContainer.h"
#include "HSSFilter.h"

using namespace AXR;

HSSRule::HSSRule()
: HSSStatement()
{
    this->type = HSSStatementTypeRule;
    this->_activeByDefault = true;
    this->observedTreeChanger = NULL;
}

HSSRule::HSSRule(const HSSRule & orig)
: HSSStatement(orig)
{
    this->_interactors = boost::unordered_map<HSSFilterType, std::vector<boost::shared_ptr<HSSDisplayObject> > >(orig._interactors);
    this->_activeByDefault = orig._activeByDefault;
    this->observedTreeChanger = NULL;
}

HSSRule::p HSSRule::clone() const
{
    return boost::static_pointer_cast<HSSRule, HSSClonable>(this->cloneImpl());
}

HSSRule::~HSSRule()
{
    unsigned i;
    for(i=0; i<this->properties.size(); i++){
        this->propertiesRemoveLast();
    }
    if(this->observedTreeChanger != NULL){
        this->observedTreeChanger->removeObserver(HSSObservablePropertyTreeChange, HSSObservablePropertyValue, this);
    }
}

std::string HSSRule::toString()
{
    std::string tempstr = std::string("HSSRule with the following selector chain: \n");
    if (this->selectorChains.size() > 0){
        HSSSelectorChain::const_it sIt;
        for (sIt=this->selectorChains.begin(); sIt!=this->selectorChains.end(); sIt++) {
            tempstr.append("   ").append((*sIt)->toString());
        }
        
    } else {
        tempstr.append("WARNING: this rule has no selector chain!\n");
    }
    
    const int pccount = this->propertiesSize();
    if(pccount > 0){
        tempstr.append(" and the following properties: \n");
        unsigned j;
        for (j=0; j<pccount; j++) {
            tempstr.append("   ").append(this->properties[j]->toString()).append("\n");
        }
    }
    
    const int srcount = this->children.size();
    if (srcount > 0) {
        tempstr.append(" with the following sub-rules: \n");
        unsigned k;
        for (k=0; k<srcount; k++) {
            tempstr.append("    ").append(this->children[k]->toString()).append("\n");
        }
    }
    
    return tempstr;
}

void HSSRule::setSelectorChains(std::vector<HSSSelectorChain::p> newChains)
{
    this->selectorChains = newChains;
    HSSSelectorChain::it it;
    for(it=this->selectorChains.begin(); it!=this->selectorChains.end(); it++){
        (*it)->setParentNode(this->shared_from_this());
    }
}

const std::vector<HSSSelectorChain::p> & HSSRule::getSelectorChains() const
{
    return this->selectorChains;
}

void HSSRule::selectorChainsAdd(HSSSelectorChain::p & newSelectorChain)
{
    if(newSelectorChain)
    {
        std_log3("Added selector chain: " << newSelectorChain->toString());
        newSelectorChain->setParentNode(this->shared_from_this());
        this->selectorChains.push_back(newSelectorChain);
    }
}

void HSSRule::selectorChainsRemove(unsigned int index)
{
    this->selectorChains.erase(this->selectorChains.begin()+index);
}

void HSSRule::selectorChainsRemoveLast()
{
    this->selectorChains.pop_back();
}

HSSSelectorChain::p & HSSRule::selectorChainsGet(unsigned index)
{
    return this->selectorChains[index];
}

HSSSelectorChain::p & HSSRule::selectorChainsLast()
{
    return this->selectorChains.back();
}

const int HSSRule::selectorChainsSize()
{
    return this->selectorChains.size();
}

const std::vector<HSSPropertyDefinition::p> & HSSRule::getProperties() const
{
    return this->properties;
}

void HSSRule::propertiesAdd(HSSPropertyDefinition::p & newProperty)
{
    if(newProperty)
    {
        std_log3("Added property: " << newProperty->toString());
        newProperty->setParentNode(this->shared_from_this());
        this->properties.push_back(newProperty);
    }
}

void HSSRule::propertiesRemove(unsigned int index)
{
    this->properties.erase(this->properties.begin()+index);
}

void HSSRule::propertiesRemoveLast()
{
    this->properties.pop_back();
}

HSSPropertyDefinition::p & HSSRule::propertiesGet(unsigned index)
{
    return this->properties[index];
}

HSSPropertyDefinition::p & HSSRule::propertiesLast()
{
    return this->properties.back();
}

const int HSSRule::propertiesSize()
{
    return this->properties.size();
}


void HSSRule::childrenAdd(HSSRule::p newRule)
{
    newRule->setParentNode(this->shared_from_this()); //parent in the node tree
    this->children.push_back(newRule);
}

HSSRule::p HSSRule::childrenGet(unsigned index)
{
    return this->children[index];
}

void HSSRule::childrenRemove(unsigned index)
{
    this->children.erase(this->children.begin()+index);
}

void HSSRule::childrenRemoveLast()
{
    this->children.pop_back();
}

const int HSSRule::childrenSize()
{
    return this->children.size();
}

void HSSRule::setInstruction(HSSInstruction::p newInstruction)
{
    this->instruction = newInstruction;
    this->instruction->setParentNode(this->shared_from_this());
}

HSSInstruction::p HSSRule::getInstruction()
{
    return this->instruction;
}

HSSRule::p HSSRule::shared_from_this()
{
    return boost::static_pointer_cast<HSSRule>(HSSStatement::shared_from_this());
}


void HSSRule::setThisObj(boost::shared_ptr<HSSDisplayObject> value)
{
    HSSSelectorChain::const_it sIt;
    for (sIt=this->selectorChains.begin(); sIt!=this->selectorChains.end(); sIt++) {
        (*sIt)->setThisObj(value);
    }
    std::vector<HSSPropertyDefinition::p>::iterator it;
    for (it=this->properties.begin(); it!=this->properties.end(); it++) {
        (*it)->setThisObj(value);
    }
//    std::vector<HSSRule::p>::iterator it2;
//    for (it2=this->children.begin(); it2!=this->children.end(); it2++) {
//        (*it2)->setThisObj(value);
//    }
    HSSStatement::setThisObj(value);
}

void HSSRule::treeChanged(HSSObservableProperty source, void*data)
{
    HSSDisplayObject::p thisObj = this->getThisObj();
    AXRController * theController = thisObj->getController();
    if(thisObj->isA(HSSObjectTypeContainer)){
        HSSContainer::p thisContainer = HSSContainer::asContainer(thisObj);
        theController->recursiveMatchRulesToDisplayObjects(this->shared_from_this(), thisContainer->getChildren(), thisContainer, false);
    }
}

bool HSSRule::getActiveByDefault()
{
    return this->_activeByDefault;
}

void HSSRule::setActiveByDefault(bool newValue)
{
    this->_activeByDefault = newValue;
}

const std::vector<boost::weak_ptr<HSSDisplayObject> > HSSRule::getAppliedTo() const
{
    return this->appliedTo;
}

void HSSRule::setAppliedTo(std::vector<boost::weak_ptr<HSSDisplayObject> > newObjects)
{
    this->appliedTo = newObjects;
}

void HSSRule::appliedToAdd(HSSDisplayObject::p displayObject)
{
    this->appliedTo.push_back(boost::weak_ptr<HSSDisplayObject>(displayObject));
}

HSSClonable::p HSSRule::cloneImpl() const
{
    HSSRule::p clone = HSSRule::p(new HSSRule(*this));
    
    HSSSelectorChain::const_it sIt;
    for (sIt=this->selectorChains.begin(); sIt!=this->selectorChains.end(); sIt++) {
        HSSSelectorChain::p clonedSelectorChain = (*sIt)->clone();
        clone->selectorChainsAdd(clonedSelectorChain);
    }
    HSSPropertyDefinition::const_it pIt;
    for (pIt=this->properties.begin(); pIt!=this->properties.end(); pIt++) {
        HSSPropertyDefinition::p clonedPropDef = (*pIt)->clone();
        clone->propertiesAdd(clonedPropDef);
    }
    std::vector<HSSRule::p>::const_iterator rIt;
    for (rIt=this->children.begin(); rIt!=this->children.end(); rIt++) {
        HSSRule::p clonedRule = (*rIt)->clone();
        clone->childrenAdd(clonedRule);
    }
    if(this->instruction) clone->setInstruction(this->instruction->clone());
    
    return clone;
}

const std::vector<boost::shared_ptr<HSSDisplayObject> > HSSRule::getOriginalScope() const
{
    return this->_originalScope;
}
void HSSRule::setOriginalScope(const std::vector<boost::shared_ptr<HSSDisplayObject> > & scope)
{
    this->_originalScope = scope;
}

void HSSRule::setObservedTreeChanger(HSSObservable * newValue)
{
    this->observedTreeChanger = newValue;
}

HSSObservable * HSSRule::getObservedTreeChanger()
{
    return this->observedTreeChanger;
}




