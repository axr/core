//
//  HSSContainer.cpp
//  mac
//
//  Created by Miro Keller on 15/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSContainer.h"
#include "AXR.h"
#include <map>

HSSContainer::HSSContainer()
: HSSDisplayObject()
{
}

HSSContainer::HSSContainer(string name)
: HSSDisplayObject(name)
{
    
}

HSSContainer::~HSSContainer()
{
    unsigned i;
    for(i=0; i<this->children.size(); i++){
        //delete this->children.back();
        this->children.pop_back();
    }
}

string HSSContainer::toString()
{
    string tempstr;
    unsigned i;
    map<string, string>::iterator it;
    
    if (this->isNamed()) {
        tempstr = string("HSSContainer: ").append(this->name);
    } else {
        tempstr = "Annonymous HSSContainer";
    }
    
    if (this->attributes.size() > 0) {
        inc_output_indent();
        tempstr.append("\n").append(output_indent("with the following attributes:"));
        for(it=this->attributes.begin(); it!=this->attributes.end(); it++)
        {
            tempstr.append("\n").append(output_indent("- ").append((*it).first).append(": ").append((*it).second));
        }
        dec_output_indent();
        tempstr.append("\n");
    }
    
    if(this->children.size() > 0){
        inc_output_indent();
        tempstr.append("\n").append(output_indent("with the following children objects:"));
        for (i=0; i<this->children.size(); i++) {
            tempstr.append("\n").append(output_indent(this->children[i]->toString()));
        }
        dec_output_indent();
        tempstr.append("\n");
    }
    
    return tempstr;
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

void HSSContainer::add(HSSDisplayObject *child)
{
    this->children.push_back(child);
    child->setParent(this);
}

void HSSContainer::remove(unsigned index)
{
    delete this->children[index];
    this->children.erase(this->children.begin()+index);
}
