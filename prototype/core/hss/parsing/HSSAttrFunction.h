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

#ifndef HSSATTRFUNCTION_H
#define HSSATTRFUNCTION_H

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
    class HSSAttrFunction : public HSSFunction
    {
    public:
        
        typedef boost::shared_ptr<HSSAttrFunction> p;
        
        /**
         *  Creates a new instance of a ref function.
         */
        HSSAttrFunction();
        
        /**
         *  Copy constructor for HSSAttrFunction objects. Do not call directly, use clone() instead.
         */
        HSSAttrFunction(const HSSAttrFunction & orig);
        
        /**
         *  Clones an instance of HSSAttrFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSAttrFunction
         */
        HSSFunction::p clone() const;
        
        /**
         *  Destructor for this class. Removes itself from the observed object.
         */
        virtual ~HSSAttrFunction();
                
        /**
         *  Getter for the attribute name.
         *  @return A string containing the attribute name we are referencing.
         */
        const std::string & getAttributeName() const;
        
        /**
         *  Setter for the attribute name.
         *  @param  newValue    A string containing the attribute name we are referencing.
         */
        void setAttributeName(std::string newValue);
        
        /**
         *  Getter for the selector chain.
         *  @return A vector of shared pointers to the selector chains.
         */
        const std::vector<HSSSelectorChain::p> & getSelectorChains() const;
        
        /**
         *  Setter for the selector chains.
         *  @param newValue     A vector of shared pointers to the new selector chains to be used.
         */
        void setSelectorChains(std::vector<HSSSelectorChain::p> newValues);
        
        /**
         *  Add a selector chain to the selector chains vector.
         *  @param newValue A shared pointer to the selector chain to be added.
         */
        void selectorChainsAdd(HSSSelectorChain::p & newSelectorChain);
        
        /**
         *  Get a selector chain by index.
         *  @param index    An unsigned integer with the index of the selector chain.
         *  @return A shared pointer to the element at that index.
         */
        HSSSelectorChain::p &selectorChainsGet(unsigned index);
        
        /**
         *  Removes a selector chain by index.
         *  @param index    An unsigned integer with the index of the selector chain to be deleted.
         */
        void selectorChainsRemove(unsigned index);
        
        /**
         *  Removes the last element in the selector chains vector.
         */
        void selectorChainsRemoveLast();
        
        /**
         *  @return the last element of the selector chains vector.
         */
        HSSSelectorChain::p &selectorChainsLast();
        
        /**
         *  @return the size of the selector chains vector
         */
        const int selectorChainsSize();
        
        /**
         *  This is the actual implementation of what the function does. It selects from the elements
         *  in the scope and then observes the property, storing  its value.
         */
        virtual boost::any _evaluate();
        
        /**
         *  Calls _evaluate();
         *  @deprecated Do not use this.
         */
        virtual boost::any _evaluate(std::deque<HSSParserNode::p> arguments);
        
        /**
         *  Callback that will update the value when the observed property changes.
         *  @param source   The property which we are observing (ignored).
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void valueChanged(HSSObservableProperty source, void*data);
        
    private:
        std::string attributeName;
        std::vector<HSSSelectorChain::p> selectorChains;
        
        HSSObservable * observed;
        HSSObservableProperty observedProperty;
        
        HSSClonable::p cloneImpl() const;
    };
}



#endif
