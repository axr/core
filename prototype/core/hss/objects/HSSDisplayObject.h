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
 *      Last changed: 2011/12/19
 *      HSS version: 1.0
 *      Core version: 0.42
 *      Revision: 27
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
#include "HSSMultipleValue.h"
#include "HSSRgb.h"
#include "HSSFont.h"
#include "HSSEvent.h"
#include "HSSBorder.h"
#include <boost/enable_shared_from_this.hpp>

namespace AXR {
    
    class HSSContainer;

    class HSSDisplayObject :  public HSSObject
    {
    public:
        
        friend class HSSContainer;
        
        typedef boost::shared_ptr<HSSDisplayObject> p;
        typedef boost::weak_ptr<HSSContainer> parentPointer;
        typedef std::vector<HSSDisplayObject::p> c;
        typedef std::vector<HSSDisplayObject::p>::iterator it;
        
        HSSDisplayObject();
        HSSDisplayObject(std::string name);
        
        void initialize();
        
        virtual ~HSSDisplayObject();
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        virtual bool canHaveChildren();
        boost::shared_ptr<HSSContainer> getParent();
        void setParent(boost::shared_ptr<HSSContainer> parent);
        void removeFromParent();
        void setIndex(unsigned newIndex);
        unsigned getIndex();
        void attributesAdd(std::string name, std::string value);
        void attributesRemove(std::string name);
        std::string getContentText();
        void setContentText(std::string text);
        void appendContentText(std::string text);
        
        void rulesAdd(HSSRule::p newRule);
        HSSRule::p rulesGet(unsigned index);
        void rulesRemove(unsigned index);
        void rulesRemoveLast();
        const int rulesSize();
        virtual void readDefinitionObjects();
        virtual void recursiveReadDefinitionObjects();
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        virtual void setProperty(HSSObservableProperty name, void * value);
        void setNeedsRereadRules(bool value);
        bool needsRereadRules();
        
        virtual void regenerateSurfaces();
        virtual void recursiveRegenerateSurfaces();
        
        void setNeedsSurface(bool value);
        bool needsSurface();
        
        void setDirty(bool value);
        bool isDirty();
        virtual void draw(cairo_t * cairo);
        virtual void drawBackground();
        virtual void drawForeground();
        virtual void drawBorders();
        virtual void recursiveDraw(cairo_t * cairo);
        
        void setNeedsLayout(bool value);
        const bool needsLayout() const;
        virtual void layout();
        virtual void recursiveLayout();
        virtual void setGlobalX(long double newValue);
        virtual void setGlobalY(long double newValue);
        
        std::string getElementName();
        void setElementName(std::string name);
        
        //alignX
        HSSParserNode::p getDAlignX();
        void setDAlignX(HSSParserNode::p value);
        void alignXChanged(HSSObservableProperty source, void*data);
        
        //alignY
        HSSParserNode::p getDAlignY();
        void setDAlignY(HSSParserNode::p value);
        void alignYChanged(HSSObservableProperty source, void*data);
        
        //anchorX
        HSSParserNode::p getDAnchorX();
        void setDAnchorX(HSSParserNode::p value);
        void anchorXChanged(HSSObservableProperty source, void*data);
        
        //anchorY
        HSSParserNode::p getDAnchorY();
        void setDAnchorY(HSSParserNode::p value);
        void anchorYChanged(HSSObservableProperty source, void*data);
        
        //flow
        HSSParserNode::p getDFlow();
        void setDFlow(HSSParserNode::p value);
        
        //height
        HSSParserNode::p getDHeight();
        void setDHeight(HSSParserNode::p value);
        void heightChanged(HSSObservableProperty source, void*data);
        
        //width
        HSSParserNode::p getDWidth();
        void setDWidth(HSSParserNode::p value);
        void widthChanged(HSSObservableProperty source, void*data);
        
        //background
        const HSSMultipleValue getDBackground() const;
        void setDBackground(HSSParserNode::p value);
        void addDBackground(HSSParserNode::p value);
        
        //font
        const HSSMultipleValue getDFont() const;
        void setDFont(HSSParserNode::p value);
        void fontChanged(HSSObservableProperty source, void*data);
        
        //behavior
        const HSSMultipleValueDefinition::p getDBehavior() const;
        void setDBehavior(HSSParserNode::p value);
        bool fireEvent(HSSEventType type);
        
        //border
        const HSSParserNode::p getDBorder() const;
        void setDBorder(HSSParserNode::p value);
        void addDBorder(HSSParserNode::p value);
        void borderChanged(HSSObservableProperty source, void*data);
        
        virtual void setDefaults();
        
        virtual bool handleEvent(HSSEventType, void* data);
        
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
        
        //if it needs to layout
        bool _needsLayout;
        std::vector<std::vector<HSSDisplayObject::p> >layoutLines;
        
        //here go the final computed values
        long double x;
        long double y;
        long double globalX;
        long double globalY;
        //width
        HSSParserNode::p dWidth;
        long double width;
        HSSObservable * observedWidth;
        HSSObservableProperty observedWidthProperty;
        //height
        HSSParserNode::p dHeight;
        long double height;
        bool heightByContent;
        HSSObservable * observedHeight;
        HSSObservableProperty observedHeightProperty;
        
        //FIXME: bounds
        
        //anchorX
        HSSParserNode::p dAnchorX;
        long double anchorX;
        HSSObservable * observedAnchorX;
        HSSObservableProperty observedAnchorXProperty;
        //anchorY
        HSSParserNode::p dAnchorY;
        long double anchorY;
        HSSObservable * observedAnchorY;
        HSSObservableProperty observedAnchorYProperty;
        //flow
        HSSParserNode::p dFlow;
        bool flow;
        bool does_float;
        //alignX
        HSSParserNode::p dAlignX;
        long double alignX;
        HSSObservable * observedAlignX;
        HSSObservableProperty observedAlignXProperty;
        //alignY
        HSSParserNode::p dAlignY;
        long double alignY;
        HSSObservable * observedAlignY;
        HSSObservableProperty observedAlignYProperty;
        //background
        HSSMultipleValue dBackground;
        HSSRgb::p backgroundColor;
        //font
        HSSMultipleValue dFont;
        HSSFont::p font;
        //behavior
        HSSMultipleValueDefinition::p dBehavior;
        boost::unordered_map<HSSEventType, std::vector<HSSEvent::p> > behavior;
        
        //FIXME: margin
        
        //border
        HSSParserNode::p dBorder;
        HSSObservable * observedBorder;
        HSSObservableProperty observedBorderProperty;
        std::vector<HSSBorder::p> border;
        long double borderBleeding;
        
        long double zoomFactor;
        //FIXME: transform
        //FIXME: effects
        //FIXME: animation
        bool visible;
        signed int drawIndex;
        unsigned int tabIndex;
        //FIXME: focused
        //FIXME: mask
        
        unsigned _index;
        
    private:
        long double _setLDProperty(
                                   void(HSSDisplayObject::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p         value,
                                   long double              percentageBase,
                                   HSSObservableProperty    observedProperty,
                                   HSSObservable *          observedObject,
                                   HSSObservableProperty    observedSourceProperty,
                                   HSSObservable *          &observedStore,
                                   HSSObservableProperty    &observedStoreProperty,
                                   const std::vector<HSSDisplayObject::p> * scope
                                   );
        
    };
}


#endif