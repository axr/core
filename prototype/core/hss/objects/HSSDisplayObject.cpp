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
 *      Last changed: 2012/06/11
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 43
 *
 ********************************************************************/

#include "HSSDisplayObject.h"
#include "../../AXR.h"
#include "../../axr/AXRDebugging.h"
#include "../../axr/errors/errors.h"
#include <math.h>
#include <boost/pointer_cast.hpp>
#include <cairo/cairo.h>
#include "../parsing/HSSParserNode.h"
#include "../parsing/HSSExpression.h"
#include "../parsing/HSSConstants.h"
#include "../parsing/HSSObjectDefinition.h"
#include "../parsing/HSSFunctions.h"
#include "HSSContainer.h"
#include <sstream>
#include <string>
#include "../../axr/AXRController.h"
#include "HSSRgb.h"
#include <cmath>


using namespace AXR;

HSSDisplayObject::HSSDisplayObject()
: HSSObject()
{
    this->initialize();
}

void HSSDisplayObject::initialize()
{
    this->type = HSSObjectTypeDisplayObject;
    
    this->_isDirty = true;
    this->_needsRereadRules = true;
    this->_needsSurface = true;
    this->_needsLayout = true;
    this->backgroundSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
    this->foregroundSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
    this->bordersSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 0, 0);
    
    this->x = this->y
    = this->globalX = this->globalY
    = this->width = this->height
    = this->anchorX = this->anchorY
    = this->alignX = this->alignY
    = 0.;
    
    this->drawIndex = this->_index = 0;
    this->tabIndex = this->zoomFactor = 1;
    this->flow = this->visible = true;
    this->overflow = false;
    /**
     *  @todo change to camelCase
     */
    this->does_float = false;
    this->heightByContent = false;
    this->_isRoot = false;
    
    this->elementName = std::string();
    this->contentText = std::string();
    
    this->observedWidth = this->observedHeight
    = this->observedAnchorX = this->observedAnchorY
    = this->observedAlignX = this->observedAlignY
    = this->observedBackground = this->observedContent = this->observedFont
    = NULL;
    
    this->borderBleeding = 0.;
    
    this->registerProperty(HSSObservablePropertyAlignX, (void *) &this->alignX);
    this->registerProperty(HSSObservablePropertyAlignY, (void *) &this->alignY);
    this->registerProperty(HSSObservablePropertyAnchorX, (void *) &this->anchorX);
    this->registerProperty(HSSObservablePropertyAnchorY, (void *) &this->anchorY);
    this->registerProperty(HSSObservablePropertyFlow, (void *) &this->flow);
    this->registerProperty(HSSObservablePropertyOverflow, (void *) &this->overflow);
    this->registerProperty(HSSObservablePropertyHeight, (void *) &this->height);
    this->registerProperty(HSSObservablePropertyWidth, (void *) &this->width);
    this->registerProperty(HSSObservablePropertyBackground, (void *) &this->background);
    this->registerProperty(HSSObservablePropertyContent, (void *) &this->content);
    this->registerProperty(HSSObservablePropertyFont, (void *) &this->font);
    this->registerProperty(HSSObservablePropertyOn, (void *) &this->on);
    this->registerProperty(HSSObservablePropertyBorder, (void *) &this->border);
    
    this->_isHover = false;
}

HSSDisplayObject::HSSDisplayObject(const HSSDisplayObject & orig)
: HSSObject(orig)
{
    this->initialize();
    this->attributes = orig.attributes;
    this->elementName = orig.elementName;
    this->contentText = orig.contentText;
    this->rules = orig.rules; //shallow copy
    this->_flagsStatus = orig._flagsStatus;
    
    this->drawIndex = orig.drawIndex;
    this->_index = orig._index;
    this->tabIndex = orig.tabIndex;
}

HSSDisplayObject::p HSSDisplayObject::clone() const{
    return boost::static_pointer_cast<HSSDisplayObject, HSSClonable>(this->cloneImpl());
}

HSSClonable::p HSSDisplayObject::cloneImpl() const{
    return HSSClonable::p(new HSSDisplayObject(*this));
}

HSSDisplayObject::~HSSDisplayObject()
{
    std_log1(std::string("destroying display object with name ").append(this->name));
    cairo_surface_destroy(this->backgroundSurface);
    cairo_surface_destroy(this->foregroundSurface);
    cairo_surface_destroy(this->bordersSurface);
}

std::string HSSDisplayObject::toString()
{
    if (this->isNamed()) {
        return std::string("HSSDisplayObject: ").append(this->name);
    } else {
        return "Annonymous HSSDisplayObject";
    }
}

std::string HSSDisplayObject::defaultObjectType()
{
    return "displayObject";
}

std::string HSSDisplayObject::defaultObjectType(std::string property)
{
    if (property == "margin"){
        return "margin";
    } else if (property == "border"){
        return "lineBorder";
    } else if (property == "background") {
        return "linearGradient";  
    } else if (property == "content") {
        return "image";
    } else if (property == "transform"){
        return "rotate";
    } else if (property == "effects"){
        return "shadow";
    } else if (property == "animation") {
        return "transition";
    } else if (property == "on") {
        return "click";
    } else if (property == "mask") {
        return "image";
    } else if (property == "font") {
        return "font";
    } else if (property == "on") {
        return "click";   
    } else {
        return HSSObject::defaultObjectType(property);
    }
}

bool HSSDisplayObject::isKeyword(std::string value, std::string property)
{
    if (   property == "anchorX"
        || property == "anchorY"
        || property == "alignX"
        || property == "alignY"
        ) {
        if (   value == "top"
            || value == "middle"
            || value == "center"
            || value == "bottom"
            || value == "left"
            || value == "right"
            ){
            return true;
        }
    } else if (property == "flow" || property == "overflow") {
        if (value == "yes" || value == "no"){
            return true;
        }
    }
    
//    else if (property == "font") {
//        if (   value == "thin"
//            || value == "ultralight"
//            || value == "light"
//            || value == "normal"
//            || value == "medium"
//            || value == "semibold"
//            || value == "bold"
//            || value == "ultrabold"
//            || value == "heavy"
//            || value == "ultraheavy"){
//            return true;
//        }
//    }
    
    //if we reached this far, let the superclass handle it
    return HSSObject::isKeyword(value, property);
}

bool HSSDisplayObject::canHaveChildren()
{
    return false;
}

boost::shared_ptr<HSSContainer> HSSDisplayObject::getParent()
{
    boost::shared_ptr<HSSContainer> parent = this->parent.lock();
    return parent;
}

void HSSDisplayObject::setParent(boost::shared_ptr<HSSContainer> parent)
{
    this->parent = pp(parent);
}

void HSSDisplayObject::removeFromParent()
{
    this->getParent()->remove(this->getIndex());
}

void HSSDisplayObject::setIndex(unsigned newIndex)
{
    this->_index = newIndex;
}

unsigned HSSDisplayObject::getIndex()
{
    return this->_index;
}

void HSSDisplayObject::attributesAdd(std::string name, std::string value)
{
    this->attributes[name] = value;
}

void HSSDisplayObject::attributesRemove(std::string name)
{
    this->attributes.erase(name);
}

std::string HSSDisplayObject::getContentText()
{
    return this->contentText;
}

void HSSDisplayObject::setContentText(std::string text)
{
    this->contentText = text;
}

void HSSDisplayObject::appendContentText(std::string text)
{
    this->contentText.append(text);
}

std::string HSSDisplayObject::getElementName()
{
    return this->elementName;
}

void HSSDisplayObject::setElementName(std::string newName)
{
    this->elementName = newName;
}

