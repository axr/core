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
 *      Last changed: 2012/02/23
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 1
 *
 ********************************************************************/

#include "HSSHoverFilter.h"

using namespace AXR;

HSSHoverFilter::HSSHoverFilter()
: HSSFilter()
{
    this->filterType = HSSFilterTypeHover;
}

HSSHoverFilter::p HSSHoverFilter::clone() const{
    return boost::static_pointer_cast<HSSHoverFilter, HSSClonable>(this->cloneImpl());
}

HSSHoverFilter::~HSSHoverFilter()
{
    
}

std::string HSSHoverFilter::toString()
{
    return "Hover Filter";
}


const std::vector<HSSDisplayObject::p> HSSHoverFilter::apply(const std::vector<HSSDisplayObject::p> &scope)
{
    //parent is selector chain, grandparent is the rule
    HSSParserNode::p ruleNode = this->getParentNode()->getParentNode();
    if(ruleNode->isA(HSSParserNodeTypeStatement)){
        HSSStatement::p ruleStatement = boost::static_pointer_cast<HSSStatement>(ruleNode);
        if(ruleStatement->isA(HSSStatementTypeRule)){
            HSSRule::p theRule = boost::static_pointer_cast<HSSRule>(ruleStatement);
            HSSDisplayObject::const_it it;
            for (it=scope.begin(); it!=scope.end(); it++) {
                theRule->connectInteractionFilter(HSSFilterTypeHover, (*it));
            }
            theRule->setActive(false);
        }
    }
    
    //the entire scope will be selected
    return scope;
}

HSSClonable::p HSSHoverFilter::cloneImpl() const{
    return HSSClonable::p(new HSSHoverFilter(*this));
}
