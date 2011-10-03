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
 *      Last changed: 2011/09/29
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/


#ifndef HSSFUNCTIONCALL_H
#define HSSFUNCTIONCALL_H

#include "HSSParserNode.h"
#include <string>
#include <deque>
#include "../objects/HSSFunction.h"
#include "../objects/HSSDisplayObject.h"

namespace AXR {
    class HSSFunctionCall : public HSSParserNode, public HSSObservable
    {
    public:
        
        typedef boost::shared_ptr<HSSFunctionCall> p;
        
        friend class HSSParser;
        friend class HSSDisplayObject;
        
        HSSFunctionCall();
        virtual ~HSSFunctionCall();
        std::string toString();
        
        const HSSFunction::p & getFunction() const;
        void setFunction(HSSFunction::p newFunction);
        
        const std::deque<HSSParserNode::p> & getArguments() const;
        void setArguments(std::deque<HSSParserNode::p> newArguments);
        void argumentsAdd(HSSParserNode::p newArgument);
        HSSParserNode::p argumentsFirst();
        const HSSParserNode::p argumentsFirst() const;
        HSSParserNode::p argumentsLast();
        const HSSParserNode::p argumentsLast() const;
        unsigned int argumentsSize();
        void argumentsRemoveFirst();
        void argumentsRemoveLast();
                
        virtual void setPercentageBase(long double value);
        virtual void setPercentageObserved(HSSObservableProperty property, HSSObservable * observed);
        
        virtual void setScope(const std::vector<HSSDisplayObject::p> * newScope);
        
        virtual long double evaluate();
        void setDirty(bool value);
        bool isDirty();
        
        void setValue(void * newValue);
        void * getValue();
        
    private:
        HSSFunction::p function;
        std::deque<HSSParserNode::p>arguments;
        
        long double percentageBase;
        HSSObservableProperty percentageObservedProperty;
        HSSObservable * percentageObserved;
        const std::vector<HSSDisplayObject::p> * scope;
        
        bool _isDirty;
        void * _value;
        
    };
}




#endif