//rules
void HSSDisplayObject::rulesAdd(HSSRule::p newRule, HSSRuleState defaultState)
{
    //the target property is set to HSSObservablePropertyValue -- should this be something else?
    newRule->observe(HSSObservablePropertyValue, HSSObservablePropertyValue, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::ruleChanged));
    
    newRule->appliedToAdd(this->shared_from_this());
    
    HSSRuleStatus::p newRS = HSSRuleStatus::p(new HSSRuleStatus());
    newRS->state = defaultState;
    newRS->rule = newRule;
    
    this->rules.push_back(newRS);
    
    //iterate over the property defs and try to find an isA property
    const std::vector<HSSPropertyDefinition::p> & props = newRule->getProperties();
    HSSPropertyDefinition::const_it it;
    for (it = props.begin(); it!=props.end(); it++) {
        HSSPropertyDefinition::p propdef = *it;
        if(propdef->getName() == "isA"){
            this->rulesAddIsAChildren(propdef, defaultState);
        }
    }
    
}

void HSSDisplayObject::rulesAddIsAChildren(HSSPropertyDefinition::p propdef, HSSRuleState defaultState)
{
    HSSParserNode::p value = propdef->getValue();
    
    switch (value->getType()) {
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                
                if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer::p thisCont = boost::static_pointer_cast<HSSContainer>(this->shared_from_this());
                    this->axrController->currentContext.push(thisCont);
                    HSSRule::const_it it;
                    const std::deque<HSSRule::p> rules = objdef->getRules();
                    for (it=rules.begin(); it!=rules.end(); it++) {
                        HSSRule::p clonedRule = (*it)->clone();
                        this->axrController->recursiveMatchRulesToDisplayObjects(clonedRule, thisCont->getChildren(), thisCont, true);
                    }
                    this->axrController->currentContext.pop();
                }
            } catch (AXRError::p e) {
                e->raise();
            }
            
            break;
        }
            
        default:
        {
            break;
        }
    }
}

HSSRule::p HSSDisplayObject::rulesGet(unsigned index)
{
    return this->rules[index]->rule;
}

void HSSDisplayObject::rulesRemove(unsigned index)
{
    this->rules.erase(this->rules.begin()+index);
}

void HSSDisplayObject::rulesRemoveLast()
{
    this->rules.pop_back();
}

const int HSSDisplayObject::rulesSize()
{
    return this->rules.size();
}

void HSSDisplayObject::setRuleStatus(HSSRule::p rule, HSSRuleState newValue)
{
    bool changed = false;
    bool found = false;

    std::vector<HSSRuleStatus::p>::iterator it;
    for (it=this->rules.begin(); it!=this->rules.end(); it++) {
        HSSRuleStatus::p rs = *it;
        HSSRule::p existingRule = rs->rule;
        if(existingRule.get()==rule.get()){
            found = true;
            if(newValue == HSSRuleStatePurge){
                if(rs->state == HSSRuleStateOn){
                    rs->state = newValue;
                    changed = true;
                    break;
                }
            } else {
                if(rs->state != newValue){
                    rs->state = newValue;
                    changed = true;
                    break;
                }
            }
        }
    }
    
    if(!found){
        this->rulesAdd(rule, newValue);
        changed = true;
    }
    
    if(changed){
        this->setNeedsRereadRules(true);
        AXRCore::getInstance()->getWrapper()->setNeedsDisplay(true);
    }
}

void HSSDisplayObject::readDefinitionObjects()
{
    if(this->_needsRereadRules){
        unsigned i, j;
    
        this->setDefaults();
        
        //if this is root, we use the window width and height of the render
        if(this->isRoot()){
            AXRCore::tp & core = AXRCore::getInstance();
            //width
            HSSNumberConstant::p newDWidth(new HSSNumberConstant(core->getRender()->getWindowWidth()));
            this->setDWidth(newDWidth);
            //height
            HSSNumberConstant::p newDHeight(new HSSNumberConstant(core->getRender()->getWindowHeight()));
            this->setDHeight(newDHeight);
        }
        
        std::string propertyName;
        for (i=0; i<this->rules.size(); i++) {
            HSSRuleStatus::p & ruleStatus = this->rules[i];
            switch (ruleStatus->state) {
                case HSSRuleStateActivate:
                {
                    ruleStatus->state = HSSRuleStateOn;
                    //fall through
                }
                    
                case HSSRuleStateOn:
                {
                    HSSRule::p & rule = ruleStatus->rule;
                    for (j=0; j<rule->propertiesSize(); j++) {
                        try
                        {
                            HSSPropertyDefinition::p& propertyDefinition = rule->propertiesGet(j);
                            propertyName = propertyDefinition->getName();
                            this->setPropertyWithName(propertyName, propertyDefinition->getValue());
                        }
                        catch (AXRError::p e){
                            e->raise();
                        }
                    }
                    break;
                }
                
                case HSSRuleStatePurge:
                {
                    ruleStatus->state = HSSRuleStateOff;
                    break;
                }
                    
                default:
                    break;
            }
        }
        
        this->_needsRereadRules = false;
    }
}

void HSSDisplayObject::setProperty(HSSObservableProperty name, HSSParserNode::p value)
{
    switch (name) {
        case HSSObservablePropertyWidth:
            this->setDWidth(value);
            break;
        case HSSObservablePropertyHeight:
            this->setDHeight(value);
            break;
        case HSSObservablePropertyAnchorX:
            this->setDAnchorX(value);
            break;
        case HSSObservablePropertyAnchorY:
            this->setDAnchorY(value);
            break;
        case HSSObservablePropertyFlow:
            this->setDFlow(value);
            break;
        case HSSObservablePropertyOverflow:
            this->setDOverflow(value);
            break;
        case HSSObservablePropertyAlignX:
            this->setDAlignX(value);
            break;
        case HSSObservablePropertyAlignY:
            this->setDAlignY(value);
            break;
        case HSSObservablePropertyBackground:
            this->setDBackground(value);
            break;
        case HSSObservablePropertyContent:
            this->setDContent(value);
            break;
        case HSSObservablePropertyFont:
            this->setDFont(value);
            break;
        case HSSObservablePropertyOn:
            this->setDOn(value);
            break;
        case HSSObservablePropertyBorder:
            this->setDBorder(value);
            break;
            
        default:
            HSSObject::setProperty(name, value);
            break;
    }
}

//deprecated I think, use the previous one
void HSSDisplayObject::setProperty(HSSObservableProperty name, void * value)
{
    switch (name) {
        case HSSObservablePropertyWidth:
            this->width = *(long double*) value;
            break;
        case HSSObservablePropertyHeight:
            this->height = *(long double*) value;
            break;
        case HSSObservablePropertyAnchorX:
            this->anchorX = *(long double*) value;
            break;
        case HSSObservablePropertyAnchorY:
            this->anchorY = *(long double*) value;
            break;
        case HSSObservablePropertyAlignX:
            this->alignX = *(long double*) value;
            break;
        case HSSObservablePropertyAlignY:
            this->alignY = *(long double*) value;
            break;
        case HSSObservablePropertyBackground:
        case HSSObservablePropertyContent:
        case HSSObservablePropertyFont:
        case HSSObservablePropertyBorder:
            /**
             *  @todo to be implemented
             */
            return;
            
        case HSSObservablePropertyOn:
            std_log("lakjdflad");
            
        default:
            return HSSObject::setProperty(name, value);
    }
    
    this->notifyObservers(name, value);
}

void HSSDisplayObject::setNeedsRereadRules(bool value)
{
    this->_needsRereadRules = value;
}

bool HSSDisplayObject::needsRereadRules()
{
    return this->_needsRereadRules;
}

void HSSDisplayObject::recursiveReadDefinitionObjects()
{
    this->readDefinitionObjects();
}

void HSSDisplayObject::regenerateSurfaces()
{
    if(this->_needsSurface){
        cairo_surface_destroy(this->backgroundSurface);
        cairo_surface_destroy(this->foregroundSurface);
        if(this->border.size() > 0)
            cairo_surface_destroy(this->bordersSurface);
        this->backgroundSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, ceil(this->width), ceil(this->height));
        this->foregroundSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, ceil(this->width), ceil(this->height));
        
        //get the sum of all sizes
        if(this->border.size() > 0){
            HSSBorder::it it;
            this->borderBleeding = 0.;
            for (it=this->border.begin(); it!=this->border.end(); it++) {
                this->borderBleeding += (*it)->getSize();
            }
            
            this->bordersSurface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, ceil(this->width + this->borderBleeding + this->borderBleeding), ceil(this->height + this->borderBleeding + this->borderBleeding));
        }
        
        this->setDirty(true);
        this->_needsSurface = false;
