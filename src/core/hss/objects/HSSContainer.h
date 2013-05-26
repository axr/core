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

#ifndef HSSCONTAINER_H
#define HSSCONTAINER_H

#include "HSSDisplayObject.h"
#include "HSSSimpleSelection.h"
#include "HSSTextEnums.h"

namespace AXR
{
    enum AXR_API HSSDirectionValue
    {
        HSSDirectionLeftToRight,
        HSSDirectionRightToLeft,
        HSSDirectionTopToBottom,
        HSSDirectionBottomToTop
    };

    class HSSShape;
    class HSSLayoutLine;

    /**
     *  @brief The object type representing an element in the XML content.
     *
     *  Each element in the XML code is represented as a HSSContainer, which is
     *  a type of display object that can contain other display object. It is capable of
     *  laying them out and to propagate events to them. It also implements the needed
     *  rendering routines for this type of object.
     */
    class AXR_API HSSContainer : public HSSDisplayObject
    {
    public:
        friend class HSSDisplayObject;
        friend class HSSLayoutLine;
        friend class HSSRenderer;

        /**
         *  Utility function for casting a shared pointer to a container to a display object.
         *  @param theContainer A shared pointer to the container.
         *  @return A shared pointer to the container as display object type.
         */
        static QSharedPointer<HSSDisplayObject> asDisplayObject(QSharedPointer<HSSContainer> theContainer);

        /**
         *  Utility function for casting a shared pointer to a display object to a container.
         *  @param theDisplayObject A shared pointer to the container as display object type.
         *  @return A shared pointer to the container
         *  @warning Be sure to only call this on shared pointers that you know for sure
         *  to actually be containers.
         */
        static QSharedPointer<HSSContainer> asContainer(QSharedPointer<HSSDisplayObject> theDisplayObject);

        /**
         *  Creates a newly allocated and initialized instance of a container.
         */
        HSSContainer(AXRController * controller);

        /**
         *  Initializes all ivars to default values.
         */
        void initialize();

        /**
         *  Copy constructor for HSSContainer objects. It will initialize the new object. See
         *  HSSDisplayObject for the copy constructor of the base class. Do not call directly,
         *  use clone() instead.
         */
        HSSContainer(const HSSContainer & orig);

        /**
         *  Clones an instance of HSSContainer and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSContainer
         */
        QSharedPointer<HSSContainer> clone() const;

        /**
         *  Destructs the container, clearing the children and allChildren.
         */
        virtual ~HSSContainer();

        virtual AXRString toString();
        virtual AXRString _toJSON(AXRString whitespace);
        virtual AXRString defaultObjectType();
        virtual AXRString defaultObjectType(AXRString property);
        virtual bool isKeyword(AXRString value, AXRString property);

        /**
         *  Add a child to the content tree.
         *  @param child    A shared pointer to the child display object.
         */
        void add(QSharedPointer<HSSDisplayObject> child);

        /**
         *  Removes a child by index.
         *  @param index    An unsigned int containing the index of the child to be removed.
         */
        void remove(size_t index);

        /**
         *  Removes all children from this container.
         */
        void clear();

        /**
         *  Recalculate the index of each child, to be called after the list of children has
         *  been altered.
         */
        void resetChildrenIndexes();

        /**
         *  If the last child is a text block, it sets the given text to the text content
         *  of the text block, or creates a new one if needed. It will trim the whitespace
         *  at the beginning and end of the text.
         *  @param text     A string containing the new content text.
         */
        virtual void setContentText(const AXRString &text);

        /**
         *  If the last child is a text block, it appends the given text to the text content
         *  of the text block, or creates a new one if needed. It will trim the whitespace
         *  at the beginning and end of the text.
         *  @param text     A string containing the new content text.
         */
        virtual void appendContentText(const AXRString &text);

        /**
         *  @return The content text of the element (potentially made up by various text blocks).
         *  @warning Currently unimplemented.
         */
        virtual AXRString getContentText();

        //void readDefinitionObjects();
        /**
         *  Propagates the readDefinitionObjects() call to all the children.
         */
        void recursiveReadDefinitionObjects();

        virtual void setProperty(HSSObservableProperty name, QSharedPointer<HSSParserNode> value);

        /**
         *  Accepts a visitor to traverse and process the HSSObject tree.
         */
        virtual void accept(HSSAbstractVisitor* visitor, bool traverse);

