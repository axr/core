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
 *      Last changed: 2012/04/22
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 7
 *
 ********************************************************************/

#include "HSSRoundedRect.h"
#include "../../axr/errors/errors.h"
#include "../parsing/HSSNumberConstant.h"
#include "../parsing/HSSPercentageConstant.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSFunction.h"
#include <boost/pointer_cast.hpp>

using namespace AXR;

HSSRoundedRect::HSSRoundedRect()
: HSSShape()
{
    this->shapeType = HSSShapeTypeRoundedRect;
    this->cornerTL = this->cornerTR = this->cornerBR = this->cornerBL = 0.;
    this->observedTLCorner = this->observedTRCorner = this->observedBRCorner = this->observedBLCorner = NULL;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("corners");
    this->setShorthandProperties(shorthandProperties);
}

HSSRoundedRect::HSSRoundedRect(const HSSRoundedRect & orig)
: HSSShape(orig)
{
    this->cornerTL = this->cornerTR = this->cornerBR = this->cornerBL = 0.;
    this->observedTLCorner = this->observedTRCorner = this->observedBRCorner = this->observedBLCorner = NULL;
    
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("corners");
    this->setShorthandProperties(shorthandProperties);
}

HSSRoundedRect::p HSSRoundedRect::clone() const{
    return boost::static_pointer_cast<HSSRoundedRect, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSRoundedRect::cloneImpl() const{
    return HSSClonable::p(new HSSRoundedRect(*this));
}

HSSRoundedRect::~HSSRoundedRect()
{
    
}

std::string HSSRoundedRect::toString()
{
    return "HSSRoundedRect";
}

std::string HSSRoundedRect::defaultObjectType()
{
    return "roundedRect";
}

std::string HSSRoundedRect::defaultObjectType(std::string property)
{
    return HSSShape::defaultObjectType(property);
}

bool HSSRoundedRect::isKeyword(std::string value, std::string property)
{
    return HSSShape::isKeyword(value, property);
}


void HSSRoundedRect::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyCorners:
            this->setDCorners(value);
            break;
            
        default:
            HSSShape::setProperty(name, value);
            break;
    }
}

HSSMultipleValue::p HSSRoundedRect::getCorners() { return this->corners; }
void HSSRoundedRect::setDCorners(HSSParserNode::p value){
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
        {
            if(this->observedTLCorner != NULL)
            {
                this->observedTLCorner->removeObserver(this->observedTLCornerProperty, HSSObservablePropertyCorners, this);
            }
            this->cornerTL = this->_setLDProperty(
                                                  &HSSRoundedRect::cornerTLChanged,
                                                  value,
                                                  100.,
                                                  HSSObservablePropertyCorners,
                                                  this,
                                                  HSSObservablePropertyValue,
                                                  this->observedTLCorner,
                                                  this->observedTLCornerProperty,
                                                  NULL );
            if(this->observedTRCorner != NULL)
            {
                this->observedTRCorner->removeObserver(this->observedTRCornerProperty, HSSObservablePropertyCorners, this);
            }
            this->cornerTR = this->_setLDProperty(
                                                  &HSSRoundedRect::cornerTRChanged,
                                                  value,
                                                  100.,
                                                  HSSObservablePropertyCorners,
                                                  this,
                                                  HSSObservablePropertyValue,
                                                  this->observedTRCorner,
                                                  this->observedTRCornerProperty,
                                                  NULL );
            if(this->observedBRCorner != NULL)
            {
                this->observedBRCorner->removeObserver(this->observedBRCornerProperty, HSSObservablePropertyCorners, this);
            }
            this->cornerBR = this->_setLDProperty(
                                                  &HSSRoundedRect::cornerBRChanged,
                                                  value,
                                                  100.,
                                                  HSSObservablePropertyCorners,
                                                  this,
                                                  HSSObservablePropertyValue,
                                                  this->observedBRCorner,
                                                  this->observedBRCornerProperty,
                                                  NULL );
            if(this->observedBLCorner != NULL)
            {
                this->observedBLCorner->removeObserver(this->observedBLCornerProperty, HSSObservablePropertyCorners, this);
            }
            this->cornerBL = this->_setLDProperty(
                                                  &HSSRoundedRect::cornerBLChanged,
                                                  value,
                                                  100.,
                                                  HSSObservablePropertyCorners,
                                                  this,
                                                  HSSObservablePropertyValue,
                                                  this->observedBLCorner,
                                                  this->observedBLCornerProperty,
                                                  NULL );
            break;
        }
            
        default:
            throw AXRWarning::p(new AXRWarning("HSSRoundedRect", "Invalid value for corners of @roundedRect object "+this->name));
    }
    
    this->dCorners = value;
    HSSMultipleValue::p newCorners(new HSSMultipleValue());
    newCorners->add(value);
    this->corners = newCorners;
    
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerTLChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerTL = *(long double*)data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerTRChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerTR = *(long double*)data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerBRChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerBR = *(long double*)data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::cornerBLChanged(AXR::HSSObservableProperty source, void *data)
{
    this->cornerBL = *(long double*)data;
    this->notifyObservers(HSSObservablePropertyCorners, &this->corners);
}