#if AXR_DEBUG_LEVEL > 0
        std::ostringstream wstr;
        wstr << this->width;
        std::ostringstream hstr;
        hstr << this->height;
        std_log1("created a new surface width:"+wstr.str()+" height:"+hstr.str());
#endif
    }
}

void HSSDisplayObject::recursiveRegenerateSurfaces()
{
    this->regenerateSurfaces();
}


void HSSDisplayObject::setNeedsSurface(bool value)
{
    this->_needsSurface = value;
}

bool HSSDisplayObject::needsSurface()
{
    return this->_needsSurface;
}


void HSSDisplayObject::setDirty(bool value)
{
    //std_log1("setting "+this->name+" dirty");
    this->_isDirty = value;
}

bool HSSDisplayObject::isDirty()
{
    return this->_isDirty;
}

void HSSDisplayObject::draw(cairo_t * cairo)
{
    std_log1("drawing "+this->elementName);
    if(this->_isDirty){
#if AXR_DEBUG_LEVEL > 0
        std::ostringstream xStream;
        xStream << this->x;
        std::ostringstream yStream;
        yStream << this->y;
        
        std_log1("redrawing contents of "+this->elementName+" with x: "+xStream.str()+" and y: "+yStream.str());
#endif
        this->_isDirty = false;
        this->drawBackground();
        this->drawForeground();
        if(this->border.size() > 0){
            this->drawBorders();
        }
    }
    
    cairo_set_source_surface(cairo, this->backgroundSurface, this->globalX, this->globalY);
    cairo_paint(cairo);
    cairo_set_source_surface(cairo, this->foregroundSurface, this->globalX, this->globalY);
    cairo_paint(cairo);
    if(this->border.size() > 0){
        cairo_set_source_surface(cairo, this->bordersSurface, this->globalX - this->borderBleeding, this->globalY - this->borderBleeding);
        cairo_paint(cairo);
    }
}

void HSSDisplayObject::drawBackground()
{
    cairo_t * cairo = cairo_create(this->backgroundSurface);
    cairo_rectangle(cairo, 0., 0., this->width, this->height);
    this->_drawBackground(cairo);
    cairo_destroy(cairo);
}

void HSSDisplayObject::_drawBackground(cairo_t * cairo)
{
    std::vector<HSSObject::p>::iterator it;
    for (it=this->background.begin(); it!= this->background.end(); it++) {
        HSSObject::p theobj = *it;
        switch (theobj->getType()) {
            case HSSObjectTypeRgb:
            {
                HSSRgb::p color = boost::static_pointer_cast<HSSRgb>(theobj);
                
                long double r = 0., g = 0., b = 0., a = 0;
                if(color){
                    r = color->getRed();
                    g = color->getGreen();
                    b = color->getBlue();
                    a = color->getAlpha();
                }
                
                cairo_set_source_rgba(cairo, (r/255), (g/255), (b/255), (a/255));
                cairo_fill_preserve(cairo);
                
                break;
            }
                
            case HSSObjectTypeGradient:
            {
                HSSLinearGradient::p grad = boost::static_pointer_cast<HSSLinearGradient>(theobj);
                grad->draw(cairo);
                break;
            }
                                
            default:
                break;
        }
    }
}

void HSSDisplayObject::drawForeground()
{
//    cairo_t * cairo = cairo_create(this->foregroundSurface);
//    
//    PangoLayout *layout;
//    PangoFontDescription *font_description;
//    
//    //FIXME: precalculate this layout somehow earlier, to get the height for the container
//    //based on the text height
//    layout = pango_cairo_create_layout (cairo);
//    pango_layout_set_width(layout, this->width * PANGO_SCALE);
//    
//    font_description = pango_font_description_new ();
//    
//    if (this->font){
//        pango_font_description_set_family (font_description, this->font->getFace().c_str());
//        
//        //FIXME: add support for weights in fonts
//        pango_font_description_set_weight (font_description, PANGO_WEIGHT_NORMAL);
//        pango_font_description_set_absolute_size (font_description, this->font->getSize() * PANGO_SCALE);
//        
//        if (this->font->getColor()){
//            HSSRgb::p textColor = boost::static_pointer_cast<HSSRgb>(this->font->getColor());
//            cairo_set_source_rgb (cairo, textColor->getRed()/255, textColor->getGreen()/255, textColor->getBlue()/255);
//        } else {
//            cairo_set_source_rgb (cairo, 0, 0, 0);
//        }
//        
//    } else {
//        pango_font_description_set_family (font_description, "monospace");
//        pango_font_description_set_weight (font_description, PANGO_WEIGHT_NORMAL);
//        pango_font_description_set_absolute_size (font_description, 12 * PANGO_SCALE);
//        cairo_set_source_rgb (cairo, 0, 0, 0);
//    }
//    
//    pango_layout_set_font_description (layout, font_description);
//    pango_layout_set_text (layout, this->getContentText().c_str(), -1);
//    
//    
//    cairo_move_to (cairo, 0, 0);
//    pango_cairo_show_layout (cairo, layout);
//    
//    g_object_unref (layout);
//    pango_font_description_free (font_description);
    
    
    
    
    
    
//    cairo_t * cairo = cairo_create(this->foregroundSurface);
//    cairo_select_font_face (cairo, "sans-serif", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
//    cairo_set_font_size (cairo, 12.0);
//    cairo_set_source_rgb (cairo, 0.0, 0.0, 0.0);
//    cairo_move_to (cairo, 0.0, 12.0);
//    std_log1(this->getContentText());
//    cairo_show_text (cairo, this->getContentText().c_str());
    
//    cairo_destroy(cairo);
}

void HSSDisplayObject::drawBorders()
{
//    cairo_t * cairo = cairo_create(this->bordersSurface);
//    cairo_rectangle(cairo, 0., 0., this->width, this->height);
//    cairo_set_source_rgb(cairo, 0,0,0);
//    cairo_stroke(cairo);
//    
//    cairo_destroy(cairo);
}

void HSSDisplayObject::recursiveDraw(cairo_t * cairo)
{
    this->draw(cairo);
}

//layout
void HSSDisplayObject::setNeedsLayout(bool value)
{
    this->_needsLayout = value;
}

const bool HSSDisplayObject::needsLayout() const
{
    return this->_needsLayout;
}

void HSSDisplayObject::layout()
{
    //do nothing
}

void HSSDisplayObject::recursiveLayout()
{
    this->layout();
}


void HSSDisplayObject::setGlobalX(long double newValue)
{
    this->globalX = newValue;
}


void HSSDisplayObject::setGlobalY(long double newValue)
{
    this->globalY = newValue;
}

//width
long double HSSDisplayObject::getWidth() { return this->width; }
HSSParserNode::p HSSDisplayObject::getDWidth()  {   return this->dWidth; }
void HSSDisplayObject::setDWidth(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for width of "+this->getElementName()));
    }
    
    
    if(value->isA(HSSParserNodeTypeKeywordConstant)){
        
    } else {
        
        HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
        if(this->observedWidth != NULL)
        {
            this->observedWidth->removeObserver(this->observedWidthProperty, HSSObservablePropertyWidth, this);
        }
        this->dWidth = value;
        HSSContainer::p parentContainer = this->getParent();
        if(parentContainer){
            this->width = floor(this->_setLDProperty(
                                               &HSSDisplayObject::widthChanged,
                                               value,
                                               parentContainer->width,
                                               observedProperty,
                                               parentContainer.get(),
                                               HSSObservablePropertyWidth,
                                               this->observedWidth,
                                               this->observedWidthProperty,
                                               &(parentContainer->getChildren())
                                               ));
        } else {
            this->width = floor(this->_setLDProperty(
                                               NULL,
                                               value,
                                               0,
                                               observedProperty,
                                               this,
                                               HSSObservablePropertyWidth,
                                               this->observedWidth,
                                               this->observedWidthProperty,
                                               NULL
                                               ));
        }
        
        
        this->notifyObservers(HSSObservablePropertyWidth, &this->width);
        this->setNeedsSurface(true);
        this->setNeedsLayout(true);
        this->setDirty(true);
    }
}

