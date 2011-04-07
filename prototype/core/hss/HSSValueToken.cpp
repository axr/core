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

#include "HSSValueToken.h"

HSSValueToken::HSSValueToken(HSSTokenType type, string value)
:HSSToken(type)
{
    
	this->type = type;
	this->value = value;
}

HSSValueToken::HSSValueToken(HSSTokenType type, char value)
:HSSToken(type)
{
    //string tempstr (1, value);
	this->type = type;
	this->value = string(1, value);
}

bool HSSValueToken::equals(HSSTokenType otherType, string otherValue)
{
	return otherType == this->type && otherValue == this->value;
}

string HSSValueToken::toString()
{
    string tokenstr = this->tokenStringRepresentation(this->type);
	return "HSSValueToken of type: " + tokenstr + " and value: " + this->value;
}