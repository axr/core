/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MA:MMD                                                         
 *            :: VD
 *           ::  º                                                         
 *          ::                                                              
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM                   
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      TMMM MMMM                                                           
 *     TMMM  .MMM                                         
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     TMM    MMA       ====================================                              
 *     TMN    MM                               
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
 *      Last changed: 2011/04/16
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 8
 *
 ********************************************************************/

#include "HSSDisplayObject.h"
#include "../../axr/AXRDebugging.h"

using namespace AXR;

HSSDisplayObject::HSSDisplayObject()
: HSSObject()
{
    std_log3(std::string("creating annonymous display object"));
    this->type = HSSObjectTypeDisplayObject;
    x = y = width = height = anchorX = anchorY = alignX = alignY = drawIndex = 0;
    tabIndex = zoomFactor = 1;
    flow = visible = true;
    does_float = false;
    
    width = 100.;
    height = 100.;
    
    elementName = std::string();
    contentText = "hello";
    
}

HSSDisplayObject::HSSDisplayObject(std::string name)
: HSSObject(name)
{
    std_log3(std::string("creating display object with name ").append(name));   
    this->type = HSSObjectTypeDisplayObject;
    x = y = width = height = anchorX = anchorY = alignX = alignY = drawIndex = 0;
    tabIndex = zoomFactor = 1;
    flow = visible = true;
    does_float = false;
    
    width = 100.;
    height = 100.;
    
    elementName = std::string();
    contentText = std::string();
}

HSSDisplayObject::~HSSDisplayObject()
{
    std_log3(std::string("destroying display object with name ").append(this->name));
}

std::string HSSDisplayObject::toString()
{
    if (this->isNamed()) {
        return std::string("HSSDisplayObject: ").append(this->name);
    } else {
        return "Annonymous HSSDisplayObject";
    }
}

std::string HSSDisplayObject::defaultObjectType(std::string property)
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

bool HSSDisplayObject::isKeyword(std::string value, std::string property)
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

boost::shared_ptr<HSSContainer> HSSDisplayObject::getParent()
{
    boost::shared_ptr<HSSContainer> parent = this->parent.lock();
    return parent;
}

void HSSDisplayObject::setParent(boost::shared_ptr<HSSContainer> parent)
{
    this->parent = parentPointer(parent);
}


void HSSDisplayObject::attributesAdd(std::string name, std::string value)
{
    this->attributes[name] = value;
}

void HSSDisplayObject::attributesRemove(std::string name)
{
    this->attributes.erase(name);
}


std::string HSSDisplayObject::getElementName()
{
    return this->elementName;
}

void HSSDisplayObject::setElementName(std::string newName)
{
    this->elementName = newName;
}

//rules

void HSSDisplayObject::rulesAdd(HSSRule::p newRule)
{
    this->rules.push_back(newRule);
}

HSSRule::p HSSDisplayObject::rulesGet(unsigned index)
{
    return this->rules[index];
}

void HSSDisplayObject::rulesRemove(unsigned index)
{
    this->rules.erase(this->rules.begin()+index);
}

void HSSDisplayObject::rulesRemoveLast()
{
    this->rules.pop_back();
}

const int HSSDisplayObject::rulesSize()
{
    return this->rules.size();
}





