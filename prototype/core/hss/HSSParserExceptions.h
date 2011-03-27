//
//  HSSParserExceptions.h
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//
#ifndef HSSPARSEREXCEPTIONS_H
#define HSSPARSEREXCEPTIONS_H


#include <exception>
#include <string>
#include "HSSToken.h"

using namespace std;

class HSSParserException
{
public:
    HSSTokenType type;
    string filename;
    string value;
    int line;
    int column;
    virtual string toString() =0;
};


class HSSUnexpectedEndOfSourceException : public HSSParserException
{
public:
    virtual string toString();
};

class HSSExpectedTokenException : public HSSParserException
{
public:
    HSSExpectedTokenException(HSSTokenType type);
    HSSExpectedTokenException(HSSTokenType type, string value);
    virtual string toString();
};

class HSSUnexpectedTokenException : public HSSParserException
{
public:
    HSSUnexpectedTokenException(HSSTokenType type);
    HSSUnexpectedTokenException(HSSTokenType type, string value);
    virtual string toString();
};

class HSSUnexpectedObjectTypeException : public HSSParserException
{
public:
    HSSUnexpectedObjectTypeException(string objectType);
    virtual string toString();
    string objectType;
};

class HSSWrongHexLengthException : public HSSParserException
{
public:
    HSSWrongHexLengthException(int length);
    virtual string toString();
    int length;
};

#endif