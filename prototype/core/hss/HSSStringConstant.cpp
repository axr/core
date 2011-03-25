//
//  HSSStringConstant.cpp
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSStringConstant.h"

HSSStringConstant::HSSStringConstant(string value)
{
    this->value = value;
}

HSSStringConstant::~HSSStringConstant()
{
    
}

HSSParserNode * HSSStringConstant::evaluate()
{
    return this;
}

void HSSStringConstant::setValue(string newValue)
{
    this->value = newValue;
}

string HSSStringConstant::getValue()
{
    return this->value;
}

string HSSStringConstant::toString()
{
    return "HSSStringConstant with value "+this->value;
}