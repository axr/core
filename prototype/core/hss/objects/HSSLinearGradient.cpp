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
 *      Last changed: 2011/12/24
 *      HSS version: 1.0
 *      Core version: 0.43
 *      Revision: 1
 *
 ********************************************************************/

#include "HSSLinearGradient.h"
#include "../parsing/HSSObjectDefinition.h"

using namespace AXR;

HSSLinearGradient::HSSLinearGradient()
:HSSGradient()
{
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("startX");
    shorthandProperties.push_back("startY");
    shorthandProperties.push_back("endX");
    shorthandProperties.push_back("endY");
    shorthandProperties.push_back("colorStops");
    shorthandProperties.push_back("balance");
    this->setShorthandProperties(shorthandProperties);
    
    this->registerProperty(HSSObservablePropertyStartX, (void *) &this->startX);
    this->registerProperty(HSSObservablePropertyStartY, (void *) &this->startY);
    this->registerProperty(HSSObservablePropertyEndX, (void *) &this->startY);
    this->registerProperty(HSSObservablePropertyEndY, (void *) &this->startY);
    
    this->startX = this->startY = this->endX = endY = 0.;
    
    this->observedStartX = this->observedStartY
    = this->observedEndX = this->observedEndY
    = NULL;
}

HSSLinearGradient::~HSSLinearGradient()
{
    
}

std::string HSSLinearGradient::toString()
{
    if (this->isNamed()) {
        return std::string("HSSLinearGradient: ").append(this->name);
    } else {
        return "Annonymous HSSLinearGradient";
    }
}

std::string HSSLinearGradient::defaultObjectType()
{
    return "linearGradient";
}

std::string HSSLinearGradient::defaultObjectType(std::string property)
{
//    if (property == "bla"){
//        
//    } else {
        return HSSGradient::defaultObjectType(property);
//    }
}

bool HSSLinearGradient::isKeyword(std::string value, std::string property)
{
    if (value == "top" || value == "bottom"){
        if (property == "startX") {
            return true;
        }
    } else if(value == "left" || value == "right"){
        if (property == "startY") {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSGradient::isKeyword(value, property);
}

void HSSLinearGradient::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyStartX:
            this->setDStartX(value);
            break;
        case HSSObservablePropertyStartY:
            this->setDStartY(value);
            break;
        case HSSObservablePropertyEndX:
            this->setDEndX(value);
            break;
        case HSSObservablePropertyEndY:
            this->setDEndY(value);
            break;
        default:
            HSSGradient::setProperty(name, value);
            break;
    }
}


long double HSSLinearGradient::getStartX() { return this->startX; }
HSSParserNode::p HSSLinearGradient::getDStartX() { return this->dStartX; }
void HSSLinearGradient::setDStartX(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        {
            this->dStartX = value;
            
            HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
            if(this->observedStartX != NULL)
            {
                this->observedStartX->removeObserver(this->observedStartXProperty, HSSObservablePropertyStartX, this);
            }
            
            this->startX = this->_setLDProperty(
                                                &HSSLinearGradient::startXChanged,
                                                value,
                                                this->getThisObj()->getWidth(),
                                                observedProperty,
                                                this->getThisObj().get(),
                                                HSSObservablePropertyStartX,
                                                this->observedStartX,
                                                this->observedStartXProperty
                                                );
            
            break;
        }
            
        default:
            break;
    }
}

void HSSLinearGradient::startXChanged(HSSObservableProperty source, void *data)
{
    switch (this->dStartX->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->startX = *(long double*)data;
            break;
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyStartX, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}


long double HSSLinearGradient::getStartY() { return this->startY; }
HSSParserNode::p HSSLinearGradient::getDStartY() { return this->dStartY; }
void HSSLinearGradient::setDStartY(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        {
            this->dStartY = value;
            
            HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
            if(this->observedStartY != NULL)
            {
                this->observedStartY->removeObserver(this->observedStartYProperty, HSSObservablePropertyStartY, this);
            }
            
            this->startY = this->_setLDProperty(
                                                &HSSLinearGradient::startYChanged,
                                                value,
                                                this->getThisObj()->getHeight(),
                                                observedProperty,
                                                this->getThisObj().get(),
                                                HSSObservablePropertyStartY,
                                                this->observedStartY,
                                                this->observedStartYProperty
                                                );
            
            break;
        }
            
        default:
            break;
    }
}


