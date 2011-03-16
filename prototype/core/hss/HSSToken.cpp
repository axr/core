/*
 *  HSSToken.cpp
 *  mac
 *
 *  Created by Miro Keller on 11/03/11.
 *  Copyright 2011 Miro Keller. All rights reserved.
 *
 */

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