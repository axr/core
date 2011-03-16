//
//  HSSPropertyDefinition.cpp
//  mac
//
//  Created by Miro Keller on 16/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSPropertyDefinition.h"

HSSPropertyDefinition::HSSPropertyDefinition(string name)
{
    this->name = name;
}

HSSPropertyDefinition::~HSSPropertyDefinition()
{
    //do nothing
}

string HSSPropertyDefinition::toString()
{
    return string("HSSPropertyDefinition: ").append(this->name);
}