void HSSDisplayObject::widthChanged(HSSObservableProperty source, void*data)
{
    HSSParserNodeType nodeType = this->dWidth->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p widthValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dWidth);
            this->width = floor(widthValue->getValue(*(long double*)data));
            break;
        }
        
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p widthExpression = boost::static_pointer_cast<HSSExpression>(this->dWidth);
            this->width = floor(widthExpression->evaluate());
            break;
        }
        
        case HSSParserNodeTypeFunctionCall:
        {
            this->width = floor(*(long double*)data);
        }
        
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyWidth, &this->width);
    this->setNeedsSurface(true);
    this->setNeedsLayout(true);
    this->setDirty(true);
}

//height
long double HSSDisplayObject::getHeight() { return this->height; }
HSSParserNode::p HSSDisplayObject::getDHeight() { return this->dHeight; }
void HSSDisplayObject::setDHeight(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for height of "+this->getElementName()));
    }
    
    
    if(value->isA(HSSParserNodeTypeKeywordConstant)){
        this->heightByContent = true;
        
    } else {
        if(this->observedHeight != NULL)
        {
            this->observedHeight->removeObserver(this->observedHeightProperty, HSSObservablePropertyHeight, this);
            this->observedHeight = NULL;
        }
        this->dHeight = value;
        this->heightByContent = false;
        
        HSSObservableProperty observedProperty;

        observedProperty = HSSObservablePropertyHeight;
        HSSContainer::p parentContainer = this->getParent();
        if(parentContainer){
            this->height = this->_setLDProperty(
                                                &HSSDisplayObject::heightChanged,
                                                value,
                                                parentContainer->height,
                                                observedProperty,
                                                parentContainer.get(),
                                                HSSObservablePropertyHeight,
                                                this->observedHeight,
                                                this->observedHeightProperty,
                                                &(parentContainer->getChildren())
                                                );
        } else {
            this->height = this->_setLDProperty(
                                                NULL,
                                                value,
                                                150,
                                                observedProperty,
                                                this,
                                                HSSObservablePropertyHeight,
                                                this->observedHeight,
                                                this->observedHeightProperty,
                                                NULL
                                                );
        }
        
        this->notifyObservers(HSSObservablePropertyHeight, &this->height);
        this->setNeedsSurface(true);
        this->setNeedsLayout(true);
        this->setDirty(true);
    }
}

void HSSDisplayObject::heightChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dHeight->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p heightValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dHeight);
            this->height = heightValue->getValue(*(long double*)data);
            break;
        }
        
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p heightExpression = boost::static_pointer_cast<HSSExpression>(this->dHeight);
            this->height = heightExpression->evaluate();
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            this->height = *(long double*)data;
            break;
        }
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->height = *(long double*)data;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyHeight, &this->height);
    this->setNeedsSurface(true);
    this->setNeedsLayout(true);
    this->setDirty(true);
}

//anchorX
long double HSSDisplayObject::getAnchorX() { return this->anchorX; }
HSSParserNode::p HSSDisplayObject::getDAnchorX() { return this->dAnchorX; }
void HSSDisplayObject::setDAnchorX(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for anchorX of "+this->getElementName()));
    }
    
    this->dAnchorX = value;
    HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
    if(this->observedAnchorX != NULL)
    {
        this->observedAnchorX->removeObserver(this->observedAnchorXProperty, HSSObservablePropertyAnchorX, this);
    }
    HSSContainer::p parentContainer = this->getParent();
    const std::vector<HSSDisplayObject::p> * scope;
    if(parentContainer){
        scope = &(parentContainer->getChildren());
    } else {
        scope = NULL;
    }
    this->anchorX = this->_setLDProperty(
                                         &HSSDisplayObject::anchorXChanged,
                                         value,
                                         this->width,
                                         observedProperty,
                                         this,
                                         HSSObservablePropertyAnchorX,
                                         this->observedAnchorX,
                                         this->observedAnchorXProperty,
                                         scope
                                         );
    this->notifyObservers(HSSObservablePropertyAnchorX, &this->anchorX);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}

void HSSDisplayObject::anchorXChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dAnchorX->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dAnchorX);
            this->anchorX = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(this->dAnchorX);
            this->anchorX = expressionValue->evaluate();
            break;
        }
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->anchorX = *(long double*)data;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyAnchorX, &this->anchorX);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}


//anchorY
long double HSSDisplayObject::getAnchorY() { return this->anchorY; }
HSSParserNode::p HSSDisplayObject::getDAnchorY() { return this->dAnchorY; }
void HSSDisplayObject::setDAnchorY(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for anchorY of "+this->getElementName()));
    }
    
    if(this->observedAnchorY != NULL)
    {
        this->observedAnchorY->removeObserver(this->observedAnchorYProperty, HSSObservablePropertyAnchorY, this);
        this->observedAnchorY = NULL;
    }
    this->dAnchorY = value;
    HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
    
    HSSContainer::p parentContainer = this->getParent();
    const std::vector<HSSDisplayObject::p> * scope;
    if(parentContainer){
        scope = &(parentContainer->getChildren());
    } else {
        scope = NULL;
    }
    this->anchorY = this->_setLDProperty(
                                         &HSSDisplayObject::anchorYChanged,
                                         value,
                                         this->height,
                                         observedProperty,
                                         this,
                                         HSSObservablePropertyAnchorY,
                                         this->observedAnchorY,
                                         this->observedAnchorYProperty,
                                         scope
                                         );
    this->notifyObservers(HSSObservablePropertyAnchorY, &this->anchorY);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}

void HSSDisplayObject::anchorYChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dAnchorY->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dAnchorY);
            this->anchorY = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(this->dAnchorY);
            this->anchorY = expressionValue->evaluate();
            break;
        }
        
        case HSSParserNodeTypeFunctionCall:
        {
            this->anchorY = *(long double*)data;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyAnchorY, &this->anchorY);
#if AXR_DEBUG_LEVEL > 0
    this->setDirty(true);
#endif
}


