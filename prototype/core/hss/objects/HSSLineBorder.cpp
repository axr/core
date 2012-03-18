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
 *      Core version: 0.45
 *      Revision: 9
 *
 ********************************************************************/

#include "HSSLineBorder.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"
#include "../parsing/HSSFunction.h"
#include "../parsing/HSSObjectDefinition.h"
#include "../../axr/AXRController.h"

using namespace AXR;

HSSLineBorder::HSSLineBorder()
:HSSBorder()
{
    std::vector<std::string> shorthandProperties;
    shorthandProperties.push_back("size");
    shorthandProperties.push_back("color");
    
    this->setShorthandProperties(shorthandProperties);
    this->registerProperty(HSSObservablePropertyColor, (void *) &this->color);
}

HSSLineBorder::~HSSLineBorder()
{
    
}

std::string HSSLineBorder::toString()
{
    if (this->isNamed()) {
        return std::string("HSSLineBorder: ").append(this->name);
    } else {
        return "Annonymous HSSLineBorder";
    }
}

std::string HSSLineBorder::defaultObjectType()
{
    return "lineBorder";
}

std::string HSSLineBorder::defaultObjectType(std::string property)
{
    if (property == "color"){
        return "rgb";
    } else if (property == "joins"){
        return "mitered";
    }  else if (property == "caps"){
        return "mitered";
    } else {
        return HSSBorder::defaultObjectType(property);
    }
}

bool HSSLineBorder::isKeyword(std::string value, std::string property)
{
    if (value == "rounded" || value == "projected"){
        if (property == "caps") {
            return true;
        }
    }
    
    //if we reached this far, let the superclass handle it
    return HSSBorder::isKeyword(value, property);
}

void HSSLineBorder::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyColor:
            this->setDColor(value);
            break;
        default:
            HSSBorder::setProperty(name, value);
            break;
    }
}

HSSObject::p HSSLineBorder::getColor() { return this->color; }
void HSSLineBorder::setDColor(HSSParserNode::p value){
    
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
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyColor, this, new HSSValueChangedCallback<HSSLineBorder>(this, &HSSLineBorder::colorChanged));
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        default:
            valid = false;
    }
    
    if(!valid)
        throw AXRWarning::p(new AXRWarning("HSSLineBorder", "Invalid value for color of "+this->name));
    
    this->notifyObservers(HSSObservablePropertyStartColor, &this->color);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSLineBorder::colorChanged(AXR::HSSObservableProperty source, void *data)
{
    std_log1("********************** colorChanged unimplemented ****************************");
}

void HSSLineBorder::draw(cairo_t * cairo)
{
    long double r = 0., g = 0., b = 0., a = 255.;
    if(this->color){
        r = this->color->getRed();
        g = this->color->getGreen();
        b = this->color->getBlue();
        a = this->color->getAlpha();
    }
    cairo_set_source_rgba(cairo, (r/255), (g/255), (b/255), (a/255));
    cairo_set_line_width(cairo, this->size);
    cairo_stroke(cairo);
}

