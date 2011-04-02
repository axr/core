//
//  HSSDisplayObject.cpp
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSDisplayObject.h"

HSSDisplayObject::HSSDisplayObject()
: HSSObject()
{
    this->parent = NULL;
}

HSSDisplayObject::HSSDisplayObject(string name)
: HSSObject(name)
{
    this->parent = NULL;
}

HSSDisplayObject::~HSSDisplayObject()
{
    this->parent = NULL;
}

string HSSDisplayObject::toString()
{
    if (this->isNamed()) {
        return string("HSSDisplayObject: ").append(this->name);
    } else {
        return "Annonymous HSSDisplayObject";
    }
}

string HSSDisplayObject::defaultObjectType(string property)
{
    if (property == "margin"){
        return "margin";
    } else if (property == "border"){
        return "lineBorder";
    } else if (property == "transform"){
        return "rotate";
    } else if (property == "effects"){
        return "shadow";
    } else if (property == "animation") {
        return "transition";
    } else if (property == "behavior") {
        return "click";
    } else if (property == "mask") {
        return "image";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSDisplayObject::isKeyword(string value, string property)
{
    if (value == "center"){
        if (   property == "anchorX"
            || property == "anchorY"
            || property == "alignX"
            || property == "alignY"
            ) {
            return true;
        }
    } else if (value == "yes" || value == "no"){
        if (property == "flow") {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

HSSContainer * HSSDisplayObject::getParent()
{
    return this->parent;
}

void HSSDisplayObject::setParent(HSSContainer *parent)
{
    //is this necessary?
    this->parent = NULL;
    this->parent = parent;
}


void HSSDisplayObject::attributesAdd(string name, string value)
{
    this->attributes[name] = value;
}

void HSSDisplayObject::attributesRemove(string name)
{
    this->attributes.erase(name);
}





