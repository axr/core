//
//  HSSStringConstant.h
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSSTRINGCONSTANT_H
#define HSSSTRINGCONSTANT_H

#include "HSSExpression.h"

class HSSStringConstant : public HSSExpression
{
public:
    HSSStringConstant(string value);
    virtual ~HSSStringConstant();
    
    void setValue(string newValue);
    string getValue();
    
    HSSParserNode * evaluate();
    
    string toString();
    
protected:
    string value;
};

#endif