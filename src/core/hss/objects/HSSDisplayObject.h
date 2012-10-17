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
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2012 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
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

#include <map>
#include <vector>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QPainter>
#include <QPainterPath>
#include "AXRGlobal.h"
#include "HSSBorder.h"
#include "HSSEvent.h"
#include "HSSFont.h"
#include "HSSMargin.h"
#include "HSSMultipleValue.h"
#include "HSSObject.h"
#include "HSSRgb.h"
#include "HSSRule.h"

class QImage;

namespace AXR
{
    class HSSContainer;
    class HSSFlag;

    /**
     *  @brief Abstract superclass for any object that should be rendered on the screen.
     *
     *  It provides basic functionality for drawing and integration into the layout system,
     *  as well as basic support for the information stored in an XML element.
     */
    class AXR_API HSSDisplayObject : public HSSObject
    {
    public:
        friend class HSSContainer;

        typedef QSharedPointer<HSSDisplayObject> p;

        /**
         *  The "parent pointer", a weak variant of a shared pointer, to break reference cycles.
         */
        typedef QWeakPointer<HSSContainer> pp;
        typedef std::vector<HSSDisplayObject::p> c;
        typedef std::vector<HSSDisplayObject::p>::iterator it;
        typedef std::vector<HSSDisplayObject::p>::const_iterator const_it;

        /**
         *  Initializes all ivars to default values.
         */
        void initialize();

        /**
         *  Destructor for this class.
         */
        virtual ~HSSDisplayObject();
        virtual AXRString toString();
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

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
        QSharedPointer<HSSContainer> getParent();

        /**
         *  Sets the given container to be the parent of this display object.
         *  @param parent   A shared pointer to the container that is the parent of this display object.
         */
        void setParent(QSharedPointer<HSSContainer> parent);

        /**
         *  Removes itself from the parent in the content tree.
         */
        void removeFromParent();

        /**
         *  Gives the sibling elements.
         *  @return A vector containing shared pointers to the siblings of this display object.
         */
        const std::vector<HSSDisplayObject::p> getSiblings();

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
        void attributesAdd(AXRString name, AXRString value);

        /**
         *  Removes an entry in the list of attributes.
         *  @param name     The name of the attribute. Will be used as they key to find the data.
         */
        void attributesRemove(AXRString name);

        /**
         *  Getter for the content text.
         *  @return A string containing the content text.
         */
        virtual AXRString getContentText();

        /**
         *  Setter for the content text.
         *  @param text     A string containing the new value for content text.
         */
        virtual void setContentText(const AXRString &text);

        /**
         *  Append a piece of text to the content text.
         *  @param text     A string containing the value to be appended to the content text.
         */
        virtual void appendContentText(const AXRString &text);

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
        int rulesSize();
        void setRuleStatus(HSSRule::p rule, HSSRuleState newValue);
        bool hasRule(HSSRule::p rule);
        virtual void readDefinitionObjects();
        virtual void recursiveReadDefinitionObjects();
        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);
        void setNeedsRereadRules(bool value);
        bool needsRereadRules();

        virtual void regenerateSurfaces(bool force = false);
        virtual void recursiveRegenerateSurfaces(bool force = false);

        void setNeedsSurface(bool value);
        bool needsSurface();

        void setDirty(bool value);
        bool isDirty();
        virtual void draw(QPainter &painter);
        virtual void drawBackground();
        virtual void _drawBackground(QPainter &painter, const QPainterPath &path);
        virtual void drawForeground();
        virtual void drawBorders();
        virtual void recursiveDraw(QPainter &painter);

        void setNeedsLayout(bool value);
        bool needsLayout() const;
        virtual void layout();
        virtual void recursiveLayout();
        virtual void setGlobalX(HSSUnit newValue);
        virtual void setGlobalY(HSSUnit newValue);

        AXRString getElementName();
        void setElementName(AXRString name);

        HSSUnit getAlignX();
        HSSParserNode::p getDAlignX();
        void setDAlignX(HSSParserNode::p value);
        void alignXChanged(HSSObservableProperty source, void*data);

