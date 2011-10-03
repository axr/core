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
 *      Last changed: 2011/09/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 7
 *
 ********************************************************************/

#include "HSSParserNode.h"

using namespace AXR;

HSSParserNode::HSSParserNode()
{
    this->nodeType = HSSParserNodeTypeGeneric;
}

std::string HSSParserNode::toString()
{
    return "Generic parser node";
}

bool HSSParserNode::isA(HSSParserNodeType otherType)
{
	return otherType == this->nodeType;
}

HSSParserNodeType HSSParserNode::getType()
{
    return this->nodeType;
}

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
    
    return types[type];
}






