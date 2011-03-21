//
//  HSSBorder.h
//  mac
//
//  Created by Miro Keller on 20/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSBORDER_H
#define HSSBORDER_H

#include <string>
#include "HSSObject.h"

using namespace std;

class HSSBorder : public HSSObject
{
public:
    virtual string toString();
    virtual string defaultObjectType(string property);
};


#endif