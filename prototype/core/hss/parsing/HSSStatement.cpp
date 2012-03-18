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
 *      Revision: 3
 *
 ********************************************************************/

#include "HSSStatement.h"

using namespace AXR;

HSSStatement::HSSStatement()
: HSSParserNode()
{
    this->nodeType = HSSParserNodeTypeStatement;
    this->type = HSSStatementTypeGeneric;
}

HSSStatement::HSSStatement(const HSSStatement &orig)
: HSSParserNode(orig)
{
    this->type = orig.type;
}

bool HSSStatement::isA(HSSStatementType otherType)
{
	return otherType == this->type;
}

HSSStatementType HSSStatement::getType()
{
    return this->type;
}

std::string HSSStatement::statementStringRepresentation(HSSStatementType type){
    std::string types[20];
	types[HSSStatementTypeGeneric] = "HSSGenericStatement";
	types[HSSStatementTypeRule] = "HSSRule";
	types[HSSStatementTypePropertyDefinition] = "HSSPropertyDefinition";
	types[HSSStatementTypeObjectDefinition] = "HSSObjectDefinition";
	types[HSSStatementTypeComment] = "HSSComment";
	types[HSSStatementTypeInstruction] = "HSSInstruction";

    return types[type];
}

HSSStatement::p HSSStatement::shared_from_this()
{
    return boost::static_pointer_cast<HSSStatement>(HSSParserNode::shared_from_this());
}

