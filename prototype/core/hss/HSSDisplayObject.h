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
#include <map>
#include "HSSObject.h"

class HSSContainer;

using namespace std;

class HSSDisplayObject : public HSSObject
{
public:
    HSSDisplayObject();
    HSSDisplayObject(string name);
    virtual ~HSSDisplayObject();
    virtual string toString();
    virtual string defaultObjectType(string property);
    virtual bool isKeyword(string value, string property);
    HSSContainer * getParent();
    void setParent(HSSContainer * parent);
    void attributesAdd(string name, string value);
    void attributesRemove(string name);
    
protected:
    HSSContainer * parent;
    map<string, string>attributes;
};

#endif