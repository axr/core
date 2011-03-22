//
//  HSSMultipleValue.cpp
//  mac
//
//  Created by Miro Keller on 22/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSMultipleValue.h"
#include "AXR.h"
#include <iostream>

HSSMultipleValue::HSSMultipleValue()
{
    this->valueList = vector<HSSValue*>();
}

HSSMultipleValue::~HSSMultipleValue()
{
    unsigned i;
    for(i=0; i<this->valueList.size(); i++){
        this->removeLast();
    }
}

string HSSMultipleValue::toString()
{
    string tempstr = string("HSSMultipleValue");
    
    const int vcount = this->size();
    if(vcount > 0){
        tempstr.append(" with the following values: \n");
        unsigned j;
        for (j=0; j<vcount; j++) {
            tempstr.append("   ").append(this->valueList[j]->toString()).append("\n");
        }
    }
    
    return tempstr;
}

void HSSMultipleValue::add(HSSValue * newValue)
{
    if(newValue != NULL)
    {
        std_log3("Added value to HSSMultipleValue: " << newValue->toString());
        this->valueList.push_back(newValue);
    }
}

void HSSMultipleValue::removeLast()
{
    delete this->valueList.back();
    this->valueList.pop_back();
}

HSSValue * HSSMultipleValue::last()
{
    return this->valueList.back();
}

const int HSSMultipleValue::size()
{
    return this->valueList.size();
}