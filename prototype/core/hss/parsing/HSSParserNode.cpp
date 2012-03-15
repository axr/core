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
 *      Revision: 11
 *
 ********************************************************************/

#include "HSSParserNode.h"
#include "../objects/HSSDisplayObject.h"

using namespace AXR;

std::string HSSParserNode::parserNodeStringRepresentation(HSSParserNodeType type){
    std::string types[20];
	types[HSSParserNodeTypeGeneric] = "HSSParserNode";
	types[HSSParserNodeTypeSelector] = "HSSRule";
	types[HSSParserNodeTypeUniversalSelector] = "HSSRule";
	types[HSSParserNodeTypeCombinator] = "HSSPropertyDefinition";
	types[HSSParserNodeTypeFilter] = "HSSObjectDefinition";
	types[HSSParserNodeTypeStatement] = "HSSStatement";
	types[HSSParserNodeTypeExpression] = "HSSExpression";
	types[HSSParserNodeTypeNumberConstant] = "HSSNumberConstant";
	types[HSSParserNodeTypePercentageConstant] = "HSSPercentageConstant";
	types[HSSParserNodeTypeStringConstant] = "HSSStringConstant";
	types[HSSParserNodeTypeKeywordConstant] = "HSSKeywordConstant";
	types[HSSParserNodeTypeObjectDefinition] = "HSSObjectDefinition";
	types[HSSParserNodeTypeObjectNameConstant] = "HSSObjectNameConstant";
	types[HSSParserNodeTypeFunctionCall] = "HSSFunctionCall";
	types[HSSParserNodeTypeMultipleValueDefinition] = "HSSParserNodeTypeMultipleValueDefinition";
    
    return types[type];
}

HSSParserNode::HSSParserNode()
{
    this->nodeType = HSSParserNodeTypeGeneric;
}

//doesn't clone any part of the node tree, nor the observers array
HSSParserNode::HSSParserNode(const HSSParserNode &orig)
{
    this->nodeType = orig.nodeType;
}

HSSParserNode::p HSSParserNode::clone() const
{
    return boost::static_pointer_cast<HSSParserNode, HSSClonable>(this->cloneImpl());
}

std::string HSSParserNode::toString()
{
    return "Generic parser node - you forgot to override toString in your subclass or somehow using HSSParserNode directly";
}

bool HSSParserNode::isA(HSSParserNodeType otherType)
{
	return otherType == this->nodeType;
}

HSSParserNodeType HSSParserNode::getType()
{
    return this->nodeType;
}

HSSParserNode::p HSSParserNode::getParentNode()
{
    if (!this->_parentNode.expired()) {
        HSSParserNode::p parent = this->_parentNode.lock();
        return parent;
    } else {
        return HSSParserNode::p();
    }
}

void HSSParserNode::setParentNode(HSSParserNode::p newParent)
{
    this->_parentNode = pp(newParent);
}

void HSSParserNode::removeFromParentNode()
{
    this->getParentNode()->removeNode(this->shared_from_this());
}

void HSSParserNode::addNode(HSSParserNode::p child)
{
    this->_childNodes.push_back(child);
}

void HSSParserNode::removeNode(HSSParserNode::p child)
{
    HSSParserNode::it it = find(this->_childNodes.begin(), this->_childNodes.end(), child);
    if(it != this->_childNodes.end()){
        this->_childNodes.erase(it);
    }
}

const std::vector<HSSParserNode::p> HSSParserNode::getChildNodes() const
{
    return this->_childNodes;
}

HSSClonable::p HSSParserNode::cloneImpl() const
{
    return HSSClonable::p(new HSSParserNode(*this));
}

void HSSParserNode::setThisObj(HSSDisplayObject::p value)
{
    this->thisObj = value;
}

HSSDisplayObject::p HSSParserNode::getThisObj()
{
    return thisObj;
}




