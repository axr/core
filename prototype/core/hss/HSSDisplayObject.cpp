/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MMMMMD                                                         
 *            :: VD
 *           ::  .                                                         
 *          ::                                                              
 *          M   NZ      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       .MMZ. MMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      AMMMMMDJMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      MM  MMMMMC         ___MMMM     VMMMMMMM6       MMMM                   
 *      MM  MMMMMMM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *     :MM AMMMMMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *    :: MMMMMMMMMM    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *   .     MMMMM~     MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         MMMMM      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        MMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       MMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      MMMM MMMM                                                           
 *     AMMM  .MMM                                         
 *     MMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     MMM    MMA       ====================================                              
 *     DMN    MM                               
 *      MN    ZM                       
 *            MM,
 *
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
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

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





