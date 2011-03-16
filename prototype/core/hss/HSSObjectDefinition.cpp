//
//  HSSRule.cpp
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSObjectDefinition.h"
#include <string>
#include <iostream>
#include "AXR.h"

//this acquires ownership of the pointer
HSSObjectDefinition::HSSObjectDefinition(HSSObject * prototype)
{
    this->prototype = prototype;
}

HSSObjectDefinition::~HSSObjectDefinition()
{
    delete this->prototype;
}

string HSSObjectDefinition::toString()
{
    string tempstr = "HSSObjectDefinition with object of type ";
    tempstr.append(this->prototype->toString());
    return tempstr;
}