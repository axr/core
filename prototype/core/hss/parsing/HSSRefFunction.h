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
 *      Last changed: 2012/06/02
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 4
 *
 ********************************************************************/

#ifndef HSSREFFUNCTION_H
#define HSSREFFUNCTION_H

#include "HSSFunction.h"
#include "../parsing/HSSSelectorChain.h"

namespace AXR {
    
    /**
     *  @brief Implementation of the ref([<modfier>] <name> [of <selector chain>]) function in HSS.
     *
     *  References allow the author of HSS documents to bind together values of either
     *  the same object (using \@this) or to another object. This class stores the arguments
     *  and implements the _evaluate() method for this type of function.
     *
     *  The modifier determines what to do in case the selector chain returns multiple elements.
     */
    class HSSRefFunction : public HSSFunction
    {
    public:
        
        typedef boost::shared_ptr<HSSRefFunction> p;
        
        /**
         *  Creates a new instance of a ref function.
         */
        HSSRefFunction();
        
        /**
         *  Copy constructor for HSSRefFunction objects. Do not call directly, use clone() instead.
         */
        HSSRefFunction(const HSSRefFunction & orig);
        
        /**
         *  Clones an instance of HSSRefFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSRefFunction
         */
        p clone() const;
        
        /**
         *  Destructor for this class. Removes itself from the observed object.
         */
        virtual ~HSSRefFunction();
        
        /**
         *  Getter for the modifier.
         *  @return A string containing the modifier that was passed.
         *
         *  @todo Make an enum based on the string and use that instead.
         */
        const std::string & getModifier() const;
        
        /**
         *  Setter for the modifier.
         *  @param newValue     A string containing the modifier for the function.
         *
         *  @todo Make an enum based on the string and use that instead.
         */
        void setModifier(std::string newValue);
        
        /**
         *  Getter for the property name.
         *  @return The observable property that we are referencing.
         */
        const HSSObservableProperty & getPropertyName() const;
        
        /**
         *  Setter for the property name.
         *  @param  newValue    The observable property that we will be observing.
         */
        void setPropertyName(HSSObservableProperty newValue);
        
        /**
         *  Getter for the selector chain.
         *  @return A shared pointer to the selector chain.
         */
        const HSSSelectorChain::p & getSelectorChain() const;
        
        /**
         *  Setter for the selector chain.
         *  @param newValue     A shared pointer to the new selector chain to be used.
         */
        void setSelectorChain(HSSSelectorChain::p newValue);
        
        /**
         *  This is the actual implementation of what the function does. It selects from the elements
         *  in the scope and then observes the property, storing  its value.
         */
        virtual void * _evaluate();
        
        /**
         *  Calls _evaluate();
         *  @deprecated Do not use this.
         */
        virtual void * _evaluate(std::deque<HSSParserNode::p> arguments);
        
        /**
         *  Callback that will update the value when the observed property changes.
         *  @param source   The property which we are observing (ignored).
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void valueChanged(HSSObservableProperty source, void*data);
        
    private:
        std::string modifier;
        HSSObservableProperty propertyName;
        HSSSelectorChain::p selectorChain;
        
        HSSObservable * observed;
        
        HSSClonable::p cloneImpl() const;
    };
}



#endif
