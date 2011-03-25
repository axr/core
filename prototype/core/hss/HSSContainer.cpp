//
//  HSSContainer.cpp
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSContainer.h"

string HSSContainer::toString()
{
    if (this->isNamed()) {
        return string("HSSContainer: ").append(this->name);
    } else {
        return "Annonymous HSSContainer";
    }
}

string HSSContainer::defaultObjectType(string property)
{
    if (property == "shape"){
        return "roundedRect";
    } else if (property == "innerMargin"){
        return "margin";
    } else if (property == "background"){
        return "image";
    } else {
        return HSSDisplayObject::defaultObjectType(property);
    }
}

bool HSSContainer::isKeyword(string value, string property)
{
    if (value == "center"){
        if (   property == "contentAlignX"
            || property == "contentAlignY" ) {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSDisplayObject::isKeyword(value, property);
}