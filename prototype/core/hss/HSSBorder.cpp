//
//  HSSBorder.cpp
//  mac
//
//  Created by Miro Keller on 20/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSBorder.h"

string HSSBorder::toString()
{
    if (this->isNamed()) {
        return string("HSSBorder: ").append(this->name);
    } else {
        return "Annonymous HSSBorder";
    }
}

string HSSBorder::defaultObjectType(string property)
{
    if (property == "effects"){
        return "shadow";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}