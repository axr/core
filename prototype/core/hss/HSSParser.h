/********************************************************************
 *             a  A                                                        
 *            AM\/MA                                                         
 *           (MMMMMD                                                         
 *            :: VD
 *           ::  .                                                         
 *          ::                                                              
 *          M   NZ      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6             
 +       .MMZ. MMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6            
 *      AMMMMMDJMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6              
 *      MM  MMMMMC         ___MMMM     VMMMMMMM6       MMMM                   
 *      MM  MMMMMMM,     AMMMMMMMM      VMMMMM6        MMMM                  
 *     :MM AMMMMMMMD    MMMMMMMMMM       MMMMMM        MMMM                   
 *    :: MMMMMMMMMM    MMMMMMMMMMM      AMMMMMMD       MMMM                   
 *   .     MMMMM~     MMMM    MMMM     AMMMMMMMMD      MMMM                   
 *         MMMMM      MMMM    MMMM    AMMM6  MMMMD     MMMM                   
 *        MMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM                   
 *       MMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM                   
 *      MMMM MMMM                                                           
 *     AMMM  .MMM                                         
 *     MMM   .MMD       ARBITRARY·······XML········RENDERING                           
 *     MMM    MMA       ====================================                              
 *     DMN    MM                               
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
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 ********************************************************************/

#ifndef HSSPARSER_H
#define HSSPARSER_H

#include "HSSTokenizer.h"
#include <string>
#include "HSSParserExceptions.h"
#include "HSSObjectExceptions.h"
#include "HSSParserNodes.h"
#include "HSSObjects.h"
#include <vector>
#include <stack>

using namespace std;

enum HSSParserContext {
    HSSParserContextRoot,
    HSSParserContextBlock,
    HSSParserContextObjectDefinition,
    HSSParserContextExpression,
    HSSParserContextSelectorChain
};


class HSSParser
{
public:
    //initialize with a pointer to the buffer where the HSS code is stored, and the lenght of the buffer
	HSSParser(char * buffer, unsigned buflen, string filename);
	//destructor
	~HSSParser();
    
    //reads and returns a pointer to the next statement in the buffer or NULL if none found
    HSSStatement * readNextStatement();
    
    string filename;
    
protected:
    HSSTokenizer * tokenizer;
    HSSToken * currentToken;
    vector<HSSParserContext> currentContext;
    stack<HSSObject *> currentObjectContext;
    
    HSSStatement * readRule();
    HSSSelectorChain * readSelectorChain(); //FIXME
    
    bool isCombinator();
    bool isCombinator(HSSToken * token);
    bool isChildrenCombinator();
    bool isPropertyDefinition();
    //will return a pointer or NULL
    //you get to be the owner of the pointer
    HSSCombinator * readChildrenCombinatorOrSkip();
    //will return a pointer or NULL
    //you get to be the owner of the pointer
    HSSCombinator * readSymbolCombinator();
    //will return a pointer or NULL
    //you get to be the owner of the pointer
    HSSSelector * readSelector();
    //will return a pointer or NULL
    //you get to be the owner of the pointer
    HSSObjectDefinition * readObjectDefinition();
    //will return a pointer or NULL
    //you get to be the owner of the pointer
    HSSPropertyDefinition * readPropertyDefinition();
    //will return a pointer or NULL
    //you get to be the owner of the pointer
    HSSInstruction * readInstruction();
    
	void readNextToken();
    
    bool atEndOfSource();
    void checkForUnexpectedEndOfSource();
    void skipExpected(HSSTokenType type);
    void skipExpected(HSSTokenType type, string value);
    void skip(HSSTokenType type);
    
    void currentObjectContextRemoveLast();
    
};

#endif