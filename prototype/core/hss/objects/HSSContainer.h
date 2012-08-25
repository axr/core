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
 *      COPYRIGHT: ©2012 - All Rights Reserved
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

#ifndef HSSCONTAINER_H
#define HSSCONTAINER_H

#include <list>
#include <cairo/cairo.h>
#include "HSSDisplayObject.h"
#include "HSSEvent.h"
#include "HSSShape.h"
#include "HSSTextBlock.h"

namespace AXR
{

    enum HSSDirectionValue
    {
        HSSDirectionLeftToRight,
        HSSDirectionRightToLeft,
        HSSDirectionTopToBottom,
        HSSDirectionBottomToTop
    };

    /**
     *  @brief The object type representing an element in the XML content.
     *
     *  Each element in the XML code is represented as a HSSContainer, which is
     *  a type of display object that can contain other display object. It is capable of
     *  laying them out and to propagate events to them. It also implements the needed
     *  rendering routines for this type of object.
     */
    class HSSContainer : public HSSDisplayObject
    {
    public:
        typedef boost::shared_ptr<HSSContainer> p;

        friend class HSSDisplayObject;

        /**
         *  @brief Display groups are used by HSSContainer to lay out elements according
         *  to the layout algorithm. They represent a set of elements that are flowing
         *  together in a direction. They also contain lines, each of whose is also a
         *  display group. When there are lines, the objects vector should not be used.
         */
        class displayGroup
        {
        public:
            typedef boost::shared_ptr<HSSContainer::displayGroup> p;
            HSSUnit x;
            HSSUnit y;
            HSSUnit width;
            HSSUnit height;
            std::string name;
            bool complete;
            std::vector<HSSContainer::displayGroup::p>lines;
            std::vector<HSSDisplayObject::p>objects;
        };

        /**
         *  Utility function for casting a shared pointer to a container to a display object.
         *  @param theContainer A shared pointer to the container.
         *  @return A shared pointer to the container as display object type.
         */
        static HSSDisplayObject::p asDisplayObject(HSSContainer::p theContainer);

        /**
         *  Utility function for casting a shared pointer to a display object to a container.
         *  @param theDisplayObject A shared pointer to the container as display object type.
         *  @return A shared pointer to the container
         *  @warning Be sure to only call this on shared pointers that you know for sure
         *  to actually be containers.
         */
        static HSSContainer::p asContainer(HSSDisplayObject::p theDisplayObject);

        /**
         *  Creates a newly allocated and initialized instance of a container.
         */
        HSSContainer();

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
        p clone() const;

        /**
         *  Destructs the container, clearing the children and allChildren.
         */
        virtual ~HSSContainer();

        virtual std::string toString();
        virtual std::string defaultObjectType();
        virtual std::string defaultObjectType(std::string property);
        virtual bool isKeyword(std::string value, std::string property);

        /**
         *  Add a child to the content tree.
         *  @param child    A shared pointer to the child display object.
         */
        void add(HSSDisplayObject::p child);

        /**
         *  Removes a child by index.
         *  @param index    An unsigned int containing the index of the child to be removed.
         */
        void remove(unsigned index);

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
        virtual void setContentText(std::string text);

        /**
         *  If the last child is a text block, it appends the given text to the text content
         *  of the text block, or creates a new one if needed. It will trim the whitespace
         *  at the beginning and end of the text.
         *  @param text     A string containing the new content text.
         */
        virtual void appendContentText(std::string text);

        /**
         *  @return The content text of the element (potentially made up by various text blocks).
         *  @warning Currently unimplemented.
         */
        virtual std::string getContentText();

        //void readDefinitionObjects();
        /**
         *  Propagates the readDefinitionObjects() call to all the children.
         */
        void recursiveReadDefinitionObjects();

        virtual void setProperty(HSSObservableProperty name, HSSParserNode::p value);

