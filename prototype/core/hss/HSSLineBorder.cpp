//
//  HSSLineBorder.cpp
//  mac
//
//  Created by Miro Keller on 20/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSLineBorder.h"

string HSSLineBorder::toString()
{
    if (this->isNamed()) {
        return string("HSSLineBorder: ").append(this->name);
    } else {
        return "Annonymous HSSLineBorder";
    }
}

string HSSLineBorder::defaultObjectType(string property)
{
    if (property == "color"){
        return "rgb";
    } else if (property == "joins"){
        return "mitered";
    }  else if (property == "caps"){
        return "mitered";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}