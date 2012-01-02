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
 *      Last changed: 2011/12/19
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 3
 *
 ********************************************************************/

#import "HSSEvent.h"
#import "../parsing/HSSObjectNameConstant.h"
#import "../parsing/HSSObjectDefinition.h"
#import "../parsing/HSSFunctionCall.h"
#import <boost/unordered_map.hpp>
#import <boost/pointer_cast.hpp>
#import "../../axr/AXRController.h"
#import "HSSAction.h"

using namespace AXR;

std::string HSSEvent::eventTypeStringRepresentation(AXR::HSSEventType eventType)
{
    boost::unordered_map<HSSEventType, std::string> types;
    types[HSSEventTypeNone          ] = "HSSEventTypeNone";
    types[HSSEventTypeLoad          ] = "HSSEventTypeLoad";
    types[HSSEventTypeClick         ] = "HSSEventTypeClick";
    types[HSSEventTypeDoubleClick   ] = "HSSEventTypeDoubleClick";
    types[HSSEventTypeTripleClick   ] = "HSSEventTypeTripleClick";
    types[HSSEventTypeMouseDown     ] = "HSSEventTypeMouseDown";
    types[HSSEventTypeMouseUp     ] = "HSSEventTypeMouseUp";
    types[HSSEventTypeMouseOver     ] = "HSSEventTypeMouseOver";
    types[HSSEventTypeMouseOut      ] = "HSSEventTypeMouseOut";
    types[HSSEventTypeMouseHold     ] = "HSSEventTypeMouseHold";
    types[HSSEventTypeMouseMove     ] = "HSSEventTypeMouseMove";
    types[HSSEventTypeClickSecondary] = "HSSEventTypeClickSecondary";
    types[HSSEventTypeClickTertiary ] = "HSSEventTypeClickTertiary";
    types[HSSEventTypeScroll        ] = "HSSEventTypeScroll";
    
    return types[eventType];
}

HSSEvent::HSSEvent(HSSEventType type)
: HSSObject()
{
    this->type = HSSObjectTypeEvent;
    this->eventType = type;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("action");
    this->setShorthandProperties(shorthandProperties);
}

HSSEvent::~HSSEvent()
{
    
}

std::string HSSEvent::toString()
{
    return "Generic HSSEvent of type"+HSSEvent::eventTypeStringRepresentation(this->eventType);
}

std::string HSSEvent::defaultObjectType()
{
    return "event";
}

std::string HSSEvent::defaultObjectType(std::string property)
{
    if (property == "action"){
        return "request";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

void HSSEvent::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyAction:
            this->setDAction(value);
            break;
            
        default:
            HSSObject::setProperty(name, value);
            break;
    }
}

bool HSSEvent::isA(HSSEventType type)
{
    return type == this->eventType;
}

HSSEventType HSSEvent::getEventType()
{
    return this->eventType;
}

std::vector<HSSAction::p> HSSEvent::getAction() { return this->action; }
const HSSParserNode::p HSSEvent::getDAction() const { return this->dAction; }
void HSSEvent::setDAction(HSSParserNode::p value)
{
    this->action.clear();
    this->dAction = value;
    this->addDAction(value);
}

void HSSEvent::addDAction(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeMultipleValueDefinition:
        {
            HSSParserNode::it iterator;
            HSSMultipleValueDefinition::p multiDef = boost::static_pointer_cast<HSSMultipleValueDefinition>(value);
            std::vector<HSSParserNode::p> values = multiDef->getValues();
            for (iterator = values.begin(); iterator != values.end(); iterator++) {
                this->addDAction(*iterator);
            }
            break;
        }
            
        case HSSParserNodeTypeObjectDefinition:
        {
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            if (objdef->getObject()->isA(HSSObjectTypeAction)) {
                objdef->setScope(this->scope);
                objdef->apply();
                HSSObject::p theObj = objdef->getObject();
                theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyAction, this, new HSSValueChangedCallback<HSSEvent>(this, &HSSEvent::actionChanged));
                this->action.push_back(boost::static_pointer_cast<HSSAction>(theObj));
            }
            
            break;
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                objdef->setScope(this->scope);
                objdef->apply();
                
                HSSObject::p obj = objdef->getObject();
                switch (obj->getObjectType()) {
                    case HSSObjectTypeAction:
                        this->action.push_back(boost::static_pointer_cast<HSSAction>(obj));
                        break;
                        
                    default:
                        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for action of @event "+this->name));
                        break;
                }
                
            } catch (AXRError::p e) {
                e->raise();
            }
            break;
        }
            
        case HSSParserNodeTypeFunctionCall:
        {
            HSSFunctionCall::p fcall = boost::static_pointer_cast<HSSFunctionCall>(value);
            HSSFunction::p fnct = fcall->getFunction();
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                
                fnct->setScope(this->scope);
                HSSParserNode::p remoteValue = *(HSSParserNode::p *)fnct->evaluate();
                this->addDAction(remoteValue);
                
            } else {
                throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid function type for action of @event "+this->name));
            }
            
            break;
        }
            
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for border of @event "+this->name));
    }
}

void HSSEvent::actionChanged(HSSObservableProperty source, void*data)
{
    std_log("HSSEvent::actionChanged unimplemented");
}

void HSSEvent::fire()
{
    std::vector<HSSAction::p> actions = this->getAction();
    unsigned i, size;
    for (i=0, size=actions.size(); i<size; i++) {
        actions[i]->fire();
    } 
}



