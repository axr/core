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
 *      Last changed: 2011/10/06
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 11
 *
 ********************************************************************/

#ifndef HSSPARSER_H
#define HSSPARSER_H

#include "../tokenizing/HSSTokenizer.h"
#include "../parsing/HSSParserExceptions.h"
#include "../parsing/HSSParserNodes.h"
#include "../objects/HSSObjectExceptions.h"
#include "../objects/HSSObjects.h"
#include <string>
#include <vector>
#include <stack>
#include <boost/shared_ptr.hpp>

namespace AXR {
    enum HSSParserContext {
        HSSParserContextRoot,
        HSSParserContextBlock,
        HSSParserContextObjectDefinition,
        HSSParserContextExpression,
        HSSParserContextSelectorChain
    };
    
    class AXRController;
    
    class HSSParser
    {
        
    public:
        typedef boost::shared_ptr<HSSParser> p;
        
        std::string filename;
        
        HSSParser(AXRController * theController);
        //initialize with a pointer to the buffer where the HSS code is stored, and the lenght of the buffer
        //HSSParser(HSSTokenizer::buf_p buffer, unsigned buflen, std::string filename);
        //destructor
        ~HSSParser();
        
        //resets all the values to factory default
        void reset();
        
        //reads the specified file
        bool loadFile(std::string filepath);
        
        //reads and returns a shared pointer to the next statement in the buffer
        HSSStatement::p readNextStatement();
        
        void currentObjectContextRemoveLast();
        unsigned int currentObjectContextSize();
        void currentObjectContextAdd(HSSObject::p theObject);
        
    protected:
        HSSTokenizer::p tokenizer;
        //weak pointer
        AXRController * controller;
        
        HSSToken::p currentToken;
        std::vector<HSSParserContext> currentContext;
        std::stack<HSSObject::p> currentObjectContext;
        
        
        HSSRule::p readRule();
        HSSSelectorChain::p readSelectorChain(HSSTokenType stopOn); //FIXME (?)
        
        bool isCombinator();
        bool isCombinator(HSSToken::p token);
        bool isChildrenCombinator();
        bool isPropertyDefinition();
        
        HSSCombinator::p readChildrenCombinatorOrSkip();
        HSSCombinator::p readSymbolCombinator();
        HSSSelector::p readSelector();
        HSSObjectDefinition::p readObjectDefinition();
        HSSPropertyDefinition::p readPropertyDefinition();
        HSSInstruction::p readInstruction();
        HSSObjectDefinition::p getObjectFromInstruction(HSSInstruction::p instruction);
        HSSRule::p readInstructionRule();
        HSSParserNode::p readExpression();
        HSSParserNode::p readFunction();
        
        HSSParserNode::p readAdditiveExpression();
        HSSParserNode::p readMultiplicativeExpression();
        HSSParserNode::p readBaseExpression();
        
        void readNextToken();
        
        bool atEndOfSource();
        void checkForUnexpectedEndOfSource();
        void skipExpected(HSSTokenType type);
        void skipExpected(HSSTokenType type, std::string value);
        void skip(HSSTokenType type);
        
    };
}

#endif