void HSSLinearGradient::startYChanged(HSSObservableProperty source, void *data)
{
    switch (this->dStartY->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->startY = *(long double*)data;
            break;
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyStartY, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSLinearGradient::getEndX() { return this->endX; }
HSSParserNode::p HSSLinearGradient::getDEndX() { return this->dEndX; }
void HSSLinearGradient::setDEndX(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        {
            this->dEndX = value;
            
            HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
            if(this->observedEndX != NULL)
            {
                this->observedEndX->removeObserver(this->observedEndXProperty, HSSObservablePropertyEndX, this);
            }
            
            this->endX = this->_setLDProperty(
                                              &HSSLinearGradient::endXChanged,
                                              value,
                                              this->getThisObj()->getWidth(),
                                              observedProperty,
                                              this->getThisObj().get(),
                                              HSSObservablePropertyEndX,
                                              this->observedEndX,
                                              this->observedEndXProperty
                                              );
            
            break;
        }
            
        default:
            break;
    }
}

void HSSLinearGradient::endXChanged(HSSObservableProperty source, void *data)
{
    switch (this->dEndX->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->endX = *(long double*)data;
            break;
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyEndX, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}


long double HSSLinearGradient::getEndY() { return this->endY; }
HSSParserNode::p HSSLinearGradient::getDEndY() { return this->dEndY; }
void HSSLinearGradient::setDEndY(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        {
            this->dEndY = value;
            
            HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
            if(this->observedEndY != NULL)
            {
                this->observedEndY->removeObserver(this->observedEndYProperty, HSSObservablePropertyEndY, this);
            }
            
            this->endY = this->_setLDProperty(
                                              &HSSLinearGradient::endYChanged,
                                              value,
                                              this->getThisObj()->getHeight(),
                                              observedProperty,
                                              this->getThisObj().get(),
                                              HSSObservablePropertyEndY,
                                              this->observedEndY,
                                              this->observedEndYProperty
                                              );
            
            break;
        }
            
        default:
            break;
    }
}


void HSSLinearGradient::endYChanged(HSSObservableProperty source, void *data)
{
    switch (this->dEndY->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeFunctionCall:
            this->endY = *(long double*)data;
            break;
            
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dEndY);
            this->endY = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyEndY, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

long double HSSLinearGradient::_setLDProperty(
                                        void(HSSLinearGradient::*callback)(HSSObservableProperty property, void* data),
                                        HSSParserNode::p       value,
                                        long double            percentageBase,
                                        HSSObservableProperty  observedProperty,
                                        HSSObservable *        observedObject,
                                        HSSObservableProperty  observedSourceProperty,
                                        HSSObservable *        &observedStore,
                                        HSSObservableProperty  &observedStoreProperty
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
            if(callback != NULL)
            {
                observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSLinearGradient>(this, callback));
                observedStore = observedObject;
                observedStoreProperty = observedProperty;
            }
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(value);
            expressionValue->setPercentageBase(percentageBase);
            expressionValue->setScope(this->scope);
            ret = expressionValue->evaluate();
            if(callback != NULL){
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSLinearGradient>(this, callback));
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
            
            break;
            
        default:
            AXRWarning::p(new AXRWarning("HSSLinearGradient", "Unknown parser node type while setting value for HSSLinearGradient property"))->raise();
            break;
    }
    
    return ret;
}

void HSSLinearGradient::draw(cairo_t * cairo)
{
    cairo_pattern_t * pat = cairo_pattern_create_linear (this->startX, this->startY, this->endX, this->endY);
    cairo_pattern_add_color_stop_rgba (pat, 0., this->startColor->getRed()/255., this->startColor->getGreen()/255., this->startColor->getBlue()/255., this->startColor->getAlpha()/255.);
    //add color stops
    cairo_pattern_add_color_stop_rgba (pat, 1., this->endColor->getRed()/255., this->endColor->getGreen()/255., this->endColor->getBlue()/255., this->endColor->getAlpha()/255.);
    cairo_set_source (cairo, pat);
    cairo_fill_preserve (cairo);
    cairo_pattern_destroy (pat);
}





