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
 *      Last changed: 2012/03/15
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 7
 *
 ********************************************************************/

#include "HSSRule.h"
#include <iostream>
#include "../../axr/AXRDebugging.h"

using namespace AXR;

HSSRule::HSSRule()
: HSSStatement()
{
    this->selectorChain = selectorChain;
    this->type = HSSStatementTypeRule;
}

HSSRule::HSSRule(const HSSRule & orig)
: HSSStatement(orig)
{
    this->_isActive = orig._isActive;
    this->_interactors = boost::unordered_map<HSSFilterType, std::vector<boost::shared_ptr<HSSDisplayObject> > >(orig._interactors);
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
}

std::string HSSRule::toString()
{
    std::string tempstr = std::string("HSSRule with the following selector chain: \n");
    if (this->selectorChain != NULL){
        tempstr.append("   ").append(this->selectorChain->toString());
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

void HSSRule::setSelectorChain(HSSSelectorChain::p newChain)
{
    this->selectorChain = newChain;
}

HSSSelectorChain::p HSSRule::getSelectorChain()
{
    return this->selectorChain;
}

void HSSRule::propertiesAdd(HSSPropertyDefinition::p & newProperty)
{
    if(newProperty)
    {
        std_log3("Added property: " << newProperty->toString());
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
}

HSSInstruction::p HSSRule::getInstruction()
{
    return this->instruction;
}

HSSRule::p HSSRule::shared_from_this()
{
    return boost::static_pointer_cast<HSSRule>(HSSStatement::shared_from_this());
}

HSSClonable::p HSSRule::cloneImpl() const
{
    HSSRule::p clone = HSSRule::p(new HSSRule(*this));
    
    if(this->selectorChain) clone->setSelectorChain(this->selectorChain->clone());
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
