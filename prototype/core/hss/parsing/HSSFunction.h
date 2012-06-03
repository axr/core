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
 *      Last changed: 2012/05/25
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 6
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
    
    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSFunctionType
     *  The type of the function, specific for each subclass.
     */
    enum HSSFunctionType
    {
        HSSFunctionTypeNone = 0, /**< Error state. */
        HSSFunctionTypeRef, /**< References a value on a selected element. */
        HSSFunctionTypeSel, /**< Creates a selection of elements. */
        HSSFunctionTypeMin, /**< Returns the passed value, or the minimum if lower. */
        HSSFunctionTypeMax, /**< Returns the passed value, or the maxium if higher. */
        HSSFunctionTypeFloor, /**< Rounds a number down to the next integer. */
        HSSFunctionTypeCeil, /**< Rounds a number up to the next integer. */
        HSSFunctionTypeRound, /**< Rounds a number to the nearest integer. */
        HSSFunctionTypeFlag, /**< Activates the given flag on the selected elements. */
        HSSFunctionTypeUnflag, /**< Deactivates the given flag on the selected elements. */
        HSSFunctionTypeToggleFlag /**< Toggles the given flag on the selected elements. */
    };
    /** @} */
    
    class AXRController;
    
    /**
     *  @brief Abstract base class for all function objects.
     *  Do not use directly, use a specific subclass instead.
     */
    class HSSFunction : public HSSParserNode
    {
    public:        
        friend class HSSParser;
        
        /**
         *  This class shouldn't be called directly, but by the subclasses. Creates a new
         *  instance of a function.
         */
        HSSFunction();
        
        /**
         *  Copy constructor for HSSFunction objects. Do not call directly, use clone() on
         *  on of the subclasses instead.
         */
        HSSFunction(const HSSFunction & orig);
        
        /**
         *  Clones an instance of HSSFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFunction
         */
        virtual ~HSSFunction();
        
        typedef boost::shared_ptr<HSSFunction> p;
        virtual std::string toString();
        
        /**
         *  Evaluate the function. The actual implementation is up to each specific subclass.
         *  @return A void pointer to some data. It's up to the caller to know what type of
         *  data is expected.
         */
        void * evaluate();
        
        /**
         *  @todo I think this one is not used
         */
        void * evaluate(std::deque<HSSParserNode::p> arguments);
        
        /**
         *  @todo make protected or private
         */
        virtual void * _evaluate();
        
        /**
         *  @todo make protected or private
         */
        virtual void * _evaluate(std::deque<HSSParserNode::p> arguments) =0;
        
        /**
         *  Method to be passed as callback when observing changes.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        virtual void propertyChanged(HSSObservableProperty property, void* data);
        
        /**
         *  The percentage base is the number that corresponds to 100%.
         *  @param value    A long double containing the base for percentage calculations.
         */
        virtual void setPercentageBase(long double value);
        
        /**
         *  When we are using percentages, we keep track of the value and update accordingly.
         *  @param property The property which we are observing.
         *  @param observed     A regular pointer to the object we are observing. 
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
         *  @return A void pointer to the value returned by the function. It's up to the caller to
         *  know what kind of data is expected.
         */
        void * getValue();
        
        /**
         *  Allows you to check if this function is of the given type.
         *  @param  type    The function type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        virtual bool isA(HSSFunctionType type);
        
        /**
         *  @return The function type of this instance.
         */
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
