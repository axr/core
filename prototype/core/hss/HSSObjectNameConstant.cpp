//
//  HSSObjectNameConstant.cpp
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSObjectNameConstant.h"

HSSObjectNameConstant::HSSObjectNameConstant(string value)
{
    this->value = value;
}

HSSObjectNameConstant::~HSSObjectNameConstant()
{
    
}

HSSParserNode * HSSObjectNameConstant::evaluate()
{
    return this;
}

void HSSObjectNameConstant::setValue(string newValue)
{
    this->value = newValue;
}

string HSSObjectNameConstant::getValue()
{
    return this->value;
}

string HSSObjectNameConstant::toString()
{
    return "HSSObjectNameConstant with value "+this->value;
}