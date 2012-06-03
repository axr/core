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
 *      Revision: 15
 *
 ********************************************************************/

#ifndef HSSPARSERNODE_H
#define HSSPARSERNODE_H
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <vector>
#include "../various/HSSObservable.h"
#include "../various/HSSClonable.h"

namespace AXR {
    
    /**
     *  @addtogroup typeEnums
     *  @{
     *  @enum HSSParserNodeType
     *  The AST is generated using various types of parser nodes, each having a different meaning or use. This
     *  enum provides a way to identifying each node type, when they are processed.
     */
    enum HSSParserNodeType
    {
        HSSParserNodeTypeGeneric = 0, /**< Error state. */
        HSSParserNodeTypeSelector, /**< Selects elements by name. */
        HSSParserNodeTypeUniversalSelector, /**< Selects all elements in a scope. */
        HSSParserNodeTypeThisSelector, /**< Selects the nearest display object. */
        HSSParserNodeTypeParentSelector, /**< Selects the parent element of the nearest display object. */
        HSSParserNodeTypeSuperSelector, /**< Selects the element that owns the current one. */
        HSSParserNodeTypeSubjectSelector, /**< Changes what will be selected by a selector chain ($). */
        HSSParserNodeTypeCombinator, /**< Defines relations between parts of the selector chain. */
        HSSParserNodeTypeFilter, /**< Reduces or alters the selection. */
        HSSParserNodeTypeStatement, /**< Basic execution block. */
        HSSParserNodeTypeExpression, /**< Mathematic operation between two values (or other expressions). */
        HSSParserNodeTypeNumberConstant, /**< A basic number in the HSS source. */
        HSSParserNodeTypePercentageConstant, /**< A percentage number in the HSS source. */
        HSSParserNodeTypeStringConstant, /**< A literal string (single or double quotes) in the HSS source. */
        HSSParserNodeTypeKeywordConstant, /**< A keyword literal in the HSS source. */
        HSSParserNodeTypeObjectDefinition, /**< Encapsulates reusable groups of property definitions. */
        HSSParserNodeTypeObjectNameConstant, /**< Used to get the object definition by name. */
        HSSParserNodeTypeFunctionCall, /**< Which function to call, and with what arguments. */
        HSSParserNodeTypeMultipleValueDefinition, /**< Encapsulates various values separated by commas. */
        HSSParserNodeTypeSelectorChain, /**< A group of selector nodes that define what elements will be selected. */
        HSSParserNodeTypeNegation, /**< Inverts the meaning of one or more selectors. */
        HSSParserNodeTypeFlag, /**< Conditionally applies properties to elements. */
        HSSParserNodeTypeRootSelector, /**< Returns the root element. */
    };
    /** @} */
    
    class HSSDisplayObject;
    
    /**
     *  @brief Abstract base class that provides the basic functionality of a parser node. Do not use directly,
     *  but one of its subclasses instead.
     *
     *  When parsing an HSS file, HSSParser will create an Abstract Syntax Tree with subclasses of this class.
     *  Go to https://docs.google.com/drawings/d/1yMMwh3uddC1fFJqWt8RMShJFGhO2pq_IHEpgQYh7dKY/ to see a diagram
     *  of the inheritance tree of parser nodes.
     */
    class HSSParserNode : public HSSObservable, public HSSClonable, public boost::enable_shared_from_this<HSSParserNode>
    {
    public:
        
        /**
         *  The shared pointer to instances of this class.
         */
        typedef boost::shared_ptr<HSSParserNode> p;
        
        /**
         *  Iterator for vectors of shared pointers to parser nodes.
         */
        typedef std::vector<HSSParserNode::p>::iterator it;
        
        /**
         *  Iterator for vectors of shared pointers to parser nodes, const version.
         */
        typedef std::vector<HSSParserNode::p>::const_iterator const_it;
        
        /**
         *  "Parent Pointer". A weak pointer used to break cyclic references.
         */
        typedef boost::weak_ptr<HSSParserNode> pp;
        
        /**
         *  When logging, you often need a string representation of the parser node type.
         *  @param type     The parser node type to represent as a string.
         *  @return A string representation of the given type.
         */
        static std::string parserNodeStringRepresentation(HSSParserNodeType type);
        
        /**
         *  Creates a new instance of a parser node. This class shouldn't be called directly,
         *  but by the subclasses.
         *  @param  type    The HSSActionType that this action corresponds to. Usually set
         *                  from withing a subclass' constructor method.
         *
         *  @todo There is no destructor on this class.
         */
        HSSParserNode();
        
        /**
         *  Copy constructor for HSSParserNode objects. Do not call directly, use clone() instead.
         */
        HSSParserNode(const HSSParserNode &orig);
        
        /**
         *  Clones an instance of HSSParserNode and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSParserNode
         */
        p clone() const;
        
        /**
         *  Prints itself as a textual representation. Each object overrides this method to .
         *  @return A textual representation of the object.
         */
        virtual std::string toString();
        
        /**
         *  Call this to know if a parser node is of the given type.
         *  @param otherType    The parser node type to compare against.
         *  @return Wether the instance is an parser node of given type.
         */
        bool isA(HSSParserNodeType otherType);
        
        /**
         *  @return The parser node type of the current instance.
         */
        HSSParserNodeType getType();
        
        /**
         *  Traversal method of the parser tree.
         *  @return The parent parser node in the parser tree or an empty pointer if none.
         */
        p getParentNode();
        
        /**
         *  Saves the given shared pointer as the parent of this instance in the parser tree.
         *  @param newParent    A shared pointer to the new parent node.
         */
        void setParentNode(p newParent);
        
        /**
         *  Removes itself from its parent node in the parser tree.
         *  @param newParent    A shared pointer to the new parent node.
         *
         *  @todo The saved pointer to the parent is not touched, it just removes itself on the parent.
         */
        void removeFromParentNode();
        
        /**
         *  Adds the given child node to the list of children
         *  @param child    A shared pointer to the child node to add.
         */
        void addNode(p child);
        
        /**
         *  Removes the given child node from the list of children.
         *  @param child    A shared pointer to the child node to remove.
         */
        void removeNode(p child);
        
        /**
         *  Traversal method of the parser tree.
         *  @param child    A vector of shared pointers to the child nodes.
         */
        const std::vector<HSSParserNode::p> getChildNodes() const;
        
        /**
         *  Setter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @param value        A shared pointer to the nearest display object.
         */
        virtual void setThisObj(boost::shared_ptr<HSSDisplayObject> value);
        /**
         *  Getter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @return A shared pointer to the nearest display object.
         */
        virtual boost::shared_ptr<HSSDisplayObject> getThisObj();
        
    protected:
        HSSParserNodeType nodeType;
        boost::shared_ptr<HSSDisplayObject> thisObj;
    private:
        pp _parentNode;
        std::vector<p> _childNodes;
        virtual HSSClonable::p cloneImpl() const;
    };
}

#endif