        HSSUnit getAlignY();
        HSSParserNode::p getDAlignY();
        void setDAlignY(HSSParserNode::p value);
        void alignYChanged(HSSObservableProperty source, void*data);

        HSSUnit getAnchorX();
        HSSParserNode::p getDAnchorX();
        void setDAnchorX(HSSParserNode::p value);
        void anchorXChanged(HSSObservableProperty source, void*data);

        HSSUnit getAnchorY();
        HSSParserNode::p getDAnchorY();
        void setDAnchorY(HSSParserNode::p value);
        void anchorYChanged(HSSObservableProperty source, void*data);

        //flow
        bool getFlow();
        HSSParserNode::p getDFlow();
        void setDFlow(HSSParserNode::p value);
        void flowChanged(HSSObservableProperty source, void*data);

        //contained
        bool getContained();
        HSSParserNode::p getDContained();
        void setDContained(HSSParserNode::p value);
        void containedChanged(HSSObservableProperty source, void*data);

        //height
        HSSUnit getHeight();
        HSSParserNode::p getDHeight();
        void setDHeight(HSSParserNode::p value);
        void heightChanged(HSSObservableProperty source, void*data);

        //width
        HSSUnit getWidth();
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

        //margin
        const HSSParserNode::p getDMargin() const;
        void setDMargin(HSSParserNode::p value);
        void addDMargin(HSSParserNode::p value);
        void marginChanged(HSSObservableProperty source, void*data);

        //padding
        const HSSParserNode::p getDPadding() const;
        void setDPadding(HSSParserNode::p value);
        void addDPadding(HSSParserNode::p value);
        void paddingChanged(HSSObservableProperty source, void*data);

        //border
        const HSSParserNode::p getDBorder() const;
        void setDBorder(HSSParserNode::p value);
        void addDBorder(HSSParserNode::p value);
        void borderChanged(HSSObservableProperty source, void*data);

        //visible
        bool getVisible() const;
        const HSSParserNode::p getDVisible() const;
        void setDVisible(HSSParserNode::p value);
        void visibleChanged(HSSObservableProperty source, void*data);

        virtual void setDefaults();

        virtual bool handleEvent(HSSEventType, void* data);

        void setHover(bool newValue);
        bool isHover();

        void ruleChanged(HSSObservableProperty source, void*data);

        void createFlag(QSharedPointer<HSSFlag> flag, HSSRuleState defaultValue);
        bool hasFlag(AXRString name);
        HSSRuleState flagState(AXRString name);
        void flagsActivate(AXRString name);
        void flagsDeactivate(AXRString name);
        void flagsToggle(AXRString name);

        bool isRoot();
        void setRoot(bool newValue);

        /**
         *  @todo make private and add accessors
         */
        std::map<AXRString, AXRString>attributes;

    protected:
        pp parent;
        AXRString elementName;
        AXRString contentText;
        std::vector<HSSRuleStatus::p> rules;

        //if the rules have changed
        bool _needsRereadRules;

        //if it needs to resize the surface
        bool _needsSurface;
        QImage * backgroundSurface;
        QImage * foregroundSurface;
        QImage * bordersSurface;

        //flags
        QMap<AXRString, HSSRuleState> _flagsStatus;
        QMap<AXRString, std::vector< QSharedPointer<HSSFlag> > > _flags;

        //if it needs to redraw
        bool _isDirty;

        //if it needs to layout
        bool _needsLayout;
        std::vector<std::vector<HSSDisplayObject::p> >layoutLines;

        //here go the final computed values
        HSSUnit x, y, globalX, globalY,
        width, innerWidth, outerWidth,
        height, innerHeight, outerHeight,
        topPadding, rightPadding, bottomPadding, leftPadding,
        topMargin, rightMargin, bottomMargin, leftMargin
        ;
        //width
        HSSParserNode::p dWidth;
        bool widthByContent;
        HSSObservable::p observedWidth;
        HSSObservableProperty observedWidthProperty;
        //height
        HSSParserNode::p dHeight;
        bool heightByContent;
        HSSObservable::p observedHeight;
        HSSObservableProperty observedHeightProperty;

