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
 *      Last changed: 2012/03/21
 *      HSS version: 1.0
 *      Core version: 0.46
 *      Revision: 5
 *
 ********************************************************************/

#ifndef HSSFUNCTION_H
#define HSSFUNCTION_H

#include <string>
#include "../objects/HSSObject.h"
#include "../objects/HSSDisplayObject.h"
#include "HSSKeywordConstant.h"
#include <boost/shared_ptr.hpp>
#include <deque>
#include <vector>

namespace AXR {
    enum HSSFunctionType
    {
        HSSFunctionTypeNone = 0,
        HSSFunctionTypeRef,
        HSSFunctionTypeSel,
        HSSFunctionTypeMin,
        HSSFunctionTypeMax,
        HSSFunctionTypeFloor,
        HSSFunctionTypeCeil,
        HSSFunctionTypeRound,
        HSSFunctionTypeFlag,
        HSSFunctionTypeUnflag,
        HSSFunctionTypeToggleFlag
    };
    
    class AXRController;
    
    class HSSFunction : public HSSParserNode
    {
    public:        
        friend class HSSParser;
        
        HSSFunction();
        HSSFunction(const HSSFunction & orig);
        virtual ~HSSFunction();
        
        typedef boost::shared_ptr<HSSFunction> p;
        
        virtual std::string toString();
        
        void * evaluate();
        void * evaluate(std::deque<HSSParserNode::p> arguments);
        
        virtual void * _evaluate();
        virtual void * _evaluate(std::deque<HSSParserNode::p> arguments) =0;
        
        virtual void propertyChanged(HSSObservableProperty property, void* data);
        
        virtual void setPercentageBase(long double value);
        virtual void setPercentageObserved(HSSObservableProperty property, HSSObservable * observed);
        
        virtual void setScope(const std::vector<HSSDisplayObject::p> * newScope);
        
        void setDirty(bool value);
        bool isDirty();
        void * getValue();
        virtual bool isA(HSSFunctionType type);
        HSSFunctionType getFunctionType();
        
        /**
         *  Setter for the controller. The controller needs to be propagated across all
         *  HSSObject subclasses, so they get access to the DOM and such.
         *  @param controller       A pointer to the AXRController that owns this object
         */
        virtual void setController(AXRController * controller);
        /**
         *  Getter for the controller.
         *  @return A pointer to the AXRController that owns this object
         */
        virtual AXRController * getController();
        
    protected:
        bool _isDirty;
        void * _value;
        HSSFunctionType functionType;
        
        long double percentageBase;
        HSSObservableProperty percentageObservedProperty;
        HSSObservable * percentageObserved;
        const std::vector<HSSDisplayObject::p> * scope;
        
        AXRController * axrController;
        
    };
}

#endif
