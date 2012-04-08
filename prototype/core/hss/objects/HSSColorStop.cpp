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
 *      Last changed: 2012/03/25
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 2
 *
 ********************************************************************/

#include "HSSColorStop.h"

#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include "../../axr/AXRController.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"
#include "../parsing/HSSFunction.h"
#include "../parsing/HSSObjectDefinition.h"

using namespace AXR;

HSSColorStop::HSSColorStop()
:HSSObject()
{
    this->type = HSSObjectTypeColorStop;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("position");
    shorthandProperties.push_back("balance");
    this->setShorthandProperties(shorthandProperties);
    
    this->registerProperty(HSSObservablePropertyColor, (void *) &this->color);
    this->registerProperty(HSSObservablePropertyPosition, (void *) &this->position);
    this->registerProperty(HSSObservablePropertyBalance, (void *) &this->balance);
    this->position = this->balance = 0.5;
}

HSSColorStop::HSSColorStop(const HSSColorStop & orig)
: HSSObject(orig)
{
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("position");
    shorthandProperties.push_back("balance");
    this->setShorthandProperties(shorthandProperties);
    
    this->registerProperty(HSSObservablePropertyColor, (void *) &this->color);
    this->registerProperty(HSSObservablePropertyPosition, (void *) &this->position);
    this->registerProperty(HSSObservablePropertyBalance, (void *) &this->balance);
    this->position = this->balance = 0.5;
}

HSSColorStop::p HSSColorStop::clone() const{
    return boost::static_pointer_cast<HSSColorStop, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSColorStop::cloneImpl() const{
    return HSSClonable::p(new HSSColorStop(*this));
}

HSSColorStop::~HSSColorStop()
{
    
}

std::string HSSColorStop::toString()
{
    if (this->isNamed()) {
        return std::string("HSSColorStop: ").append(this->name);
    } else {
        return "Annonymous HSSColorStop";
    }
}

std::string HSSColorStop::defaultObjectType()
{
    return "colorStop";
}

std::string HSSColorStop::defaultObjectType(std::string property)
{
    if (   property == "color"){
        return "rgb";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSColorStop::isKeyword(std::string value, std::string property)
{
    if (value == "start" || value == "middle" || value == "end"){
        if (property == "position" || property == "balance") {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

void HSSColorStop::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyColor:
            this->setDColor(value);
            break;
        case HSSObservablePropertyPosition:
            this->setDPosition(value);
            break;
        case HSSObservablePropertyBalance:
            this->setDBalance(value);
            break;
        default:
            HSSObject::setProperty(name, value);
            break;
    }
}

HSSRgb::p HSSColorStop::getColor() { return this->color; }
HSSParserNode::p HSSColorStop::getDColor() { return this->dColor; }
void HSSColorStop::setDColor(HSSParserNode::p value)
{
    bool valid = true;
    
    switch (value->getType()) {
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dColor = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            objdef->setScope(this->scope);
            objdef->apply();
            HSSObject::p theobj = objdef->getObject();
            if (theobj && theobj->isA(HSSObjectTypeRgb)) {
                this->color = boost::static_pointer_cast<HSSRgb>(theobj);
            } else {
                valid = false;
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            this->dColor = value;
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                this->setDColor(objdef);
                
            } catch (HSSObjectNotFoundException * e) {
                std_log(e->toString());
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->dColor = value;
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                fnct->setScope(this->scope);
                this->color = *(HSSRgb::p *)fnct->evaluate();
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyColor, this, new HSSValueChangedCallback<HSSColorStop>(this, &HSSColorStop::colorChanged));
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        default:
            valid = false;
    }
    
    if(!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for color of "+this->name));
    
    this->notifyObservers(HSSObservablePropertyColor, &this->color);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSColorStop::colorChanged(HSSObservableProperty source, void*data)
{
    switch (this->dColor->getType()) {
        case HSSParserNodeTypeFunctionCall:
            this->color = *(HSSRgb::p*)data;
            break;
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyPosition, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSColorStop::getPosition() { return this->position; }
HSSParserNode::p HSSColorStop::getDPosition() { return this->dPosition; }
void HSSColorStop::setDPosition(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
            this->dPosition = value;
            this->position = this->_setLDProperty(
                                                 &HSSColorStop::positionChanged,
                                                 value,
                                                 1.,
                                                 HSSObservablePropertyPosition,
                                                 this->observedPosition,
                                                 this->observedPositionProperty
                                                 );
            
            break;
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->dPosition = value;
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                fnct->setScope(this->scope);
                this->position = *(long double *)fnct->evaluate();
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyPosition, this, new HSSValueChangedCallback<HSSColorStop>(this, &HSSColorStop::positionChanged));
                
            } else {
                throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid function type for position of "+this->name));
            }
            
            break;
        }
            
        default:
            throw AXRWarning::p(new AXRWarning("HSSColorStop", "Invalid value for position of "+this->name));
    }
    this->notifyObservers(HSSObservablePropertyPosition, &this->position);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSColorStop::positionChanged(HSSObservableProperty source, void*data)
{
    switch (this->dPosition->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->position = *(long double*)data;
            break;
            
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dPosition);
            this->position = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyPosition, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSColorStop::getBalance() { return this->balance; }
HSSParserNode::p HSSColorStop::getDBalance() { return this->dBalance; }
void HSSColorStop::setDBalance(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
            this->dBalance = value;
            this->balance = this->_setLDProperty(
                                                 &HSSColorStop::balanceChanged,
                                                 value,
                                                 1.,
                                                 HSSObservablePropertyBalance,
                                                 this->observedBalance,
                                                 this->observedBalanceProperty
                                                 );
            
            break;
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->dBalance = value;
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                fnct->setScope(this->scope);
                this->balance = *(long double *)fnct->evaluate();
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyBalance, this, new HSSValueChangedCallback<HSSColorStop>(this, &HSSColorStop::balanceChanged));
                
            } else {
                throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid function type for balance of "+this->name));
            }
            
            break;
        }
            
        default:
            throw AXRWarning::p(new AXRWarning("HSSColorStop", "Invalid value for balance of "+this->name));
    }
    this->notifyObservers(HSSObservablePropertyBalance, &this->balance);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSColorStop::balanceChanged(AXR::HSSObservableProperty source, void *data)
{
    switch (this->dBalance->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->balance = *(long double*)data;
            break;
        
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dBalance);
            this->balance = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyBalance, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSColorStop::_setLDProperty(
                                        void(HSSColorStop::*callback)(HSSObservableProperty property, void* data),
                                        HSSParserNode::p         value,
                                        long double              percentageBase,
                                        HSSObservableProperty    observedSourceProperty,
                                        HSSObservable *          &observedStore,
                                        HSSObservableProperty    &observedStoreProperty
                                        )
{
    long double ret;
    
    HSSParserNodeType nodeType = value->getType();
    switch (nodeType) {
        case HSSParserNodeTypeNumberConstant:
        {
            HSSNumberConstant::p numberValue = boost::static_pointer_cast<HSSNumberConstant>(value);
            ret = numberValue->getValue();
            break;
        }
            
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(value);
            ret = percentageValue->getValue(percentageBase);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(value);
            expressionValue->setPercentageBase(percentageBase);
            expressionValue->setScope(this->scope);
            ret = expressionValue->evaluate();
            if(callback != NULL){
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSColorStop>(this, callback));
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
            
            break;
            
        default:
            AXRWarning::p(new AXRWarning("HSSColorStop", "Unknown parser node type while setting value for HSSLineGradient property"))->raise();
            break;
    }
    
    return ret;
}