//flow
bool HSSDisplayObject::getFlow() { return this->flow; }
HSSParserNode::p HSSDisplayObject::getDFlow() { return this->dFlow; }
void HSSDisplayObject::setDFlow(HSSParserNode::p value)
{
    bool valid = true;
    
    switch (value->getType()) {
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dFlow = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            objdef->setScope(this->scope);
            objdef->apply();
            HSSObject::p theobj = objdef->getObject();
            if (theobj && theobj->isA(HSSObjectTypeValue)) {
                this->flow = (bool)boost::static_pointer_cast<HSSValue>(theobj)->getIntValue();
            } else {
                valid = false;
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            this->dFlow = value;
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                this->setDFlow(objdef);
                
            } catch (HSSObjectNotFoundException * e) {
                std_log(e->toString());
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->dFlow = value;
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                fnct->setScope(this->scope);
                this->flow = *(bool *)fnct->evaluate();
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyFlow, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::flowChanged));
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            HSSKeywordConstant::p keywordValue = boost::static_pointer_cast<HSSKeywordConstant>(value);
            if(keywordValue->getValue() == "yes"){
                this->flow = true;
                break;
            } else if (keywordValue->getValue() == "no"){
                this->flow = false;
                break;
            } else {
                //fall through
            }
        }
            
        default:
            valid = false;
    }
    
    if(!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for flow of "+this->name));
    
    this->notifyObservers(HSSObservablePropertyFlow, &this->flow);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSDisplayObject::flowChanged(HSSObservableProperty source, void*data)
{
    switch (this->dFlow->getType()) {
        case HSSParserNodeTypeFunctionCall:
            this->flow = *(bool*)data;
            break;
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyFlow, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

//overflow
bool HSSDisplayObject::getOverflow() { return this->overflow; }
HSSParserNode::p HSSDisplayObject::getDOverflow() { return this->dOverflow; }
void HSSDisplayObject::setDOverflow(HSSParserNode::p value)
{
    bool valid = true;
    
    switch (value->getType()) {
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dOverflow = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            objdef->setScope(this->scope);
            objdef->apply();
            HSSObject::p theobj = objdef->getObject();
            if (theobj && theobj->isA(HSSObjectTypeValue)) {
                this->overflow = (bool)boost::static_pointer_cast<HSSValue>(theobj)->getIntValue();
            } else {
                valid = false;
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            this->dOverflow = value;
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                this->setDOverflow(objdef);
                
            } catch (HSSObjectNotFoundException * e) {
                std_log(e->toString());
            }
            
            break;
        }
            
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->dOverflow = value;
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                fnct->setScope(this->scope);
                this->overflow = *(bool *)fnct->evaluate();
                
                fnct->observe(HSSObservablePropertyValue, HSSObservablePropertyOverflow, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::overflowChanged));
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            HSSKeywordConstant::p keywordValue = boost::static_pointer_cast<HSSKeywordConstant>(value);
            if(keywordValue->getValue() == "yes"){
                this->overflow = true;
                break;
            } else if (keywordValue->getValue() == "no"){
                this->overflow = false;
                break;
            } else {
                //fall through
            }
        }
            
        default:
            valid = false;
    }
    
    if(!valid)
        throw AXRWarning::p(new AXRWarning("HSSDGradient", "Invalid value for overflow of "+this->name));
    
    this->notifyObservers(HSSObservablePropertyOverflow, &this->overflow);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

void HSSDisplayObject::overflowChanged(HSSObservableProperty source, void*data)
{
    switch (this->dOverflow->getType()) {
        case HSSParserNodeTypeFunctionCall:
            this->overflow = *(bool*)data;
            break;
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyOverflow, data);
    this->notifyObservers(HSSObservablePropertyValue, NULL);
}

//alignX
long double HSSDisplayObject::getAlignX() { return this->alignX; }
HSSParserNode::p HSSDisplayObject::getDAlignX() { return this->dAlignX; }
void HSSDisplayObject::setDAlignX(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for alignX of "+this->getElementName()));
    }
    
    if(this->observedAlignX != NULL)
    {
        this->observedAlignX->removeObserver(this->observedAlignXProperty, HSSObservablePropertyAlignX, this);
        this->observedAlignX = NULL;
    }
    this->dAlignX = value;
    
    if(value->isA(HSSParserNodeTypeKeywordConstant)){
        
        HSSKeywordConstant::p keywordValue = boost::static_pointer_cast<HSSKeywordConstant>(value);
        if(keywordValue->getValue() == "auto"){
            HSSContainer::p parentContainer = this->getParent();
            if(parentContainer){
                this->alignX = parentContainer->contentAlignX;
                parentContainer->observe(HSSObservablePropertyContentAlignX, HSSObservablePropertyAlignX, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::alignXChanged));
                this->observedAlignX = parentContainer.get();
                this->observedAlignXProperty = HSSObservablePropertyContentAlignX;
            } else {
                this->alignX = 0;
            }
//            this->alignX = 0;
            
        } else if (keywordValue->getValue() == "left"){
            this->setDAlignX(HSSParserNode::p(new HSSNumberConstant(0)));
        } else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center"){
            this->setDAlignX(HSSParserNode::p(new HSSPercentageConstant(50)));
        } else if (keywordValue->getValue() == "right"){
            this->setDAlignX(HSSParserNode::p(new HSSPercentageConstant(100)));
        } else {
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for alignX of "+this->getElementName()));
        }
        
    } else {
        HSSObservableProperty observedProperty = HSSObservablePropertyWidth;
        HSSContainer::p parentContainer = this->getParent();
        if(parentContainer){
            this->alignX = this->_setLDProperty(
                                               &HSSDisplayObject::alignXChanged,
                                               value,
                                               parentContainer->width,
                                               observedProperty,
                                               parentContainer.get(),
                                               HSSObservablePropertyAlignX,
                                               this->observedAlignX,
                                               this->observedAlignXProperty,
                                               &(parentContainer->getChildren())
                                               );
            parentContainer->setNeedsLayout(true);
        } else {
            this->alignX = this->_setLDProperty(
                                               NULL,
                                               value,
                                               0,
                                               observedProperty,
                                               this,
                                               HSSObservablePropertyAlignX,
                                               this->observedAlignX,
                                               this->observedAlignXProperty,
                                               NULL
                                               );
        }
    }
    
    this->notifyObservers(HSSObservablePropertyAlignX, &this->alignX);
}

void HSSDisplayObject::alignXChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dAlignX->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dAlignX);
            this->alignX = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(this->dAlignX);
            this->alignX = expressionValue->evaluate();
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            this->alignX = *(long double*)data;
        }
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->alignX = *(long double*)data;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyAlignX, &this->alignX);
}

//alignY
long double HSSDisplayObject::getAlignY() { return this->alignY; }
HSSParserNode::p HSSDisplayObject::getDAlignY() { return this->dAlignX; }
void HSSDisplayObject::setDAlignY(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeNumberConstant:
        case HSSParserNodeTypePercentageConstant:
        case HSSParserNodeTypeExpression:
        case HSSParserNodeTypeKeywordConstant:
        case HSSParserNodeTypeFunctionCall:
            break;
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for alignY of "+this->getElementName()));
    }
    
    if(this->observedAlignY != NULL)
    {
        this->observedAlignY->removeObserver(this->observedAlignYProperty, HSSObservablePropertyAlignY, this);
        this->observedAlignY = NULL;
    }
    this->dAlignY = value;
    
    if(value->isA(HSSParserNodeTypeKeywordConstant)){
        
        HSSKeywordConstant::p keywordValue = boost::static_pointer_cast<HSSKeywordConstant>(value);
        if(keywordValue->getValue() == "auto"){
            HSSContainer::p parentContainer = this->getParent();
            if(parentContainer){
                this->alignY = parentContainer->contentAlignY;
                parentContainer->observe(HSSObservablePropertyContentAlignY, HSSObservablePropertyAlignY, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::alignYChanged));
                this->observedAlignY = parentContainer.get();
                this->observedAlignYProperty = HSSObservablePropertyContentAlignY;
            } else {
                this->alignY = 0;
            }
            //            this->alignY = 0;
        } else if (keywordValue->getValue() == "top"){
            this->setDAlignY(HSSParserNode::p(new HSSNumberConstant(0)));
        } else if (keywordValue->getValue() == "middle" || keywordValue->getValue() == "center"){
            this->setDAlignY(HSSParserNode::p(new HSSPercentageConstant(50)));
        } else if (keywordValue->getValue() == "bottom"){
            this->setDAlignY(HSSParserNode::p(new HSSPercentageConstant(100)));
        } else {
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for alignY of "+this->getElementName()));
        }
        
    } else {
        HSSObservableProperty observedProperty = HSSObservablePropertyHeight;
        HSSContainer::p parentContainer = this->getParent();
        if(parentContainer){
            this->alignY = this->_setLDProperty(
                                                &HSSDisplayObject::alignYChanged,
                                                value,
                                                parentContainer->height,
                                                observedProperty,
                                                parentContainer.get(),
                                                HSSObservablePropertyAlignY,
                                                this->observedAlignY,
                                                this->observedAlignYProperty,
                                                &(parentContainer->getChildren())
                                                );
            parentContainer->setNeedsLayout(true);
        } else {
            this->alignY = this->_setLDProperty(
                                               NULL,
                                               value,
                                               0,
                                               observedProperty,
                                               this,
                                               HSSObservablePropertyAlignY,
                                               this->observedAlignY,
                                               this->observedAlignYProperty,
                                               NULL
                                               );
        }
    }
    
    this->notifyObservers(HSSObservablePropertyAlignY, &this->alignY);
}