        /**
         *  Lays out the children according to the layout algorithm, first in the primary direction
         *  and then in the secondary one.
         */
        void layout();

        /**
         *  Propagates the layout() call to all the children.
         */
        void recursiveLayout();

        /**
         *  Recursively resets the layout of all the children.
         */
        void recursiveResetLayout();

        /**
         *  Sets the global horizontal position, which will be used to determine where in the
         *  window the container will be placed. It will also adjust the position of the children.
         *  @param newValue     An HSSUnit containing the new horizontal position.
         */
        virtual void setGlobalX(HSSUnit newValue);

        /**
         *  Sets the global vertical position, which will be used to determine where in the
         *  window the container will be placed. It will also adjust the position of the children.
         *  @param newValue     An HSSUnit containing the new vertical position.
         */
        virtual void setGlobalY(HSSUnit newValue);

        /**
         *  Replaces the whole list of children with a new one.
         *  @param newChildren  A vector of shared pointers to display objects.
         */
        void setChildren(QSharedPointer<HSSSimpleSelection> newChildren);

        /**
         *  @return The list of children, excluding text blocks.
         */
        QSharedPointer<HSSSimpleSelection> getChildren() const;

        /**
         *  @param includeTextBlocks    Wether to include the text blocks or just actual children.
         *  @return The list of children.
         */
        QSharedPointer<HSSSimpleSelection> getChildren(bool includeTextBlocks) const;

        /**
         *  Getter for the definition object of contentAlignX.
         *  @return A shared pointer to the parser node containing the definition object of contentAlignX.
         */
        QSharedPointer<HSSParserNode> getDContentAlignX();

