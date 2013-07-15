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

#include <QVariant>
#include "HSSDisplayObject.h"
#include "HSSSimpleSelection.h"
#include "HSSTextEnums.h"

namespace AXR
{
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
        friend class HSSCascader;
        friend class HSSRenderer;
        friend class HSSLayout;

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

        virtual void setDefaults();
        virtual AXRString toString();
        virtual QVariantMap toMap() const;
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
         *  Accepts a visitor to traverse and process the HSSObject tree.
         */
        virtual void accept(HSSAbstractVisitor* visitor, HSSVisitorFilterFlags filterFlags);

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

        const HSSUnit getContentAlignX() const;
        QSharedPointer<HSSObject> computeContentAlignX(QSharedPointer<HSSParserNode> parserNode);
        void listenContentAlignX(QSharedPointer<HSSObject> theObj);
        const HSSUnit getContentAlignY() const;
        QSharedPointer<HSSObject> computeContentAlignY(QSharedPointer<HSSParserNode> parserNode);
        void listenContentAlignY(QSharedPointer<HSSObject> theObj);
        const HSSDirectionValue getDirection() const;
        const HSSDirectionValue getWrapDirection() const;
        //content
        const AXRString getContent() const;

        QSharedPointer<HSSObject> computeContent(QSharedPointer<HSSParserNode> parserNode);
        //shape
        QSharedPointer<HSSObject> computeShape(QSharedPointer<HSSParserNode> parserNode);
        /**
         *  Getter for the value of shape.
         *  @return A shared pointer to the shape object.
         */
        QSharedPointer<HSSShape> getShape();
        void listenShape(QSharedPointer<HSSObject> theObj);
        void notifyShape(QSharedPointer<HSSObject> theObj);

        void shapeChanged(const AXRString source, const QSharedPointer<HSSObject> theObj);

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
        bool distributeX;
        bool distributeXLinear;
        bool distributeY;
        bool distributeYLinear;

        QSharedPointer<HSSContainer> shared_from_this();

    private:
        void _initialize();
        virtual QSharedPointer<HSSClonable> cloneImpl() const;

        static bool alignXSmaller(const QSharedPointer<HSSDisplayObject>& a, const QSharedPointer<HSSDisplayObject>& b)
        {
            return a->getAlignX() < b->getAlignX();
        }
        static bool alignYSmaller(const QSharedPointer<HSSDisplayObject>& a, const QSharedPointer<HSSDisplayObject>& b)
        {
            return a->getAlignY() < b->getAlignY();
        }
    };
}

Q_DECLARE_METATYPE(AXR::HSSDirectionValue)
Q_DECLARE_METATYPE(AXR::HSSDirectionValue*)

#endif
