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

#ifndef HSSFUNCTION_H
#define HSSFUNCTION_H

#include "HSSParserNode.h"

namespace AXR
{
    class HSSSimpleSelection;

    /**
     *  @brief Abstract base class for all function objects.
     */
    class AXR_API HSSFunction : public HSSParserNode
    {
    public:
        friend class HSSCodeParser;

        /**
         *  Clones an instance of HSSFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFunction
         */
        virtual QSharedPointer<HSSFunction> clone() const;

        /**
         *  Destructor for this class.
         */
        virtual ~HSSFunction();

        virtual AXRString toString();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        virtual bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        /**
         *  Evaluate the function. The actual implementation is up to each specific subclass.
         *  @return A void pointer to some data. It's up to the caller to know what type of
         *  data is expected.
         */
        QSharedPointer<HSSObject> evaluate();

        /**
         *  @todo I think this one is not used
         */
        QSharedPointer<HSSObject> evaluate(std::deque<QSharedPointer<HSSParserNode> > arguments);

        /**
         *  @todo make protected or private
         */
        virtual QSharedPointer<HSSObject> _evaluate();

        /**
         *  Method to be passed as callback when observing changes.
         *  @param property The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        virtual void propertyChanged(const AXRString property, const AXRString target, QSharedPointer<HSSObject> theObj);

        /**
         *  Setter for the scope which to pass to members like references or selections.
         *  @param newScope     The new scope, a shared pointer to a simple selection.
         */
        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);
        
        /**
         *  Setter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @param value        A shared pointer to the nearest display object.
         */
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> newThis);

        /**
         *  Whenever a function needs to recalculate its value, it should be set to true.
         *  @param value    A boolean, wether it needs to recalculate or not.
         */
        void setDirty(bool value);

        /**
         *  Tells if the function needs to recalculate its value.
         *  @return Wether it needs recalculating or not.
         */
        bool isDirty();

        /**
         *  Getter for the value.
         *  @return A pointer to the object returned by the function.
         */
        QSharedPointer<HSSObject> getValue();

        /**
         *  Allows you to check if this function is of the given type.
         *  @param  type    The function type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        virtual bool isA(HSSFunctionType type) const;

        /**
         *  @return The function type of this instance.
         */
        HSSFunctionType getFunctionType() const;

        std::deque<QSharedPointer<HSSParserNode> > getArguments();
        void setArguments(std::deque<QSharedPointer<HSSParserNode> > arguments);
        void addArgument(QSharedPointer<HSSParserNode> argument);
        AXRString getName();
        void setName(AXRString newName);
        
        virtual void fireTimer(AXRString timerName);

    protected:
        /**
         *  Creates a new instance of a function, with given function type. Most of the time this
         *  will be HSSFunctionTypeCustom.
         *  @param type The type of the function subclass for identification purposes.
         */
        HSSFunction(HSSFunctionType type, AXRController * controller);

        /**
         *  Copy constructor for HSSFunction objects. Do not call directly, use clone() on
         *  on of the subclasses instead.
         */
        HSSFunction(const HSSFunction & orig);

        bool _isDirty;
        QSharedPointer<HSSObject> _value;

        QSharedPointer<HSSSimpleSelection> scope;

    private:
        HSSFunctionType functionType;
        std::deque<QSharedPointer<HSSParserNode> > _arguments;
        AXRString _name;

        virtual QSharedPointer<HSSClonable> cloneImpl() const;
    };
}

#endif
