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
 ********************************************************************/

#import "HSSEvent.h"
#import "../parsing/HSSObjectNameConstant.h"
#import "../parsing/HSSObjectDefinition.h"
#import "../parsing/HSSFunction.h"
#import <boost/unordered_map.hpp>
#import <boost/pointer_cast.hpp>
#import "../../axr/AXRController.h"
#import "HSSAction.h"
#import "HSSFlagAction.h"
#import "HSSFunctionAction.h"

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
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSEvent: creating event object");
    this->type = HSSObjectTypeEvent;
    this->eventType = type;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("action");
    this->setShorthandProperties(shorthandProperties);
    
    this->registerProperty(HSSObservablePropertyAction, &this->action);
}

HSSEvent::HSSEvent(const HSSEvent & orig)
: HSSObject(orig)
{
    this->eventType = orig.eventType;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("action");
    this->setShorthandProperties(shorthandProperties);
    
    this->registerProperty(HSSObservablePropertyAction, &this->action);
}

HSSEvent::p HSSEvent::clone() const{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSEvent: cloning event object");
    return boost::static_pointer_cast<HSSEvent, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSEvent::cloneImpl() const{
    return HSSClonable::p(new HSSEvent(*this));
}

HSSEvent::~HSSEvent()
{
    axr_log(AXR_DEBUG_CH_GENERAL_SPECIFIC, "HSSEvent: destructing event object");
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
                objdef->setThisObj(this->getThisObj());
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
                objdef->setThisObj(this->getThisObj());
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
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            switch (fnct->getFunctionType()) {
                case HSSFunctionTypeRef:
                {
                    fnct->setScope(this->scope);
                    fnct->setThisObj(this->getThisObj());
                    HSSParserNode::p remoteValue = *(HSSParserNode::p *)fnct->evaluate();
                    this->addDAction(remoteValue);
                    break;
                }
                    
                case HSSFunctionTypeFlag:
                {
                    fnct->setScope(this->scope);
                    fnct->setThisObj(this->getThisObj());
                    HSSFlagFunction::p flagFnct = boost::static_pointer_cast<HSSFlagFunction>(fnct);
                    HSSFlagAction::p flagAction = HSSFlagAction::p(new HSSFlagAction());
                    flagAction->setFlagFunction(flagFnct);
                    flagAction->setController(this->getController());
                    flagAction->setScope(this->scope);
                    flagAction->setThisObj(this->getThisObj());
                    this->action.push_back(flagAction);
                    break;
                }
                    
                case HSSFunctionTypeCustom:
                {
                    fnct->setScope(this->scope);
                    fnct->setThisObj(this->getThisObj());
                    HSSFunction::p theFnct = boost::static_pointer_cast<HSSFunction>(fnct);
                    HSSFunctionAction::p fnctAction = HSSFunctionAction::p(new HSSFunctionAction());
                    fnctAction->setFunction(theFnct);
                    fnctAction->setController(this->getController());
                    fnctAction->setScope(this->scope);
                    fnctAction->setThisObj(this->getThisObj());
                    this->action.push_back(fnctAction);
                    break;
                }
                    
                default:
                    throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid function type for action of @event "+this->name));
            }
            
            break;
        }
            
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for action of @event "+this->name));
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



