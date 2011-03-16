//
//  HSSValueToken.cpp
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 2011 Miro Keller. All rights reserved.
//

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
    string tempstr (1, value);
	this->type = type;
	this->value = value;
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