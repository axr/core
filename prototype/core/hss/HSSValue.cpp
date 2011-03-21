//
//  HSSValue.cpp
//  mac
//
//  Created by Miro Keller on 20/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSValue.h"
#include <sstream>

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





