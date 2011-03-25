//
//  HSSPercentageConstant.h
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSPERCENTAGECONSTANT_H
#define HSSPERCENTAGECONSTANT_H

#include "HSSNumberConstant.h"

class HSSPercentageConstant : public HSSNumberConstant
{
public:
    HSSPercentageConstant(long double value);
    virtual ~HSSPercentageConstant();
    virtual string toString();
};

#endif