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
 *      Last changed: 2011/12/28
 *      HSS version: 1.0
 *      Core version: 0.43
 *      Revision: 6
 *
 ********************************************************************/

#include "HSSFont.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include "../../axr/AXRController.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"
#include "../parsing/HSSFunctionCall.h"
//#include <sstream>
#include "../parsing/HSSObjectDefinition.h"

using namespace AXR;

HSSFont::HSSFont()
:HSSObject()
{
    this->type = HSSObjectTypeFont;
    
    this->size = 18.;
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("face");
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    shorthandProperties.push_back("weight");
    this->setShorthandProperties(shorthandProperties);
}

HSSFont::~HSSFont()
{
    
}

std::string HSSFont::toString()
{
    if (this->isNamed()) {
        return std::string("HSSFont: ").append(this->name);
    } else {
        return "Annonymous HSSFont";
    }
}

std::string HSSFont::defaultObjectType()
{
    return "font";
}

std::string HSSFont::defaultObjectType(std::string property)
{
    if (property == "color"){
        return "rgb";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

void HSSFont::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertySize:
            this->setDSize(value);
            break;
        case HSSObservablePropertyFace:
            this->setDFace(value);
            break;
        case HSSObservablePropertyColor:
            this->setDColor(value);
            break;
        case HSSObservablePropertyWeight:
            this->setDWeight(value);
            break;
            
        default:
            HSSObject::setProperty(name, value);
            break;
    }
}

long double HSSFont::getSize() { return this->size; }
void HSSFont::setDSize(HSSParserNode::p value){
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for size of @font object "+this->name));
    }
    
    this->dSize = value;
    this->size = this->_setLDProperty(
                                       &HSSFont::sizeChanged,
                                       value,
                                       18.,
                                       HSSObservablePropertySize,
                                       this->observedSize,
                                       this->observedSizeProperty
                                       );
    
    this->notifyObservers(HSSObservablePropertySize, &this->size);
}

void HSSFont::sizeChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** sizeChanged unimplemented ****************************");
}

std::string HSSFont::getFace() { return this->face; }
void HSSFont::setDFace(HSSParserNode::p value){
    
    if (value->isA(HSSParserNodeTypeStringConstant)){
        this->dFace = value;
        HSSStringConstant::p theString = boost::static_pointer_cast<HSSStringConstant>(value);
        this->face = theString->getValue();
    } else {
        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for face of @font object "+this->name));
    }
}
void HSSFont::faceChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** faceChanged unimplemented ****************************");
}

HSSRgb::p HSSFont::getColor() { return this->color; }
HSSParserNode::p HSSFont::getDColor() { return this->dColor; }
void HSSFont::setDColor(HSSParserNode::p value)
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
            HSSFunctionCall::p fcall = boost::static_pointer_cast<HSSFunctionCall>(value);
            HSSFunction::p fnct = fcall->getFunction();
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                fnct->setScope(this->scope);
                this->color = *(HSSRgb::p *)fnct->evaluate();
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyColor, this, new HSSValueChangedCallback<HSSFont>(this, &HSSFont::colorChanged));
                
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

void HSSFont::colorChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** colorChanged unimplemented ****************************");
}

HSSKeywordConstant::p HSSFont::getWeight() { return this->weight; }
void HSSFont::setDWeight(HSSParserNode::p value){
    this->dWeight = value;
}

void HSSFont::weightChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** weightChanged unimplemented ****************************");
}

long double HSSFont::_setLDProperty(
                                    void(HSSFont::*callback)(HSSObservableProperty property, void* data),
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
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSFont>(this, callback));
                observedStore = expressionValue.get();
                observedStoreProperty = HSSObservablePropertyValue;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
            
            break;
            
        default:
            AXRWarning::p(new AXRWarning("HSSFont", "Unknown parser node type while setting value for HSSFont property"))->raise();
            break;
    }
    
    return ret;
}