void HSSDisplayObject::alignYChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dAlignY->getType();
    switch (nodeType) {
        case HSSParserNodeTypePercentageConstant:
        {
            HSSPercentageConstant::p percentageValue = boost::static_pointer_cast<HSSPercentageConstant>(this->dAlignY);
            this->alignY = percentageValue->getValue(*(long double*)data);
            break;
        }
            
        case HSSParserNodeTypeExpression:
        {
            HSSExpression::p expressionValue = boost::static_pointer_cast<HSSExpression>(this->dAlignY);
            this->alignY = expressionValue->evaluate();
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            this->alignY = *(long double*)data;
        }
            
        case HSSParserNodeTypeFunctionCall:
        {
            this->alignY = *(long double*)data;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyAlignY, &this->alignY);
}


//background
HSSParserNode::p HSSDisplayObject::getDBackground() { return this->dBackground; }
void HSSDisplayObject::setDBackground(HSSParserNode::p value)
{
    this->background.clear();
    this->dBackground = value;
    this->addDBackground(value);
}


void HSSDisplayObject::addDBackground(HSSParserNode::p value)
{
    bool valid = true;
    
    switch (value->getType()) {
        case HSSParserNodeTypeMultipleValueDefinition:
        {
            HSSParserNode::it iterator;
            HSSMultipleValueDefinition::p multiDef = boost::static_pointer_cast<HSSMultipleValueDefinition>(value);
            std::vector<HSSParserNode::p> values = multiDef->getValues();
            for (iterator = values.begin(); iterator != values.end(); iterator++) {
                this->addDBackground(*iterator);
            }
            break;
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                this->addDBackground(this->axrController->objectTreeGet(objname->getValue()));
                
            } catch (AXRError::p e) {
                e->raise();
            }
            break;
        }
            
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dBackground = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            if (objdef->getObject()->isA(HSSObjectTypeRgb) || objdef->getObject()->isA(HSSObjectTypeGradient)) {
                HSSContainer::p parent = this->getParent();
                if(parent){
                    objdef->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    objdef->setScope(&(thisCont->getChildren()));
                }
                objdef->setThisObj(this->shared_from_this());
                objdef->apply();
                HSSObject::p theObj = objdef->getObject();
                theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyBackground, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::backgroundChanged));
                this->background.push_back(theObj);
            } else {
                valid = false;
            }
            
            break;
        }
            
        case HSSParserNodeTypeFunctionCall:
        {
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                
                HSSContainer::p parent = this->getParent();
                if(parent){
                    fnct->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    fnct->setScope(&(thisCont->getChildren()));
                }
                HSSParserNode::p remoteValue = *(HSSParserNode::p *)fnct->evaluate();
                if(remoteValue){
                    try {
                        this->addDBackground(remoteValue);
                    } catch (AXRError::p e) {
                        e->raise();
                    }
                }
                
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            if(boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue() == "none"){
                //ignore
            } else {
                valid = false;
            }
            break;
        }
            
        default:
            valid = false;
            break;
    }
    
    if(!valid){
        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for background of "+this->getElementName()));
    }
    
    this->notifyObservers(HSSObservablePropertyBackground, &this->background);
}


void HSSDisplayObject::backgroundChanged(HSSObservableProperty source, void*data)
{
    
}

//content
HSSParserNode::p HSSDisplayObject::getDContent() { return this->dContent; }
void HSSDisplayObject::setDContent(HSSParserNode::p value)
{
    this->content.clear();
    this->dContent = value;
    this->addDContent(value);
}


void HSSDisplayObject::addDContent(HSSParserNode::p value)
{
    bool valid = true;
    
    switch (value->getType()) {
        case HSSParserNodeTypeMultipleValueDefinition:
        {
            HSSParserNode::it iterator;
            HSSMultipleValueDefinition::p multiDef = boost::static_pointer_cast<HSSMultipleValueDefinition>(value);
            std::vector<HSSParserNode::p> values = multiDef->getValues();
            for (iterator = values.begin(); iterator != values.end(); iterator++) {
                this->addDContent(*iterator);
            }
            break;
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                this->addDContent(this->axrController->objectTreeGet(objname->getValue()));
                
            } catch (AXRError::p e) {
                e->raise();
            }
            break;
        }
            
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dContent = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            if (objdef->getObject()->isA(HSSObjectTypeRgb) || objdef->getObject()->isA(HSSObjectTypeGradient)) {
                HSSContainer::p parent = this->getParent();
                if(parent){
                    objdef->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    objdef->setScope(&(thisCont->getChildren()));
                }
                objdef->setThisObj(this->shared_from_this());
                objdef->apply();
                HSSObject::p theObj = objdef->getObject();
                theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyContent, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::contentChanged));
                this->content.push_back(theObj);
            } else {
                valid = false;
            }
            
            break;
        }
            
        case HSSParserNodeTypeFunctionCall:
        {
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                
                HSSContainer::p parent = this->getParent();
                if(parent){
                    fnct->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    fnct->setScope(&(thisCont->getChildren()));
                }
                HSSParserNode::p remoteValue = *(HSSParserNode::p *)fnct->evaluate();
                if(remoteValue){
                    try {
                        this->addDContent(remoteValue);
                    } catch (AXRError::p e) {
                        e->raise();
                    }
                }
                
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            if(boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue() == "none"){
                //ignore
            } else {
                valid = false;
            }
            break;
        }
            
        case HSSParserNodeTypeStringConstant:
        {
            this->setContentText(boost::static_pointer_cast<HSSStringConstant>(value)->getValue());
            break;
        }
            
        default:
            valid = false;
            break;
    }
    
    if(!valid){
        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for content of "+this->getElementName()));
    }
    
    this->notifyObservers(HSSObservablePropertyContent, &this->content);
}


void HSSDisplayObject::contentChanged(HSSObservableProperty source, void*data)
{
    std_log("unimplemented yet");
}

const HSSParserNode::p HSSDisplayObject::getDFont() const { return this->dFont; }
void HSSDisplayObject::setDFont(HSSParserNode::p value)
{
    this->font.clear();
    this->dFont = value;
    this->addDFont(value);
}

void HSSDisplayObject::addDFont(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeMultipleValueDefinition:
        {
            HSSParserNode::it iterator;
            HSSMultipleValueDefinition::p multiDef = boost::static_pointer_cast<HSSMultipleValueDefinition>(value);
            std::vector<HSSParserNode::p> values = multiDef->getValues();
            for (iterator = values.begin(); iterator != values.end(); iterator++) {
                this->addDFont(*iterator);
            }
            break;
        }
            
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dFont = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            if (objdef->getObject()->isA(HSSObjectTypeFont)) {
                HSSContainer::p parent = this->getParent();
                if(parent){
                    objdef->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    objdef->setScope(&(thisCont->getChildren()));
                }
                
                objdef->apply();
                HSSObject::p theObj = objdef->getObject();
                theObj->observe(HSSObservablePropertyFont, HSSObservablePropertyFont, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::fontChanged));
                this->font.push_back(boost::static_pointer_cast<HSSFont>(theObj));
            }
            
            break;
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                objdef->apply();
                
                HSSObject::p obj = boost::static_pointer_cast<HSSObject>(objdef->getObject());
                switch (obj->getObjectType()) {
                    case HSSObjectTypeFont:
                        this->font.push_back(boost::static_pointer_cast<HSSFont>(obj));
                        break;
                        
                    default:
                        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for font of "+this->getElementName()));
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
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                
                HSSContainer::p parent = this->getParent();
                if(parent){
                    fnct->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    fnct->setScope(&(thisCont->getChildren()));
                }
                HSSParserNode::p remoteValue = *(HSSParserNode::p *)fnct->evaluate();
                this->addDFont(remoteValue);
                
            } else {
                throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid function type for font of "+this->getElementName()));
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            std::string kwValue = boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue();
            if(kwValue == "inherit"){
                if(this->observedFont != NULL){
                    this->observedFont->removeObserver(this->observedFontProperty, HSSObservablePropertyFont, this);
                }
                HSSContainer::p parent = this->getParent();
                this->font = *(std::vector<HSSFont::p> *) parent->getProperty(HSSObservablePropertyFont);
                parent->observe(HSSObservablePropertyFont, HSSObservablePropertyFont, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::fontChanged));
                
            } else {
                AXRWarning::p(new AXRWarning("HSSDisplayObject", "Unknown keyword value for font of "+this->getElementName()))->raise();
            }
            break;
        }
            
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for font of "+this->getElementName()));
    }
    
    this->notifyObservers(HSSObservablePropertyFont, &this->font);
}

