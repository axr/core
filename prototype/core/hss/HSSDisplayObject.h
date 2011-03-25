//
//  HSSDisplayObject.h
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSDISPLAYOBJECT_H
#define HSSDISPLAYOBJECT_H

#include <string>
#include "HSSObject.h"

using namespace std;

class HSSDisplayObject : public HSSObject
{
public:
    virtual string toString();
    virtual string defaultObjectType(string property);
    virtual bool isKeyword(string value, string property);
};

#endif