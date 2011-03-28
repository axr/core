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
    HSSParserException(string filename, int line, int column);
    
    string filename;
    int line;
    int column;
    virtual string toString();
};


class HSSUnexpectedEndOfSourceException : public HSSParserException
{
public:
    HSSUnexpectedEndOfSourceException(string filename, int line, int column);
    virtual string toString();
};

class HSSExpectedTokenException : public HSSParserException
{
public:
    HSSExpectedTokenException(HSSTokenType type, string filename, int line, int column);
    HSSExpectedTokenException(HSSTokenType type, string value, string filename, int line, int column);
    virtual string toString();
    HSSTokenType type;
    string value;
};

class HSSUnexpectedTokenException : public HSSParserException
{
public:
    HSSUnexpectedTokenException(HSSTokenType type, string filename, int line, int column);
    HSSUnexpectedTokenException(HSSTokenType type, string value, string filename, int line, int column);
    virtual string toString();
    HSSTokenType type;
    string value;
};

class HSSUnexpectedObjectTypeException : public HSSParserException
{
public:
    HSSUnexpectedObjectTypeException(string type, string filename, int line, int column);
    virtual string toString();
    string type;
};

class HSSWrongHexLengthException : public HSSParserException
{
public:
    HSSWrongHexLengthException(int length, string filename, int line, int column);
    virtual string toString();
    int length;
};

#endif