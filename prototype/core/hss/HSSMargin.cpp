//
//  HSSMargin.cpp
//  mac
//
//  Created by Miro Keller on 22/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSMargin.h"

HSSMargin::HSSMargin()
{
    this->segments = HSSMultipleValue();
}

HSSMargin::~HSSMargin()
{
    
}

string HSSMargin::toString()
{
    return "HSSMargin with size: "+this->size.getStringValue();
}

void HSSMargin::setSize(HSSValue newSize){
    this->size = newSize;
}

HSSValue HSSMargin::getSize()
{
    return this->size;
}