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
 *      Last changed: 2011/09/26
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

#include "HSSFont.h"
#include "../../axr/AXRDebugging.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"
//#include <sstream>
#include "../parsing/HSSObjectDefinition.h"

using namespace AXR;

HSSFont::HSSFont()
:HSSObject()
{
    this->type = HSSObjectTypeFont;
    
    this->size = 0.;
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

std::string HSSFont::defaultObjectType(std::string property)
{
    if (property == "color"){
        return "rgba";
    } else {
        return HSSObject::defaultObjectType(property);
    }
}


//FIXME: split into individual setters and call them from here
void HSSFont::setProperty(std::string name, HSSParserNode::p value)
{
    if( name == "size" ){
        this->setDSize(value);
        
    } else if (name == "face"){
        this->setDFace(value);
        
    } else if (name == "color"){
        this->setDColor(value);
        
    } else if (name == "weight"){
        this->setDWeight(value);
        
    } else {
        HSSObject::setProperty(name, value);
    }
}

long double HSSFont::getSize() { return this->size; }
void HSSFont::setDSize(HSSParserNode::p value){
    this->dSize = value;
    this->size = this->_setLDProperty(
                                       &HSSFont::sizeChanged,
                                       value,
                                       18.,
                                       HSSObservablePropertySize,
                                       this->observedSize,
                                       this->observedSizeProperty
                                       );
}

void HSSFont::sizeChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** sizeChanged unimplemented ****************************");
}

std::string HSSFont::getFace() { return this->face; }
void HSSFont::setDFace(HSSParserNode::p value){
    this->dFace = value;
    if (value->isA(HSSParserNodeTypeStringConstant)){
        HSSStringConstant::p theString = boost::static_pointer_cast<HSSStringConstant>(value);
        this->face = theString->getValue();
    }
}
void HSSFont::faceChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** faceChanged unimplemented ****************************");
}

HSSObject::p HSSFont::getColor() { return this->color; }
void HSSFont::setDColor(HSSParserNode::p value){
    this->dColor = value;
    if (value->isA(HSSParserNodeTypeObjectDefinition)){
        HSSObjectDefinition::p dColor = boost::static_pointer_cast<HSSObjectDefinition>(value);
        this->color = dColor->getObject();
    }
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
            throw "unknown parser node type while setting value for HSSFont property";
            break;
    }
    
    return ret;
}