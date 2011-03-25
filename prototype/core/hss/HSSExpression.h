//
//  HSSExpression.h
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSEXPRESSION_H
#define HSSEXPRESSION_H

#include "HSSParserNode.h"

class HSSExpression : public HSSParserNode
{
public:
    HSSExpression();
    virtual ~HSSExpression();
    virtual HSSParserNode * evaluate() =0;
};

#endif