void HSSRoundedRect::draw(cairo_t * cairo, double long x, double long y, double long width, double long height)
{
    double long degrees = M_PI / 180.0;
    
    cairo_new_sub_path (cairo);
    cairo_arc (cairo, x + width - this->cornerTR, y + this->cornerTR, this->cornerTR, -90 * degrees, 0 * degrees);
    cairo_arc (cairo, x + width - this->cornerBR, y + height - this->cornerBR, this->cornerBR, 0 * degrees, 90 * degrees);
    cairo_arc (cairo, x + cornerBL, y + height - this->cornerBL, this->cornerBL, 90 * degrees, 180 * degrees);
    cairo_arc (cairo, x + this->cornerTL, y + this->cornerTL, this->cornerTL, 180 * degrees, 270 * degrees);
    cairo_close_path (cairo);
}


long double HSSRoundedRect::_setLDProperty(
                                           void(HSSRoundedRect::*callback)(HSSObservableProperty property, void* data),
                                           HSSParserNode::p       value,
                                           long double            percentageBase,
                                           HSSObservableProperty  observedProperty,
                                           HSSObservable *        observedObject,
                                           HSSObservableProperty  observedSourceProperty,
                                           HSSObservable *        &observedStore,
                                           HSSObservableProperty  &observedStoreProperty,
                                           const std::vector<HSSDisplayObject::p> * scope
) {
    long double ret;
    
    HSSParserNodeType nodeType = value->getType();
    switch (nodeType) {
        case HSSParserNodeTypeNumberConstant:
        {
            HSSNumberConstant::p numberValue = boost::static_pointer_cast<HSSNumberConstant>(value);
            ret = numberValue->getValue();
            observedStore = NULL;
            break;
        }
            
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(value);
            ret = percentageValue->getValue(percentageBase);
            if(callback != NULL)
            {
                observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSRoundedRect>(this, callback));
                observedStore = observedObject;
                observedStoreProperty = observedProperty;
            }
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(value);
            expressionValue->setPercentageBase(percentageBase);
            expressionValue->setPercentageObserved(observedProperty, observedObject);
            expressionValue->setScope(scope);
            ret = expressionValue->evaluate();
            if(callback != NULL){
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSRoundedRect>(this, callback));
                observedStore = expressionValue.get();
                observedStoreProperty = HSSObservablePropertyValue;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
            
            break;
            
        case HSSParserNodeTypeFunctionCall:
        {
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            fnct->setPercentageBase(percentageBase);
            fnct->setPercentageObserved(observedProperty, observedObject);
            fnct->setScope(scope);
            
            ret = *(long double*)fnct->evaluate();
            if(callback != NULL){
                fnct->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSRoundedRect>(this, callback));
                observedStore = fnct.get();
                observedStoreProperty = HSSObservablePropertyValue;
            }
            break;
        }
            
        default:
            break;
    }
    
    return ret;
}

