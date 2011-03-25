//
//  HSSPercentageConstant.cpp
//  mac
//
//  Created by Miro Keller on 25/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSPercentageConstant.h"
#include <sstream>

HSSPercentageConstant::HSSPercentageConstant(long double value)
: HSSNumberConstant(value)
{
    
}

HSSPercentageConstant::~HSSPercentageConstant()
{
    
}

string HSSPercentageConstant::toString()
{
    string ret = "HSSPercentageConstant with value ";
    std::ostringstream tempstream;
    //create a temp stream to convert the float to a string
    tempstream << this->value;
    ret.append(tempstream.str());
    ret.append("%");
    return ret;
}