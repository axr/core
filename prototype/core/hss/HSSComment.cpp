//
//  HSSComment.cpp
//  mac
//
//  Created by Miro Keller on 28/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSComment.h"

HSSComment::HSSComment(string value)
{
    this->value = value;
}

string HSSComment::getValue()
{
    return this->value;
}

void HSSComment::setValue(string newValue)
{
    this->value = newValue;
}

string HSSComment::toString()
{
    return "HSSComment with value: "+this->value;
}