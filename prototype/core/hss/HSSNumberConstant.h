//
//  HSSNumberConstant.h
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSNUMBERCONSTANT_H
#define HSSNUMBERCONSTANT_H

#include "HSSExpression.h"

class HSSNumberConstant : public HSSExpression
{
public:
    HSSNumberConstant(long double value);
    virtual ~HSSNumberConstant();
    
    void setValue(long double newValue);
    long double getValue();
    
    HSSParserNode * evaluate();
    
    string toString();
    
protected:
    long double value;
};

#endif