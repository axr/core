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
 *      Revision: 14
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
    enum HSSParserNodeType
    {
        HSSParserNodeTypeGeneric = 0,
        HSSParserNodeTypeSelector,
        HSSParserNodeTypeUniversalSelector,
        HSSParserNodeTypeThisSelector,
        HSSParserNodeTypeParentSelector,
        HSSParserNodeTypeSuperSelector,
        HSSParserNodeTypeSubjectSelector,
        HSSParserNodeTypeCombinator,
        HSSParserNodeTypeFilter,
        HSSParserNodeTypeStatement,
        HSSParserNodeTypeExpression,
        HSSParserNodeTypeNumberConstant,
        HSSParserNodeTypePercentageConstant,
        HSSParserNodeTypeStringConstant,
        HSSParserNodeTypeKeywordConstant,
        HSSParserNodeTypeObjectDefinition,
        HSSParserNodeTypeObjectNameConstant,
        HSSParserNodeTypeFunctionCall,
        HSSParserNodeTypeMultipleValueDefinition,
        HSSParserNodeTypeSelectorChain,
        HSSParserNodeTypeNegation,
        HSSParserNodeTypeFlag,
        HSSParserNodeTypeRootSelector,
    };
    
    class HSSDisplayObject;

    
    class HSSParserNode : public HSSObservable, public HSSClonable, public boost::enable_shared_from_this<HSSParserNode>
    {
    public:
        typedef boost::shared_ptr<HSSParserNode> p;
        typedef std::vector<HSSParserNode::p>::iterator it;
        typedef std::vector<HSSParserNode::p>::const_iterator const_it;
        typedef boost::weak_ptr<HSSParserNode> pp;
        
        static std::string parserNodeStringRepresentation(HSSParserNodeType type);
        
        HSSParserNode();
        HSSParserNode(const HSSParserNode &orig);
        p clone() const;
        virtual std::string toString();
        
        bool isA(HSSParserNodeType otherType);
        HSSParserNodeType getType();
        
        p getParentNode();
        void setParentNode(p newParent);
        void removeFromParentNode();
        void addNode(p child);
        void removeNode(p child);
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
