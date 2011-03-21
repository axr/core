//
//  HSSParser.cpp
//  mac
//
//  Created by Miro Keller on 13/03/11.
//  Copyright 2011 Miro Keller. All rights reserved.
//

#include "HSSParser.h"
#include "HSSValueToken.h"
#include <iostream>
#include <cstdlib>

#include "AXR.h"

HSSParser::HSSParser(char * buffer, unsigned buflen)
{
    this->tokenizer = new HSSTokenizer(buffer, buflen);
    this->currentToken = NULL;
    this->currentContext.push(HSSParserContextRoot);
    
    this->readNextToken();
}

HSSParser::~HSSParser()
{
    delete this->tokenizer;
}

HSSStatement * HSSParser::readNextStatement()
{
    if(this->currentContext.top() == HSSParserContextRoot)
    {
        //the file was empty
        if(this->atEndOfSource())
            return NULL;
        
        if(this->currentToken->isA(HSSInstructionSign)){
            //create a instruction statement
            return NULL;
        }
        
        //if the statement starts with an object sign, it is an object definition
        if(this->currentToken->isA(HSSObjectSign)){
            //create an object definition
            return this->readObjectDefinition();
        }
        
        //if the statement starts with an identifier, universal selector or combinator it is a rule
        if(this->currentToken->isA(HSSIdentifier)
           || (this->currentToken->isA(HSSSymbol) && VALUE_TOKEN(this->currentToken)->equals(HSSSymbol, "*"))
           || this->isCombinator()){ //FIXME: search for combinators as well
            return this->readRule();
        }
        
        return NULL;
    } else {
        std_log1("reading in anything other than root context is not implemented yet");
        return NULL;
    }
}

HSSStatement * HSSParser::readRule()
{
    security_brake_init()
    
    //throw error if at end of source
    this->checkForUnexpectedEndOfSource();
    
    //initialize the rule
    HSSSelectorChain * selectorChain = this->readSelectorChain();
    HSSRule * ret = new HSSRule(selectorChain);
    
    //we're not in a selector anymore
    this->currentContext.pop();
    //now we're inside the block
    this->currentContext.push(HSSParserContextBlock);
    
    //if the file ends here, fuuuuuuu[...]
    this->checkForUnexpectedEndOfSource();
    
    //we expect a block to open
    this->skipExpected(HSSBlockOpen);
    //skip any whitespace
    this->skip(HSSWhitespace);
    
    //read the inner part of the block
    while (!this->currentToken->isA(HSSBlockClose))
    {
        //std_log1(this->currentToken->toString());
        //if we find an identifier, we must peek forward to see if it is a property name
        if(this->currentToken->isA(HSSIdentifier)){
            if (this->isPropertyDefinition()){
                ret->propertiesAdd(this->readPropertyDefinition());
            } else {
                //recursive omg!
                std_log1("omg recursiving!");
                this->readRule();
            }
        } else {
            throw HSSUnexpectedTokenException(this->currentToken->type);
        }
        
        security_brake()
    }
    security_brake_reset()
    
    //we're out of the block, read next token
    this->readNextToken();
    //leave the block context
    this->currentContext.pop();
    if(!this->atEndOfSource()){
        //ignore all the whitespace after the block
        this->skip(HSSWhitespace);
    }
    
    return ret;
}

