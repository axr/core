//
//  HSSObject.cpp
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSObject.h"
#include "HSSObjects.h"
#include "HSSObjectExceptions.h"


HSSObject * HSSObject::newObjectWithType(string type){
    HSSObject * ret;
    
    if (type == "container"){
        ret = new HSSContainer();
    } else if (type == "displayObject"){
        ret = new HSSDisplayObject();
    } else if (type == "lineBorder") {
        ret = new HSSLineBorder();
    } else if (type == "object") {
        ret = new HSSObject();
    } else if (type == "value") {
        ret = new HSSValue();
    } else if (type == "margin"){
        ret = new HSSMargin();
    } else {
        throw HSSUnknownObjectTypeException(type);
    }
    
    return ret;
}


HSSObject::HSSObject()
{
    this->_isNamed = false;
    this->name = "";
}

HSSObject::HSSObject(string name)
{
    this->name = name;
    this->_isNamed = true;
}

HSSObject::~HSSObject()
{
    
}

string HSSObject::toString()
{
    if (this->isNamed()) {
        return string("HSSObject: ").append(this->name);
    } else {
        return "Annonymous HSSObject";
    }
}

bool HSSObject::isNamed()
{
    return this->_isNamed;
}

void HSSObject::setName(string newName)
{
    this->name = newName;
    this->_isNamed = true;
}

void HSSObject::dropName()
{
    this->name = "";
    this->_isNamed = false;
}


string HSSObject::defaultObjectType(string property){
    return "value";
}

bool HSSObject::isKeyword(string value, string property)
{
    if(value == "default" || value == "inherit" || value == "undefined" || value == "none"){
        return true;
    } else {
        return false;
    }
}

