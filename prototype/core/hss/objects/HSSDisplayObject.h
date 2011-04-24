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
 *      Last changed: 2011/04/24
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 10
 *
 ********************************************************************/

#ifndef HSSDISPLAYOBJECT_H
#define HSSDISPLAYOBJECT_H

#include <string>
#include <map>
#include <vector>
#include "HSSObject.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include "../parsing/HSSRule.h"
#include <cairo/cairo.h>

namespace AXR {
    
    class HSSContainer;

    class HSSDisplayObject : public HSSObject
    {
    public:
        typedef boost::shared_ptr<HSSDisplayObject> p;
        typedef boost::weak_ptr<HSSContainer> parentPointer;
        
        HSSDisplayObject();
        HSSDisplayObject(std::string name);
        virtual ~HSSDisplayObject();
        virtual std::string toString();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        boost::shared_ptr<HSSContainer> getParent();
        void setParent(boost::shared_ptr<HSSContainer> parent);
        void attributesAdd(std::string name, std::string value);
        void attributesRemove(std::string name);
        
        void rulesAdd(HSSRule::p newRule);
        HSSRule::p rulesGet(unsigned index);
        void rulesRemove(unsigned index);
        void rulesRemoveLast();
        const int rulesSize();
        void readDefinitionObjects();
        virtual void recursiveReadDefinitionObjects();
        void setNeedsRereadRules(bool value);
        bool needsRereadRules();
        
        virtual void regenerateSurfaces();
        virtual void recursiveRegenerateSurfaces();
        
        void setNeedsSurface(bool value);
        bool needsSurface();
        
        void setDirty(bool value);
        bool isDirty();
        virtual void draw(cairo_t * cairo);
        virtual void recursiveDraw(cairo_t * cairo);
        
        std::string getElementName();
        void setElementName(std::string name);
        
        void propertyChanged(HSSObservableProperty property, void* data);
        
        //width
        HSSParserNode::p getDWidth();
        void setDWidth(HSSParserNode::p value);
        //height
        HSSParserNode::p getDHeight();
        void setDHeight(HSSParserNode::p value);
        //anchorX
        HSSParserNode::p getDAnchorX();
        void setDAnchorX(HSSParserNode::p value);
        //anchorY
        HSSParserNode::p getDAnchorY();
        void setDAnchorY(HSSParserNode::p value);
        //flow
        HSSParserNode::p getDFlow();
        void setDFlow(HSSParserNode::p value);
        //alignX
        HSSParserNode::p getDAlignX();
        void setDAlignX(HSSParserNode::p value);
        //alignY
        HSSParserNode::p getDAlignY();
        void setDAlignY(HSSParserNode::p value);
        
        virtual void setDefaults();
        
    protected:
        parentPointer parent;
        std::map<std::string, std::string>attributes;
        std::string elementName;
        std::string contentText;
        std::vector<HSSRule::p> rules;
        
        //if the rules have changed
        bool _needsRereadRules;
        
        //if it needs to resize the surface
        bool _needsSurface;
        cairo_surface_t * backgroundSurface;
        cairo_surface_t * foregroundSurface;
        cairo_surface_t * bordersSurface;
        
        //if it needs to redraw
        bool _isDirty;
        
        //here go the final computed values
        double x;
        double y;
        double height;
        HSSObject * observedHeight;
        double width;
        HSSObject * observedWidth;
        //FIXME: bounds
        double anchorX;
        double anchorY;
        bool flow;
        bool does_float;
        double alignX;
        double alignY;
        //FIXME: margin
        //FIXME: border
        double zoomFactor;
        //FIXME: transform
        //FIXME: effects
        //FIXME: animation
        //FIXME: behavior
        bool visible;
        signed int drawIndex;
        unsigned int tabIndex;
        //FIXME: focused
        //FIXME: mask
        
        //here go the definition objects
        HSSParserNode::p dWidth;
        HSSParserNode::p dHeight;
        HSSParserNode::p dAnchorX;
        HSSParserNode::p dAnchorY;
        HSSParserNode::p dFlow;
        HSSParserNode::p dAlignX;
        HSSParserNode::p dAlignY;
    };

}


#endif