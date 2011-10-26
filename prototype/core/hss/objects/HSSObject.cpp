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
 *      Last changed: 2011/10/22
 *      HSS version: 1.0
 *      Core version: 0.4
 *      Revision: 14
 *
 ********************************************************************/

#include "HSSObject.h"
#include "HSSObjects.h"
#include "HSSObjectExceptions.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include "HSSMultipleValue.h"
#include "../parsing/HSSMultipleValueDefinition.h"
#include "../parsing/HSSObjectNameConstant.h"
#include "../../axr/AXRController.h"

using namespace AXR;

HSSObject::p HSSObject::newObjectWithType(std::string type){
    
    //FIXME: this is UH-gly
    if (type == "container"){
        return HSSContainer::p(new HSSContainer());
    } else if (type == "displayObject"){
        return HSSDisplayObject::p(new HSSDisplayObject());
    } else if (type == "lineBorder") {
        return HSSLineBorder::p(new HSSLineBorder());
    } else if (type == "object") {
        return HSSObject::p(new HSSObject());
    } else if (type == "value") {
        return HSSValue::p(new HSSValue());
    } else if (type == "margin"){
        return HSSMargin::p(new HSSMargin());
    } else if (type == "rgba"){
        return HSSRgba::p(new HSSRgba());
    } else if (type == "font"){
        return HSSFont::p(new HSSFont());
    } else if (type == "rectangle"){
        return HSSRectangle::p(new HSSRectangle());
    } else if (type == "roundedRect"){
        return HSSRoundedRect::p(new HSSRoundedRect());
    } else if (type == "circle"){
        return HSSCircle::p(new HSSCircle());
    } else {
        throw AXRError::p(new AXRError("HSSObject", type));
    }
    
    return HSSObject::p();
}


HSSObject::HSSObject()
{
    this->_isNamed = false;
    this->name = "";
    this->type = HSSObjectTypeGeneric;
    this->shorthandIndex = 0;
}

HSSObject::HSSObject(std::string name)
{
    this->name = name;
    this->_isNamed = true;
    this->type = HSSObjectTypeGeneric;
    this->shorthandIndex = 0;
}


HSSObject::~HSSObject()
{
    
}

bool HSSObject::isA(HSSObjectType otherType)
{
	return otherType == this->type;
}

HSSObjectType HSSObject::getType()
{
    return this->type;
}

bool HSSObject::isKeyword(std::string value, std::string property)
{
    if(   value == "default"
       || value == "inherit"
       || value == "undefined"
       || value == "none"      ){
        return true;
    } else {
        return false;
    }
}

bool HSSObject::isFunction(std::string value, std::string property)
{
    if(   value == "min"
       || value == "max"
       || value == "floor"
       || value == "ceil"
       || value == "round"
       || value == "ref"
       || value == "sel"       ){
        return true;
    } else {
        return false;
    }
}


std::string HSSObject::toString()
{
    if (this->isNamed()) {
        return std::string("HSSObject: ").append(this->name);
    } else {
        return "Annonymous HSSObject";
    }
}

bool HSSObject::isNamed()
{
    return this->_isNamed;
}

void HSSObject::setName(std::string newName)
{
    this->name = newName;
    this->_isNamed = true;
}

void HSSObject::dropName()
{
    this->name = "";
    this->_isNamed = false;
}


std::string HSSObject::defaultObjectType(){
    return "value";
}

std::string HSSObject::defaultObjectType(std::string property){
    return "value";
}

std::string HSSObject::getPropertyForCurrentValue()
{
    bool done = false;
    std::string ret;
    
    security_brake_init();
    while (!done) {
        ret = this->shorthandProperties[this->shorthandIndex];
        if (!this->skipShorthand[ret]){
            return ret;
        } else {
            done = !this->shorthandNext();
        }
        security_brake();
    }
    security_brake_reset();
    return "";
}

void HSSObject::setShorthandProperties(std::vector<std::string> newValues)
{
    this->shorthandProperties = newValues;
    this->shorthandReset();
}

void HSSObject::shorthandSkip(std::string propertyName)
{
    this->skipShorthand[propertyName] = true;
}

bool HSSObject::shorthandNext()
{
    if( this->shorthandIndex < this->shorthandProperties.size() + 1){
        this->shorthandIndex++;
        return true;
    }
    return false;
}

void HSSObject::shorthandReset()
{
    this->shorthandIndex = 0;
}

unsigned HSSObject::getShorthandIndex()
{
    return this->shorthandIndex;
}

void HSSObject::setShorthandIndex(unsigned newValue)
{
    if( newValue < this->shorthandProperties.size()){
        this->shorthandIndex = newValue;
    } else {
        this->shorthandIndex = this->shorthandProperties.size();
    }
}

HSSParserNode::p HSSObject::getDIsA() { return this->dIsA; }
void HSSObject::setDIsA(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeObjectDefinition:
        case HSSParserNodeTypeObjectNameConstant:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
        case HSSParserNodeTypeMultipleValueDefinition:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSObject", "Invalid value for font of "+this->name));
    }
    
    if(!this->dIsA){
        this->dIsA = boost::shared_ptr<HSSMultipleValueDefinition>(new HSSMultipleValueDefinition());
    }
    this->dIsA->getValues()->add(value);
    
    switch (value->getType()) {
        case HSSParserNodeTypeObjectDefinition:
        {
            AXRError("HSSObject", "Unimplemented").raise();
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                objdef->apply();
                std::deque<HSSPropertyDefinition::p> properties = objdef->getProperties();
                unsigned i, size;
                for (i = 0, size = properties.size(); i<size; i++){
                    this->setProperty(HSSObservable::observablePropertyFromString(properties[i]->getName()), properties[i]->getValue());
                }
                
                
            } catch (HSSObjectNotFoundException * e) {
                std_log(e->toString());
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            break;
        }
            
        default:
        {
            std_log1("unkown parser node type in font property of display object "+this->name+": "+HSSParserNode::parserNodeStringRepresentation(value->getType()));
            break;
        }
    }
    
    this->notifyObservers(HSSObservablePropertyIsA, &this->dIsA);
}

void HSSObject::isAChanged(AXR::HSSObservableProperty source, void *data)
{
    
}

void HSSObject::setPropertyWithName(std::string name, HSSParserNode::p value)
{
    HSSObservableProperty property = HSSObservable::observablePropertyFromString(name);
    if(property != HSSObservablePropertyNone){
        this->setProperty(property, value);
    } else {
        AXRWarning::p(new AXRWarning("HSSDisplayObject", "Unknown property "+name+", ignoring value"))->raise();
    }
}

void HSSObject::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    if (name == HSSObservablePropertyIsA) {
        this->setDIsA(value);
        
    } else {
        AXRWarning::p(new AXRWarning("HSSDisplayObject", "Unknown property "+HSSObservable::observablePropertyStringRepresentation(name)+", ignoring value"))->raise();
    }
}

void HSSObject::setProperty(HSSObservableProperty name, void * value)
{
    AXRWarning::p(new AXRWarning("HSSDisplayObject", "Unknown property "+HSSObservable::observablePropertyStringRepresentation(name)+", ignoring value"))->raise();
}

void * HSSObject::getProperty(HSSObservableProperty name)
{
    return this->properties[name];
}

void HSSObject::registerProperty(HSSObservableProperty name, void * property)
{
    this->properties[name] = property;
}





