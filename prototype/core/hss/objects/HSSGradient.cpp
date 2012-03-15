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
 *      Last changed: 2012/03/15
 *      HSS version: 1.0
 *      Core version: 0.43
 *      Revision: 2
 *
 ********************************************************************/

#include "HSSGradient.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include "../../axr/AXRController.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"
#include "../parsing/HSSFunction.h"
#include "../parsing/HSSObjectDefinition.h"

using namespace AXR;

HSSGradient::HSSGradient()
:HSSObject()
{
    this->type = HSSObjectTypeGradient;
    this->registerProperty(HSSObservablePropertyStartColor, (void *) &this->startColor);
    this->registerProperty(HSSObservablePropertyEndColor, (void *) &this->endColor);
    this->registerProperty(HSSObservablePropertyBalance, (void *) &this->balance);
}

HSSGradient::~HSSGradient()
{
    
}

std::string HSSGradient::toString()
{
    if (this->isNamed()) {
        return std::string("HSSGradient: ").append(this->name);
    } else {
        return "Annonymous HSSGradient";
    }
}

std::string HSSGradient::defaultObjectType()
{
    return "linearGradient";
}

std::string HSSGradient::defaultObjectType(std::string property)
{
    if (   property == "startColor"
        || property == "endColor"){
        return "rgb";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSGradient::isKeyword(std::string value, std::string property)
{
    if (value == "inside" || value == "centered" || value == "outside"){
        if (property == "position") {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

void HSSGradient::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyStartColor:
            this->setDStartColor(value);
            break;
        case HSSObservablePropertyEndColor:
            this->setDEndColor(value);
            break;
        case HSSObservablePropertyBalance:
            this->setDBalance(value);
            break;
        default:
            HSSObject::setProperty(name, value);
            break;
    }
}

HSSRgb::p HSSGradient::getStartColor() { return this->startColor; }
HSSParserNode::p HSSGradient::getDStartColor() { return this->dStartColor; }
void HSSGradient::setDStartColor(HSSParserNode::p value)
{
    bool valid = true;
    
    switch (value->getType()) {
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dStartColor = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            objdef->setScope(this->scope);
            objdef->apply();
            HSSObject::p theobj = objdef->getObject();
            if (theobj && theobj->isA(HSSObjectTypeRgb)) {
                this->startColor = boost::static_pointer_cast<HSSRgb>(theobj);
            } else {
                valid = false;
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            this->dStartColor = value;
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                this->setDStartColor(objdef);
                
            } catch (HSSObjectNotFoundException * e) {
                std_log(e->toString());
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->dStartColor = value;
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                fnct->setScope(this->scope);
                this->startColor = *(HSSRgb::p *)fnct->evaluate();
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyStartColor, this, new HSSValueChangedCallback<HSSGradient>(this, &HSSGradient::startColorChanged));
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        default:
            valid = false;
    }
    
    if(!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for startColor of "+this->name));
    
    this->notifyObservers(HSSObservablePropertyStartColor, &this->startColor);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSGradient::startColorChanged(HSSObservableProperty source, void*data)
{
    
}

HSSRgb::p HSSGradient::getEndColor() { return this->endColor; }
HSSParserNode::p HSSGradient::getDEndColor() { return this->dEndColor; }
void HSSGradient::setDEndColor(HSSParserNode::p value)
{
    bool valid = true;
    
    switch (value->getType()) {
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dEndColor = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            objdef->setScope(this->scope);
            objdef->apply();
            HSSObject::p theobj = objdef->getObject();
            if (theobj && theobj->isA(HSSObjectTypeRgb)) {
                this->endColor = boost::static_pointer_cast<HSSRgb>(theobj);
            } else {
                valid = false;
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            this->dStartColor = value;
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                this->setDEndColor(objdef);
                
            } catch (HSSObjectNotFoundException * e) {
                std_log(e->toString());
            }
            
            break;
        }
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->dEndColor = value;
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                fnct->setScope(this->scope);
                this->endColor = *(HSSRgb::p *)fnct->evaluate();
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyEndColor, this, new HSSValueChangedCallback<HSSGradient>(this, &HSSGradient::endColorChanged));
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        default:
            valid = false;
    }
    
    if(!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for endColor of "+this->name));
    
    this->notifyObservers(HSSObservablePropertyEndColor, &this->endColor);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSGradient::endColorChanged(HSSObservableProperty source, void*data)
{
    
}

long double HSSGradient::getBalance() { return this->balance; }
HSSParserNode::p HSSGradient::getDBalance() { return this->dBalance; }
void HSSGradient::setDBalance(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
            this->dBalance = value;
            this->balance = this->_setLDProperty(
                                              &HSSGradient::balanceChanged,
                                              value,
                                              18.,
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
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyBalance, this, new HSSValueChangedCallback<HSSGradient>(this, &HSSGradient::balanceChanged));
                
            } else {
                throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid function type size of "+this->name));
            }
            
            break;
        }
            
        default:
            throw AXRWarning::p(new AXRWarning("HSSGradient", "Invalid value for size of "+this->name));
    }
    this->notifyObservers(HSSObservablePropertyBalance, &this->balance);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSGradient::balanceChanged(AXR::HSSObservableProperty source, void *data)
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

long double HSSGradient::_setLDProperty(
                                      void(HSSGradient::*callback)(HSSObservableProperty property, void* data),
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
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSGradient>(this, callback));
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
            
            break;
            
        default:
            AXRWarning::p(new AXRWarning("HSSLineGradient", "Unknown parser node type while setting value for HSSLineGradient property"))->raise();
            break;
    }
    
    return ret;
}


