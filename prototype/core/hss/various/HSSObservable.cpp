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

#include "HSSObservable.h"
#include <iostream>
#include "../../axr/AXRDebugging.h"
#include <boost/unordered_map.hpp>

using namespace AXR;

std::string HSSObservable::observablePropertyStringRepresentation(HSSObservableProperty property){
    static std::string types[60];
    static bool HSSObservableHasInitializedTypes = false;
    if (!HSSObservableHasInitializedTypes) {
        HSSObservableHasInitializedTypes = true;
        //HSSObject
        types[HSSObservablePropertyIsA] = "HSSObservablePropertyIsA";
        
        //HSSDisplayObject
        types[HSSObservablePropertyValue] = "HSSObservablePropertyValue";
        types[HSSObservablePropertyWidth] = "HSSObservablePropertyWidth";
        types[HSSObservablePropertyHeight] = "HSSObservablePropertyHeight";
        types[HSSObservablePropertyAnchorX] = "HSSObservablePropertyAnchorX";
        types[HSSObservablePropertyAnchorY] = "HSSObservablePropertyAnchorY";
        types[HSSObservablePropertyFlow] = "HSSObservablePropertyFlow";
        types[HSSObservablePropertyOverflow] = "HSSObservablePropertyOverflow";
        types[HSSObservablePropertyAlignX] = "HSSObservablePropertyAlignX";
        types[HSSObservablePropertyAlignY] = "HSSObservablePropertyAlignY";
        types[HSSObservablePropertyBackground] = "HSSObservablePropertyBackground";
        types[HSSObservablePropertyContent] = "HSSObservablePropertyContent";
        types[HSSObservablePropertyOn] = "HSSObservablePropertyOn";
        types[HSSObservablePropertyMargin] = "HSSObservablePropertyMargin";
        types[HSSObservablePropertyPadding] = "HSSObservablePropertyPadding";
        types[HSSObservablePropertyBorder] = "HSSObservablePropertyBorder";
        types[HSSObservablePropertyFlagChanged] = "HSSObservablePropertyFlagChanged";
        
        //HSSContainer
        types[HSSObservablePropertyContentAlignX] = "HSSObservablePropertyContentAlignX";
        types[HSSObservablePropertyContentAlignY] = "HSSObservablePropertyContentAlignY";
        types[HSSObservablePropertyDirectionPrimary] = "HSSObservablePropertyDirectionPrimary";
        types[HSSObservablePropertyDirectionSecondary] = "HSSObservablePropertyDirectionSecondary";
        types[HSSObservablePropertyFont] = "HSSObservablePropertyFont";
        types[HSSObservablePropertyShape] = "HSSObservablePropertyShape";
        
        //HSSTextBlock
        types[HSSObservablePropertyTransform] = "HSSObservablePropertyTransform";
        types[HSSObservablePropertyTextAlign] = "HSSObservablePropertyTextAlign";
        types[HSSObservablePropertyText] = "HSSObservablePropertyText";
        
        //HSSRgb
        types[HSSObservablePropertyRed] = "HSSObservablePropertyRed";
        types[HSSObservablePropertyGreen] = "HSSObservablePropertyGreen";
        types[HSSObservablePropertyBlue] = "HSSObservablePropertyBlue";
        types[HSSObservablePropertyAlpha] = "HSSObservablePropertyAlpha";
        
        //HSSGradient
        types[HSSObservablePropertyStartColor] = "HSSObservablePropertyStartColor";
        types[HSSObservablePropertyEndColor] = "HSSObservablePropertyEndColor";
        types[HSSObservablePropertyBalance] = "HSSObservablePropertyBalance";
        types[HSSObservablePropertyColorStops] = "HSSObservablePropertyColorStops";
        
        //HSSLinearGradient
        types[HSSObservablePropertyStartX] = "HSSObservablePropertyStartX";
        types[HSSObservablePropertyStartY] = "HSSObservablePropertyStartY";
        types[HSSObservablePropertyEndX] = "HSSObservablePropertyEndX";
        types[HSSObservablePropertyEndY] = "HSSObservablePropertyEndY";
        
        //HSSColorStop
        types[HSSObservablePropertyPosition] = "HSSObservablePropertyPosition";
        
        //HSSFont
        types[HSSObservablePropertySize] = "HSSObservablePropertySize";
        types[HSSObservablePropertyFace] = "HSSObservablePropertyFace";
        types[HSSObservablePropertyColor] = "HSSObservablePropertyColor";
        types[HSSObservablePropertyWeight] = "HSSObservablePropertyWeight";
        
        //HSSRoundedRect
        types[HSSObservablePropertyCorners] = "HSSObservablePropertyCorners";
        
        //HSSEvent
        types[HSSObservablePropertyAction] = "HSSObservablePropertyAction";
        
        //HSSRequest
        types[HSSObservablePropertySrc] = "HSSObservablePropertySrc";
        types[HSSObservablePropertyTarget] = "HSSObservablePropertyTarget";
        types[HSSObservablePropertyMode] = "HSSObservablePropertyMode";
        types[HSSObservablePropertyContentTarget] = "HSSObservablePropertyContentTarget";
        
        //various
        types[HSSObservablePropertyTreeChange] = "HSSObservablePropertyTreeChange";
        
        //HSSPolygon
        types[HSSObservablePropertySides] = "HSSObservablePropertySides";
        types[HSSObservablePropertyAngle] = "HSSObservablePropertyAngle";
    }
    
    return types[property];
}