        /**
         *  Setter for the definition object of contentAlignX. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of contentAlignX.
         */
        void setDContentAlignX(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect contentAlignX.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void contentAlignXChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the definition object of contentAlignY.
         *  @return A shared pointer to the parser node containing the definition object of contentAlignY.
         */
        QSharedPointer<HSSParserNode> getDContentAlignY();

        /**
         *  Setter for the definition object of contentAlignY. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of contentAlignY.
         */
        void setDContentAlignY(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect contentAlignX.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void contentAlignYChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the definition object of directionPrimary.
         *  @return A shared pointer to the parser node containing the definition object of directionPrimary.
         */
        QSharedPointer<HSSParserNode> getDDirectionPrimary();

        /**
         *  Setter for the definition object of directionPrimary. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of directionPrimary.
         */
        void setDDirectionPrimary(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect directionPrimary.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void directionPrimaryChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the definition object of directionSecondary.
         *  @return A shared pointer to the parser node containing the definition object of directionSecondary.
         */
        QSharedPointer<HSSParserNode> getDDirectionSecondary();

        /**
         *  Setter for the definition object of directionSecondary. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of
         *                  directionSecondary.
         */
        void setDDirectionSecondary(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect directionSecondary.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void directionSecondaryChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the value of shape.
         *  @return A shared pointer to the shape object.
         */
        QSharedPointer<HSSShape> getShape();

        /**
         *  Getter for the definition object of shape.
         *  @return A shared pointer to the parser node containing the definition object of shape.
         */
        QSharedPointer<HSSParserNode> getDShape();

        /**
         *  Setter for the definition object of shape. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of shape
         */
        void setDShape(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect shape.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void shapeChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the actual value of tetxAlign
         *  @return The HSSTextAlignType that is currently being used
         */
        HSSTextAlignType getTextAlign();

        /**
         *  Getter for the definition object of textAlign.
         *  @return A shared pointer to the parser node containing the definition object of textAlign.
         */
        QSharedPointer<HSSParserNode> getDTextAlign();

        /**
         *  Setter for the definition object of textAlign. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of textAlign.
         */
        void setDTextAlign(QSharedPointer<HSSParserNode> value);

        /**
         *  Method to be passed as callback when observing changes that will affect textAlign.
         *  @param source   The property which we are observing.
         *  @param data     A regular pointer to the data that is sent along the notification.
         */
        void textAlignChanged(HSSObservableProperty source, void*data);

        /**
         *  Sets default values for this object type. Should be called before applying other property
         *  definitions.
         */
        void setDefaults();

        /**
         *  Propagates the call to handleEvent() to all children and to the base class implementation.
         *  @param event    The event that is being sent.
         *  @return Whether it has been handled or not.
         */
        virtual bool handleEvent(HSSInputEvent *event);

        /**
         *  Stores the pointer to the controller and propagates it to all children.
         *  @param controller   A regular pointer to the controller that owns this object.
         */
        virtual void setController(AXRController * controller);

    protected:
        QSharedPointer<HSSSimpleSelection> children;
        QSharedPointer<HSSSimpleSelection> allChildren;

        QSharedPointer<HSSParserNode> dContentAlignX;
        HSSUnit contentAlignX;
        HSSObservable * observedContentAlignX;
        HSSObservableProperty observedContentAlignXProperty;
        bool distributeX;
        bool distributeXLinear;
        QSharedPointer<HSSParserNode> dContentAlignY;
        HSSUnit contentAlignY;
        HSSObservable * observedContentAlignY;
        HSSObservableProperty observedContentAlignYProperty;
        bool distributeY;
        bool distributeYLinear;

        QSharedPointer<HSSParserNode> dDirectionPrimary;
        HSSDirectionValue directionPrimary;
        HSSObservable * observedDirectionPrimary;
        HSSObservableProperty observedDirectionPrimaryProperty;
        QSharedPointer<HSSParserNode> dDirectionSecondary;
        HSSDirectionValue directionSecondary;
        HSSObservable * observedDirectionSecondary;
        HSSObservableProperty observedDirectionSecondaryProperty;
        QSharedPointer<HSSParserNode> dShape;
        QSharedPointer<HSSShape> shape;
        HSSObservable * observedShape;
        HSSObservableProperty observedShapeProperty;

        //textAlign
        HSSTextAlignType textAlign;
        QSharedPointer<HSSParserNode> dTextAlign;
        HSSObservable * observedTextAlign;
        HSSObservableProperty observedTextAlignProperty;

        QSharedPointer<HSSContainer> shared_from_this();

    private:
        HSSUnit _evaluatePropertyValue(
                                   void(HSSContainer::*callback)(HSSObservableProperty property, void* data),
                                   QSharedPointer<HSSParserNode> value,
                                   HSSUnit percentageBase,
                                   HSSObservableProperty observedProperty,
                                   HSSObservable * observedObject,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty,
                                   QSharedPointer<HSSSimpleSelection> scope
                                   );

        inline bool _overlaps(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const;
        inline bool _overlaps_horizontal(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const;
        inline bool _overlaps_vertical(const QSharedPointer<HSSDisplayObject> & childA, const QSharedPointer<HSSDisplayObject> & childB) const;

        virtual QSharedPointer<HSSClonable> cloneImpl() const;

        inline void _placeOnAlignmentPoint(const QSharedPointer<HSSDisplayObject> & child);
        inline bool _layoutTick() const;
        inline void _setGlobalPosition(const QSharedPointer<HSSDisplayObject> & child) const;
        inline const QSharedPointer<HSSLayoutLine> _getTargetGroup(const QSharedPointer<HSSDisplayObject> & child, std::vector<QSharedPointer<HSSLayoutLine> > & groups) const;
        inline void _setPositions(const QSharedPointer<HSSLayoutLine> & group) const;
        inline void _lineArrangeX(const QSharedPointer<HSSDisplayObject> & child, HSSUnit startX, HSSUnit lineWidth) const;
        inline void _lineArrangeY(const QSharedPointer<HSSDisplayObject> & child, HSSUnit startY, HSSUnit lineHeight) const;
        inline bool _arrangeLines(const QSharedPointer<HSSLayoutLine> &group, const HSSDirectionValue direction) const;
        inline bool _distributeLines(const QSharedPointer<HSSLayoutLine> &group, const HSSDirectionValue direction) const;
        inline HSSUnit _getLineHeight(const QSharedPointer<HSSLayoutLine> & line) const;
        static bool alignXSmaller(const QSharedPointer<HSSDisplayObject>& a, const QSharedPointer<HSSDisplayObject>& b)
        {
            return a->alignX < b->alignX;
        }
        static bool alignYSmaller(const QSharedPointer<HSSDisplayObject>& a, const QSharedPointer<HSSDisplayObject>& b)
        {
            return a->alignY < b->alignY;
        }
    };
}

Q_DECLARE_METATYPE(AXR::HSSDirectionValue)
Q_DECLARE_METATYPE(AXR::HSSDirectionValue*)

#endif
