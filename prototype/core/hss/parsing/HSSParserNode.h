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
 *      Last changed: 2011/12/29
 *      HSS version: 1.0
 *      Core version: 0.43
 *      Revision: 12
 *
 ********************************************************************/

#ifndef HSSPARSERNODE_H
#define HSSPARSERNODE_H
#include <string>
#include <boost/shared_ptr.hpp>
#include <vector>

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
        HSSParserNodeTypeMultipleValueDefinition
    };

    
    class HSSParserNode
    {
    public:
        typedef boost::shared_ptr<HSSParserNode> p;
        typedef std::vector<HSSParserNode::p>::iterator it;
        
        static std::string parserNodeStringRepresentation(HSSParserNodeType type);
        
        HSSParserNode();
        virtual std::string toString();
        
        bool isA(HSSParserNodeType otherType);
        HSSParserNodeType getType();
        
    protected:
        HSSParserNodeType nodeType;
    };
}

#endif
