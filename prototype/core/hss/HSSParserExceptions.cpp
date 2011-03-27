//
//  HSSParserExceptions.cpp
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSParserExceptions.h"
#include <sstream>

string HSSUnexpectedEndOfSourceException::toString()
{
    return "HSSParser Error: unexpected end of source";
}


HSSExpectedTokenException::HSSExpectedTokenException(HSSTokenType type)
{
    this->type = type;
    this->value = "";
}

HSSExpectedTokenException::HSSExpectedTokenException(HSSTokenType type, string value)
{
    this->type = type;
    this->value = value;
}

string HSSExpectedTokenException::toString()
{
    if(this->value.length() == 0)
    {
        return "HSSParser Error: Expected token of type "+HSSToken::tokenStringRepresentation(this->type);
    } else {
        return "HSSParser Error: Expected token of type "+HSSToken::tokenStringRepresentation(this->type)+" and value "+value;
    }
}

HSSUnexpectedTokenException::HSSUnexpectedTokenException(HSSTokenType type)
{
    this->type = type;
    this->value = "";
}

HSSUnexpectedTokenException::HSSUnexpectedTokenException(HSSTokenType type, string value)
{
    this->type = type;
    this->value = value;
}

string HSSUnexpectedTokenException::toString()
{
    if(this->value.length() == 0)
    {
        return "HSSParser Error: Unexpected token of type "+HSSToken::tokenStringRepresentation(this->type);
    } else {
        return "HSSParser Error: Unexpected token of type "+HSSToken::tokenStringRepresentation(this->type)+" and value "+this->value;
    }
}

HSSUnexpectedObjectTypeException::HSSUnexpectedObjectTypeException(string type)
{
    this->objectType = type;
}

string HSSUnexpectedObjectTypeException::toString()
{
    return "HSSParser Error: Unexpected object type "+this->objectType;
}

HSSWrongHexLengthException::HSSWrongHexLengthException(int length)
{
    this->length = length;
}

string HSSWrongHexLengthException::toString()
{
    std::ostringstream tempstream;
    tempstream << this->length;
    return "HSSParser Error: The hex number has a wrong length: "+tempstream.str()+". It must be either 1, 2, 3, 4, 6 or 8 digits long.";
}



