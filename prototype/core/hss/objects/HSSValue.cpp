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

#include "HSSValue.h"
#include <sstream>

using namespace AXR;

HSSValue::HSSValue()
{
    this->type = HSSValueNumberInt;
    this->intValue = 0;
}

HSSValue::HSSValue(long int value)
{
    this->type = HSSValueNumberInt;
    this->intValue = value;
}

HSSValue::HSSValue(long double value)
{
    this->type = HSSValueNumberFloat;
    this->floatValue = value;
}

HSSValue::HSSValue(std::string value)
{
    this->type = HSSValueString;
    this->stringValue = value;
}

//this is used to set a keyword
HSSValue::HSSValue(void * passNULL, std::string keyword)
{
    this->type = HSSValueKeyword;
    this->stringValue = keyword;
}

std::string HSSValue::toString()
{
    std::string ret;
    std::ostringstream tempstream;
    
    if (this->isNamed()) {
        ret = "HSSValue: " + this->name + " with value: ";
    } else {
        ret = "Annonymous HSSValue with value: ";
    }
    
    switch (this->type) {
        case HSSValueNumberInt:
            //create a temp stream to convert the int to a string
            tempstream << this->intValue;
            ret.append(tempstream.str());
            break;
        case HSSValueNumberFloat:
            //create a temp stream to convert the float to a string
            tempstream << this->floatValue;
            ret.append(tempstream.str());
            break;
        case HSSValueString:
        case HSSValueKeyword:
            ret.append(this->stringValue);
            break;
        default:
            throw HSSUnknownValueTypeException();
            break;
    }
    
    return ret;
}

std::string HSSValue::defaultObjectType(std::string property)
{
    if (property == "value"){
        return "value";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

HSSValueType HSSValue::getType()
{
    return this->type;
}

void HSSValue::setValue(long int value)
{
    this->type = HSSValueNumberInt;
    this->intValue = value;
}

void HSSValue::setValue(long double value)
{
    this->type = HSSValueNumberFloat;
    this->floatValue = value;
}

void HSSValue::setValue(std::string value)
{
    this->type = HSSValueString;
    this->stringValue = value;
}

void HSSValue::setKWValue(std::string keyword)
{
    this->type = HSSValueKeyword;
    this->stringValue = keyword;
}

std::string HSSValue::getStringValue()
{
    if (this->type == HSSValueString) {
        return this->stringValue;
    } else if (this->type == HSSValueNumberInt || this->type == HSSValueNumberFloat){
        std::ostringstream tempstream;
        tempstream << (this->type == HSSValueNumberInt ? this->intValue : this->floatValue);
        return tempstream.str();
    } else {
        throw HSSUnknownValueTypeException();
    }
}

long int HSSValue::getIntValue()
{
    if(this->type == HSSValueNumberInt){
        return this->intValue;
    } else {
        //FIXME: parse string to int
        throw HSSUnknownValueTypeException();
    }
}

long double HSSValue::getFloatValue()
{
    if(this->type == HSSValueNumberFloat){
        return this->floatValue;
    } else {
        //FIXME: parse string to float
        throw HSSUnknownValueTypeException();
    }
}



