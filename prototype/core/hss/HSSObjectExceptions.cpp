//
//  HSSObjectExceptions.cpp
//  mac
//
//  Created by Miro Keller on 20/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSObjectExceptions.h"

HSSUnknownObjectTypeException::HSSUnknownObjectTypeException(string type)
{
    this->type = type;
}

string HSSUnknownObjectTypeException::toString()
{
    return "HSSObject Error: unknown object type "+this->type+"while trying to create a new object";
}

string HSSUnknownValueTypeException::toString()
{
    return "HSSValue Error: unkown value type";
}