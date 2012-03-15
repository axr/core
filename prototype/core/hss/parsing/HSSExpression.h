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
 ********************************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2012/03/15
 *      HSS version: 1.0
 *      Core version: 0.45
 *      Revision: 12
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
    
    enum HSSExpressionType
    {
        HSSExpressionTypeGeneric = 0,
        HSSExpressionTypeSum,
        HSSExpressionTypeSubtraction,
        HSSExpressionTypeMultiplication,
        HSSExpressionTypeDivision,
    };
    
    class HSSExpression : public HSSParserNode, public HSSObservable
    {
    public:
        typedef boost::shared_ptr<HSSExpression> p;
        
        HSSExpression(HSSParserNode::p _left, HSSParserNode::p _right);
        virtual ~HSSExpression();
        std::string toString();
        long double evaluate();
        virtual long double calculate(long double leftval, long double rightval) =0;
        
        bool isA(HSSExpressionType otherType);
        HSSExpressionType getExpressionType();
        
        static std::string expressionTypeStringRepresentation(HSSExpressionType type);
        
        void setLeft(HSSParserNode::p newLeft);
        HSSParserNode::p getLeft();
        void setRight(HSSParserNode::p newRight);
        HSSParserNode::p getRight();
        
        virtual void propertyChanged(HSSObservableProperty property, void* data);
        
        virtual void leftChanged(HSSObservableProperty property, void* data);
        virtual void rightChanged(HSSObservableProperty property, void* data);
        
        virtual void setPercentageBase(long double value);
        virtual void setPercentageObserved(HSSObservableProperty property, HSSObservable * observed);
        virtual void setScope(const std::vector<HSSDisplayObject::p> * newScope);
        virtual void setThisObj(boost::shared_ptr<HSSDisplayObject> value);
        
        void setDirty(bool value);
        bool isDirty();
        
        void setValue(long double newValue);
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
