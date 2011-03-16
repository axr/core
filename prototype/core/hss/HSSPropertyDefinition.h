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
#include <string>
using namespace std;

class HSSPropertyDefinition : public HSSStatement
{
public:
    HSSPropertyDefinition(string name);
    ~HSSPropertyDefinition();
    string toString();
    
protected:
    string name;
};

#endif