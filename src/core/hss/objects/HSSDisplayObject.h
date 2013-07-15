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
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axrproject.org
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

namespace AXR
{
    class HSSAbstractVisitor;
    class HSSBorder;
    class HSSContainer;
    class HSSFlag;
    class HSSFont;
    class HSSInputEvent;
    class HSSMargin;
    class HSSMouseEvent;
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
        friend class HSSLayoutLine;
        friend class HSSLayout;
        friend class HSSRenderer;

        /**
         *  The "parent pointer", a weak variant of a shared pointer, to break reference cycles.
         */
        typedef QWeakPointer<HSSContainer> pp;

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
        void rulesAddIsAChildren(QSharedPointer<HSSPropertyDefinition> propdef, HSSRuleState defaultState, QSharedPointer<HSSRule> & parentRule);
        QSharedPointer<HSSRule> rulesGet(unsigned index);
        void rulesRemove(unsigned index);
        void rulesRemoveLast();
        size_t rulesSize() const;
        void setRuleStatus(QSharedPointer<HSSRule> rule, HSSRuleState newValue);
        bool hasRule(QSharedPointer<HSSRule> rule);
        void setNeedsRereadRules(bool value);
        bool needsRereadRules();

        void setNeedsSurface(bool value);
        bool needsSurface();

        void setDirty(bool value);
        bool isDirty();
        virtual void accept(HSSAbstractVisitor* visitor, bool traverse);

        void setNeedsLayout(bool value);
        bool needsLayout() const;
        virtual void recursiveResetLayout();
        virtual void setGlobalX(HSSUnit newValue);
        virtual void setGlobalY(HSSUnit newValue);

        AXRString getElementName();
        void setElementName(AXRString name);

        virtual bool handleEvent(HSSInputEvent *event);
        virtual bool handleMouseEvent(HSSMouseEvent *event);

        void setHover(bool newValue);
        bool isHover();

        void setPress(bool newValue);
        bool isPress();

        void ruleChanged(const AXRString source, const QSharedPointer<HSSObject> theObj);

        void createFlag(QSharedPointer<HSSFlag> flag, HSSRuleState defaultValue);
        bool hasFlag(AXRString name);
        HSSRuleState flagState(AXRString name);
        void flagsActivate(AXRString name);
        void flagsDeactivate(AXRString name);
        void flagsToggle(AXRString name);

        bool isRoot();
        void setRoot(bool newValue);

        bool hasOwnWidth() const;
        void setHasOwnWidth(bool newValue);
        bool hasOwnHeight() const;
        void setHasOwnHeight(bool newValue);

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
        /**
         *  @todo add "bounds" property
         */
        HSSUnit x, y, globalX, globalY;
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

        void _setInnerWidth(bool notify = true);
        void _setInnerHeight(bool notify = true);
        void _setOuterWidth(bool notify = true);
        void _setOuterHeight(bool notify = true);

    private:
        bool _isHover;
        bool _isPress;
        bool _isRoot;
        bool _hasOwnWidth;
        bool _hasOwnHeight;

        bool _layoutFlagIsInSecondaryGroup;
        bool _layoutFlagIsInSecondaryLine;
        bool _layoutFlagVerticallyAligned;
        bool _layoutFlagLockTop;
        HSSUnit _layoutLockTopPosition;
        bool _layoutFlagLockBottom;
        HSSUnit _layoutLockBottomPosition;
        std::string _debugName;

        void _initialize();
    };
}

Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSFont> > *)
Q_DECLARE_METATYPE(std::vector<QSharedPointer<AXR::HSSMargin> > *)

#endif
