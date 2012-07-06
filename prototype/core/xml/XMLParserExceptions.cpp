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
 ********************************************************************/

#include "XMLParserExceptions.h"

#include <sstream>

using namespace AXR;

XMLParserException::XMLParserException(std::string filename)
{
    this->filename = filename;
    
    this->line = 0;
    this->column = 0;
    this->_tracks_file_position = false;
}

XMLParserException::XMLParserException(std::string filename, int line, int column)
{
    this->filename = filename;
    this->line = line;
    this->column = column;
    this->_tracks_file_position = true;
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


XMLUnknownSheetTypeException::XMLUnknownSheetTypeException(std::string filename, int line, int column)
: XMLParserException(filename, line, column)
{
    
}

std::string XMLUnknownSheetTypeException::toString()
{
    return "XMLParser Error: unknown stylesheet type.\n"+XMLParserException::toString();
}


XMLUnknownCallbackTypeException::XMLUnknownCallbackTypeException()
{
    
}

std::string XMLUnknownCallbackTypeException::toString()
{
    return "XMLParser Error: unknown callback type.";
}


XMLControllerNotSetException::XMLControllerNotSetException(std::string filename)
: XMLParserException(filename)
{
    
}

std::string XMLControllerNotSetException::toString()
{
    return "XMLParser Error: the controller was not set.\n"+XMLParserException::toString();
}




