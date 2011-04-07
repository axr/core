/**********************************************************
 *
 *      d8888 Y88b   d88P 8888888b.  
 *      d88888  Y88b d88P  888   Y88b 
 *      d88P888   Y88o88P   888    888 
 *      d88P 888    Y888P    888   d88P 
 *      88P  888    d888b    8888888P"  
 *      d88P   888   d88888b   888 T88b   
 *      d8888888888  d88P Y88b  888  T88b  
 *      d88P     888 d88P   Y88b 888   T88b 
 *      
 *      ARBITRARY·······XML········RENDERING
 *      ====================================
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
 **********************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 **********************************************************/

#include "HSSToken.h"
#include <iostream>
#include <string>

using namespace std;

HSSToken::HSSToken(HSSTokenType type)
{
	this->type = type;
}

bool HSSToken::isA(HSSTokenType otherType)
{
	return otherType == this->type;
}

string HSSToken::toString()
{
    string tokenstr = HSSToken::tokenStringRepresentation(this->type);
	return "HSSToken of type: " + tokenstr;
}

string HSSToken::tokenStringRepresentation(HSSTokenType type){
    string types[20];
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