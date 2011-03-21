//
//  HSSValue.h
//  mac
//
//  Created by Miro Keller on 20/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSVALUE_H
#define HSSVALUE_H

#include <string>
#include "HSSObject.h"
#include "HSSObjectExceptions.h"

using namespace std;

enum HSSValueType {
    HSSValueNumberInt,
    HSSValueNumberFloat,
    HSSValueString,
    HSSValueKeyword
};

class HSSValue : public HSSObject
{
public:
    HSSValue(long int value);
    HSSValue(long double value);
    HSSValue(string value);
    HSSValue(void * passNULL, string keyword);
    
    virtual string toString();
    virtual string defaultObjectType(string property);
    HSSValueType getType();
    
    void setValue(long int value);
    void setValue(long double value);
    void setValue(string value);
    void setKWValue(string keyword);
    
protected:
    HSSValueType type;
    string stringValue;
    long int intValue;
    long double floatValue;
};

#endif