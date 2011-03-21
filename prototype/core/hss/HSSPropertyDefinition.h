//
//  HSSPropertyDefinition.h
//  mac
//
//  Created by Miro Keller on 16/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSPROPERTYDEFINITION_H
#define HSSPROPERTYDEFINITION_H

#include "HSSStatement.h"
#include "HSSObject.h"
#include <string>
using namespace std;

class HSSPropertyDefinition : public HSSStatement
{
public:
    HSSPropertyDefinition();
    HSSPropertyDefinition(string name);
    HSSPropertyDefinition(string name, HSSStatement * value);
    ~HSSPropertyDefinition();
    string toString();
    void setName(string name);
    string getName();
    void setValue(HSSStatement * value);
    HSSStatement * getValue();
    
protected:
    string name;
    HSSStatement * value;
};

#endif