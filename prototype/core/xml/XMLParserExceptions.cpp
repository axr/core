//
//  XMLParserExceptions.cpp
//  mac
//
//  Created by Miro Keller on 02/04/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "XMLParserExceptions.h"

#include <sstream>

using namespace AXR;

XMLParserException::XMLParserException(std::string filename, int line, int column)
{
    this->filename = filename;
    this->line = line;
    this->column = column;
}

std::string XMLParserException::toString()
{
    std::ostringstream linnum;
    linnum << this->line;
    std::ostringstream colnum;
    colnum << this->column;
    return "On file: "+this->filename + "  Line: "+linnum.str() + "  Column: "+colnum.str();
}

XMLUnexpectedEndOfSourceException::XMLUnexpectedEndOfSourceException(std::string filename, int line, int column)
: XMLParserException(filename, line, column)
{
    
}

std::string XMLUnexpectedEndOfSourceException::toString()
{
    return "XMLParser Error: unexpected end of source.\n"+XMLParserException::toString();
}

XMLNoElementsException::XMLNoElementsException(std::string filename, int line, int column)
: XMLParserException(filename, line, column)
{
    
}

std::string XMLNoElementsException::toString()
{
    return "XMLParser Error: the document contained no elements.\n"+XMLParserException::toString();
}