HSSSelectorChain * HSSParser::readSelectorChain()
{
    security_brake_init();
    
    HSSSelectorChain * ret = new HSSSelectorChain();
    
    //first we need to look at the selector chain
    //set the appropriate context
    this->currentContext.push(HSSParserContextSelectorChain);
    //parse the selector chain until we find the block
    while (this->currentToken != NULL && !this->currentToken->isA(HSSBlockOpen)) {
        std_log3(this->currentToken->toString());
        
        //if it's an identifier, it's a simple selector
        if (this->currentToken->isA(HSSIdentifier)){
            ret->add(this->readSelector());
            //adds only if needed
            ret->add(this->readChildrenCombinatorOrSkip());
            
            //a symbol, probably a combinator
        } else if (this->currentToken->isA(HSSSymbol)) {
            const char currentTokenValue = *(VALUE_TOKEN(this->currentToken)->value).c_str();
            switch (currentTokenValue) {
                case '=':
                case '-':
                case '+':
                case '>': //FIXME: special handling for text selection combinators?
                    ret->add(this->readSymbolCombinator());
                    break;
                case '*':
                    ret->add(new HSSUniversalSelector());
                    this->readNextToken();
                    //adds only if needed
                    ret->add(this->readChildrenCombinatorOrSkip());
                    
                    break;
                case '.':
                    //we need to check if it is really a combinator or just a syntax error
                    if(   VALUE_TOKEN(this->currentToken)->value == ".."
                       || VALUE_TOKEN(this->currentToken)->value == "..."  ){
                        ret->add(this->readSymbolCombinator());
                        break;
                    }
                    //huh? we didn't expect any other symbol
                default:
                    throw HSSUnexpectedTokenException(HSSSymbol, string(1,currentTokenValue));
                    delete ret;
                    return NULL;
            }
            //we didn't expect any other type of token
        } else {
            throw HSSUnexpectedTokenException(this->currentToken->type);
            delete ret;
            return NULL;
        }
        
        security_brake();
    }
    security_brake_reset();
    
    return ret;
}

bool HSSParser::isCombinator()
{
    return this->isCombinator(this->currentToken);
}

bool HSSParser::isCombinator(HSSToken * token)
{
    //are we in a context that accepts combinators?
    HSSParserContext context = this->currentContext.top();
    if (context == HSSParserContextExpression) {
        return false;
    }
    //all combinators are symbols
    if(token->isA(HSSSymbol) ){
        const char currentTokenChar = *(static_cast<HSSValueToken*>(token)->value).c_str();
        switch (currentTokenChar) {
            case '=':
            case '-':
            case '+':
            case '.':
            case '>':
                return true;
                //fixme
                
            default:
                return false;
        }
    } else if ( token->isA(HSSWhitespace) ){
        return this->isChildrenCombinator();
    }
    return false;
}

//this function assumes currentToken to be whitespace
bool HSSParser::isChildrenCombinator()
{
    std_log4("----- peeking ------ ");
    //if the next token is anything other than a combinator or an open block the whitespace means children combinator
    HSSToken * peekToken = this->tokenizer->peekNextToken();
    std_log4(peekToken->toString());
    bool ret = !this->isCombinator(peekToken) && !peekToken->isA(HSSBlockOpen);
    delete peekToken;
    this->tokenizer->resetPeek();
    std_log4("----- finished peeking ------ ");
    return ret;
}

//this function assumes currentToken to be an identifier
bool HSSParser::isPropertyDefinition()
{
    std_log4("----- peeking ------ ");
    //if the next token is a colon, BINGO!
    HSSToken * peekToken = this->tokenizer->peekNextToken();
    std_log4(peekToken->toString());
    bool ret = peekToken->isA(HSSColon);
    delete peekToken;
    this->tokenizer->resetPeek();
    std_log4("----- finished peeking ------ ");
    return ret;
}

HSSCombinator * HSSParser::readChildrenCombinatorOrSkip()
{
    //are we dealing with whitespace?
    if(this->currentToken->isA(HSSWhitespace)){
        if(this->isChildrenCombinator()){
            HSSCombinator * newCombinator = new HSSCombinator(HSSChildrenCombinator);
            this->readNextToken();
            return newCombinator;
        } else {
            //alright, ignore it
            this->skip(HSSWhitespace);
            return NULL;
        }
    } else {
        //done, nothing to see here, move along...
        return NULL;
    }
}

