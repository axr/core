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

#ifndef HSSFLAGFUNCTION_H
#define HSSFLAGFUNCTION_H

#import "HSSFunction.h"
#import "../objects/HSSAction.h"

namespace AXR {
    
    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSFlagFunctionType
     *  The type of the flagging function. It will determine what the function
     *  actually does.
     */
    enum HSSFlagFunctionType
    {
        HSSFlagFunctionTypeNone = 0, /**< Error state. */
        HSSFlagFunctionTypeFlag, /**< Always set the flag to active. */
        HSSFlagFunctionTypeUnflag, /**< Always set the flag to inactive. */
        HSSFlagFunctionTypeToggleFlag /**< When a flag is active, deactivate it, and viceversa. */
    };
    /** @} */
    
    /**
     *  @brief Function type to manipulate the activation status of a flag.
     *  All of the flagging functions [flag(), unflag() and toggleFlag()] are handled by this class.
     */
    class HSSFlagFunction : public HSSFunction
    {
    public:
        
        typedef boost::shared_ptr<HSSFlagFunction> p;
        
        /**
         *  When logging, you often need a string representation of the flag type.
         *  @param name   The action type to represent as a string.
         *  @return A string representation of the given type.
         */
        static HSSFlagFunctionType flagFunctionTypeFromString(std::string name);
        
        /**
         *  Creates a new instance of a flagging function, of the given type.
         *  @param flagFunctionType     The HSSFlagFunctionType that corresponds with the desired
         *                              behavior of the function.
         */
        HSSFlagFunction(HSSFlagFunctionType flagFunctionType);
        
        /**
         *  Copy constructor for HSSAction objects. Do not call directly, use clone() instead.
         */
        HSSFlagFunction(const HSSFlagFunction & orig);
        
        /**
         *  Clones an instance of HSSFlagFunction and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSFlagFunction
         */
        p clone() const;
        
        /**
         *  Destructor for this class.
         */
        virtual ~HSSFlagFunction();
        
        /**
         *  Getter for the name parameter.
         *  @return A string containing the name of the HSS property the function should access.
         */
        const std::string & getName();
        
        /**
         *  Setter for the name parameter.
         *  @param newValue The new HSS property name which the function should use.
         */
        void setName(std::string newValue);
        
        /**
         *  Getter for the selector chain parameter.
         *  @return A shared pointer to the selector chain that should be used in the function.
         */
        const std::vector<HSSSelectorChain::p> & getSelectorChains() const;
        
        /**
         *  Setter for the selector chain parameter.
         *  @param newValue A shared pointer to the selector chain which the function should use.
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
         *  @todo make private
         */
        virtual void * _evaluate();
        
        /**
         *  @todo is this one even used?
         */
        virtual void * _evaluate(std::deque<HSSParserNode::p> arguments);
        
        /**
         *  Method to be passed as callback when observing changes.
         *  @param source   The property which we are observing.
         *  @param data     A pointer to the data that is sent along the notification.
         */
        void valueChanged(HSSObservableProperty source, void*data);
        
        /**
         *  @return The flag type of this instance.
         */
        HSSFlagFunctionType getFlagFunctionType();
        
    protected:
        std::string _name;
        std::vector<HSSSelectorChain::p> selectorChains;
        HSSFlagFunctionType _flagFunctionType;
        
    private:
        HSSClonable::p cloneImpl() const;
    };
}



#endif
