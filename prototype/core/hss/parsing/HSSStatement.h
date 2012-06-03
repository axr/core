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
 *      Last changed: 2012/06/03
 *      HSS version: 1.0
 *      Core version: 0.47
 *      Revision: 6
 *
 ********************************************************************/

#ifndef HSSSTATEMENT_H
#define HSSSTATEMENT_H

#include "HSSParserNode.h"
#include <string>
#include <boost/shared_ptr.hpp>

namespace AXR {
    
    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSStatementType
     *  The type of the statement, specific for each subclass. 
     */
    enum HSSStatementType
    {
        HSSStatementTypeGeneric = 0, /**< Error state. */
        HSSStatementTypeRule, /**< Rules are used to apply property definitions to elements. */
        HSSStatementTypePropertyDefinition, /**< Define a value for a property name. */
        HSSStatementTypeObjectDefinition, /**< Define a reusable group of property definitions under a name. */
        HSSStatementTypeComment, /**< . */
        HSSStatementTypeInstruction /**< . */
    };
    /** @} */
    
    /**
     *  @brief Abstract base class that provides the basic functionality for statements. Do not use directly,
     *  but one of its subclasses instead.
     *
     *  Statements are the basic execution units for the rendering engine. They are high level abstractions
     *  of groups of parser nodes that have a particular meaning (e.g. a rule is for applying properties to an element,
     *  and is made up of a selector chain, and property definitions.
     */
    class HSSStatement : public HSSParserNode
    {
    public:
        
        /**
         *  The shared pointer to instances of this class.
         */
        typedef boost::shared_ptr<HSSStatement> p;
        
        /**
         *  Creates a new instance of a statement node. This class shouldn't be called directly,
         *  but by the subclasses.
         */
        HSSStatement();
        
        /**
         *  Copy constructor for HSSStatement nodes. Do not call directly, use clone() on one of the
         *  subclasses instead.
         */
        HSSStatement(const HSSStatement & orig);
        
        virtual std::string toString() =0;
        
        /**
         *  Call this to know if a statement is of the given type.
         *  @param otherType    The statement type to compare against.
         *  @return Wether the instance is a statement of given type.
         */
        bool isA(HSSStatementType otherType);
        
        /**
         *  @return The statement type of the current instance.
         */
        HSSStatementType getType();
        
        /**
         *  When logging, you often need a string representation of the statement type.
         *  @param type     The statement type to represent as a string.
         *  @return A string representation of the given type.
         */
        std::string statementStringRepresentation(HSSStatementType type);
        
    protected:
        HSSStatement::p shared_from_this();
        
        HSSStatementType type;
    };
}


#endif
