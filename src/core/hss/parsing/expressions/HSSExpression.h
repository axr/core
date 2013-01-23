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

#ifndef HSSEXPRESSION_H
#define HSSEXPRESSION_H

#include <QSharedPointer>
#include "HSSParserNode.h"

namespace AXR
{
    class HSSSimpleSelection;

    /**
     *  @brief Abstract base class for all expressions.
     */
    class AXR_API HSSExpression : public HSSParserNode
    {
    public:
        /**
         *  Destructor for this class.
         */
        virtual ~HSSExpression();
        AXRString toString();
        HSSUnit evaluate();

        /**
         *  Each subclass implements the specific operation that needs to be done.
         *
         *  @param leftval  A HSSUnit that holds the left value.
         *  @param rightval  A HSSUnit that holds the right value.
         */
        virtual HSSUnit calculate(HSSUnit leftval, HSSUnit rightval) = 0;

        bool isA(HSSExpressionType otherType);
        HSSExpressionType getExpressionType();

        /**
         *  When logging, you often need a string representation of the expression type.
         *  @param type     The expression type to represent as a string.
         *  @return A string representation of the given type.
         */
        static AXRString expressionTypeStringRepresentation(HSSExpressionType type);

        /**
         *  Setter for the left hand member of the expression.
         *  @param  newLeft     A shared pointer to a parser node representing the left
         *                      member of the expression.
         */
        void setLeft(QSharedPointer<HSSParserNode> newLeft);

        /**
         *  Getter for the left hand member of the expression.
         *  @return A shared pointer to a parser node representing the left member of
         *  the expression.
         */
        QSharedPointer<HSSParserNode> getLeft();

        /**
         *  Setter for the right hand member of the expression.
         *  @param  newRight    A shared pointer to a parser node representing the right
         *                      member of the expression.
         */
        void setRight(QSharedPointer<HSSParserNode> newRight);

        /**
         *  Getter for the right hand member of the expression.
         *  @return A shared pointer to a parser node representing the right member of
         *  the expression.
         */
        QSharedPointer<HSSParserNode> getRight();

        virtual void propertyChanged(HSSObservableProperty property, void* data);

        /**
         *  Method to be passed as callback when observing changes that will affect the left
         *  member of the expression.
         *  @param property The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        virtual void leftChanged(HSSObservableProperty property, void* data);

        /**
         *  Method to be passed as callback when observing changes that will affect the right
         *  member of the expression.
         *  @param property The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        virtual void rightChanged(HSSObservableProperty property, void* data);

        /**
         *  The percentage base is the number that corresponds to 100%.
         *  @param value    A HSSUnit containing the base for percentage calculations.
         */
        virtual void setPercentageBase(HSSUnit value);

        /**
         *  When we are using percentages, we keep track of the value and update accordingly.
         *  @param property     The property which we are observing.
         *  @param observed     A regular pointer to the object we are observing.
         *
         *  @todo what if both left and right are percentages?
         */
        virtual void setPercentageObserved(HSSObservableProperty property, HSSObservable * observed);

        /**
         *  Setter for the scope which to pass to members like references or selections.
         *  @param newScope     The new scope, a shared pointer to a simple selection.
         */
        virtual void setScope(QSharedPointer<HSSSimpleSelection> newScope);

        /**
         *  Setter for the "\@this object" which to pass to members like references or selections.
         *  @param value    A shared pointer to a display object representing the \@this object.
         */
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> value);

        /**
         *  Whenever an expression needs to recalculate its value, it should be set to true.
         *  @param value    A boolean, wether it needs to recalculate or not.
         */
        void setDirty(bool value);

        /**
         *  Tells if the expression needs to recalculate its value.
         *  @return Wether it needs recalculating or not.
         */
        bool isDirty();

        /**
         *  Setter for the value.
         *  @param newValue     A HSSUnit containing the new calculated value for this expression.
         */
        void setValue(HSSUnit newValue);

        /**
         *  Getter for the value.
         *  @return The calculated value of the expression.
         */
        HSSUnit getValue();

    protected:
        /**
         *  Creates a new instance of an expression.
         *  This class shouldn't be called directly, but by the subclasses.
         *  @param _left    The value on the left hand side of the operator.
         *  @param _right   The value on the right hand side of the operator.
         *  @param type     The type of this expression subclass for identification purposes.
         */
        HSSExpression(HSSExpressionType type, QSharedPointer<HSSParserNode> _left, QSharedPointer<HSSParserNode> _right, AXRController * controller);

        /**
         *  Copy constructor for HSSExpression objects. Do not call directly, use clone() instead.
         */
        HSSExpression(const HSSExpression &orig);

        QSharedPointer<HSSParserNode> left;
        HSSUnit leftval;
        HSSUnit rightval;
        QSharedPointer<HSSParserNode> right;

        HSSUnit percentageBase;
        HSSObservableProperty percentageObservedProperty;
        HSSObservable * percentageObserved;

        HSSObservableProperty leftObservedProperty;
        HSSObservable * leftObserved;
        HSSObservableProperty rightObservedProperty;
        HSSObservable * rightObserved;

        QSharedPointer<HSSSimpleSelection> scope;

        bool _isDirty;
        HSSUnit _value;

    private:
        HSSExpressionType expressionType;
    };
}

#endif
