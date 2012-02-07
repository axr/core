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
 *      Last changed: 2012/01/28
 *      HSS version: 1.0
 *      Core version: 0.44
 *      Revision: 5
 *
 ********************************************************************/

#include "HSSValue.h"
#include <sstream>
#include "../../axr/errors/errors.h"

using namespace AXR;

HSSValue::HSSValue()
{
    this->valueType = HSSValueNumberInt;
    this->type = HSSObjectTypeValue;
    this->intValue = 0;
}

HSSValue::HSSValue(long int value)
{
    this->valueType = HSSValueNumberInt;
    this->type = HSSObjectTypeValue;
    this->intValue = value;
}

HSSValue::HSSValue(long double value)
{
    this->valueType = HSSValueNumberFloat;
    this->type = HSSObjectTypeValue;
    this->floatValue = value;
}

HSSValue::HSSValue(std::string value)
{
    this->valueType = HSSValueString;
    this->type = HSSObjectTypeValue;
    this->stringValue = value;
}

//this is used to set a keyword
HSSValue::HSSValue(void * passNULL, std::string keyword)
{
    this->valueType = HSSValueKeyword;
    this->type = HSSObjectTypeValue;
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
    
    switch (this->valueType) {
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
            ret.append("### Unknown value type ###");
            break;
    }
    
    return ret;
}

std::string HSSValue::defaultObjectType()
{
    return "value";
}

std::string HSSValue::defaultObjectType(std::string property)
{
    if (property == "value"){
        return "value";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

HSSValueType HSSValue::getValueType()
{
    return this->valueType;
}

void HSSValue::setValue(long int value)
{
    this->valueType = HSSValueNumberInt;
    this->intValue = value;
}

void HSSValue::setValue(long double value)
{
    this->valueType = HSSValueNumberFloat;
    this->floatValue = value;
}

void HSSValue::setValue(std::string value)
{
    this->valueType = HSSValueString;
    this->stringValue = value;
}

void HSSValue::setKWValue(std::string keyword)
{
    this->valueType = HSSValueKeyword;
    this->stringValue = keyword;
}

std::string HSSValue::getStringValue()
{
    if (this->valueType == HSSValueString || this->valueType == HSSValueKeyword) {
        return this->stringValue;
    } else if (this->valueType == HSSValueNumberInt || this->valueType == HSSValueNumberFloat){
        std::ostringstream tempstream;
        tempstream << (this->valueType == HSSValueNumberInt ? this->intValue : this->floatValue);
        return tempstream.str();
    } else {
        throw AXRError::p(new AXRError("HSSValue", "Unknown value type"));
    }
}

long int HSSValue::getIntValue()
{
    if(this->valueType == HSSValueNumberInt){
        return this->intValue;
    } else {
        //FIXME: parse string to int
        throw AXRError::p(new AXRError("HSSValue", "Unknown value type"));
    }
}

long double HSSValue::getFloatValue()
{
    if(this->valueType == HSSValueNumberFloat){
        return this->floatValue;
    } else {
        //FIXME: parse string to float
        throw AXRError::p(new AXRError("HSSValue", "Unknown value type"));
    }
}



