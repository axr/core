/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MMMMMD                                                         
 *            :: VD
 *           ::  .                                                         
 *          ::                                                              
 *          M   NZ      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       .MMZ. MMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      AMMMMMDJMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      MM  MMMMMC         ___MMMM     VMMMMMMM6       MMMM                   
 *      MM  MMMMMMM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *     :MM AMMMMMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *    :: MMMMMMMMMM    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *   .     MMMMM~     MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         MMMMM      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        MMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       MMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      MMMM MMMM                                                           
 *     AMMM  .MMM                                         
 *     MMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     MMM    MMA       ====================================                              
 *     DMN    MM                               
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
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

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



