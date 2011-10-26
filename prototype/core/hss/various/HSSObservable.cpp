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
 *      Last changed: 2011/10/23
 *      HSS version: 1.0
 *      Core version: 0.4
 *      Revision: 7
 *
 ********************************************************************/

#include "HSSObservable.h"
#include <iostream>
#include "../../axr/AXRDebugging.h"
#include <boost/unordered_map.hpp>

using namespace AXR;

std::string HSSObservable::observablePropertyStringRepresentation(HSSObservableProperty property){
    static std::string types[30];
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
        types[HSSObservablePropertyAlignX] = "HSSObservablePropertyAlignX";
        types[HSSObservablePropertyAlignY] = "HSSObservablePropertyAlignY";
        
        //HSSContainer
        types[HSSObservablePropertyContentAlignX] = "HSSObservablePropertyContentAlignX";
        types[HSSObservablePropertyContentAlignY] = "HSSObservablePropertyContentAlignY";
        types[HSSObservablePropertyDirectionPrimary] = "HSSObservablePropertyDirectionPrimary";
        types[HSSObservablePropertyDirectionSecondary] = "HSSObservablePropertyDirectionSecondary";
        types[HSSObservablePropertyBackground] = "HSSObservablePropertyBackground";
        types[HSSObservablePropertyFont] = "HSSObservablePropertyFont";
        types[HSSObservablePropertyShape] = "HSSObservablePropertyShape";
        
        //HSSRgba
        types[HSSObservablePropertyRed] = "HSSObservablePropertyRed";
        types[HSSObservablePropertyGreen] = "HSSObservablePropertyGreen";
        types[HSSObservablePropertyBlue] = "HSSObservablePropertyBlue";
        types[HSSObservablePropertyAlpha] = "HSSObservablePropertyAlpha";
        
        //HSSFont
        types[HSSObservablePropertySize] = "HSSObservablePropertySize";
        types[HSSObservablePropertyFace] = "HSSObservablePropertyFace";
        types[HSSObservablePropertyColor] = "HSSObservablePropertyColor";
        types[HSSObservablePropertyWeight] = "HSSObservablePropertyWeight";
        
        //HSSRoundedRect
        types[HSSObservablePropertyCorners] = "HSSObservablePropertyCorners";
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
        properties["alignX"] = HSSObservablePropertyAlignX;
        properties["alignY"] = HSSObservablePropertyAlignY;
        
        //HSSContainer
        properties["contentAlignX"] = HSSObservablePropertyContentAlignX;
        properties["contentAlignY"] = HSSObservablePropertyContentAlignY;
        properties["directionPrimary"] = HSSObservablePropertyDirectionPrimary;
        properties["directionSecondary"] = HSSObservablePropertyDirectionSecondary;
        properties["background"] = HSSObservablePropertyBackground;
        properties["font"] = HSSObservablePropertyFont;
        properties["shape"] = HSSObservablePropertyShape;
        
        //HSSRgba
        properties["red"] = HSSObservablePropertyRed;
        properties["green"] = HSSObservablePropertyGreen;
        properties["blue"] = HSSObservablePropertyBlue;
        properties["alpha"] = HSSObservablePropertyAlpha;
        
        //HSSFont
        properties["size"] = HSSObservablePropertySize;
        properties["face"] = HSSObservablePropertyFace;
        properties["color"] = HSSObservablePropertyColor;
        properties["weight"] = HSSObservablePropertyWeight;
        
        //HSSRoundedRect
        properties["corners"] = HSSObservablePropertyCorners;
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
        std_log3("added observer for "+HSSObservable::observablePropertyStringRepresentation(target));
    } else {
        HSSObservable::observed theObserved;
        theObserved[hash] = callback;
        this->_propertyObservers[target] = theObserved;
        std_log3("added observer for new "+HSSObservable::observablePropertyStringRepresentation(target));
    }
}

void HSSObservable::removeObserver(HSSObservableProperty target, HSSObservableProperty source, HSSObservable * object)
{
    if(this->_propertyObservers.count(target) != 0){
        HSSObservable::observed &theObserved = this->_propertyObservers[target];
        std::size_t hash = 0;
        boost::hash_combine(hash, object);
        boost::hash_combine(hash, source);
        if(theObserved.count(hash) != 0){
            theObserved.erase(hash);
            std_log3("removing observer for "+HSSObservable::observablePropertyStringRepresentation(target));
            return;
        }
    }
    std_log3("tried to remove non existent observer for "+HSSObservable::observablePropertyStringRepresentation(target));
}

void HSSObservable::propertyChanged(HSSObservableProperty property, void *data)
{
    std_log1("property changed");
}

void HSSObservable::notifyObservers(HSSObservableProperty property, void *data)
{
    HSSObservable::observed::iterator it;
    if(this->_propertyObservers.count(property) != 0){
        HSSObservable::observed &theObserved = this->_propertyObservers[property];
        for (it=theObserved.begin(); it != theObserved.end() ; it++) {
            HSSCallback * callback = (*it).second;
            if(data == NULL){
                std_log1("data is null");
            }
            callback->call(property, data);
        }
    }
}
