/*
 *  HSSToken.h
 *  mac
 *
 *  Created by Miro Keller on 11/03/11.
 *  Copyright 2011 Miro Keller. All rights reserved.
 *
 */

#ifndef HSSTOKEN_H
#define HSSTOKEN_H

#include <string>
using namespace std;

enum HSSTokenType
{
	HSSNone = 0, //FIXME: is this needed?
	HSSIdentifier,
	HSSNumber,
	HSSPercentageNumber,
    HSSHexNumber,
	HSSDoubleQuoteString,
	HSSSingleQuoteString,
	HSSSymbol,
	HSSWhitespace,
    HSSInstructionSign,
	HSSObjectSign,
	HSSBlockOpen,
	HSSBlockClose,
    HSSColon,
	HSSEndOfStatement,
	HSSParenthesisOpen,
	HSSParenthesisClose,
    HSSLineComment,
    HSSBlockComment
};

class HSSToken
{
public:
    static string tokenStringRepresentation(HSSTokenType type);
    
	HSSTokenType type;
	HSSToken(HSSTokenType type);
    bool isA(HSSTokenType otherType);
	virtual string toString();
};

#endif