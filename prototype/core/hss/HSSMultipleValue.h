//
//  HSSMultipleValue.h
//  mac
//
//  Created by Miro Keller on 22/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSMULTIPLEVALUE_H
#define HSSMULTIPLEVALUE_H

#include "HSSObject.h"
#include "HSSValue.h"
#include <vector>

class HSSMultipleValue : public HSSObject
{
public:
    HSSMultipleValue();
    virtual ~HSSMultipleValue();
    virtual string toString();
    
    //adds a pointer to newStatement to the properties list
    //this function acquires ownership of the statement
    void add(HSSValue * newValue);
    //removes last statement from the list and then deletes it
    void removeLast();
    //returns a pointer to the last statement in the list
    HSSValue * last();
    //returns how many statements there are in the properties list
    const int size();
    
protected:
    vector<HSSValue *>valueList;
};

#endif