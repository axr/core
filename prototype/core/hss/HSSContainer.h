//
//  HSSContainer.h
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSCONTAINER_H
#define HSSCONTAINER_H

#include <string>
#include "HSSDisplayObject.h"

using namespace std;

class HSSContainer : public HSSDisplayObject
{
public:
    virtual string toString();
    virtual string defaultObjectType(string property);
};

#endif