HSSObservableProperty HSSObservable::observablePropertyFromString(std::string name)
{
    static boost::unordered_map<std::string, HSSObservableProperty> properties;
    
    if (properties.size() == 0) {
        //HSSObject
        properties["isA"] = HSSObservablePropertyIsA;
        
        //HSSDisplayObject
        properties["value"] = HSSObservablePropertyValue;
        properties["width"] = HSSObservablePropertyWidth;
        properties["height"] = HSSObservablePropertyHeight;
        properties["anchorX"] = HSSObservablePropertyAnchorX;
        properties["anchorY"] = HSSObservablePropertyAnchorY;
        properties["flow"] = HSSObservablePropertyFlow;
        properties["overflow"] = HSSObservablePropertyOverflow;
        properties["alignX"] = HSSObservablePropertyAlignX;
        properties["alignY"] = HSSObservablePropertyAlignY;
        properties["background"] = HSSObservablePropertyBackground;
        properties["content"] = HSSObservablePropertyContent;
        properties["on"] = HSSObservablePropertyOn;
        
        //HSSContainer
        properties["contentAlignX"] = HSSObservablePropertyContentAlignX;
        properties["contentAlignY"] = HSSObservablePropertyContentAlignY;
        properties["directionPrimary"] = HSSObservablePropertyDirectionPrimary;
        properties["directionSecondary"] = HSSObservablePropertyDirectionSecondary;
        properties["font"] = HSSObservablePropertyFont;
        properties["shape"] = HSSObservablePropertyShape;
        properties["on"] = HSSObservablePropertyOn;
        properties["margin"] = HSSObservablePropertyMargin;
        properties["padding"] = HSSObservablePropertyPadding;
        properties["border"] = HSSObservablePropertyBorder;
        
        //HSSTextBlock
        properties["transform"] = HSSObservablePropertyTransform;
        properties["textAlign"] = HSSObservablePropertyTextAlign;
        properties["text"] = HSSObservablePropertyText;
        
        //HSSRgb
        properties["red"] = HSSObservablePropertyRed;
        properties["green"] = HSSObservablePropertyGreen;
        properties["blue"] = HSSObservablePropertyBlue;
        properties["alpha"] = HSSObservablePropertyAlpha;
        
        //HSSGradient
        properties["startColor"] = HSSObservablePropertyStartColor;
        properties["endColor"] = HSSObservablePropertyEndColor;
        properties["balance"] = HSSObservablePropertyBalance;
        properties["colorStops"] = HSSObservablePropertyColorStops;
        
        //HSSLinearGradient
        properties["startX"] = HSSObservablePropertyStartX;
        properties["startY"] = HSSObservablePropertyStartY;
        properties["endX"] = HSSObservablePropertyEndX;
        properties["endY"] = HSSObservablePropertyEndY;
        
        //HSSColorStop
        properties["position"] = HSSObservablePropertyPosition;
        
        //HSSFont
        properties["size"] = HSSObservablePropertySize;
        properties["face"] = HSSObservablePropertyFace;
        properties["color"] = HSSObservablePropertyColor;
        properties["weight"] = HSSObservablePropertyWeight;
        
        //HSSRoundedRect
        properties["corners"] = HSSObservablePropertyCorners;
        //HSSPolygon
        properties["sides"] = HSSObservablePropertySides;
        properties["angle"] = HSSObservablePropertyAngle;
        
        //HSSEvent
        properties["action"] = HSSObservablePropertyAction;
        
        //HSSRequest
        properties["src"] = HSSObservablePropertySrc;
        properties["target"] = HSSObservablePropertyTarget;
        properties["mode"] = HSSObservablePropertyMode;
        properties["contentTarget"] = HSSObservablePropertyContentTarget;
    }
    
    
    return properties[name];
}