void HSSDisplayObject::fontChanged(HSSObservableProperty source, void *data)
{
    HSSParserNodeType nodeType = this->dFont->getType();
    switch (nodeType) {
        case HSSParserNodeTypeObjectDefinition:
        case HSSParserNodeTypeObjectNameConstant:
        case HSSParserNodeTypeFunctionCall:
        case HSSParserNodeTypeKeywordConstant:
        {
            this->font = *(std::vector<HSSFont::p> *) data;
            this->setDirty(true);
            break;
        }
            
        default:
            break;
    }
    
    this->notifyObservers(HSSObservablePropertyFont, &this->font);
}


//on
HSSParserNode::p HSSDisplayObject::getDOn() { return this->dOn; }
void HSSDisplayObject::setDOn(HSSParserNode::p value)
{
    this->on.clear();
    this->dOn = value;
    this->addDOn(value);
}


void HSSDisplayObject::addDOn(HSSParserNode::p value)
{
    bool valid = true;
    
    switch (value->getType()) {
        case HSSParserNodeTypeMultipleValueDefinition:
        {
            HSSParserNode::it iterator;
            HSSMultipleValueDefinition::p multiDef = boost::static_pointer_cast<HSSMultipleValueDefinition>(value);
            std::vector<HSSParserNode::p> values = multiDef->getValues();
            for (iterator = values.begin(); iterator != values.end(); iterator++) {
                this->addDOn(*iterator);
            }
            break;
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                this->addDOn(this->axrController->objectTreeGet(objname->getValue()));
                
            } catch (AXRError::p e) {
                e->raise();
            }
            break;
        }
            
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dOn = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            if (objdef->getObject()->isA(HSSObjectTypeEvent)) {
                HSSContainer::p parent = this->getParent();
                if(parent){
                    objdef->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    objdef->setScope(&(thisCont->getChildren()));
                }

                objdef->setThisObj(this->shared_from_this());
                objdef->apply();
                HSSObject::p theObj = objdef->getObject();
                
                HSSEvent::p theEvent = boost::static_pointer_cast<HSSEvent>(objdef->getObject());
                HSSEventType eventType = theEvent->getEventType();
                if(this->on.find(eventType) == this->on.end()){
                    std::vector<HSSObject::p>newVector;
                    this->on[eventType] = newVector;
                }
                this->on[eventType].push_back(theEvent);
                
                theEvent->observe(HSSObservablePropertyValue, HSSObservablePropertyOn, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::onChanged));
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        case HSSParserNodeTypeFunctionCall:
        {
            HSSFunction::p fnct = boost::static_pointer_cast<HSSFunction>(value);
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                HSSContainer::p parent = this->getParent();
                if(parent){
                    fnct->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    fnct->setScope(&(thisCont->getChildren()));
                }
                fnct->setThisObj(this->shared_from_this());
                HSSParserNode::p remoteValue = *(HSSParserNode::p *)fnct->evaluate();
                if(remoteValue){
                    try {
                        this->addDOn(remoteValue);
                    } catch (AXRError::p e) {
                        e->raise();
                    }
                }
                
            } else {
                valid = false;
            }
            
            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            if(boost::static_pointer_cast<HSSKeywordConstant>(value)->getValue() == "none"){
                //ignore
            } else {
                valid = false;
            }
            break;
        }
            
        default:
            valid = false;
            break;
    }
    
    if(!valid){
        throw AXRWarning::p(new AXRWarning("HSSGradient", "Invalid value for colorStops of "+this->getName()));
    }
    
    this->notifyObservers(HSSObservablePropertyOn, &this->on);
}


void HSSDisplayObject::onChanged(HSSObservableProperty source, void*data)
{
    
}

bool HSSDisplayObject::fireEvent(HSSEventType type)
{
    bool fired = false;
    if(this->on.find(type) != this->on.end()){
        std::vector<HSSObject::p> events = this->on[type];
        HSSObject::it it;
        for (it=events.begin(); it!=events.end(); it++) {
            if((*it)->isA(HSSObjectTypeEvent)){
                boost::static_pointer_cast<HSSEvent>(*it)->fire();
                fired = true;
            }
        }
    }
    return fired;
}

const HSSParserNode::p HSSDisplayObject::getDBorder() const { return this->dBorder; }
void HSSDisplayObject::setDBorder(HSSParserNode::p value)
{
    this->border.clear();
    this->dBorder = value;
    this->addDBorder(value);
}

void HSSDisplayObject::addDBorder(HSSParserNode::p value)
{
    switch (value->getType()) {
        case HSSParserNodeTypeMultipleValueDefinition:
        {
            HSSParserNode::it iterator;
            HSSMultipleValueDefinition::p multiDef = boost::static_pointer_cast<HSSMultipleValueDefinition>(value);
            std::vector<HSSParserNode::p> values = multiDef->getValues();
            for (iterator = values.begin(); iterator != values.end(); iterator++) {
                this->addDBorder(*iterator);
            }
            break;
        }
            
        case HSSParserNodeTypeObjectDefinition:
        {
            this->dBorder = value;
            HSSObjectDefinition::p objdef = boost::static_pointer_cast<HSSObjectDefinition>(value);
            if (objdef->getObject()->isA(HSSObjectTypeBorder)) {
                HSSContainer::p parent = this->getParent();
                if(parent){
                    objdef->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    objdef->setScope(&(thisCont->getChildren()));
                }
                
                objdef->apply();
                HSSObject::p theObj = objdef->getObject();
                theObj->observe(HSSObservablePropertyValue, HSSObservablePropertyTarget, this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::borderChanged));
                this->border.push_back(boost::static_pointer_cast<HSSBorder>(theObj));
            }
            
            break;
        }
            
        case HSSParserNodeTypeObjectNameConstant:
        {
            try {
                HSSObjectNameConstant::p objname = boost::static_pointer_cast<HSSObjectNameConstant>(value);
                HSSObjectDefinition::p objdef = this->axrController->objectTreeGet(objname->getValue());
                objdef->apply();
                
                HSSObject::p obj = boost::static_pointer_cast<HSSObject>(objdef->getObject());
                switch (obj->getObjectType()) {
                    case HSSObjectTypeBorder:
                        this->border.push_back(boost::static_pointer_cast<HSSBorder>(obj));
                        break;
                        
                    default:
                        throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for border of "+this->getElementName()));
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
            if(fnct && fnct->isA(HSSFunctionTypeRef)){
                
                HSSContainer::p parent = this->getParent();
                if(parent){
                    fnct->setScope(&(parent->getChildren()));
                } else if(this->isA(HSSObjectTypeContainer)){
                    HSSContainer * thisCont = static_cast<HSSContainer *>(this);
                    fnct->setScope(&(thisCont->getChildren()));
                }
                HSSParserNode::p remoteValue = *(HSSParserNode::p *)fnct->evaluate();
                this->addDBorder(remoteValue);
                
            } else {
                throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid function type for border of "+this->getElementName()));
            }

            break;
        }
            
        case HSSParserNodeTypeKeywordConstant:
        {
            HSSKeywordConstant::p keywordValue = boost::static_pointer_cast<HSSKeywordConstant>(value);
            if (keywordValue->getValue() == "none"){
                break;
            }
        }
        
        default:
            throw AXRWarning::p(new AXRWarning("HSSDisplayObject", "Invalid value for border of "+this->getElementName()));
    }
}

void HSSDisplayObject::borderChanged(HSSObservableProperty source, void*data)
{
    
}


//defaults
void HSSDisplayObject::setDefaults()
{
    //width
    HSSPercentageConstant::p newDWidth(new HSSPercentageConstant(100.));
    this->setDWidth(newDWidth);
    //height
    HSSKeywordConstant::p newDHeight(new HSSKeywordConstant("content"));
    this->setDHeight(newDHeight);
    //anchorX
    HSSPercentageConstant::p newDAnchorX(new HSSPercentageConstant(50));
    this->setDAnchorX(newDAnchorX);
    //anchorY
    HSSPercentageConstant::p newDAnchorY(new HSSPercentageConstant(50));
    this->setDAnchorY(newDAnchorY);
    //flow
    HSSKeywordConstant::p newDFlow(new HSSKeywordConstant("yes"));
    this->setDFlow(newDFlow);
    //alignX
    HSSKeywordConstant::p newDAlignX(new HSSKeywordConstant("auto"));
    this->setDAlignX(newDAlignX);
    //alignY
    HSSKeywordConstant::p newDAlignY(new HSSKeywordConstant("auto"));
    this->setDAlignY(newDAlignY);
    //background
    HSSKeywordConstant::p newDBackground(new HSSKeywordConstant("none"));
    this->setDBackground(newDBackground);
    //content
    HSSKeywordConstant::p newDContent(new HSSKeywordConstant("none"));
    this->setDContent(newDContent);
    //on
    HSSKeywordConstant::p newDOn(new HSSKeywordConstant("none"));
    this->setDOn(newDOn);
    //border
    HSSKeywordConstant::p newDBorder(new HSSKeywordConstant("none"));
    this->setDBorder(newDBorder);
}


long double HSSDisplayObject::_setLDProperty(
                                             void(HSSDisplayObject::*callback)(HSSObservableProperty property, void* data),
                                             HSSParserNode::p       value,
                                             long double            percentageBase,
                                             HSSObservableProperty  observedProperty,
                                             HSSObservable *        observedObject,
                                             HSSObservableProperty  observedSourceProperty,
                                             HSSObservable *        &observedStore,
                                             HSSObservableProperty  &observedStoreProperty,
                                             const std::vector<HSSDisplayObject::p> * scope
                                             )
{
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
                observedObject->observe(observedProperty, observedSourceProperty, this, new HSSValueChangedCallback<HSSDisplayObject>(this, callback));
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
            expressionValue->setThisObj(this->shared_from_this());
            ret = expressionValue->evaluate();
            if(callback != NULL){
                expressionValue->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSDisplayObject>(this, callback));
                observedStore = expressionValue.get();
                observedStoreProperty = HSSObservablePropertyValue;
            } else {
                observedStore = NULL;
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
            fnct->setThisObj(this->shared_from_this());
            
            ret = *(long double*)fnct->evaluate();
            if(callback != NULL){
                fnct->observe(HSSObservablePropertyValue, observedSourceProperty, this, new HSSValueChangedCallback<HSSDisplayObject>(this, callback));
                observedStore = fnct.get();
                observedStoreProperty = HSSObservablePropertyValue;
            } else {
                observedStore = NULL;
            }
            break;
        }
        
        default:
            break;
    }
    
    return ret;
}


