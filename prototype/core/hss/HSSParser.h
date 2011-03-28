//
//  HSSParser.h
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

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