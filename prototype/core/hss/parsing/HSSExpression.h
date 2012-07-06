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

#ifndef HSSEXPRESSION_H
#define HSSEXPRESSION_H

#include "HSSParserNode.h"
#include <boost/shared_ptr.hpp>
#include <vector>
#include "../various/HSSObservable.h"
#include "../objects/HSSDisplayObject.h"

namespace AXR {
    
    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSExpressionType
     *  The type of the expression, specific for each subclass.
     */
    enum HSSExpressionType
    {
        HSSExpressionTypeGeneric = 0, /**< Error state. */
        HSSExpressionTypeSum, /**< A+B. */
        HSSExpressionTypeSubtraction, /**< A-B. */
        HSSExpressionTypeMultiplication, /**< A*B. */
        HSSExpressionTypeDivision, /**< A/B. */
    };
    /** @} */
    
    /**
     *  @brief Abstract base class for all expressions.
     *  Do not use directly, use a specific subclass instead.
     */
    class HSSExpression : public HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSExpression> p;
        
        /**
         *  Creates a new instance of an expression.
         *  This class shouldn't be called directly, but by the subclasses.
         *  @param left     The value on the left hand side of the operator.
         *  @param right    The value on the right hand side of the operator.
         */
        HSSExpression(HSSParserNode::p _left, HSSParserNode::p _right);
        
        /**
         *  Copy constructor for HSSExpression objects. Do not call directly, use clone() instead.
         */
        HSSExpression(const HSSExpression &orig);
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSExpression();
        std::string toString();
        long double evaluate();
        
        /**
         *  Each subclass implements the specific operation that needs to be done.
         *
         *  @param leftval  A long double that holds the left value.
         *  @param rightval  A long double that holds the right value.
         */
        virtual long double calculate(long double leftval, long double rightval) =0;
        
        bool isA(HSSExpressionType otherType);
        HSSExpressionType getExpressionType();
        
        /**
         *  When logging, you often need a string representation of the expression type.
         *  @param type     The expression type to represent as a string.
         *  @return A string representation of the given type.
         */
        static std::string expressionTypeStringRepresentation(HSSExpressionType type);
        
        /**
         *  Setter for the left hand member of the expression.
         *  @param  newLeft     A shared pointer to a parser node representing the left
         *                      member of the expression.
         */
        void setLeft(HSSParserNode::p newLeft);
        
        /**
         *  Getter for the left hand member of the expression.
         *  @return A shared pointer to a parser node representing the left member of
         *  the expression.
         */
        HSSParserNode::p getLeft();
        
        /**
         *  Setter for the right hand member of the expression.
         *  @param  newLeft     A shared pointer to a parser node representing the right
         *                      member of the expression.
         */
        void setRight(HSSParserNode::p newRight);
        
        /**
         *  Getter for the right hand member of the expression.
         *  @return A shared pointer to a parser node representing the right member of
         *  the expression.
         */
        HSSParserNode::p getRight();
        
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
         *  @param value    A long double containing the base for percentage calculations.
         */
        virtual void setPercentageBase(long double value);
        
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
         *  @param newScope     The new scope, a regular pointer to a vector of shared poninters
         *  to display obects.
         *
         *  @todo how is memory handled for the scopes?
         */
        virtual void setScope(const std::vector<HSSDisplayObject::p> * newScope);
        
        /**
         *  Setter for the "\@this object" which to pass to members like references or selections.
         *  @param value    A shared pointer to a display object representing the \@this object.
         */
        virtual void setThisObj(boost::shared_ptr<HSSDisplayObject> value);
        
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
         *  @param newValue     A long double containing the new calculated value for this expression.
         */
        void setValue(long double newValue);
        
        /**
         *  Getter for the value.
         *  @return The calculated value of the expression.
         */
        long double getValue();

    protected:
        HSSExpressionType expressionType;
        HSSParserNode::p left;
        long double leftval;
        long double rightval;
        HSSParserNode::p right;
        
        long double percentageBase;
        HSSObservableProperty percentageObservedProperty;
        HSSObservable * percentageObserved;
        
        
        HSSObservableProperty leftObservedProperty;
        HSSObservable * leftObserved;
        HSSObservableProperty rightObservedProperty;
        HSSObservable * rightObserved;
        
        const std::vector<HSSDisplayObject::p> * scope;
        
        bool _isDirty;
        long double _value;
    };
}

#endif
