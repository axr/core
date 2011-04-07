/**********************************************************
 *
 *      d8888 Y88b   d88P 8888888b.  
 *      d88888  Y88b d88P  888   Y88b 
 *      d88P888   Y88o88P   888    888 
 *      d88P 888    Y888P    888   d88P 
 *      88P  888    d888b    8888888P"  
 *      d88P   888   d88888b   888 T88b   
 *      d8888888888  d88P Y88b  888  T88b  
 *      d88P     888 d88P   Y88b 888   T88b 
 *      
 *      ARBITRARY·······XML········RENDERING
 *      ====================================
 *
 *      AUTHORS: Miro Keller
 *      
 *      COPYRIGHT: ©2011 - All Rights Reserved
 *
 *      LICENSE: see License.txt file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 **********************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 **********************************************************/

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