        /**
         *  Propagates the regenerateSurfaces() call to all the children.
         */
        void recursiveRegenerateSurfaces();

        /**
         *  Propagates the draw() call to all the children.
         *  @param cairo    A regular pointer to the cairo handle used for drawing.
         */
        void recursiveDraw(cairo_t * cairo);

        /**
         *  Sets the shape of the container and then calls the base class method for actually
         *  drawing.
         */
        virtual void drawBackground();

        /**
         *  Calculates the position for each border and then draws them.
         */
        virtual void drawBorders();

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
        void setChildren(std::vector<HSSDisplayObject::p> newChildren);

        /**
         *  @return The list of children, excluding text blocks.
         */
        const std::vector<HSSDisplayObject::p>& getChildren() const;

        /**
         *  @param includeTextBlocks    Wether to include the text blocks or just actual children.
         *  @return The list of children.
         */
        const std::vector<HSSDisplayObject::p>& getChildren(bool includeTextBlocks) const;

        /**
         *  Getter for the definition object of contentAlignX.
         *  @return A shared pointer to the parser node containing the definition object of contentAlignX.
         */
        HSSParserNode::p getDContentAlignX();

        /**
         *  Setter for the definition object of contentAlignX. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of contentAlignX.
         */
        void setDContentAlignX(HSSParserNode::p value);

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
        HSSParserNode::p getDContentAlignY();

        /**
         *  Setter for the definition object of contentAlignY. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of contentAlignY.
         */
        void setDContentAlignY(HSSParserNode::p value);

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
        HSSParserNode::p getDDirectionPrimary();

        /**
         *  Setter for the definition object of directionPrimary. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of directionPrimary.
         */
        void setDDirectionPrimary(HSSParserNode::p value);

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
        HSSParserNode::p getDDirectionSecondary();

        /**
         *  Setter for the definition object of directionSecondary. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of
         *                  directionSecondary.
         */
        void setDDirectionSecondary(HSSParserNode::p value);

        /**
         *  Method to be passed as callback when observing changes that will affect directionSecondary.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void directionSecondaryChanged(HSSObservableProperty source, void*data);

        /**
         *  Getter for the definition object of shape.
         *  @return A shared pointer to the parser node containing the definition object of shape.
         */
        HSSParserNode::p getDShape();

        /**
         *  Setter for the definition object of shape. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of shape
         */
        void setDShape(HSSParserNode::p value);

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
        HSSParserNode::p getDTextAlign();

        /**
         *  Setter for the definition object of textAlign. It will use the value as needed.
         *  @param value    A shared pointer to the parser node containing the definition object of textAlign.
         */
        void setDTextAlign(HSSParserNode::p value);

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
         *  @param type     The type of the event that is being sent.
         *  @param data     A regular pointer to the data that is sent along with the event (coordinates,
         *                  for example).
         *  @return Wether it has been handled or not.
         */
        virtual bool handleEvent(HSSEventType type, void* data);

        /**
         *  Stores the pointer to the controller and propagates it to all children.
         *  @param controller   A regular pointer to the controller that owns this object.
         */
        virtual void setController(AXRController * controller);

    protected:
        std::vector<HSSDisplayObject::p>children;
        std::vector<HSSDisplayObject::p>allChildren;

        HSSParserNode::p dContentAlignX;
        HSSUnit contentAlignX;
        HSSObservable * observedContentAlignX;
        HSSObservableProperty observedContentAlignXProperty;
        bool distributeX;
        bool distributeXLinear;
        HSSParserNode::p dContentAlignY;
        HSSUnit contentAlignY;
        HSSObservable * observedContentAlignY;
        HSSObservableProperty observedContentAlignYProperty;
        bool distributeY;
        bool distributeYLinear;