HSSObservable::HSSObservable()
{
    
}

HSSObservable::~HSSObservable()
{
    this->_propertyObservers.clear();
}

void HSSObservable::observe(HSSObservableProperty target, HSSObservableProperty source, HSSObservable * object, HSSCallback *callback)
{
    //std_log1("added observer: "+object->name);
    std::size_t hash = 0;
    boost::hash_combine(hash, object);
    boost::hash_combine(hash, source);
    
    if(this->_propertyObservers.count(target) != 0){
        HSSObservable::observed &theObserved = this->_propertyObservers[target];
        theObserved[hash] = callback;
        axr_log(AXR_DEBUG_CH_OBSERVING, "added observer for "+HSSObservable::observablePropertyStringRepresentation(target));
    } else {
        HSSObservable::observed theObserved;
        theObserved[hash] = callback;
        this->_propertyObservers[target] = theObserved;
        axr_log(AXR_DEBUG_CH_OBSERVING, "added observer for new "+HSSObservable::observablePropertyStringRepresentation(target));
    }
}

void HSSObservable::removeObserver(HSSObservableProperty target, HSSObservableProperty source, HSSObservable * object)
{
    if(this->_propertyObservers.find(target) != this->_propertyObservers.end()){
        HSSObservable::observed &theObserved = this->_propertyObservers[target];
        std::size_t hash = 0;
        boost::hash_combine(hash, object);
        boost::hash_combine(hash, source);
        if(theObserved.count(hash) != 0){
            delete theObserved[hash];
            theObserved.erase(hash);
            axr_log(AXR_DEBUG_CH_OBSERVING, "removing observer for "+HSSObservable::observablePropertyStringRepresentation(target));
            return;
        }
    }
    std_log("####### tried to remove non existent observer for "+HSSObservable::observablePropertyStringRepresentation(target));
}

void HSSObservable::propertyChanged(HSSObservableProperty property, void *data)
{
    axr_log(AXR_DEBUG_CH_OBSERVING, "property changed");
}

void HSSObservable::notifyObservers(HSSObservableProperty property, void *data)
{
    HSSObservable::observed::iterator it;
    if(this->_propertyObservers.count(property) != 0){
        HSSObservable::observed &theObserved = this->_propertyObservers[property];
        for (it=theObserved.begin(); it != theObserved.end() ; it++) {
            HSSCallback * callback = (*it).second;
            if(data == NULL){
                axr_log(AXR_DEBUG_CH_OBSERVING, "data is null");
            }
            callback->call(property, data);
        }
    }
}
