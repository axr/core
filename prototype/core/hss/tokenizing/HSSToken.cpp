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
 *      Last changed: 2011/04/16
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 3
 *
 ********************************************************************/


#include "HSSToken.h"
#include <iostream>

using namespace AXR;

HSSToken::HSSToken(HSSTokenType type)
{
	this->type = type;
}

bool HSSToken::isA(HSSTokenType otherType)
{
	return otherType == this->type;
}

HSSTokenType HSSToken::getType()
{
    return this->type;
}

std::string HSSToken::toString()
{
    std::string tokenstr = HSSToken::tokenStringRepresentation(this->type);
	return "HSSToken of type: " + tokenstr;
}

std::string HSSToken::tokenStringRepresentation(HSSTokenType type){
    std::string types[20];
	types[HSSNone] = "HSSNone";
	types[HSSIdentifier] = "HSSIdentifier";
	types[HSSNumber] = "HSSNumber";
	types[HSSPercentageNumber] = "HSSPercentageNumber";
    types[HSSHexNumber] = "HSSHexNumber";
	types[HSSDoubleQuoteString] = "HSSDoubleQuoteString";
	types[HSSSingleQuoteString] = "HSSSingleQuoteString";
	types[HSSSymbol] = "HSSSymbol";
	types[HSSWhitespace] = "HSSWhitespace";
	types[HSSInstructionSign] = "HSSInstructionSign";
	types[HSSObjectSign] = "HSSObjectSign";
	types[HSSBlockOpen] = "HSSBlockOpen";
	types[HSSBlockClose] = "HSSBlockClose";
	types[HSSColon] = "HSSColon";
	types[HSSEndOfStatement] = "HSSEndOfStatement";
	types[HSSParenthesisOpen] = "HSSParenthesisOpen";
	types[HSSParenthesisClose] = "HSSParenthesisClose";
    types[HSSLineComment] = "HSSLineComment";
    types[HSSBlockComment] = "HSSBlockComment";
    return types[type];
}