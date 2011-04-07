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

XMLUnknownProcessingInstructionException::XMLUnknownProcessingInstructionException(std::string filename, int line, int column)
: XMLParserException(filename, line, column)
{
    
}

std::string XMLUnknownProcessingInstructionException::toString()
{
    return "XMLParser Error: unknown processing instruction.\n"+XMLParserException::toString();
}

XMLMalformedProcessingInstructionException::XMLMalformedProcessingInstructionException(std::string filename, int line, int column)
: XMLParserException(filename, line, column)
{
    
}

std::string XMLMalformedProcessingInstructionException::toString()
{
    return "XMLParser Error: the processing instruction was malformed.\n"+XMLParserException::toString();
}







