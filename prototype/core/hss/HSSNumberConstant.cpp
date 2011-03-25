//
//  HSSNumberConstant.cpp
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSNumberConstant.h"
#include <sstream>


HSSNumberConstant::HSSNumberConstant(long double value)
{
    this->value = value;
}

HSSNumberConstant::~HSSNumberConstant()
{
    
}

HSSParserNode * HSSNumberConstant::evaluate()
{
    return new HSSNumberConstant(this->value);
}

void HSSNumberConstant::setValue(long double newValue)
{
    this->value = newValue;
}

long double HSSNumberConstant::getValue()
{
    return this->value;
}

string HSSNumberConstant::toString()
{
    string ret = "HSSNumberConstant with value ";
    std::ostringstream tempstream;
    //create a temp stream to convert the float to a string
    tempstream << this->value;
    ret.append(tempstream.str());
    return ret;
}