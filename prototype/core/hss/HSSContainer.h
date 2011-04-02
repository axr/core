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
#include <vector>
#include "HSSDisplayObject.h"

using namespace std;

class HSSContainer : public HSSDisplayObject
{
public:
    HSSContainer();
    HSSContainer(string name);
    virtual ~HSSContainer();
    virtual string toString();
    virtual string defaultObjectType(string property);
    virtual bool isKeyword(string value, string property);
    
    void add(HSSDisplayObject * child);
    void remove(unsigned index);
    
protected:
    vector<HSSDisplayObject *>children;
};

#endif