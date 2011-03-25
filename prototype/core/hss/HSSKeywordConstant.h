//
//  HSSKeywordConstant.h
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSKEYWORDCONSTANT_H
#define HSSKEYWORDCONSTANT_H

#include "HSSExpression.h"

class HSSKeywordConstant : public HSSExpression
{
public:
    HSSKeywordConstant(string value);
    virtual ~HSSKeywordConstant();
    
    void setValue(string newValue);
    string getValue();
    
    HSSParserNode * evaluate();
    
    string toString();
    
protected:
    string value;
};

#endif