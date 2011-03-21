//
//  HSSObject.h
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSOBJECT_H
#define HSSOBJECT_H

#import <string>
using namespace std;

class HSSObject
{
public:
    
    static HSSObject * newObjectWithType(string type);
    
    string name;
    
    HSSObject();
    HSSObject(string name);
    ~HSSObject();
    
    virtual string toString();
    bool isNamed();
    void setName(string newName);
    void dropName();
    
    virtual string defaultObjectType(string property);
    
private:
    bool _isNamed;
};

#endif