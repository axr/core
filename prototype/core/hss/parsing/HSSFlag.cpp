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
 *      Last changed: 2012/03/21
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 1
 *
 ********************************************************************/

#include "HSSFlag.h"
#include "../../AXR.h"
#include <boost/unordered_map.hpp>
#include "../objects/HSSDisplayObject.h"
#include "../../axr/AXRController.h"

using namespace AXR;

HSSFlag::HSSFlag()
: HSSParserNode()
{
    this->nodeType = HSSParserNodeTypeFlag;
    this->_purging = false;
}

HSSFlag::p HSSFlag::clone() const{
    return boost::static_pointer_cast<HSSFlag, HSSClonable>(this->cloneImpl());
}

HSSFlag::~HSSFlag()
{
    
}

std::string HSSFlag::toString()
{
    return "Flag with name "+this->_name;
}

std::string HSSFlag::getName()
{
    return this->_name;
}

void HSSFlag::setName(std::string newValue)
{
    this->_name = newValue;
}

void HSSFlag::apply(HSSDisplayObject::p displayObject, bool negating)
{
    //parent is selector chain, grandparent is the rule
    HSSParserNode::p ruleNode = this->getParentNode()->getParentNode();
    if(ruleNode->isA(HSSParserNodeTypeStatement)){
        HSSStatement::p ruleStatement = boost::static_pointer_cast<HSSStatement>(ruleNode);
        if(ruleStatement->isA(HSSStatementTypeRule)){
            HSSRule::p theRule = boost::static_pointer_cast<HSSRule>(ruleStatement);
            
            theRule->setActiveByDefault(negating);
            displayObject->createFlag(this->shared_from_this(), (negating ? HSSRuleStateOn : HSSRuleStateOff));
        }
    }
}

void HSSFlag::flagChanged(HSSRuleState newStatus){
    //parent is selector chain, grandparent is the rule
    HSSParserNode::p selectorChainNode = this->getParentNode();
    HSSParserNode::p ruleNode = selectorChainNode->getParentNode();
    if(ruleNode->isA(HSSParserNodeTypeStatement)){
        HSSStatement::p ruleStatement = boost::static_pointer_cast<HSSStatement>(ruleNode);
        if(ruleStatement->isA(HSSStatementTypeRule)){
            HSSRule::p theRule = boost::static_pointer_cast<HSSRule>(ruleStatement);
            std::vector<HSSDisplayObject::p> scope = theRule->getOriginalScope();
            AXRController::p controller = AXRCore::getInstance()->getController();
            controller->setSelectorChain(theRule->getSelectorChain());
            this->setPurging(true);
            std::vector<std::vector<HSSDisplayObject::p> > selection = controller->selectHierarchical(scope, this->getThisObj(), false, false);
            
            std::vector<std::vector<HSSDisplayObject::p> >::const_iterator outer;
            std::vector<HSSDisplayObject::p>::const_iterator inner;
            for (outer=selection.begin(); outer!=selection.end(); outer++) {
                const std::vector<HSSDisplayObject::p> & innerv = *outer;
                for (inner=innerv.begin(); inner!=innerv.end(); inner++) {
                    (*inner)->setRuleStatus(theRule, newStatus);
                }
            }
        }
    }
}

const std::vector<HSSDisplayObject::p> HSSFlag::filter(const std::vector<HSSDisplayObject::p> &scope, bool negating)
{
    if(scope.size() > 0){
        std::vector<HSSDisplayObject::p> ret;
        HSSDisplayObject::const_it it;
        for (it=scope.begin(); it!=scope.end(); it++) {
            const HSSDisplayObject::p & theDO = *it;
            if(this->isPurging()){
                HSSRuleState state = theDO->flagState(this->getName());
                switch (state) {
                    case HSSRuleStatePurge:
                    case HSSRuleStateActivate:
                        ret.push_back(theDO);
                        break;
                        
                    default:
                        break;
                }
                
            } else {
                bool firstMatch = (theDO->flagState(this->getName()) == HSSRuleStateOn) && !negating;
                bool secondMatch = (theDO->flagState(this->getName()) == HSSRuleStateOff) && negating;
                if(firstMatch || secondMatch){
                    ret.push_back(theDO);
                }
            }
        }
        this->setPurging(false);
        return ret;
    } else {
        this->setPurging(false);
        return scope;
    }
}

bool HSSFlag::isPurging()
{
    return this->_purging;
}

void HSSFlag::setPurging(bool newValue)
{
    this->_purging = newValue;
}

HSSClonable::p HSSFlag::cloneImpl() const{
    return HSSClonable::p(new HSSFlag(*this));
}

HSSFlag::p HSSFlag::shared_from_this()
{
    return boost::static_pointer_cast<HSSFlag>(HSSParserNode::shared_from_this());
}