        HSSParserNode::p dDirectionPrimary;
        HSSDirectionValue directionPrimary;
        HSSObservable * observedDirectionPrimary;
        HSSObservableProperty observedDirectionPrimaryProperty;
        HSSParserNode::p dDirectionSecondary;
        HSSDirectionValue directionSecondary;
        HSSObservable * observedDirectionSecondary;
        HSSObservableProperty observedDirectionSecondaryProperty;
        HSSParserNode::p dShape;
        HSSShape::p shape;
        HSSObservable * observedShape;
        HSSObservableProperty observedShapeProperty;

        //textAlign
        HSSTextAlignType textAlign;
        HSSParserNode::p dTextAlign;
        HSSObservable * observedTextAlign;
        HSSObservableProperty observedTextAlignProperty;

        HSSContainer::p shared_from_this();

    private:
        long double _setLDProperty(
                                   void(HSSContainer::*callback)(HSSObservableProperty property, void* data),
                                   HSSParserNode::p value,
                                   long double percentageBase,
                                   HSSObservableProperty observedProperty,
                                   HSSObservable * observedObject,
                                   HSSObservableProperty observedSourceProperty,
                                   HSSObservable * &observedStore,
                                   HSSObservableProperty &observedStoreProperty,
                                   const std::vector<HSSDisplayObject::p> * scope
                                   );

        void _recursiveCreateSecondaryGroups(
                                             std::vector<HSSDisplayObject::p>::iterator lineAIt,
                                             std::vector<HSSDisplayObject::p>::iterator lineAStopIt,
                                             std::vector<HSSDisplayObject::p>::iterator lineBIt,
                                             std::vector<HSSDisplayObject::p>::iterator lineBStopIt,
                                             displayGroup::p &targetA, displayGroup::p &targetB,
                                             displayGroup::p &newGroup,
                                             std::vector<displayGroup::p>::iterator pglIt,
                                             std::vector<displayGroup::p>::iterator pglStopIt,
                                             bool addToSecondaryGroups,
                                             std::vector<displayGroup::p> &secondaryGroups,
                                             bool needsShoveling,
                                             bool onlyAddToBIfNotInGroupYet
                                             );

        void _qs_swap(std::vector<displayGroup::p> &arr, int a, int b);
        int _qs_partition(std::vector<displayGroup::p> &arr, int left, int right, int pivotIndex);
        void _qs_sort(std::vector<displayGroup::p> &arr, int left, int right);

        bool _overlaps(HSSDisplayObject::p & childA, HSSDisplayObject::p & childB);
        bool _overlaps_horizontal(HSSDisplayObject::p & childA, HSSDisplayObject::p & childB);
        bool _overlaps_vertical(HSSDisplayObject::p & childA, HSSDisplayObject::p & childB);
        bool _addChildToGroupIfNeeded(HSSDisplayObject::p &child, displayGroup::p &group, HSSDirectionValue direction, bool overflow);
        //std::vector<HSSContainer::displayGroup::p> _getGroupsOverlapping(HSSDisplayObject::p &child, std::vector<HSSContainer::displayGroup::p> &group, HSSDirectionValue direction);
        bool _mergeGroupsIfNeeded(displayGroup::p &group, displayGroup::p &otherGroup, HSSDirectionValue direction);
        void _arrange(displayGroup::p &groups, HSSDirectionValue direction);
        bool _arrangeLines(displayGroup::p &groups, HSSDirectionValue direction, bool isFirstGroup);
        void _recursiveGetPushGroup(HSSDisplayObject::p objA, HSSDisplayObject::p objB, std::vector<displayGroup::p>::iterator linesIt, std::vector<displayGroup::p>::iterator stopIt, displayGroup::p &ret);
        void _distribute(displayGroup::p &groups, HSSDirectionValue direction);
        bool _recursiveFindTopConstraint(long double & constraint, displayGroup::p group, int i, HSSDisplayObject::p child);
        bool _recursiveFindBottomConstraint(long double & constraint, displayGroup::p group, int i, HSSDisplayObject::p child);
        HSSClonable::p cloneImpl() const;

    };
}

#endif
