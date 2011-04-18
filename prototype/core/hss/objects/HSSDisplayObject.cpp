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
 *      Last changed: 2011/04/18
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 10
 *
 ********************************************************************/

#include "HSSDisplayObject.h"
#include "../../axr/AXRDebugging.h"
#include <math.h>
#include "HSSParserNode.h"
#include "HSSExpression.h"
#include <boost/pointer_cast.hpp>

using namespace AXR;

HSSDisplayObject::HSSDisplayObject()
: HSSObject()
{
    std_log3(std::string("creating annonymous display object"));
    this->type = HSSObjectTypeDisplayObject;
    
    this->_isDirty= true;
    this->_needsRereadRules = false;
    this->_needsSurface = true;
    this->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
    
    x = y = width = height = anchorX = anchorY = alignX = alignY = drawIndex = 0;
    tabIndex = zoomFactor = 1;
    flow = visible = true;
    does_float = false;
    
    width = 0.;
    height = 0.;
    
    elementName = std::string();
    contentText = "hello";
    
}

HSSDisplayObject::HSSDisplayObject(std::string name)
: HSSObject(name)
{
    std_log3(std::string("creating display object with name ").append(name));   
    this->type = HSSObjectTypeDisplayObject;
    
    this->_isDirty = true;
    this->_needsRereadRules = false;
    this->_needsSurface = true;
    
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
    cairo_surface_destroy(this->surface);
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

void HSSDisplayObject::readDefinitionObjects()
{
    if(this->_needsRereadRules){
        unsigned i, j;
        
        std::string propertyName;
        for (i=0; i<this->rules.size(); i++) {
            HSSRule::p& rule = this->rules[i];
            
            for (j=0; j<rule->propertiesSize(); j++) {
                HSSPropertyDefinition::p& propertyDefinition = rule->propertiesGet(j);
                propertyName = propertyDefinition->getName();
                
                if(propertyName == "width"){
                    //store a copy of the value
                    this->dWidth = propertyDefinition->getValue();
                    if(this->dWidth->isA(HSSParserNodeTypeExpression)){
                        HSSExpression::p widthExpression = boost::static_pointer_cast<HSSExpression>(this->dWidth);
                        this->width = widthExpression->evaluate();
                    }
                    
                } else if(propertyName == "height"){
                    //store a copy of the value
                    this->dHeight = propertyDefinition->getValue();
                    if(this->dHeight->isA(HSSParserNodeTypeExpression)){
                        HSSExpression::p heightExpression = boost::static_pointer_cast<HSSExpression>(this->dHeight);
                        this->height = heightExpression->evaluate();
                    }
                }
            }
        }
        
        this->_needsRereadRules = false;
    }
}

void HSSDisplayObject::setNeedsRereadRules(bool value)
{
    this->_needsRereadRules = value;
}

bool HSSDisplayObject::needsRereadRules()
{
    return this->_needsRereadRules;
}

void HSSDisplayObject::recursiveReadDefinitionObjects()
{
    this->readDefinitionObjects();
}

void HSSDisplayObject::regenerateSurface()
{
    if(this->_needsSurface){
        cairo_surface_destroy(this->surface);
        this->surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, ceil(this->width), ceil(this->height));
        this->setDirty(true);
        this->_needsSurface = false;
        fprintf(stderr, "created a new surface width: %f, height: %f\n", this->width, this->height);
    }
}

void HSSDisplayObject::recursiveRegenerateSurfaces()
{
    this->regenerateSurface();
}


void HSSDisplayObject::setNeedsSurface(bool value)
{
    this->_needsSurface = value;
}

bool HSSDisplayObject::needsSurface()
{
    return this->_needsSurface;
}


void HSSDisplayObject::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSDisplayObject::isDirty()
{
    return this->_isDirty;
}

//width
HSSParserNode::p HSSDisplayObject::getDWidth()  {   return this->dWidth; }
void HSSDisplayObject::setDWidth(HSSParserNode::p newDWidth)
{
    this->dWidth = newDWidth;
    if(newDWidth->isA(HSSParserNodeTypeExpression)){
        HSSExpression::p widthExpresssion = boost::static_pointer_cast<HSSExpression>(newDWidth);
        this->width = widthExpresssion->evaluate();
    }
    
    this->setNeedsSurface(true);
    this->setDirty(true);
}
//height
HSSParserNode::p HSSDisplayObject::getDHeight() { return this->dHeight; }
void HSSDisplayObject::setDHeight(HSSParserNode::p newDHeight)
{
    this->dHeight = newDHeight;;
    if(newDHeight->isA(HSSParserNodeTypeExpression)){
        HSSExpression::p heightExpresssion = boost::static_pointer_cast<HSSExpression>(newDHeight);
        this->height = heightExpresssion->evaluate();
    }
    
    this->setNeedsSurface(true);
    this->setDirty(true);
}



