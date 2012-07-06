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
    class HSSFlag;
    
    /**
     *  @brief Abstract superclass for any object that should be rendered on the screen.
     *
     *  This is an abstract class, and should not be used directly. Create a specific subclass
     *  instead. It provides basic functionality for drawing and integration into the layout system,
     *  as well as basic support for the information stored in an XML element.
     */
    class HSSDisplayObject : public HSSObject
    {
    public:
        
        friend class HSSContainer;
        
        typedef boost::shared_ptr<HSSDisplayObject> p;
        
        /**
         *  The "parent pointer", a weak variant of a shared pointer, to break reference cycles.
         */
        typedef boost::weak_ptr<HSSContainer> pp;
        typedef std::vector<HSSDisplayObject::p> c;
        typedef std::vector<HSSDisplayObject::p>::iterator it;
        typedef std::vector<HSSDisplayObject::p>::const_iterator const_it;
        
        /**
         *  Constructor for HSSDisplayObject objects.
         */
        HSSDisplayObject();      
        
        /**
         *  Constructor for HSSDisplayObject objects.
         *  @param type     Subclasses pass their own type here.
         */
        HSSDisplayObject(HSSObjectType type);
        
        /**
         *  Initializes all ivars to default values.
         */
        void initialize();
        
        /**
         *  Copy constructor for HSSDisplayObject objects.
         */
        HSSDisplayObject(const HSSDisplayObject & orig);
        
        /**
         *  Clones an instance of HSSDisplayObject and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSDisplayObject.
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSDisplayObject();
        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);
        
        /**
         *  Each subclass should return wether it can have children or not. Right now the only
         *  one which does is HSSContainer.
         *  @return Wether it can have children or not.
         */
        virtual bool canHaveChildren();
        
        /**
         *  Gives access to the parent element in the content tree.
         *  @return A shared pointer to the parent element in the content tree.
         */
        boost::shared_ptr<HSSContainer> getParent();
        
        /**
         *  Sets the given container to be the parent of this display object.
         *  @param parent   A shared pointer to the container that is the parent of this display object.
         */
        void setParent(boost::shared_ptr<HSSContainer> parent);
        
        /**
         *  Removes itself from the parent in the content tree.
         */
        void removeFromParent();
        
        /**
         *  Stores the given index.
         */
        void setIndex(unsigned newIndex);
        
        /**
         *  @return The stored index.
         */
        unsigned getIndex();
        
        /**
         *  Add an entry in the list of attributes.
         *  @param name     The name of the attribute. Will be used as they key to the data.
         *  @param value    The content of the attribute.
         */
        void attributesAdd(std::string name, std::string value);
        
        /**
         *  Removes an entry in the list of attributes.
         *  @param name     The name of the attribute. Will be used as they key to find the data.
         */
        void attributesRemove(std::string name);
        
        /**
         *  Getter for the content text.
         *  @return A string containing the content text.
         */
        virtual std::string getContentText();
        
        /**
         *  Setter for the content text.
         *  @param text     A string containing the new value for content text.
         */
        virtual void setContentText(std::string text);
        
        /**
         *  Append a piece of text to the content text.
         *  @param text     A string containing the value to be appended to the content text.
         */
        virtual void appendContentText(std::string text);
        
        /**
         *  Add a rule to the list of rules associated with this display object.
         *  @param newRule      A shared pointer to the rule.
         *  @param defaultState The default state in which the rule will be in (activated or not, etc).
         */
        void rulesAdd(HSSRule::p newRule, HSSRuleState defaultState);
        void rulesAddIsAChildren(HSSPropertyDefinition::p propdef, HSSRuleState defaultState);
        HSSRule::p rulesGet(unsigned index);
        void rulesRemove(unsigned index);
        void rulesRemoveLast();
        const int rulesSize();
        void setRuleStatus(HSSRule::p rule, HSSRuleState newValue);
        bool hasRule(HSSRule::p rule);
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
        virtual void _drawBackground(cairo_t * cairo);
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
        long double getAlignX();
        HSSParserNode::p getDAlignX();
        void setDAlignX(HSSParserNode::p value);
        void alignXChanged(HSSObservableProperty source, void*data);
        
        //alignY
        long double getAlignY();
        HSSParserNode::p getDAlignY();
        void setDAlignY(HSSParserNode::p value);
        void alignYChanged(HSSObservableProperty source, void*data);
        
        //anchorX
        long double getAnchorX();
        HSSParserNode::p getDAnchorX();
        void setDAnchorX(HSSParserNode::p value);
        void anchorXChanged(HSSObservableProperty source, void*data);
        
        //anchorY
        long double getAnchorY();
        HSSParserNode::p getDAnchorY();
        void setDAnchorY(HSSParserNode::p value);
        void anchorYChanged(HSSObservableProperty source, void*data);
        
        //flow
        bool getFlow();
        HSSParserNode::p getDFlow();
        void setDFlow(HSSParserNode::p value);
        void flowChanged(HSSObservableProperty source, void*data);
        
        //overflow
        bool getOverflow();
        HSSParserNode::p getDOverflow();
        void setDOverflow(HSSParserNode::p value);
        void overflowChanged(HSSObservableProperty source, void*data);
        
        //height
        long double getHeight();
        HSSParserNode::p getDHeight();
        void setDHeight(HSSParserNode::p value);
        void heightChanged(HSSObservableProperty source, void*data);
        
        //width
        long double getWidth();
        HSSParserNode::p getDWidth();
        void setDWidth(HSSParserNode::p value);
        void widthChanged(HSSObservableProperty source, void*data);
        
        //background
        HSSParserNode::p getDBackground();
        void setDBackground(HSSParserNode::p value);
        void addDBackground(HSSParserNode::p value);
        void backgroundChanged(HSSObservableProperty source, void*data);
        
        //content
        HSSParserNode::p getDContent();
        void setDContent(HSSParserNode::p value);
        void addDContent(HSSParserNode::p value);
        void contentChanged(HSSObservableProperty source, void*data);
        
        //font
        const HSSParserNode::p getDFont() const;
        void setDFont(HSSParserNode::p value);
        void addDFont(HSSParserNode::p value);
        void fontChanged(HSSObservableProperty source, void*data);
        
        //on
        HSSParserNode::p getDOn();
        void setDOn(HSSParserNode::p value);
        void addDOn(HSSParserNode::p value);
        void onChanged(HSSObservableProperty source, void*data);
        bool fireEvent(HSSEventType type);
        
        //border
        const HSSParserNode::p getDBorder() const;
        void setDBorder(HSSParserNode::p value);
        void addDBorder(HSSParserNode::p value);
        void borderChanged(HSSObservableProperty source, void*data);
        
        virtual void setDefaults();
        
        virtual bool handleEvent(HSSEventType, void* data);
        
        void setHover(bool newValue);
        bool isHover();
        
        void ruleChanged(HSSObservableProperty source, void*data);
        
        void createFlag(boost::shared_ptr<HSSFlag> flag, HSSRuleState defaultValue);
        bool hasFlag(std::string name);
        HSSRuleState flagState(std::string name);
        void flagsActivate(std::string name);
        void flagsDeactivate(std::string name);
        void flagsToggle(std::string name);
        
        bool isRoot();
        void setRoot(bool newValue);
        
        /**
         *  @todo make private and add accessors
         */
        std::map<std::string, std::string>attributes;
        
    protected:
        pp parent;
        std::string elementName;
        std::string contentText;
        std::vector<HSSRuleStatus::p> rules;
        
        //if the rules have changed
        bool _needsRereadRules;
        
        //if it needs to resize the surface
        bool _needsSurface;
        cairo_surface_t * backgroundSurface;
        cairo_surface_t * foregroundSurface;
        cairo_surface_t * bordersSurface;
        
        //flags
        boost::unordered_map<std::string, HSSRuleState> _flagsStatus;
        boost::unordered_map<std::string, std::vector< boost::shared_ptr<HSSFlag> > > _flags;
        
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
        
        /**
         *  @todo add "bounds" property
         */
        
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
        //overflow
        HSSParserNode::p dOverflow;
        bool overflow;
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
        HSSParserNode::p dBackground;
        HSSObservable * observedBackground;
        HSSObservableProperty observedBackgroundProperty;
        std::vector<HSSObject::p> background;
        //content
        HSSParserNode::p dContent;
        HSSObservable * observedContent;
        HSSObservableProperty observedContentProperty;
        std::vector<HSSObject::p> content;
        //font
        HSSParserNode::p dFont;
        HSSObservable * observedFont;
        HSSObservableProperty observedFontProperty;
        std::vector<HSSFont::p> font;
        
        //on
        HSSParserNode::p dOn;
        HSSObservable * observedOn;
        HSSObservableProperty observedOnProperty;
        boost::unordered_map<HSSEventType, std::vector<HSSObject::p> > on;
        
        /**
         *  @todo add "margin" property
         */
        
        //border
        HSSParserNode::p dBorder;
        HSSObservable * observedBorder;
        HSSObservableProperty observedBorderProperty;
        std::vector<HSSBorder::p> border;
        long double borderBleeding;
        
        long double zoomFactor;
        
        /**
         *  @todo add "transform" property
         */
        /**
         *  @todo add "effects" property
         */
        /**
         *  @todo add "animation" property
         */
        
        bool visible;
        signed int drawIndex;
        unsigned int tabIndex;
        
        /**
         *  @todo add "focused" property
         */
        /**
         *  @todo add "mask" property
         */
        
        unsigned _index;
        
        HSSDisplayObject::p shared_from_this();
        
        
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
        HSSClonable::p cloneImpl() const;
        bool _isHover;
        bool _isRoot;
    };
}


#endif
