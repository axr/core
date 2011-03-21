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
    string tempstr = string("HSSObjectDefinition with object of type:");
    tempstr.append(this->prototype->toString());
    
    const int pccount = this->propertiesSize();
    if(pccount > 0){
        tempstr.append(" and the following properties: \n");
        unsigned j;
        for (j=0; j<pccount; j++) {
            tempstr.append("   ").append(this->properties[j]->toString()).append("\n");
        }
    }
    
    return tempstr;
}

void HSSObjectDefinition::propertiesAdd(HSSStatement * newStatement)
{
    if(newStatement != NULL)
    {
        std_log3("Added node of type " << newStatement->toString());
        this->properties.push_back(newStatement);
    }
}

void HSSObjectDefinition::propertiesRemoveLast()
{
    delete this->properties.back();
    this->properties.pop_back();
}

HSSStatement * HSSObjectDefinition::propertiesLast()
{
    return this->properties.back();
}

const int HSSObjectDefinition::propertiesSize()
{
    return this->properties.size();
}