//
//  HSSPropertyDefinition.cpp
//  mac
//
//  Created by Miro Keller on 16/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSPropertyDefinition.h"

HSSPropertyDefinition::HSSPropertyDefinition()
{
    this->name = "";
    this->value = NULL;
}

HSSPropertyDefinition::HSSPropertyDefinition(string name)
{
    this->name = name;
    this->value = NULL;
}

HSSPropertyDefinition::HSSPropertyDefinition(string name, HSSParserNode * value)
{
    this->name = name;
    this->value = value;
}

HSSPropertyDefinition::~HSSPropertyDefinition()
{
    delete this->value;
}

string HSSPropertyDefinition::toString()
{
    string ret = "HSSPropertyDefinition: " + this->name;
    if(this->value != NULL){
        ret.append(" with value: "+this->value->toString());
    }
    return ret;
}

void HSSPropertyDefinition::setName(string name)
{
    this->name = name;
}

string HSSPropertyDefinition::getName()
{
    return this->name;
}

void HSSPropertyDefinition::setValue(HSSParserNode * value)
{
    delete this->value;
    this->value = value;
}

HSSParserNode * HSSPropertyDefinition::getValue()
{
    return this->value;
}