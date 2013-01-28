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

#include <vector>
#include <QSharedPointer>
#include "HSSObject.h"

class QImage;
class QPainter;
class QPainterPath;

namespace AXR
{
    class HSSBorder;
    class HSSContainer;
    class HSSFlag;
    class HSSFont;
    class HSSMargin;
    class HSSPropertyDefinition;
    class HSSRule;
    class HSSRuleStatus;
    class HSSSimpleSelection;

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

        /**
         *  The "parent pointer", a weak variant of a shared pointer, to break reference cycles.
         */
        typedef QWeakPointer<HSSContainer> pp;

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
        QSharedPointer<HSSSimpleSelection> getSiblings();

        /**
         *  Gives the sibling elements after the current one.
         *  @return A vector containing shared pointers to the next siblings of this display object.
         */
        QSharedPointer<HSSSimpleSelection> getNextSiblings();

        /**
         *  Gives the sibling elements before the current one.
         *  @return A vector containing shared pointers to the previous siblings of this display object.
         */
        QSharedPointer<HSSSimpleSelection> getPreviousSiblings();

        /**
         *  Stores the given index.
         */
        void setIndex(size_t newIndex);

        /**
         *  @return The stored index.
         */
        size_t getIndex() const;

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
        void rulesAdd(QSharedPointer<HSSRule> newRule, HSSRuleState defaultState);
        void rulesAddIsAChildren(QSharedPointer<HSSPropertyDefinition> propdef, HSSRuleState defaultState);
        QSharedPointer<HSSRule> rulesGet(unsigned index);
        void rulesRemove(unsigned index);
        void rulesRemoveLast();
        size_t rulesSize() const;
        void setRuleStatus(QSharedPointer<HSSRule> rule, HSSRuleState newValue);
        bool hasRule(QSharedPointer<HSSRule> rule);
        virtual void readDefinitionObjects();
        virtual void recursiveReadDefinitionObjects();
        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);
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
        virtual void recursiveResetLayout();
        virtual void setGlobalX(HSSUnit newValue);
        virtual void setGlobalY(HSSUnit newValue);

        AXRString getElementName();
        void setElementName(AXRString name);

        HSSUnit getAlignX();
        QSharedPointer<HSSParserNode> getDAlignX();
        void setDAlignX(QSharedPointer<HSSParserNode> value);
        void alignXChanged(HSSObservableProperty source, void*data);

        HSSUnit getAlignY();
        QSharedPointer<HSSParserNode> getDAlignY();
        void setDAlignY(QSharedPointer<HSSParserNode> value);
        void alignYChanged(HSSObservableProperty source, void*data);

        HSSUnit getAnchorX();
        QSharedPointer<HSSParserNode> getDAnchorX();
        void setDAnchorX(QSharedPointer<HSSParserNode> value);
        void anchorXChanged(HSSObservableProperty source, void*data);

        HSSUnit getAnchorY();
        QSharedPointer<HSSParserNode> getDAnchorY();
        void setDAnchorY(QSharedPointer<HSSParserNode> value);
        void anchorYChanged(HSSObservableProperty source, void*data);

        //flow
        bool getFlow();
        QSharedPointer<HSSParserNode> getDFlow();
        void setDFlow(QSharedPointer<HSSParserNode> value);
        void flowChanged(HSSObservableProperty source, void*data);

        //contained
        bool getContained();
        QSharedPointer<HSSParserNode> getDContained();
        void setDContained(QSharedPointer<HSSParserNode> value);
        void containedChanged(HSSObservableProperty source, void*data);

        //height
        HSSUnit getHeight();
        QSharedPointer<HSSParserNode> getDHeight();
        void setDHeight(QSharedPointer<HSSParserNode> value);
        void heightChanged(HSSObservableProperty source, void*data);

        //width
        HSSUnit getWidth();
        QSharedPointer<HSSParserNode> getDWidth();
        void setDWidth(QSharedPointer<HSSParserNode> value);
        void widthChanged(HSSObservableProperty source, void*data);

        //background
        QSharedPointer<HSSParserNode> getDBackground();
        void setDBackground(QSharedPointer<HSSParserNode> value);
        void addDBackground(QSharedPointer<HSSParserNode> value);
        void backgroundChanged(HSSObservableProperty source, void*data);

        //content
        QSharedPointer<HSSParserNode> getDContent();
        void setDContent(QSharedPointer<HSSParserNode> value);
        void addDContent(QSharedPointer<HSSParserNode> value);
        void contentChanged(HSSObservableProperty source, void*data);

        //font
        const QSharedPointer<HSSParserNode> getDFont() const;
        void setDFont(QSharedPointer<HSSParserNode> value);
        void addDFont(QSharedPointer<HSSParserNode> value);
        void fontChanged(HSSObservableProperty source, void*data);

        //on
        QSharedPointer<HSSParserNode> getDOn();
        void setDOn(QSharedPointer<HSSParserNode> value);
        void addDOn(QSharedPointer<HSSParserNode> value);
        void onChanged(HSSObservableProperty source, void*data);
        bool fireEvent(HSSEventType type);

        //margin
        const QSharedPointer<HSSParserNode> getDMargin() const;
        void setDMargin(QSharedPointer<HSSParserNode> value);
        void addDMargin(QSharedPointer<HSSParserNode> value);
        void marginChanged(HSSObservableProperty source, void*data);

        //padding
        const QSharedPointer<HSSParserNode> getDPadding() const;
        void setDPadding(QSharedPointer<HSSParserNode> value);
        void addDPadding(QSharedPointer<HSSParserNode> value);
        void paddingChanged(HSSObservableProperty source, void*data);

        //border
        const QSharedPointer<HSSParserNode> getDBorder() const;
        void setDBorder(QSharedPointer<HSSParserNode> value);
        void addDBorder(QSharedPointer<HSSParserNode> value);
        void borderChanged(HSSObservableProperty source, void*data);

        //visible
        bool getVisible() const;
        const QSharedPointer<HSSParserNode> getDVisible() const;
        void setDVisible(QSharedPointer<HSSParserNode> value);
        void visibleChanged(HSSObservableProperty source, void*data);

        virtual void setDefaults();

        virtual bool handleEvent(HSSEventType, void* data);

        void setHover(bool newValue);
        bool isHover();

        void setPress(bool newValue);
        bool isPress();

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
        std::vector<QSharedPointer<HSSRuleStatus> > rules;

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
        std::vector<HSSSimpleSelection >layoutLines;

        //here go the final computed values
        HSSUnit x, y, globalX, globalY,
        width, innerWidth, outerWidth,
        height, innerHeight, outerHeight,
        topPadding, rightPadding, bottomPadding, leftPadding,
        topMargin, rightMargin, bottomMargin, leftMargin
        ;
        //width
        QSharedPointer<HSSParserNode> dWidth;
        bool widthByContent;
        QSharedPointer<HSSObservable> observedWidth;
        HSSObservableProperty observedWidthProperty;
        //height
        QSharedPointer<HSSParserNode> dHeight;
        bool heightByContent;
        QSharedPointer<HSSObservable> observedHeight;
        HSSObservableProperty observedHeightProperty;

        /**
         *  @todo add "bounds" property
         */

        //anchorX
        QSharedPointer<HSSParserNode> dAnchorX;
        HSSUnit anchorX;
        bool _anchorXdefault;
        QSharedPointer<HSSObservable> observedAnchorX;
        HSSObservableProperty observedAnchorXProperty;
        //anchorY
        QSharedPointer<HSSParserNode> dAnchorY;
        HSSUnit anchorY;
        bool _anchorYdefault;
        QSharedPointer<HSSObservable> observedAnchorY;
        HSSObservableProperty observedAnchorYProperty;
        //flow
        QSharedPointer<HSSParserNode> dFlow;
        bool flow;
        bool does_float;
        //contained
        QSharedPointer<HSSParserNode> dContained;
        bool contained;
        //alignX
        QSharedPointer<HSSParserNode> dAlignX;
        HSSUnit alignX;
        QSharedPointer<HSSObservable> observedAlignX;
        HSSObservableProperty observedAlignXProperty;
        //alignY
        QSharedPointer<HSSParserNode> dAlignY;
        HSSUnit alignY;
        QSharedPointer<HSSObservable> observedAlignY;
        HSSObservableProperty observedAlignYProperty;
        //background
        QSharedPointer<HSSParserNode> dBackground;
        QSharedPointer<HSSObservable> observedBackground;
        HSSObservableProperty observedBackgroundProperty;
        std::vector<QSharedPointer<HSSObject> > background;
        //content
        QSharedPointer<HSSParserNode> dContent;
        QSharedPointer<HSSObservable> observedContent;
        HSSObservableProperty observedContentProperty;
        std::vector<QSharedPointer<HSSObject> > content;
        //font
        QSharedPointer<HSSParserNode> dFont;
        QSharedPointer<HSSObservable> observedFont;
        HSSObservableProperty observedFontProperty;
        std::vector<QSharedPointer<HSSFont> > font;

        //on
        QSharedPointer<HSSParserNode> dOn;
        QSharedPointer<HSSObservable> observedOn;
        HSSObservableProperty observedOnProperty;
        QMap<HSSEventType, std::vector<QSharedPointer<HSSObject> > > on;

        //margin
        QSharedPointer<HSSParserNode> dMargin;
        QSharedPointer<HSSObservable> observedMargin;
        HSSObservableProperty observedMarginProperty;
        std::vector<QSharedPointer<HSSMargin> > margin;

        //padding
        QSharedPointer<HSSParserNode> dPadding;
        QSharedPointer<HSSObservable> observedPadding;
        HSSObservableProperty observedPaddingProperty;
        std::vector<QSharedPointer<HSSMargin> > padding;

        //border
        QSharedPointer<HSSParserNode> dBorder;
        QSharedPointer<HSSObservable> observedBorder;
        HSSObservableProperty observedBorderProperty;
        std::vector<QSharedPointer<HSSBorder> > border;
        HSSUnit borderBleeding;

        //visible
        QSharedPointer<HSSParserNode> dVisible;
        QSharedPointer<HSSObservable> observedVisible;
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

        size_t _index;

        QSharedPointer<HSSDisplayObject> shared_from_this();

    protected:
        /**
         *  Copy constructor for HSSDisplayObject objects.
         */
        HSSDisplayObject(const HSSDisplayObject & orig);

        /**
         *  Constructor for HSSDisplayObject objects.
         *  @param type     Subclasses pass their own type here.
         */
        HSSDisplayObject(HSSObjectType type, AXRController * controller);

        void _setInnerWidth();
        void _setInnerHeight();
        void _setOuterWidth();
        void _setOuterHeight();

    private:
        HSSUnit _evaluatePropertyValue(
                                   void(HSSDisplayObject::*callback)(HSSObservableProperty property, void* data),
                                   QSharedPointer<HSSParserNode> value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedProperty,
                                   QSharedPointer<HSSObservable> observedObject,
                                   HSSObservableProperty observedSourceProperty,
                                   QSharedPointer<HSSObservable> &observedStore,
                                   HSSObservableProperty &observedStoreProperty,
                                   QSharedPointer<HSSSimpleSelection> scope
                                   );
        bool _isHover;
        bool _isPress;
        bool _isRoot;

        bool _layoutFlagIsInSecondaryGroup;
        bool _layoutFlagIsInSecondaryLine;
        bool _layoutFlagVerticallyAligned;
        bool _layoutFlagLockTop;
        HSSUnit _layoutLockTopPosition;
        bool _layoutFlagLockBottom;
        HSSUnit _layoutLockBottomPosition;
        std::string _debugName;
    };
}

Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSMargin> > *)

#endif
