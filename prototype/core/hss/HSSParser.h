/**********************************************************
 *
 *      d8888 Y88b   d88P 8888888b.  
 *      d88888  Y88b d88P  888   Y88b 
 *      d88P888   Y88o88P   888    888 
 *      d88P 888    Y888P    888   d88P 
 *      88P  888    d888b    8888888P"  
 *      d88P   888   d88888b   888 T88b   
 *      d8888888888  d88P Y88b  888  T88b  
 *      d88P     888 d88P   Y88b 888   T88b 
 *      
 *      ARBITRARY·······XML········RENDERING
 *      ====================================
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
 **********************************************************
 *
 *      FILE INFORMATION:
 *      =================
 *      Last changed: 2011/04/04
 *      HSS version: 1.0
 *      Core version: 0.3
 *      Revision: 1
 *
 **********************************************************/

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