//
//  HSSRule.cpp
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSRule.h"
#include <string>
#include <iostream>
#include "AXR.h"

HSSRule::HSSRule(HSSSelectorChain * selectorChain)
{
    this->selectorChain = selectorChain;
}

HSSRule::~HSSRule()
{
    delete this->selectorChain;
    unsigned i;
    for(i=0; i<this->properties.size(); i++){
        this->propertiesRemoveLast();
    }
}

string HSSRule::toString()
{
    string tempstr = string("HSSRule with the following selector chain: \n");
    if (this->selectorChain != NULL){
        tempstr.append("   ").append(this->selectorChain->toString());
    } else {
        tempstr.append("WARNING: this rule has no selector chain!\n");
    }
    
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

void HSSRule::propertiesAdd(HSSStatement * newStatement)
{
    if(newStatement != NULL)
    {
        std_log3("Added node of type " << newStatement->toString());
        this->properties.push_back(newStatement);
    }
}

void HSSRule::propertiesRemoveLast()
{
    delete this->properties.back();
    this->properties.pop_back();
}

HSSStatement * HSSRule::propertiesLast()
{
    return this->properties.back();
}

const int HSSRule::propertiesSize()
{
    return this->properties.size();
}