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

#ifndef HSSCOMBINATOR_H
#define HSSCOMBINATOR_H

#include "HSSParserNode.h"
#include <string>
#include <boost/shared_ptr.hpp>

namespace AXR {
    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSCombinatorType
     *  The type of the combinator which will determine what the behavior will be when used in AXRController.
     */
    enum HSSCombinatorType
    {
        HSSCombinatorTypeSiblings, /**< Select elements on the same level (=) */
        HSSCombinatorTypePreviousSiblings, /**< Select elements on the same level that come before the previously selected elements (-) */
        HSSCombinatorTypeNextSiblings, /**< Select elements on the same level that come after the previously selected elements (+) */
        HSSCombinatorTypeChildren, /**< Select children of the previously selected elements (whitespace) */
        HSSCombinatorTypeDescendants, /**< Select descendants of the previously selected elements (..) */
        HSSCombinatorTypeAllDescendants, /**< Deprecated */
        HSSCombinatorTypeTextSelection /**< Switches the selection to text space */
    };
    /** @} */
    
    /**
     *  @brief Combinators are used inside of selector chains, to define the relations between the elements that 
     *  are selected.
     *  
     *  This class encapsulates all types of combinators. The actual behavior of the selection
     *  is defined in AXRController.
     */
    class HSSCombinator : public HSSParserNode {
    public:
        typedef boost::shared_ptr<HSSCombinator> p;
        
        /**
         *  Creates a new instance of a combinator node, of the type you give. All types of combinators
         *  can be encapsulated with this class.
         */
        HSSCombinator(HSSCombinatorType type);
        
        /**
         *  Clones an instance of HSSCombinator and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSCombinator
         */
        p clone() const;
        
        /**
         *  When logging, you often need a string representation of the combinator type.
         *  @param type     The combinator type to represent as a string.
         *  @return A string representation of the given type.
         */
        static std::string combinatorStringRepresentation(HSSCombinatorType type);
        
        /**
         *  Allows you to check if this combinator is of the given type.
         *  @param  otherType   The combinator type to which to check against.
         *  @return Wether it is of the given type or not.
         */
        bool isA(HSSCombinatorType otherType);
        
        /**
         *  @return The action type of this instance.
         */
        const HSSCombinatorType & getCombinatorType() const;
        
        virtual std::string toString();
    protected:
        HSSCombinatorType combinatorType;
    private:
        virtual HSSClonable::p cloneImpl() const;
    };
}


#endif
