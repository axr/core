//
//  HSSObjectExceptions.h
//  mac
//
//  Created by Miro Keller on 20/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#ifndef HSSOBJECTEXCEPTIONS_H
#define HSSOBJECTEXCEPTIONS_H


#include <exception>
#include <string>
#include "HSSToken.h"

using namespace std;

class HSSUnknownObjectTypeException
{
public:
    HSSUnknownObjectTypeException(string type);
    virtual string toString();
    string type;
};

class HSSUnknownValueTypeException
{
public:
    virtual string toString();
};

#endif