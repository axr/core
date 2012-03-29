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
 *      Last changed: 2012/03/29
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 5
 *
 ********************************************************************/

#include "HSSValueToken.h"
#include <sstream>

using namespace AXR;

HSSValueToken::HSSValueToken(HSSTokenType type, std::string value, unsigned line, unsigned column)
:HSSToken(type, line, column)
{
    
	this->type = type;
    if(this->isNumeric()){
        this->longValue = strtold(value.c_str(), NULL);
    } else {
        this->stringValue = value;
    }
}

HSSValueToken::HSSValueToken(HSSTokenType type, char value, unsigned line, unsigned column)
:HSSToken(type, line, column)
{
    //std::string tempstr (1, value);
	this->type = type;
	this->stringValue = std::string(1, value);
}

HSSValueToken::HSSValueToken(HSSTokenType type, double long value, unsigned line, unsigned column)
:HSSToken(type, line, column)
{
    this->type = type;
    this->longValue = value;
}

std::string HSSValueToken::getString()
{
    return this->stringValue;
}

double long HSSValueToken::getLong()
{
    return this->longValue;
}

bool HSSValueToken::equals(HSSTokenType otherType, std::string otherValue)
{
	return otherType == this->type && otherValue == this->stringValue;
}

bool HSSValueToken::equals(HSSTokenType otherType, double long otherValue)
{
	return otherType == this->type && otherValue == this->longValue;
}

std::string HSSValueToken::toString()
{
    std::string tokenstr = this->tokenStringRepresentation(this->type);
    if(this->isNumeric()){
        std::ostringstream tempstream;
        tempstream << this->longValue;
        return "HSSValueToken of type: " + tokenstr + " and value: " + tempstream.str();
    } else {
        return "HSSValueToken of type: " + tokenstr + " and value: " + this->stringValue;
    }
	
}

bool HSSValueToken::isNumeric()
{
    return this->type == HSSNumber || this->type == HSSPercentageNumber;
}



