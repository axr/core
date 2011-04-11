/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MA:MMD                                                         
 *            :: VD
 *           ::  º                                                         
 *          ::                                                              
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM                   
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      TMMM MMMM                                                           
 *     TMMM  .MMM                                         
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     TMM    MMA       ====================================                              
 *     TMN    MM                               
 *      MN    ZM                       
 *            MM,
 *
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
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/09
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 2
 *
 ********************************************************************/

#include "HSSParserExceptions.h"
#include <sstream>

using namespace AXR;

HSSParserException::HSSParserException(std::string filename, int line, int column)
{
    this->filename = filename;
    this->line = line;
    this->column = column;
}

std::string HSSParserException::toString()
{
    std::ostringstream linnum;
    linnum << this->line;
    std::ostringstream colnum;
    colnum << this->column;
    return "On file: "+this->filename + "  Line: "+linnum.str() + "  Column: "+colnum.str();
}

HSSUnexpectedEndOfSourceException::HSSUnexpectedEndOfSourceException(std::string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    
}

std::string HSSUnexpectedEndOfSourceException::toString()
{
    return "HSSParser Error: unexpected end of source.\n"+HSSParserException::toString();
}


HSSExpectedTokenException::HSSExpectedTokenException(HSSTokenType type, std::string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
    this->value = "";
}

HSSExpectedTokenException::HSSExpectedTokenException(HSSTokenType type, std::string value, std::string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
    this->value = value;
}

std::string HSSExpectedTokenException::toString()
{
    if(this->value.length() == 0)
    {
        return "HSSParser Error: Expected token of type "+HSSToken::tokenStringRepresentation(this->type)+"\n"+HSSParserException::toString();
    } else {
        return "HSSParser Error: Expected token of type "+HSSToken::tokenStringRepresentation(this->type)+" and value \n"+value+HSSParserException::toString();
    }
}

HSSUnexpectedTokenException::HSSUnexpectedTokenException(HSSTokenType type, std::string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
    this->value = "";
}

HSSUnexpectedTokenException::HSSUnexpectedTokenException(HSSTokenType type, std::string value, std::string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
    this->value = value;
}

std::string HSSUnexpectedTokenException::toString()
{
    if(this->value.length() == 0)
    {
        return "HSSParser Error: Unexpected token of type "+HSSToken::tokenStringRepresentation(this->type)+"\n"+HSSParserException::toString();
    } else {
        return "HSSParser Error: Unexpected token of type "+HSSToken::tokenStringRepresentation(this->type)+" and value \n"+this->value+HSSParserException::toString();
    }
}

HSSUnexpectedObjectTypeException::HSSUnexpectedObjectTypeException(std::string type, std::string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->type = type;
}

std::string HSSUnexpectedObjectTypeException::toString()
{
    return "HSSParser Error: Unexpected object type \n"+this->type+HSSParserException::toString();
}

HSSWrongHexLengthException::HSSWrongHexLengthException(int length, std::string filename, int line, int column)
: HSSParserException(filename, line, column)
{
    this->length = length;
}

std::string HSSWrongHexLengthException::toString()
{
    std::ostringstream tempstream;
    tempstream << this->length;
    return "HSSParser Error: The hex number has a wrong length: "+tempstream.str()+". It must be either 1, 2, 3, 4, 6 or 8 digits long.\n"+HSSParserException::toString();
}