bool HSSDisplayObject::handleEvent(HSSEventType type, void* data)
{
    switch (type) {
        case HSSEventTypeMouseDown:
        case HSSEventTypeMouseUp:
        case HSSEventTypeClick:
        {
            HSSPoint thePoint = *(HSSPoint*)data;
            
            if(     this->globalX < thePoint.x && this->globalX + this->width >= thePoint.x
               &&   this->globalY < thePoint.y && this->globalY + this->height >= thePoint.y){
               
                //std_log(this->getElementName());
                return this->fireEvent(type);
            }
            break;
        }
            
        case HSSEventTypeLoad:
        {
            return this->fireEvent(type);
        }
            
            
        case HSSEventTypeMouseMove:
        {
            HSSPoint thePoint = *(HSSPoint*)data;
            
            if(     this->globalX < thePoint.x && this->globalX + this->width >= thePoint.x
               &&   this->globalY < thePoint.y && this->globalY + this->height >= thePoint.y){
                
                if(!this->_isHover)
                {
                    this->setHover(true);
                }
                //std_log(this->getElementName());
                return this->fireEvent(type);
            } else if (this->_isHover) {
                this->setHover(false);
            }
            break;
        }
            
        default:
            throw AXRError::p(new AXRError("HSSDisplayObject", "Unknown event type"));
    }
    
    return false;
}

void HSSDisplayObject::setHover(bool newValue)
{
    if(this->_isHover != newValue)
    {
        this->_isHover = newValue;
        if(newValue == TRUE){
            this->flagsActivate("hover");
        } else {
            this->flagsDeactivate("hover");
        }
    }
}

bool HSSDisplayObject::isHover()
{
    return this->_isHover;
}

void HSSDisplayObject::ruleChanged(HSSObservableProperty source, void*data)
{
    //HSSRule * theRule = (HSSRule*)data;
    this->setNeedsRereadRules(true);
    AXRCore::getInstance()->getWrapper()->setNeedsDisplay(true);
}

void HSSDisplayObject::createFlag(HSSFlag::p flag, HSSRuleState defaultValue)
{
    this->_flagsStatus[flag->getName()] = defaultValue;
    this->_flags[flag->getName()].push_back(flag);
}

bool HSSDisplayObject::hasFlag(std::string name)
{
    if(this->_flagsStatus.find(name) != this->_flagsStatus.end()){
        return true;
    }
    return false;
}
        
HSSRuleState HSSDisplayObject::flagState(std::string name)
{
    if(this->_flagsStatus.find(name) != this->_flagsStatus.end()){
        return this->_flagsStatus[name];
    }
    return HSSRuleStateOff;
}

void HSSDisplayObject::flagsActivate(std::string name)
{
    if(this->hasFlag(name)){
        std_log3("activate flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = HSSRuleStateActivate;
        std::vector<HSSFlag::p> flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        std::vector<HSSFlag::p>::iterator it;
        for(it=flags.begin(); it!=flags.end(); it++){
            (*it)->flagChanged(newValue);    
        }   
        this->_flagsStatus[name] = HSSRuleStateOn;
    } else {
        //std_log("No flag with name "+name+" on element "+this->getElementName());
    }
}

void HSSDisplayObject::flagsDeactivate(std::string name)
{
    if(name == "*"){
        boost::unordered_map<std::string, std::vector<HSSFlag::p> >::const_iterator it;
        std_log3("deactivating all flags on element "+this->getElementName());
        for (it=this->_flags.begin(); it!=this->_flags.end(); it++) {
            HSSRuleState newValue = HSSRuleStatePurge;
            std::vector<HSSFlag::p> flags = it->second;
            this->_flagsStatus[it->first] = newValue;
            std::vector<HSSFlag::p>::iterator it;
            for(it=flags.begin(); it!=flags.end(); it++){
                (*it)->flagChanged(newValue);    
            }

        }
        
        
    } else if(this->hasFlag(name)){
        std_log3("deactivate flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = HSSRuleStatePurge;
        std::vector<HSSFlag::p> flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        std::vector<HSSFlag::p>::iterator it;
        for(it=flags.begin(); it!=flags.end(); it++){
            (*it)->flagChanged(newValue);    
        }
        this->_flagsStatus[name] = HSSRuleStateOff;
        
    } else {
        //std_log("No flag with name "+name+" on element "+this->getElementName());
    }
}

void HSSDisplayObject::flagsToggle(std::string name)
{
    if(this->hasFlag(name)){
        std_log3("toggle flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = (this->_flagsStatus[name] == HSSRuleStateOn ? HSSRuleStatePurge : HSSRuleStateActivate);
        std::vector<HSSFlag::p> flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        std::vector<HSSFlag::p>::iterator it;
        for(it=flags.begin(); it!=flags.end(); it++){
            (*it)->flagChanged(newValue);    
        }
        this->_flagsStatus[name] = (newValue == HSSRuleStateActivate ? HSSRuleStateOn : HSSRuleStateOff);
    } else {
        //std_log("No flag with name "+name+" on element "+this->getElementName());
    }
}

bool HSSDisplayObject::isRoot()
{
    return this->_isRoot;
}

void HSSDisplayObject::setRoot(bool newValue)
{
    this->_isRoot = newValue;
}


HSSDisplayObject::p HSSDisplayObject::shared_from_this()
{
    return boost::static_pointer_cast<HSSDisplayObject>(HSSObject::shared_from_this());
}
