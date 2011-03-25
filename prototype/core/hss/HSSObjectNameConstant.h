//
//  HSSObjectNameConstant.h
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSOBJECTNAMECONSTANT_H
#define HSSOBJECTNAMECONSTANT_H

#include "HSSExpression.h"

class HSSObjectNameConstant : public HSSExpression
{
public:
    HSSObjectNameConstant(string value);
    virtual ~HSSObjectNameConstant();
    
    void setValue(string newValue);
    string getValue();
    
    HSSParserNode * evaluate();
    
    string toString();
    
protected:
    string value;
};

#endif