//this expects the current token to be a symbol
HSSCombinator * HSSParser::readSymbolCombinator()
{
    //FIXME: check the context
    HSSCombinator * ret;
    const char currentTokenChar = *(VALUE_TOKEN(this->currentToken)->value).c_str();
    switch (currentTokenChar) {
        case '=':
            ret = new HSSCombinator(HSSSiblingsCombinator);
            break;
        case '-':
            ret = new HSSCombinator(HSSPreviousSiblingsCombinator);
            break;
        case '+':
            ret = new HSSCombinator(HSSNextSiblingsCombinator);
            break;
        case '.':
            if(static_cast<HSSValueToken *>(this->currentToken)->value == ".."){
                ret = new HSSCombinator(HSSDescendantsCombinator);
            } else if (static_cast<HSSValueToken *>(this->currentToken)->value == "..."){
                ret = new HSSCombinator(HSSAllDescendantsCombinator);
            }
            
            break;
        default:
            throw HSSUnexpectedTokenException(HSSSymbol, string(1, currentTokenChar));
            return NULL;
    }
    
    this->readNextToken();
    this->skip(HSSWhitespace);
    return ret;
}

//this assumes that the currentToken is an identifier
HSSSelector * HSSParser::readSelector()
{
    HSSSelector * ret = new HSSSelector(VALUE_TOKEN(this->currentToken)->value);
    this->readNextToken();
    return ret;
}


//this assumes currentToken is an object sign
HSSObjectDefinition * HSSParser::readObjectDefinition()
{
    HSSObjectDefinition * ret;
    string objtype;
    HSSObject * obj;
    
    //yeah, yeah, we know the @ already
    this->skipExpected(HSSObjectSign);
    //end of file would be fatal
    this->checkForUnexpectedEndOfSource();
    
    //store the current context for later use
    HSSParserContext outerContext = this->currentContext.top();
    //set the appropriate context
    this->currentContext.push(HSSParserContextObjectDefinition);
    
    //first we need to know what type of object it is
    if (this->currentToken->isA(HSSWhitespace)) {
        //damn, we'll have to derive that from the context
        if (outerContext == HSSParserContextRoot){
            objtype = "container";
        } else {
            //FIXME
            std_log1("deriving object types from context is only supported in root context yet");
            objtype = "object";
        }
    } else if(this->currentToken->isA(HSSIdentifier)){
        //alright, we've got a type, look it up
        objtype = VALUE_TOKEN(this->currentToken)->value;
        this->readNextToken();
        this->checkForUnexpectedEndOfSource();
    } else {
        throw HSSUnexpectedTokenException(this->currentToken->type);
        return NULL;
    }
    
    //try to create an object of that type
    try {
        obj = HSSObject::newObjectWithType(objtype);
    } catch (HSSUnknownObjectTypeException e) {
        throw HSSUnexpectedObjectTypeException(e.type);
    }
    
    
    //get the name of the object
    if (this->currentToken->isA(HSSWhitespace)) {
        this->skip(HSSWhitespace);
    }
    if (this->currentToken->isA(HSSIdentifier)) {
        obj->setName(VALUE_TOKEN(this->currentToken)->value);
        this->readNextToken();
    } else if (this->currentToken->isA(HSSBlockOpen)){
        //it is the opening curly brace, therefore an annonymous object:
        //do nothing
    } else {
        throw HSSUnexpectedTokenException(this->currentToken->type);
        delete obj;
        return NULL;
    }
    
    ret = new HSSObjectDefinition(obj);
    this->skip(HSSWhitespace);
    this->skipExpected(HSSBlockOpen);
    this->skip(HSSWhitespace);
    
    //now we're inside the block
    this->currentContext.push(HSSParserContextBlock);
    
    //if the file ends here, fuuuuuuu[...]
    this->checkForUnexpectedEndOfSource();
    
    //read the inner part of the block
    while (!this->currentToken->isA(HSSBlockClose)){
        ret->propertiesAdd(this->readPropertyDefinition());
    }
    
    //we're out of the block, we expect a closing brace
    this->skipExpected(HSSBlockClose);
    //leave the block context
    this->currentContext.pop();
    //leave the object definition context
    this->currentContext.pop();
    if(!this->atEndOfSource()){
        //ignore all the whitespace after the block
        this->skip(HSSWhitespace);
    }
    
    return ret;
}

