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

#ifndef HSSPROPERTYPATH_H
#define HSSPROPERTYPATH_H

#include <deque>
#include "HSSScopedParserNode.h"

namespace AXR
{
    class HSSObject;
    class HSSPropertyPathNode;

    /**
     *  @brief A property path contains multiple property path nodes.
     */
    class AXR_API HSSPropertyPath : public HSSScopedParserNode
    {
    public:
        struct query
        {
            QSharedPointer<HSSObject> value;
            QSharedPointer<HSSObject> object;
            AXRString propertyName;
        };

        /**
         *  Creates a new instance of a property path.
         */
        HSSPropertyPath(AXRController * controller);

        /**
         *  Copy constructor for HSSPropertyPath objects
         */
        HSSPropertyPath(const HSSPropertyPath & orig);

        /**
         *  Clones an instance of HSSPropertyPath and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSPropertyPath
         */
        QSharedPointer<HSSPropertyPath> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSPropertyPath();

        virtual AXRString toString();
        virtual AXRString stringRep();
        
        AXRString toKeyString() const;

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);
        
        /**
         *  Sets the initial and end iterators
         */
        void initialize();
        void goToNext();

        void setStackNode(QSharedPointer<HSSObject> object, QSharedPointer<HSSParserNode> value,  bool initializing = true);
        void applyModifier(QSharedPointer<HSSObject> object, QSharedPointer<HSSParserNode> value, bool initializing = true);
        query getComputed(QSharedPointer<HSSObject> object);

        /**
         *  Getter for nodes.
         *  @return A vector of shared pointers to parser nodes that are stored inside this
         *  property path.
         */
        std::deque<QSharedPointer<HSSPropertyPathNode> > getNodes();

        /**
         *  Setter for nodes.
         *  @param newNodes    A vector of shared pointers to parser nodes that to store them inside this
         *  property path.
         */
        void setNodes(std::deque<QSharedPointer<HSSPropertyPathNode> > newNodes);

        /**
         *  Adds a node to the nodes list.
         *  @param newValue     The path node to be added.
         */
        void add(QSharedPointer<HSSPropertyPathNode> newValue);

        /**
         *  Convenience mode to add a string directly.
         *  @param newValue     A string with the property name.
         */
        void add(AXRString newValue);

        void prepend(QSharedPointer<HSSPropertyPathNode> newValue);
        void prepend(AXRString newValue);

        /**
         *  Returns the first value from the nodes.
         *  @param newNode    A shared pointer to the first path node.
         */
        QSharedPointer<HSSPropertyPathNode> front() const;

        /**
         *  How many nodes are in the path.
         *  @return The number of nodes.
         */
        size_t size() const;

        /**
         *  Removes and returns the first element in the nodes list.
         *  @return The first element that was removed from the list.
         */
        QSharedPointer<HSSPropertyPathNode> popFront();

        QSharedPointer<HSSPropertyPath> cloneFromCurrentIterator();

        QSharedPointer<HSSObject> evaluate();
        void observe();
        void observeVar(HSSString varName);
        void valueChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> remoteObj);
        void evaluateSet(QSharedPointer<HSSObject> theObj);
        
        void setThisObj(QSharedPointer<AXR::HSSDisplayObject> value);
        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);

    private:
        void _setModifiers(QSharedPointer<HSSObject> theObj, AXRString propertyName, QSharedPointer<HSSParserNode> value, QSharedPointer<HSSObject> baseObj);
        std::deque<QSharedPointer<HSSPropertyPathNode> > _nodes;
        std::deque<QSharedPointer<HSSPropertyPathNode> >::iterator _iterator;
        std::deque<QSharedPointer<HSSPropertyPathNode> >::iterator _endIterator;
        bool _initialized;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif
