//
//  HSSKeywordConstant.cpp
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSKeywordConstant.h"

HSSKeywordConstant::HSSKeywordConstant(string value)
{
    this->value = value;
}

HSSKeywordConstant::~HSSKeywordConstant()
{
    
}

HSSParserNode * HSSKeywordConstant::evaluate()
{
    return this;
}

void HSSKeywordConstant::setValue(string newValue)
{
    this->value = newValue;
}

string HSSKeywordConstant::getValue()
{
    return this->value;
}

string HSSKeywordConstant::toString()
{
    return "HSSKeywordConstant with value "+this->value;
}