HSSPropertyDefinition * HSSParser::readPropertyDefinition()
{
    //end of source is no good
    this->checkForUnexpectedEndOfSource();
    
    HSSPropertyDefinition * ret;
    
    if (this->currentToken->isA(HSSIdentifier)){
        //FIXME: do something with the property here
        ret = new HSSPropertyDefinition(VALUE_TOKEN(this->currentToken)->value);
        this->readNextToken();
        //now must come a colon
        this->skipExpected(HSSColon);
        //we don't give a f$%# about whitespace
        this->skip(HSSWhitespace);
        //now comes either an object definition, a literal value or an expression
        //object
        if (this->currentToken->isA(HSSObjectSign)){
            ret->setValue(this->readObjectDefinition());
            //this->readNextToken();
            
        } else if (this->currentToken->isA(HSSSingleQuoteString) || this->currentToken->isA(HSSDoubleQuoteString)){
            ret->setValue(new HSSObjectDefinition(new HSSValue(VALUE_TOKEN(this->currentToken)->value)));
            this->readNextToken();
            
        //number literal
        } else if (this->currentToken->isA(HSSNumber)){
            //FIXME: parse the number and see if it is an int or a float
            long double num = strtold(VALUE_TOKEN(this->currentToken)->value.c_str(), NULL);            
            ret->setValue(new HSSObjectDefinition(new HSSValue(num)));
            this->readNextToken();
            
        } else if (this->currentToken->isA(HSSPercentageNumber)) {
            this->readNextToken(); //FIXME
            
        } else if (this->currentToken->isA(HSSIdentifier)){
            //FIXME: this is either a keyword or an object name
            std_log1("reading identifier");
            this->readNextToken();
        } else {
            throw HSSUnexpectedTokenException(this->currentToken->type);
            delete ret;
            return NULL;
        }
        
        this->skip(HSSWhitespace);
        //expect a semicolon or the closing brace
        if(this->currentToken->isA(HSSEndOfStatement)){
            this->readNextToken();
            this->skip(HSSWhitespace);
            
        } else if (this->currentToken->isA(HSSBlockClose)){
            //alright, this is the end of the property definition
            std_log1("end of property definition");
        } else {
            throw HSSUnexpectedTokenException(this->currentToken->type);
            delete ret;
            return NULL;
        }
        
        return ret;
    } else {
        throw HSSUnexpectedTokenException(this->currentToken->type);
        this->readNextToken();
        return NULL;
    }
}

void HSSParser::readNextToken()
{
    //delete the old token
    if(this->currentToken != NULL){
        delete this->currentToken;
    }
    //read next one
    this->currentToken = this->tokenizer->readNextToken();
}

bool HSSParser::atEndOfSource()
{
    return this->currentToken == NULL;
}

void HSSParser::checkForUnexpectedEndOfSource()
{
    if (this->atEndOfSource()) {
        throw HSSUnexpectedEndOfSourceException();
    }
}

void HSSParser::skipExpected(HSSTokenType type)
{
    this->checkForUnexpectedEndOfSource();
    if (!this->currentToken->isA(type)) {
        throw HSSExpectedTokenException(type);
    }
    this->readNextToken();
}

void HSSParser::skipExpected(HSSTokenType type, string value)
{
    this->checkForUnexpectedEndOfSource();
    HSSValueToken * currentToken = static_cast<HSSValueToken *>(this->currentToken);
    if (!currentToken->equals(type, value)) {
        throw HSSExpectedTokenException(type, value);
    }
    this->readNextToken();
}

void HSSParser::skip(HSSTokenType type)
{
    if(this->currentToken->isA(type)){
        this->readNextToken();
    }
}