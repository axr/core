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
 *      Last changed: 2011/11/24
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 7
 *
 ********************************************************************/

#include "HSSBorder.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"

using namespace AXR;

HSSBorder::HSSBorder()
:HSSObject()
{
    this->type = HSSObjectTypeBorder;
    this->registerProperty(HSSObservablePropertySize, (void *) &this->size);
}

HSSBorder::~HSSBorder()
{
    
}

std::string HSSBorder::toString()
{
    if (this->isNamed()) {
        return std::string("HSSBorder: ").append(this->name);
    } else {
        return "Annonymous HSSBorder";
    }
}

std::string HSSBorder::defaultObjectType()
{
    return "border";
}

std::string HSSBorder::defaultObjectType(std::string property)
{
    if (property == "effects"){
        return "shadow";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSBorder::isKeyword(std::string value, std::string property)
{
    if (value == "inside" || value == "centered" || value == "outside"){
        if (property == "position") {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

void HSSBorder::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertySize:
            this->setDSize(value);
            break;
        default:
            HSSObject::setProperty(name, value);
            break;
    }
}

long double HSSBorder::getSize() { return this->size; }
void HSSBorder::setDSize(HSSParserNode::p value){
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSBorder", "Invalid value for size of "+this->name));
    }
    
    this->dSize = value;
    this->size = this->_setLDProperty(
                                      &HSSBorder::sizeChanged,
                                      value,
                                      18.,
                                      HSSObservablePropertySize,
                                      this->observedSize,
                                      this->observedSizeProperty
                                      );
    
    this->notifyObservers(HSSObservablePropertySize, &this->size);
}

void HSSBorder::sizeChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** sizeChanged unimplemented ****************************");
}

long double HSSBorder::_setLDProperty(
                                          void(HSSBorder::*callback)(HSSObservableProperty property, void* data),
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
            ret = percentageValue->getValue(255.0);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(value);
            expressionValue->setPercentageBase(255.0);
            //expressionValue->setScope(scope);
            ret = expressionValue->evaluate();
            if(callback != NULL){
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSBorder>(this, callback));
                observedStore = expressionValue.get();
                observedStoreProperty = HSSObservablePropertyValue;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
            
            break;
            
        default:
            AXRWarning::p(new AXRWarning("HSSLineBorder", "Unknown parser node type while setting value for HSSLineBorder property"))->raise();
            break;
    }
    
    return ret;
}


