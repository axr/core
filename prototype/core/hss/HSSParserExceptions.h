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

class HSSUnexpectedEndOfSourceException
{
public:
    virtual string toString();
};

class HSSExpectedTokenException
{
public:
    HSSExpectedTokenException(HSSTokenType type);
    HSSExpectedTokenException(HSSTokenType type, string value);
    virtual string toString();
private:
    HSSTokenType type;
    string value;
};

class HSSUnexpectedTokenException
{
public:
    HSSUnexpectedTokenException(HSSTokenType type);
    HSSUnexpectedTokenException(HSSTokenType type, string value);
    virtual string toString();
private:
    HSSTokenType type;
    string value;
};

class HSSUnexpectedObjectTypeException
{
public:
    HSSUnexpectedObjectTypeException(string type);
    virtual string toString();
private:
    string type;
};

#endif