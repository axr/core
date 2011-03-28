//
//  HSSParserExceptions.cpp
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSParserExceptions.h"
#include <sstream>

HSSParserException::HSSParserException(string filename, int line, int column)
{
    this->filename = filename;
    this->line = line;
    this->column = column;
}

string HSSParserException::toString()
{
    std::ostringstream linnum;
    linnum << this->line;
    std::ostringstream colnum;
    colnum << this->column;
    return "On file: "+this->filename + "  Line: "+linnum.str() + "  Column: "+colnum.str();
}

HSSUnexpectedEndOfSourceException::HSSUnexpectedEndOfSourceException(string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    
}

string HSSUnexpectedEndOfSourceException::toString()
{
    return "HSSParser Error: unexpected end of source.\n"+HSSParserException::toString();
}


HSSExpectedTokenException::HSSExpectedTokenException(HSSTokenType type, string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
    this->value = "";
}

HSSExpectedTokenException::HSSExpectedTokenException(HSSTokenType type, string value, string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
    this->value = value;
}

string HSSExpectedTokenException::toString()
{
    if(this->value.length() == 0)
    {
        return "HSSParser Error: Expected token of type "+HSSToken::tokenStringRepresentation(this->type)+"\n"+HSSParserException::toString();
    } else {
        return "HSSParser Error: Expected token of type "+HSSToken::tokenStringRepresentation(this->type)+" and value \n"+value+HSSParserException::toString();
    }
}

HSSUnexpectedTokenException::HSSUnexpectedTokenException(HSSTokenType type, string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
    this->value = "";
}

HSSUnexpectedTokenException::HSSUnexpectedTokenException(HSSTokenType type, string value, string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
    this->value = value;
}

string HSSUnexpectedTokenException::toString()
{
    if(this->value.length() == 0)
    {
        return "HSSParser Error: Unexpected token of type "+HSSToken::tokenStringRepresentation(this->type)+"\n"+HSSParserException::toString();
    } else {
        return "HSSParser Error: Unexpected token of type "+HSSToken::tokenStringRepresentation(this->type)+" and value \n"+this->value+HSSParserException::toString();
    }
}

HSSUnexpectedObjectTypeException::HSSUnexpectedObjectTypeException(string type, string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
}

string HSSUnexpectedObjectTypeException::toString()
{
    return "HSSParser Error: Unexpected object type \n"+this->type+HSSParserException::toString();
}

HSSWrongHexLengthException::HSSWrongHexLengthException(int length, string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->length = length;
}

string HSSWrongHexLengthException::toString()
{
    std::ostringstream tempstream;
    tempstream << this->length;
    return "HSSParser Error: The hex number has a wrong length: "+tempstream.str()+". It must be either 1, 2, 3, 4, 6 or 8 digits long.\n"+HSSParserException::toString();
}



