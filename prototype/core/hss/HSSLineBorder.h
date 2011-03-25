//
//  HSSLineBorder.h
//  mac
//
//  Created by Miro Keller on 20/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSLINEBORDER_H
#define HSSLINEBORDER_H

#include <string>
#include "HSSBorder.h"

using namespace std;

class HSSLineBorder : public HSSBorder
{
public:
    virtual string toString();
    virtual string defaultObjectType(string property);
    virtual bool isKeyword(string value, string property);
};


#endif