        /**
         *  @todo add "bounds" property
         */

        //anchorX
        HSSParserNode::p dAnchorX;
        HSSUnit anchorX;
        bool _anchorXdefault;
        HSSObservable::p observedAnchorX;
        HSSObservableProperty observedAnchorXProperty;
        //anchorY
        HSSParserNode::p dAnchorY;
        HSSUnit anchorY;
        bool _anchorYdefault;
        HSSObservable::p observedAnchorY;
        HSSObservableProperty observedAnchorYProperty;
        //flow
        HSSParserNode::p dFlow;
        bool flow;
        bool does_float;
        //contained
        HSSParserNode::p dContained;
        bool contained;
        //alignX
        HSSParserNode::p dAlignX;
        HSSUnit alignX;
        HSSObservable::p observedAlignX;
        HSSObservableProperty observedAlignXProperty;
        //alignY
        HSSParserNode::p dAlignY;
        HSSUnit alignY;
        HSSObservable::p observedAlignY;
        HSSObservableProperty observedAlignYProperty;
        //background
        HSSParserNode::p dBackground;
        HSSObservable::p observedBackground;
        HSSObservableProperty observedBackgroundProperty;
        std::vector<HSSObject::p> background;
        //content
        HSSParserNode::p dContent;
        HSSObservable::p observedContent;
        HSSObservableProperty observedContentProperty;
        std::vector<HSSObject::p> content;
        //font
        HSSParserNode::p dFont;
        HSSObservable::p observedFont;
        HSSObservableProperty observedFontProperty;
        std::vector<HSSFont::p> font;

        //on
        HSSParserNode::p dOn;
        HSSObservable::p observedOn;
        HSSObservableProperty observedOnProperty;
        QMap<HSSEventType, std::vector<HSSObject::p> > on;

        //margin
        HSSParserNode::p dMargin;
        HSSObservable::p observedMargin;
        HSSObservableProperty observedMarginProperty;
        std::vector<HSSMargin::p> margin;

        //padding
        HSSParserNode::p dPadding;
        HSSObservable::p observedPadding;
        HSSObservableProperty observedPaddingProperty;
        std::vector<HSSMargin::p> padding;

        //border
        HSSParserNode::p dBorder;
        HSSObservable::p observedBorder;
        HSSObservableProperty observedBorderProperty;
        std::vector<HSSBorder::p> border;
        HSSUnit borderBleeding;

        //visible
        HSSParserNode::p dVisible;
        HSSObservable::p observedVisible;
        HSSObservableProperty observedVisibleProperty;
        bool visible;

        HSSUnit zoomFactor;

        /**
         *  @todo add "transform" property
         */
        /**
         *  @todo add "effects" property
         */
        /**
         *  @todo add "animation" property
         */

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

    protected:
        /**
         *  Copy constructor for HSSDisplayObject objects.
         */
        HSSDisplayObject(const HSSDisplayObject & orig);

        /**
         *  Constructor for HSSDisplayObject objects.
         *  @param type     Subclasses pass their own type here.
         */
        HSSDisplayObject(HSSObjectType type);

        void _setInnerWidth();
        void _setInnerHeight();
        void _setOuterWidth();
        void _setOuterHeight();

    private:
        HSSUnit _evaluatePropertyValue(
                                   void(HSSDisplayObject::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedProperty,
                                   HSSObservable::p observedObject,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable::p &observedStore,
                                   HSSObservableProperty &observedStoreProperty,
                                   const std::vector<HSSDisplayObject::p> * scope
                                   );
        bool _isHover;
        bool _isRoot;

        bool _layoutFlagIsInSecondaryGroup;
        bool _layoutFlagIsInSecondaryLine;
        bool _layoutFlagVerticallyAligned;
        bool _layoutFlagLockTop;
        HSSUnit _layoutLockTopPosition;
        bool _layoutFlagLockBottom;
        HSSUnit _layoutLockBottomPosition;
    };
}

Q_DECLARE_METATYPE(std::vector< std::vector<AXR::HSSDisplayObject::p> >);

#endif
