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

