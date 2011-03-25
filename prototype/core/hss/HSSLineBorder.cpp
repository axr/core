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

bool HSSLineBorder::isKeyword(string value, string property)
{
    if (value == "rounded" || value == "projected"){
        if (property == "caps") {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSBorder::isKeyword(value, property);
}