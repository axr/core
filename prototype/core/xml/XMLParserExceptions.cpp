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







