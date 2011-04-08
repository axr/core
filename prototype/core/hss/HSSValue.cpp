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

#include "HSSValue.h"
#include <sstream>

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

HSSValue::HSSValue(string value)
{
    this->type = HSSValueString;
    this->stringValue = value;
}

//this is used to set a keyword
HSSValue::HSSValue(void * passNULL, string keyword)
{
    this->type = HSSValueKeyword;
    this->stringValue = keyword;
}

string HSSValue::toString()
{
    string ret;
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

string HSSValue::defaultObjectType(string property)
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

void HSSValue::setValue(string value)
{
    this->type = HSSValueString;
    this->stringValue = value;
}

void HSSValue::setKWValue(string keyword)
{
    this->type = HSSValueKeyword;
    this->stringValue = keyword;
}

string HSSValue::getStringValue()
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



